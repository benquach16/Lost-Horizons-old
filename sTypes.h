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

#ifndef _STYPES_H_
#define _STYPES_H_

//If you are wondering what this file is
//its a massive definitions file
//containing all the definitions for all the ships, items, planets, etc.
//Any and all objects in the game should be defined in this file
//So everything is in one place, easy to modify n shit

#include "stdafx.h"
#include "irrlicht.h"
#include "vector"



//Finite State Machines
//Hopefully
//My AI programming is basically terrible
enum AI_STATE
{
	STATE_SEARCH,
	STATE_ATTACK,
	STATE_DEFEND,
	STATE_FLEE,
	STATE_REARM,
};


//game factions
enum ship_faction
{
	FACTION_TERRAN_FEDERATION=0,
	FACTION_PROVIAN_CONSORTIUM=1,
	FACTION_PIRATE=2,
	FACTION_NEUTRAL=3
};

//only ship and station should be here
enum ship_types 
{
	TYPE_SHIP,
	TYPE_STATION,
};

//turret classes
//heavy turrets should only be used in capital ships
//primary turrets should be used in heavier warships
//secondary turrets should be used in light warships
//light turrets are fighter and missile defense
enum good_types
{
	TYPE_HEAVY,
	TYPE_PRIMARY,
	TYPE_SECONDARY,
	TYPE_LIGHT,
	TYPE_GENERAL,
	TYPE_CONSTRUCTION,
};

//crew types
enum crew_types
{
	TYPE_OFFICER,
	TYPE_CHEF,
	TYPE_CREWMEMBER,
	TYPE_MEDIC,
	TYPE_MARINE,
	TYPE_ENGINEER,
};

//dispose of this later
enum turret_class
{
	TYPE_TURRET_RAIL,						// railgun				- primary
	TYPE_TURRET_PHOTON,						// photon cannon		- primary
	TYPE_TURRET_PD,							// point defense		- light
	TYPE_TURRET_FLAK,						// flak cannons			- light
	TYPE_TURRET_ANTIMATTER,					// antimatter cannons	- secondary
	TYPE_TURRET_PLASMA,						// plasma cannon		- secondary
	TYPE_TURRET_ION_CANNON,					// ion cannon			- heavy
	TYPE_TURRET_KRYON_TORPEDO,				// kryon torpedo		- heavy
	TYPE_TURRET_HEAVY_AUTOCANNON,			// heavy autocannon		- primary
	TYPE_TURRET_AUTOCANNON,					// autocannon			- secondary
	TYPE_TURRET_FUSION,						// fusion accelerator   - heavy
	TYPE_TURRET_SIEGE_DRIVER,				// siege driver			- heavy
	TYPE_TURRET_PHANTOM_MISSILE,			// phantom missiles		- secondary
	TYPE_TURRET_BANSHEE_MISSILE,			// banshee missile		- primary
	TYPE_TURRET_CRUISE_MISSILE				// cruise missile		- heavy
};




//item base class
class item_base
{
public:
	wchar_t *name;
	wchar_t *description;
	int index;	//used for saving/loading
	good_types type;
	int cost;
	int weight;
};


class planet_base
{
public:
	wchar_t *name;
	wchar_t *description;
	int rotation_speed;
	bool corona;
	bool clouds;
	irr::core::vector3df scale;
	irr::core::vector3df cloud_scale;
	irr::core::stringc cloud_map;
	irr::core::stringc planet_map;
	irr::core::stringc planet_model;

	int material_antimatter;
	int material_heavy_metals;
	int material_kryozine;
	int material_nanoalloy;
	int bonus_morale;
	int bonus_food;
	int bonus_population;
	int regen_time;
};

class ship_base
{
public:
	//base definition for all ships
	int index;
	wchar_t *name;
	wchar_t *description;
	bool station;
	

	int hullPoints;
	int armorPoints;
	int shieldPoints;
	float maxTurn;
	int maxVelocity;
	int energy;
	int cost;
	int cargoSpace;
	int maxFighters;
	int maxCrewSize;

	int num_turrets;
	int num_secondary_turrets;
	int num_light_turrets;
	int num_heavy_turrets;

	wchar_t *full_arc_turret_front;
	wchar_t *full_arc_turret_rear;


	irr::core::stringc ship;
	irr::core::stringc normal_map;
	irr::core::vector3df scale;
	bool contrails;

};

class fighter_base
{
public:
	int index;
	wchar_t *name;
	wchar_t *description;

	int hullPoints;
	float maxTurn;
	int maxVelocity;

	int cost;
	int fuel;

	irr::core::stringc ship;
	irr::core::vector3df scale;
};


//turret class
class turret_base : public item_base
{
public:


	int rotation_speed;
	int reload_time;


	wchar_t *model;
	wchar_t *icon;
	irr::core::vector3df scale;
};

//for use in managing ships
class officer_base
{
public:
	wchar_t *name;
	wchar_t *title;
	int exp;
	wchar_t *bonuses;
};




