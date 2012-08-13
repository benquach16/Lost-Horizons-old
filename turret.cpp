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
#include "turret.h"

using namespace irr;
using namespace scene;


//turret object code
//controlled mostly by outside objects

turret::turret(irr::IrrlichtDevice *graphics,int turret_number,scene::IBoneSceneNode *bone, core::vector3df& pos_offset, turret_base *t_class, scene::ISceneNode *newparent) : item(t_class)
{
	this->bone = bone;
	this->offset = pos_offset;
	this->t_class = t_class;
	this->turret_number = turret_number;
	this->icon = graphics->getVideoDriver()->getTexture(t_class->icon);
	t_model = graphics->getSceneManager()->addAnimatedMeshSceneNode(graphics->getSceneManager()->getMesh(t_class->model),0,-1,vector3df(0,0,0),vector3df(0,0,0),vector3df(1,1,1),true);
	t_model->setScale(t_class->scale);
	this->rotate_speed = t_class->rotation_speed;
	this->reload_time = t_class->reload_time;
	
}

turret::~turret()
{
	//make sure to kill the model aswell
	t_model->setVisible(false);
	t_model->remove();
}

//change turret class
void turret::setTurret(turret_base *newclass)
{
	t_class = newclass;
	//t_model->remove();
	//t_model = graphics->getSceneManager()->addAnimatedMeshSceneNode(graphics->getSceneManager()->getMesh(t_class->model));
	t_model->setScale(newclass->scale);
	this->rotate_speed = newclass->rotation_speed;
	this->reload_time = newclass->reload_time;

}

void turret::aimTurret(core::vector3df& newrot, f32 frameDeltaTime)
{
	//This func slowly rotates the turret to face the desired rotation
	//Makes it more turretlike


	//t_model->setRotation(newrot);
	core::vector3df t_rot;
	t_rot = t_model->getRotation();

	t_rot.X = newrot.X;
	if(t_model->getRotation().Y<newrot.Y)
	{
		t_rot.Y+=rotate_speed*frameDeltaTime;
	}
	if(t_model->getRotation().Y>newrot.Y)
	{
		t_rot.Y-=rotate_speed*frameDeltaTime;
	}
	//t_rot.Z = 0;
	t_model->setRotation(t_rot);
}

//set position
void turret::setPos(core::vector3df& newpos)
{
	t_model->setPosition(newpos);
}

//get rotation
core::vector3df turret::getRot() const
{
	return t_model->getRotation();
}

//get position
core::vector3df turret::getPos() const
{
	return t_model->getAbsolutePosition();
}

//get turret offset
core::vector3df turret::getOffset()
{
	return offset;
}

//get turret num
int turret::getTurretNum() const
{
	return turret_number;
}

//get position of turretbone
core::vector3df turret::getBonePos()
{
	return bone->getAbsolutePosition();
}

//get pos of shoot pos
core::vector3df turret::getFirePos()
{
	return t_model->getJointNode("shoot")->getAbsolutePosition();
}

//return turret bone type
scene::IBoneSceneNode *turret::getBone()
{ 
	return bone;
}

int turret::getRotSpeed()
{
	return rotate_speed;
}

int turret::getReloadTime()
{
	return t_class->reload_time;
}

turret_base *turret::getTurretClass()
{
	return t_class;
}

ITexture *turret::getIcon()
{
	return icon;
}