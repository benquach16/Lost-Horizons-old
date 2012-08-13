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


#ifndef _CREW_H_
#define _CREW_H_

#include "sTypes.h"
#include "irrlicht.h"

using namespace irr;
using namespace core;

enum CREW_TYPE
{
	CREW_OFFICER=0,
	CREW_ENGINEER=1,
	CREW_MARINE=2,
	CREW_GUNNERY=3,
	CREW_MEDIC=4
};


class crew
{
public:
	
	crew(CREW_TYPE type = CREW_ENGINEER, bool generate_name = true);
	~crew();
private:
	bool officer;

	int health;
	int maxhealth;
	int morale;
	int maxmorale;
	int experience;
	int cost;

};



#endif