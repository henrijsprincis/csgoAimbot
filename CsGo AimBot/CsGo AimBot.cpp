// ReadingProcessMemory.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include<Windows.h>
#include<Psapi.h>
#include<stdio.h>//Has enum process modules
#include<tchar.h>//has tchar

#include <chrono>//for sleeping
#include <thread>
#include <cmath>

//m_dwBoneMatrix: __________________ 0x26A8
HWND hwnd;
DWORD procID;
HANDLE handle;

char client_panorama_name[40] = "client_panorama.dll";
char engine_name[40] = "engine.dll";
//force  attack = client_panorama.dll + dw_force_attack
//formula for view angle "engine.dll + dwCLientState" point to. Then add view angle offset
int local_player_offset = 13818756;//hex: (0xD29B1C) => 0xD2DB84
int jump_offset = 260;//104 in hex
int dwForceJump = 85855904;//0x51E0EA0 in hex
int entityList = 81009140;//0x4D3D67C => 0x4D419F4
int coordinate_offset = 160; // A0 in hex
int setViewAngle = 19848;//set view angles (0x4D88)
int dwClientState = 0x589DCC;//dwClientState
int iTeamNum = 0xF4;
int iHealth = 256;
int dwBoneMatrix = 9896;//
int first_bone_offset = 12;
int head_bone_offset = 48 * 8;

//user derived stuff
int client_panorama;
int engine_dll;
int local_player_from_all_address;
int local_player_from_all_address_points_to;
int dwClientState_value;
int local_player_address;
int local_player_points_to;


//arrays
float x_coord = 0;
float y_coord = 0;
float z_coord = 0;
float x_coords[40] = {};
float y_coords[40] = {};
float z_coords[40] = {};
int team_values[40] = {};
int hp_values[40] = {};
//local player
float localx;
float localy;
float localz;
int localt;

