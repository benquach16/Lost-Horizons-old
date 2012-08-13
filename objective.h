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

#ifndef _OBJECTIVE_H_
#define _OBJECTIVE_H_

#pragma once

#include "irrlicht.h"
#include "player.h"
#include "ship.h"


using namespace irr;
using namespace core;

//Defines what kind of objectives the player can receive
enum OBJ_TYPES
{
	OBJ_GOTO = 0,
	OBJ_SWEEP = 1,
	OBJ_DEFEND = 2,
	OBJ_PATROL = 3,
	OBJ_GET = 4,
	OBJ_DOCK = 5,
};

class CObjective
{
public:
	CObjective(const wchar_t *obj_title = L"OBJ_TITLE", OBJ_TYPES type = OBJ_GOTO, vector3df& objective_point = vector3df(0,0,0));
	~CObjective();
	bool loop(irr::IrrlichtDevice *graphics, bool display_obj, Player *CPlayer, std::vector<CShip*> ship_manager);
	void changeTitle(const wchar_t *newtitle)
	{
		objective_title = newtitle;
	}
	const wchar_t *getTitle()
	{
		return objective_title;
	}
	void drop();

private:
	core::vector2d<s32> array_pos;
	gui::IGUIImage *target_array;
	scene::ISceneNode *nav_buoy;	//used to tell where the objective is for the player
	vector3df objective_location;
	CShip *objective_target;
	OBJ_TYPES objective_type;
	const wchar_t *objective_title;
	bool objective_complete;
	bool display_objective;

};


#endif