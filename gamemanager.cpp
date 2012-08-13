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
#include "gamemanager.h"
#include "GFramework.h"




#define SELECTDISTANCE 32
#define CARGODISTANCE 500
#define SHOOTTIME 600
#define THINKTIME 100



//constructor
gameManager::gameManager(irr::IrrlichtDevice *graphics, KeyListener *receiver, irrklang::ISoundEngine *sound)
{

	//save vaariables for later use
	this->graphics=graphics;
	this->receiver = receiver;
	this->sound = sound;
	pd_think_time = graphics->getTimer()->getTime();
	playerDead=false;
	think_time = 0;
}

//super nuke
gameManager::~gameManager()
{
	//nuke function
	//delete everything
	for(int i=0; i<ship_manager.size(); i++)
	{
		ship_manager[i]->drop();
		ship_manager.erase(ship_manager.begin()+i);
	}
	ship_manager.clear();
	for(int i=0; i<fighter_manager.size(); i++)
	{
		fighter_manager[i]->drop();
		fighter_manager.erase(fighter_manager.begin()+i);
	}
	fighter_manager.clear();
	for(int i=0; i<projectile_manager.size(); i++)
	{
		projectile_manager[i]->drop();
		projectile_manager.erase(projectile_manager.begin()+i);
	}
	projectile_manager.clear();
	for(int i=0; i<pd_manager.size(); i++)
	{
		pd_manager[i]->drop();
		pd_manager.erase(pd_manager.begin()+i);
	}
	pd_manager.clear();
	for(int i=0; i<planet_manager.size(); i++)
	{
		planet_manager[i]->drop();
		planet_manager.erase(planet_manager.begin()+i);
	}
	planet_manager.clear();
	for(int i=0; i<cargo_manager.size(); i++)
	{
		cargo_manager[i]->drop();
		cargo_manager.erase(cargo_manager.begin()+i);
	}
	cargo_manager.clear();

}

void gameManager::drop()
{
	delete this;
}


//----------------------------------------------------------------------------------------------
//Insert AI functions here
//Call from gamemanager loop
CShip *gameManager::getClosestShip(CShip *ship)
{
	core::vector3df pos = ship->getPos();
	//unsigned for double the spaec
	for(unsigned int i = 0; i<ship_manager.size(); i++)
	{
		if(pos.getDistanceFrom(ship_manager[i]->getPos())<7000)
		{
			return ship_manager[i];
		}
	}

	return 0;
}


CShip *gameManager::getClosestAlliedShip(CShip *ship)
{
	core::vector3df pos = ship->getPos();
	for(unsigned int i = 0; i < ship_manager.size(); i ++)
	{
		//make sure it cant attack ships that are neutral
		if(ship_manager[i]->getShipFaction()!=FACTION_NEUTRAL)
		{
			if(ship_manager[i]->getShipFaction()==ship->getShipFaction())
			{
				if(pos.getDistanceFrom(ship_manager[i]->getPos())<7000)
				{
					return ship_manager[i];
				}
			}
		}
	}

	return 0;
}

//gets closest enemy ship within 7km
CShip *gameManager::getClosestEnemyShip(CShip *ship)
{
	core::vector3df pos = ship->getPos();
	//scan the ship array
	for(unsigned int i = 0; i < ship_manager.size(); i ++)
	{
		//make sure it cant attack ships that are neutral
		if(ship_manager[i]->getShipFaction()!=FACTION_NEUTRAL)
		{
			if(ship_manager[i]->getShipFaction()!=ship->getShipFaction())
			{
				if(pos.getDistanceFrom(ship_manager[i]->getPos())<7000)
				{
					if(ship_manager[i]->getHullPoints()>0)
						return ship_manager[i];
				}
			}
		}
	}

	return 0;
}


//function to determine the closest hostile station
CShip *gameManager::getClosestEnemyStation(CShip *ship)
{
	core::vector3df pos = ship->getPos();
	//temporary variables
	int min_value_so_far = 0;
	int min_value_temp = 0;
	CShip *temp = 0;

	for(unsigned int i = 0; i<station_manager.size(); i++)
	{
		if(station_manager[i]->getShipFaction()!=FACTION_NEUTRAL)
		{
			if(station_manager[i]->getShipFaction()!=ship->getShipFaction())
			{
				//stores the distance from current station
				min_value_temp = pos.getDistanceFrom(station_manager[i]->getPos());

				//if the i variable hits station manager size (-1 of course, since the count starts at 0)
				if(i = station_manager.size()-1)
				{
					return temp;
				}
				else
				{
					//initializer
					if(min_value_so_far = 0)
					{
						min_value_so_far = min_value_temp;
						temp = station_manager[i];
					}
					else
					{
						//if the value is smaller than the other ones
						//store until another smaller variable appears.
						if(min_value_temp < min_value_so_far)
						{
							min_value_so_far = min_value_temp;
							temp = station_manager[i];
						}
					}
				}
			}
		}
	}
}

CShip *gameManager::getClosestEnemyShipFighter(fighter *f)
{
	core::vector3df pos = f->getPos();
	//scan the ship array
	for(unsigned int i = 0; i < ship_manager.size(); i ++)
	{
		//make sure it cant attack ships that are neutral
		if(ship_manager[i]->getShipFaction()!=FACTION_NEUTRAL)
		{
			if(ship_manager[i]->getShipFaction()!=f->getFighterFaction())
			{
				if(pos.getDistanceFrom(ship_manager[i]->getPos())<7000)
				{
					return ship_manager[i];
				}
			}
		}
	}

	return 0;
}

//function to determine the closest hostile station
planet *gameManager::getClosestEnemyPlanet(CShip *ship)
{
	core::vector3df pos = ship->getPos();
	//temporary variables
	int min_value_so_far = 0;
	planet *temp = 0;
	for(unsigned int i = 0; i<planet_manager.size(); i++)
	{
		if(planet_manager[i]->getFaction()!=FACTION_NEUTRAL)
		{
			if(planet_manager[i]->getFaction()!= ship->getShipFaction())
			{
				if(min_value_so_far == 0)
				{
					min_value_so_far = pos.getDistanceFrom(planet_manager[i]->getPos());
					temp = planet_manager[i];
				}
				else
				{
					if(pos.getDistanceFrom(planet_manager[i]->getPos()) < min_value_so_far)
					{
						min_value_so_far = pos.getDistanceFrom(planet_manager[i]->getPos());
						temp = planet_manager[i];
					}
				}
			}
		}
	}
	return temp;
}

//same as above function except that it searches for allied planets
planet *gameManager::getClosestAlliedPlanet(CShip *ship)
{
	core::vector3df pos = ship->getPos();
	int min_value_so_far = 0;
	planet *temp = 0;
	for(unsigned int i = 0; i<planet_manager.size(); i++)
	{
		if(planet_manager[i]->getFaction()!=FACTION_NEUTRAL)
		{
			if(planet_manager[i]->getFaction()== ship->getShipFaction())
			{
				if(min_value_so_far == 0)
				{
					min_value_so_far = pos.getDistanceFrom(planet_manager[i]->getPos());
					temp = planet_manager[i];
				}
				else
				{
					if(pos.getDistanceFrom(planet_manager[i]->getPos()) < min_value_so_far)
					{
						min_value_so_far = pos.getDistanceFrom(planet_manager[i]->getPos());
						temp = planet_manager[i];
					}
				}
			}
		}
	}
	return temp;
}

fighter *gameManager::getClosestEnemyFighter(fighter *f)
{
	core::vector3df pos = f->getPos();
	//scan the ship array
	for(unsigned int i = 0; i < fighter_manager.size(); i ++)
	{
		//make sure it cant attack ships that are neutral
		if(fighter_manager[i]->getFighterFaction()!=FACTION_NEUTRAL)
		{
			if(fighter_manager[i]->getFighterFaction()!=f->getFighterFaction())
			{
				if(pos.getDistanceFrom(fighter_manager[i]->getPos())<1000)
				{
					return fighter_manager[i];
				}
			}
		}
	}

	return 0;
}

fighter *gameManager::getClosestEnemyFighterShip(CShip *ship)
{
	core::vector3df pos = ship->getPos();
	//scan the ship array
	for(unsigned int i = 0; i < fighter_manager.size(); i ++)
	{
		//make sure it cant attack ships that are neutral
		if(fighter_manager[i]->getFighterFaction()!=FACTION_NEUTRAL)
		{
			if(fighter_manager[i]->getFighterFaction()!=ship->getShipFaction())
			{
				if(pos.getDistanceFrom(fighter_manager[i]->getPos())<1000)
				{
					return fighter_manager[i];
				}
			}
		}
	}

	return 0;
}

