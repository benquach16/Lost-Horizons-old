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


//fighter.h
//Code for space fighters they zoom around n stuff

#ifndef _FIGHTER_H_
#define _FIGHTER_H_

#pragma once

#include "object.h"
#include "irrlicht.h"
#include "irrklang.h"
#include "ship.h"
#include "sTypes.h"


using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace irrklang;


//only thing these fighters do

class fighter : public CObject
{
public:
	bool alive;
	struct cannonStruct
	{
		int cannon_time;
		bool cannon;
		int missile_time;
		bool missile;
	};
	struct cannonStruct cannon;

	struct turningCircle
	{
		float X;
		float Y;
		float Z;
	};
	struct turningCircle turn;

	fighter(irr::IrrlichtDevice *graphics = 0,
		irrklang::ISoundEngine *sound =0,
		vector3df &pos = vector3df(0,0,0),
		vector3df &rot = vector3df(0,0,0),
		fighter_base *f_type = fighters().TERR_DRAGONFLY,
		ship_faction faction = FACTION_NEUTRAL,
		scene::IAnimatedMeshSceneNode *home_base = 0,
		const wchar_t *name = L"__test_fighter__");

	//AI functions
	void drop();
	void AIRun(f32 frameDeltaTime);
	void shoot();
	void fireMissile();
	void patrol(vector3df point);
	void moveToPoint(vector3df point);
	void moveAwayFromPoint(vector3df &point);
	void setTarget(CShip *target);
	CShip *getTarget();
	void setFighterTarget(fighter *f);
	fighter *getFighterTarget();


	scene::IAnimatedMeshSceneNode *getHomeBase();

	void setHomeBase(scene::IAnimatedMeshSceneNode *newbase);
	bool getReturnHome();
	void setReturnHome(bool value);
	int getFuel();
	
	ship_faction getFighterFaction();
	fighter_base *getFighterClass();
	
	int getVelocity();
	void setVelocity(int velocity);
	int getMaxVelocity();
	int getHullPoints();
	vector3df getPos();
	void setPos(vector3df &newpos);
	vector3df getRot();
	void setRot(vector3df &newrot);

	void damageFighter(int damage);
	~fighter();

	bool getHostileToPlayer();

	scene::IBoneSceneNode *exhaust;
	scene::IAnimatedMeshSceneNode *fighter_model;
private:
	//temporary vectors
	vector3df sRot;
	vector3df sPos;
	vector3df rotSlow;

	scene::IAnimatedMeshSceneNode *home_base;

	gui::IGUIImage *target_array;
	core::vector2d<s32> array_pos;

	irr::IrrlichtDevice *graphics;
	irrklang::ISoundEngine *sound;


	ship_faction faction;
	fighter_base *f_type;

	CShip *target;
	fighter *fighter_target;

	int hullPoints;
	int velocity;
	int maxVelocity;
	int maxTurn;
	int last_move;

	int fuel;

	bool dogfighting;
	bool return_home;

	bool hostile;
};

#endif