//long one
class items
{
public:
	item_base *WATER;
	item_base *MILK;
	item_base *RATIONS;
	item_base *HYDROGEN;
	item_base *HEAVY_METALS;
	item_base *KRYOZINE;
	item_base *NANOALLOY;
	item_base *ANTIMATTER;
	item_base *ELECTRONICS;
	item_base *ANTIBIOTICS;
	item_base *HQ_CONSTRUCTION;
	item_base *SHIPYARD_CONSTRUCTION;
	turret_base *PRI_RAIL;
	turret_base *PRI_PHOTON;
	turret_base *SEC_ANTIMATTER;
	turret_base *SEC_PLASMA;
	turret_base *LIGHT_GATLING;
	turret_base *PRI_GAUSS;
	turret_base *PRI_PARTICLE;
	

	//used in saving/loading
	//and it might be useful later
	std::vector<item_base*> item_list;
	std::vector<turret_base*> turret_list;
	items()
	{
		WATER = new item_base;
		WATER->index = 0;
		WATER->name = L"Water";
		WATER->description = L"Water is a common but important commodity bought and sold by space traders";
		WATER->cost = 15;
		WATER->weight = 2;
		WATER->type = TYPE_GENERAL;
		item_list.push_back(WATER);

		MILK = new item_base;
		MILK->index = 1;
		MILK->name = L"Milk";
		MILK->description = L"Milk is made from cows";
		MILK->cost = 60;
		MILK->weight = 1;
		item_list.push_back(MILK);

		RATIONS = new item_base;
		RATIONS->index = 2;
		RATIONS->name = L"Rations";
		RATIONS->description = L"Rations contain all the food needed by things that are alive. Essential for maintaining a population on a planet.";
		RATIONS->cost = 20;
		RATIONS->weight  =5;
		item_list.push_back(RATIONS);

		HYDROGEN = new item_base;
		HYDROGEN->index = 3;
		HYDROGEN->name = L"Hydrogen";
		HYDROGEN->description = L"Hydrogen is a flammable gas that has many uses throughout the known universe, from fuel to explosives";
		HYDROGEN->cost = 50;
		HYDROGEN->weight = 5;
		item_list.push_back(HYDROGEN);

		HEAVY_METALS = new item_base;
		HEAVY_METALS->index = 4;
		HEAVY_METALS->name = L"Heavy Metals";
		HEAVY_METALS->description = L"Heavy metals is the name for the hyper-strong composite materials used in the production of ships and space stations, found primarily in barren planets";
		HEAVY_METALS->cost = 980;
		HEAVY_METALS->weight = 200;
		item_list.push_back(HEAVY_METALS);

		KRYOZINE = new item_base;
		KRYOZINE->index = 5;
		KRYOZINE->name = L"Kryozine";
		KRYOZINE->description = L"Kryozine is an essential gas harvested from atmospheres, that is used in the production of energy weapons and shields";
		KRYOZINE->cost = 520;
		KRYOZINE->weight = 50;
		item_list.push_back(KRYOZINE);

		NANOALLOY = new item_base;
		NANOALLOY->index = 6;
		NANOALLOY->name = L"Nano Alloys";
		NANOALLOY->description = L"Nano Alloys are super-light and strong materials, used in various important parts of spacecraft, such as engines and turret barrels, and is manufactured in planets with major industrial complexes";
		NANOALLOY->cost = 400;
		NANOALLOY->weight = 200;
		item_list.push_back(NANOALLOY);

		ANTIMATTER = new item_base;
		ANTIMATTER->index = 7;
		ANTIMATTER->name = L"Antimatter";
		ANTIMATTER->description = L"Antimatter is a volitile substance that is used in the production of ship engines";
		ANTIMATTER->cost = 800;
		ANTIMATTER->weight = 100;
		item_list.push_back(ANTIMATTER);

		ELECTRONICS = new item_base;
		ELECTRONICS->index = 8;
		ELECTRONICS->name = L"Electronics";
		ELECTRONICS->description = L"Electronics are components used to build devices that are used throughout the known universe";
		ELECTRONICS->cost = 55;
		ELECTRONICS->weight = 10;
		item_list.push_back(ELECTRONICS);

		ANTIBIOTICS = new item_base;
		ANTIBIOTICS->index = 8;
		ANTIBIOTICS->name = L"Antibiotics";
		ANTIBIOTICS->description = L"Valuable goods that are need to fight diseases. Incredibly rare in the outer rim colonies";
		ANTIBIOTICS->cost = 300;
		ANTIBIOTICS->weight = 10;
		item_list.push_back(ANTIBIOTICS);

		//railguns
		PRI_RAIL = new turret_base;
		PRI_RAIL->index = 9;
		PRI_RAIL->name = L"Railguns";
		PRI_RAIL->description = L"Railguns use magnetic rails to hurl projectiles at high velocity, giving it extremely long range";
		PRI_RAIL->type = TYPE_PRIMARY;
		PRI_RAIL->rotation_speed = 20;
		PRI_RAIL->reload_time = 1000;
		PRI_RAIL->cost = 11500;
		PRI_RAIL->weight = 60;
		PRI_RAIL->model = L"res/models/equipment/primary_rail.x";
		PRI_RAIL->icon = L"res/menu/loadout/rail.png";
		PRI_RAIL->scale = irr::core::vector3df(0.8,0.8,0.8);
		item_list.push_back(PRI_RAIL);
		turret_list.push_back(PRI_RAIL);

		//photon
		PRI_PHOTON = new turret_base;
		PRI_PHOTON->index =10;
		PRI_PHOTON->name = L"Photon Cannons";
		PRI_PHOTON->description = L"Photon Cannons charges photon particles and fires them at the enemy ship, causing immense damage";
		PRI_PHOTON->type = TYPE_PRIMARY;
		PRI_PHOTON->rotation_speed=10;
		PRI_PHOTON->reload_time=1200;
		PRI_PHOTON->cost=12300;
		PRI_PHOTON->weight = 60;
		PRI_PHOTON->model = L"res/models/equipment/primary_rail.x";
		PRI_PHOTON->icon = L"res/menu/loadout/rail.png";
		PRI_PHOTON->scale = irr::core::vector3df(0.8,0.8,0.8);
		item_list.push_back(PRI_PHOTON);
		turret_list.push_back(PRI_PHOTON);

		//antimatter
		SEC_ANTIMATTER = new turret_base;
		SEC_ANTIMATTER->index =11;
		SEC_ANTIMATTER->name = L"Antimatter Cannons";
		SEC_ANTIMATTER->description = L"This weapon fires antimatter stored inside of a projectile, which explodes violently when it strikes another ship";
		SEC_ANTIMATTER->type = TYPE_SECONDARY;
		SEC_ANTIMATTER->rotation_speed = 40;
		SEC_ANTIMATTER->reload_time = 500;
		SEC_ANTIMATTER->cost = 5500;
		SEC_ANTIMATTER->weight = 30;
		SEC_ANTIMATTER->model = L"res/models/equipment/secondary_am.x";
		SEC_ANTIMATTER->icon = L"res/menu/loadout/rail.png";
		SEC_ANTIMATTER->scale = irr::core::vector3df(0.8,0.8,0.8);
		item_list.push_back(SEC_ANTIMATTER);
		turret_list.push_back(SEC_ANTIMATTER);

		//plasma
		SEC_PLASMA = new turret_base;
		SEC_PLASMA->index = 12;
		SEC_PLASMA->name = L"Plasma Cannons";
		SEC_PLASMA->description = L"Plasma cannons fire superheated particles that does more damage than the Terran Antimatter cannon";
		SEC_PLASMA->type = TYPE_SECONDARY;
		SEC_PLASMA->rotation_speed = 35;
		SEC_PLASMA->reload_time = 600;
		SEC_PLASMA->cost = 5500;
		SEC_PLASMA->weight = 20;
		SEC_PLASMA->model = L"res/models/equipment/secondary_am.x";
		SEC_PLASMA->icon = L"res/menu/loadout/rail.png";
		SEC_PLASMA->scale = irr::core::vector3df(0.8,0.8,0.8);
		item_list.push_back(SEC_PLASMA);
		turret_list.push_back(SEC_PLASMA);

		//gatling
		LIGHT_GATLING = new turret_base;
		LIGHT_GATLING->index = 13;
		LIGHT_GATLING->name = L"Gatling Point-Defense Turret";
		LIGHT_GATLING->description = L"A gatling turret is very effective versus fighters and missiles, but useless versus capital warships";
		LIGHT_GATLING->type = TYPE_LIGHT;
		LIGHT_GATLING->rotation_speed = 310;
		LIGHT_GATLING->reload_time = 300;
		LIGHT_GATLING->cost = 3600;
		LIGHT_GATLING->weight = 20;
		LIGHT_GATLING->model = L"res/models/equipment/light_gatling.x";
		LIGHT_GATLING->icon = L"res/menu/loadout/rail.png";
		LIGHT_GATLING->scale = irr::core::vector3df(0.8,0.8,0.8);
		item_list.push_back(LIGHT_GATLING);
		turret_list.push_back(LIGHT_GATLING);

		PRI_GAUSS = new turret_base;
		PRI_GAUSS->index = 14;
		PRI_GAUSS->name = L"Gauss Cannon";
		PRI_GAUSS->description = L"Gauss Cannons use multiple coils to accelerate a projectile to high speeds";
		PRI_GAUSS->type = TYPE_PRIMARY;
		PRI_GAUSS->rotation_speed = 20;
		PRI_GAUSS->reload_time = 1050;
		PRI_GAUSS->cost = 12500;
		PRI_GAUSS->weight = 20;

	}
};

