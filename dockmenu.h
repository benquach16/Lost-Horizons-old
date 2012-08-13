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


//Dock Menu
//Opens when player is docked at a station

#ifndef _DOCKMENU_H_
#define _DOCKMENU_H_

#include "irrlicht.h"
#include "player.h"
#include "keylistener.h"
#include "ship.h"

using namespace irr;
using namespace gui;
using namespace video;
using namespace core;

class CDockMenu
{
public:
	CDockMenu(irr::IrrlichtDevice *graphics, KeyListener *receiver);
	~CDockMenu();



	void setupTabs();
	void setVisible(bool isvisible);
	void menuLoop(Player *CPlayer,CShip* docked_station);
	void runMarket(Player *CPlayer,CShip* docked_station);
	void runRepair(Player *CPlayer);

	void drop();

	void setMenuOpen(bool open)
	{
		menu_open=open;
	}
	
private:
	bool menu_open;

	irr::IrrlichtDevice *graphics;
	KeyListener *receiver;
	core::dimension2d<s32> t;
	core::rect<s32> window_size;
	gui::IGUIElement *control;
	gui::IGUITabControl *tab_control;
	gui::IGUITab *market;
	struct marketTabStruct
	{
		gui::IGUIListBox *buying;
		gui::IGUIStaticText *buy_cost;
		gui::IGUIStaticText *buy_description;
		gui::IGUIListBox *selling;
		gui::IGUIStaticText *sell_cost;
		gui::IGUIButton *buy_button;
		bool buy_pressed;
		gui::IGUIButton *sell_button;
		bool sell_pressed;
		gui::IGUIStaticText *player_money;
	};
	marketTabStruct marketTab;

	std::vector<item*> temp_pri;
	std::vector<item*> temp_sec;
	gui::IGUITab *repair;
	struct repairTabStruct
	{
		gui::IGUIButton *repair_hull;
		gui::IGUIStaticText *repair_hull_price;
		gui::IGUIButton *repair_armor;
		gui::IGUIStaticText *repair_armor_price;

		gui::IGUIComboBox *weapon_primary;
		gui::IGUIStaticText *primary;
		gui::IGUIStaticText *primary_description;
		gui::IGUIComboBox *weapon_secondary;
		gui::IGUIStaticText *secondary;
		gui::IGUIStaticText *secondary_description;
		gui::IGUIComboBox *weapon_light;
		gui::IGUIStaticText *light;
		gui::IGUIStaticText *light_description;
	};
	repairTabStruct repairTab;

	gui::IGUITab *tavern;
	gui::IGUITab *headquarter;
};

#endif