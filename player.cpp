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
#include "player.h"

//implement into damage function
#define EASY 2;
#define MEDIUM 1.25;
#define HARD 1;


//Player object code
//Although it uses alot of the same stuff as the ship object, theres too many differences between the player
//plus i fucked up

Player::Player(irr::IrrlichtDevice *graphics,irrklang::ISoundEngine *sound, CAlertBox *alertBox, KeyListener *receiver, const core::vector3df& ship_position, ship_base *s_class, ShaderCallBack *callback) : 
CObject(L"player"), graphics(graphics), sound(sound), alertBox(alertBox), callback(callback)
{
	//store variables
	this->setID(L"Player");
	
	//setup player variables
	//INHERIT VARIABLES FROM SHIP CLASS
	this->s_class			=	s_class;
	this->hullPoints		=	s_class->hullPoints;
	this->maxHullPoints		=	s_class->hullPoints;
	this->armorPoints		=	s_class->armorPoints;
	this->maxArmorPoints	=	s_class->armorPoints;
	this->shieldPoints		=	s_class->shieldPoints;
	this->maxVelocity		=	s_class->maxVelocity;
	this->maxTurn			=	s_class->maxTurn;
	this->velocity			=	0;
	this->maxShieldPoints	=	shieldPoints;
	this->maxFighters		=	s_class->maxFighters;
	this->numFighters		=	s_class->maxFighters;
	this->f_class			=	fighters().TERR_DRAGONFLY;		//temporary until creation of market
	crew.morale				=	100;
	crew.maxnum				=	s_class->maxCrewSize;
	crew.num				=	crew.maxnum;

	shipDamaged				=	false;


	ship=graphics->getSceneManager()->addAnimatedMeshSceneNode(graphics->getSceneManager()->getMesh(s_class->ship));
	ship->setPosition(ship_position);	//sets the position


	scene::ITriangleSelector* selector = 0;
	selector = graphics->getSceneManager()->createTriangleSelector(ship);
	ship->setTriangleSelector(selector);
	graphics->getSceneManager()->createCollisionResponseAnimator(selector,graphics->getSceneManager()->getActiveCamera());
	selector->drop();

	turn.X = ship->getRotation().X;
	turn.Y = ship->getRotation().Y;
	turn.Z = ship->getRotation().Z;

	exhaust_01 = ship->getJointNode("exhaust1");
	exhaust_02 = ship->getJointNode("exhaust2");
	exhaust_03 = ship->getJointNode("exhaust3");




	last_time = 0;
	fighter_creation_time		=	graphics->getTimer()->getTime();
	fighter_launch_time			=	1000;
	pd_aim=false;
	docked=false;

	cannonFired.primary_time	=	graphics->getTimer()->getTime();
	cannonFired.primary			=	false;
	cannonFired.secondary_time	=	graphics->getTimer()->getTime();
	cannonFired.secondary		=	false;
	cannonFired.light_time		=	graphics->getTimer()->getTime();
	cannonFired.light			=	false;
	cannonFired.flak_time		=	graphics->getTimer()->getTime();

	engineSound = sound->play3D("res/sounds/engine.ogg",getPos(),true,false,true);	
	engineSound->drop();

	shipHum = sound->play2D("res/sounds/internal/hum.wav",true,false,true);
	shipHum->drop();

	//all subsystems have 0-100 health value
	subsystem.bridge			=	100;
	subsystem.deck1				=	100;
	subsystem.deck2				=	100;
	subsystem.elevator			=	100;
	subsystem.engine			=	100;
	subsystem.lightweapons		=	100;
	subsystem.power				=	100;
	subsystem.primaryweapons	=	100;
	subsystem.secondaryweapons	=	100;
	subsystem.shield			=	100;
	subsystem.warpdrive			=	100;


	//some starting money
	money=1000;

	
	//create normalmap material


	s32 bump = video::EMT_SOLID;
	
	bump = graphics->getVideoDriver()->getGPUProgrammingServices()->addHighLevelShaderMaterialFromFiles("shaders/bumpmap.vsh","vertexMain",video::EVST_VS_3_0,"shaders/bumpmap.psh","pixelMain",video::EPST_PS_3_0,callback);

	if(ship)
	{
		//setup player values
		//setup camera collision with player
		scene::ITriangleSelector* selector = 0;
		selector = graphics->getSceneManager()->createTriangleSelector(ship);
		ship->setTriangleSelector(selector);

		scene::ISceneNodeAnimator* anim = graphics->getSceneManager()->createCollisionResponseAnimator(
			selector, graphics->getSceneManager()->getActiveCamera(), core::vector3df(30,30,30),vector3df(0,0,0));
		graphics->getSceneManager()->getActiveCamera()->addAnimator(anim);

		//setup bumpmap

		if(callback->shader_enabled==true)
		{
			video::ITexture *normal_map = graphics->getVideoDriver()->getTexture(s_class->normal_map);
			graphics->getVideoDriver()->makeNormalMapTexture(normal_map,20.f);
			ship->setMaterialTexture(2,normal_map);

			ship->setMaterialType((video::E_MATERIAL_TYPE)bump);
		}


		ship->setScale(s_class->scale);
		ship->setRotation(core::vector3df(0,0,0));
		ship->setMaterialFlag(video::EMF_LIGHTING,true);
		//ship->setMaterialFlag(video::EMF_FOG_ENABLE,true);
		ship->getMaterial(0).Lighting =true;
		ship->getMaterial(0).NormalizeNormals=true;
		//ship->getMaterial(0).Shininess=128;
		ship->getMesh()->setBoundingBox(core::aabbox3df(-300,-200,-300,300,200,300));
	}

	
	//setup particle effects
	//engine exhausts
	scene::IParticleSystemSceneNode *exhaust_ps1=graphics->getSceneManager()->addParticleSystemSceneNode(false,exhaust_01);
	scene::IParticleSystemSceneNode *exhaust_ps2=graphics->getSceneManager()->addParticleSystemSceneNode(false,exhaust_02);
	scene::IParticleSystemSceneNode *exhaust_ps3=graphics->getSceneManager()->addParticleSystemSceneNode(false,exhaust_03);
	scene::IParticleEmitter *em = exhaust_ps1->createBoxEmitter(core::aabbox3d<f32>(-15,-50,-15,15,50,15), core::vector3df(0.0f,0.0f,0.0f),600,1200,video::SColor(0,0,0,0),
                video::SColor(0,255,255,255),       // brightest color
                300,600,0,                         // min and max age, angle
                core::dimension2df(10.f,10.f),         // min size
                core::dimension2df(25.f,25.f));        // max size
	exhaust_ps1->setEmitter(em);
	exhaust_ps2->setEmitter(em);
	exhaust_ps3->setEmitter(em);
	em->drop();



	//attach particle effects
	exhaust_ps1->setMaterialTexture(0,graphics->getVideoDriver()->getTexture("res/textures/engine_trails.pcx"));
	exhaust_ps1->setMaterialType(video::EMT_TRANSPARENT_VERTEX_ALPHA);
	exhaust_ps1->setMaterialFlag(video::EMF_LIGHTING,false);
	exhaust_ps2->setMaterialTexture(0,graphics->getVideoDriver()->getTexture("res/textures/engine_trails.pcx"));
	exhaust_ps2->setMaterialType(video::EMT_TRANSPARENT_VERTEX_ALPHA);
	exhaust_ps2->setMaterialFlag(video::EMF_LIGHTING,false);
	exhaust_ps3->setMaterialTexture(0,graphics->getVideoDriver()->getTexture("res/textures/engine_trails.pcx"));
	exhaust_ps3->setMaterialType(video::EMT_TRANSPARENT_VERTEX_ALPHA);
	exhaust_ps3->setMaterialFlag(video::EMF_LIGHTING,false);

	corona = graphics->getSceneManager()->addBillboardSceneNode(exhaust_01,dimension2d<f32>(300,300),vector3df(0,0,10));
	corona->setMaterialTexture(0,graphics->getVideoDriver()->getTexture("res/textures/engine_corona.png"));
	corona->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);
	corona->setMaterialFlag(video::EMF_LIGHTING, false);
	corona2 = graphics->getSceneManager()->addBillboardSceneNode(exhaust_02,dimension2d<f32>(300,300),vector3df(0,0,10));
	corona2->setMaterialTexture(0,graphics->getVideoDriver()->getTexture("res/textures/engine_corona.png"));
	corona2->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);
	corona2->setMaterialFlag(video::EMF_LIGHTING,false);
	corona3 = graphics->getSceneManager()->addBillboardSceneNode(exhaust_03,dimension2d<f32>(300,300),vector3df(0,0,10));
	corona3->setMaterialTexture(0,graphics->getVideoDriver()->getTexture("res/textures/engine_corona.png"));
	corona3->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);
	corona3->setMaterialFlag(video::EMF_LIGHTING,false);

	//setup turrets
	for(int i = 0; i < s_class->num_turrets; i ++)
	{
		//string conversion n shit
		int temp = i + 1;				
		stringc str("turret_0");		
		str+=temp;						//lets string read each turret bone from the model, must be named turret_01, turret_02, etc.
		char *char_str = new char[9];	//char 9 because the string "turret_0" is 8 characters, and you want the number to fit. so double digit turret numbers will probably crash the game
		strcpy(char_str,str.c_str());

		scene::IBoneSceneNode *bone = ship->getJointNode(char_str);
		turret *t = new turret (graphics,temp,bone, core::vector3df(0,0,0), items().PRI_RAIL ,ship);
		turret_manager.push_back(t);
	}

	//setup secondary turrets
	//mostly the same as above loop
	for(int i = 0; i<s_class->num_secondary_turrets; i++)
	{
		int temp = i+1;
		stringc str("secondary_turret_0");
		str+=temp;
		char *char_str = new char[19];
		strcpy(char_str,str.c_str());

		scene::IBoneSceneNode *bone = ship->getJointNode(char_str);
		turret *t = new turret (graphics,temp,bone,core::vector3df(0,0,0), items().SEC_ANTIMATTER,ship);
		secondary_turret_manager.push_back(t);
	}



	//setup light
	//mostly the same as above loop
	for(int i = 0; i<s_class->num_light_turrets; i++)
	{
		int temp = i+1;
		stringc str("light_turret_0");
		str+=temp;
		char *char_str = new char[15];
		strcpy(char_str,str.c_str());

		scene::IBoneSceneNode *bone = ship->getJointNode(char_str);
		turret *t = new turret (graphics,temp,bone,core::vector3df(0,0,0), items().LIGHT_GATLING,ship);
		light_turret_manager.push_back(t);
	}

	primary_turret = items().PRI_RAIL;
	secondary_turret = items().SEC_ANTIMATTER;
	light_turret = items().LIGHT_GATLING;
	item* pri = new item(items().PRI_RAIL);
	Player::addItemToCargo(pri);
	item* sec = new item(items().SEC_ANTIMATTER);
	addItemToCargo(sec);
	item* light = new item(items().LIGHT_GATLING);
	addItemToCargo(light);

	item* i = new item(items().RATIONS);
	addItemToCargo(i);
}

