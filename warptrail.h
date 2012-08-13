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

#ifndef _WARPTRAIL_H_
#define _WARPTRAIL_H_

#pragma once
#include "irrlicht.h"
#include "irrklang.h"
#include "effect.h"

class warptrail : public CEffect
{
public:
	warptrail(irr::IrrlichtDevice *graphics,irrklang::ISoundEngine *sound, vector3df &pos, vector3df &rot);
	void loop(vector3df &pos);
	void drop();
	~warptrail();

private:
	irr::IrrlichtDevice *graphics;
	irrklang::ISoundEngine *sound;
	scene::IMeshSceneNode *model;

	vector3df pos;
	vector3df rot;
};

#endif