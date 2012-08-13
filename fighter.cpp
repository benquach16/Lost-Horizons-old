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
#include "fighter.h"

//3km patrol distance
#define PATROLDISTANCE 600



fighter::fighter(irr::IrrlichtDevice *graphics,irrklang::ISoundEngine *sound, vector3df &pos, vector3df &rot, fighter_base *f_type, ship_faction faction, scene::IAnimatedMeshSceneNode *home_base, const wchar_t *name) : 
CObject(name), alive(true), graphics(graphics), sound(sound), f_type(f_type), return_home(false)
{
	//initialize variables
	this->maxTurn = f_type->maxTurn;
	this->faction = faction;
	this->hullPoints = f_type->hullPoints;
	this->home_base = home_base;

	turn.X = rot.X;
	turn.Y = rot.Y;
	turn.Z = rot.Z;

	target = 0;
	fighter_target = 0;
	dogfighting=false;
	velocity = 0;
	last_move = 0;

	cannon.cannon=false;
	cannon.cannon_time = graphics->getTimer()->getTime();
	cannon.missile=false;
	cannon.missile_time = graphics->getTimer()->getTime();

	fighter_model = graphics->getSceneManager()->addAnimatedMeshSceneNode(graphics->getSceneManager()->getMesh(f_type->ship));
	if(fighter_model)
	{
		fighter_model->setPosition(pos);
		fighter_model->setRotation(rot);
		fighter_model->setScale(f_type->scale);
	}

	//create engine plumes
	exhaust = fighter_model->getJointNode("exhaust");
	scene::IParticleSystemSceneNode *ps = graphics->getSceneManager()->addParticleSystemSceneNode(false,exhaust);
	scene::IParticleEmitter *em = ps->createSphereEmitter(core::vector3df(0,0,0),1,core::vector3df(0,0.023,0),100,200,video::SColor(128,128,128,140),video::SColor(0,0,0,10),500,1000,0,core::dimension2df(2,2),core::dimension2df(5,5));
	ps->setEmitter(em);
	em->drop();
	ps->setMaterialTexture(0,graphics->getVideoDriver()->getTexture("res/textures/engine_trails.pcx"));
	ps->setMaterialFlag(video::EMF_LIGHTING,false);
	ps->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);

	//set timer
	fuel = graphics->getTimer()->getTime()+f_type->fuel;

	if(faction == FACTION_PROVIAN_CONSORTIUM)
		hostile = true;
	if(faction == FACTION_TERRAN_FEDERATION)
		hostile = false;
	if(faction == FACTION_PIRATE)
		hostile = true;

	//set gui interface so player can see them
	array_pos = graphics->getSceneManager()->getSceneCollisionManager()->getScreenCoordinatesFrom3DPosition(getPos(),graphics->getSceneManager()->getActiveCamera());
	if(hostile == true)
		target_array = graphics->getGUIEnvironment()->addImage(graphics->getVideoDriver()->getTexture("res/menu/enemy_fighter_target_array.png"),array_pos);
	else
		target_array = graphics->getGUIEnvironment()->addImage(graphics->getVideoDriver()->getTexture("res/menu/fighter_target_array.png"),array_pos);
}

fighter::~fighter()
{
	fighter_model->setVisible(false);
	target_array->remove();
}

void fighter::AIRun(f32 frameDeltaTime)
{
	if(hullPoints>0)
	{

		array_pos = graphics->getSceneManager()->getSceneCollisionManager()->getScreenCoordinatesFrom3DPosition(getPos(),graphics->getSceneManager()->getActiveCamera());
		target_array->setRelativePosition(vector2d<s32>(array_pos.X-8,array_pos.Y-8));
		//movement
		if(getRot().Y< turn.Y)		//rotate right
		{
			sRot = getRot();
			rotSlow.Y = (abs(getRot().Y-turn.Y));	//simulate inertia
			if(rotSlow.Y > maxTurn)
			{
				rotSlow.Y = maxTurn;
			}
			sRot.Y+=rotSlow.Y *frameDeltaTime;
			setRot(sRot);
		}
		if(getRot().Y > turn.Y)	//rotate left
		{
			sRot = getRot();
			rotSlow.Y = (abs(getRot().Y-turn.Y));	//simulate inertia
			if(rotSlow.Y > maxTurn)
			{
				rotSlow.Y = maxTurn;
			}
			sRot.Y-=rotSlow.Y *frameDeltaTime;
			setRot(sRot);
		}
		if(getRot().X < turn.X)	//rotate up
		{
			sRot = getRot();
			rotSlow.X = (abs(getRot().X-turn.X));	//simulate inertia
			if(rotSlow.X > maxTurn)
			{
				rotSlow.X = maxTurn;
			}
			sRot.X+=rotSlow.X *frameDeltaTime;
			setRot(sRot);
		}
		if(getRot().X > turn.X)	//rotate down
		{
			sRot = getRot();
			rotSlow.X = (abs(getRot().X-turn.X));	//simulate inertia
			if(rotSlow.X > maxTurn)
			{
				rotSlow.X = maxTurn;
			}
			sRot.X-=rotSlow.X *frameDeltaTime;
			setRot(sRot);
		}

		if(velocity!=0)
		{
			sPos=getPos();
			float i = getRot().Y;
			float z = -(getRot().X);	//if i dont do this the ship doesnt rotate right


			sPos.Y=frameDeltaTime*velocity*(sin(z * 3.14/180));
			sPos.Y+=getPos().Y;

			sPos.X=frameDeltaTime*velocity*(sin(i * 3.14/180));
			sPos.X+=getPos().X;

			sPos.Z=frameDeltaTime*velocity*(cos(i * 3.14/180));
			sPos.Z+=getPos().Z;


			setPos(sPos);
		}

	}
}