Player::~Player()
{
	for(int i=0; i<turret_manager.size();i++)
	{
		turret_manager[i]->drop();
		turret_manager.erase(turret_manager.begin()+i);
	}
	turret_manager.clear();

	for(int i=0; i<secondary_turret_manager.size();i++)
	{
		secondary_turret_manager[i]->drop();
		secondary_turret_manager.erase(secondary_turret_manager.begin()+i);
	}
	secondary_turret_manager.clear();
}

void Player::drop()
{
	delete this;
}

float Player::getDistToTarget(CShip* player_target)
{
	float x,y,z;
	x = player_target->getPos().X - getPos().X;
	y = player_target->getPos().Y - getPos().Y;
	z = player_target->getPos().Z - getPos().Z;
	float dist = sqrt(x*x + y*y +z*z);
	return dist;
}

core::vector3df Player::aimTurrets(core::vector3df turret_pos, CShip* player_target)
{
	//This code uses the atan2 func to get the angle between the turret and the target ship
	//to aim properly
	float Y = player_target->getRot().Y;
	float X = -player_target->getRot().X;
	core::vector3df pPos;
	float angleY, angleX, tmp, toTargetX, toTargetY, toTargetZ;
	//float dist = getDistToTarget(player_target);

	//estimate distance in front of target
	//very rough and expensive

	pPos.Y = player_target->getVelocity() * sin(Y * 3.14159/ 180);
	pPos.Y += player_target->getPos().Y;
	pPos.X = player_target->getVelocity() * cos(Y * 3.14159 / 180);
	pPos.X += player_target->getPos().X;
	pPos.Z = player_target->getVelocity() * cos( X *3.14159 / 180 );
	pPos.Z += player_target->getPos().Z;

	toTargetX = (pPos.X - turret_pos.X);
	toTargetZ = (pPos.Z - turret_pos.Z);
	toTargetY = (pPos.Y - turret_pos.Y);

	//toTargetX=(player_target->getPos().X - turret_manager[i]->getPos().X);
	//toTargetZ=(player_target->getPos().Z - turret_manager[i]->getPos().Z);
	toTargetY=(player_target->getPos().Y - turret_pos.Y);


	angleY = std::atan2(toTargetX,toTargetZ)*180/3.1415;


	tmp = sqrt(toTargetX*toTargetX + toTargetZ*toTargetZ);
	angleX = std::atan2(tmp,toTargetY)*180/3.14;
	angleX-=90;


	if(angleX<0)
		angleX+=360;
	if(angleX>360)
		angleX-=360;



	angleX+=rand()%2-1;
	angleY+=rand()%2-1;

	return core::vector3df(angleX,angleY,0);
}

