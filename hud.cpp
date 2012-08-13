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
#include "hud.h"


using namespace irr;
using namespace scene;
using namespace gui;

//the hud
//here be dragons.

int mfd_x = 140;

core::dimension2d<u32> t;
//initalize hud
Hud::Hud(irr::IrrlichtDevice *graphics, Player *CPlayer)
{
	this->CPlayer = CPlayer;
	this->graphics = graphics;
	

	//setup fonts
	t = graphics->getVideoDriver()->getScreenSize();
	gui::IGUIFont *large_font= graphics->getGUIEnvironment()->getFont("res/font/verdana_large.xml");
	gui::IGUIFont *small_font = graphics->getGUIEnvironment()->getFont("res/font/verdana_small.xml");
	gui::IGUIFont *micro = graphics->getGUIEnvironment()->getFont("res/font/system.xml");
	gui::IGUIFont *astro = graphics->getGUIEnvironment()->getFont("res/font/astro.xml");
	graphics->getGUIEnvironment()->getSkin()->setFont(micro);

	hud_image = graphics->getGUIEnvironment()->addImage(graphics->getVideoDriver()->getTexture("res/menu/hud.png"),core::position2d<s32>(-90,t.Height-190));
	target_icon = graphics->getGUIEnvironment()->addImage(graphics->getVideoDriver()->getTexture("res/menu/target.png"),core::vector2d<s32>(0,0));
	target_icon->setVisible(false);

	planet_target_icon = graphics->getGUIEnvironment()->addImage(graphics->getVideoDriver()->getTexture("res/menu/target.png"),core::vector2d<s32>(0,0));
	planet_target_icon->setVisible(false);

	

	//Create all the text shown on the screen
	//Ship Information
	velocity = graphics->getGUIEnvironment()->addStaticText(L"velocity",rect<int>(10,t.Height-110,t.Width/2+35,t.Height),false);
	if(velocity)
	{
		velocity->setOverrideColor(video::SColor(255,255,255,255));
		velocity->setOverrideFont(large_font);

	}
	hull = graphics->getGUIEnvironment()->addStaticText(L"hull",rect<s32>(10,t.Height-70,120,t.Height));
	if(hull)
	{
		hull->setOverrideColor(video::SColor(255,255,255,255));
		hull->setOverrideFont(small_font);
	}
	armor = graphics->getGUIEnvironment()->addStaticText(L"armor",rect<s32>(10,t.Height-55,120,t.Height));
	if(armor)
	{
		armor->setOverrideColor(video::SColor(255,255,255,255));
		armor->setOverrideFont(small_font);
	}
	shield = graphics->getGUIEnvironment()->addStaticText(L"shield",rect<s32>(10,t.Height-40,120,t.Height));
	if(shield)
	{
		shield->setOverrideColor(video::SColor(255,255,255,255));
		shield->setOverrideFont(small_font);
	}

	cannons_primary = graphics->getGUIEnvironment()->addStaticText(L"Primary Turrets",rect<s32>(200,t.Height-40,400,t.Height));
	if(cannons_primary)
	{
		cannons_primary->setOverrideColor(video::SColor(255,255,255,255));
		cannons_primary->setOverrideFont(small_font);
	}
	cannons_primary_range = graphics->getGUIEnvironment()->addStaticText(L"Railguns [5500m]",rect<s32>(200,t.Height-20,400,t.Height));
	if(cannons_primary_range)
	{
		cannons_primary_range->setOverrideColor(video::SColor(255,255,255,255));
		cannons_primary_range->setOverrideFont(small_font);
	}
	cannons_secondary = graphics->getGUIEnvironment()->addStaticText(L"Secondary Turrets", rect<s32>(400,t.Height-40,600,t.Height));
	if(cannons_secondary)
	{
		cannons_secondary->setOverrideColor(video::SColor(255,255,255,255));
		cannons_secondary->setOverrideFont(small_font);
	}
	cannons_secondary_range = graphics->getGUIEnvironment()->addStaticText(L"Antimatter Cannons [4000m]",rect<s32>(400,t.Height-20,600,t.Height));
	if(cannons_secondary_range)
	{
		cannons_secondary_range->setOverrideColor(video::SColor(255,255,255,255));
		cannons_secondary_range->setOverrideFont(small_font);
	}
	//End Ship Information

	//Start Target Information
	target_info = graphics->getGUIEnvironment()->addImage(graphics->getVideoDriver()->getTexture("res/menu/target_info.png"),core::position2d<s32>(t.Width-mfd_x,20));

	target = graphics->getGUIEnvironment()->addStaticText(L"No Target Selected",rect<s32>(t.Width-mfd_x,20,t.Width-20,140));
	if(target)
	{
		target->setOverrideColor(video::SColor(255,255,255,255));
		target->setOverrideFont(micro);
	}
	target_ship = graphics->getGUIEnvironment()->addStaticText(L"",rect<s32>(t.Width-mfd_x,35,t.Width-20,140));
	if(target_ship)
	{
		target_ship->setOverrideColor(video::SColor(255,255,255,255));
		target_ship->setOverrideFont(micro);
	}
	target_hull = graphics->getGUIEnvironment()->addStaticText(L"",rect<s32>(t.Width-mfd_x,65,t.Width-20,140));
	if(target_hull)
	{
		target_hull->setOverrideColor(video::SColor(255,255,255,255));
		target_hull->setOverrideFont(micro);
	}
	target_armor = graphics->getGUIEnvironment()->addStaticText(L"",rect<s32>(t.Width-mfd_x,80,t.Width-20,140));
	if(target_armor)
	{
		target_armor->setOverrideColor(video::SColor(255,255,255,255));
		target_armor->setOverrideFont(micro);
	}
	target_shield = graphics->getGUIEnvironment()->addStaticText(L"",rect<s32>(t.Width-mfd_x,95,t.Width-20,140));
	if(target_shield)
	{
		target_shield->setOverrideColor(video::SColor(255,255,255,255));
		target_shield->setOverrideFont(micro);
	}
	target_distance = graphics->getGUIEnvironment()->addStaticText(L"",rect<s32>(t.Width-mfd_x,110,t.Width-20,140));
	if(target_distance)
	{
		target_distance->setOverrideColor(video::SColor(255,255,255,255));
		target_distance->setOverrideFont(micro);
	}
	//end target information

	//start planet information
	planet_info = graphics->getGUIEnvironment()->addImage(graphics->getVideoDriver()->getTexture("res/menu/target_info.png"),core::position2d<s32>(t.Width-mfd_x,150));

	planet_target = graphics->getGUIEnvironment()->addStaticText(L"No Planet Selected", rect<s32>(t.Width-mfd_x,150,t.Width-20,250));
	if(planet_target)
	{
		planet_target->setOverrideColor(video::SColor(255,255,255,255));
		planet_target->setOverrideFont(micro);
	}
	planet_distance = graphics->getGUIEnvironment()->addStaticText(L"",rect<s32>(t.Width-mfd_x,180,t.Width-20,250));
	if(planet_distance)
	{
		planet_distance->setOverrideColor(video::SColor(255,255,255,255));
		planet_distance->setOverrideFont(micro);
	}
	planet_type = graphics->getGUIEnvironment()->addStaticText(L"",rect<s32>(t.Width-mfd_x,195,t.Width-20,250));
	if(planet_type)
	{
		planet_type->setOverrideColor(video::SColor(255,255,255,255));
		planet_type->setOverrideFont(micro);
	}
	planet_relations = graphics->getGUIEnvironment()->addStaticText(L"",rect<s32>(t.Width-mfd_x,210,t.Width,250));
	if(planet_relations)
	{
		planet_relations->setOverrideColor(video::SColor(255,255,255,255));
		planet_relations->setOverrideFont(micro);
	}
	//end planet hud information
	

	//etc
	view = graphics->getGUIEnvironment()->addStaticText(L"VIEW : TACTICAL", rect<s32>(20,10,400,400));
	if(view)
	{
		view->setOverrideColor(video::SColor(255,255,255,255));
		view->setOverrideFont(astro);
	}
	location_system = graphics->getGUIEnvironment()->addStaticText(L"LOCATION : TAU CETI", rect<s32>(20,40,400,400));
	if(location_system)
	{
		location_system->setOverrideColor(video::SColor(255,255,255,255));
		location_system->setOverrideFont(astro);
	}
}

