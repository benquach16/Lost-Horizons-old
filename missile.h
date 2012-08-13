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


//missile.h
//code for guided missiles and torpedos

#ifndef _MISSILE_H_
#define _MISSILE_H_

#pragma once

#include "irrlicht.h"
#include "object.h"

using namespace irr;
using namespace core;
class missile
{
public:
	missile(irr::IrrlichtDevice *graphics = 0, core::vector3df &pos = core::vector3df(0,0,0),
		vector3df &rot = vector3df(0,0,0), irr::scene::IAnimatedMeshSceneNode *ship = 0);

	void loop(f32 frameDeltaTime);
	int checkRange();
	void drop();
	int getDamage();
	scene::IAnimatedMeshSceneNode *getTarget()
	{
		return target;
	}
	vector3df getPos()
	{
		return projectile->getPosition();
	}
	int getRange()
	{
		return range;
	}
	int getHealth()
	{
		return health;
	}
	void damage(int damage)
	{
		health -= damage;
	}
	int getDistFromTarget();
	~missile();
private:
	vector3df pos;
	vector3df rot;

	irr::IrrlichtDevice *graphics;
	scene::IAnimatedMeshSceneNode *target;
	scene::IAnimatedMeshSceneNode *projectile;

	//stats
	int base_damage;
	int min_damage;
	int velocity;
	int range;

	int health;
};

#endif