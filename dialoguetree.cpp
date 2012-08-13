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
#include "dialoguetree.h"

CDialogueTree::CDialogueTree(irr::IrrlichtDevice *graphics, const wchar_t *speaker, const wchar_t *text) : finished_dialogue(false), button_press(false)
{
	//create the main dialogue box
	main = new CDialogueBox(graphics,speaker,text); 
}

CDialogueTree::~CDialogueTree()
{
	main->drop();
	//just delete
}

void CDialogueTree::loop()
{

	if(main->getOKButton()->isPressed())
	{
		if(button_press==false)
		{
			button_press=true;
			//go on with dialogue tree
			if(text_list.size()>0)
			{
				main->changeText(text_list[0]);
				text_list.erase(text_list.begin());	

			}
			else
			{
				//finished dialogue
				finished_dialogue=true;
			}
		}
	}
	else
	{
		button_press=false;
	}
}

void CDialogueTree::drop()
{
	delete this;
}