class planets
{
public:
	planet_base *HABITABLE;
	planet_base *GAS;
	planet_base *INFERNO;
	planet_base *OCEAN;
	planet_base *ICE;
	planet_base *MOON;
	planet_base *ASTEROIDS;
	planet_base *BARREN;
	planets()
	{
		HABITABLE = new planet_base;
		HABITABLE->name = L"Habitable";
		HABITABLE->description = L"Habitable type planets are valuable due to their unique ability to sustain life.";
		HABITABLE->rotation_speed = 0.1;
		HABITABLE->scale = irr::core::vector3df(16000,16000,16000);
		HABITABLE->planet_model = "res/models/planets/habitable.x";
		HABITABLE->cloud_map = "res/textures/planets/1.png";
		HABITABLE->cloud_scale = irr::core::vector3df(16200,16200,16200);
		HABITABLE->material_antimatter = 500;
		HABITABLE->material_kryozine = 500;
		HABITABLE->material_heavy_metals = 500;
		HABITABLE->regen_time = 3600;
		HABITABLE->clouds=true;
		HABITABLE->corona=true;

		BARREN = new planet_base;
		BARREN->name = L"Barren";
		BARREN->description = L"Barren type planets tend to have many resources but are not suitable for mass human population";
		BARREN->rotation_speed = 0.1;
		BARREN->scale = irr::core::vector3df(16000,16000,16000);
		BARREN->planet_model = "res/models/planets/habitable.x";
		BARREN->cloud_map = "res/textures/planets/1.png";
		BARREN->cloud_scale = irr::core::vector3df(16200,16200,16200);
		BARREN->material_antimatter = 100;
		BARREN->material_kryozine = 0;
		BARREN->material_heavy_metals = 1500;
		BARREN->regen_time = 3000;
		BARREN->clouds=true;
		BARREN->corona=true;

		MOON = new planet_base;
		MOON->name = L"Moon";
		MOON->description = L"Moons are rocks that orbit real planets";
		MOON->rotation_speed = 0.1;
		MOON->scale = irr::core::vector3df(3000,3000,3000);
		MOON->planet_model = "res/models/planets/habitable.x";
		MOON->material_antimatter = 0;
		MOON->material_heavy_metals = 400;
		MOON->material_kryozine = 400;
		MOON->material_nanoalloy =100;
		MOON->regen_time = 5000;
		MOON->clouds=false;
		MOON->corona=false;
	

		GAS = new planet_base;
		GAS->name = L"Gas Giant";
		GAS->rotation_speed = 1;
		GAS->scale =  irr::core::vector3df(30000,30000,30000);
		GAS->planet_model = "res/models/planets/habitable.x";
		GAS->cloud_map = "res/textures/planets/2.png";
		GAS->cloud_scale = irr::core::vector3df(30500,30500,30500);
		GAS->clouds=true;
		GAS->corona = true;

		ASTEROIDS = new planet_base;
		ASTEROIDS->name = L"Asteroids";
		ASTEROIDS->description = L"Provides great places for pirates to hide in";
		ASTEROIDS->rotation_speed = 1;
		ASTEROIDS->scale = irr::core::vector3df(8,8,8);
		ASTEROIDS->planet_model = "res/models/planets/asteroid.x";
		ASTEROIDS->cloud_scale = irr::core::vector3df(1,1,1);
		ASTEROIDS->clouds=false;
		ASTEROIDS->corona=false;
	}
};