void gameManager::searchForTarget(CShip *ship, Player *CPlayer, f32 frameDeltaTime)
{
	//if player is really close, he takes priority
	//other than that, other ai ships get shot at first
	if(ship->getWarping()==false)
	{
		if(getClosestEnemyShip(ship)!=0 || ship->getPos().getDistanceFrom(CPlayer->getPos()) < 7000)
		{
			if(ship->getHostileToPlayer()==true)
			{
				if(ship->getPos().getDistanceFrom(CPlayer->getPos())<7000)
				{
					//attack player
					//since player isnt actually of CShip object, this has to occur
					ship->engagePlayer(aimAtPlayer(CPlayer),frameDeltaTime);
				}
				else
				{
					ship->setTarget(getClosestEnemyShip(ship));
				}
			}
			else
			{
				ship->setTarget(getClosestEnemyShip(ship));
			}
		}
	}
}



//i'm kind of stupid and made ship and gamemanager function named the same
void gameManager::engageCurrentTarget(CShip *ship, Player *CPlayer, f32 frameDeltaTime)
{
	//ai has target
	//engage
	if(ship->getTarget()->ship)
	{
		if(ship->getHostileToPlayer()==true)
		{
			//if player is closer than the current target
			//engage the player
			if(ship->getPos().getDistanceFrom(CPlayer->getPos())<ship->getPos().getDistanceFrom(ship->getTarget()->getPos()))
			{
				ship->engagePlayer(aimAtPlayer(CPlayer),frameDeltaTime);
				if(ship->getNumFighters()>0)
				{
					//make timer for fighter creation
					if(ship->getFighterCreationTime() < graphics->getTimer()->getTime())
					{
						if(ship->getShipFaction()==FACTION_PROVIAN_CONSORTIUM)
						{
							vector3df pos = ship->getPos();
							pos+=vector3df(rand()%100-50,rand()%100-50,rand()%100-50);
							fighter *f = new fighter(graphics,sound,ship->getPos(),ship->getRot(),fighters().PROV_RAPTOR,ship->getShipFaction(),ship->ship);
							f->moveToPoint(CPlayer->getPos());
							addFighter(f);
							ship->modNumFighters(-1);
							ship->setFighterCreationTime(graphics->getTimer()->getTime()+1000);
						}
					}
				}
			}
			else
			{
				ship->engageCurrentTarget(frameDeltaTime);
			}
		}
		else
		{
			ship->engageCurrentTarget(frameDeltaTime);
		}
	}
	else
	{
		//if current target died
		//set current target to zero
		ship->setTarget(0);
	}
}

//makes turrets be able to aim at the player
core::vector3df gameManager::aimAtPlayer(Player *CPlayer)
{
	float Y = CPlayer->getRot().Y;
	float X = -CPlayer->getRot().X;
	core::vector3df pPos;
	//pPos.Y = CPlayer->getVelocity() * sin(Y * 3.1415/ 180);
	pPos.Y = CPlayer->getPos().Y;
	pPos.X = CPlayer->getVelocity() * cos(Y *3.1415/180);
	pPos.X += CPlayer->getPos().X;
	pPos.Z = CPlayer->getVelocity() * cos( X *3.1415 / 180 );
	pPos.Z += CPlayer->getPos().Z;
	return pPos;
}
//end ai funcs
//-----------------------------------------------------------------------------------------------------------

