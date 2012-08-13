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


//gamemanager.h
//manages the game objects
//aka the name
//gamemanager

#ifndef _GAMEMANAGER_H_
#define _GAMEMANAGER_H_

#pragma once

#include "irrlicht.h"
#include "irrklang.h"
#include "projectile.h"
#include "missile.h"
#include "ship.h"
#include "planet.h"
#include "keylistener.h"
#include "explosion.h"
#include "smallexplosion.h"
#include "impact.h"
#include "muzzleflash.h"
#include "areaofeffect.h"
#include "effect.h"
#include "player.h"
#include "cargo.h"
#include "vector"
#include "stack"
#include "shadercallback.h"
#include "string"
#include "sTypes.h"
#include "fighter.h"
#include "alertbox.h"
#include "stdlib.h"

using namespace irr;
using namespace core;
using namespace video;
using namespace gui;


//class code
//variables, func decs
class gameManager
{
public:
	int think_time;

	//vectors, how could i live without you
	//these contain most of the objects in the game
	//are meant to be run through individually so everything gets updated
	std::vector<projectile*> projectile_manager;
	std::vector<projectile*> pd_manager;
	std::vector<missile*> missile_manager;
	std::vector<CShip*> ship_manager;
	std::vector<fighter*> fighter_manager;
	std::vector<CShip*> station_manager;
	std::vector<planet*> planet_manager;
	std::vector<cargo*> cargo_manager;
	std::vector<CEffect*> effect_manager;
	std::vector<areaofeffect*> aoe_manager;
	

	bool playerDead;

	//primary constructor function
	gameManager(irr::IrrlichtDevice *graphics, KeyListener* receiver, irrklang::ISoundEngine *sound);
	~gameManager();
	void drop();
	void gameManagerLoop(f32 frameDeltaTime, irrklang::ISoundEngine *sound, Player* CPlayer, CAlertBox *alertBox, ShaderCallBack *callback);
	
	//add a projectile to the projectile vector
	void addProjectile(projectile* shot);
	void addMissile(missile *m);
	//
	void createProjectile();
	//add a created ship to the shipmanager vector
	void addShip(CShip* ship);
	void addFighter(fighter* f);
	void createShip();
	//legacy real soon
	void addStation(CShip* station);
	void addPlanet(planet* p);
	//station management loop
	void stationLoop(f32 frameDeltaTime, irrklang::ISoundEngine *sound,CShip *station, ShaderCallBack *callback);

	//loop to update projectiles
	void projectileAnimationManager(f32 frameDeltaTime, irrklang::ISoundEngine *sound, Player *CPlayer);
	void pdAnimationManager(f32 frameDeltaTime, irrklang::ISoundEngine *sound);
	void missileAnimationManager(f32 frameDeltaTime, irrklang::ISoundEngine *sound, Player *CPlayer);
	void aoeManager(Player *CPlayer);
	//loop to update other stuff
	void fighterManager(f32 frameDeltaTime, irrklang::ISoundEngine *sound, Player *CPlayer);
	void cargoManager(f32 frameDeltaTime, Player *CPlayer, CAlertBox *alertBox);
	void planetAnimationManager(f32 frameDeltaTime);
	void effectAnimationManager();
	void playerShoot(irr::IrrlichtDevice *graphics,Player *CPlayer, irrklang::ISoundEngine *sound, CShip* player_target, f32 frameDeltaTime, int subsys);
	void destroyObjects(Player *CPlayer);
	CShip* getClosestStation(Player *CPlayer);


	void makeAllObjectsVisible(bool visibility);
	void showShipTargets(bool ishidden);
	void showPlanetTargets(bool ishiden);

	planet* selectPlanet();

	//ai funcs
	//used to get distances and ships for ai
	CShip* getClosestShip(CShip* ship);
	CShip* getClosestAlliedShip(CShip* ship);
	CShip* getClosestEnemyShip(CShip* ship);
	CShip* getClosestEnemyShipFighter(fighter* f);
	CShip* getClosestAlliedStation(CShip* ship);
	CShip* getClosestEnemyStation(CShip* ship);
	CShip* getClosestNeutralStation(CShip* ship);
	planet* getClosestEnemyPlanet(CShip* ship);
	planet* getClosestAlliedPlanet(CShip* ship);
	fighter* getClosestEnemyFighter(fighter* f);
	fighter* getClosestEnemyFighterShip(CShip* ship);
	core::vector3df aimAtPlayer(Player *CPlayer);
	void searchForTarget(CShip* ship,Player *CPlayer, f32 frameDeltaTime);
	void engageCurrentTarget(CShip* ship, Player *CPlayer, f32 frameDeltaTime);
	//end this block
	

	CShip* getTarget();
	planet* getTargetPlanet();

	void shootCurrentTarget();


	//save all the objects and stuff
	stringw floatToString(float num);
	stringw intToString(int num);
	void saveObjects(io::IXMLWriter *writer);
	void loadShips(io::IXMLReader *reader, int numobjects, ShaderCallBack *callback);
	void loadFighters(io::IXMLReader *reader, int numobjects);
	void loadCargo(io::IXMLReader *reader, int numobjects);

private:

	irr::IrrlichtDevice *graphics;
	KeyListener* receiver;
	irrklang::ISoundEngine *sound;

	bool showShips;
	bool showPlanets;

	int pd_think_time;
};

#endif