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

#ifndef _DIALOGUEBOX_H_
#define _DIALOGUEBOX_H_

#include "irrlicht.h"

using namespace irr;
using namespace core;
using namespace scene;
using namespace gui;

class CDialogueBox 
{
public:
	CDialogueBox(irr::IrrlichtDevice *graphics, const wchar_t *speaker_name, const wchar_t *text);
	~CDialogueBox();
	void loop();
	void changeText(const wchar_t *text);
	void changeSpeaker(const wchar_t *text);
	gui::IGUIButton *getOKButton()
	{
		return ok;
	}
	gui::IGUIButton *getYesButton()
	{
		return yes;
	}
	gui::IGUIButton *getNoButton()
	{
		return no;
	}
	void drop();

private:
	irr::IrrlichtDevice *graphics;
	gui::IGUIWindow *window;
	gui::IGUIStaticText *speaker;
	gui::IGUIStaticText *main;
	gui::IGUIButton	*ok;
	gui::IGUIButton *yes;
	gui::IGUIButton *no;

	core::dimension2d<u32> screen_size;
	rect<s32> window_size;

	const wchar_t *speaker_name;
	const wchar_t *text;

};


#endif