//monster func
//controls ships
void gameManager::gameManagerLoop(f32 frameDeltaTime, irrklang::ISoundEngine *sound, Player* CPlayer, CAlertBox *alertBox, ShaderCallBack *callback)
{

	//AI ship loop
	//Controls the ship AI
	//
	//Heres how the projectile shooting works.
	//Its not the best way to do it I'm sure
	//In order to have projectiles be able to collide with other objects without resorting to the inbuild irrlicht collision system because I don't understand it
	//You need to create the projectile in the gameManager class so it can be added to the projectile array
	//Since there is no way to send objects 'up' a heirachy level, the gameManager class needs to be constantly checking the ship classes if it wants to shoot
	//If the ship wants to shoot, the ship returns true for the primary_shoot variable
	//The ship itself does not create the projectile, the gameManager class does
	//So the gameManager object constantly scans each ship object to see if it wants to shoot, and then creates the projectile in a higher hierchy
	//Not the best way to do it, but I can't figure out any other ways.
	//
	//Also, the ship_manager loop MUST go before the projectile manager loop

	for(unsigned int i=0;i<ship_manager.size();i++)
	{
		if(ship_manager[i]->getHullPoints()>0)
		{
			

			
			ship_manager[i]->AIRun(frameDeltaTime);
			
			core::vector3df pos = ship_manager[i]->getPos();

			//if ship is over 10km away from the player, it is out of range
			
			if(pos.getDistanceFrom(CPlayer->getPos())>10000)
			{
				ship_manager[i]->setTargetArrayVisible(false);
			}
			else
			{
				//make sure that player cannot target ships outside of range
				if(showShips != false)
					ship_manager[i]->setTargetArrayVisible(true);
				else
					ship_manager[i]->setTargetArrayVisible(false);
			}
		

			//hides ships when they are 50km away from the camera
			//otherwise, make them visible
			if(pos.getDistanceFrom(graphics->getSceneManager()->getActiveCamera()->getPosition())>CAMERA_HIDE_DISTANCE)
			{
				ship_manager[i]->setVisible(false);
			}
			else
			{
				ship_manager[i]->setVisible(true);
			}

			
			//BEGIN AI FUNCTIONS 
			/////////////////////////////////////////////////////////
			if(ship_manager[i]->getHullPoints() <800)
			{
				ship_manager[i]->setState(STATE_FLEE);
			}

			//target search
			//make sure ship doesnt have a target first
			//before searching
			if(ship_manager[i]->getState() == STATE_SEARCH)
			{
				//having problems with ships shooting wreckages

				//ship_manager[i]->resetCannons();
				if(ship_manager[i]->getTarget()==0)
				{
					if(pos.getDistanceFrom(CPlayer->getPos())>6000 || ship_manager[i]->getHostileToPlayer()==false)
						ship_manager[i]->resetCannons();
					//ship_manager[i]->resetCannons();
					//search if ship has no current target
					if(ship_manager[i]->getShipFaction()!=FACTION_NEUTRAL)
					{
						searchForTarget(ship_manager[i],CPlayer,frameDeltaTime);
						
						ship_manager[i]->setVelocity(ship_manager[i]->getMaxVelocity()/2);
						if(getClosestAlliedPlanet(ship_manager[i])->getPos().getDistanceFrom(pos) < 30000)
						{
							ship_manager[i]->patrolArea(getClosestAlliedPlanet(ship_manager[i])->getPos());
						}
						
					}

					//ai roles come into effect here
					
				}
				else
				{
					//if it has a target, shoot at it
					if(ship_manager[i]->getTarget()->getHullPoints()>0)
					{
						if(pos.getDistanceFrom(ship_manager[i]->getTarget()->getPos())<10000)
							engageCurrentTarget(ship_manager[i],CPlayer,frameDeltaTime);
						else
							ship_manager[i]->setTarget(0);
					}
					else
					{
						ship_manager[i]->setTarget(0);
					}

					if(ship_manager[i]->getNumFighters()>0)
					{
						//make timer for fighter creation
						if(ship_manager[i]->getFighterCreationTime() < graphics->getTimer()->getTime())
						{
							if(ship_manager[i]->getShipFaction()==FACTION_TERRAN_FEDERATION)
							{
								vector3df pos = ship_manager[i]->getPos();
								pos+=vector3df(rand()%100-50,rand()%100-50,rand()%100-50);
								fighter *f = new fighter(graphics,sound,ship_manager[i]->getPos(),ship_manager[i]->getRot(),fighters().TERR_DRAGONFLY,ship_manager[i]->getShipFaction(),ship_manager[i]->ship);
								f->setTarget(getClosestEnemyShip(ship_manager[i]));
								addFighter(f);
								ship_manager[i]->modNumFighters(-1);
								ship_manager[i]->setFighterCreationTime(graphics->getTimer()->getTime()+1000);
							}
							if(ship_manager[i]->getShipFaction()==FACTION_PROVIAN_CONSORTIUM)
							{
								vector3df pos = ship_manager[i]->getPos();
								pos+=vector3df(rand()%100-50,rand()%100-50,rand()%100-50);
								fighter *f = new fighter(graphics,sound,ship_manager[i]->getPos(),ship_manager[i]->getRot(),fighters().PROV_RAPTOR,ship_manager[i]->getShipFaction(),ship_manager[i]->ship);
								f->setTarget(getClosestEnemyShip(ship_manager[i]));
								addFighter(f);
								ship_manager[i]->modNumFighters(-1);
								ship_manager[i]->setFighterCreationTime(graphics->getTimer()->getTime()+1000);
							}
						}
					}
				}
			}
			if(ship_manager[i]->getState() == STATE_FLEE)
			{
				ship_manager[i]->resetCannons();
				//warp to closest allied planet if it is farther than warp dist
				if(getClosestAlliedPlanet(ship_manager[i])->getPos().getDistanceFrom(pos) > SHIP_WARP_DISTANCE)
				{
					ship_manager[i]->warpToPlanet(getClosestAlliedPlanet(ship_manager[i]));
				}
			}

			if(ship_manager[i]->getState()== STATE_ATTACK)
			{
			}
			//use point defenses
			if(getClosestEnemyFighterShip(ship_manager[i])!=0)
			{
				ship_manager[i]->PDatPoint(getClosestEnemyFighterShip(ship_manager[i])->getPos(),frameDeltaTime);
				ship_manager[i]->pdShoot();
				if(ship_manager[i]->getNumFighters()>0)
				{
					//make timer for fighter creation
					if(ship_manager[i]->getFighterCreationTime() < graphics->getTimer()->getTime())
					{
						if(ship_manager[i]->getShipFaction()==FACTION_TERRAN_FEDERATION)
						{
							vector3df pos = ship_manager[i]->getPos();
							pos+=vector3df(rand()%100-50,rand()%100-50,rand()%100-50);
							fighter *f = new fighter(graphics,sound,ship_manager[i]->getPos(),ship_manager[i]->getRot(),fighters().TERR_DRAGONFLY,ship_manager[i]->getShipFaction(),ship_manager[i]->ship);
							f->setFighterTarget(getClosestEnemyFighterShip(ship_manager[i]));
							addFighter(f);
							ship_manager[i]->modNumFighters(-1);
							ship_manager[i]->setFighterCreationTime(graphics->getTimer()->getTime()+1000);
						}
						if(ship_manager[i]->getShipFaction()==FACTION_PROVIAN_CONSORTIUM)
						{
							vector3df pos = ship_manager[i]->getPos();
							pos+=vector3df(rand()%100-50,rand()%100-50,rand()%100-50);
							fighter *f = new fighter(graphics,sound,ship_manager[i]->getPos(),ship_manager[i]->getRot(),fighters().PROV_RAPTOR,ship_manager[i]->getShipFaction(),ship_manager[i]->ship);
							f->setFighterTarget(getClosestEnemyFighterShip(ship_manager[i]));
							addFighter(f);
							ship_manager[i]->modNumFighters(-1);
							ship_manager[i]->setFighterCreationTime(graphics->getTimer()->getTime()+1000);
						}
					}
				}
			}
			else
				ship_manager[i]->resetPD();
			//END AI FUNCS
			///////////////////////////

			//ship collision funcs
			//basic bounding box collision
			//nothin fancy
			for(unsigned int l=i+1; l<ship_manager.size();l++)
			{
				//cant crash with oneself
				if(ship_manager[l]!=ship_manager[i])
				{
					if(ship_manager[i]->ship->getTransformedBoundingBox().intersectsWithBox(ship_manager[l]->ship->getTransformedBoundingBox()))
					{
						//collision
						vector3df pos = ship_manager[l]->getPos()-ship_manager[i]->getPos();
						vector3df rot = -(pos.getHorizontalAngle());
						ship_manager[i]->applyForce(rot,ship_manager[i]->getVelocity(),frameDeltaTime);
					}
				}
			}
			if(ship_manager[i]->ship->getTransformedBoundingBox().intersectsWithBox(CPlayer->ship->getTransformedBoundingBox()))
			{
				//collision
				vector3df pos = CPlayer->getPos()-ship_manager[i]->getPos();
				vector3df rot = pos.getHorizontalAngle();
				ship_manager[i]->applyForce(-rot,ship_manager[i]->getVelocity(),frameDeltaTime);
				CPlayer->applyForce(rot,CPlayer->getVelocity(),frameDeltaTime);
			}
			//if ship wants to shoot
			//shoot primary turrets
			//havent implemented secondary turrets for ai yet
			if(ship_manager[i]->cannon.light_shoot==true)
			{
				for(unsigned int t = 0; t<ship_manager[i]->getLightTurretManager().size();t++)
				{
					if(rand()%ship_manager[i]->getLightTurretManager()[t]->getReloadTime()<SHOOTTIME*frameDeltaTime)
					{
					sound->play3D("res/sounds/weapons/gatling.wav",ship_manager[i]->getPos(),false,false,false);
					projectile *p  = new gatlingBullet(graphics, ship_manager[i]->getLightTurretManager()[t]->getPos(), ship_manager[i]->getLightTurretManager()[t]->getRot(),CPlayer->ship);
					pd_manager.push_back(p);
					}
				}
			}
			
			if(ship_manager[i]->getShipFaction()==FACTION_PROVIAN_CONSORTIUM)
			{
				
				if(ship_manager[i]->cannon.primary_shoot==true)
				{
				
					for(unsigned int t = 0; t<ship_manager[i]->getTurretManager().size();t++)
					{
						if(rand()%ship_manager[i]->getTurretManager()[t]->getReloadTime()<SHOOTTIME*frameDeltaTime)
						{
						sound->play3D("res/sounds/weapons/photon.wav",ship_manager[i]->getPos(),false,false,false);
						projectile *p  = new photonCannonShot(graphics, ship_manager[i]->getTurretManager()[t]->getPos(), ship_manager[i]->getTurretManager()[t]->getRot(),ship_manager[i]->ship);
						projectile_manager.push_back(p);
						}
					}
				
				
				}	
				if(ship_manager[i]->cannon.secondary_shoot==true)
				{
					for(unsigned int t = 0; t<ship_manager[i]->getSecondaryTurretManager().size();t++)
					{
						if(rand()%ship_manager[i]->getSecondaryTurretManager()[t]->getReloadTime()<SHOOTTIME*frameDeltaTime)
						{
						sound->play3D("res/sounds/weapons/plasma.wav",ship_manager[i]->getPos(),false,false,false);
						projectile *p  = new plasmaShot(graphics, ship_manager[i]->getSecondaryTurretManager()[t]->getPos(), ship_manager[i]->getSecondaryTurretManager()[t]->getRot(),ship_manager[i]->ship);
						projectile_manager.push_back(p);
						}
					}
				}		

			}
			else
			{
				if(ship_manager[i]->cannon.primary_shoot==true)
				{
				
					for(unsigned int t = 0; t<ship_manager[i]->getTurretManager().size();t++)
					{
						if(rand()%ship_manager[i]->getTurretManager()[t]->getReloadTime()<SHOOTTIME*frameDeltaTime)
						{
						sound->play3D("res/sounds/weapons/rail.wav",ship_manager[i]->getPos(),false,false,false);
						projectile *p  = new railgunShot(graphics, ship_manager[i]->getTurretManager()[t]->getPos(), ship_manager[i]->getTurretManager()[t]->getRot(),ship_manager[i]->ship);
						projectile_manager.push_back(p);
						}
					}
				
				
				}	
				if(ship_manager[i]->cannon.secondary_shoot==true)
				{
					
					for(unsigned int t = 0; t<ship_manager[i]->getSecondaryTurretManager().size();t++)
					{
						if(rand()%ship_manager[i]->getSecondaryTurretManager()[t]->getReloadTime()<SHOOTTIME*frameDeltaTime)
						{
						sound->play3D("res/sounds/weapons/antimatter.wav",ship_manager[i]->getPos(),false,false,false);
						projectile *p  = new antiMatterShot(graphics, ship_manager[i]->getSecondaryTurretManager()[t]->getPos(), ship_manager[i]->getSecondaryTurretManager()[t]->getRot(),ship_manager[i]->ship);
						projectile_manager.push_back(p);
						}
					}
				
				
				}	
			}
			
			//if the current ship is also a station
			//run through stationloop function for the ship as well
			//stations are essential ships that cannot move
			//and they also let other ships dock with them
			//no real difference other than that
			if(ship_manager[i]->getIsStation()==true)
			{
				stationLoop(frameDeltaTime,sound,ship_manager[i], callback);
			}

		}
		else
		{

			//exploldes
			ship_manager[i]->alive=false;


		}
	}

	
	//Player function, see below

	//required loop processes for each object in the game
	cargoManager(frameDeltaTime,CPlayer, alertBox);
	aoeManager(CPlayer);
	fighterManager(frameDeltaTime,sound,CPlayer);
	pdAnimationManager(frameDeltaTime,sound);
	projectileAnimationManager(frameDeltaTime,sound, CPlayer);
	missileAnimationManager(frameDeltaTime,sound,CPlayer);
	planetAnimationManager(frameDeltaTime);
	effectAnimationManager();
	destroyObjects(CPlayer);
}