int find_client_dll(char module_name[40]) {
	wchar_t local_module_name[40];
	for (int i = 0; i < 40; i++) {
		wchar_t a = module_name[i];
		local_module_name[i] = a;
	}

	int client_panorama = 0;
	HMODULE hMods[1024];//will contain the list of modules
	DWORD cbNeeded;//no clue what this means
	//std::cout << "HANDLE: " << handle << std::endl;
	if (EnumProcessModules(handle, hMods, sizeof(hMods), &cbNeeded)) {//HIDE ME
		for (int i = 0; i < (cbNeeded / sizeof(HMODULE)); i++) {

			TCHAR szModName[MAX_PATH];

			// Get the full path to the module's file.

			if (GetModuleFileNameEx(handle, hMods[i], szModName,
				sizeof(szModName) / sizeof(TCHAR)))
			{
				// Print the module name and handle value.
				if (wcsstr(szModName, local_module_name)) {
					//std::wcout << szModName << std::endl;
					//std::cout << hMods[i] << std::endl;
					client_panorama = (int)hMods[i];
					return client_panorama;
					//std::cout << client_panorama<<std::endl;
				}
			}
		}

	}
	return client_panorama;
}
void initilize_globals() {
	hwnd = FindWindowA(NULL, "Counter-Strike: Global Offensive");//Find cs:go
	if (hwnd == NULL) {
		std::cout << "Error: Didn't find csgo window";
		exit(-1);
	}
	GetWindowThreadProcessId(hwnd, &procID);
	if (procID == NULL) {
		std::cout << "Error: Didn't find process ID";
		exit(-1);
	}
	handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, procID);
	//set derived stuff
	client_panorama = find_client_dll(client_panorama_name);
	engine_dll = find_client_dll(engine_name);
	std::cout << "Client panorama.dll: " << client_panorama << std::endl;
	local_player_from_all_address = client_panorama + entityList;
	ReadProcessMemory(handle, (PBYTE*)local_player_from_all_address, &local_player_from_all_address_points_to, sizeof(int), 0);
}
int update_positions(float x_coords[40], float y_coords[40], float z_coords[40], int team_values[40], int hp_values[40]) {//also returns the number of players
	float x_coord = 0;
	float y_coord = 0;
	float z_coord = 0;

	int team_value = -1;
	int hp_value = -1;
	bool allow_skip = true;


	//update local player position
	local_player_address = client_panorama + local_player_offset;
	//std::cout << "local player address: " << local_player_address << std::endl;
	ReadProcessMemory(handle, (PBYTE*)local_player_address, &local_player_points_to, sizeof(int), 0);
	ReadProcessMemory(handle, (PBYTE*)(local_player_points_to + iTeamNum), &localt, sizeof(int), 0);
	ReadProcessMemory(handle, (PBYTE*)(local_player_points_to + coordinate_offset), &localx, sizeof(int), 0);
	ReadProcessMemory(handle, (PBYTE*)(local_player_points_to + coordinate_offset + 4), &localy, sizeof(int), 0);
	ReadProcessMemory(handle, (PBYTE*)(local_player_points_to + coordinate_offset + 8), &localz, sizeof(int), 0);
	

	for (int i = 0; i < 40; i++) {
		ReadProcessMemory(handle, (PBYTE*)(local_player_from_all_address + (0x10 * i)), &local_player_from_all_address_points_to, sizeof(int), 0);
		/*
		if (local_player_from_all_address_points_to == 0) {
			//std::cout << "We've reached the end!" << std::endl;
			if (allow_skip) allow_skip = false;
			else{
			return i;
			break;
			}
		}*/
		int bone_matrix_base_address = local_player_from_all_address_points_to + dwBoneMatrix;//player base + dwbone
		int bone_matrix_value;//get the val it points to
		float head_bone;
		ReadProcessMemory(handle, (PBYTE*)(bone_matrix_base_address), &bone_matrix_value, sizeof(int), 0);
		ReadProcessMemory(handle, (PBYTE*)(bone_matrix_value + head_bone_offset + first_bone_offset), &head_bone, sizeof(int), 0);
		//std::cout << "Head bone: " << head_bone;


		ReadProcessMemory(handle, (PBYTE*)(local_player_from_all_address_points_to + iTeamNum), &team_value, sizeof(int), 0);
		team_values[i] = team_value;
		ReadProcessMemory(handle, (PBYTE*)(local_player_from_all_address_points_to + iHealth), &hp_value, sizeof(int), 0);
		hp_values[i] = hp_value;
		
		
		//ReadProcessMemory(handle, (PBYTE*)(local_player_from_all_address_points_to + coordinate_offset), &x_coord, sizeof(int), 0);//64 is health offset (position)
		ReadProcessMemory(handle, (PBYTE*)(bone_matrix_value + head_bone_offset + first_bone_offset), &x_coord, sizeof(int), 0);//headbone
		x_coords[i] = x_coord;
		//ReadProcessMemory(handle, (PBYTE*)(local_player_from_all_address_points_to + coordinate_offset + 4), &y_coord, sizeof(int), 0);
		ReadProcessMemory(handle, (PBYTE*)(bone_matrix_value + head_bone_offset + first_bone_offset + 16), &y_coord, sizeof(int), 0);//headbone
		y_coords[i] = y_coord;
		//ReadProcessMemory(handle, (PBYTE*)(local_player_from_all_address_points_to + coordinate_offset + 8), &z_coord, sizeof(int), 0);
		ReadProcessMemory(handle, (PBYTE*)(bone_matrix_value + head_bone_offset + first_bone_offset + 32), &z_coord, sizeof(int), 0);//headbone
		z_coords[i] = z_coord-64;
	}
	return 39;


}
int find_closest_player(int nr_of_players) {
	int return_me = -1;
	double current_closest_distance = 10000000;
	double abs_closest = 10000000;
	int player_to_aim_at = -1;
	for (int i = 0; i < nr_of_players; i++) {
		current_closest_distance = (localx - x_coords[i]) * (localx - x_coords[i]) + (localy - y_coords[i]) * (localy- y_coords[i]) + (localz - z_coords[i]) * (localz - z_coords[i]);
		//std::cout << current_closest_distance << " : distance" << std::endl;
		if (current_closest_distance < abs_closest && localt != team_values[i] && hp_values[i] != 0) {
			abs_closest = current_closest_distance;
			//std::cout << std::endl << "return me:" << i << std::endl;
			return_me = i;
		}
	}
	//std::cout << " return me " << return_me;
	return return_me;

}
float* calculate_view_angle(int closest) {
	float static return_me[2];
	return_me[1] = 0;
	float player_x = localx;//x_coords[0];
	float player_y = localy;//y_coords[0];
	float player_z = localz;//z_coords[0];

	float other_player_x = x_coords[closest];
	float other_player_y = y_coords[closest];
	float other_player_z = z_coords[closest];
	
	//std::cout << player_x << std::endl;
	//std::cout << player_y << std::endl;
	//std::cout << player_z << std::endl;

	//std::cout << other_player_x << std::endl;
	//std::cout << other_player_y << std::endl;
	//std::cout << other_player_z << std::endl;

	//horizontal is 1
	float opp = 10;
	float adj = 10;
	float hyp = 10;
	float y = 0;
	opp = player_y - other_player_y;
	adj = player_x - other_player_x;//if horizontal angle is 0, then only x changes when you move along it
	if(player_x > other_player_x && player_y > other_player_y){
		return_me[1] = atan(opp / adj) * 180 / 3.1415 - 180;
	}
	if (other_player_x > player_x&& player_y > other_player_y) {
		return_me[1] = atan(opp / adj) * 180 / 3.1415;
	}
	if (player_x > other_player_x&& other_player_y > player_y) {
		return_me[1] = atan(opp / adj) * 180 / 3.1415 + 180;
	}
	if (other_player_x > player_x&& other_player_y > player_y) {
		return_me[1] = atan(opp / adj) * 180 / 3.1415;
	}
	//adj = player_y - other_player_y;

	opp = player_z - other_player_z;//(player_z - other_player_z) * (player_z - other_player_z)
	hyp = sqrt(pow(player_x - other_player_x, 2) + (pow(player_y - other_player_y, 2) + pow(player_z - other_player_z,2)));
	

	y = asin(opp / hyp) * 180/ 3.1415;

	//std::cout << "y: " << y << std::endl;
	return_me[0] = y;
	return return_me;
}
void aimbot_loop() {
	int nr_of_players = update_positions(x_coords, y_coords, z_coords, team_values, hp_values);
	int closest = find_closest_player(nr_of_players);
	if (closest != -1) {
		float* aim_at = calculate_view_angle(closest);
		ReadProcessMemory(handle, (PBYTE*)(engine_dll + dwClientState), &dwClientState_value, sizeof(int), 0);
		float horizontal_angle;
		float vertical_angle;
		ReadProcessMemory(handle, (PBYTE*)(dwClientState_value + setViewAngle), &vertical_angle, sizeof(float), 0);
		ReadProcessMemory(handle, (PBYTE*)(dwClientState_value + setViewAngle + 4), &horizontal_angle, sizeof(float), 0);
		WriteProcessMemory(handle, (PBYTE*)(dwClientState_value + setViewAngle), &aim_at[0], sizeof(float), 0);
		WriteProcessMemory(handle, (PBYTE*)(dwClientState_value + setViewAngle + 4), &aim_at[1], sizeof(float), 0);
	}

}
int main()
{
	initilize_globals();
	//Aimbot bit
	while(true){
		if (GetKeyState('E')) {//*Check if high-order bit is not set (1 << 15)*/
			aimbot_loop();
		}
		if ((GetKeyState('X') & 0x8000)) {
				exit(1);
				break;
			}
		Sleep(20);

	}
}
	

//deal with this later
/*
int local_player_address = client_panorama + local_player_offset;
int local_player_points_to;
ReadProcessMemory(handle, (PBYTE*)local_player_address, &local_player_points_to, sizeof(int), 0);
int jumping = local_player_points_to + jump_offset;
int in_air;
ReadProcessMemory(handle, (PBYTE*)jumping, &in_air, sizeof(int), 0);
*/
//WriteProcessMemory(handle, (LPVOID)0x00ABEA6C, &newValue, sizeof(newValue), 0);
//ReadProcessMemory(handle, (PBYTE*)0x07816CA4, &readTest, sizeof(readTest), 0);
//std::cout << readTest << std::endl;


// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