class fighters
{
public:
	fighter_base *TERR_DRAGONFLY;
	fighter_base *PROV_RAPTOR;

	std::vector<fighter_base*> fighter_list;
	fighters()
	{
		TERR_DRAGONFLY = new fighter_base;
		TERR_DRAGONFLY->index = 0;
		TERR_DRAGONFLY->name = L"Dragonfly Class Fighter";
		TERR_DRAGONFLY->description = L"The Dragonfly is a fast fighter that is very well rounded and capable of any task";
		TERR_DRAGONFLY->hullPoints = 20;
		TERR_DRAGONFLY->maxTurn = 50;
		TERR_DRAGONFLY->maxVelocity = 210;
		TERR_DRAGONFLY->cost = 1200;
		TERR_DRAGONFLY->ship = "res/models/ships/fighter.x";
		TERR_DRAGONFLY->scale = irr::core::vector3df(0.3,0.3,0.3);
		TERR_DRAGONFLY->fuel = 360000;
		fighter_list.push_back(TERR_DRAGONFLY);

		PROV_RAPTOR = new fighter_base;
		PROV_RAPTOR->index = 1;
		PROV_RAPTOR->name = L"Raptor Class Fighter";
		PROV_RAPTOR->description = L"The Raptor is an extremely speedy interceptor that is excellent at destroying targets during the first run";
		PROV_RAPTOR->hullPoints = 20;
		PROV_RAPTOR->maxTurn = 30;
		PROV_RAPTOR->maxVelocity = 300;
		PROV_RAPTOR->cost = 1500;
		PROV_RAPTOR->ship = "res/models/ships/raptor.x";
		PROV_RAPTOR->scale = irr::core::vector3df(0.3,0.3,0.3);
		PROV_RAPTOR->fuel = 240000;
		fighter_list.push_back(PROV_RAPTOR);

	}
};