void Player::manageTurrets(CShip* player_target, f32 frameDeltaTime)
{

	//set turret position
	for(unsigned int i = 0; i < turret_manager.size(); i++)
	{
		turret_manager[i]->setPos(turret_manager[i]->getBonePos());
	}
	for(unsigned int i = 0; i<secondary_turret_manager.size();i++)
	{
		secondary_turret_manager[i]->setPos(secondary_turret_manager[i]->getBonePos());
	}
	for(unsigned int i = 0; i<light_turret_manager.size();i++)
	{
		light_turret_manager[i]->setPos(light_turret_manager[i]->getBonePos());
	}

	//calculate angle to target using arctan
	if(player_target!=0)
	{
		if(player_target->getHullPoints()>0)
		{
			//go through all the turrets and aim them
			for(unsigned int i = 0;i< turret_manager.size();i++)
			{
				turret_manager[i]->aimTurret(aimTurrets(turret_manager[i]->getPos(),player_target), frameDeltaTime);
			}
			for(unsigned int i = 0; i < secondary_turret_manager.size(); i++)
			{
				secondary_turret_manager[i]->aimTurret(aimTurrets(secondary_turret_manager[i]->getPos(),player_target),frameDeltaTime);
			}
		}
	}
	else
	{
		//return turrets to standard rotation
		for(unsigned int i = 0;i< turret_manager.size();i++)
		{
			turret_manager[i]->aimTurret(getRot(),frameDeltaTime);
		}
		for(unsigned int i = 0; i < secondary_turret_manager.size(); i++)
		{
			secondary_turret_manager[i]->aimTurret(getRot(),frameDeltaTime);
		}
		

	}
	if(pd_aim!=true)
	{
		for(unsigned int i=0; i< light_turret_manager.size();i++)
		{
			light_turret_manager[i]->aimTurret(getRot(),frameDeltaTime);
		}
	}
}

