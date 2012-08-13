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
#include "sun.h"

//probably should only have two of these in a scene max
//quaesars are cool
sun::sun(IrrlichtDevice *graphics, core::vector3df& position)
{
	model = graphics->getSceneManager()->addSphereSceneNode(10000.F,256,0,-1,position);
	model->setMaterialFlag(video::EMF_LIGHTING,false);
	graphics->getSceneManager()->addLightSceneNode(0,position,video::SColor(255,255,255,255),1000000);


	corona= graphics->getSceneManager()->addBillboardSceneNode(model,core::dimension2d<f32>(100000,100000));
	corona->setMaterialTexture(0,graphics->getVideoDriver()->getTexture("res/particlewhite.bmp"));
	corona->setMaterialFlag(video::EMF_LIGHTING, false);
	corona->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);
	//gives it a cool lens flare
	corona3 = graphics->getSceneManager()->addBillboardSceneNode(model,dimension2d<f32>(130000,110000),vector3df(0,0,0));
	corona3->setMaterialTexture(0,graphics->getVideoDriver()->getTexture("res/textures/engine_corona.png"));
	corona3->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);
}

sun::~sun()
{
	model->drop();
}

void sun::drop()
{
	delete this;
}
