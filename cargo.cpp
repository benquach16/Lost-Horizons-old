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
#include "cargo.h"

#define SPINSPEED 10

cargo::cargo(irr::IrrlichtDevice *graphics, vector3df &pos) : CObject(L"Loot")
{
	this->graphics = graphics;
	this->pos = pos;

	model = graphics->getSceneManager()->addAnimatedMeshSceneNode(graphics->getSceneManager()->getMesh("res/models/equipment/loot.3DS"));
	model->setPosition(pos);

	item *i = new item(items().ELECTRONICS);
	addItemToInventory(i);

}

void cargo::loop(f32 frameDeltaTime)
{
	//make the cargobox spin
	vector3df temp = model->getRotation();
	temp.Y+=SPINSPEED*frameDeltaTime;
	temp.Z+=1*frameDeltaTime;

	model->setRotation(temp);
}

void cargo::drop()
{
	delete this;
}

cargo::~cargo()
{
	model->setVisible(false);
	model->remove();
}

std::vector<item*> cargo::getInventory()
{
	return inventory;
}

void cargo::addItemToInventory(item* itemtoadd)
{
	inventory.push_back(itemtoadd);
}

vector3df cargo::getPos()
{
	return pos;
}