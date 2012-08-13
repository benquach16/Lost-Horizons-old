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
#include "dockmenu.h"


//contructor
CDockMenu::CDockMenu(irr::IrrlichtDevice *graphics, KeyListener *receiver) : graphics(graphics), receiver(receiver)
{
	t = graphics->getVideoDriver()->getScreenSize();
	window_size = core::rect<s32>(100, 50, t.Width- 100, t.Height-50);

	gui::IGUIFont *micro = graphics->getGUIEnvironment()->getFont("res/font/system.xml");
	graphics->getGUIEnvironment()->getSkin()->setFont(micro);
	control = graphics->getGUIEnvironment()->addGUIElement("control");
	tab_control = graphics->getGUIEnvironment()->addTabControl(window_size,control,true);

	market = tab_control->addTab(L"Market",1);
	repair = tab_control->addTab(L"Drydock",2);
	tavern = tab_control->addTab(L"Tavern",3);
	headquarter = tab_control->addTab(L"Headquarters",4);

	tab_control->setVisible(false);

	setupTabs();
}

CDockMenu::~CDockMenu()
{
	tab_control->remove();
}

void CDockMenu::drop()
{
	delete this;
}

void CDockMenu::setupTabs()
{
	//setup tabs again
	//more really blocky looking code
	//nothing complex here at all, just really long arguments
	graphics->getGUIEnvironment()->addStaticText(L"Station Goods", rect<s32>(20,20,window_size.getWidth()/2,40),false,true,market);
	graphics->getGUIEnvironment()->addStaticText(L"Your Cargo", rect<s32>(window_size.getWidth()/2,20,window_size.getWidth(),40),false,true,market);
	marketTab.buying = graphics->getGUIEnvironment()->addListBox(core::rect<s32>(20,50,window_size.getWidth()/2-20,window_size.getHeight()/2),market);
	marketTab.buy_cost = graphics->getGUIEnvironment()->addStaticText(L"Price:",rect<s32>(20,window_size.getHeight()/2+10, window_size.getWidth()/2-20,window_size.getHeight()/2+30),false,true,market);
	marketTab.buy_description = graphics->getGUIEnvironment()->addStaticText(L"",rect<s32>(20,window_size.getHeight()/2+40,window_size.getWidth()-20,window_size.getHeight()-70),true,true,market);
	marketTab.buy_button = graphics->getGUIEnvironment()->addButton(rect<s32>(120,window_size.getHeight()-60,220,window_size.getHeight()-40),market,0,L"Buy");
	marketTab.selling = graphics->getGUIEnvironment()->addListBox(rect<s32>(window_size.getWidth()/2,50,window_size.getWidth()-20,window_size.getHeight()/2),market);
	marketTab.sell_cost = graphics->getGUIEnvironment()->addStaticText(L"Price:",rect<s32>(window_size.getWidth()/2,window_size.getHeight()/2+10,window_size.getWidth()-20,window_size.getHeight()/2+30),false,true,market);
	marketTab.sell_button = graphics->getGUIEnvironment()->addButton(rect<s32>(window_size.getWidth()-240,window_size.getHeight()-60,window_size.getWidth()-140,window_size.getHeight()-40),market,-1,L"Sell");
	marketTab.player_money = graphics->getGUIEnvironment()->addStaticText(L"Credits:", rect<s32>(200,20,window_size.getWidth(),40),false,true,market);
	marketTab.buy_pressed=false;
	marketTab.sell_pressed=false;


	repairTab.repair_armor = graphics->getGUIEnvironment()->addButton(rect<s32>(window_size.getWidth()-140,window_size.getHeight()-120,window_size.getWidth()-20,window_size.getHeight()-100),repair,-1,L"Repair Armor");
	repairTab.repair_armor_price = graphics->getGUIEnvironment()->addStaticText(L"Cost :", rect<s32>(window_size.getWidth()-140,window_size.getHeight()-100,window_size.getWidth()-20,window_size.getHeight()-80),false,true,repair);
	repairTab.repair_hull = graphics->getGUIEnvironment()->addButton(rect<s32>(window_size.getWidth()-140,window_size.getHeight()-80,window_size.getWidth()-20,window_size.getHeight()-60),repair,-1, L"Repair Hull");
	repairTab.repair_hull_price = graphics->getGUIEnvironment()->addStaticText(L"Cost :", rect<s32>(window_size.getWidth()-140,window_size.getHeight()-60,window_size.getWidth()-20,window_size.getHeight()-40),false,true,repair);
	repairTab.weapon_primary = graphics->getGUIEnvironment()->addComboBox(rect<s32>(10,40,200,60),repair);
	repairTab.primary = graphics->getGUIEnvironment()->addStaticText(L"Primary Weapon Systems", rect<s32>(10,10,300,30),false,true,repair);
	repairTab.primary_description = graphics->getGUIEnvironment()->addStaticText(L"", rect<s32>(320,10,window_size.getWidth()/2,90),false,true,repair);
	repairTab.weapon_secondary = graphics->getGUIEnvironment()->addComboBox(rect<s32>(10,90,200,110),repair);
	repairTab.secondary = graphics->getGUIEnvironment()->addStaticText(L"Secondary Weapon Systems", rect<s32>(10,60,200,80),false,true,repair);
	repairTab.secondary_description = graphics->getGUIEnvironment()->addStaticText(L"", rect<s32>(320,60,window_size.getWidth()/2,140), false,true,repair);
}

