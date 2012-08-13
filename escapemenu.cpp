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
#include "escapemenu.h"



core::dimension2d<s32> t;
//setup once
CEscapeMenu::CEscapeMenu(irr::IrrlichtDevice *graphics)
{
	this->graphics = graphics;
	t=graphics->getVideoDriver()->getScreenSize();
	escape_window = graphics->getGUIEnvironment()->addWindow(rect<s32>(t.Width/2-100,t.Height/2-200,t.Width/2+100,t.Height/2+200),true,L"Menu");
	escape_window->setDraggable(false);
	escape_window->setDrawTitlebar(false);
	escape_window->getCloseButton()->setVisible(false);
	
	save_button = graphics->getGUIEnvironment()->addButton(rect<s32>(50,40,150,60),escape_window,1,L"Save Game");
	quit_button = graphics->getGUIEnvironment()->addButton(rect<s32>(50,80,150,100),escape_window,2,L"Quit Game");
}


void CEscapeMenu::drop()
{
	delete this;
}

CEscapeMenu::~CEscapeMenu()
{
	escape_window->remove();
}

void CEscapeMenu::run()
{
}

bool CEscapeMenu::getSaveButtonPressed()
{
	return save_button->isPressed();
}

bool CEscapeMenu::getQuitButtonPressed()
{
	return quit_button->isPressed();
}

void CEscapeMenu::setVisible(bool isvisible)
{
	escape_window->setVisible(isvisible);
}

bool CEscapeMenu::getVisible()
{
	return escape_window->isVisible();
}