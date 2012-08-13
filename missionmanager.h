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


//missionmanager.h
//Used to manage the missions that the player has taken on
//missionmanging works like this
//theres a missionmanager and it constantly loops thruogh the missions
//which constantly loops through its objectives to see if its been completed
//very simple


#ifndef _MISSIONMANAGER_H_
#define _MISSIONMANAGER_H_

#pragma once

#include "mission.h"
#include "missionlist.h"
#include "vector"

using namespace irr;
using namespace core;
using namespace io;

//manager class
class missionManager
{
public:
	//constructor
	missionManager(irr::IrrlichtDevice *graphics);
	//removes everything
	//only way to restore is thorugh save file
	~missionManager();

	//function to add mission to the list
	void addMission(CMission *newmission);
	//remove mission, might need better function in the future
	//comparing memory addresses is not as good as strings
	//which is which object id generation is imporrtant
	void removeMission(CMission *missiontoremove);

	//loop function
	void loop(Player *CPlayer, CMission *current_mission, std::vector<CShip*> ship_manager,CAlertBox *alertBox);


	//save functions
	stringw intToString(int num);
	void saveMissions(io::IXMLWriter *writer);
	void loadMissions(io::IXMLReader *reader, int nummissions);

	void drop();

	std::vector<CMission*> getMissionList()
	{
		return mission_list;
	}

private:
	irr::IrrlichtDevice *graphics;
	CMission *current_mission;
	std::vector<CMission*> mission_list;
};

#endif