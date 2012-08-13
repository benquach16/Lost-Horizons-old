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


//areaofeffect.h
//basically an object that damages all ships around it
//area of effect
//mostly just a placeholder
//since its managed by gamemanger

#ifndef _AREAOFEFFECT_H_
#define _AREAOFEFFECT_H_

#include "irrlicht.h"

using namespace irr;
using namespace core;

class areaofeffect
{
public:
	areaofeffect(irr::IrrlichtDevice *graphics, vector3df &pos, int radius, int maxdamage, int mindamage);
	~areaofeffect();
	void drop();
	int getRadius()
	{
		return radius;
	}
	int getMaxDamage()
	{
		return maxdamage;
	}
	int getMinDamage()
	{
		return mindamage;
	}
	int getTime()
	{
		return time;
	}
	vector3df getPos()
	{
		return pos;
	}

private:
	int radius;
	int maxdamage;
	int mindamage;
	int time;	//only lives for a limited amount of time

	irr::IrrlichtDevice *graphics;
	vector3df pos;
};

#endif