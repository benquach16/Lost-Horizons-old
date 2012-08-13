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
#include "planet.h"

planet::planet(irr::IrrlichtDevice *graphics, planet_base *planet_type, core::vector3df& position, ship_faction faction, const wchar_t *name, core::stringc texture) : CObject(name)
{
	this->graphics = graphics;
	this->faction = faction;
	this->planet_type = planet_type;
	this->model = graphics->getSceneManager()->addAnimatedMeshSceneNode(graphics->getSceneManager()->getMesh(planet_type->planet_model));
	this->model->setPosition(position);
	this->model->setScale(planet_type->scale);
	this->model->setMaterialTexture(0,graphics->getVideoDriver()->getTexture(texture));
	this->model->getMaterial(0).NormalizeNormals=true;
	this->model->setMaterialType(video::EMT_SOLID);

	//ensure only certain planets get clouds
	//having moons with clouds would be dumb
	if(planet_type->clouds==true)
	{
		this->cloud = graphics->getSceneManager()->addAnimatedMeshSceneNode(graphics->getSceneManager()->getMesh(planet_type->planet_model));
		this->cloud->setPosition(position);
		this->cloud->setScale(planet_type->cloud_scale);
		this->cloud->setMaterialTexture(0,graphics->getVideoDriver()->getTexture(planet_type->cloud_map));
		this->cloud->getMaterial(0).NormalizeNormals=true;
		this->cloud->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL);
	}

	if(planet_type->corona==true)
	{
		corona =  graphics->getSceneManager()->addCubeSceneNode(3,0,-1,this->model->getPosition());
		corona->setMaterialTexture(0,graphics->getVideoDriver()->getTexture("res/textures/atmos.png"));
		corona->setScale(core::vector3df(planet_type->cloud_scale.X,planet_type->cloud_scale.Y,0));
		corona->setRotation(graphics->getSceneManager()->getActiveCamera()->getRotation());
		corona->setMaterialFlag(video::EMF_LIGHTING,false);
		corona->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL);
	}
	array_pos = graphics->getSceneManager()->getSceneCollisionManager()->getScreenCoordinatesFrom3DPosition(position,graphics->getSceneManager()->getActiveCamera());
	target_array = graphics->getGUIEnvironment()->addImage(graphics->getVideoDriver()->getTexture("res/menu/target_array.png"),array_pos);
}

//This func is called by the gameManager loop
void planet::rotate(f32 frameDeltaTime)
{
	array_pos = graphics->getSceneManager()->getSceneCollisionManager()->getScreenCoordinatesFrom3DPosition(model->getPosition(),graphics->getSceneManager()->getActiveCamera());
	target_array->setRelativePosition(vector2d<s32>(array_pos.X-32,array_pos.Y-32));

	
	core::vector3df rot = model->getRotation();
	
	rot.Y+=planet_type->rotation_speed*frameDeltaTime;
	model->setRotation(rot);

	if(planet_type->clouds==true)
	{
		core::vector3df tmp = cloud->getRotation();
		tmp.Y-=planet_type->rotation_speed*frameDeltaTime;
		cloud->setRotation(tmp);
	}
}


//TODO: add planet capture system
void planet::planetCapture()
{
}

int planet::getFactionRelation()
{
	return this->factionRelations;
}

void planet::setFactionRelation(int newrelation)
{
	this->factionRelations = newrelation;
}

void planet::setFaction(ship_faction faction)
{
	this->faction = faction;
}

ship_faction planet::getFaction()
{
	return faction;
}

vector2d<int> planet::getArrayPos()
{
	return this->array_pos;
}

void planet::setArrayVisible(bool visible)
{
	target_array->setVisible(visible);
}

core::vector3df planet::getPos()
{
	return this->model->getPosition();
}

planet_base *planet::getPlanetType()
{
	return this->planet_type;
}

void planet::setCloudsVisible(bool visible)
{
	if(planet_type->clouds==true)
		cloud->setVisible(visible);
}

planet::~planet()
{
	model->remove();
	target_array->remove();
	if(planet_type->clouds==true)
	{
		cloud->remove();
	}

	if(planet_type->corona==true)
	{
		corona->remove();
	}
}

void planet::drop()
{
	delete this;
}