//remember
//the CSHIP CLASS doubles as a station
//make sure station definitions dont let stations move though
//or else that would look really fucking stupid
void gameManager::stationLoop(f32 frameDeltaTime, irrklang::ISoundEngine *sound, CShip *station, ShaderCallBack *callback)
{

	if(station->getHullPoints()>0)
	{

		//assign station a planet for resources for ship production
		//basically, check if there is a friendly planet within 20km, then use that planet for resources
		if(station->getHomePlanet()==0)
		{
			for (unsigned int l =0; l<planet_manager.size(); l++)
			{
				if(station->getPos().getDistanceFrom(planet_manager[l]->getPos())<SHIP_WARP_DISTANCE)
				{
					station->setHomePlanet(planet_manager[l]);
				}
			}
		}
		else
		{
			
			//if station has a home planet
			//begin ship production
			//if(station->getShipClass() == ships().HQ)
			//{
				//only HQs can build ships
				//or else that would be broken
				if(station->getStarshipCreationTime() < graphics->getTimer()->getTime())
				{
					const wchar_t *pname = ships().provian_ship_name[rand()%ships().provian_ship_name.size()];
					if(station->getShipFaction()==FACTION_PROVIAN_CONSORTIUM)
					{
						CShip *newship  = new CShip(graphics,sound,station->getPos(),station->getRot(),ships().PROV_ISHTAR_CRUISER,FACTION_PROVIAN_CONSORTIUM, pname, callback);
						addShip(newship);
						//six minute timer before creating a anew ship
						//TODO: change with ship
						station->setStarshipCreationTime(graphics->getTimer()->getTime() + 360000);
					}
					if(station->getShipFaction()==FACTION_TERRAN_FEDERATION)
					{
						const wchar_t *tname = ships().terran_ship_name[rand()%ships().terran_ship_name.size()];
						CShip *newship  = new CShip(graphics,sound,station->getPos(),station->getRot(),ships().TERR_PRAETORIAN_CRUISER,FACTION_TERRAN_FEDERATION,tname, callback);
						addShip(newship);
						station->setStarshipCreationTime(graphics->getTimer()->getTime() + 360000);
					}
				}
				

			//}
		}

		//if there are ships around launch fighters
		if(getClosestEnemyShip(station)!=0)
		{
			if(station->getNumFighters()>0)
			{
				//make timer for fighter creation
				if(station->getFighterCreationTime() < graphics->getTimer()->getTime())
				{
					if(station->getShipFaction()==FACTION_TERRAN_FEDERATION)
					{
						vector3df pos = station->getPos();
						pos+=vector3df(rand()%100-50,rand()%100-50,rand()%100-50);
						fighter *f = new fighter(graphics,sound,station->getPos(),station->getRot(),fighters().TERR_DRAGONFLY,station->getShipFaction(),station->ship);
						f->setTarget(getClosestEnemyShip(station));
						addFighter(f);
						station->modNumFighters(-1);
						station->setFighterCreationTime(graphics->getTimer()->getTime()+1000);
					}
					if(station->getShipFaction()==FACTION_PROVIAN_CONSORTIUM)
					{
						vector3df pos = station->getPos();
						pos+=vector3df(rand()%100-50,rand()%100-50,rand()%100-50);
						fighter *f = new fighter(graphics,sound,station->getPos(),station->getRot(),fighters().PROV_RAPTOR,station->getShipFaction(),station->ship);
						f->setTarget(getClosestEnemyShip(station));
						addFighter(f);
						station->modNumFighters(-1);
						station->setFighterCreationTime(graphics->getTimer()->getTime()+1000);
					}
				}
			}


		}

		//if under attack launch distress

	}


}

//---------------------------------------------------------------------------------------------------------------------
//fighter run code
void gameManager::fighterManager(f32 frameDeltaTime, irrklang::ISoundEngine *sound, Player *CPlayer)
{
	//similar to how the main loop manages ships
	//this manages fighters
	//fighters thankfully, are much dumber
	for(unsigned int i=0; i<fighter_manager.size(); i++)
	{
		//ensures no crashes occur
		if(fighter_manager[i]->getHullPoints()>0)
		{
			//only fight if still have fuel
			vector3df pos = fighter_manager[i]->getPos();
			fighter_manager[i]->AIRun(frameDeltaTime);
			//MAX SPEED!
			fighter_manager[i]->setVelocity(fighter_manager[i]->getMaxVelocity());
			//theres not really a good way i could make the fighters that are owned by the player respond to its command
			//and make the player and fighter control sync
			//so i have to do some ugly functions that i know will work
			//but look out of place and are in bad locations


			if(fighter_manager[i]->getFuel()<graphics->getTimer()->getTime())
				fighter_manager[i]->setReturnHome(true);
			

			if(fighter_manager[i]->getReturnHome()!=true)
			{
				//fighting other fighters is a bigger priority
				if(fighter_manager[i]->getFighterTarget()==0)
				{
					if(fighter_manager[i]->getFighterFaction()==FACTION_PROVIAN_CONSORTIUM)
					{
						if(pos.getDistanceFrom(CPlayer->getPos())<5000)
						{
							//attack player
							fighter_manager[i]->moveToPoint(CPlayer->getPos());
							if(pos.getDistanceFrom(CPlayer->getPos())<1000)
							{
								fighter_manager[i]->fireMissile();
								fighter_manager[i]->moveAwayFromPoint(CPlayer->getPos());
							}
						}
					}
					if(getClosestEnemyFighter(fighter_manager[i])!=0)
					{
						//shootin tiempo
						fighter_manager[i]->setFighterTarget(getClosestEnemyFighter(fighter_manager[i]));
					}
					if(fighter_manager[i]->getTarget()==0)
					{
						//if no target patroll around n stuff
						fighter_manager[i]->patrol(fighter_manager[i]->getHomeBase()->getPosition());
						if(getClosestEnemyShipFighter(fighter_manager[i])!=0)
						{
							fighter_manager[i]->setTarget(getClosestEnemyShipFighter(fighter_manager[i]));
						}
					}
					else
					{
						//protect from crashes

						if(fighter_manager[i]->getTarget()->alive==true)
						{
							//attack
							//if fighter too far aim at target
							if(pos.getDistanceFrom(CPlayer->getPos())<pos.getDistanceFrom(fighter_manager[i]->getTarget()->getPos()))
							{
								if(fighter_manager[i]->getFighterFaction()==FACTION_PROVIAN_CONSORTIUM)
								{
									if(pos.getDistanceFrom(CPlayer->getPos())<5000)
									{
										//attack player
										fighter_manager[i]->moveToPoint(CPlayer->getPos());
										if(pos.getDistanceFrom(CPlayer->getPos())<1000)
										{
											fighter_manager[i]->fireMissile();
											fighter_manager[i]->moveAwayFromPoint(CPlayer->getPos());
										}
									}
								}
							}
							else
							{
								if(pos.getDistanceFrom(fighter_manager[i]->getTarget()->getPos())>FIGHTER_CLOSE_RANGE)
								{
									fighter_manager[i]->moveToPoint(fighter_manager[i]->getTarget()->getPos());
								}
								else
								{
									//shoot missile and then bounce
									fighter_manager[i]->fireMissile();
									fighter_manager[i]->moveAwayFromPoint(fighter_manager[i]->getTarget()->getPos());
									if(pos.getDistanceFrom(fighter_manager[i]->getTarget()->getPos())>FIGHTER_FAR_RANGE)
									{
										//break off
										fighter_manager[i]->setTarget(0);
									}
								}
							}
							//missile launch code
							if(fighter_manager[i]->cannon.missile==true)
							{
								if(pos.getDistanceFrom(CPlayer->getPos()) < 1000)
								{
									missile *m = new missile(graphics,pos,fighter_manager[i]->getRot(),CPlayer->ship);
									missile_manager.push_back(m);
								}
								else
								{
									missile *m = new missile(graphics,pos,fighter_manager[i]->getRot(),fighter_manager[i]->getTarget()->ship);
									missile_manager.push_back(m);
								}
							}
						}
						else
						{
							//target dead
							fighter_manager[i]->setTarget(0);
						}
					}
				}
				else
				{
					//dogfight
					fighter_manager[i]->setTarget(0);
					//huge crashing problem with fighters
					//its really annoying
					if(!fighter_manager[i]->getFighterTarget()->fighter_model)
						fighter_manager[i]->setFighterTarget(0);
					else
					{
						if(fighter_manager[i]->getFighterTarget()->getHullPoints()>0 && fighter_manager[i]->getFighterTarget()->alive!=false)
						{
							if(pos.getDistanceFrom(fighter_manager[i]->getFighterTarget()->getPos())>500)
							{
								fighter_manager[i]->moveToPoint(fighter_manager[i]->getFighterTarget()->getPos());
							}
							else
							{
								fighter_manager[i]->shoot();
								fighter_manager[i]->moveAwayFromPoint(fighter_manager[i]->getFighterTarget()->getPos());

							}

							if(fighter_manager[i]->cannon.cannon==true)
							{
								//projectile *p = new gatlingBullet(graphics,fighter_manager[i]->getPos(),fighter_manager[i]->getRot(),0);
								//pd_manager.push_back(p);
								if(rand()%3<2)
									fighter_manager[i]->getFighterTarget()->damageFighter(4);
							}
						}
						else
							fighter_manager[i]->setFighterTarget(0);
					}

				}

			}
			else
			{
				//return home
				//test this out later
				//could cause problems
				//if(fighter_manager[i]->getHomeBase()!=0)
				{
					fighter_manager[i]->moveToPoint(fighter_manager[i]->getHomeBase()->getPosition());
					
					if(pos.getDistanceFrom(fighter_manager[i]->getHomeBase()->getPosition()) < FIGHTER_DOCK_RANGE)
					{
						//dock
						if(CPlayer->ship==fighter_manager[i]->getHomeBase())
						{
							CPlayer->addFighterCount(1);
							fighter_manager[i]->alive = false;
							//fighter_manager[i]->drop();
							//fighter_manager.erase(fighter_manager.begin()+i);
						}
						else
						{
							for(int n=0; n<ship_manager.size(); n++)
							{
								if(ship_manager[n]->ship==fighter_manager[i]->getHomeBase())
								{
									ship_manager[n]->modNumFighters(1);
									fighter_manager[i]->alive = false;
									//fighter_manager[i]->drop();
									//fighter_manager.erase(fighter_manager.begin()+i);							

								}
							}
						}
					}
					
				}
			}
		}
		else
		{
			//explode
			//smallexplosion *e = new smallexplosion(graphics,sound,fighter_manager[i]->getPos());
			//effect_manager.push_back(e);
			fighter_manager[i]->alive = false;
			//fighter_manager[i]->drop();
			//fighter_manager.erase(fighter_manager.begin()+i);
		}
	}
}

