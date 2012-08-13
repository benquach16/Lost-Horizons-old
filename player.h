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

//player.h
//manages the player class
//nowhere near threadsafe


#ifndef _PLAYER_H
#define _PLAYER_H

#pragma once

#include "irrlicht.h"
#include "ship.h"
#include "keylistener.h"
#include "turret.h"
#include "projectile.h"
#include "sTypes.h"
#include "item.h"
#include "fighter.h"
#include "shadercallback.h"
#include "alertbox.h"

#include "vector"
#include "cstring"
#include "sstream"
#include "iostream"

using namespace irr;
using namespace core;
using namespace scene;
using namespace io;


//main player code
class Player : public CObject
{
public:
	ShaderCallBack *callback;			//adjust 


	irr::scene::IAnimatedMeshSceneNode *ship;



	io::IAttributes *ship_attributes;	//legacy

	core::vector3df pRot;
	core::vector3df pPos;
	core::vector3df rotSlow;
	core::vector3df posSlow;



	scene::IBoneSceneNode *exhaust_01;
	scene::IBoneSceneNode *exhaust_02;
	scene::IBoneSceneNode *exhaust_03;


	//arrays to store turrets
	std::vector<turret*> turret_manager;
	std::vector<turret*> secondary_turret_manager;
	std::vector<turret*> light_turret_manager;
	std::vector<turret*> heavy_turret_manager;

	std::vector<fighter*> fighter_manager;		//legacy

	//direction ship wants to turn
	struct turningCircle
	{
		float X;
		float Y;
		float Z;
	};
	//cannon timer
	//legacy
	struct cannonStats
	{
		int primary_time;
		bool primary;
		int secondary_time;
		bool secondary;
		int light_time;
		bool light;
		int flak_time;
	};
	//crew
	struct crewStats
	{
		int morale;
		int num;
		int maxnum;
		int experience;
		int maxexperience;
	};
	//11 subsystems
	struct subsystemStats
	{
		int deck1;
		int deck2;
		int elevator;
		int bridge;
		int engine;
		int warpdrive;
		int primaryweapons;
		int secondaryweapons;
		int lightweapons;
		int shield;
		int power;
	};


	struct turningCircle turn;
	struct cannonStats cannonFired;
	struct crewStats crew;
	struct subsystemStats subsystem;



	Player(irr::IrrlichtDevice *graphics = (irr::IrrlichtDevice*)0,
		irrklang::ISoundEngine *sound = (irrklang::ISoundEngine*)0,
		CAlertBox *alertBox=(CAlertBox*)0,
		KeyListener *receiver=(KeyListener*)0,
		const core::vector3df& ship_position = vector3df(0,0,0),
		ship_base *s_class = ships().TERR_PRAETORIAN_CRUISER,
		ShaderCallBack *callback = 0);


	virtual ~Player();
	void drop();


	void playerRun(f32 frameDeltaTime);
	void manageTurrets(CShip* player_target, f32 frameDeltaTime);
	void shoot();
	void shootPD();
	void aimPd(vector3df &point,f32 frameDeltaTime);
	void damagePlayer(int damage);
	
	
	core::vector3df aimTurrets(core::vector3df turret_pos, CShip* player_target);			//turret aim code
	core::vector3df getRot() const;
	core::vector3df getPos() const;
	void setPos(const core::vector3df& pPos);
	void setRot(const core::vector3df& pRot);
	void setVelocity(float velocity);
	void setDocked(bool state);
	void applyForce(vector3df &rotation, int amount, f32 frameDeltaTime);

	float getDistToTarget(CShip* player_target);

	//return funcs
	float getVelocity();
	int getMaxSpeed();
	float getMaxTurn();
	int getHull() const;
	int getMaxHull();
	int getArmor() const;
	int getMaxArmor();
	int getShield() const;
	int getMoney() const;
	void repairHull();
	void repairArmor();
	void setMoney(int newamount);
	bool getDocked() const;
	ship_base *getCurrentShip() const;
	void resetCannons();
	void resetPD();

	//cargo funcs
	std::vector<item*> getCargo();
	void clearCargo();
	void addItemToCargo(item* newitem);
	void removeItemFromCargo(item *itemtodelete);

	//loadout funcs
	turret_base* getPrimaryTurret();
	void setPrimaryTurret(turret_base *type);
	int getNumPrimaryWeaponsInCargo();
	turret_base* getSecondaryTurret();
	void setSecondaryTurret(turret_base *type);
	int getNumSecondaryWeaponsInCargo();
	turret_base *getLightTurret();
	void setLightTurret(turret_base *type);
	int getNumLightWeaponsInCargo();

	//will be annoying in implement
	void setShipType(ship_base *type);

	void setFighterCount(int count);
	void addFighterCount(int count);
	int getMaxFighters();
	int getNumFighters();
	fighter_base *getFighterType();
	void setFighterType(fighter_base *fighter_type);
	int getFighterCreationTime();
	int getFighterLaunchTime();
	void setFighterLaunchTime(int time);
	void setFighterCreationTime(int time);

	void saveObject(io::IXMLWriter *writer);
	void saveCargo(io::IXMLWriter *writer);
	void loadObject(io::IXMLReader *reader);
	void loadCargo(io::IXMLReader *reader, int numobjects);

private:
	int numFighters;
	int maxFighters;
	int fighter_creation_time;
	int fighter_launch_time;

	irr::IrrlichtDevice *graphics;
	irrklang::ISoundEngine *sound;

	irrklang::ISound *engineSound;
	irrklang::ISound *shipHum;

	CAlertBox *alertBox;

	int money;

	ship_base *s_class;
	fighter_base *f_class;

	bool pd_aim;

	int last_time;
	int hullPoints;
	int armorPoints;
	int shieldPoints;

	bool shipDamaged;

	int maxHullPoints;
	int maxArmorPoints;
	int maxShieldPoints;
	
	int maxVelocity;
	float velocity;
	float maxTurn;

	int energy;
	int maxEnergy;

	bool docked;

	std::vector<item*> cargo_manager;

	
	//turrets
	turret_base *primary_turret;
	turret_base *secondary_turret;
	turret_base *heavy_turret;
	turret_base *light_turret;

	stringw floatToString(float num);
	stringw intToString(int num);

	scene::IBillboardSceneNode *corona;
	scene::IBillboardSceneNode *corona2;
	scene::IBillboardSceneNode *corona3;

};




#endif