void Player::aimPd(vector3df& point,f32 frameDeltaTime)
{
	pd_aim=true;
	for(unsigned int i=0; i< light_turret_manager.size();i++)
	{
		vector3df diff = point - light_turret_manager[i]->getPos();
		
		light_turret_manager[i]->aimTurret(diff.getHorizontalAngle(),frameDeltaTime);
	
	}
}

//main player loop, called from gameloop
void Player::playerRun(f32 frameDeltaTime)
{
	//callback->fvLightPosition[0] = graphics->getSceneManager()->getActiveCamera()->getAbsolutePosition();

	scene::ICameraSceneNode *pCam = graphics->getSceneManager()->getActiveCamera();
	float Y = pCam->getPosition().Y-getPos().Y;
	Y=abs(Y/2);
	int trans = abs(getVelocity()) * 2.5;
	trans-=Y;
	if(trans<1)
		trans=0;
	if(trans>255)
		trans=255;
	corona->setColor(SColor(trans,trans,trans,trans));
	corona2->setColor(SColor(trans,trans,trans,trans));
	corona3->setColor(SColor(trans,trans,trans,trans));

	if(armorPoints>0)
	{
		shipDamaged=false;
	}
	if(getHull()>0)
	{

		//if player is not docked at a station
		if(getDocked()!=true)
		{
			engineSound->setPosition(getPos());
			//Shields regenerate 1 point every 0.1 seconds
			if(shieldPoints<maxShieldPoints)
			{
				if(last_time<graphics->getTimer()->getTime())
				{
					shieldPoints+=1;
					last_time = graphics->getTimer()->getTime()+100;	//milliseconds
				}
			}
			//slow player down to nothing if engine is knocked out
			if(subsystem.engine<1)
			{
				setVelocity(getVelocity()/2);
			}
			//player rotation code
			//slowly rotate player toward a point
			//player needs power to be intact in order to turn
			if(subsystem.power>0)
			{
				if(ship->getRotation().Y<turn.Y)	//ship wants to rotate right
				{
					pRot = ship->getRotation();
					rotSlow.Y=0.5*(abs(getRot().Y-turn.Y));	//simulate inertia
					rotSlow.Z=0.5*(abs(getRot().Y-turn.Y));	//simulate inertia
					if(rotSlow.Z>4)
						rotSlow.Z=4;
					if(rotSlow.Y>getMaxTurn())
						rotSlow.Y=getMaxTurn();
					pRot.Z=-rotSlow.Z;
					pRot.Y+=rotSlow.Y*frameDeltaTime;


					setRot(pRot);
				}
				if(ship->getRotation().Y>turn.Y)	//ship wants to rotate left
				{
					pRot = ship->getRotation();
					rotSlow.Y=0.5*(abs(getRot().Y-turn.Y));	//simulate inertia
					rotSlow.Z=0.5*(abs(getRot().Y-turn.Y));	//simulate inertia
					if(rotSlow.Z>4)
						rotSlow.Z=4;
					if(rotSlow.Y>getMaxTurn())
						rotSlow.Y=getMaxTurn();
					pRot.Y-=rotSlow.Y*frameDeltaTime;
					pRot.Z=rotSlow.Z;

					setRot(pRot);
				}
				if(ship->getRotation().X>turn.X)	//turn up
				{
					pRot=ship->getRotation();
					rotSlow.X=0.5*(abs(ship->getRotation().X-turn.X));
					if(rotSlow.X>getMaxTurn())
						rotSlow.X=getMaxTurn();
					pRot.X-=rotSlow.X*frameDeltaTime;
					ship->setRotation(pRot);
				}
				if(ship->getRotation().X<turn.X)	//turn down
				{
					pRot=ship->getRotation();
					rotSlow.X=0.5*(abs(ship->getRotation().X-turn.X));
					if(rotSlow.X>getMaxTurn())
						rotSlow.X=getMaxTurn();
					pRot.X+=rotSlow.X*frameDeltaTime;
					ship->setRotation(pRot);
				}
			}


			//if player has velocity move player in direction
			if(getVelocity()!=0)
			{
				//if(getVelocity()<1000)
				//{
				
					pPos=ship->getAbsolutePosition();
					float i = getRot().Y;
					float z = -(getRot().X);	//if i dont do this the ship doesnt rotate right

					//use trig to determine where the player should move to
					pPos.Y=frameDeltaTime*getVelocity()*(sin(z * 3.14/180));

					pPos.Y+=ship->getAbsolutePosition().Y;
				

					pPos.X=frameDeltaTime*(getVelocity())*(sin(i * 3.14/180));
					pPos.X+=ship->getAbsolutePosition().X;

					pPos.Z=frameDeltaTime*getVelocity()*(cos(i * 3.14/180));
					pPos.Z+=ship->getAbsolutePosition().Z;


					setPos(pPos);
				//}
				//else
				//{
					/*
					pPos=ship->getAbsolutePosition();
					float i = getRot().Y;
					float z = -(getRot().X);	//if i dont do this the ship doesnt rotate right

					//use trig to determine where the player should move to
					pPos.Y=getVelocity()*(sin(z * 3.14/180));

					pPos.Y+=ship->getAbsolutePosition().Y;
				

					pPos.X=(getVelocity())*(sin(i * 3.14/180));
					pPos.X+=ship->getAbsolutePosition().X;

					pPos.Z=getVelocity()*(cos(i * 3.14/180));
					pPos.Z+=ship->getAbsolutePosition().Z;


					setPos(pPos);
				}
				*/
			}

			if(getVelocity()<-10)
			{
				//automatically force the player to speed up
				//if velocity is less than -1
				velocity-=(2+velocity/2)*frameDeltaTime;
			}
		}
		else
		{
			//if player is docked
		}
	}
	else
	{

		//player destroyed
		//hide ship
		ship->setVisible(false);
	}

}