//delete everything
Hud::~Hud()
{
	//long list of the same stuff
	velocity->remove();
	hull->remove();
	armor->remove();
	shield->remove();
	cannons_primary->remove();
	cannons_primary_range->remove();
	cannons_secondary->remove();
	cannons_secondary_range->remove();
	//cannons_light->remove();
	target->remove();
	target_ship->remove();
	target_distance->remove();
	target_hull->remove();
	target_armor->remove();
	target_shield->remove();

	planet_target->remove();
	planet_distance->remove();
	planet_type->remove();
	planet_relations->remove();
	

	target_icon->remove();
	planet_target_icon->remove();
	hud_image->remove();
	target_info->remove();
	planet_info->remove();
	
	view->remove();
	location_system->remove();
}

void Hud::drop()
{
	delete this;
}

void Hud::updateHud(CShip* player_target, planet* jump_target)
{
	//update all the strings on the hud
	if(velocity)
	{
		int v = CPlayer->getVelocity();
		core::stringw velo_str(L"");
		velo_str+=v;
		velo_str+=L"m/s";
		velocity->setText(velo_str.c_str());
	}
	if(hull)
	{
		int h = CPlayer->getHull();
		core::stringw hull_str(L"Hull [");
		hull_str += h;
		hull_str+= L"]";
		hull->setText(hull_str.c_str());
	}
	if(armor)
	{
		int a = CPlayer->getArmor();
		core::stringw armor_str(L"Armor [");
		armor_str += a;
		armor_str+= L"]";
		armor->setText(armor_str.c_str());
	}
	if(shield)
	{
		int s = CPlayer->getShield();
		if( s < 0)
			s=0;
		core::stringw shield_str(L"Shield [");
		shield_str +=s;
		shield_str+=L"]";
		shield->setText(shield_str.c_str());
	}
	/*
	outdated
	if(cannons_primary)
	{
		//gets the recharge time of player cannons
		//converts it to a percent value
		int primary_recharge = 2000;
		float primary = ((graphics->getTimer()->getTime()+primary_recharge) - CPlayer->cannonFired.primary_time);
		if(primary>primary_recharge)
			primary=primary_recharge;
		primary=primary/primary_recharge;
		primary=primary*100;
		int c = primary;
		core::stringw cannon_p_str(L"Primary Turrets [");
		cannon_p_str+=c;
		cannon_p_str+=L"%]";
		cannons_primary->setText(cannon_p_str.c_str());

	}
	if(cannons_secondary)
	{
		int secondary_recharge = 500;
		float secondary = ((graphics->getTimer()->getTime()+secondary_recharge) - CPlayer->cannonFired.secondary_time);
		if(secondary>secondary_recharge)
		{
			secondary = secondary_recharge;
		}
		secondary=secondary/secondary_recharge;
		secondary=secondary*100;
		int c = secondary;
		core::stringw cannon_s_str(L"Secondary Turrets [");
		cannon_s_str+=c;
		cannon_s_str+="%]";
		cannons_secondary->setText(cannon_s_str.c_str());
	}
	*/
	if(target)
	{
		if(player_target!=0)
		{
			
			core::vector2d<s32> pos = graphics->getSceneManager()->getSceneCollisionManager()->getScreenCoordinatesFrom3DPosition(player_target->getPos(),graphics->getSceneManager()->getActiveCamera());
			target_icon->setVisible(true);
			target_icon->setRelativePosition(core::vector2d<s32>(pos.X-32,pos.Y-32));
			graphics->getVideoDriver()->draw2DLine(vector2d<int>(pos.X,0),vector2d<int>(pos.X,t.Height),video::SColor(128,128,128,128));
			graphics->getVideoDriver()->draw2DLine(vector2d<int>(0,pos.Y),vector2d<int>(t.Width,pos.Y),video::SColor(128,128,128,128));

			//ship
			stringw tship = player_target->getShipClass()->name;
			target_ship->setText(tship.c_str());

			//hull
			int th = player_target->getHullPoints();
			core::stringw target_hull_str(L"Hull [");
			target_hull_str+=th;
			target_hull_str+=L"]";
			target_hull->setText(target_hull_str.c_str());

			//armor
			int ta = player_target->getArmorPoints();
			core::stringw target_armor_str(L"Armor [");
			target_armor_str+=ta;
			target_armor_str+=L"]";
			target_armor->setText(target_armor_str.c_str());

			//shields
			int ts = player_target->getShieldPoints();
			if (ts<0)
				ts = 0;
			core::stringw target_shield_str(L"Shield [");
			target_shield_str+=ts;
			target_shield_str+=L"]";
			target_shield->setText(target_shield_str.c_str());

			//distance
			int dist = getDistance(player_target);
			core::stringw target_dist_str(L"");
			target_dist_str+=dist;
			target_dist_str+=L"m";
			target_distance->setText(target_dist_str.c_str());

			target->setText(player_target->getName());
		}
		else
		{
			//if theres no target
			//hide everything
			target_icon->setVisible(false);
			target->setText(L"No Target Selected");
			target_hull->setText(L"");
			target_ship->setText(L"");
			target_armor->setText(L"");
			target_shield->setText(L"");
			target_distance->setText(L"");
		}
	}

	if(planet_target)
	{
		if(jump_target!=0)
		{

			core::vector2d<s32> pos = graphics->getSceneManager()->getSceneCollisionManager()->getScreenCoordinatesFrom3DPosition(jump_target->getPos(),graphics->getSceneManager()->getActiveCamera());
			planet_target_icon->setVisible(true);
			planet_target_icon->setRelativePosition(core::vector2d<s32>(pos.X-32,pos.Y-32));
			graphics->getVideoDriver()->draw2DLine(vector2d<int>(pos.X,0),vector2d<int>(pos.X,t.Height),video::SColor(128,128,128,128));
			graphics->getVideoDriver()->draw2DLine(vector2d<int>(0,pos.Y),vector2d<int>(t.Width,pos.Y),video::SColor(128,128,128,128));

			//distance
			int p_dst = getDistancePlanet(jump_target);
			core::stringw p_dist_str(L"");
			p_dist_str+=p_dst;
			p_dist_str+=L"km";
			planet_distance->setText(p_dist_str.c_str());

			//name
			planet_target->setText(jump_target->getName());

			//planet faction
			if(jump_target->getFaction()==FACTION_NEUTRAL)
			{
				planet_relations->setText(L"Neutral");
			}
			if(jump_target->getFaction()==FACTION_TERRAN_FEDERATION)
			{
				planet_relations->setText(L"Terran Federation");
			}
			if(jump_target->getFaction()==FACTION_PROVIAN_CONSORTIUM)
			{
				planet_relations->setText(L"Provian Consortium");
			}
			if(jump_target->getFaction()==FACTION_PIRATE)
			{
				planet_relations->setText(L"Pirates");
			}
			planet_type->setText(jump_target->getPlanetType()->name);
		}
		else
		{
			planet_target_icon->setVisible(false);
			planet_target->setText(L"No Planet Selected");
			planet_distance->setText(L"");
			planet_type->setText(L"");
			planet_relations->setText(L"");
		}
	}
}

//gets the distance from player to target
int Hud::getDistance(CShip* player_target)
{
	float z = CPlayer->getPos().Z-player_target->getPos().Z;
	float y = CPlayer->getPos().Y-player_target->getPos().Y;
	float x = CPlayer->getPos().X-player_target->getPos().X;

	float dist = (sqrt(pow(x,2)+pow(y,2)+pow(z,2)));

	return dist;
}

//returns distance from planet in kilometers
float Hud::getDistancePlanet(planet* jump_target)
{
	float z = CPlayer->getPos().Z - jump_target->getPos().Z;
	float y = CPlayer->getPos().Y - jump_target->getPos().Y;
	float x = CPlayer->getPos().X - jump_target->getPos().X;
	float dist = (sqrt(x*x+y*y+z*z))/1000.f;
	return dist;
}

