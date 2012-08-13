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
#include "impact.h"


//setup impact effects
impact::impact(irr::IrrlichtDevice *graphics,irrklang::ISoundEngine *sound, core::vector3df& pos) : CEffect(graphics,sound,pos)
{
	end_time = graphics->getTimer()->getTime()+200;
	setEnd(false);
	this->graphics = graphics;
	node = graphics->getSceneManager()->addCubeSceneNode(1.f);
	node->setPosition(pos);

	scene::IParticleSystemSceneNode *ps = graphics->getSceneManager()->addParticleSystemSceneNode(false,node);
	scene::IParticleEmitter *em = ps->createSphereEmitter(core::vector3df(0,0,0),1,core::vector3df(0,0,0),2,5,
		video::SColor(255,0,0,0),video::SColor(255,255,255,255),500,1000,0,
		core::dimension2df(1,1),
		core::dimension2df(2,2));
	ps->setEmitter(em);
	em->drop();
	ps->setMaterialTexture(0,graphics->getVideoDriver()->getTexture("res/textures/fire.png"));
	ps->setMaterialFlag(video::EMF_LIGHTING,false);
	ps->setMaterialType(video::EMT_TRANSPARENT_VERTEX_ALPHA);
	scene::IParticleAffector *af = ps->createAttractionAffector(node->getPosition(),-250.F);
	ps->addAffector(af);
	af->drop();

	scene::IParticleSystemSceneNode *debris = graphics->getSceneManager()->addParticleSystemSceneNode(false,node);
	scene::IParticleEmitter *deb = debris->createSphereEmitter(vector3df(0,0,0),1,vector3df(0,0,0),40,80,video::SColor(0,255,255,255),video::SColor(0,255,255,255),500,750,0,dimension2df(5,5),dimension2df(20,20));
	debris->setEmitter(deb);
	deb->drop();
	scene::IParticleAffector *explo2 = debris->createAttractionAffector(node->getPosition(),-50.f);
	debris->addAffector(explo2);
	debris->setMaterialTexture(0,graphics->getVideoDriver()->getTexture("res/glow.pcx"));
	debris->setMaterialFlag(video::EMF_LIGHTING,false);
	debris->setMaterialType(video::EMT_TRANSPARENT_VERTEX_ALPHA);



}

impact::~impact()
{
	node->remove();
}



void impact::loop()
{
	if(end_time<graphics->getTimer()->getTime())
	{	
		setEnd(true);
	}
}

void impact::drop()
{
	delete this;
}