//data for all ships
// frigates are small, mobile anti-fighter flak ships
// destroyers are fast patrol ships
// cruisers are the most balanced ships in the arsenal
// battlecruisers are glass cannons, little armor/shields but lots of heavy weapons
// dreadnoughts are slow ships with tons of defenses and firepower
// titans are ridiculously huge with only heavy cannons mounts, meant for sieging positions
// carriers are a good fleet support ship
// motherships can create ships on the fly, like in homeworld
//		mostly just a joke though
class ships
{
public:
	ship_base *TERR_IMPERIUM_FRIGATE;		//Terran Federation 'Imperium' Class Frigate
	ship_base *TERR_LEGION_DESTROYER;		//Terran Federation 'Legion' Class Destroyer
	ship_base *TERR_PRAETORIAN_CRUISER;		//Terran Federation 'Praetorian' Class Cruiser
	ship_base *TERR_QUAESTOR_BATTLECRUISER;	//Terran Federation 'Quaestor' Class Battlecruiser
	ship_base *TERR_DOMINION_DREAD;			//Terran Federation 'Dominion' Class Dreadnought
	ship_base *TERR_CENTURION_TITAN;		//Terran Federation 'Centurion' Class Titan
	ship_base *TERR_TIBERIUS_CARRIER;		//Terran Federation 'Tiberius' Class Carrier

	ship_base *PROV_CELESTIS_FRIGATE;		//Provian Consortium 'Celestis' Class Frigate
	ship_base *PROV_ARES_DESTROYER;			//Provian Consortium 'Ares' Class Destroyer
	ship_base *PROV_ISHTAR_CRUISER;			//Provian Consortium 'Ishtar' Class Cruiser
	ship_base *PROV_ITERION_BATTLECRUISER;	//Provian Consortium 'Iterion' Class Battlecruiser
	ship_base *PROV_AETHER_DREAD;			//Provian Consortium 'Aether' Class Dreadnought
	ship_base *PROV_ANSHAR_TITAN;			//Provian Consortium 'Anshar' Class Titan
	ship_base *PROV_THANATOS_CARRIER;		//Provian Consortium 'Thanatos' Class Carrier

	ship_base *TRADING_STATION;				//Civilian Trade Station
	ship_base *MINING_STATION;
	ship_base *SHIPYARD;					//Shipyard
	ship_base *EQUIPMENT_STATION;			//Upgrade Station
	ship_base *BATTLE_STATION;				//Battle Station (lol)
	ship_base *HQ;							//HQ

	//mainly used to catagorize ships and load from files
	std::vector<ship_base*> ship_list;

	//names for ships
	std::vector<const wchar_t*> terran_ship_name;
	std::vector<const wchar_t*> provian_ship_name;
	std::vector<const wchar_t*> ship_name_extra;