//-------------------------------------------------------------------------------------------
//projectileAnimationManager
//This function is designed in order to update each projectile created by the game
//BEGIN FUNC
void gameManager::projectileAnimationManager(f32 frameDeltaTime,irrklang::ISoundEngine *sound, Player *CPlayer)
{
	//This function scans through each projectile and runs them
	//irrlicht collision detection isnt used
	//I do not understand it
	//So, heres my ghetto collision detection

	//12/4 currently the collision detection kills fps by about 30
	//get new collision detection
	//TODO: properly integrate irrlicht/bullet collision detection
	for(unsigned int i=0;i<projectile_manager.size();++i)
	{
		core::vector3df dist = (projectile_manager[i]->bullet->getPosition());
		bool tmp;
		tmp = false;
		if(projectile_manager[i]->checkRange()<projectile_manager[i]->getRange())
		{
			projectile_manager[i]->projRun(frameDeltaTime);

			//Make sure the projectile created by the player doesn't destroy the players own ship
			
			if(projectile_manager[i]->getShip()!=CPlayer->ship)
			{
				if(CPlayer->ship->getTransformedBoundingBox().isPointInside(projectile_manager[i]->bullet->getPosition()))
				{
					irrklang::ISound *impactsound = sound->play3D("res/sounds/hit.wav",CPlayer->getPos(),false,false,false);
					CPlayer->damagePlayer(projectile_manager[i]->damage);
					impact *imp = new impact(graphics,sound,dist);
					effect_manager.push_back(imp);
					tmp = true;
				}
			}

			//attempt to make brute force approach not so brute force
			//double axis aligned collision checking
			std::vector<CShip*> speed;
			speed = ship_manager;
			for(unsigned int l=0; l<speed.size();l++)
			{
				bool tmp=false;
				if(speed[l]->getPos().X > projectile_manager[i]->bullet->getPosition().X + 600)
				{
					tmp=true;
				}
				if(speed[l]->getPos().X < projectile_manager[i]->bullet->getPosition().X - 600)
				{
					tmp=true;
				}
				if(speed[l]->getPos().Y > projectile_manager[i]->bullet->getPosition().Y + 600)
				{
					tmp=true;
				}
				if(speed[l]->getPos().Y < projectile_manager[i]->bullet->getPosition().Y - 600)
				{
					tmp=true;
				}
				if(tmp==true)
					speed.erase(speed.begin()+l);
			}
			for(unsigned int l=0;l<speed.size();l++)
			{
				//Make sure AI doesn't destroy itself	
				if(speed[l]->ship!=projectile_manager[i]->getShip())
				{
					if(speed[l]->getHullPoints()>0)
					{
						if(speed[l]->ship->getTransformedBoundingBox().isPointInside(projectile_manager[i]->bullet->getPosition()))
						{
							irrklang::ISound *impactsound = sound->play3D("res/sounds/hit.wav",speed[l]->getPos(),false,false,false);
							speed[l]->damageShip(projectile_manager[i]->damage);
							//create impact
							impact *imp = new impact(graphics,sound,dist);
							effect_manager.push_back(imp);
							tmp = true;
						}				
					}
				}
			}
			
			//the tmp bool MUST be used, or else the program can crash
			//If it is not used, then projectile_manager might call a projectile object that was recently deleted
			//giving an out of range error

			if(tmp==true)
			{
				//delete the projectile if its hit something
				projectile_manager[i]->drop();
				projectile_manager.erase(projectile_manager.begin()+i);
			}
		}
		else
		{
			//delete projectiles that have exceeded their maximum range
			projectile_manager[i]->drop();
			projectile_manager.erase(projectile_manager.begin()+i);
		}
	}
}
//END FUNC
//-----------------------------------------------------------------------------------------------


//for fighters
void gameManager::pdAnimationManager(f32 frameDeltaTime, irrklang::ISoundEngine *sound)
{
	for(unsigned int i=0;i<pd_manager.size();i++)
	{
		core::vector3df dist = (pd_manager[i]->bullet->getPosition());
		bool tmp;
		tmp = false;
		if(pd_manager[i]->checkRange() < pd_manager[i]->getRange())
		{
			pd_manager[i]->projRun(frameDeltaTime);
			
			
			for(int l=0; l<fighter_manager.size();l++)
			{
				if(fighter_manager[l]->getHullPoints()>0)
				{
					if(dist.getDistanceFrom(fighter_manager[l]->getPos())<20)
					{
						//damage
						irrklang::ISound *impactsound = sound->play3D("res/sounds/hit.wav",fighter_manager[l]->getPos(),false,false,false);
						fighter_manager[l]->damageFighter(pd_manager[i]->damage);
						//create impact
						impact *imp = new impact(graphics,sound,dist);
						effect_manager.push_back(imp);
						tmp = true;
					}
				}
			}
			for(int l = 0; l<missile_manager.size();l++)
			{
				if(missile_manager[l]->getHealth()>0)
				{
					if(dist.getDistanceFrom(missile_manager[l]->getPos())<50)
					{
						missile_manager[l]->damage(pd_manager[i]->damage);
						tmp=true;
					}
				}
			}
			
			if(tmp==true)
			{
				//delete the projectile if its hit something
				pd_manager[i]->drop();
				pd_manager.erase(pd_manager.begin()+i);
			}
		}
		else
		{
			pd_manager[i]->drop();
			pd_manager.erase(pd_manager.begin()+i);
		}
	}
}

//END FUNCT
//----------------------------------------------------------------------------------------------------------

