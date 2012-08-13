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

#ifndef _HUD_H_
#define _HUD_H_

#pragma once

#include "irrlicht.h"
#include "irrklang.h"
#include "player.h"
#include "planet.h"
#include "math.h"


using namespace irr;

class Hud
{
public:




	Hud(irr::IrrlichtDevice *graphics, Player *CPlayer);
	~Hud();
	void updateHud(CShip* player_target, planet* jump_target);
	void updateAnnoucements();						//legacy, but could be used later
	void addAnnoucement(const wchar_t);				//legacy, but could be used later

	int getDistance(CShip* player_target);
	float getDistancePlanet(planet* jump_target);

	void drop();

private:
	//oh my god
	Player *CPlayer;
	irr::IrrlichtDevice *graphics;
	irr::gui::IGUIStaticText *velocity;
	irr::gui::IGUIStaticText *hull;
	irr::gui::IGUIStaticText *armor;
	irr::gui::IGUIStaticText *shield;
	irr::gui::IGUIStaticText *cannons_primary;
	irr::gui::IGUIStaticText *cannons_primary_range;
	irr::gui::IGUIStaticText *cannons_secondary;
	irr::gui::IGUIStaticText *cannons_secondary_range;
	irr::gui::IGUIStaticText *cannons_light;
	irr::gui::IGUIStaticText *cannons_light_range;

	irr::gui::IGUIStaticText *target;
	irr::gui::IGUIStaticText *target_ship;
	irr::gui::IGUIStaticText *target_distance;
	irr::gui::IGUIStaticText *target_hull;
	irr::gui::IGUIStaticText *target_armor;
	irr::gui::IGUIStaticText *target_shield;

	irr::gui::IGUIStaticText *planet_target;
	irr::gui::IGUIStaticText *planet_distance;
	irr::gui::IGUIStaticText *planet_type;
	irr::gui::IGUIStaticText *planet_relations;
	

	gui::IGUIImage *target_icon;
	gui::IGUIImage *planet_target_icon;
	gui::IGUIImage *hud_image;
	gui::IGUIImage *target_info;
	gui::IGUIImage *planet_info;

	gui::IGUIListBox *annoucements;
	
	irr::gui::IGUIStaticText *view;
	irr::gui::IGUIStaticText *location_system;
};

#endif