	ships()
	{

		//ship names
		//terran_ship_name.push_back(L"'TFS-Ecstasy of Gold'");
		//terran_ship_name.push_back(L"'TFS-Sword of Judgement'");
		terran_ship_name.push_back(L"'TFS-Agamemnon'");
		terran_ship_name.push_back(L"'TFS-Hyperion'");
		terran_ship_name.push_back(L"'TFS-Ordinator'");
		terran_ship_name.push_back(L"'TFS-Aegis'");
		terran_ship_name.push_back(L"'TFS-Excalibur'");
		terran_ship_name.push_back(L"'TFS-Talos'");
		terran_ship_name.push_back(L"'TFS-Daedalus'");
		terran_ship_name.push_back(L"'TFS-Damocles'");
		terran_ship_name.push_back(L"'TFS-Andromeda'");
		terran_ship_name.push_back(L"'TFS-Dire Wolf'");
		//provian_ship_name.push_back(L"'PCV-Wrath of God'");
		//provian_ship_name.push_back(L"'PCV-Spear of Achilles'");
		provian_ship_name.push_back(L"'PCV-Gallipoli'");
		provian_ship_name.push_back(L"'PCV-Eberlin'");
		provian_ship_name.push_back(L"'PCV-Istanbul'");
		provian_ship_name.push_back(L"'PCV-Enfali'");
		provian_ship_name.push_back(L"'PCV-Antilles'");
		provian_ship_name.push_back(L"'PCV-Palisade'");
		provian_ship_name.push_back(L"'PCV-Prometheus'");
		provian_ship_name.push_back(L"'PCV-Atlas'");
		provian_ship_name.push_back(L"'PCV-Calypso'");
		provian_ship_name.push_back(L"'PCV-Honor Guard'");

		//trading station
		TRADING_STATION = new ship_base;
		TRADING_STATION->index = 0;
		TRADING_STATION->name = L"Trading Station";
		TRADING_STATION->description = L"The trading station is a civilian space station where goods are bought and sold";
		TRADING_STATION->station = true;
		TRADING_STATION->hullPoints = 20000;
		TRADING_STATION->armorPoints = 0;
		TRADING_STATION->shieldPoints = 10000;
		TRADING_STATION->maxTurn = 0;
		TRADING_STATION->maxVelocity = 0;
		TRADING_STATION->num_heavy_turrets = 0;
		TRADING_STATION->ship="res/models/stations/trade.x";
		TRADING_STATION->normal_map="res/models/stations/trade_normal.jpg";
		TRADING_STATION->scale = irr::core::vector3df(7,7,7);
		TRADING_STATION->contrails = false;
		TRADING_STATION->energy=10000;
		TRADING_STATION->maxFighters=10;
		ship_list.push_back(TRADING_STATION);

		//mining station
		MINING_STATION = new ship_base;
		MINING_STATION->index=1;
		MINING_STATION->name = L"Mining Station";
		MINING_STATION->description = L"Mining stations are generally found around asteroids, in order to harvest minerals";
		MINING_STATION->station=true;
		MINING_STATION->hullPoints=10000;
		MINING_STATION->armorPoints=5000;
		MINING_STATION->shieldPoints=1000;
		MINING_STATION->maxTurn=0;
		MINING_STATION->maxVelocity=0;
		MINING_STATION->num_heavy_turrets=0;
		MINING_STATION->num_turrets = 0;
		MINING_STATION->ship="res/models/stations/mine.3ds";
		MINING_STATION->normal_map="res/models/stations/knormal.jpg";
		MINING_STATION->scale=irr::core::vector3df(7,7,7);
		MINING_STATION->contrails=false;
		MINING_STATION->maxFighters=0;
		ship_list.push_back(MINING_STATION);

		//shipyard
		SHIPYARD = new ship_base;
		SHIPYARD->index=2;
		SHIPYARD->name = L"Shipyard";
		SHIPYARD->description = L"Shipyards are orbital construction platforms used to create and develop powerful warships";
		SHIPYARD->station = true;
		SHIPYARD->hullPoints = 10000;
		SHIPYARD->armorPoints=5000;
		SHIPYARD->shieldPoints = 10000;
		SHIPYARD->maxTurn=0;
		SHIPYARD->maxVelocity = 0;
		SHIPYARD->num_heavy_turrets = 0;
		SHIPYARD->num_turrets = 0;
		SHIPYARD->num_secondary_turrets=0;
		SHIPYARD->num_light_turrets = 0;
		SHIPYARD->ship = "res/models/stations/shipyard.x";
		SHIPYARD->normal_map = "res/textures/ships/shiphull_normal.jpg";
		SHIPYARD->scale = irr::core::vector3df(2.2,2.2,2.2);
		SHIPYARD->contrails = false;
		SHIPYARD->maxFighters = 0;
		ship_list.push_back(SHIPYARD);

		//equipment station
		EQUIPMENT_STATION = new ship_base;
		EQUIPMENT_STATION->index=3;
		EQUIPMENT_STATION->name = L"Equipment Station";
		EQUIPMENT_STATION->description = L"Equipment Stations are orbital platforms that rearm and equip starships";
		ship_list.push_back(EQUIPMENT_STATION);

		//battle station
		BATTLE_STATION = new ship_base;
		BATTLE_STATION->index=4;
		BATTLE_STATION->name = L"Battle Station";
		BATTLE_STATION->description = L"Battle Stations are powerful orbital defense stations that have the capacitor to fire powerful weapons that cannot be used on warships";
		ship_list.push_back(BATTLE_STATION);

		//headquarters
		HQ = new ship_base;
		HQ->index=5;
		HQ->name = L"Headquarters";
		HQ->description = L"Headquarters are stations responsible for the coordination of local forces. These are required to deploy allied starships";
		HQ->station = true;
		HQ->hullPoints = 40000;
		HQ->armorPoints = 40000;
		HQ->shieldPoints = 10000;
		HQ->maxTurn = 0;
		HQ->maxVelocity = 0;
		HQ->num_heavy_turrets = 0;
		HQ->num_turrets = 0;
		HQ->num_light_turrets = 0;
		HQ->num_secondary_turrets = 0;
		HQ->ship = "res/models/stations/station.3ds";
		HQ->normal_map = "res/models/stations/bump.jpg";
		HQ->scale = irr::core::vector3df(70,70,70);
		HQ->contrails = false;
		HQ->energy = 10000;
		HQ->maxFighters = 10;
		ship_list.push_back(HQ);

		//imperium frigate
		TERR_IMPERIUM_FRIGATE = new ship_base;
		TERR_IMPERIUM_FRIGATE->index=6;
		TERR_IMPERIUM_FRIGATE->name = L"Imperium Class Frigate (FF)";
		TERR_IMPERIUM_FRIGATE->description = L"The Imperium Class Frigate is a light and fast warship commonly found within Terran tradelanes and is used as an anti-figher flak warship in fleet operations";
		ship_list.push_back(TERR_IMPERIUM_FRIGATE);

		//legion destroyer
		TERR_LEGION_DESTROYER = new ship_base;
		TERR_LEGION_DESTROYER->index=7;
		TERR_LEGION_DESTROYER->name = L"Legion Class Destroyer (DD)";
		TERR_LEGION_DESTROYER->description = L"The Legion Class Destroyer is a fast starship commonly used by the Terran Navy as a fast patrol vessel, essential for defeating light enemy warships and holding ground until reinforcements arrive";
		TERR_LEGION_DESTROYER->station = false;
		TERR_LEGION_DESTROYER->hullPoints = 1200;
		TERR_LEGION_DESTROYER->armorPoints = 900;
		TERR_LEGION_DESTROYER->shieldPoints = 1500;
		TERR_LEGION_DESTROYER->maxTurn = 15;
		TERR_LEGION_DESTROYER->maxVelocity = 156;
		TERR_LEGION_DESTROYER->num_turrets = 2;
		TERR_LEGION_DESTROYER->num_secondary_turrets = 5;
		TERR_LEGION_DESTROYER->num_light_turrets = 6;
		TERR_LEGION_DESTROYER->num_heavy_turrets = 0;
		TERR_LEGION_DESTROYER->ship ="res/models/ships/legion.x";
		TERR_LEGION_DESTROYER->normal_map ="res/textures/ships/shiphull_normal.jpg";
		TERR_LEGION_DESTROYER->maxCrewSize = 700;
		TERR_LEGION_DESTROYER->scale = irr::core::vector3df(0.25f,0.25f,0.25f);
		TERR_LEGION_DESTROYER->energy = 600;
		TERR_LEGION_DESTROYER->contrails = true;
		TERR_LEGION_DESTROYER->maxFighters = 4;
		ship_list.push_back(TERR_LEGION_DESTROYER);

		//praetorian cruiser
		TERR_PRAETORIAN_CRUISER = new ship_base;
		TERR_PRAETORIAN_CRUISER->index = 8;
		TERR_PRAETORIAN_CRUISER->name = L"Praetorian Class Cruiser (CA)";
		TERR_PRAETORIAN_CRUISER->description = L"The Praetorian Class Cruiser comprises the bulk of the Terran Federation Fleet. With multiple secondary and primary turrets, the Praetorian Class Cruiser is a flexible ship fit for any situation";
		TERR_PRAETORIAN_CRUISER->station = false;
		TERR_PRAETORIAN_CRUISER->hullPoints = 2000;
		TERR_PRAETORIAN_CRUISER->armorPoints = 1000;
		TERR_PRAETORIAN_CRUISER->shieldPoints = 2000;
		TERR_PRAETORIAN_CRUISER->maxTurn = 10;
		TERR_PRAETORIAN_CRUISER->maxVelocity = 118;
		TERR_PRAETORIAN_CRUISER->num_turrets = 4;
		TERR_PRAETORIAN_CRUISER->full_arc_turret_front = L"turret_03";
		TERR_PRAETORIAN_CRUISER->full_arc_turret_rear = L"turret_04";
		TERR_PRAETORIAN_CRUISER->num_secondary_turrets = 4;
		TERR_PRAETORIAN_CRUISER->num_light_turrets = 4;
		TERR_PRAETORIAN_CRUISER->num_heavy_turrets = 1;
		TERR_PRAETORIAN_CRUISER->ship="res/models/ships/praetorian.x";
		TERR_PRAETORIAN_CRUISER->normal_map = "res/textures/ships/shiphull_normal.jpg";
		TERR_PRAETORIAN_CRUISER->scale = irr::core::vector3df(0.3f,0.3f,0.3f);
		TERR_PRAETORIAN_CRUISER->contrails=true;
		TERR_PRAETORIAN_CRUISER->energy=1000;
		TERR_PRAETORIAN_CRUISER->maxCrewSize=1200;
		TERR_PRAETORIAN_CRUISER->maxFighters=10;
		ship_list.push_back(TERR_PRAETORIAN_CRUISER);

		//quaestor battlecruiser
		TERR_QUAESTOR_BATTLECRUISER = new ship_base;
		TERR_QUAESTOR_BATTLECRUISER->index=9;
		TERR_QUAESTOR_BATTLECRUISER->name = L"Quaestor Class Battlecruiser (CC)";
		TERR_QUAESTOR_BATTLECRUISER->description = L"The Quaestor Battlecruiser is the Terran Fleet's hit and run warship. It uses its speed to quickly strike at slower Provian warships of the same tonnage, and its many turrets to issue massive damage";
		ship_list.push_back(TERR_QUAESTOR_BATTLECRUISER);

		//dominion dreadnought
		TERR_DOMINION_DREAD = new ship_base;
		TERR_DOMINION_DREAD->index=10;
		TERR_DOMINION_DREAD->name = L"Dominion Class Dreadnought (DN)";
		TERR_DOMINION_DREAD->description = L"The Dominion Dreadnought is the powerhouse of the Terran Fleet. It's massive shield generators allow it to sustain combat for long periods of time, and its many heavy turrets allows it to annihilate enemy fleets at extreme range";
		ship_list.push_back(TERR_DOMINION_DREAD);
		
		//centurion titan
		TERR_CENTURION_TITAN = new ship_base;
		TERR_CENTURION_TITAN->index = 11;
		TERR_CENTURION_TITAN->name = L"Centurion Class Titan (TN)";
		TERR_CENTURION_TITAN->description = L"The Centurion Class Titan is the pinnacle of Terran starship design. This massive ship extends to over 2 kilometers long and can be equipped with powerful siege weapons that crushes enemy stations and positions far beyond their combat range";
		ship_list.push_back(TERR_CENTURION_TITAN);

		//tiberius carrier
		TERR_TIBERIUS_CARRIER = new ship_base;
		TERR_TIBERIUS_CARRIER->index = 12;
		TERR_TIBERIUS_CARRIER->name = L"Tiberius Class Carrier (CV)";
		TERR_TIBERIUS_CARRIER->description = L"Carriers are excellent for deploying large amounts of fighters to strike at other enemy warships or to provide cover for friendly ships";
		ship_list.push_back(TERR_TIBERIUS_CARRIER);

		//celestis frigate
		PROV_CELESTIS_FRIGATE = new ship_base;
		PROV_CELESTIS_FRIGATE->index = 13;
		PROV_CELESTIS_FRIGATE->name = L"Celestis Class Frigate (FF)";
		PROV_CELESTIS_FRIGATE->description = L"The Celestis Class Frigate is the primary escort vessel of the Provian Armada. Although slower than its Terran counterpart, its powerful light turrets can tear apart any fighter swarm that gets too close";
		ship_list.push_back(PROV_CELESTIS_FRIGATE);

		//ares destroyer
		PROV_ARES_DESTROYER = new ship_base;
		PROV_ARES_DESTROYER->index=14;
		PROV_ARES_DESTROYER->name = L"Ares Class Destroyer (DD)";
		PROV_ARES_DESTROYER->description = L"The Ares Destroyer is a powerful patrol ship, capable of taking on much larger ships, and is able to quickly establish control of sectors";
		PROV_ARES_DESTROYER->station = false;
		PROV_ARES_DESTROYER->hullPoints = 1200;
		PROV_ARES_DESTROYER->armorPoints = 1200;
		PROV_ARES_DESTROYER->shieldPoints = 300;
		PROV_ARES_DESTROYER->maxVelocity = 147;
		PROV_ARES_DESTROYER->maxTurn = 15;
		PROV_ARES_DESTROYER->num_turrets = 2;
		PROV_ARES_DESTROYER->num_secondary_turrets = 5;
		PROV_ARES_DESTROYER->num_light_turrets = 6;
		PROV_ARES_DESTROYER->num_heavy_turrets = 0;
		PROV_ARES_DESTROYER->ship = "res/models/ships/ares.x";
		PROV_ARES_DESTROYER->normal_map = L"res/textures/ships/shiphull_bump.jpg";
		PROV_ARES_DESTROYER->scale = irr::core::vector3df(0.36,0.36,0.36);
		PROV_ARES_DESTROYER->contrails = true;
		PROV_ARES_DESTROYER->maxFighters = 4;
		ship_list.push_back(PROV_ARES_DESTROYER);

		//ishtar cruiser
		PROV_ISHTAR_CRUISER = new ship_base;
		PROV_ISHTAR_CRUISER->index=15;
		PROV_ISHTAR_CRUISER->name = L"Ishtar Class Cruiser (CA)";
		PROV_ISHTAR_CRUISER->description = L"The Ishtar Class Cruiser is the primary ship of the Provian Consortium Armada. It is fast and has the ability to absorb damage with its heavy armor compliment";
		PROV_ISHTAR_CRUISER->station = false;
		PROV_ISHTAR_CRUISER->hullPoints = 2000;
		PROV_ISHTAR_CRUISER->armorPoints = 1500;
		PROV_ISHTAR_CRUISER->shieldPoints = 500;
		PROV_ISHTAR_CRUISER->maxVelocity = 107;
		PROV_ISHTAR_CRUISER->maxTurn = 10;
		PROV_ISHTAR_CRUISER->num_turrets = 4;
		PROV_ISHTAR_CRUISER->num_secondary_turrets = 4;
		PROV_ISHTAR_CRUISER->num_light_turrets = 4;
		PROV_ISHTAR_CRUISER->num_heavy_turrets = 1;
		PROV_ISHTAR_CRUISER->ship="res/models/ships/ishtar.x";
		PROV_ISHTAR_CRUISER->normal_map = "res/textures/ships/shiphull_bump.jpg";
		PROV_ISHTAR_CRUISER->scale = irr::core::vector3df(0.22f,0.22f,0.22f);
		PROV_ISHTAR_CRUISER->contrails=true;
		PROV_ISHTAR_CRUISER->energy=1000;
		PROV_ISHTAR_CRUISER->maxCrewSize=1200;
		PROV_ISHTAR_CRUISER->maxFighters=10;
		ship_list.push_back(PROV_ISHTAR_CRUISER);

	}
};

class officers
{
public:
	officer_base *helmsman;
	officer_base *fire_control;
	officer_base *engineering;
	officers()
	{
		helmsman = new officer_base;
		helmsman->name = L"Dan Calamari";
		helmsman->title = L"Helmsman";
		helmsman->exp = 100;
		helmsman->bonuses = L"+5 Turning Speed, +1 Ship Speed";

		fire_control = new officer_base;
		fire_control->name = L"Kara Ashbeck";
		fire_control->title = L"Fire Control Officer";
		fire_control->exp = 100;
		fire_control->bonuses = L"+1 Firing Rate, +5 Point Defense Accuracy";
		
		engineering = new officer_base;
		engineering->name = L"Vincent deLuca";
		engineering->title = L"Engineering Officer";
		engineering->exp = 100;
		engineering->bonuses = L"+1 Repair Rate";


	}
};

#endif _STYPES_H_


