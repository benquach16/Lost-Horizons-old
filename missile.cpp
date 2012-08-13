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

#include "stdafx.h"
#include "missile.h"

missile::missile(irr::IrrlichtDevice *graphics, vector3df &pos, vector3df &rot, irr::scene::IAnimatedMeshSceneNode *ship)
{
	this->graphics = graphics;
	this->rot = rot;
	this->pos = pos;

	this->base_damage = 400;
	this->min_damage = 10;
	this->velocity = 500;
	this->range = 2000;
	this->health= 10;
	this->target = ship;

	projectile = graphics->getSceneManager()->addAnimatedMeshSceneNode(graphics->getSceneManager()->getMesh("res/models/missile.x"));

	projectile->setMaterialFlag(video::EMF_LIGHTING,false);
	//bullet->getMesh()->setBoundingBox(core::aabbox3df(-2,-2,-2,2,2,2));
	projectile->setPosition(pos);
	projectile->setRotation(rot);
	

	scene::IParticleSystemSceneNode *ps = graphics->getSceneManager()->addParticleSystemSceneNode(false,projectile);
	scene::IParticleEmitter *em = ps->createSphereEmitter(core::vector3df(0,0,0),1,core::vector3df(0,0.023,0),100,200,video::SColor(128,128,128,140),video::SColor(0,0,0,10),200,500,0,core::dimension2df(5,5),core::dimension2df(10,10));
	ps->setEmitter(em);
	em->drop();
	ps->setMaterialTexture(0,graphics->getVideoDriver()->getTexture("res/textures/engine_trails.pcx"));
	ps->setMaterialFlag(video::EMF_LIGHTING,false);
	ps->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);
}

missile::~missile()
{
	projectile->remove();
}

void missile::drop()
{
	delete this;
}

//check range function
int missile::checkRange()
{
	float z = pos.Z-projectile->getPosition().Z;
	float y = pos.Y-projectile->getPosition().Y;
	float x = pos.X-projectile->getPosition().X;
	int dist = sqrt(x*x+y*y+z*z);
	return dist;
}

int missile::getDistFromTarget()
{
	float z = target->getPosition().Z-projectile->getPosition().Z;
	float y = target->getPosition().Y-projectile->getPosition().Y;
	float x = target->getPosition().X-projectile->getPosition().X;
	int dist = sqrt(x*x+y*y+z*z);
	return dist;
}

//fly to target
void missile::loop(f32 frameDeltaTime)
{
	//point to target
	vector3df pPos;
	pPos = projectile->getPosition();
	vector3df diff = target->getPosition() - projectile->getPosition();
	vector3df angle = diff.getHorizontalAngle();
	projectile->setRotation(angle);

	//movement code
	float i = projectile->getRotation().Y;
	float z = -(projectile->getRotation().X);
	pPos.X=frameDeltaTime*velocity*(sin(i*3.14/180));
	pPos.X+=projectile->getPosition().X;
	pPos.Z=frameDeltaTime*velocity*(cos(i*3.14/180));
	pPos.Z+=projectile->getPosition().Z;
	pPos.Y=frameDeltaTime*velocity*(sin(z*3.14/180));
	pPos.Y+=projectile->getPosition().Y;
	projectile->setPosition(pPos);
}