void gameManager::missileAnimationManager(f32 frameDeltaTime, irrklang::ISoundEngine *sound, Player *CPlayer)
{
	for(unsigned int i=0 ;i<missile_manager.size();i++)
	{
		//missiles only impact the ship they're shooting at
		//for speed issues
		vector3df pos = missile_manager[i]->getPos();
		bool tmp;
		tmp=false;
		if(missile_manager[i]->checkRange()<missile_manager[i]->getRange())
		{
			if(missile_manager[i]->getHealth()>0)
			{
				missile_manager[i]->loop(frameDeltaTime);
				//explode at certain range
				if(missile_manager[i]->getDistFromTarget()<50)
				{
					//create aoe cause missiles are like that
					smallexplosion *e = new smallexplosion(graphics,sound,pos);
					effect_manager.push_back(e);
					areaofeffect *a = new areaofeffect(graphics,pos,400,400,10);
					aoe_manager.push_back(a);
					tmp=true;
				}
			}
			else
			{
				smallexplosion *e = new smallexplosion(graphics,sound,pos);
				effect_manager.push_back(e);
				tmp=true;
			}
			if(tmp==true)
			{
				missile_manager[i]->drop();
				missile_manager.erase(missile_manager.begin()+i);
			}
		}
		else
		{
			//delete
			missile_manager[i]->drop();
			missile_manager.erase(missile_manager.begin()+i);
		}
	}
}
//---------------------------------------------------------------------------------------
void gameManager::aoeManager(Player *CPlayer)
{
	for(unsigned int i=0; i<aoe_manager.size();i++)
	{
		vector3df pos = aoe_manager[i]->getPos();
		for(unsigned int l=0; l<ship_manager.size();l++)
		{
			//inside radius
			if(ship_manager[l]->getHullPoints()>0)
			{
				if(pos.getDistanceFrom(ship_manager[l]->getPos())<aoe_manager[i]->getRadius())
				{
					int dist = pos.getDistanceFrom(ship_manager[l]->getPos());
					int max_d = aoe_manager[i]->getMaxDamage()/2 - aoe_manager[i]->getMaxDamage()/dist;
					ship_manager[l]->damageShip(max_d);
				}
			}
			
		}
		
		if(pos.getDistanceFrom(CPlayer->getPos())< aoe_manager[i]->getRadius())
		{
			int dist = pos.getDistanceFrom(CPlayer->getPos());
			int max_d = aoe_manager[i]->getMaxDamage()/dist;
			CPlayer->damagePlayer(max_d);
		}

		//delete the aoe after its done its dirty deeds
		aoe_manager[i]->drop();
		aoe_manager.erase(aoe_manager.begin()+i);
	}
}
//--------------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------------
//rotates the cargo
void gameManager::cargoManager(f32 frameDeltaTime, Player *CPlayer, CAlertBox *alertBox)
{
	for(unsigned int i=0; i<cargo_manager.size();i++)
	{
		cargo_manager[i]->loop(frameDeltaTime);

		//if player is close enough
		//pick it up
		if(CPlayer->getPos().getDistanceFrom(cargo_manager[i]->getPos())<CARGODISTANCE)
		{
			std::vector<item*> temp = cargo_manager[i]->getInventory();
			for(unsigned int l=0; l<temp.size();l++)
			{
				CPlayer->addItemToCargo(temp[l]);
				
			}
			//player gets random amount of cash
			int cash = rand()%600;
			CPlayer->setMoney(CPlayer->getMoney()+cash);

			alertBox->addText(L"Cargo Loaded", NORMAL);

			//then delete
			cargo_manager[i]->drop();
			cargo_manager.erase(cargo_manager.begin()+i);
		}
	}
}
//-----------------------------------------------------------------------------------------


//show planet targetting reticules
void gameManager::showPlanetTargets(bool ishidden)
{
	for (unsigned int i = 0; i<planet_manager.size(); i++)
	{
		planet_manager[i]->setArrayVisible(ishidden);
	}
}
//ditto for ships
void gameManager::showShipTargets(bool ishidden)
{
	showShips = ishidden;
}

void gameManager::planetAnimationManager(f32 frameDeltaTime)
{
	//Rotates the planets
	//minor function
	for(unsigned int i = 0;i<planet_manager.size();i++)
	{
		//corrona faces cam
		core::vector3df diff = planet_manager[i]->model->getPosition() - graphics->getSceneManager()->getActiveCamera()->getPosition();
		core::vector3df angle = diff.getHorizontalAngle();
		planet_manager[i]->rotate(frameDeltaTime);
		if(planet_manager[i]->getPlanetType()->corona==true)
			planet_manager[i]->corona->setRotation(angle);

		//hide plant clouds when camera is far away
		if(planet_manager[i]->getPos().getDistanceFrom(graphics->getSceneManager()->getActiveCamera()->getPosition())>CAMERA_HIDE_DISTANCE)
			planet_manager[i]->setCloudsVisible(false);
		else
			planet_manager[i]->setCloudsVisible(true);
	}
}

//this function simply runs through each effect
void gameManager::effectAnimationManager()
{
	for(unsigned int i=0; i <effect_manager.size();i++)
	{
		if(effect_manager[i]->getEnd()==false)
		{
			effect_manager[i]->loop();
		}
		else
		{
			effect_manager[i]->drop();
			effect_manager.erase(effect_manager.begin()+i);
		}
	}
}

//-----------------------------------------------------------------------------------------------------------
//function for playershoot
//called from the gameloop
//all the shooting management is done in this file because all the other ships are managed in this file
//so shooting has to be done in the same file
void gameManager::playerShoot(irr::IrrlichtDevice *graphics,Player* CPlayer, irrklang::ISoundEngine *sound, CShip *player_target, f32 frameDeltaTime, int subsys)
{

	//The same thing that applies to ships applies to the player
	//However, the Player projectiles must also come from the turret
	//and shoot different projectiles depending on what type of turret the player has installed
	//turn off broadsides only for demo purposes
	if(CPlayer->cannonFired.primary==true)
	{

		for(unsigned int i=0;i<CPlayer->turret_manager.size();i++)
		{
			//if(angleY + 40 < CPlayer->getRot().Y || angleY-40 > CPlayer->getRot().Y)
			//{
				//most turrets can only fire broadsides
				//cause broadsiding is cool as shit
				//Scan through the turrets that the player has
			/*
				float x = CPlayer->turret_manager[i]->getPos().X - CPlayer->getPos().X;
				float y = CPlayer->turret_manager[i]->getPos().Y - CPlayer->getPos().Y;
				float z = CPlayer->turret_manager[i]->getPos().Z - CPlayer->getPos().Z;
				float angleY = std::atan2(x,z)*180/3.14;
				*/
			if(rand()%CPlayer->turret_manager[i]->getReloadTime()<SHOOTTIME*frameDeltaTime)
			{
				if(CPlayer->getPrimaryTurret()->name==items().PRI_RAIL->name)
				{
					sound->play3D("res/sounds/weapons/rail.wav",CPlayer->turret_manager[i]->getPos(),false,false,false);
					projectile *p  = new railgunShot(graphics, CPlayer->turret_manager[i]->getFirePos(), CPlayer->turret_manager[i]->getRot(),CPlayer->ship);
					projectile_manager.push_back(p);
				}
				if(CPlayer->getPrimaryTurret()->name==items().PRI_PHOTON->name)
				{
					sound->play3D("res/sounds/weapons/photon.wav",CPlayer->turret_manager[i]->getPos(),false,false,false);
					projectile *p  = new photonCannonShot(graphics, CPlayer->turret_manager[i]->getFirePos(), CPlayer->turret_manager[i]->getRot(),CPlayer->ship);
					projectile_manager.push_back(p);
				}
				muzzleflash *m = new muzzleflash(graphics,sound,CPlayer->turret_manager[i]->getFirePos(), CPlayer->turret_manager[i]->getRot(), CPlayer->turret_manager[i]->getBone());
				effect_manager.push_back(m);

				if(player_target!=0)
				{
					if(rand()%300<3)
					{
						if(player_target->getShieldPoints()<1)
						{
							if(subsys==0)
							{
								player_target->subsystem.engine-=rand()%10;
							}
							if(subsys==1)
							{
								player_target->subsystem.warpdrive-=rand()%10;
							}
							if(subsys==2)
							{
								player_target->subsystem.primary_weapons-=rand()%10;
							}
							if(subsys==3)
							{
								player_target->subsystem.secondary_weapons-=rand()%10;
							}
							if(subsys==4)
							{
								player_target->subsystem.light_weapons-=rand()%10;
							}
						}
					}
				}
			}
			//}
		}
	}

	if(CPlayer->cannonFired.secondary == true)
	{
		//Scan through the turrets that the player has
		/*
		float x = player_target->getPos().X - CPlayer->getPos().X;
		float y = player_target->getPos().Y - CPlayer->getPos().Y;
		float z = player_target->getPos().Z - CPlayer->getPos().Z;
		float angleY = std::atan2(x,z)*180/3.14;
		*/
			for(unsigned int i = 0; i< CPlayer->secondary_turret_manager.size();i++)
			{
				if(rand()%CPlayer->secondary_turret_manager[i]->getReloadTime()<SHOOTTIME*frameDeltaTime)
				{
					//if(angleY + 40 < CPlayer->getRot().Y || angleY-40 > CPlayer->getRot().Y)
					//{

					sound->play3D("res/sounds/weapons/antimatter.wav",CPlayer->secondary_turret_manager[i]->getPos(),false,false,false);
					projectile *p  = new antiMatterShot(graphics, CPlayer->secondary_turret_manager[i]->getFirePos(), CPlayer->secondary_turret_manager[i]->getRot(),CPlayer->ship);
					projectile_manager.push_back(p);
					muzzleflash *m = new muzzleflash(graphics,sound,CPlayer->secondary_turret_manager[i]->getFirePos(), CPlayer->secondary_turret_manager[i]->getRot(), CPlayer->secondary_turret_manager[i]->getBone());
					effect_manager.push_back(m);

					//}
				}

				
			}
	}

	if(CPlayer->cannonFired.light == true)
	{

		for(unsigned int i=0; i<CPlayer->light_turret_manager.size();i++)
		{
			if(rand()%CPlayer->light_turret_manager[i]->getReloadTime()<SHOOTTIME*frameDeltaTime)
			{
				sound->play3D("res/sounds/weapons/gatling.wav",CPlayer->light_turret_manager[i]->getPos(),false,false,false);
				projectile *p  = new gatlingBullet(graphics, CPlayer->light_turret_manager[i]->getFirePos(), CPlayer->light_turret_manager[i]->getRot(),CPlayer->ship);
				pd_manager.push_back(p);
				muzzleflash *m = new muzzleflash(graphics,sound,CPlayer->light_turret_manager[i]->getFirePos(), CPlayer->light_turret_manager[i]->getRot(), CPlayer->light_turret_manager[i]->getBone());
				effect_manager.push_back(m);
			}
		}
	}

	//BUG: point defense turrets never stop shooting
	int tmp=0;
	for(unsigned int i=0;i<fighter_manager.size();i++)
	{
		if(fighter_manager[i]->getHostileToPlayer()==true)
		{
			if(CPlayer->getPos().getDistanceFrom(fighter_manager[i]->getPos())<2000)
			{
				CPlayer->aimPd(fighter_manager[i]->getPos(),frameDeltaTime);
				CPlayer->shootPD();
				tmp+=1;
			}
			//else
				//CPlayer->resetPD();
		}
	}
	if(tmp==0)
		CPlayer->resetPD();
}
//end funct
//-----------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//destroyObjects()
//cleaner function
//call this last in order to make everything way way more clean
//should no longer have reference errors with this function
void gameManager::destroyObjects(Player *CPlayer)
{
	for(int i=0;i<ship_manager.size();i++)
	{
		//if no longer alive
		//delete the object
		if(ship_manager[i]->alive!=true)
		{
			explosion* e = new explosion(graphics,sound,ship_manager[i]->getPos());
			effect_manager.push_back(e);
			irrklang::ISound *s = sound->play3D("res/sounds/reactor_explo.wav",ship_manager[i]->getPos(),false,false,true);
			s->setMinDistance(5000.f);
			s->drop();
			cargo *c = new cargo(graphics,ship_manager[i]->getPos());
			cargo_manager.push_back(c);
			ship_manager[i]->drop();
			ship_manager.erase(ship_manager.begin()+i);
		}
	}
	for(int i=0;i< fighter_manager.size(); i++)
	{
		if(fighter_manager[i]->alive!=true)
		{
			smallexplosion *e = new smallexplosion(graphics,sound,fighter_manager[i]->getPos());
			irrklang::ISound *impactsound = sound->play3D("res/sounds/hit.wav",fighter_manager[i]->getPos(),false,false,false);
			effect_manager.push_back(e);
			fighter_manager[i]->drop();
			fighter_manager.erase(fighter_manager.begin()+i);
		}
	}

	// i need this, but i dont know why the hell i put it here
	//creates explosion on player death
	if(CPlayer->getHull()<1)
	{
		if(playerDead==false)
		{
			explosion* e = new explosion(graphics,sound,CPlayer->getPos());
			effect_manager.push_back(e);
			playerDead=true;
		}
	}
}
//end funct // destroyObjects()
//-------------------------------------------------------------------