void Player::applyForce(vector3df &rotation, int amount, f32 frameDeltaTime)
{
	vector3df pos = getPos();
	float i = rotation.Y;
	float z = -rotation.X;
	pos.Y=frameDeltaTime*amount*(sin(z * 3.14/180));
	pos.Y+=getPos().Y;

	pos.X=frameDeltaTime*amount*(sin(i * 3.14/180));
	pos.X+=getPos().X;

	pos.Z=frameDeltaTime*amount*(cos(i * 3.14/180));
	pos.Z+=getPos().Z;
	setPos(pos);
}


void Player::damagePlayer(int damage)
{
	//if shield exists, damage shield first
	if(shieldPoints>0)
	{
		shieldPoints-=damage;
	}
	else
	{
		//if armor exists, damage armor with resistance
		if(armorPoints>0)
		{
			armorPoints-=damage/1.15;
		}
		else
		{
			if(hullPoints>0)
			{
				//hull damaged directly?
				//do subsystem damage/kill crewmemebers
				if(crew.num>0)
				{
					crew.num-=rand()%40;
					if(crew.morale>0)
					{
						crew.morale-=rand()%4;
					}
				}
				int i = rand()%9;
				if(i == 1)
				{
					subsystem.deck1-=rand()%10;
					subsystem.deck2-=rand()%10;
					subsystem.elevator-=rand()%10;
				}
				if(i == 2)
				{
					subsystem.engine-=rand()%7;
					subsystem.power-=rand()&7;
					subsystem.shield-=rand()%7;
				}
				if(i == 5)
				{
					subsystem.bridge-=rand()%10;
				}

				if(shipDamaged==false)
				{
					sound->play2D("res/sounds/internal/klaxon.wav",false,false,false);
					alertBox->addText(L"Hull Breached",ALERT);
					shipDamaged=true;
				}
				hullPoints-=damage;
			}
		}
	}
}


