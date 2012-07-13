//////////////////////////////////////////////////////////////////////
//2011 Ben Quach
//
//This file is part of the "Lost Horizons" video game concept demo
//main.cpp
//This is the main starting point of the application
//use this file to create startup things such as 
//shortcut commands or splash screens
//or just basic setup in general
//////////////////////////////////////////////////////////////////////

//--------------------------------------------------------------------
//developer documentation
//here is documentation for developers using this code
//
//
//hierarchy 
//	main -> init
//	init -> main menu
//	init -> gameloop
//	gameloop -> gamemanager
//		gamemanager -> ship		(ingame objects)
//		gamemanager -> fighter
//		gamemanager -> missile
//		gamemanager	-> projectile
//		gamemanager -> cargo
//	gameloop -> hud
//	gameloop -> player/player control
//	gameloop -> menus
//	gameloop -> missionmanager
//		missionmanager -> mission
//			mission -> objective
//
//
//init.cpp contains most of the ingame setups and configurations
//this file is only meant to manage stuff before the initial video driver is set up

#include "stdafx.h"
#include "init.h"

//TODO:
//CREATE A STRUCT THAT CONTAINS VARIABLES THAT ARE MEANT TO BE MODIFIED BY SHORTCUT COMMANDS
//THEN SEND TO CINIT CLASS

//function meant to parse command line shortcuts
//and then modify the options ingame appropriately
//WIP FUNCTION
int commandLineArguments(int argc, char *argv[])
{
	for(int i=1; i<argc; i++)
	{
		//scroll through each command line argument

        if(!strcmp(argv[i], "-windowed"))
        {

			//setup windowed
        }
        if(!strcmp(argv[i], "-fullscreen"))
        {

			//setup fullscreen
        }
	}

	return 0;
}

//starting point of application
int main(int argc, char *argv[])
{
	commandLineArguments(argc,argv);
	//create initializer
	//start fullscreen game
	CInit init;


	return 0;	//returns 0
}

