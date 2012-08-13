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
#include "warptrail.h"
//testing for now
warptrail::warptrail(irr::IrrlichtDevice *graphics, irrklang::ISoundEngine *sound, vector3df &pos, vector3df &rot) : CEffect(graphics,sound,pos)
{
	this->graphics = graphics;
	this->sound = sound;

	model = graphics->getSceneManager()->addSphereSceneNode(300,32,0,-1,pos,rot);
	model->getMesh()->setMaterialFlag(video::EMF_BACK_FACE_CULLING,false);
	
}

void warptrail::loop(vector3df &pos)
{
	model->setPosition(pos);
}

void warptrail::drop()
{
	delete this;
}

warptrail::~warptrail()
{
	model->setVisible(false);
	model->remove();
}