//-----------------------------------------------------------
//IMPORTANT MISC FUNCTIONS

//legacy code, probably won't be used
void gameManager::addProjectile(projectile* shot)
{
	projectile_manager.push_back(shot);
}

//used by the gameloop object to add ships to the ship manager
void gameManager::addShip(CShip* ship)
{
	ship_manager.push_back(ship);
}

void gameManager::addFighter(fighter* f)
{
	fighter_manager.push_back(f);
}

//legacy
void gameManager::addStation(CShip* station)
{
	ship_manager.push_back(station);
}

void gameManager::addPlanet(planet* p)
{
	planet_manager.push_back(p);
}

CShip* gameManager::getTarget()
{
	//Some pretty basic code here
	//Just scan through all the ships inside the ship_manager array
	//and then get the position of the mouse
	//get the distance between the target icon on the ship and the mouse using trig
	//then if the mouse is closer than 32 pixels to the target icon thing
	//select the ship then return it to the player_target variable inside the gameloop object
	vector2d<int> t;
	t.X = receiver->mouseX();
	t.Y = receiver->mouseY();

	for(unsigned int i=0;i<ship_manager.size();i++)
	{
		//ensure player can see the targetting rectangle
		if(ship_manager[i]->getWithinRadarRange()==true)
		{
			vector2d<int> pos;
			pos = ship_manager[i]->getArrayPos();
			const float x = t.X-pos.X;
			const float y = t.Y-pos.Y;
			float dist = sqrt(x*x + y*y);
			if(dist<SELECTDISTANCE)
			{
				return ship_manager[i];
			}
		}
	}

	//Makes sure game doesn't crash
	return 0;
}

planet* gameManager::getTargetPlanet()
{
	//similiar to getTarget() function but for planets

	vector2d<int> t;
	t.X = receiver->mouseX();
	t.Y = receiver->mouseY();

	for( unsigned int i = 0; i < planet_manager.size(); i++)
	{
		vector2d<int> pos;
		pos = planet_manager[i]->getArrayPos();
		const float x = t.X - pos.X;
		const float y = t.Y - pos.Y;
		float dist = sqrt( x*x + y*y);

		if(dist<SELECTDISTANCE)
		{
			return planet_manager[i];
		}
	}

	return 0;
}


//legacy
//delete later
CShip *gameManager::getClosestStation(Player *CPlayer)
{
	for(unsigned int i=0;i<station_manager.size();i++)
	{
		int dist = station_manager[i]->getDistToPoint(CPlayer->getPos());
		if(dist<1000)
		{
			return station_manager[i];
		}
	}

	return 0;
}

//---------------------------------------------------------------------------------------------------------------------
//SAVE and LOAD FUNCTIONS START HERE
//---------------------------------------------------------------------------------------------------------------------

//converts a float to a string
stringw gameManager::floatToString(float num)
{
	stringw t(L"");
	t+=num;
	return t;
}
//same as above but with int
//so save files dont get cluttered with useless decimals
//template blah blah blah shut up
stringw gameManager::intToString(int num)
{
	stringw t(L"");
	t+=num;
	return t;
}

