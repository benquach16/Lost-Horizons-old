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
#include "menu.h"


video::SColor warning = video::SColor(255,255,255,0);
video::SColor bad = video::SColor(255,255,0,0);

//initializer
CMenu::CMenu(irr::IrrlichtDevice *graphics, irrklang::ISoundEngine *sound, KeyListener *receiver, Player *CPlayer)
{
	this->graphics=graphics;
	this->sound = sound;
	this->receiver = receiver;

	selected_mission = 0;
	cargoTab.cargo_temp = 0;
	cargoTab.cargo_button_pressed=false;

	t = graphics->getVideoDriver()->getScreenSize();
	gui::IGUIFont *micro = graphics->getGUIEnvironment()->getFont("res/font/system.xml");

	graphics->getGUIEnvironment()->getSkin()->setFont(micro);




	//set window size for menu
	window_size = core::rect<s32>(100, 50, t.Width- 100, t.Height-50);

	//create each of the tabs
	control_tab = graphics->getGUIEnvironment()->addTabControl(window_size,0,true);
	ship = control_tab->addTab(L"Ship",1);
	cargo = control_tab->addTab(L"Cargo",2);
	crew = control_tab->addTab(L"Crew",3);
	loadout = control_tab->addTab(L"Loadout",4);
	hanger = control_tab->addTab(L"Hanger",5);
	missions = control_tab->addTab(L"Mission Log",6);
	control_tab->setVisible(false);
	
	setupTabs();
}

CMenu::~CMenu()
{
	control_tab->remove();
}

void CMenu::drop()
{
	delete this;
}


