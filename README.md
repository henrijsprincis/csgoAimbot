# CSGOAimbot
CSGO aimbot written in C++ (Outdated)

![image](https://github.com/henrijsprincis/csgoAimbot/assets/38922533/c5644952-3dcc-4bc4-8a44-394706b6e858)

## Run Instructions
1. Open CSGo AimBot.sln in visual studio.
2. Click Build.
3. Run CSGO.exe
4. Run Aimbot.exe

## Dos and Donts
1. Do not use this in online multiplayer games as you will be banned
2. Do update the memory addresses in "CsGo AimBot.cpp" to match with the current version of the game.
3. Enjoy

## Implementation Overview
1. Get the position of the head bone of nearby players using the player manager.
2. Find the closest player that's on the enemy team.
3. Use pythagoras to set the view angle and aim at their head.
