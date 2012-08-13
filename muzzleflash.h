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

#ifndef _MUZZLEFLASH_H_
#define _MUZZLEFLASH_H_

#pragma once

#include "irrlicht.h"
#include "effect.h"

using namespace irr;
using namespace core;
using namespace scene;

class muzzleflash : public CEffect
{
public:
	int end_time;
	scene::ISceneNode *node;
	muzzleflash(irr::IrrlichtDevice *graphics, irrklang::ISoundEngine *sound, core::vector3df &pos, core::vector3df &rot, scene::IBoneSceneNode *parent);
	~muzzleflash();
	void loop();
	void drop();
private:
	irr::IrrlichtDevice *graphics;
	scene::IBoneSceneNode *parent;
};

#endif