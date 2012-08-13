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

#ifndef _DIALOGUETREE_H_
#define _DIALOGUETREE_H_


#include "irrlicht.h"
#include "dialoguebox.h"
#include "vector"

using namespace irr;
using namespace core;
using namespace scene;
using namespace gui;

class CDialogueTree
{
public:
	CDialogueTree(irr::IrrlichtDevice *graphics, const wchar_t *speaker, const wchar_t *text);
	~CDialogueTree();
	void loop();
	void drop();
	bool getFinished()
	{
		return finished_dialogue;
	}
	void addText(const wchar_t* text)
	{
		text_list.push_back(text);
	}

	
private:
	CDialogueBox *main;
	std::vector<const wchar_t*> text_list;

	bool button_press;
	bool finished_dialogue;
};

#endif