//setup each of the gui elements in each tab
//each element of the tab pages are stored in a struct so you can read it easier
void CMenu::setupTabs()
{
	//ship tab
	shipTab.ship = graphics->getGUIEnvironment()->addStaticText(L"", 
		core::rect<s32>(10,20,window_size.getWidth() - 10,100),
		false,true,ship);

	shipTab.description = graphics->getGUIEnvironment()->addStaticText(L"",
		core::rect<s32>(10,60,window_size.getWidth() - 10,100),
		false,true,ship);

	shipTab.subsystems = graphics->getGUIEnvironment()->addStaticText(L"Ship Subsystems",
		rect<s32>(10,120,window_size.getWidth()-10,window_size.getHeight()-10),
		true,true,ship);

	shipTab.systems_list = graphics->getGUIEnvironment()->addListBox(
		rect<s32>(20,140,window_size.getWidth()/2-10,window_size.getHeight()-20),ship);
	shipTab.systems_health = graphics->getGUIEnvironment()->addStaticText(L"Integrity : ",
		rect<s32>(window_size.getWidth()/2,140,window_size.getWidth()-40,220),
		false,true,ship);
	shipTab.crew_avail = graphics->getGUIEnvironment()->addStaticText(L"Crew Available : ",
		rect<s32>(window_size.getWidth()/2,170,window_size.getWidth()-40,190),
		false,true,ship);
	shipTab.crew_req = graphics->getGUIEnvironment()->addStaticText(L"Crew Required : ",
		rect<s32>(window_size.getWidth()/2,200,window_size.getWidth()-40,220),
		false,true,ship);
	//is there a better way to do this?
	//oh yeah.
	shipTab.systems_list->addItem(L"Bridge");
	shipTab.systems_list->addItem(L"Deck 1");
	shipTab.systems_list->addItem(L"Deck 2");
	shipTab.systems_list->addItem(L"Elevator");
	shipTab.systems_list->addItem(L"Engine");
	shipTab.systems_list->addItem(L"Warp Drive");
	shipTab.systems_list->addItem(L"Shield Module");
	shipTab.systems_list->addItem(L"Powerplant");
	shipTab.systems_list->addItem(L"Primary Weapon System");
	shipTab.systems_list->addItem(L"Secondary Weapon System");
	shipTab.systems_list->addItem(L"Light Weapon System");
	shipTab.repair = graphics->getGUIEnvironment()->addButton(rect<s32>(window_size.getWidth()-240,window_size.getHeight()-60,window_size.getWidth()-140,window_size.getHeight()-40)
		,ship,-1,L"Repair");
	shipTab.replace = graphics->getGUIEnvironment()->addButton(rect<s32>(window_size.getWidth()-120,window_size.getHeight()-60,window_size.getWidth()-20,window_size.getHeight()-40)
		,ship,-1,L"Replace");

	//cargo tab
	cargoTab.cargo_list = graphics->getGUIEnvironment()->addListBox(core::rect<s32>(10,20, window_size.getWidth()/4,window_size.getHeight()-100),cargo);
	cargoTab.cargo_selected_description = graphics->getGUIEnvironment()->addStaticText(L"Description:",rect<s32>
																						(window_size.getWidth()/2,20,window_size.getWidth()-10,window_size.getHeight()-100)
																						,true,true,cargo);
	cargoTab.cargo_selected_cost = graphics->getGUIEnvironment()->addStaticText(L"Cost:",rect<s32>(window_size.getWidth()/4,20,window_size.getWidth()/4+window_size.getWidth()/8,window_size.getHeight()-100),true,true,cargo);
	cargoTab.cargo_selected_weight = graphics->getGUIEnvironment()->addStaticText(L"Weight:",rect<s32>(window_size.getWidth()/4+window_size.getWidth()/8,20,window_size.getWidth()/2,window_size.getHeight()-100),true,true,cargo);
	cargoTab.cargo_remove = graphics->getGUIEnvironment()->addButton(rect<s32>(10,window_size.getHeight()-60,100,window_size.getHeight()-40),cargo,-1,L"Jettison Cargo");
	cargoTab.money = graphics->getGUIEnvironment()->addStaticText(L"Credits :", rect<s32>(100,window_size.getHeight()-60,300,window_size.getHeight()-40),false,true,cargo);

	crewTab.officers = graphics->getGUIEnvironment()->addTreeView(rect<s32>(10,10,window_size.getWidth()/2-10,window_size.getHeight()/2),crew);
	crewTab.officers_node = crewTab.officers->getRoot()->addChildBack(L"Ship Officers");
	crewTab.crew_node = crewTab.officers->getRoot()->addChildBack(L"Extra Crew");
	crewTab.prisioners_node = crewTab.officers->getRoot()->addChildBack(L"Prisioners");
	crewTab.passengers_node = crewTab.officers->getRoot()->addChildBack(L"Passengers");
	crewTab.helmsman = crewTab.officers_node->addChildBack(L"Helmsman");
	crewTab.firecontrol = crewTab.officers_node->addChildBack(L"Fire Control Officer");
	crewTab.engineering = crewTab.officers_node->addChildBack(L"Engineering Officer");
	crewTab.navigation = crewTab.officers_node->addChildBack(L"Navigation Officer");
	crewTab.radar = crewTab.officers_node->addChildBack(L"Radar Officer");

	crewTab.officer_name = graphics->getGUIEnvironment()->addStaticText(L"Name :", rect<s32>(window_size.getWidth()/2,10,window_size.getWidth()-10,window_size.getHeight()/2),true,true,crew);
	crewTab.officer_title = graphics->getGUIEnvironment()->addStaticText(L"Position :", rect<s32>(window_size.getWidth()/2,30,window_size.getWidth()-10,window_size.getHeight()/2),true,true,crew);
	crewTab.officer_experience = graphics->getGUIEnvironment()->addStaticText(L"Experience :", rect<s32>(window_size.getWidth()/2,50,window_size.getWidth()-10,window_size.getHeight()/2),true,true,crew);
	crewTab.officer_bonus = graphics->getGUIEnvironment()->addStaticText(L"Bonuses :", rect<s32>(window_size.getWidth()/2,70,window_size.getWidth()-10,window_size.getHeight()/2),true,true,crew);
	crewTab.crew_num = graphics->getGUIEnvironment()->addStaticText(L"Crew Size: ", rect<s32>(10,window_size.getHeight()/2+15,window_size.getWidth()-10,window_size.getHeight()-10),true,true,crew);
	crewTab.crew_morale = graphics->getGUIEnvironment()->addStaticText(L"Crew Morale: 100%", rect<s32>(10,window_size.getHeight()/2+55,window_size.getWidth()-10,window_size.getHeight()-10),false,true,crew);
	

	loadoutTab.weapons = graphics->getGUIEnvironment()->addStaticText(L"Weapon Systems",rect<s32>(10,10,window_size.getWidth()/2-10,300),true,true,loadout);
	loadoutTab.primary_name = graphics->getGUIEnvironment()->addStaticText(L"Primary Weapon Slot",rect<s32>(98,30,window_size.getWidth()-20,160),false,true,loadout);
	loadoutTab.primary_slot = graphics->getGUIEnvironment()->addImage(
		graphics->getVideoDriver()->getTexture(L"res/menu/loadout/slot.png"),vector2d<s32>(20,30),true,loadout);
	loadoutTab.primary_description = graphics->getGUIEnvironment()->addStaticText(L"Primary Weapon Description", rect<s32>(98,50,window_size.getWidth()-20,110),false,true,loadout);

	loadoutTab.secondary_name = graphics->getGUIEnvironment()->addStaticText(L"Secondary Weapon Slot", rect<s32>(98,114,window_size.getWidth()-20,180),false,true,loadout);
	loadoutTab.secondary_slot = graphics->getGUIEnvironment()->addImage(
		graphics->getVideoDriver()->getTexture(L"res/menu/loadout/slot.png"),vector2d<s32>(20,114),true,loadout);
	loadoutTab.secondary_description = graphics->getGUIEnvironment()->addStaticText(L"Secondary Weapon Description", rect<s32>(98,144,window_size.getWidth()-20,190),false,true,loadout);

	loadoutTab.light_name = graphics->getGUIEnvironment()->addStaticText(L"Light Weapon Slot", rect<s32>(98,198,window_size.getWidth()-20,220),false,true,loadout);
	loadoutTab.light_slot = graphics->getGUIEnvironment()->addImage(
		graphics->getVideoDriver()->getTexture(L"res/menu/loadout/slot.png"), vector2d<s32>(20,198),true,loadout);
	loadoutTab.light_description = graphics->getGUIEnvironment()->addStaticText(L"Light Weapon Description", rect<s32>(98,228,window_size.getWidth()-20,250),false,true,loadout);

	hangerTab.fighter_list = graphics->getGUIEnvironment()->addTreeView(rect<s32>(10,10,window_size.getWidth()/2,window_size.getHeight()-40),hanger);
	hangerTab.fighters = hangerTab.fighter_list->getRoot()->addChildBack(L"Fighters");
	hangerTab.shuttles = hangerTab.fighter_list->getRoot()->addChildBack(L"Shuttles");
	hangerTab.recall = graphics->getGUIEnvironment()->addButton(rect<s32>(window_size.getWidth()-140,window_size.getHeight()-90,window_size.getWidth()-10,window_size.getHeight()-70),hanger,-1,L"Recall Fighters");
	hangerTab.send = graphics->getGUIEnvironment()->addButton(rect<s32>(window_size.getWidth()-140,window_size.getHeight()-130,window_size.getWidth()-10,window_size.getHeight()-110),hanger,-1,L"Launch Fighters");

	missionTab.mission_list = graphics->getGUIEnvironment()->addListBox(rect<s32>(10,10,window_size.getWidth()/2,window_size.getHeight()-100),missions);
	missionTab.mission_description = graphics->getGUIEnvironment()->addStaticText(L"",rect<s32>(window_size.getWidth()/2,10,window_size.getWidth()-10,window_size.getHeight()/2-20),true,true,missions);
	missionTab.objective_list = graphics->getGUIEnvironment()->addListBox(rect<s32>(window_size.getWidth()/2,window_size.getHeight()/2-10,window_size.getWidth()-10,window_size.getHeight()-10),missions);
	
}


