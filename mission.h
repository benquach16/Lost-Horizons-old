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

//mission.h
//mission class file
//this file is far, far more threadsafe than player.h

#ifndef _MISSION_H_
#define _MISSION_H_

#pragma once

#include "irrlicht.h"
#include "objective.h"
#include "item.h"
#include "vector"
#include "player.h"
#include "alertbox.h"


using namespace irr;
using namespace core;
using namespace video;

class CMission
{
public:
	//uses an array as an argument
	//to store the objectives
	CMission(const wchar_t *title = L"MISS_TITLE", const wchar_t *description=L"MISS_DESC",
		std::vector<CObjective*> objectives = (std::vector<CObjective*>)0,
		int cash_reward=0,
		std::vector<item*> item_reward=(std::vector<item*>)0);
	~CMission();

	//Looped from the missionmanager object
	void loop(irr::IrrlichtDevice *graphics, Player *CPlayer, bool iscurrentmission, std::vector<CShip*> ship_manager, CAlertBox *alertBox);
	//deletion function
	void drop();

	//Used in saving and loading
	void setState(int objectivecount);
	void save(io::IXMLWriter *writer);
	void load(io::IXMLReader *reader);

	//changes the title of the mission
	void changeTitle(const wchar_t *new_title)
	{
		mission_title = new_title;
	}

	//changes the description
	void changeDescription(const wchar_t *new_desc)
	{
		mission_description = new_desc;
	}

	//returns title of mission
	const wchar_t *getTitle()
	{
		return mission_title;
	}

	//returns description
	const wchar_t *getDesc()
	{
		return mission_description;
	}

	//returns objectives in a vector to be read by the mission manager
	std::vector<CObjective*> getObjectives()
	{
		return mission_objectives;
	}

	//toggled for the missionmanager
	bool getMissionComplete()
	{
		return mission_complete;
	}
	//used by missionmanager
	void setMissionComplete(bool complete)
	{
		mission_complete = complete;
	}
	//retrun rewards
	int getCashReward()
	{
		return cash_reward;
	}
	std::vector<item*> getRewards()
	{
		return item_reward;
	}

	//for saving and loading and identfying mission
	void setIndex(int index)
	{
		this->index = index;
	}
	int getIndex()
	{
		return index;
	}
private:
	//important variables

	int index;			//used for saving and loading

	const wchar_t *mission_title;
	const wchar_t *mission_description;
	std::vector<CObjective*> mission_objectives;
	int cash_reward;
	bool mission_complete;
	std::vector<item*> item_reward;
};


#endif