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
#include "ship.h"
#include "GFramework.h"



//main ship code
//player object does not inheriet from ship code

CShip::CShip(irr::IrrlichtDevice *graphics,irrklang::ISoundEngine *sound,  const core::vector3df& ship_position, const core::vector3df& ship_rotation,ship_base *s_class,ship_faction faction, const wchar_t *name, ShaderCallBack *callback) : 
CObject(name), graphics(graphics), sound(sound), callback(callback)
{
	//initialize variables for ship
	alive=true;
	this->s_class = s_class;

	this->isStation = s_class->station;
	this->hullPoints =s_class->hullPoints;
	this->shieldPoints = s_class->shieldPoints;
	this->armorPoints = s_class->armorPoints;
	this->maxTurn = s_class->maxTurn;

	this->maxShieldPoints = s_class->shieldPoints;
	this->maxVelocity = s_class->maxVelocity;
	this->numFighters = s_class->maxFighters;
	this->maxFighters = s_class->maxFighters;


	this->faction = faction;
	this->type = type;
	this->energy = s_class->energy;

	//variable initialization
	last_time = graphics->getTimer()->getTime();

	cannon.primary = graphics->getTimer()->getTime();
	cannon.secondary = graphics->getTimer()->getTime();
	cannon.light = graphics->getTimer()->getTime();

	//economy initializiation
	buy_modifier = 1.1;
	sell_modifier = 0.8;
	indemand_modifier = 1.5;
	buy_amount_modifier = 1;


	
	s32 bump = video::EMT_SOLID;
	
	bump = graphics->getVideoDriver()->getGPUProgrammingServices()->addHighLevelShaderMaterialFromFiles("shaders/bumpmap.vsh","vertexMain",video::EVST_VS_3_0,"shaders/bumpmap.psh","pixelMain",video::EPST_PS_3_0,callback);

	ship=graphics->getSceneManager()->addAnimatedMeshSceneNode(graphics->getSceneManager()->getMesh(s_class->ship));
	if(ship)
	{
		/*
		scene::ITriangleSelector* selector = 0;
		selector = graphics->getSceneManager()->createTriangleSelector(ship);
		ship->setTriangleSelector(selector);
		scene::ISceneNodeAnimator* anim = graphics->getSceneManager()->createCollisionResponseAnimator(
			selector, graphics->getSceneManager()->getActiveCamera(), core::vector3df(30,30,30),vector3df(0,0,0));
		graphics->getSceneManager()->getActiveCamera()->addAnimator(anim);
		selector->drop();
		anim->drop();
		*/
		if(callback->shader_enabled==true)
		{
			video::ITexture *normal_map = graphics->getVideoDriver()->getTexture(s_class->normal_map);
			graphics->getVideoDriver()->makeNormalMapTexture(normal_map,20.f);
			ship->setMaterialTexture(2,normal_map);
			ship->setMaterialType((video::E_MATERIAL_TYPE)bump);
		}

		ship->setPosition(ship_position);
		ship->setRotation(ship_rotation);
		ship->setScale(s_class->scale);

		ship->setMaterialFlag(video::EMF_LIGHTING,true);
		ship->getMaterial(0).NormalizeNormals=true;
		ship->getMesh()->setBoundingBox(core::aabbox3df(-400,-300,-400,400,300,400));
	}	

	//setup the initial turn variables
	turn.X = ship->getRotation().X;
	turn.Y = ship->getRotation().Y;
	turn.Z = ship->getRotation().Z;

	fireRot.X = ship->getRotation().X;
	fireRot.Y = ship->getRotation().Y;
	fireRot.Z = ship->getRotation().Z;

	subsystem.engine = 100;
	subsystem.light_weapons	=100;
	subsystem.primary_weapons=100;
	subsystem.secondary_weapons	=100;
	subsystem.warpdrive	=100;


	velocity = 0;

	station_ship_creation_time=0;
	fighter_creation_time=0;

	home_planet=0;

	target=0;

	warp=false;

	state = STATE_SEARCH;



	//temporary until I implement faction warfare
	//so I guess its actually permanent
	if(faction==FACTION_PROVIAN_CONSORTIUM)
		hostile = true;
	if(faction==FACTION_NEUTRAL)
		hostile = false;
	if(faction==FACTION_TERRAN_FEDERATION)
		hostile = false;


	//initialize ship roles
	int r = rand()%3;
	if(r==1)
		setRole(ROLE_ATTACKING);
	if(r==2)
		setRole(ROLE_DEFENDING);
	if(r==3)
		setRole(ROLE_TRADING);




	
	if(s_class->contrails!=false)
	{
		exhaust_01 = ship->getJointNode("exhaust1");
		exhaust_02 = ship->getJointNode("exhaust2");
		exhaust_03 = ship->getJointNode("exhaust3");

		
		//setup particle effects
		scene::IParticleSystemSceneNode *exhaust_ps1=graphics->getSceneManager()->addParticleSystemSceneNode(false,exhaust_01);
		scene::IParticleSystemSceneNode *exhaust_ps2=graphics->getSceneManager()->addParticleSystemSceneNode(false,exhaust_02);
		scene::IParticleSystemSceneNode *exhaust_ps3=graphics->getSceneManager()->addParticleSystemSceneNode(false,exhaust_03);
		scene::IParticleEmitter *em = exhaust_ps1->createBoxEmitter(core::aabbox3d<f32>(-15,-50,-15,15,50,15), // emitter size
					core::vector3df(0.0f,0.0f,0.0f),   // initial direction
					400,800,                             // emit rate
					video::SColor(0,255,255,255),       // darkest color
					video::SColor(0,255,255,255),       // brightest color
					300,500,0,                         // min and max age, angle
					core::dimension2df(10.f,10.f),         // min size
					core::dimension2df(25.f,25.f));        // max size
		exhaust_ps1->setEmitter(em);
		exhaust_ps2->setEmitter(em);
		exhaust_ps3->setEmitter(em);

		em->drop();
		exhaust_ps1->setMaterialTexture(0,graphics->getVideoDriver()->getTexture("res/textures/engine_trails.pcx"));
		exhaust_ps1->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);
		exhaust_ps1->setMaterialFlag(video::EMF_LIGHTING,false);
		exhaust_ps2->setMaterialTexture(0,graphics->getVideoDriver()->getTexture("res/textures/engine_trails.pcx"));
		exhaust_ps2->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);
		exhaust_ps2->setMaterialFlag(video::EMF_LIGHTING,false);
		exhaust_ps3->setMaterialTexture(0,graphics->getVideoDriver()->getTexture("res/textures/engine_trails.pcx"));
		exhaust_ps3->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);
		exhaust_ps3->setMaterialFlag(video::EMF_LIGHTING,false);
	}

	//Create 2d picture over ship
	//to indicate that it is a target
	array_pos = graphics->getSceneManager()->getSceneCollisionManager()->getScreenCoordinatesFrom3DPosition(getPos(),graphics->getSceneManager()->getActiveCamera());
	if(hostile==true)
		target_array = graphics->getGUIEnvironment()->addImage(graphics->getVideoDriver()->getTexture("res/menu/target_array_enemy.png"),array_pos);
	else
		target_array = graphics->getGUIEnvironment()->addImage(graphics->getVideoDriver()->getTexture("res/menu/target_array.png"),array_pos);

	//engine_sound = sound->play3D("res/sounds/engine.ogg",getPos(),true,false,true);


	//setup primary turrets
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
	//secondary
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


}

