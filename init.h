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


//int.h
//Manages the graphics rendering loop
//main menu
//and config file loading



#ifndef INIT_H
#define INIT_H

#include "irrlicht.h"
#include "irrklang.h"
#include "gameloop.h"
#include "keylistener.h"
#include "mainmenu.h"
#include "optionmenu.h"
#include "fader.h"
#include "screenquad.h"

#include "iostream"
#include "fstream"


using namespace irr;
using namespace std;
using namespace irrklang;
using namespace io;


//used by main.cpp for command line arguments
//implent everything later
struct commandParams
{
	bool cFullscreen;
	bool cDisableAI;
};


class CInit
{
public:

	//Initializer, setup variables and such
	CInit();

	//destructor
	~CInit();

	//Set up the game
	void StartMenu();
	void StartGame();

	//if need to generate new config cause of changed settings
	void createNewVideoConfig(int x,int y, bool windowed, bool vsync, bool quality);
	void readVideoConfig();

	friend class Game;		//legacy
	float time;

	//used to nuke everything and ensure there are no memory leaks
	void drop();


	//important event objects
	irrklang::ISoundEngine *sound;
	KeyListener *receiver;
	irr::IrrlichtDevice *graphics;

	//new cursor
	gui::IGUIImage *cursor;

	
	GameLoop *Game;
	
	bool load;


private:
	//used if exiting game to the main menu
	bool exit;

	//used for special fx only
	CFader *fader;
};

#endif