//save all the ships into the xml file
void gameManager::saveObjects(io::IXMLWriter *writer)
{
	//save number of ships
	//for easier loading
	core::array<stringw> value;
	value.push_back(L"num");
	core::array<stringw> num;
	num.push_back(intToString(ship_manager.size()));
	writer->writeElement(L"shipStats",true,value,num);
	//create a different element
	//for each ship
	//each element is simply labelled with a number
	for (unsigned int i = 0; i < ship_manager.size(); i++)
	{
		core::array<stringw> attributes;
		core::array<stringw> values;

		attributes.push_back(L"faction");
		values.push_back(intToString(ship_manager[i]->getShipFaction()));
		attributes.push_back(L"name");
		values.push_back(ship_manager[i]->getName());

		attributes.push_back(L"ship");
		values.push_back(intToString(ship_manager[i]->getShipClass()->index));

		attributes.push_back(L"hullPoints");
		values.push_back(floatToString(ship_manager[i]->getHullPoints()));
		attributes.push_back(L"shieldPoints");
		values.push_back(floatToString(ship_manager[i]->getShieldPoints()));
		attributes.push_back(L"armorPoints");
		values.push_back(floatToString(ship_manager[i]->getArmorPoints()));

		attributes.push_back(L"posX");
		values.push_back(floatToString(ship_manager[i]->getPos().X));
		attributes.push_back(L"posY");
		values.push_back(floatToString(ship_manager[i]->getPos().Y));
		attributes.push_back(L"posZ");
		values.push_back(floatToString(ship_manager[i]->getPos().Z));

		attributes.push_back(L"rotX");
		values.push_back(floatToString(ship_manager[i]->getRot().X));
		attributes.push_back(L"rotY");
		values.push_back(floatToString(ship_manager[i]->getRot().Y));
		attributes.push_back(L"rotZ");
		values.push_back(floatToString(ship_manager[i]->getRot().Z));

		attributes.push_back(L"velocity");
		values.push_back(floatToString(ship_manager[i]->getVelocity()));

		attributes.push_back(L"id");
		values.push_back(ship_manager[i]->getID());

		attributes.push_back(L"numFighters");
		values.push_back(intToString(ship_manager[i]->getNumFighters()));

		attributes.push_back(L"shipCreationTime");
		values.push_back(intToString(ship_manager[i]->getStarshipCreationTime()));


		//write into element shipStats
		writer->writeElement(intToString(i).c_str(),true,attributes,values);

	}
	writer->writeClosingTag(L"shipStats");

	//now save fighters
	core::array<stringw> fvalue;
	fvalue.push_back(L"num");
	core::array<stringw> fnum;
	fnum.push_back(intToString(fighter_manager.size()));
	writer->writeElement(L"fighterStats",true,fvalue,fnum);
	for(unsigned int i=0; i<fighter_manager.size();i++)
	{
		core::array<stringw> attributes;
		core::array<stringw> values;
		attributes.push_back(L"faction");
		values.push_back(intToString(fighter_manager[i]->getFighterFaction()));
		//attributes.push_back(L"name");
		//values.push_back(fighter_manager[i]->getName());

		attributes.push_back(L"fighter");
		values.push_back(intToString(fighter_manager[i]->getFighterClass()->index));

		attributes.push_back(L"hullPoints");
		values.push_back(floatToString(fighter_manager[i]->getHullPoints()));

		attributes.push_back(L"posX");
		values.push_back(floatToString(fighter_manager[i]->getPos().X));
		attributes.push_back(L"posY");
		values.push_back(floatToString(fighter_manager[i]->getPos().Y));
		attributes.push_back(L"posZ");
		values.push_back(floatToString(fighter_manager[i]->getPos().Z));

		attributes.push_back(L"rotX");
		values.push_back(floatToString(fighter_manager[i]->getRot().X));
		attributes.push_back(L"rotY");
		values.push_back(floatToString(fighter_manager[i]->getRot().Y));
		attributes.push_back(L"rotZ");
		values.push_back(floatToString(fighter_manager[i]->getRot().Z));

		attributes.push_back(L"velocity");
		values.push_back(floatToString(fighter_manager[i]->getVelocity()));

		//homebase
		
		attributes.push_back(L"homebase");
		for(int n=0; n<ship_manager.size(); n++)
		{
			if(ship_manager[n]->ship == fighter_manager[i]->getHomeBase())
			{
				values.push_back(ship_manager[n]->getID());
			}
		}
		
		//write into element shipStats
		writer->writeElement(intToString(i).c_str(),true,attributes,values);
	}
	writer->writeClosingTag(L"fighterStats");

	//save loot
	core::array<stringw> cvalue;
	cvalue.push_back(L"num");
	core::array<stringw> cnum;
	cnum.push_back(intToString(cargo_manager.size()));
	writer->writeElement(L"cargoStats",true,cvalue,cnum);
	for(int i=0; i<cargo_manager.size();i++)
	{
		core::array<stringw> attributes;
		core::array<stringw> values;

		attributes.push_back(L"posX");
		values.push_back(floatToString(cargo_manager[i]->getPos().X));
		attributes.push_back(L"posY");
		values.push_back(floatToString(cargo_manager[i]->getPos().Y));
		attributes.push_back(L"posZ");
		values.push_back(floatToString(cargo_manager[i]->getPos().Z));
	}
}

//load attributes from xml file
void gameManager::loadShips(io::IXMLReader *reader, int numobjects, ShaderCallBack *callback)
{
	//for num of ships inside of file
	for(u32 i = 0;i<numobjects;i++)
	{
		if(core::stringw(intToString(i)).equals_ignore_case(reader->getNodeName()))
		{
			//create ship
			//and set ship stats

			ship_faction faction;
			ship_base *sclass;

			
			int t = reader->getAttributeValueAsInt(L"faction");
			if(t==0)
			{
				faction = FACTION_TERRAN_FEDERATION;
			}
			if(t==1)
			{
				faction = FACTION_PROVIAN_CONSORTIUM;
			}
			if(t==2)
			{
				faction = FACTION_PIRATE;
			}
			if(t==3)
			{
				faction = FACTION_NEUTRAL;
			}

			int s = reader->getAttributeValueAsInt(L"ship");
			for(unsigned int l=0; l<ships().ship_list.size();l++)
			{
				if(ships().ship_list[l]->index==s)
				{
					sclass = ships().ship_list[l];
				}
			}

			//set the position and rotation
			vector3df pos;
			vector3df rot;
			pos.X = reader->getAttributeValueAsFloat(L"posX");
			pos.Y = reader->getAttributeValueAsFloat(L"posY");
			pos.Z = reader->getAttributeValueAsFloat(L"posZ");

			rot.X = reader->getAttributeValueAsFloat(L"rotX");
			rot.Y = reader->getAttributeValueAsFloat(L"rotY");
			rot.Z = reader->getAttributeValueAsFloat(L"rotZ");

			//ship creation time
			int ship_creation = reader->getAttributeValueAsInt(L"shipCreationTime");

			//load name
			stringw str(L"");
			str += reader->getAttributeValue(L"name");
			int size =str.size();
			wchar_t *buffer;
			buffer = new wchar_t[size+1];
			//re encode the string onto the new space
			for(int n=0;n<size;n++)
			{
				buffer[n]=str[n];
			}
			//termination character
			buffer[size]='\0';

			//load id
			stringw id(L"");
			id += reader->getAttributeValue(L"id");
			int size_id =id.size();
			wchar_t *buffer_id;
			buffer_id = new wchar_t[size_id+1];
			//re encode the string onto the new space
			for(int n=0;n<size_id;n++)
			{
				buffer_id[n]=id[n];
			}
			//termination character
			buffer_id[size_id]='\0';

			CShip *newship = new CShip(graphics,sound,pos,rot,sclass,faction,buffer,callback);
			newship->setStarshipCreationTime(ship_creation);
			newship->setID(buffer_id);

			newship->setHullPoints(reader->getAttributeValueAsInt(L"hullPoints"));
			newship->setShieldPoints(reader->getAttributeValueAsInt(L"shieldPoints"));
			newship->setArmorPoints(reader->getAttributeValueAsInt(L"armorPoints"));

			int fighters = reader->getAttributeValueAsInt(L"numFighters");
			newship->setNumFighters(fighters);
			addShip(newship);
			//newship->setVelocity(reader->getAttributeValueAsFloat(L"velocity"));
		}
	}

	
}


void gameManager::loadFighters(io::IXMLReader *reader, int numobjects)
{
	for(u32 i=0; i<numobjects; i++)
	{
		if(core::stringw(intToString(i)).equals_ignore_case(reader->getNodeName()))
		{
			ship_faction faction;
			fighter_base *fclass;

			int t = reader->getAttributeValueAsInt(L"faction");

			if(t==0)
			{
				faction = FACTION_TERRAN_FEDERATION;
			}
			if(t==1)
			{
				faction = FACTION_PROVIAN_CONSORTIUM;
			}
			if(t==2)
			{
				faction = FACTION_PIRATE;
			}
			if(t==3)
			{
				faction = FACTION_NEUTRAL;
			}

			int s = reader->getAttributeValueAsInt(L"fighter");
			for(u32 l=0; l<fighters().fighter_list.size(); l++)
			{
				if(fighters().fighter_list[l]->index==s)
				{
					fclass = fighters().fighter_list[l];
				}
			}

			//set the position and rotation
			vector3df pos;
			vector3df rot;
			pos.X = reader->getAttributeValueAsFloat(L"posX");
			pos.Y = reader->getAttributeValueAsFloat(L"posY");
			pos.Z = reader->getAttributeValueAsFloat(L"posZ");

			rot.X = reader->getAttributeValueAsFloat(L"rotX");
			rot.Y = reader->getAttributeValueAsFloat(L"rotY");
			rot.Z = reader->getAttributeValueAsFloat(L"rotZ");

			//load name
			stringw str(L"");
			str += reader->getAttributeValue(L"name");
			int size =str.size();
			wchar_t *buffer;
			buffer = new wchar_t[size+1];
			//re encode the string onto the new space
			for(int n=0;n<size;n++)
			{
				buffer[n]=str[n];
			}
			//termination character
			buffer[size]='\0';

			fighter *f = new fighter(graphics,sound,pos,rot,fclass,faction,0,buffer);
			//load homebase
			stringw id(L"");
			id += reader->getAttributeValue(L"homebase");
			int size_id =id.size();
			wchar_t *buffer_id;
			buffer_id = new wchar_t[size_id+1];
			//re encode the string onto the new space
			for(int n=0;n<size_id;n++)
			{
				buffer_id[n]=id[n];
			}
			//termination character
			buffer_id[size_id]='\0';
			if(buffer_id==L"Player")
			{
				//f->setHomeBase(CPlayer->ship);
			}
			for(int n = 0; n<ship_manager.size(); n++)
			{
				if(ship_manager[n]->getID()==buffer_id)
				{
					f->setHomeBase(ship_manager[n]->ship);
				}
			}
		}
	}
}

