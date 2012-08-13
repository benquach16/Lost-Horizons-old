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
#include "missionmanager.h"

missionManager::missionManager(irr::IrrlichtDevice *graphics) : graphics(graphics)
{
	//initializer lists are more efficient than initializing
	//in the constructor

}

void missionManager::loop(Player *CPlayer, CMission *current_mission,std::vector<CShip*> ship_manager,CAlertBox *alertBox)
{
	//loop thru all of the missions
	for(unsigned int i = 0; i<mission_list.size();i++)
	{
		if(mission_list[i]->getMissionComplete()!=true)
		{
			//missions with less than 1 objective auto completes
			if(mission_list[i]->getObjectives().size()>0)
			{
				//this is needed because the current mission has a bool that it needs to turn on
				//in order to display the mission objectives in the 3d world
				if(current_mission!=0)
				{
					if(mission_list[i]==current_mission)
					{
						mission_list[i]->loop(graphics,CPlayer,true, ship_manager,alertBox);
					}
					else
						mission_list[i]->loop(graphics,CPlayer,false, ship_manager,alertBox);
				}
				else
					mission_list[i]->loop(graphics,CPlayer,false, ship_manager, alertBox);
			}
			else
			{
				//objectives complete
				//mission complete
				//give rewards
				mission_list[i]->setMissionComplete(true);
				//mission stays in your log
				//title is modified to include complete
				//TODO: remove missions without breaking the game
				alertBox->addText(L"Mission Complete",NORMAL);
				CPlayer->setMoney(CPlayer->getMoney()+mission_list[i]->getCashReward());
				//give rewards
				for(int n=0; n<mission_list[i]->getRewards().size(); n++)
				{
					CPlayer->addItemToCargo(mission_list[i]->getRewards()[n]);
				}
				//stringw title = L"[COMPLETE]";
				//title+=mission_list[i]->getTitle();
				//mission_list[i]->changeTitle(title.c_str());

			}
		}
	}
}

//delete everything function
missionManager::~missionManager()
{
	//drop everything inside the mission list
	//then clear
	for(unsigned int i=0 ;i< mission_list.size(); i++)
	{
		mission_list[i]->drop();
		mission_list.erase(mission_list.begin()+i);
	}
	mission_list.clear();
}

void missionManager::drop()
{
	delete this;
}

//add mission to the end of the missionlist
void missionManager::addMission(CMission *newmission)
{
	mission_list.push_back(newmission);
}

//replace with better func later
void missionManager::removeMission(CMission *missiontoremove)
{

	//scans through the array
	//then if it finds the right mission, delete it
	for(unsigned int i=0; i<mission_list.size();i++)
	{
		if(missiontoremove == mission_list[i])
		{
			mission_list.erase(mission_list.begin()+i);
		}
	}
}



//--------
//save load functions
//--------

stringw missionManager::intToString(int num)
{
	stringw t(L"");
	t+=num;
	return t;
}

void missionManager::saveMissions(io::IXMLWriter *writer)
{
	core::array<stringw> value;
	value.push_back(L"num");
	core::array<stringw> num;
	//save number of missions
	num.push_back(intToString(mission_list.size()));

	writer->writeElement(L"missions",true,value,num);
	
	for(unsigned int i=0; i< mission_list.size(); i++)
	{
		core::array<stringw> attributes;
		core::array<stringw> values;

		attributes.push_back(L"index");
		values.push_back(intToString(mission_list[i]->getIndex()));

		attributes.push_back(L"state");
		values.push_back(intToString(mission_list[i]->getObjectives().size()));
		writer->writeElement(intToString(i).c_str(),true,attributes,values);
	}
	writer->writeClosingTag(L"missions");
}

//load the index of the mission and then the state
void missionManager::loadMissions(io::IXMLReader *reader, int nummissions)
{
	for(u32 i = 0; i<nummissions; i++)
	{
		if(core::stringw(intToString(i)).equals_ignore_case(reader->getNodeName()))
		{
			int index = reader->getAttributeValueAsInt(L"index");
			int obj = reader->getAttributeValueAsInt(L"state");

			addMission(missionCreate(graphics,index).tut);
		}
	}
}