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
#include "muzzleflash.h"

//constructor
//save bone parent to constantly update position
muzzleflash::muzzleflash(irr::IrrlichtDevice *graphics, irrklang::ISoundEngine *sound, core::vector3df &pos, core::vector3df& rot, scene::IBoneSceneNode *parent) :
CEffect(graphics,sound,pos), parent(parent)
{
	end_time = graphics->getTimer()->getTime()+1000;
	this->graphics = graphics;
	setEnd(false);

	node = graphics->getSceneManager()->addAnimatedMeshSceneNode(graphics->getSceneManager()->getMesh("res/models/misc/muzz.x"));
	node->setMaterialFlag(video::EMF_LIGHTING,false);
	node->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);
	node->setRotation(rot);
	node->setPosition(pos);
	
	
	/*
	scene::IParticleSystemSceneNode *ps = graphics->getSceneManager()->addParticleSystemSceneNode(false,node);
	scene::IParticleEmitter *em = ps->createSphereEmitter(core::vector3df(0,0,0),1,core::vector3df(0,0,0),10,20,
		video::SColor(255,0,0,0),video::SColor(255,255,255,255),1000,2000,0,
		core::dimension2df(2,2),
		core::dimension2df(2,2));
	ps->setEmitter(em);
	em->drop();
	ps->setMaterialTexture(0,graphics->getVideoDriver()->getTexture("res/textures/fire.png"));
	ps->setMaterialFlag(video::EMF_LIGHTING,false);
	ps->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);
	scene::IParticleAffector *af = ps->createAttractionAffector(node->getPosition(),-50.F);
	ps->addAffector(af);
	af->drop();
	*/
}

muzzleflash::~muzzleflash()
{
	node->remove();
}

void muzzleflash::loop()
{
	node->setPosition(parent->getPosition());
	if(end_time<graphics->getTimer()->getTime())
	{	
		setEnd(true);
	}
}

void muzzleflash::drop()
{
	delete this;
}