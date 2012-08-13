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
#include "dialoguebox.h"

//setup basic box
//TODO: ADD DIALOGue oPTiONS
CDialogueBox::CDialogueBox(irr::IrrlichtDevice *graphics, const wchar_t *speaker_name, const wchar_t *text) : graphics(graphics)
{
	screen_size = graphics->getVideoDriver()->getScreenSize();
	window_size = rect<s32>(screen_size.Width/2-200,100,screen_size.Width/2+200,400);
	window = graphics->getGUIEnvironment()->addWindow(window_size);
	window->getCloseButton()->setVisible(false);
	window->setText(L"Communications Console");

	gui::IGUIFont *micro = graphics->getGUIEnvironment()->getFont("res/font/system.xml");
	speaker = graphics->getGUIEnvironment()->addStaticText(speaker_name,rect<s32>(10,30,400,50),false,true,window);
	main = graphics->getGUIEnvironment()->addStaticText(text,rect<s32>(10,70,400,220),false,true,window);

	ok = graphics->getGUIEnvironment()->addButton(rect<s32>(150,240,250,260),window,-1,L"Ok");
}

void CDialogueBox::changeText(const wchar_t *text)
{
	main->setText(text);
}

void CDialogueBox::drop()
{

	delete this;
}

CDialogueBox::~CDialogueBox()
{
	window->setVisible(false);
	window->remove();
}