//main loop for the menu
//runs all of the tabs
void CMenu::menuLoop(Player *CPlayer, missionManager *missionM)
{
	dimension2d<u32> t = graphics->getVideoDriver()->getScreenSize();
	//menu_open variable controlled in the gameloop object
	if(menu_open==true)
	{
		control_tab->setVisible(true);
		runShip(CPlayer);
		runCargo(CPlayer);
		runCrew(CPlayer);
		runLoadout(CPlayer);
		runHanger(CPlayer);
		runMissions(missionM);
	}
	else
	{
		control_tab->setVisible(false);
	}
}

//function to control the ship tab of the player menu
//called from menuLoop func
void CMenu::runShip(Player *CPlayer)
{
	shipTab.ship->setText(CPlayer->getCurrentShip()->name);
	shipTab.description->setText(CPlayer->getCurrentShip()->description);
	stringw crew_av = L"Crew Available : ";
	crew_av+=CPlayer->crew.num;

	shipTab.crew_avail->setText(crew_av.c_str());
	int tmp = shipTab.systems_list->getSelected();

	if(tmp!=-1)
	{
		//if player selected a subsystem
		int i;
		int repair_c;

		if(tmp==0)
		{
			//BRIDGE
			i = CPlayer->subsystem.bridge;
		}
		if(tmp==1)
		{
			//Deck 1
			i = CPlayer->subsystem.deck1;
		}
		if(tmp==2)
		{
			//Deck 2
			i = CPlayer->subsystem.deck2;
		}
		if(tmp==3)
		{
			//elevator
			i = CPlayer->subsystem.elevator;
		}
		if(tmp==4)
		{
			//engine
			i = CPlayer->subsystem.engine;
		}
		if(tmp==5)
		{
			//warp
			i = CPlayer->subsystem.warpdrive;
		}
		if(tmp==6)
		{
			//shield
			i = CPlayer->subsystem.shield;
		}
		if(tmp==7)
		{
			//power
			i = CPlayer->subsystem.power;
		}
		if(tmp==8)
		{
			//primary
			i = CPlayer->subsystem.primaryweapons;
		}
		if(tmp==9)
		{
			//secondary
			i = CPlayer->subsystem.secondaryweapons;
		}
		if(tmp==10)
		{
			//light
			i = CPlayer->subsystem.lightweapons;
		}
		//shud be no more than 10
		stringw t = L"Integrity : ";
		t+=i;
		t+=L"%";
		shipTab.systems_health->setText(t.c_str());

		stringw crew_req = L"Crew Required : ";
		repair_c = 5*(100-i);
		crew_req+=repair_c;
		shipTab.crew_req->setText(crew_req.c_str());

		if(shipTab.repair->isPressed())
		{
			//if enough crew is avail
			if(repair_c<CPlayer->crew.num)
			{
				if(i==0)
				{
					CPlayer->subsystem.bridge=100;
				}
			}
		}
	}
}


