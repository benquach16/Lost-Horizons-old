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


#ifndef _MENU_H_
#define _MENU_H_

#pragma once

#include "irrlicht.h"
#include "irrklang.h"
#include "keylistener.h"
#include "progressbar.h"
#include "missionmanager.h"
#include "player.h"

using namespace irr;
using namespace scene;
using namespace gui;
using namespace core;

//class player uses to manage he ship
class CMenu
{
public:


	//Constructor
	//needs all the drivers to be saved into a variable, and the player object to access variables
	CMenu(irr::IrrlichtDevice *graphics, irrklang::ISoundEngine *sound, KeyListener *receiver, Player *CPlayer);

	//destructor
	~CMenu();



	//create everything in the tabs
	void setupTabs();
	void menuLoop(Player *CPlayer, missionManager *missionM);
	void setMenuOpen(bool state);
	bool getMenuOpen();

	void runShip(Player *CPlayer);
	void runCargo(Player *CPlayer);
	void runCrew(Player *CPlayer);
	void runHanger(Player *CPlayer);
	void runLoadout(Player *CPlayer);
	void runMissions(missionManager *missionM);
	CMission *getSelectedMission();

	void drop();

	bool getReturnFighters()
	{
		if(hangerTab.recall->isPressed())
		{
			return true;
		}
		return false;
	}
	bool getSendFighters()
	{
		if(hangerTab.send->isPressed())
		{
			return true;
		}
		return false;
	}
private:
	bool menu_open;
	core::rect<s32> window_size;	//size of the menu window
	dimension2d<u32> t;				// size of actual application window
	CMission *selected_mission;

	irr::IrrlichtDevice *graphics;
	irrklang::ISoundEngine *sound;
	KeyListener *receiver;

	gui::IGUITabControl *control_tab;
	std::vector<irr::u32> *cargo_list;		//store each of the cargo items in an array so i dont repeat the list over and over.


	//tab for ship and everything in it
	gui::IGUITab *ship;
	struct shipTabStruct
	{
		gui::IGUIStaticText *ship;
		gui::IGUIStaticText *description;
		gui::IGUIStaticText *subsystems;
		gui::IGUIListBox *systems_list;
		gui::IGUIStaticText *systems_health;
		gui::IGUIStaticText *crew_req;
		gui::IGUIStaticText *crew_avail;
		gui::IGUIButton *repair;
		gui::IGUIButton *replace;


	};
	shipTabStruct shipTab;

	//tab for cargo
	gui::IGUITab *cargo;
	struct cargoTabStruct
	{
		gui::IGUIListBox *cargo_list;
		int cargo_temp;
		gui::IGUIStaticText *cargo_selected_description;
		gui::IGUIStaticText *cargo_selected_cost;
		gui::IGUIStaticText *cargo_selected_weight;
		gui::IGUIButton *cargo_remove;
		bool cargo_button_pressed;
		std::vector<int> cargo_array;
		gui::IGUIStaticText *money;
	};
	cargoTabStruct cargoTab;

	//tab for crew
	gui::IGUITab *crew;
	struct crewTabStruct
	{
		gui::IGUITreeView *officers;
		gui::IGUITreeViewNode *officers_node;
		gui::IGUITreeViewNode *crew_node;
		gui::IGUITreeViewNode *prisioners_node;
		gui::IGUITreeViewNode *passengers_node;
		gui::IGUITreeViewNode *helmsman;
		gui::IGUITreeViewNode *firecontrol;
		gui::IGUITreeViewNode *engineering;
		gui::IGUITreeViewNode *navigation;
		gui::IGUITreeViewNode *radar;
		gui::IGUIStaticText *officer_name;
		gui::IGUIStaticText *officer_title;
		gui::IGUIStaticText *officer_experience;
		gui::IGUIStaticText *officer_bonus;


		gui::IGUIStaticText *crew_num;
		gui::IGUIStaticText *crew_morale;
	};
	crewTabStruct crewTab;


	//tab for loadout
	gui::IGUITab *loadout;
	struct loadoutTabStruct
	{
		//three!
		gui::IGUIStaticText *weapons;
		gui::IGUIImage *primary_slot;
		gui::IGUIStaticText *primary_name;
		gui::IGUIStaticText *primary_description;


		gui::IGUIImage *secondary_slot;
		gui::IGUIStaticText *secondary_name;
		gui::IGUIStaticText *secondary_description;

		gui::IGUIImage *light_slot;
		gui::IGUIStaticText *light_name;
		gui::IGUIStaticText *light_description;


	};
	loadoutTabStruct loadoutTab;


	//tab for hanger
	gui::IGUITab *hanger;
	struct hangerTabStruct
	{
		gui::IGUITreeView *fighter_list;
		gui::IGUITreeViewNode *fighters;
		gui::IGUITreeViewNode *shuttles;

		gui::IGUIStaticText *fighter_name;
		gui::IGUIStaticText *fighter_description;
		gui::IGUIStaticText *fighter_hull;
		gui::IGUIStaticText *fighter_speed;
		gui::IGUIStaticText *fighter_time;

		gui::IGUIButton *recall;
		gui::IGUIButton *send;
	};
	hangerTabStruct hangerTab;

	//tab for missions
	gui::IGUITab *missions;
	struct missionTabStruct
	{
		gui::IGUIListBox *mission_list;
		gui::IGUIStaticText *mission_description;
		gui::IGUIListBox *objective_list;
		gui::IGUIButton *mission_remove;
	};
	missionTabStruct missionTab;

	int tab;
};



#endif