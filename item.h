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

#ifndef _ITEM_H_
#define _ITEM_H_

#include "irrlicht.h"
#include "object.h"
#include "sTypes.h"

class item : public CObject
{
public:
	item(item_base *item_type);
	~item();
	item_base *getItemType();
	void drop();
	wchar_t *getItemDescription();
	int getItemWeight();
	int getItemCost();
	int getItemIndex();
private:
	item_base *item_class;
	int cost;
	int weight;
	bool equipped;
};

#endif