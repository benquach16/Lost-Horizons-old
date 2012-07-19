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
#include "targetmenu.h"

//create everything
//setup for subsystem targetting
CTargetMenu::CTargetMenu(irr::IrrlichtDevice *graphics)
{
	//setup
	tmp=-1;
	this->graphics = graphics;
	this->screen_size = graphics->getVideoDriver()->getScreenSize();
	window_size = rect<s32>(screen_size.Width/2-200,screen_size.Height/2-200,screen_size.Width/2+200,screen_size.Height/2+200);
	window = graphics->getGUIEnvironment()->addWindow(window_size);
	window->setDrawTitlebar(false);
	window->getCloseButton()->setVisible(false);

	ship_name = graphics->getGUIEnvironment()->addStaticText(L"Ship Name", rect<s32>(10,10,window_size.getWidth()-10,30),false,true,window);
	ship_class = graphics->getGUIEnvironment()->addStaticText(L"Ship Class", rect<s32>(10,30,window_size.getWidth()-10,50),false,true,window);
	//ship_description = graphics->getGUIEnvironment()->addStaticText(L"Ship Description", rect<s32>(10,50,window_size.getWidth()-10,90),false,true,window);
	target_subsystem = graphics->getGUIEnvironment()->addStaticText(L"Target Subsystem :", rect<s32>(20,90,window_size.getWidth()-10,110),false,true,window);

	subsystems = graphics->getGUIEnvironment()->addListBox(rect<s32>(10,110,window_size.getWidth()/2,window_size.getHeight()-10),window);
	subsystems->addItem(L"Engine");
	subsystems->addItem(L"Warp Drive");
	subsystems->addItem(L"Primary Weapons");
	subsystems->addItem(L"Secondary Weapons");
	subsystems->addItem(L"Light Weapons");
	subsystem_health = graphics->getGUIEnvironment()->addStaticText(L"Health :", rect<s32>(window_size.getWidth()/2+10,110,window_size.getWidth()-10,window_size.getHeight()-10),false,true, window);

}

CTargetMenu::~CTargetMenu()
{
	//destructor
	window->drop();
}

void CTargetMenu::drop()
{
	delete this;
}

void CTargetMenu::setVisible(bool isvisible)
{
	window->setVisible(isvisible);
}

bool CTargetMenu::getVisible()
{
	return window->isVisible();
}

//get target subsystem health
void CTargetMenu::loop(CShip *player_target)
{
	if(player_target!=0)
	{
		ship_name->setText(player_target->getName());
		ship_class->setText(player_target->getShipClass()->name);
		//ship_description->setText(player_target->getShipClass()->description);

		//selected
		tmp = subsystems->getSelected();
		if(tmp!=-1)
		{
			int i;
			if(tmp==0)
			{
				//engine
				i = player_target->subsystem.engine;
			}
			if(tmp==1)
			{
				i=player_target->subsystem.warpdrive;
			}
			if(tmp==2)
			{
				i=player_target->subsystem.primary_weapons;
			}
			if(tmp==3)
			{
				i=player_target->subsystem.secondary_weapons;
			}
			if(tmp==4)
			{
				i=player_target->subsystem.light_weapons;
			}
			stringw text;
			text = L"Health :";
			text+=i;
			subsystem_health->setText(text.c_str());
		}
	}
	else
		tmp=-1;
}

//called in  gameloop to set damage
int CTargetMenu::getSelected()
{
	return tmp;
}