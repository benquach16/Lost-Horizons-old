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

#ifndef _PLANET_H_
#define _PLANET_H_

#include "irrlicht.h"
#include "sTypes.h"
#include "object.h"
#include "shadercallback.h"

using namespace irr;
using namespace scene;
using namespace core;

class planet : public CObject
{
public:
	
	scene::IAnimatedMeshSceneNode *model;
	scene::ISceneNode *cloud;
	scene::ISceneNode *corona;

	//constructor
	planet(irr::IrrlichtDevice *graphics, planet_base *planet_type, core::vector3df& position, ship_faction faction, const wchar_t *name, core::stringc texture);
	//give the planets some animation
	void rotate(f32 frameDeltaTime);
	void planetCapture();
	int getFactionRelation();
	void setFactionRelation(int newrelation);
	ship_faction getFaction();
	void setFaction(ship_faction faction);
	vector2d<int> getArrayPos();
	void setArrayVisible(bool visible);
	core::vector3df getPos();
	planet_base *getPlanetType();
	void setCloudsVisible(bool visible);

	void drop();
	~planet(); //supernova

private:

	ship_faction faction;
	planet_base *planet_type;
	irr::IrrlichtDevice *graphics;
	int factionRelations;
	core::vector2d<int> array_pos;
	gui::IGUIImage *target_array;

};



#endif