void CDockMenu::menuLoop(Player *CPlayer,CShip* docked_station)
{
	
	if(menu_open==true)
	{
		tab_control->setVisible(true);
		runMarket(CPlayer, docked_station);
		runRepair(CPlayer);
		//super important
	}
	else
	{
		tab_control->setVisible(false);
	}
}

void CDockMenu::runMarket(Player *CPlayer, CShip* docked_station)
{
	//update player money
	stringw money= L"Credits:";
	money+=CPlayer->getMoney();
	marketTab.player_money->setText(money.c_str());
	//code to make the list refresh
	//made this way to make as little crashes as possible
	if(marketTab.buying->getItemCount()!=docked_station->getInventory().size())
	{
		marketTab.buying->clear();
		//marketTab.buying->remove();
		//marketTab.buying = graphics->getGUIEnvironment()->addListBox(core::rect<s32>(20,50,window_size.getWidth()/2-20,window_size.getHeight()/2),market);
		for(unsigned int i=0; i<docked_station->getInventory().size();i++)
		{
			marketTab.buying->addItem(docked_station->getInventory()[i]->getName());
		}
	}
	if(marketTab.selling->getItemCount()!=CPlayer->getCargo().size())
	{
		marketTab.selling->clear();
		//marketTab.selling->remove();
		//marketTab.selling = graphics->getGUIEnvironment()->addListBox(rect<s32>(window_size.getWidth()/2,50,window_size.getWidth()-20,window_size.getHeight()/2),market);
		for(unsigned int i=0; i<CPlayer->getCargo().size();i++)
		{
			marketTab.selling->addItem(CPlayer->getCargo()[i]->getName());
		}
	}
	
	//variable to see if player selected something to buy
	int buy = marketTab.buying->getSelected();
	if(buy!=-1)
	{
		marketTab.buy_description->setText(docked_station->getInventory()[buy]->getItemDescription());
		stringw t = L"Price:";

		int temp_cost;
		//price adjustment
		temp_cost=docked_station->getInventory()[buy]->getItemCost();
		temp_cost = temp_cost*docked_station->getModifierBuy();
		t+=temp_cost;

		marketTab.buy_cost->setText(t.c_str());

		//if buy button pressed
		if(marketTab.buy_button->isPressed())
		{

			if(marketTab.buy_pressed!=true)
			{
				//make sure player has enough money
				if(CPlayer->getMoney()>temp_cost)
				{
					//buy item
					//add item to cargo
					CPlayer->setMoney(CPlayer->getMoney()-temp_cost);
					CPlayer->addItemToCargo(docked_station->getInventory()[buy]);
					marketTab.buy_pressed=true;
				}
			}
		}
		else
		{
			marketTab.buy_pressed=false;
		}


	}
	//end buy


	int sell = marketTab.selling->getSelected();
	if(sell!=-1)
	{
		//adjust price if the station does not have the item

		stringw t = L"Price:";
		int cost_temp;
		//price adjustment time
		if(docked_station->itemInInventory(CPlayer->getCargo()[sell])==true)
		{
			cost_temp=CPlayer->getCargo()[sell]->getItemCost();
			cost_temp = cost_temp*docked_station->getModifierSell();
			t+=cost_temp;
		}
		else
		{
			cost_temp = CPlayer->getCargo()[sell]->getItemCost();
			cost_temp = cost_temp*docked_station->getModifierInDemand();
			t+=cost_temp;
			//adjust price
		}
		
		marketTab.sell_cost->setText(t.c_str());

		//if sell button pressed
		if(marketTab.sell_button->isPressed())
		{
			
			if(marketTab.sell_pressed!=true)
			{
				//remove from cargo
				CPlayer->setMoney(CPlayer->getMoney()+cost_temp);
				CPlayer->removeItemFromCargo(CPlayer->getCargo()[sell]);
				marketTab.selling->clear();
				//marketTab.selling->remove();
				//marketTab.selling = graphics->getGUIEnvironment()->addListBox(rect<s32>(window_size.getWidth()/2,50,window_size.getWidth()-20,window_size.getHeight()/2),market);
				marketTab.sell_pressed=true;
				
			}
			else
			{
				marketTab.sell_pressed=false;
			}
		}
	}
}


