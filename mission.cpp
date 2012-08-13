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
#include "mission.h"


CMission::CMission(const wchar_t *title, const wchar_t *description, std::vector<CObjective*> objectives, int cash_reward, std::vector<item*> item_reward) : mission_complete(false)
{

	//set up all the variables
	//store all the mission data from the arguments
	this->mission_title = title;
	this->mission_description = description;
	this->mission_objectives = objectives;
	this->cash_reward = cash_reward;
	this->item_reward = item_reward;
}


//delete all the objectives
//clear the list
CMission::~CMission()
{
	for(int i=0 ;i<mission_objectives.size(); i++)
	{
		mission_objectives[i]->drop();
		mission_objectives.erase(mission_objectives.begin()+i);
	}
	mission_objectives.clear();
}

//
void CMission::drop()
{
	delete this;
}

void CMission::loop(irr::IrrlichtDevice *graphics, Player *CPlayer, bool iscurrentmission, std::vector<CShip*> ship_manager,CAlertBox *alertBox)
{
	//make sure mission actually has objectives
	//if not, complete mission
	if(mission_objectives.size()>>0)
	{
		//only display the mission objective if the mission is current
		if(mission_objectives[0]->loop(graphics, iscurrentmission,CPlayer, ship_manager)==true)
		{
			//completed mission? then delete it
			mission_objectives[0]->drop();
			mission_objectives.erase(mission_objectives.begin());
			alertBox->addText(L"Objective Complete",NORMAL);
			//go onto nextmission
		}
	}
	else
	{
		//complete mission
	}
}

//-------------------------------------------------------------------------------
// save and load functions
//-------------------------------------------------------------------------------

void CMission::setState(int objectivecount)
{
	if(mission_objectives.size()!=objectivecount)
	{
		mission_objectives[0]->drop();
		mission_objectives.erase(mission_objectives.begin());
	}
}

void CMission::save(io::IXMLWriter *writer)
{
	//each mission has a mission state and an index
	//save the mission state and the mission index
	//then we can replace where we left off in the mission

	
}
