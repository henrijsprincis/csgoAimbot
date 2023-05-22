# CSGOAimbot
CSGO aimbot written in C++ (Outdated)

![csgogif](https://github.com/henrijsprincis/csgoAimbot/assets/38922533/4786d330-7a7c-4727-8bb1-4cf81e6cec4a)


## Run Instructions
1. Open CSGo AimBot.sln in visual studio.
2. Click Build.
3. Run CSGO.exe
4. Run Aimbot.exe

## Dos and Donts
1. Do not use this in online multiplayer games as you will be banned.
2. Do update the memory addresses in "CsGo AimBot.cpp" to match with the current version of the game.
3. Enjoy.

Note that the memory addresses have not been updated since April 2020 

## Implementation Overview
1. Get the position of the head bone of nearby players using the player manager.
2. Find the closest player that's on the enemy team.
3. Use pythagoras to set the view angle and aim at their head.
