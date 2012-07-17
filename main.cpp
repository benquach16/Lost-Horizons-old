//////////////////////////////////////////////////////////////////////
///Copyright (C) 2011-2012 Benjamin Quach
//
//This file is part of the "Lost Horizons" video game demo
//
//"Lost Horizons" is free software: you can redistribute it and/or modify
//it under the terms of the GNU General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.
//
//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.
//
//You should have received a copy of the GNU General Public License
//along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
///////////////////////////////////////////////////////////////////////


//This is the main starting point of the application
//use this file to create startup things such as 
//shortcut commands or splash screens
//or just basic setup in general

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
//	gameloop -> dialogue manager
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
	commandParams params;
	for(int i=1; i<argc; i++)
	{
		//scroll through each command line argument

        if(!strcmp(argv[i], "-windowed"))
        {
			//setup windowed
			params.cFullscreen = false;
        }
        if(!strcmp(argv[i], "-fullscreen"))
        {

			//setup fullscreen
			params.cFullscreen = true;
        }
		if(!strcmp(argv[i], "-noai"))
		{
			//disable ai
			params.cDisableAI = true;
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

