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


//gameloop.h
//The gameloop object is the primary object running through all of the
//functions inside of the game.
//IT IS AT THE TOP OF THE HIERACHY
//besides the main graphics loop of course
//And it detects the player keypresses and controls most of player run stuff

#ifndef GAMELOOP_H
#define GAMELOOP_H

#pragma once

#include "irrlicht.h"
#include "irrklang.h"


#include "ship.h"
#include "player.h"
#include "hud.h"
#include "turret.h"
#include "projectile.h"
#include "keylistener.h"
#include "planet.h"
#include "gamemanager.h"
#include "fighter.h"
#include "dockmenu.h"
#include "targetmenu.h"
#include "explosion.h"
#include "menu.h"
#include "sun.h"
#include "warptrail.h"
#include "fighter.h"
#include "escapemenu.h"
#include "dialoguebox.h"
#include "dialoguetree.h"
#include "dialoguemanager.h"
#include "sTypes.h"
#include "missionlist.h"
#include "alertbox.h"

#include "missionmanager.h"
#include "shadercallback.h"

#include "vector"



using namespace irr;
using namespace irrklang;
using namespace scene;
using namespace core;
using namespace video;
using namespace io;


//Object
class GameLoop
{
public:
	//mun conference at edison :(

	//Construct gameloop object
	//using video/sound variables given by the starting class
	GameLoop(irr::IrrlichtDevice *graphics,KeyListener *receiver, irrklang::ISoundEngine *sound, bool loadgame);
	~GameLoop();
	
	//Used only if running a new game
	//Mostly temporary until replaced by real scene management system
	void newGame();

	//Opens up an xml save file and loads all the information from it
	void loadGame();


	//just deletes the class
	bool exitGame();



	//Main game function, goes through all the sub objects and the pause menu
	void Run();	


	// function for player input
	void playerControl(KeyListener *receiver, f32 frameDeltaTime,irrklang::ISoundEngine *sound);

	// detects camera input using mouse
	void cameraControl(KeyListener *receiver, f32 frameDeltaTime);


	//basic functions needed by the class
	void setTurnCircleGUI(bool is_visible);
	void turnOnMenu(bool on);
	float charToFloat(wchar_t text);

	//delete everything in the game
	//and clear it out
	void drop();

private:

	//Important variables
	//Stores the main graphics renderer
	irr::IrrlichtDevice *graphics;

	//Stores soundengine
	irrklang::ISoundEngine *sound;

	//Stores key listeneer
	KeyListener *receiver;

	//Variables used for frame independant movement
	f32 then;
	f32 frameDeltaTime;
	int test_timer;		//test

	std::vector<scene::IBillboardSceneNode*> dust_manager;

	//variable for use by the player
	bool GamePaused;
	bool GameExit;
	bool fired;
	bool isHudVisible;
	bool Warp;
	bool LaunchFighters;
	bool warptrail_created;
	bool CommandView;
	bool NavView;
	float velocity;

	//game object variables
	CShip *player_target;
	CMission *current_mission;
	planet *jump_target;
	CShip *docked_station;

	sun *tau_ceti_star;
	//even more
	CAlertBox *alertBox;
	CTargetMenu *targetMenu;
	CEscapeMenu *escapeMenu;
	Player *CPlayer;
	Hud *CHud;
	gameManager *Manager;
	missionManager *missionM;
	dialogueManager *dialogueM;
	CMenu *gameMenu;
	CDockMenu *dockMenu;
	ShaderCallBack *callback;				//to be used later

	CMission *tut;
	bool display_tut;
	//lots of variables
	irr::scene::ICameraSceneNode *pCam;		//camera object
	gui::IGUIImage *player_nav;				//used in the nav view
	irr::scene::IMeshSceneNode *ship;		//legacy delete later
	irr::scene::ISceneNode *XYcircle;
	irr::scene::ISceneNode *ZYcircle;
	irr::scene::IMeshSceneNode *TurningArrow;

	float camera_sway;						//legacy
	vector3df camera_old_target;			//legacy

};


#endif