/*
lemme figure out a good way to arrage cargo into a list....
*/
void CMenu::runCargo(Player *CPlayer)
{
	if(cargoTab.cargo_list->getItemCount()!=CPlayer->getCargo().size())
	{

		//disgusting hack
		//but it fucking works
		//see, the elegant and clean way would to simply delete everything on the list
		//but it doesn't fucking work
		//so i just deleted the entire list and created a new one
		//cargoTab.cargo_list->remove();
		cargoTab.cargo_list->clear();
		//cargoTab.cargo_list = graphics->getGUIEnvironment()->addListBox(core::rect<s32>(10,20, window_size.getWidth()/4,window_size.getHeight()-100),cargo);
		for(int i=0; i<CPlayer->getCargo().size(); i++)
		{
			cargoTab.cargo_list->addItem(CPlayer->getCargo()[i]->getName());
		}
	}

	//set the stats to the selected item
	int tmp = cargoTab.cargo_list->getSelected();	
	if(tmp!=-1)
	{
		core::stringw desc,price,weight;
		desc = L"Description:";
		price = L"Cost:";
		weight = L"Weight:";
		desc+=CPlayer->getCargo()[tmp]->getItemDescription();
		price+=CPlayer->getCargo()[tmp]->getItemCost();
		weight+=CPlayer->getCargo()[tmp]->getItemWeight();
		price+=L"credits";
		weight+=L"kg";
		cargoTab.cargo_selected_description->setText(desc.c_str());
		cargoTab.cargo_selected_cost->setText(price.c_str());
		cargoTab.cargo_selected_weight->setText(weight.c_str());
	}
	else
	{
		cargoTab.cargo_selected_description->setText(L"Description:");
		cargoTab.cargo_selected_cost->setText(L"Cost:");
		cargoTab.cargo_selected_weight->setText(L"Weight:");
	}
	

	//apparently buttons do shit as long as the button is held down
	//so the variables are necessary
	//oh well, better than coding my own gui system
	//that would be a bitch
	if(cargoTab.cargo_remove->isPressed())
	{
		if(cargoTab.cargo_button_pressed==false)
		{
			if(tmp!=-1)
			{
				CPlayer->removeItemFromCargo(CPlayer->getCargo()[tmp]);
				cargoTab.cargo_button_pressed=true;
			}
		}
	}
	else
	{
		cargoTab.cargo_button_pressed=false;
	}

	stringw money_str = L"Credits :";
	money_str+=CPlayer->getMoney();
	cargoTab.money->setText(money_str.c_str());
}

