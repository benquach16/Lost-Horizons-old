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

#ifndef _PROJECTILE_H_
#define _PROJECTILE_H_

#include "irrlicht.h"
#include "irrklang.h"
#include "math.h"

using namespace irr;
using namespace scene;

class projectile
{
public:

	irr::scene::IAnimatedMeshSceneNode *bullet;
	f32 frameDeltaTime;

	projectile():graphics(0){};
	projectile(irr::IrrlichtDevice *graphics, core::vector3df& pos = core::vector3df(0,0,0), core::vector3df& rot = core::vector3df(0,0,0), irr::scene::IAnimatedMeshSceneNode *ship = 0);
	~projectile();

	void projRun(f32 frameDeltaTime);
	int damage;

	virtual int checkRange();
	virtual void drop();

	core::vector3df getRot()
	{
		return bullet->getRotation();
	}
	IAnimatedMeshSceneNode *getShip()
	{
		return ship;
	}
	int getRange()
	{
		return range;
	}
protected:
	irr::IrrlichtDevice *graphics;
	int velocity;
	core::vector3df pos;
	core::vector3df *rot;
	core::vector3df pPos;
	int range;

	irr::scene::IAnimatedMeshSceneNode *ship;
};

//other projectile classes
//railgun
class railgunShot : public projectile
{
public:
	railgunShot(irr::IrrlichtDevice *graphics, core::vector3df& pos,  core::vector3df& rot,irr::scene::IAnimatedMeshSceneNode *ship);
};

//photon cannon
class photonCannonShot : public projectile
{
public:
	photonCannonShot(irr::IrrlichtDevice *graphics, core::vector3df& pos,  core::vector3df& rot,irr::scene::IAnimatedMeshSceneNode *ship);
};


class antiMatterShot : public projectile
{
public:
	antiMatterShot(irr::IrrlichtDevice *graphics, core::vector3df& pos, core::vector3df& rot, irr::scene::IAnimatedMeshSceneNode *ship);
};

class plasmaShot : public projectile
{
public:
	plasmaShot(irr::IrrlichtDevice *graphics, core::vector3df& pos, core::vector3df& rot, irr::scene::IAnimatedMeshSceneNode *ship);
};

class gatlingBullet : public projectile
{
public:
	gatlingBullet(irr::IrrlichtDevice *graphics, core::vector3df& pos, core::vector3df& rot, irr::scene::IAnimatedMeshSceneNode *ship);
};

#endif