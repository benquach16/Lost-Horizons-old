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
#include "item.h"

item::item(item_base *item_type) : CObject(item_type->name), equipped(false)
{
	this->item_class = item_type;
	item::setName(item_type->name);
	this->cost = item_type->cost;
	this->weight = item_type->weight;
}

item_base *item::getItemType()
{
	return item_class;
}

wchar_t *item::getItemDescription()
{
	return item_class->description;
}

int item::getItemCost()
{
	return cost;
}

int item::getItemWeight()
{
	return weight;
}

void item::drop()
{
	delete this;
}

int item::getItemIndex()
{
	return item_class->index;
}

item::~item()
{
}