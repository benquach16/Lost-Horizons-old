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
#include "areaofeffect.h"

#define AOE_TIME 500

areaofeffect::areaofeffect(irr::IrrlichtDevice *graphics, vector3df &pos, int radius, int maxdamage, int mindamage):
graphics(graphics), pos(pos), radius(radius), maxdamage(maxdamage), mindamage(mindamage)
{
	//initialize variables
	this->time			=		graphics->getTimer()->getTime()+AOE_TIME;	
}

areaofeffect::~areaofeffect()
{
}

void areaofeffect::drop()
{
	delete this;
}