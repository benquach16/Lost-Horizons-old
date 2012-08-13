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



//missionlist.h
//load mission text from xmls
//xml text is probably not going to happen within my timeframe though
//and create missions

#ifndef _MISSIONLIST_H_
#define _MISSIONLIST_H_


#include "irrlicht.h"
#include "mission.h"


using namespace irr;
using namespace core;
using namespace io;

class missionCreate
{
public:
	CMission *tut;
	missionCreate(irr::IrrlichtDevice *graphics, int mission) : graphics(graphics)
	{
		//create tutorial mission
		if(mission==1)
		{
			//TODO: READ STRINGS FROM XML
			std::vector<CObjective*> obj;
			std::vector<item*> reward;
			CObjective *first = new CObjective(L"- Aid allied ships in clearing the space of all enemy ships", OBJ_SWEEP, vector3df(-3000,0,2000));
			obj.push_back(first);
			tut = new CMission(L"Tutorial Mission: Getting acquainted with the game", 
				L"Welcome to your first command! Here, you will run through a series of walkthrough missions in order to get acquainted with the game",obj,2000);
			tut->setIndex(1);
		}

		//create basic sweep and clear mission
		//randomly generate objectives?
		if(mission==2)
		{

		}
	}

private:
	io::IXMLReader *reader;
	irr::IrrlichtDevice *graphics;
};



#endif