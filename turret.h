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


//turret.h
//turret control code

#ifndef _TURRET_H_
#define _TURRET_H_

#include "irrlicht.h"
#include "projectile.h"
#include "item.h"

#include "vector"



using namespace irr;
using namespace core;
using namespace scene;
using namespace video;

class turret : public item
{
public:
	irr::IrrlichtDevice *graphics;
	irr::scene::IAnimatedMeshSceneNode *t_model;



	turret(irr::IrrlichtDevice *graphics,int turret_number, scene::IBoneSceneNode *bone, core::vector3df& offset, turret_base *t_class, scene::ISceneNode *newparent);
	~turret();

	void aimTurret(core::vector3df& newrot, f32 frameDeltaTime);
	void setPos(core::vector3df& newpos);
	core::vector3df getRot() const;
	core::vector3df getPos() const;
	core::vector3df getOffset();
	core::vector3df getBonePos();
	core::vector3df getFirePos();
	scene::IBoneSceneNode *getBone();

	int getTurretNum() const;
	int getReloadTime();
	int getRotSpeed();

	ITexture *getIcon();

	void drop()
	{

		delete this;
	}

	projectile *createProjectile();

	turret_base *getTurretClass();

private:

	int turret_number;
	int rotate_speed;
	int reload_time;
	turret_base *t_class;
	scene::IBoneSceneNode *bone;
	core::vector3df offset;
	video::ITexture *icon;
};

#endif