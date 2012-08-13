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
#include "projectile.h"

//Projectile Code
//Here we have the 'template' projectile
//All other projectiles are based off its code
//same stats as a railgun, except it has a different, cooler FX
projectile::projectile(irr::IrrlichtDevice *graphics, core::vector3df& pos,  core::vector3df& rot,irr::scene::IAnimatedMeshSceneNode *ship) :
pos(pos), rot(&rot), graphics(graphics), ship(ship)
{
	//setup variables
	//this is just the base projectile, so none of these stats really matter
	this->velocity = 1750;
	this->damage = 50;
	this->range = 6000;
	this->ship = ship;
	//Creates empty scene node for position
	bullet = graphics->getSceneManager()->addAnimatedMeshSceneNode(graphics->getSceneManager()->getMesh("res/models/projectile.x"));
	bullet->setMaterialFlag(video::EMF_LIGHTING,false);
	//bullet->getMesh()->setBoundingBox(core::aabbox3df(-2,-2,-2,2,2,2));
	bullet->setPosition(pos);
	bullet->setScale(core::vector3df(0.3,0.3,0.4));
	bullet->setRotation(rot);
	
	bullet->setMaterialTexture(0,graphics->getVideoDriver()->getTexture("res/textures/railgun.png"));
	bullet->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);
}
projectile::~projectile()
{
	bullet->remove();
}

//Checks the range from the original point of origin to set max range of bullets
int projectile::checkRange()
{
	float z = pos.Z-bullet->getPosition().Z;
	float y = pos.Y-bullet->getPosition().Y;
	float x = pos.X-bullet->getPosition().X;
	int dist = sqrt(x*x+y*y+z*z);
	return dist;
}

void projectile::projRun(f32 frameDeltaTime)
{
	//if still has range left
	//move toward initial rotation
	pPos = bullet->getPosition();
	float i = bullet->getRotation().Y;
	float z = -(bullet->getRotation().X);
	pPos.X=frameDeltaTime*velocity*(sin(i*3.14/180));
	pPos.X+=bullet->getPosition().X;
	pPos.Z=frameDeltaTime*velocity*(cos(i*3.14/180));
	pPos.Z+=bullet->getPosition().Z;
	pPos.Y=frameDeltaTime*velocity*(sin(z*3.14/180));
	pPos.Y+=bullet->getPosition().Y;
	bullet->setPosition(pPos);
}

void projectile::drop()
{
	delete this;
}



//actual projectile
//set its stats to different stats
//i created a different object for each projectile cause i wanted more control over what each projectile looked like
railgunShot::railgunShot(irr::IrrlichtDevice *graphics, core::vector3df& pos,  core::vector3df& rot,irr::scene::IAnimatedMeshSceneNode *ship) : projectile(graphics,pos,rot,ship)
{
	this->damage=17;
	this->range = 5500;
	this->velocity = 1450;
	bullet->setMaterialTexture(0,graphics->getVideoDriver()->getTexture("res/textures/railgun.png"));
	/*
	//determine performance hit of attaching particle effects to projectiles
	scene::IParticleSystemSceneNode *ps = graphics->getSceneManager()->addParticleSystemSceneNode(false,bullet);
	scene::IParticleEmitter *em = ps->createSphereEmitter(core::vector3df(0,0,0),5,core::vector3df(0,0.023,0),50,100,video::SColor(128,128,128,140),video::SColor(0,0,0,10),100,250,0,core::dimension2df(1,1),core::dimension2df(2,2));
	ps->setEmitter(em);
	em->drop();
	ps->setMaterialTexture(0,graphics->getVideoDriver()->getTexture("res/textures/smoke.png"));
	ps->setMaterialFlag(video::EMF_LIGHTING,false);
	ps->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);
	*/
}

photonCannonShot::photonCannonShot(irr::IrrlichtDevice *graphics, core::vector3df& pos,  core::vector3df& rot,irr::scene::IAnimatedMeshSceneNode *ship) : projectile(graphics,pos,rot,ship)
{
	this->damage = 20;
	this->range =  4500;
	this->velocity = 1150;
	bullet->setMaterialTexture(0,graphics->getVideoDriver()->getTexture("res/textures/fire.png"));
}

antiMatterShot::antiMatterShot(irr::IrrlichtDevice *graphics, core::vector3df& pos, core::vector3df& rot, irr::scene::IAnimatedMeshSceneNode *ship) : projectile(graphics,pos,rot,ship)
{
	this->damage = 13;
	this->range = 4000;
	this->velocity = 1600;
	this->bullet->setScale(core::vector3df(0.5,0.5,0.3));
	bullet->setMaterialTexture(0,graphics->getVideoDriver()->getTexture("res/textures/particle.png"));
}


plasmaShot::plasmaShot(irr::IrrlichtDevice *graphics, core::vector3df& pos, core::vector3df& rot, irr::scene::IAnimatedMeshSceneNode *ship) : projectile(graphics,pos,rot,ship)
{
	this->damage = 16;
	this->range =  3200;
	this->velocity = 1450;
	this->bullet->setScale(core::vector3df(0.6,0.6,0.2));
	bullet->setMaterialTexture(0,graphics->getVideoDriver()->getTexture("res/textures/fire.png"));
}

gatlingBullet::gatlingBullet(irr::IrrlichtDevice *graphics, core::vector3df& pos, core::vector3df& rot, irr::scene::IAnimatedMeshSceneNode *ship) : projectile(graphics,pos,rot,ship)
{
	this->damage = 10;
	this->range = 1000;
	this->velocity = 1900;
	this->bullet->setScale(core::vector3df(0.2,0.2,0.2));
	bullet->setMaterialTexture(0,graphics->getVideoDriver()->getTexture("res/textures/fire.png"));
}