void CMenu::runCrew(Player *CPlayer)
{
	//get crewnums from player
	if(crewTab.crew_num)
	{
		stringw t = L"Crew Size : ";
		int max = CPlayer->crew.maxnum;
		int i = CPlayer->crew.num;
		t+=i;
		t+=L"/";
		t+=max;
		crewTab.crew_num->setText(t.c_str());
	}
	//set morale
	if(crewTab.crew_morale)
	{
		stringw t = L"Crew Morale : ";
		int morale = CPlayer->crew.morale;
		t+=morale;
		t+=L"%";
		crewTab.crew_morale->setText(t.c_str());
	}

	//crewman selection
	if(crewTab.helmsman->getSelected())
	{
		stringw str = L"Position :";
		str+=officers().helmsman->title;
		crewTab.officer_title->setText(str.c_str());

		stringw name = L"Name :";
		name+=officers().helmsman->name;
		crewTab.officer_name->setText(name.c_str());

		stringw bonus = L"Bonuses :";
		bonus+=officers().helmsman->bonuses;
		crewTab.officer_bonus->setText(bonus.c_str());
	}
	if(crewTab.firecontrol->getSelected())
	{
		stringw str = L"Position :";
		str+=officers().fire_control->title;
		crewTab.officer_title->setText(str.c_str());

		stringw name = L"Name :";
		name+=officers().fire_control->name;
		crewTab.officer_name->setText(name.c_str());

		stringw bonus = L"Bonuses :";
		bonus+=officers().fire_control->bonuses;
		crewTab.officer_bonus->setText(bonus.c_str());	
	}
}

void CMenu::runLoadout(Player *CPlayer)
{
	//show the slots and the weapons inside of them
	loadoutTab.primary_name->setText(CPlayer->turret_manager[0]->getName());
	loadoutTab.primary_slot->setImage(CPlayer->turret_manager[0]->getIcon());
	loadoutTab.primary_description->setText(CPlayer->turret_manager[0]->getItemDescription());

	loadoutTab.secondary_name->setText(CPlayer->secondary_turret_manager[0]->getName());
	loadoutTab.secondary_slot->setImage(CPlayer->secondary_turret_manager[0]->getIcon());
	loadoutTab.secondary_description->setText(CPlayer->secondary_turret_manager[0]->getItemDescription());

	loadoutTab.light_name->setText(CPlayer->light_turret_manager[0]->getName());
	loadoutTab.light_slot->setImage(CPlayer->light_turret_manager[0]->getIcon());
	loadoutTab.light_description->setText(CPlayer->light_turret_manager[0]->getItemDescription());
}

void CMenu::runHanger(Player *CPlayer)
{
	if(hangerTab.fighters->getChildCount()!=CPlayer->getNumFighters())
	{
		hangerTab.fighters->clearChilds();
		for(int i=0; i<CPlayer->getNumFighters(); i++)
			hangerTab.fighters->addChildBack(CPlayer->getFighterType()->name);
	}

}

void CMenu::runMissions(missionManager *missionM)
{
	//update the missions list continously with the mission manager
	if(missionTab.mission_list->getItemCount()!=missionM->getMissionList().size())
	{
		for(unsigned int i = 0; i<missionM->getMissionList().size();i++)
		{
			missionTab.mission_list->addItem(missionM->getMissionList()[i]->getTitle());
		}
	}
	int tmp = missionTab.mission_list->getSelected();

	//if player selected something
	//display mission description and objectives
	if(tmp!=-1)
	{
		//theres a vector out of range bug when the selected mission is complete
		if(missionM->getMissionList().size()>=tmp)
		{
			//also set that as current mission
			selected_mission = missionM->getMissionList()[tmp];
			missionTab.mission_description->setText(missionM->getMissionList()[tmp]->getDesc());

			//OPTIMIZE THIS PLEASE
			//done
			missionTab.objective_list->clear();
			//missionTab.objective_list->remove();
			//missionTab.objective_list = graphics->getGUIEnvironment()->addListBox(rect<s32>(window_size.getWidth()/2,window_size.getHeight()/2-10,window_size.getWidth()-10,window_size.getHeight()-10),missions);
			if(missionTab.objective_list->getItemCount()!=missionM->getMissionList()[tmp]->getObjectives().size())
			{
				for(unsigned int i=0; i<missionM->getMissionList()[tmp]->getObjectives().size();i++)
				{
					missionTab.objective_list->addItem(missionM->getMissionList()[tmp]->getObjectives()[i]->getTitle());
				}
			}
			else
			{
				//if nothin selected
				//ensure player doesnt have active mission
				selected_mission = 0;
			}
		}
		else
			tmp=-1;
	}
}

//give the selected mission
//to the gameloop object
//so that we can do stuff with it
CMission *CMenu::getSelectedMission()
{
	if(selected_mission!=0)
	{
		return selected_mission;
	}
	else
	{
		return 0;
	}
}

void CMenu::setMenuOpen(bool state)
{
	menu_open = state;
}

bool CMenu::getMenuOpen()
{
	return menu_open;
}