//The shooting works the same way as the other ships
//The gamemanager constantly checks the variable if its true
//then creates the projectile in the gamemanager obj
void Player::shoot()
{
	//BUG : if the player times the spacebar release correctly, cannonFired.primary will not get set to false
	//so it never stops firing
	//find new way to call shoot function
	//or reset variables

	if(cannonFired.secondary_time < graphics->getTimer()->getTime())
	{
		if(subsystem.secondaryweapons>0)
		{
			cannonFired.secondary_time = graphics->getTimer()->getTime()+secondary_turret_manager[0]->getReloadTime();
			cannonFired.secondary = true;
		}
	}
	else
	{
		//cannonFired.secondary = false;
	}
	if(cannonFired.primary_time<graphics->getTimer()->getTime())
	{
		//set timer
		if(subsystem.primaryweapons>0)
		{
			cannonFired.primary_time=graphics->getTimer()->getTime()+turret_manager[0]->getReloadTime();
			cannonFired.primary=true;
		}
	}
	else
	{
		//cannonFired.primary=false;
	}


}

void Player::shootPD()
{
	if(cannonFired.light_time<graphics->getTimer()->getTime())
	{
		//set timer
		if(subsystem.lightweapons>0)
		{
			cannonFired.light_time=graphics->getTimer()->getTime()+light_turret_manager[0]->getReloadTime();
			cannonFired.light=true;
		}
	}
	else
	{
		//cannonFired.light=false;
	}
}

void Player::resetCannons()
{
	cannonFired.primary=false;
	cannonFired.secondary=false;
}


void Player::resetPD()
{
	cannonFired.light=false;
}

//loadout functions
//replace stuff
void Player::setShipType(ship_base *type)
{
	//replace all the variables
	this->s_class			=	s_class;
	this->hullPoints		=	s_class->hullPoints;
	this->maxHullPoints		=	s_class->hullPoints;
	this->armorPoints		=	s_class->armorPoints;
	this->maxArmorPoints	=	s_class->armorPoints;
	this->shieldPoints		=	s_class->shieldPoints;
	this->maxVelocity		=	s_class->maxVelocity;
	this->maxTurn			=	s_class->maxTurn;
	this->maxShieldPoints	=	shieldPoints;
	this->maxFighters		=	s_class->maxFighters;
	this->numFighters		=	s_class->maxFighters;

}