void fighter::drop()
{
	delete this;
}

void fighter::damageFighter(int damage)
{
	hullPoints -=damage;
}


void fighter::patrol(vector3df point)
{
	//make sure flying within bounds
	if(getPos().getDistanceFrom(point)>PATROLDISTANCE)
	{

		vector3df diff = point - getPos();
		turn.X = diff.getHorizontalAngle().X;
		turn.Y = diff.getHorizontalAngle().Y;
		turn.X += rand()%10;
		
	}
}

//basic function
//turn away from the point
void fighter::moveAwayFromPoint(vector3df &pos)
{
	const float x = (pos.X - getPos().X);
	const float y = (pos.Y - getPos().Y);
	const float z = (pos.Z - getPos().Z);
	float angleY = std::atan2(x,z)*180/3.1451296;
	
	float tmp = sqrt(x*x + z*z);
	float angleX = std::atan2(tmp,y)*180/3.1451296;
	angleX -= 270;
	
	turn.Y = -angleY;
	turn.Y+=rand()%180-90;
	turn.X = angleX;
	turn.X+=rand()%180-90;
}

//almost the same as above func
void fighter::moveToPoint(vector3df pos)
{
		vector3df diff = pos - getPos();
		turn.X = diff.getHorizontalAngle().X;
		turn.Y = diff.getHorizontalAngle().Y;

}



//fire torpedo
void fighter::fireMissile()
{
	if(cannon.missile_time < graphics->getTimer()->getTime())
	{
		cannon.missile=true;
		cannon.missile_time = graphics->getTimer()->getTime()+5000;
	}
	else
	{
		cannon.missile=false;
	}
}

void fighter::shoot()
{
	if(cannon.cannon_time < graphics->getTimer()->getTime())
	{
		cannon.cannon = true;
		cannon.cannon_time= graphics->getTimer()->getTime()+400;
	}
	else
		cannon.cannon=false;
}

//really basic functions go ehre
int fighter::getVelocity()
{
	return velocity;
}

void fighter::setVelocity(int velocity)
{
	this->velocity = velocity;
}

int fighter::getMaxVelocity()
{
	return f_type->maxVelocity;
}

ship_faction fighter::getFighterFaction()
{
	return faction;
}

fighter_base *fighter::getFighterClass()
{
	return f_type;
}

bool fighter::getReturnHome()
{
	return return_home;
}

void fighter::setReturnHome(bool value)
{
	return_home = value;
}

void fighter::setTarget(CShip *target)
{
	this->target = target;
}
void fighter::setFighterTarget(fighter *target)
{
	this->fighter_target = target;
}

scene::IAnimatedMeshSceneNode *fighter::getHomeBase()
{
	return home_base;
}

CShip *fighter::getTarget()
{
	if(target!=0)
	{
		return target;
	}
	return 0;
}

fighter *fighter::getFighterTarget()
{
	if(fighter_target!=0)
		return fighter_target;
	return 0;
}

void fighter::setHomeBase(scene::IAnimatedMeshSceneNode *newbase)
{
	//probably wont be used
	home_base = newbase;
}

int fighter::getFuel()
{
	return fuel;
}

int fighter::getHullPoints()
{
	return hullPoints;
}

vector3df fighter::getRot()
{
	return fighter_model->getRotation();
}

vector3df fighter::getPos()
{
	return fighter_model->getPosition();
}

void fighter::setRot(vector3df &newrot)
{
	fighter_model->setRotation(newrot);
}

void fighter::setPos(vector3df &newpos)
{
	fighter_model->setPosition(newpos);
}

bool fighter::getHostileToPlayer()
{
	return hostile;
}