void CDockMenu::runRepair(Player *CPlayer)
{
	//repair prices
	int hull = CPlayer->getMaxHull() - CPlayer->getHull();
	int armor = CPlayer->getMaxArmor() - CPlayer->getArmor();
	hull=hull/4;
	armor=armor/4;
	stringw hull_str = L"Cost :";
	hull_str+=hull;
	repairTab.repair_hull_price->setText(hull_str.c_str());
	stringw armor_str = L"Cost :";
	armor_str+=armor;
	repairTab.repair_armor_price->setText(armor_str.c_str());

	//repair armor
	if(repairTab.repair_armor->isPressed())
	{
		if(CPlayer->getMoney()>armor)
		{
			CPlayer->repairArmor();
			CPlayer->setMoney(CPlayer->getMoney()-armor);
		}
	}

	if(repairTab.repair_hull->isPressed())
	{
		if(CPlayer->getMoney()>hull)
		{
			CPlayer->repairHull();
			CPlayer->setMoney(CPlayer->getMoney()-hull);
		}
	}

	//update list
	//use a vector to hold the available weapons
	//so that referencing them will be easy
	if(repairTab.weapon_primary->getItemCount()!=CPlayer->getNumPrimaryWeaponsInCargo())
	{
		repairTab.weapon_primary->clear();
		temp_pri.clear();
		for(int i=0; i< CPlayer->getCargo().size(); i++)
		{
			if(CPlayer->getCargo()[i]->getItemType()->type==TYPE_PRIMARY)
			{
				repairTab.weapon_primary->addItem(CPlayer->getCargo()[i]->getName());
				temp_pri.push_back(CPlayer->getCargo()[i]);
			}
		}
	}
	//count
	if(repairTab.weapon_secondary->getItemCount()!=CPlayer->getNumSecondaryWeaponsInCargo())
	{
		repairTab.weapon_secondary->clear();
		temp_sec.clear();
		for(int i=0; i <CPlayer->getCargo().size(); i++)
		{
			if(CPlayer->getCargo()[i]->getItemType()->type == TYPE_SECONDARY)
			{
				repairTab.weapon_secondary->addItem(CPlayer->getCargo()[i]->getName());
				temp_sec.push_back(CPlayer->getCargo()[i]);
			}
		}
	}

	//change player weapons
	int pri = repairTab.weapon_primary->getSelected();
	if(pri!=-1)
	{
		for(int i=0; i<items().turret_list.size(); i++)
		{
			if(items().turret_list[i]->index == temp_pri[pri]->getItemIndex())
			{
				CPlayer->setPrimaryTurret(items().turret_list[i]);
				//settext
				repairTab.primary_description->setText(items().turret_list[i]->description);
			}
		}
	}
	else
	{
		//reset text
		repairTab.primary_description->setText(L"");
	}

	int sec = repairTab.weapon_secondary->getSelected();
	if(sec!=-1)
	{
		for(int i=0; i<items().turret_list.size();i++)
		{
			if(items().turret_list[i]->index == temp_sec[sec]->getItemIndex())
			{
				CPlayer->setSecondaryTurret(items().turret_list[i]);
				repairTab.secondary_description->setText(items().turret_list[i]->description);
			}
		}
	}
}
