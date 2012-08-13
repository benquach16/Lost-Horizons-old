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
#include "objective.h"

//initialize all the variables
//tell the game that this objective exists
CObjective::CObjective(const wchar_t *obj_title, OBJ_TYPES type, vector3df& objective_point) : objective_title(obj_title)
{
	this->objective_type = type;
	this->objective_complete = false;
	this->objective_location = objective_point;

	nav_buoy = 0;
}

CObjective::~CObjective()
{
	if(nav_buoy!=0)
	{
		target_array->remove();
		nav_buoy->remove();
	}
}

//main loop
//check if player is fulfilling objective
bool CObjective::loop(irr::IrrlichtDevice *graphics, bool display_obj, Player *CPlayer, std::vector<CShip*> ship_manager)
{
	if(display_obj==true)
	{
		//if the player has this as his active quest
		//show the navigation
		
		if(nav_buoy ==0)
		{
			//set up player navigational information
			nav_buoy = graphics->getSceneManager()->addAnimatedMeshSceneNode(graphics->getSceneManager()->getMesh("res/models/nav_buoy.X"));
			nav_buoy->setMaterialFlag(video::EMF_LIGHTING,false);
			nav_buoy->setScale(vector3df(1,1,1));
			nav_buoy->setPosition(objective_location);

			//create 2d interface
			target_array = graphics->getGUIEnvironment()->addImage(graphics->getVideoDriver()->getTexture("res/menu/nav_array.png"),core::vector2d<s32>(0,0));
		}
		else
		{

			//once you create all the information
			//update it constantly
			//this is for aesthetics more than anything
			//it doesnt actually contribute gameplay value
			array_pos = graphics->getSceneManager()->getSceneCollisionManager()->getScreenCoordinatesFrom3DPosition(objective_location,graphics->getSceneManager()->getActiveCamera());
			nav_buoy->setRotation(nav_buoy->getRotation()+vector3df(0,1,0));
			target_array->setRelativePosition(vector2d<s32>(array_pos.X-32,array_pos.Y-32));
			core::dimension2d<u32> t = graphics->getVideoDriver()->getScreenSize();

			//draw two lines on the screen pointing to the nav beacon
			//cause it looks cool
			graphics->getVideoDriver()->draw2DLine(vector2d<int>(array_pos.X,0),vector2d<int>(array_pos.X,t.Height),video::SColor(128,128,128,128));
			graphics->getVideoDriver()->draw2DLine(vector2d<int>(0,array_pos.Y),vector2d<int>(t.Width,array_pos.Y),video::SColor(128,128,128,128));
		}
	}
	//If the objective is a go to point objective
	if(objective_type==OBJ_GOTO)
	{
		if(CPlayer->getPos().getDistanceFrom(objective_location)<500)
		{
			//if player is closer than 500
			//return true
			return true;
		}
	}
	//sweep objective
	if(objective_type==OBJ_SWEEP)
	{
		int tmp=0;
		for(unsigned int i=0; i<ship_manager.size(); i++)
		{
			if(ship_manager[i]->getShipFaction()==FACTION_PROVIAN_CONSORTIUM)
			{
				if(ship_manager[i]->getPos().getDistanceFrom(objective_location)<10000)
				{
					tmp+=1;
				}
			}
		}
		if(tmp==0)
			return true;
	}
	return false;
}

void CObjective::drop()
{
	delete this;
}