CShip::~CShip()
{
	ship->setVisible(false);
	//engine_sound->drop();
	velocity=0;
	target_array->remove();
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


//This is the primary run function for AI ships
void CShip::AIRun(f32 frameDeltaTime)
{
	if(hullPoints>0)
	{
		//update 2d picture position
		
		//engine_sound->setPosition(getPos());
		//ensure ships stop being retarded
		if(turn.X > 50)
			turn.X = 50;
		if(turn.X < -50)
			turn.X = -50;

		array_pos = graphics->getSceneManager()->getSceneCollisionManager()->getScreenCoordinatesFrom3DPosition(getPos(),graphics->getSceneManager()->getActiveCamera());
		target_array->setRelativePosition(vector2d<s32>(array_pos.X-32,array_pos.Y-32));
		movement(frameDeltaTime);
		
		//Shields regenerate 1 point every 0.1 seconds
		if(shieldPoints<maxShieldPoints)
		{
			if(last_time<graphics->getTimer()->getTime())
			{
				shieldPoints+=1;
				last_time = graphics->getTimer()->getTime()+100;	//milliseconds
			}
		}

		//manage the ship turrets
		//remember that shots are generated in the gameManager loop
		for(u32 i = 0; i<turret_manager.size();i++)
		{
			turret_manager[i]->setPos(turret_manager[i]->getBonePos());
		}
		for(u32 i = 0; i<secondary_turret_manager.size();i++)
		{
			secondary_turret_manager[i]->setPos(secondary_turret_manager[i]->getBonePos());
		}
		for(u32 i = 0; i<light_turret_manager.size();i++)
		{
			light_turret_manager[i]->setPos(light_turret_manager[i]->getBonePos());
		}

	}
}

//Movement function
//All movement code gets processed here
void CShip::movement(f32 frameDeltaTime)
{

	if(ship->getRotation().Y<turn.Y)	//ship wants to rotate right
	{
		sRot = ship->getRotation();
		rotSlow.Y=0.5*(abs(ship->getRotation().Y-turn.Y));	//simulate inertia

		if(rotSlow.Z>4)
			rotSlow.Z=4;
		if(rotSlow.Y>maxTurn)
			rotSlow.Y=maxTurn;
		sRot.Y+=rotSlow.Y*frameDeltaTime;
		sRot.Z=-rotSlow.Z;
		ship->setRotation(sRot);
	}

	if(ship->getRotation().Y>turn.Y)	//ship wants to rotate left
	{
		sRot = ship->getRotation();
		rotSlow.Y=0.5*(abs(ship->getRotation().Y-turn.Y));	//simulate inertia
		if(rotSlow.Z>4)
			rotSlow.Z=4;
		if(rotSlow.Y>maxTurn)
			rotSlow.Y=maxTurn;
		sRot.Y-=rotSlow.Y*frameDeltaTime;
		sRot.Z=rotSlow.Z;
		ship->setRotation(sRot);
	}
	if(ship->getRotation().X<turn.X)	//ship wants to rotate up
	{
		sRot = ship->getRotation();
		rotSlow.X=0.5*(abs(ship->getRotation().X-turn.X));	//simulate inertia
		if(rotSlow.X>maxTurn)
			rotSlow.X=maxTurn;
		sRot.X+=rotSlow.X*frameDeltaTime;
		ship->setRotation(sRot);
	}
	if(ship->getRotation().X>turn.X)	//ship wants to rotate down
	{
		sRot = ship->getRotation();
		rotSlow.X=0.5*(abs(ship->getRotation().X-turn.X));	//simulate inertia
		if(rotSlow.X>maxTurn)
			rotSlow.X=maxTurn;
		sRot.X-=rotSlow.X*frameDeltaTime;
		ship->setRotation(sRot);
	}
	//only move ship if ship has velocity
	if(subsystem.engine >0)
	{
		if(velocity!=0)
		{
			sPos=getPos();
			float i = ship->getRotation().Y;
			float z = -(ship->getRotation().X);	//if i dont do this the ship doesnt rotate right


			sPos.Y=frameDeltaTime*velocity*(sin(z * 3.14/180));
			sPos.Y+=getPos().Y;

			sPos.X=frameDeltaTime*velocity*(sin(i * 3.14/180));
			sPos.X+=getPos().X;

			sPos.Z=frameDeltaTime*velocity*(cos(i * 3.14/180));
			sPos.Z+=getPos().Z;


			ship->setPosition(sPos);
		}
	}

	if(getVelocity()<-10)
	{
		//automatically force the player to speed up
		//if velocity is less than -1
		velocity+=(2+velocity/2)*frameDeltaTime;
	}

}

//used in ship collisions
//used to push the ship in a specific spot without ruining the rotation or velocity
void CShip::applyForce(vector3df &rotation, int amount, f32 frameDeltaTime)
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


void CShip::damageShip(int damage)
{
	//Damage shields first, then armor (which has resistance) then hull.
	if(shieldPoints>0)
	{
		shieldPoints-=damage;
	}
	else
	{
		if(armorPoints>0)
		{
			armorPoints-=damage/1.15;
		}
		else
		{
			if(hullPoints>0)
			{
				hullPoints-=damage;
			}
		}
	}
}

//rotates the ship to point
void CShip::rotateToPoint(core::vector3df& point)
{
	float x,y,z, angleY,angleX,tmp;
	x = (point.X - getPos().X);
	y = (point.Y - getPos().Y);
	z = (point.Z - getPos().Z);
	angleY = std::atan2(x,z)*180/3.1415296;
	
	tmp = sqrt(x*x + z*z);
	angleX = std::atan2(tmp,y)*180/3.1415296;
	angleX -=90;
	
	turn.Y = angleY;
	turn.X = angleX;

}
//rotate away from the taret
void CShip::rotateAwayFromPoint(core::vector3df& point)
{
	const float x = (point.X - getPos().X);
	const float y = (point.Y - getPos().Y);
	const float z = (point.Z - getPos().Z);
	float angleY = std::atan2(x,z)*180/3.1451296;
	
	float tmp = sqrt(x*x + z*z);
	float angleX = std::atan2(tmp,y)*180/3.1451296;
	angleX -= 270;
	
	turn.Y = -angleY;
	turn.Y+=rand()%20-10;

	//turn.X = -angleX;
	//turn.X += rand()%50-25;

}


//patrol a 30km long area around the point
void CShip::patrolArea(core::vector3df& point)
{
	if(getPos().getDistanceFrom(point)>30000)
	{
		rotateToPoint(point);
	}
	else
	{
		if(last_move<graphics->getTimer()->getTime())
		{
			turn.Y += rand()%720 - 360;
			turn.X += rand()%20-10;
			last_move = graphics->getTimer()->getTime() + rand()%10000;
		}
	}
}

//aim turrets at point
void CShip::shootAtPoint(core::vector3df& point, f32 frameDeltaTime)
{
	for(u32 i = 0; i< turret_manager.size();i++)
	{
		const float x = (point.X - turret_manager[i]->getPos().X);
		const float y = (point.Y - turret_manager[i]->getPos().Y);
		const float z = (point.Z - turret_manager[i]->getPos().Z);
		float angleY = std::atan2(x,z)*180/3.145296;

		float tmp  = sqrt(x*x+z*z);
		float angleX = std::atan2(tmp,y)*180/3.14159265;

		angleX-=90;

	

		//make them less pinpoint accurate
		angleX+=rand()%2-1;
		angleY+=rand()%2-1;

		turret_manager[i]->aimTurret(vector3df(angleX,angleY,0),frameDeltaTime);
	}
	for(u32 i = 0; i< secondary_turret_manager.size();i++)
	{
		const float x = (point.X - secondary_turret_manager[i]->getPos().X);
		const float y = (point.Y - secondary_turret_manager[i]->getPos().Y);
		const float z = (point.Z - secondary_turret_manager[i]->getPos().Z);
		float angleY = std::atan2(x,z)*180/3.145296;

		float tmp  = sqrt(x*x+z*z);
		float angleX = std::atan2(tmp,y)*180/3.14159265;

		angleX-=90;

	

		//make them less pinpoint accurate
		angleX+=rand()%2-1;
		angleY+=rand()%2-1;

		secondary_turret_manager[i]->aimTurret(vector3df(angleX,angleY,0),frameDeltaTime);
	}
}

void CShip::PDatPoint(vector3df& point, f32 frameDeltaTime)
{
	for(u32 i = 0; i< light_turret_manager.size();i++)
	{
		const float x = (point.X - light_turret_manager[i]->getPos().X);
		const float y = (point.Y - light_turret_manager[i]->getPos().Y);
		const float z = (point.Z - light_turret_manager[i]->getPos().Z);
		float angleY = std::atan2(x,z)*180/3.145296;

		float tmp  = sqrt(x*x+z*z);
		float angleX = std::atan2(tmp,y)*180/3.14159265;

		angleX-=90;

	

		//make them less pinpoint accurate
		angleX+=rand()%2-1;
		angleY+=rand()%2-1;

		light_turret_manager[i]->aimTurret(vector3df(angleX,angleY,0),frameDeltaTime);
	}
}

//Creates a shot
//then the gameManager picks up the shot to add to the projectile_manager
projectile *CShip::addShot(core::vector3df& pos)
{
	irrklang::ISound *shot = sound->play3D("res/sounds/photon.wav",getPos(),false,false,false);
	projectile *p = new photonCannonShot(graphics, pos, core::vector3df(fireRot.X,fireRot.Y,fireRot.Z),ship);
	return p;

}

//The gameManager uses this to see if the ship wants to shoot
//if the ship wants to shoot, it returns true
//which then the gameManager creates a bullet
void CShip::ableToShoot()
{
	if(subsystem.primary_weapons>0)
	{
		if(cannon.primary<graphics->getTimer()->getTime())
		{
			//BUG : WHEN THE TURRET_MANAGER[0] IS USED, SHIPS THAT DONT HAVE A PRIMARY TURRT
			//AKA STATIONS MAKE THE GAME CRASH
			//fixed
			if(s_class->num_turrets>0)
			{
				cannon.primary = graphics->getTimer()->getTime()+turret_manager[0]->getReloadTime();
				cannon.primary_shoot=true;
			}
		}
		else
		{
			//cannon.primary_shoot=false;
		}
	}
	else
		cannon.primary=false;

	if(subsystem.secondary_weapons>0)
	{
		if(cannon.secondary<graphics->getTimer()->getTime())
		{
			if(s_class->num_secondary_turrets>0)
			{
				cannon.secondary = graphics->getTimer()->getTime()+secondary_turret_manager[0]->getReloadTime();
				cannon.secondary_shoot=true;
			}
		}
		else
		{
			//cannon.secondary_shoot=false;
		}
	}
	else
		cannon.secondary=false;
}

void CShip::pdShoot()
{
	if(subsystem.light_weapons>0)
	{
		if(cannon.light<graphics->getTimer()->getTime())
		{
			if(s_class->num_light_turrets>0)
			{
				cannon.light = graphics->getTimer()->getTime()+light_turret_manager[0]->getReloadTime();
				cannon.light_shoot=true;
			}
		}
		else
		{
			//cannon.light_shoot=false;
		}
	}
}

//set cannons to false
void CShip::resetCannons()
{
	cannon.primary_shoot=false;
	cannon.secondary_shoot=false;
	
}

void CShip::resetPD()
{
	cannon.light_shoot = false;
}

//player engagement code
//although, it can be easily used to engage other ships
void CShip::engagePlayer(core::vector3df& playerpos, f32 frameDeltaTime)
{
	shootAtPoint(playerpos,frameDeltaTime);
	ableToShoot();
	dodgeFire();
	if(getDistToPoint(playerpos)>5000) //outside effective range....
	{
		rotateToPoint(playerpos);
	}
	else
	{
		if(getDistToPoint(playerpos)<3000) //too close...
		{
			rotateAwayFromPoint(playerpos);
		}
	}

}

//engages current target
void CShip::engageCurrentTarget(f32 frameDeltaTime)
{
	if(target!=0 && target->getHullPoints()>0)
	{

		float Y = target->getRot().Y;
		float X = -target->getRot().X;
		core::vector3df pPos;
		//pPos.Y = target->getVelocity() * sin(Y * 3.1415/ 180);
		pPos.Y = target->getPos().Y;
		pPos.X = target->getVelocity() * cos(Y *3.1415/180);
		pPos.X += target->getPos().X;
		pPos.Z = target->getVelocity() * cos( X *3.1415 / 180 );
		pPos.Z += target->getPos().Z;
		shootAtPoint(pPos,frameDeltaTime);
		if(getDistToPoint(target->getPos())<5000)
			ableToShoot();
		else
			resetCannons();
		dodgeFire();
		if(getDistToPoint(target->getPos())>5000) //outside effective range....
		{
			rotateToPoint(target->getPos());
		}
		else
		{
			if(getDistToPoint(target->getPos())<3000) //too close...
			{
				rotateAwayFromPoint(target->getPos());
			}
		}
		
	}

}

//returns distance to point, no biggie
float CShip::getDistToPoint(core::vector3df& point)
{
	float x = getPos().X - point.X;
	float y = getPos().Y - point.Y;
	float z = getPos().Z - point.Z;

	float dist = sqrt(x*x+y*y+z*z);
	return dist;
}

//Makes ships slightly less retarded and less predictable
void CShip::dodgeFire()
{
	velocity=maxVelocity;
	if(last_move<graphics->getTimer()->getTime())
	{
		turn.Y += rand()%720 - 360;
		turn.X += rand()%40-20;
		last_move = graphics->getTimer()->getTime() + rand()%10000;
	}
}


void CShip::warpToPlanet(planet* target_planet)
{
	//safety
	if(subsystem.warpdrive > 0)
	{
		if(target_planet != 0)
		{
			if(getWarping()!=true)
			{
				//basic rotation to point code
				rotateToPoint(target_planet->getPos());

				setWarping(true);
			}
		
			else
			{
				//if warping is true
				if(getRot().Y+3>turn.Y && getRot().Y-3<turn.Y && getRot().X+3>turn.X && getRot().X-3 < turn.X)
				{
					if(getPos().getDistanceFrom(target_planet->getPos())>SHIP_WARP_DISTANCE+rand()%1000-500)
					{
						setVelocity(SHIP_WARP_SPEED);
					}
					else
					{
						setVelocity(0);
						setWarping(false);
					}
				}
			}
		}
	}
}

void CShip::setState(AI_STATE state)
{
	this->state = state;
}

AI_STATE CShip::getState()
{
	return state;
}

int CShip::getStarshipCreationTime()
{
	return this->station_ship_creation_time;
}

void CShip::setStarshipCreationTime(int time)
{
	station_ship_creation_time = time;
}

int CShip::getFighterCreationTime()
{
	return fighter_creation_time;
}

void CShip::setFighterCreationTime(int time)
{
	fighter_creation_time = time;
}

bool CShip::getWarping()
{
	return warp;
}
void CShip::setWarping(bool warp)
{
	this->warp = warp;
}
void CShip::setTargetArrayVisible(bool visible)
{
	target_array->setVisible(visible);
}

void CShip::setHomePlanet(planet *homeplanet)
{
	home_planet = homeplanet;
}
planet *CShip::getHomePlanet()
{
	if(home_planet!=0)
		return home_planet;
	return 0;
}

//Various return funcs
bool CShip::getWithinRadarRange()
{
	return target_array->isVisible();
}
bool CShip::getIsStation() { return this->isStation; }
void CShip::setVisible(bool visible) { ship->setVisible(visible); }
void CShip::setVelocity(float newvelocity) { velocity = newvelocity; }
int CShip::getHullPoints() const { return this->hullPoints; }
int CShip::getArmorPoints() const { return this->armorPoints; }
int CShip::getShieldPoints() const { return this->shieldPoints; }
bool CShip::getHostileToPlayer() const { return this->hostile; }
int CShip::getNumFighters()
{
	return numFighters;
}
void CShip::setNumFighters(int fighters)
{
	numFighters = fighters;
}
void CShip::modNumFighters(int fighters)
{
	numFighters +=fighters;
}
void CShip::drop()
{
	alive=false;
	delete this;

}
core::vector3df CShip::getPos() const
{
	return ship->getPosition();
}
void CShip::setPos(vector3df newpos)
{
	this->ship->setPosition(newpos);
}
core::vector3df CShip::getRot() const
{
	return ship->getRotation();
}
void CShip::setRot(vector3df newrot)
{
	this->ship->setRotation(newrot);
}
void CShip::setHullPoints(int hullpoints)
{
	this->hullPoints = hullpoints;
}
void CShip::setShieldPoints(int shieldpoints)
{
	this->shieldPoints = shieldpoints;
}
void CShip::setArmorPoints(int armorpoints)
{
	this->armorPoints = armorpoints;
}

ship_base *CShip::getShipClass()
{
	return s_class;
}
ship_faction CShip::getShipFaction()
{
	return faction;
}
float CShip::getVelocity()
{
	return velocity;
}
float CShip::getMaxVelocity()
{
	return this->maxVelocity;
}
int CShip::getEnergy() const
{
	return energy;
}
void CShip::setTarget(CShip *target)
{
	this->target = target;
}

CShip *CShip::getTarget()
{
	//return 0 if there is no target
	if(target!=0)
	{
		return target;
	}
	return 0;
}

vector2d<int> CShip::getArrayPos()
{
	return this->array_pos;
}

std::vector<turret*> CShip::getTurretManager()
{
	return this->turret_manager;
}
std::vector<turret*> CShip::getSecondaryTurretManager()
{
	return this->secondary_turret_manager;
}
std::vector<turret*> CShip::getLightTurretManager()
{
	return this->light_turret_manager;
}
std::vector<item*> CShip::getInventory()
{
	return this->inventory;
}
void CShip::setInventory(std::vector<item*> in)
{
	this->inventory = in;
}
//add item to inventory array
void CShip::addItemToInventory(item* add)
{
	inventory.push_back(add);
}
//check if item is in the inventory of the ship
bool CShip::itemInInventory(item *check)
{
	for(unsigned int i=0; i<inventory.size();i++)
	{
		//same item index means same item
		//ish
		if(check->getItemIndex()==inventory[i]->getItemIndex())
		{
			return true;
		}
	}
	return false;
}

float CShip::getModifierBuy()
{
	return buy_modifier;
}
float CShip::getModifierSell()
{
	return sell_modifier;
}
float CShip::getModifierInDemand()
{
	return indemand_modifier;
}
float CShip::getModifierBuyAmount()
{
	return buy_amount_modifier;
}

void CShip::setRole(SHIP_ROLE role)
{
	this->role = role;
}

SHIP_ROLE CShip::getRole()
{
	return role;
}