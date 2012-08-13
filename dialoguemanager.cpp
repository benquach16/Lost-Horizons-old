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
#include "dialoguemanager.h"

dialogueManager::dialogueManager()
{
	//nothing to see here
}

dialogueManager::~dialogueManager()
{
	for(int i=0; i<dialogue_manager.size(); i++)
	{
		dialogue_manager[i]->drop();
		dialogue_manager.erase(dialogue_manager.begin()+i);
	}
	dialogue_manager.clear();
}

void dialogueManager::drop()
{
	delete this;
}

void dialogueManager::addTree(CDialogueTree *newtree)
{
	dialogue_manager.push_back(newtree);
}

void dialogueManager::loop()
{
	for(unsigned int i=0; i<dialogue_manager.size(); i++)
	{
		dialogue_manager[i]->loop();
		if(dialogue_manager[i]->getFinished()==true)
		{
			//delete
			dialogue_manager[i]->drop();
			dialogue_manager.erase(dialogue_manager.begin()+i);
		}
	}
}