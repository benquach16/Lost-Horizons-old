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

//effect.h
//base class
//used to make effects easy to animate and manage

#ifndef _EFFECT_H_
#define _EFFECT_H_

#pragma once

#include "irrlicht.h"
#include "irrklang.h"

using namespace irr;
using namespace core;

class CEffect
{
public:
	//Initializer
	//Most functions will be replaced by the actual effect functions
	CEffect(irr::IrrlichtDevice *graphics = 0,irrklang::ISoundEngine *sound = 0, core::vector3df& pos = vector3df(0,0,0));
	~CEffect();
	virtual void loop();
	virtual void drop();
	bool getEnd();

	void setEnd(bool value);

private:
	bool end;
	irr::IrrlichtDevice *graphics;
};

#endif