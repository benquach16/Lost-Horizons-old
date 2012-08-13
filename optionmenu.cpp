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
#include "optionmenu.h"


//setup GUI and initialize variables
COptionMenu::COptionMenu(irr::IrrlichtDevice *graphics)
{
	core::dimension2d<u32> t;
	setVisible(false);
	t =graphics->getVideoDriver()->getScreenSize();
	//create window
	window = graphics->getGUIEnvironment()->addWindow(rect<s32>(t.Width/2-300,t.Height/2-200,t.Width/2+300,t.Height/2+200),true,L"Options");
	//window = graphics->getGUIEnvironment()->addTabControl(rect<s32>(t.Width/2-300,t.Height/2-200,t.Width/2+300,t.Height/2+200),0,false);
	//ensure no bullshit occurs
	window->setDraggable(false);
	window->setDrawTitlebar(false);
	window->getCloseButton()->setVisible(false);

	quit = graphics->getGUIEnvironment()->addButton(rect<s32>(480,360,580,380),window,-1,L"Close");
	apply= graphics->getGUIEnvironment()->addButton(rect<s32>(360,360,460,380),window,-1,L"Apply");

	//create resolution menus
	//to select resolution
	graphics->getGUIEnvironment()->addStaticText(L"Resolution",rect<s32>(20,20,100,40),false,true,window);
	graphics->getGUIEnvironment()->addStaticText(L"x",rect<s32>(130,40,140,60),false,true,window);
	resolutionX = graphics->getGUIEnvironment()->addComboBox(rect<s32>(20,40,120,60),window);
	resolutionX->addItem(L"1024");
	resolutionX->addItem(L"1280");
	resolutionX->addItem(L"1366");
	resolutionX->addItem(L"1440");
	resolutionX->addItem(L"1600");
	resolutionX->addItem(L"1680");
	resolutionX->addItem(L"1920");

	resolutionY = graphics->getGUIEnvironment()->addComboBox(rect<s32>(150,40,250,60),window);
	resolutionY->addItem(L"768");
	resolutionY->addItem(L"800");
	resolutionY->addItem(L"900");
	resolutionY->addItem(L"960");
	resolutionY->addItem(L"1024");
	resolutionY->addItem(L"1050");
	resolutionY->addItem(L"1080");

	graphics->getGUIEnvironment()->addStaticText(L"Graphics Quality", rect<s32>(20,80,220,100),false,true,window);
	low_fi = graphics->getGUIEnvironment()->addComboBox(rect<s32>(20,100,220,120),window);
	low_fi->addItem(L"High Quality Graphics");
	low_fi->addItem(L"Low Quality Graphics");

	graphics->getGUIEnvironment()->addStaticText(L"You must restart the game in order for any graphics changes to take affect", rect<s32>(20,300,400,320),false,true,window);

	windowed = graphics->getGUIEnvironment()->addCheckBox(true,rect<s32>(20,320,40,340),window);
	graphics->getGUIEnvironment()->addStaticText(L"Windowed",rect<s32>(50,320,150,340),false,true,window);
}

void COptionMenu::loop()
{
	//ensure menu is hidden or not hidden
	window->setVisible(visible);
	if(quit->isPressed())
	{
		visible=false;
	}
}

COptionMenu::~COptionMenu()
{
}

int COptionMenu::getResolutionX()
{
	//convert wchar_t to int
	//and return
	const wchar_t *t = resolutionX->getItem(resolutionX->getSelected());
	char *i = new char[wcslen(t)+1];
	wcstombs(i,t,wcslen(t)+1);
	return std::atoi(i);

	
}
int COptionMenu::getResolutionY()
{
	//THIS WORKS
	const wchar_t *t = resolutionY->getItem(resolutionY->getSelected());
	char *i = new char[wcslen(t)+1];
	wcstombs(i,t,wcslen(t)+1);
	return std::atoi(i);
}

//if the radio button is checked
//taht means the player wants windowed
bool COptionMenu::getWindowed()
{
	if(windowed->isChecked())
	{
		return true;
	}
	return false;
}

bool COptionMenu::getHighQuality()
{
	if(low_fi->getSelected()==0)
	{
		//high qual
		return true;
	}
	else
		return false;
}

//to apply settings to ini file
//return apply button
gui::IGUIButton *COptionMenu::getApplyButton()
{
	return apply;
}