void Player::setPrimaryTurret(turret_base *type)
{
	//delete old turrets
	//and create new ones
	primary_turret = type;
	for(unsigned int i=0;i<turret_manager.size();i++)
	{
		turret_manager[i]->setTurret(type);
	}
}

void Player::setSecondaryTurret(turret_base *type)
{
	secondary_turret = type;
	for(unsigned int i=0; i<secondary_turret_manager.size();i++)
	{
		secondary_turret_manager[i]->setTurret(type);
	}
}

//for use by dockmenu
int Player::getNumPrimaryWeaponsInCargo()
{
	int tmp=0;
	for(unsigned int i=0; i<cargo_manager.size();i++)
	{
		if(cargo_manager[i]->getItemType()->type==TYPE_PRIMARY)
		{
			tmp+=1;
		}
	}
	return tmp;
}
//ditto
int Player::getNumSecondaryWeaponsInCargo()
{
	int tmp=0;
	for(unsigned int i=0; i<cargo_manager.size();i++)
	{
		if(cargo_manager[i]->getItemType()->type==TYPE_SECONDARY)
		{
			tmp+=1;
		}
	}
	return tmp;
}



void Player::addFighterCount(int count)
{
	numFighters+=count;
}

void Player::setFighterCount(int count)
{
	numFighters=count;
}

int Player::getMaxFighters()
{
	return maxFighters;
}

int Player::getNumFighters()
{
	return numFighters;
}

void Player::setFighterType(fighter_base *fighter_type)
{
	f_class = fighter_type;
}

fighter_base *Player::getFighterType()
{
	return f_class;
}

int Player::getFighterCreationTime()
{
	return fighter_creation_time;
}

int Player::getFighterLaunchTime()
{
	return fighter_launch_time;
}

void Player::setFighterCreationTime(int time)
{
	fighter_creation_time = time;
}

void Player::setFighterLaunchTime(int time)
{
	fighter_launch_time = time;
}

void Player::loadObject(io::IXMLReader *reader)
{
	//load variables from the xml reader
	vector3df pos;
	vector3df rot;
	pos.X = reader->getAttributeValueAsFloat(L"posX");
	pos.Y = reader->getAttributeValueAsFloat(L"posY");
	pos.Z = reader->getAttributeValueAsFloat(L"posZ");
	rot.X = reader->getAttributeValueAsFloat(L"rotX");
	rot.Y = reader->getAttributeValueAsFloat(L"rotY");
	rot.Z = reader->getAttributeValueAsFloat(L"rotZ");
	//set position and other important variables
	setPos(pos);
	setRot(rot);


	hullPoints = reader->getAttributeValueAsFloat(L"hullPoints");
	shieldPoints = reader->getAttributeValueAsFloat(L"shieldPoints");
	armorPoints = reader->getAttributeValueAsFloat(L"armorPoints");
	velocity = reader->getAttributeValueAsFloat(L"velocity");
}


//loads cargo data from savefile
void Player::loadCargo(io::IXMLReader *reader, int numobjects)
{
	//load cargo from the xml file
	for(int i=0; i<numobjects;i++)
	{
		//get index number
		int tmp = reader->getAttributeValueAsInt(L"index");

		//scan through items array
		for(int l=0; l<items().item_list.size();l++)
		{
			if(tmp==items().item_list[l]->index)
			{
				//found same index
				//add to player
				item *i = new item(items().item_list[l]);
				addItemToCargo(i);
			}
		}
	}
}




//quick functions
stringw Player::floatToString(float num)
{
	stringw t(L"");
	t+=num;
	return t;
}
stringw Player::intToString(int num)
{
	stringw t(L"");
	t+=num;
	return t;
}

