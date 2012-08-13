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

//ship.h
//includes ships and stations
//stations are simply ships that dont move and you can dock at them

#ifndef _SHIP_H
#define _SHIP_H

#pragma once

#include "irrlicht.h"
#include "irrklang.h"
#include "sTypes.h"
#include "projectile.h"
#include "explosion.h"
#include "object.h"
#include "planet.h"
#include "shadercallback.h"
#include "turret.h"


#include "vector"

using namespace irr;
using namespace scene;
using namespace core;

//to make certain ships do certain things
enum SHIP_ROLE
{
	ROLE_ATTACKING=0,
	ROLE_DEFENDING=1,
	ROLE_TRADING=2,
};


class CShip : public CObject
{
public:

	//disgusting

	int last_time;
	int last_move;
	bool alive;

	struct turningCircle
	{
		float X;
		float Y;
		float Z;
	};
	struct turningCircle turn;

	struct cannonFired
	{
		int primary;
		bool primary_shoot;
		int secondary;
		bool secondary_shoot;
		int light;
		bool light_shoot;
	};
	struct cannonFired cannon;

	struct fireC
	{
		float X;
		float Y;
		float Z;
	};
	struct fireC fireRot;

	struct subsystemStats
	{
		int primary_weapons;
		int secondary_weapons;
		int light_weapons;
		int warpdrive;
		int engine;
	};
	struct subsystemStats subsystem;


	gui::IGUIImage *target_array;


	core::vector3df sRot;
	core::vector3df sPos;
	core::vector3df rotSlow;
	core::vector3df posSlow;


	//double check this if needed later
	scene::IBoneSceneNode *turret_node[4];

	scene::IBoneSceneNode *exhaust_01;
	scene::IBoneSceneNode *exhaust_02;
	scene::IBoneSceneNode *exhaust_03;


	irr::IrrlichtDevice *graphics;
	irrklang::ISoundEngine *sound;
	irr::scene::IAnimatedMeshSceneNode *ship;
	f32 frameDeltaTime;

	ship_base *s_class;

	ship_types type;

	//initializer method
	//it has some default constructors, but when using it make sure to fill in everything
	//unless debugging for something
	CShip(irr::IrrlichtDevice *graphics=(irr::IrrlichtDevice *)0,
		irrklang::ISoundEngine *sound = (irrklang::ISoundEngine *) 0,
		const core::vector3df& ship_position = core::vector3df(0,0,0),
		const core::vector3df& ship_rotation = core::vector3df(0,0,0),
		ship_base *s_class = ships().PROV_ISHTAR_CRUISER,
		ship_faction faction= FACTION_NEUTRAL,
		const wchar_t *name = L"__test_ship",
		ShaderCallBack *callback = 0);


	virtual ~CShip();

	float getDistToPoint(core::vector3df& point);
	void AIRun(f32 frameDeltaTime);
	void manageTurrets(f32 frameDeltaTime);
	void movement(f32 frameDeltaTime);
	void damageShip(int damage);
	void applyForce(vector3df &rotation, int amount, f32 frameDeltaTime);
	void drop();

	//rotatates ship to a point
	void rotateToPoint(core::vector3df& point);
	void rotateAwayFromPoint(core::vector3df& point);
	void shootAtPoint(core::vector3df& point, f32 frameDeltaTime);
	void PDatPoint(vector3df &point, f32 frameDeltaTime);
	void engagePlayer(core::vector3df& playerpos, f32 frameDeltaTime);
	void engageCurrentTarget(f32 frameDeltaTime);
	void dodgeFire();
	void patrol(core::vector3df& point1, core::vector3df& point2);
	void warpToPlanet(planet* target_planet);
	void patrolArea(core::vector3df& point);

	void ableToShoot();
	void pdShoot();
	//necessary for bugfixing
	void resetCannons();
	void resetPD();
	projectile *addShot(core::vector3df& pos);

	//'get' funcs
	int getHullPoints() const;
	int getArmorPoints() const;
	int getShieldPoints() const;
	bool getHostileToPlayer() const;
	float getVelocity();
	float getMaxVelocity();
	void setVelocity(float newvelocity);

	bool getWarping();
	void setWarping(bool warp);

	bool getIsStation();

	core::vector3df getPos() const;
	void setPos(vector3df newpos);
	core::vector3df getRot() const;
	void setRot(vector3df newrot);
	void setHullPoints(int hullpoints);
	void setShieldPoints(int shieldpoints);
	void setArmorPoints(int armorpoints);
	ship_base *getShipClass();
	ship_faction getShipFaction();
	int getEnergy() const;
	void setVisible(bool visible);
	void setHomePlanet(planet *homeplanet);
	planet *getHomePlanet();
	void setTargetArrayVisible(bool visible);
	bool getWithinRadarRange();

	//station functions
	int getStarshipCreationTime();
	void setStarshipCreationTime(int time);
	int getFighterCreationTime();
	void setFighterCreationTime(int time);
	vector2d<int> getArrayPos();

	void setTarget(CShip *target);
	CShip *getTarget();

	//ai state and role functions
	void setState(AI_STATE state);
	AI_STATE getState();
	void setRole(SHIP_ROLE role);
	SHIP_ROLE getRole();

	std::vector<turret*> getTurretManager();
	std::vector<turret*> getSecondaryTurretManager();
	std::vector<turret*> getLightTurretManager();
	std::vector<item*> getInventory();
	void setInventory(std::vector<item*> in);
	void addItemToInventory(item *add);
	bool itemInInventory(item *check);
	const wchar_t *test;

	int getNumFighters();
	void setNumFighters(int fighters);
	void modNumFighters(int fighters);

	//ecostuff
	float getModifierBuy();
	float getModifierSell();
	float getModifierInDemand();
	float getModifierBuyAmount();
private:
	int numFighters;
	int maxFighters;


	irrklang::ISound *engine_sound;
	core::vector2d<s32> array_pos;

	std::vector<turret*> turret_manager;
	std::vector<turret*> secondary_turret_manager;
	std::vector<turret*> light_turret_manager;
	std::vector<item*> inventory;

	//ship stat variables

	bool hostile;
	bool shoot;

	bool warp;
	bool isStation;
	int station_ship_creation_time;
	int fighter_creation_time;
	planet* home_planet;
	

	CShip *target;
	planet *jump_target;
	bool target_player;

	int hullPoints;
	int armorPoints;
	int shieldPoints;

	int maxVelocity;
	float velocity;
	float maxTurn;
	
	ship_faction faction;


	int energy;
	int maxEnergy;

	int maxShieldPoints;
	int maxArmorPoints;
	int maxHullPoints;

	core::vector3df *ship_position;
	core::vector3df *ship_rotation;

	AI_STATE state;
	SHIP_ROLE role;

	//ship station economy variables
	float buy_modifier;
	float sell_modifier;
	float indemand_modifier;
	float buy_amount_modifier;

	ShaderCallBack *callback;

};


#endif _SHIP_H