//save variables into an xml file
void Player::saveObject(io::IXMLWriter *writer)
{
	//create two arrags that carry all the needed values
	//then save it into the xml
	core::array<stringw> attributes;
	core::array<stringw> values;

	attributes.push_back("hullPoints");
	values.push_back(floatToString(hullPoints));
	attributes.push_back("shieldPoints");
	values.push_back(floatToString(shieldPoints));
	attributes.push_back("armorPoints");
	values.push_back(floatToString(armorPoints));

	attributes.push_back("posX");
	values.push_back(floatToString(getPos().X));
	attributes.push_back("posY");
	values.push_back(floatToString(getPos().Y));
	attributes.push_back("posZ");
	values.push_back(floatToString(getPos().Z));

	attributes.push_back("rotX");
	values.push_back(floatToString(getRot().X));
	attributes.push_back("rotY");
	values.push_back(floatToString(getRot().Y));
	attributes.push_back("rotZ");
	values.push_back(floatToString(getRot().Z));

	attributes.push_back("velocity");
	values.push_back(floatToString(getVelocity()));

	
	writer->writeElement(L"playerStats",false,
		attributes,values);
		
	/*
	writer->writeElement(L"playerStats");
	writer->writeLineBreak();
	player_attributes->addInt("hullPoints",hullPoints);
	player_attributes->addFloat("X",getPos().X);
	player_attributes->addFloat("Y",getPos().Y);
	player_attributes->addFloat("Z",getPos().Z);
	writer->writeClosingTag(L"playerStats");
	*/
}

//save cargo to xml
void Player::saveCargo(io::IXMLWriter *writer)
{
	//create a nested element
	//for the cargo
	//it works by having a different element inside of the playerCargo
	//each element will contain the index value of the item
	//to save and load it
	core::array<stringw> value;
	value.push_back(L"num");
	core::array<stringw> num;
	num.push_back(intToString(cargo_manager.size()));
	//save the amount of items in the array
	//this will be used when using the for loop to load everything
	writer->writeElement(L"playerCargo",true,value,num);

	for(unsigned int i = 0; i<cargo_manager.size();i++)
	{
		core::array<stringw>attributes;
		attributes.push_back(L"index");
		core::array<stringw>values;
		//save index to the element
		values.push_back(intToString(cargo_manager[i]->getItemIndex()));
		//save element containing item index
		writer->writeElement(intToString(i).c_str(),true,attributes,values);
	}

	writer->writeClosingTag(L"playerCargo");
}


//should probably use a template or something for this
//painful to read
//but its alot of the same stuff
core::vector3df Player::getRot() const{return ship->getRotation();}
core::vector3df Player::getPos() const{return ship->getPosition();}
void Player::setPos(const core::vector3df& pPos){ship->setPosition(pPos);}
void Player::setRot(const core::vector3df& pRot){ship->setRotation(pRot);}
int Player::getMaxSpeed(){return this->maxVelocity;}
float Player::getMaxTurn() {return this->maxTurn;}
void Player::setVelocity(float velocity) {this->velocity = velocity;}
float Player::getVelocity() {return this->velocity;}
int Player::getHull() const { return this->hullPoints; }
int Player::getMaxHull() { return this->maxHullPoints; }
int Player::getArmor() const { return this->armorPoints;}
int Player::getMaxArmor() { return this->maxArmorPoints; }
int Player::getShield() const { return this->shieldPoints;}
int Player::getMoney() const { return this->money;}
void Player::setMoney(int newamount) { money = newamount;}
void Player::setDocked(bool state) { docked = state; }
bool Player::getDocked() const { return this->docked; }
ship_base *Player::getCurrentShip() const { return this->s_class; }

//cargo functions
std::vector<item*> Player::getCargo() { return this->cargo_manager; }
void Player::addItemToCargo(item* newitem)
{
	cargo_manager.push_back(newitem);
}
void Player::removeItemFromCargo(item* itemtodelete)
{
	//scanning thru func doesnt work too well
	//so new method is needed
	for(int i=0;i<cargo_manager.size();i++)
	{
		if(itemtodelete==cargo_manager[i])
		{

			//why deleting objects maek crash
			//cargo_manager[i]->drop();
			cargo_manager.erase(cargo_manager.begin()+i);
		}
	}

}

//get functions
turret_base* Player::getPrimaryTurret()
{
	return primary_turret;
}
turret_base* Player::getSecondaryTurret()
{
	return secondary_turret;
}
turret_base* Player::getLightTurret()
{
	return light_turret;
}


void Player::repairArmor()
{
	armorPoints = getMaxArmor();
}

void Player::repairHull()
{
	hullPoints = getMaxHull();
}