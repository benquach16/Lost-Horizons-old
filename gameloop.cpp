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


//gameloop.cpp
//'main' gameloop object
//checks for player input
//creates objects and sets up level
//updates all the other objects in the scene
//and manages the camera control code (for now)

//-----------------------
//developer note:
//refer to main.cpp
//for program hierarchy
//
//THIS IS HOW THE PLANE INGAME WORKS, MODIFY ROTATION AND POSITION ACCOORDING TO THIS!!!
//CARTESIAN PLANE
//POSITION
//X = FRONT AND BACK
//Y = UP AND DOWN
//Z = LEFT AND RIGHT
//				|
//				|
//				|Y
//				/\
//			   /  \
//			 X/    \Z
//
//ROTATION
//X = UP AND DOWN
//Y = LEFT AND RIGHT
//Z = TILT, NOT USED
//

//SECOND IMPORTANT NOTE :
//CAMERA FOV IS IN RADIANS!!!!!!

//global variable for screen size
//should not change without a game restart

#include "stdafx.h"
#include "gameloop.h"
#include "GFramework.h"




//useful variables
bool menuop;
bool menu_open;
bool target_menu_open;
bool dock_open;

float angle;
float angle2;
float distance;
float nav_distance;

int tMouseX;
int tMouseY;
float tMouseW;
float tMouseW_old;


//TODO
//BRING SHADERS INTO MAIN LOOP SO THAT THEY CAN BE MANAGED GLOBALLY
//INSTEAD OF CREATING THEM INSIDE EACH OF THE OBJECTS CLASS



//for easier saving and loading
//to be used later
enum SYSTEMS_GAME
{
	SYSTEM_TAU_CETI = 0,
	SYSTEM_DX_CANCRI = 1,
	SYSTEM_EPSILION_INDI=2,
};



core::dimension2d<int> screen_size;


//constructor to inherit objects from init class
GameLoop::GameLoop(irr::IrrlichtDevice *graphics, KeyListener *receiver, irrklang::ISoundEngine *sound, bool loadgame, bool quality):
graphics(graphics),sound(sound), receiver(receiver), GamePaused(false), pCam(0),then(0),velocity(0)
{
	//set up game
	//establish game object variables initialized in the init.cpp file

	//initialize essential stuff first
	//setup variables
	player_target	=	0;		//very important to initialize it to 0
	jump_target		=	0;		//ditto
	velocity		=	0;
    angle			=	0;
    angle2			=	0;
	camera_sway		=	0;

	//for camera
    distance		=	1000.0;
	nav_distance	=	400000.0;

	fired			=	false;
	GameExit		=	false;
	menu_open		=	false;
	dock_open		=	false;

	NavView			=	false;
	CommandView		=	false;
	Warp			=	false;
	LaunchFighters	=	false;

	display_tut		=	false;

	//replace this with non shitty function
	//irrlichts timer is so fucking broken
	//need PRECISE timer
	u32 then		=	graphics->getTimer()->getTime();

	//set the global variable
	screen_size		=	graphics->getVideoDriver()->getScreenSize();

	//add spacebox, skybox, whatever the hell you call it
	//TODO: adjust this to the system scene file
	//TODO: create system scene manager
	scene::ISceneNode *skybox = graphics->getSceneManager()->addSkyBoxSceneNode(
		graphics->getVideoDriver()->getTexture("res/textures/skyboxes/1/space_top3.jpg"),
		graphics->getVideoDriver()->getTexture("res/textures/skyboxes/1/space_bottom4.jpg"),
		graphics->getVideoDriver()->getTexture("res/textures/skyboxes/1/space_left2.jpg"),
		graphics->getVideoDriver()->getTexture("res/textures/skyboxes/1/space_right1.jpg"),
		graphics->getVideoDriver()->getTexture("res/textures/skyboxes/1/space_front5.jpg"),
		graphics->getVideoDriver()->getTexture("res/textures/skyboxes/1/space_back6.jpg"));


	//some flat lighting
	graphics->getSceneManager()->setAmbientLight(video::SColor(155,32,32,32));
	//star class comes with lighting
	tau_ceti_star = new sun(graphics,core::vector3df(-60000,0,-100000));


	//create shader and assign lighting
	//chcek graphics quality
	callback = new ShaderCallBack;
	if(quality==true)
	{
		callback->shader_enabled = true;
		callback->fBumpStrength=4;
		callback->fSpecularStrength=1;
		callback->fSpecularPower=20;

		callback->fvAmbient=SColorf(0.5,0.5,0.5);
	
		callback->fLightStrength[0]=1;
		callback->fvLightColor[0]=SColorf(0.6,0.6,0.6);
		callback->fvLightPosition[0]=graphics->getSceneManager()->getActiveCamera()->getAbsolutePosition();

		//sun
		callback->fLightStrength[1] = 5000000;
		callback->fvLightColor[1] = SColorf(0.7,0.7,0.9);
		callback->fvLightPosition[1] = vector3df(-60000,0,-100000);
	}
	else
	{
		callback->shader_enabled = false;
	}

	//create essential objects
	alertBox	=	new CAlertBox(graphics);
	Manager		=	new gameManager(graphics,receiver,sound);
	CPlayer		=	new Player(graphics,sound,alertBox,receiver,core::vector3df(0,0,-3000),ships().TERR_PRAETORIAN_CRUISER, callback);
	CHud		=	new Hud(graphics,CPlayer);
	gameMenu	=	new CMenu(graphics,sound,receiver, CPlayer);
	escapeMenu	=	new CEscapeMenu(graphics);
	missionM	=	new missionManager(graphics);
	dialogueM	=	new dialogueManager();
	targetMenu	=	new CTargetMenu(graphics);
	dockMenu	=	new CDockMenu(graphics,receiver);

	//player camera create
	pCam = graphics->getSceneManager()->addCameraSceneNode();
	if(pCam)
	{

		pCam->setFarValue(CAMERA_VIEW_DISTANCE);
		pCam->setFOV(CAMERA_FOV);

	}

	//important GUI stuff
	//shows what direction the ship wants to face
	XYcircle = graphics->getSceneManager()->addCubeSceneNode();
	if(XYcircle)
	{
		XYcircle->setPosition(CPlayer->getPos());
		XYcircle->setScale(core::vector3df(60,0,60));
		XYcircle->setMaterialTexture(0,graphics->getVideoDriver()->getTexture("res/textures/circle.png"));
		XYcircle->setMaterialFlag(video::EMF_LIGHTING,false);
		XYcircle->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL);
		XYcircle->setMaterialFlag(video::EMF_ZBUFFER,true);
		XYcircle->setVisible(false);
	}
	ZYcircle = graphics->getSceneManager()->addCubeSceneNode();
	if(ZYcircle)
	{
		ZYcircle->setPosition(CPlayer->getPos());
		ZYcircle->setScale(core::vector3df(0,60,60));
		ZYcircle->setMaterialTexture(0,graphics->getVideoDriver()->getTexture("res/textures/circle.png"));
		ZYcircle->setMaterialFlag(video::EMF_LIGHTING,false);
		ZYcircle->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL);
		ZYcircle->setMaterialFlag(video::EMF_ZBUFFER,true);
		ZYcircle->setVisible(false);
	}
	TurningArrow=graphics->getSceneManager()->addCubeSceneNode();
	if(TurningArrow)
	{
		TurningArrow->setScale(core::vector3df(60,0,60));
		TurningArrow->setMaterialTexture(0,graphics->getVideoDriver()->getTexture("res/textures/arrow.png"));
		TurningArrow->setMaterialFlag(video::EMF_LIGHTING, false);

		TurningArrow->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL);
		TurningArrow->setVisible(false);
	}
	core::dimension2d<s32> p = graphics->getSceneManager()->getSceneCollisionManager()->getScreenCoordinatesFrom3DPosition(CPlayer->getPos(),pCam);
	player_nav = graphics->getGUIEnvironment()->addImage(graphics->getVideoDriver()->getTexture("res/menu/buoy.png"),p);
	player_nav->setVisible(false);

	//GLOBAL EFFECTS
	//FOR STUFF LIKE SPASCE FOG
	//AND STARDUST
	//CAUSE THAT NEEDS TO FOLLOW THE CAMERA NOT THE PLAYER OR ANYTHING ELSE!!!!



	//TEMPORARY
	//SETUP SYSTEM
	terran = new planet(graphics,planets().HABITABLE,core::vector3df(25000,-1000,14000),FACTION_TERRAN_FEDERATION, L"Blacksun Delta","res/textures/planets/habitable_1.jpg");
	Manager->addPlanet(terran);

	planet *terran2 = new planet(graphics,planets().BARREN,core::vector3df(-30000,00,300000),FACTION_TERRAN_FEDERATION, L"Argrea","res/textures/planets/barren_1.jpg");
	Manager->addPlanet(terran2);


	planet *prov = new planet(graphics,planets().HABITABLE,core::vector3df(-300000,-5000,20000),FACTION_PROVIAN_CONSORTIUM, L"New Imperial","res/textures/planets/earth.jpg");
	Manager->addPlanet(prov);


	planet *roid = new planet(graphics,planets().ASTEROIDS,core::vector3df(-50000,0,-10000),FACTION_PROVIAN_CONSORTIUM, L"Asteroid Belt","res/roid.jpg");
	Manager->addPlanet(roid);
	//create nebula with particle system

	planet *moon = new planet(graphics,planets().MOON,core::vector3df(-5000,-1000,-25000),FACTION_TERRAN_FEDERATION, L"Drall", "res/textures/planets/moon.jpg");
	Manager->addPlanet(moon);

	planet* gasgiant = new planet(graphics,planets().GAS,core::vector3df(300000,-3000,-300000),FACTION_NEUTRAL, L"Farstar", "res/textures/planets/gas.jpg");
	Manager->addPlanet(gasgiant);

	//stardust effect
	for(int i = 0;i<50;i++)
	{
		scene::IBillboardSceneNode *s = graphics->getSceneManager()->addBillboardSceneNode(0,dimension2df(500,500),vector3df(0,0,0));
		s->setMaterialTexture(0,graphics->getVideoDriver()->getTexture("res/textures/dust.png"));
		s->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);
		s->setMaterialFlag(video::EMF_LIGHTING,false);
		dust_manager.push_back(s);
	}

	//load game or create new scene
	if(loadgame==true)
	{
		//newGame();
		loadGame();
	}
	else
	{
		newGame();
	}

	

}

GameLoop::~GameLoop()
{
	//destructor
	//nuke everything
	//also ew
	CHud->drop();
	escapeMenu->drop();
	Manager->drop();

	//gameMenu->drop();
	//dockMenu->drop();
	missionM->drop();
	dialogueM->drop();
	//targetMenu->drop();
	//be careful about removing scenenodes
	//often it is much easier tojust clear the scene
	//XYcircle->remove();
	//ZYcircle->remove();
	//TurningArrow->remove();
	//pCam->remove();
	//tau_ceti_star->drop();
	CPlayer->drop();

	//did i miss somethin?
	graphics->getSceneManager()->clear();
	graphics->getGUIEnvironment()->clear();

}

void GameLoop::drop()
{
	delete this;
}

void GameLoop::newGame()
{
	//move to xml file
	gui::IGUIWindow *tutorial = graphics->getGUIEnvironment()->addWindow(rect<s32>(0,0,350,420),false,L"Controls");
	graphics->getGUIEnvironment()->addImage(graphics->getVideoDriver()->getTexture("res/menu/tutorial/controls.png"),core::position2d<s32>(0,20),-1,tutorial);
	CDialogueTree *test = new CDialogueTree(graphics,L"Admiral Brennan",L"Hello Captain. I know that you've been in stasis for a long time now, but the Terran Navy is extremely undermanned, especially with the disappearance of the Earth jumpgate all those years ago.");
	test->addText(L"On your left is a short log detailing how to use your ship functions. Go over it carefully. Try using the X and Z keys in order to adjust your cruiser's speed, and the WASD keys to tell your helmsman to adjust your ship orientation. Once you're done, press OK");
	test->addText(L"Right now, we've got enemy cruisers in orbit around this planet. We're going to need your help in eleminating these ships. I'll try to update you on combating enemy warships as quickly as I can.");
	test->addText(L"I've given you a mission to destroy these enemy ships. In order to check your list of missions, open up your ship's command console and check your missions log. You can then check the missions objectives. Now, in order to attack enemy capital ships, you must target them by clicking on them, then you can them open fire with your primary turrets by holding down Spacebar");
	test->addText(L"Remember, you're in charge of a warship with a crew compliment of 1200 men, not a fighter. Don't be a hero, you've got over a thousand lives on your ship. Pick your fights intelligently and tactically. Winning fights helps level up your crew and officers, which helps your ship run more efficiently");
	test->addText(L"Once you've finished destroying these enemy ships, I'll contact you again");
	dialogueM->addTree(test);
	//PLACEHOLDER
	//TODO: ADD INITIAL SHIPS TO SYSTEM SCENE FILE

	
	const wchar_t *pname = ships().provian_ship_name[rand()%ships().provian_ship_name.size()];
	CShip *prov_cruiser1 = new CShip(graphics,sound, core::vector3df(-5000,50,5000),core::vector3df(0,0,0),ships().PROV_ARES_DESTROYER,FACTION_PROVIAN_CONSORTIUM,pname, callback);
	Manager->addShip(prov_cruiser1);

	const wchar_t *tname = ships().terran_ship_name[rand()%ships().terran_ship_name.size()];
	CShip *terr_cruiser1 = new CShip(graphics,sound, core::vector3df(-500,0,0),core::vector3df(0,0,0),ships().TERR_LEGION_DESTROYER,FACTION_TERRAN_FEDERATION, tname, callback);
	Manager->addShip(terr_cruiser1);

	const wchar_t *tname2 = ships().terran_ship_name[rand()%ships().terran_ship_name.size()];
	//CShip *terr_cruiser2 = new CShip(graphics,sound, core::vector3df(500,500,-2000),core::vector3df(0,0,0),ships().TERR_LEGION_DESTROYER,FACTION_TERRAN_FEDERATION, tname2);
	//Manager->addShip(terr_cruiser2);

	const wchar_t *tname3 = ships().terran_ship_name[rand()%ships().terran_ship_name.size()];
	//CShip *terr_cruiser3 = new CShip(graphics,sound, core::vector3df(1500,500,-1000),core::vector3df(0,0,0),ships().TERR_PRAETORIAN_CRUISER,FACTION_TERRAN_FEDERATION, tname3);
	//Manager->addShip(terr_cruiser3);
	const wchar_t *tname4 = ships().terran_ship_name[rand()%ships().terran_ship_name.size()];
	//CShip *terr_cruiser4 = new CShip(graphics,sound, core::vector3df(1500,500,-1000),core::vector3df(0,0,0),ships().TERR_PRAETORIAN_CRUISER,FACTION_TERRAN_FEDERATION, tname4);
	//Manager->addShip(terr_cruiser4);
	const wchar_t *tname5 = ships().terran_ship_name[rand()%ships().terran_ship_name.size()];
	//CShip *terr_cruiser5 = new CShip(graphics,sound, core::vector3df(0,-500,0),core::vector3df(0,0,0),ships().TERR_PRAETORIAN_CRUISER,FACTION_TERRAN_FEDERATION, tname5);
	//Manager->addShip(terr_cruiser5);
	
	CShip *s = new CShip(graphics,sound,core::vector3df(500,-1000,3000),core::vector3df(0,0,0),ships().HQ,FACTION_TERRAN_FEDERATION,L"Terran System Headquarters", callback);
	if(s)
	{

		//give station stuff to sell
		item* in = new item(items().NANOALLOY);
		s->addItemToInventory(in);
		item* in2 = new item(items().WATER);
		s->addItemToInventory(in2);
		item* in3 = new item(items().LIGHT_GATLING);
		s->addItemToInventory(in3);
		item* in4 = new item(items().PRI_PHOTON);
		s->addItemToInventory(in4);
		item* in5 = new item(items().SEC_PLASMA);
		s->addItemToInventory(in5);
		Manager->addStation(s);

		
		//CPlayer->setPos(vector3df(-5000,0,2000));
		//CPlayer->setMoney(50000);
	}
	CShip *trade = new CShip(graphics,sound,vector3df(-3000,0,-13000),vector3df(0,0,0),ships().TRADING_STATION,FACTION_TERRAN_FEDERATION,L"Drall Trading Station", callback);
	if(trade)
	{
		item *i = new item(items().HEAVY_METALS);
		trade->addItemToInventory(i);
		item *i2 = new item(items().RATIONS);
		trade->addItemToInventory(i2);
		Manager->addStation(trade);
	}
	
	CShip *argrea_trade = new CShip(graphics,sound,vector3df(-30000,0,275000),vector3df(0,0,0),ships().TRADING_STATION, FACTION_TERRAN_FEDERATION, L"Argrea Trading Station", callback);
	Manager->addStation(argrea_trade);
	
	CShip *shipyard = new CShip(graphics,sound,vector3df(5000,-20,-3000), vector3df(0,0,0), ships().SHIPYARD, FACTION_TERRAN_FEDERATION, L"Blacksun Delta Shipyard", callback);
	if(shipyard)
	{
		item *rail = new item(items().PRI_RAIL);
		shipyard->addItemToInventory(rail);
	}
	Manager->addStation(shipyard);
	
	
	CShip *s2 = new CShip(graphics,sound,core::vector3df(-160000,-4000,3000),core::vector3df(0,0,0),ships().HQ,FACTION_PROVIAN_CONSORTIUM,L"Provian Headquarters", callback);
	Manager->addStation(s2);

	CShip *mine = new CShip(graphics,sound,core::vector3df(-40000,0,0),core::vector3df(0,0,0),ships().MINING_STATION,FACTION_PROVIAN_CONSORTIUM,L"Provian Mining Base", callback);
	Manager->addStation(mine);


	const wchar_t *pname2 = ships().provian_ship_name[rand()%ships().provian_ship_name.size()];
	CShip *prov_cruiser2 = new CShip(graphics,sound, core::vector3df(-40000,0,-2000),core::vector3df(0,0,0),ships().PROV_ISHTAR_CRUISER,FACTION_PROVIAN_CONSORTIUM,pname2, callback);
	Manager->addShip(prov_cruiser2);

	const wchar_t *pname3 = ships().provian_ship_name[rand()%ships().provian_ship_name.size()];
	CShip *prov_cruiser3 = new CShip(graphics,sound, core::vector3df(-7000,0,8000),core::vector3df(0,0,0),ships().PROV_ISHTAR_CRUISER,FACTION_PROVIAN_CONSORTIUM,pname3 ,callback);
	Manager->addShip(prov_cruiser3);
	const wchar_t *pname4 = ships().provian_ship_name[rand()%ships().provian_ship_name.size()];
	CShip *prov_cruiser4 = new CShip(graphics,sound, core::vector3df(-8000,1000,7000),core::vector3df(0,0,0),ships().PROV_ISHTAR_CRUISER,FACTION_PROVIAN_CONSORTIUM,pname4, callback);
	Manager->addShip(prov_cruiser4);
	const wchar_t *pname5 = ships().provian_ship_name[rand()%ships().provian_ship_name.size()];
	CShip *prov_cruiser5 = new CShip(graphics,sound, core::vector3df(-7000,0,6000),core::vector3df(0,0,0),ships().PROV_ARES_DESTROYER,FACTION_PROVIAN_CONSORTIUM,pname5, callback);
	Manager->addShip(prov_cruiser5);


	const wchar_t *pname6 = ships().provian_ship_name[rand()%ships().provian_ship_name.size()];
	//CShip *prov_cruiser6 = new CShip(graphics,sound, core::vector3df(-7000,-1000,4000),core::vector3df(0,0,0),ships().PROV_ISHTAR_CRUISER,FACTION_PROVIAN_CONSORTIUM,pname6);
	//Manager->addShip(prov_cruiser6);
	const wchar_t *pname7 = ships().provian_ship_name[rand()%ships().provian_ship_name.size()];
	//CShip *prov_cruiser7 = new CShip(graphics,sound, core::vector3df(-6000,500,6000),core::vector3df(0,0,0),ships().PROV_ISHTAR_CRUISER,FACTION_PROVIAN_CONSORTIUM,pname7);
	//Manager->addShip(prov_cruiser7);
	

	//create first objective
	//TODO: SAVE MISSIONS
	//TODO: READ MISSION TEXT FROM XML
	//USING STRINGS IN CODE IS NO BUENO
	missionM->addMission(missionCreate(graphics,1).tut);
}

void GameLoop::saveGame()
{

	io::IXMLWriter *writer;
	writer = graphics->getFileSystem()->createXMLWriter("saves/save.lsav");
	writer->writeXMLHeader();
	writer->writeComment(L"edit this if you're lame or you're me");

	//save ingame timer
	core::array<stringw> value;
	value.push_back(L"time");
	core::array<stringw> num;
	//save number of missions
	stringw t(L"");
	t+=graphics->getTimer()->getTime();
	num.push_back(t);
	writer->writeElement(L"gameInfo",true,value,num);
	//save all scene objects into xml file
	//donnt save hud and crap cause that changes dynamically

	writer->writeLineBreak();
	CPlayer->saveObject(writer);
	writer->writeLineBreak();
	CPlayer->saveCargo(writer);
	writer->writeLineBreak();
	Manager->saveObjects(writer);
	writer->writeLineBreak();
	missionM->saveMissions(writer);
	writer->drop();
}

//Load game from xml saved file
//TODO: LOAD MISSIONS
void GameLoop::loadGame()
{

	//xml stored save file
	//read data
	io::IXMLReader *reader = graphics->getFileSystem()->createXMLReader("saves/save.lsav");
	if(!reader)
	{
		//couldnt read the save file for some reason
	}
	stringw currentSection;
	int num_ships;
	int num_cargo;
	int num_missions;
	while(reader->read())
	{
		switch(reader->getNodeType())
		{
		case io::EXN_ELEMENT:
			{
				//load time info
				if(core::stringw(L"gameInfo").equals_ignore_case(reader->getNodeName()))
				{
					graphics->getTimer()->setTime(reader->getAttributeValueAsInt(0));
				}
				//read from playerstats
				if(core::stringw(L"playerStats").equals_ignore_case(reader->getNodeName()))
				{
					CPlayer->loadObject(reader);
				}
				//load from cargo elemeent
				//the extra code is because there are nested elements
				if(currentSection==L"" && core::stringw(L"playerCargo").equals_ignore_case(reader->getNodeName()))
				{
					currentSection=L"playerCargo";
					num_cargo = reader->getAttributeValueAsInt(0);
				}
				else if (currentSection.equals_ignore_case(L"playerCargo"))
				{
					CPlayer->loadCargo(reader,num_cargo);
				}
				//read from shipstats element
				if(currentSection==L"" && core::stringw(L"shipStats").equals_ignore_case(reader->getNodeName()))
				{
					currentSection=L"shipStats";
					num_ships = reader->getAttributeValueAsInt(0);
				}
				else if (currentSection.equals_ignore_case(L"shipStats"))
				{
					Manager->loadShips(reader,num_ships,callback);
				}
				
				//read from missions element
				if(currentSection==L"" && core::stringw(L"missions").equals_ignore_case(reader->getNodeName()))
				{
					currentSection=L"missions";
					num_missions = reader->getAttributeValueAsInt(0);
				}
				else if(currentSection.equals_ignore_case(L"missions"))
				{
					missionM->loadMissions(reader,num_missions);
				}
			}
			break;
		case io::EXN_ELEMENT_END:
			{
				currentSection=L"";
			}
			break;
		}
	}
	
	printf("End of Loading Process");
	reader->drop();
}

bool GameLoop::exitGame()
{
	//returns true if game wants to exit
	return GameExit;
}

void GameLoop::Run()
{
	//main game loop

	//determine fps difference for frame independent movement
	
	u32 now = graphics->getTimer()->getTime();
	frameDeltaTime = (f32)(now - then) *0.001f; // Time in seconds
	then = now;


	//escape menu code
	if(receiver->IsKeyDown(irr::KEY_ESCAPE))
	{
		if(menu_open==false)
		{
			if(GamePaused==false)
			{
				menu_open = true;
				GamePaused=true;
			}
			else
			{
				menu_open = true;
				GamePaused=false;
			}
		}
	}
	else
	{
		menu_open = false;
	}

	

	//IF GAME RUNNING
	if(GamePaused==false)
	{
		escapeMenu->setVisible(false);
		//set to camera pos
		sound->setListenerPosition(pCam->getPosition(),pCam->getRotation());
		//
		//CAMPAIGN STUFF
		//PLEASE PUT THIS
		//INSIDE SPECIFIC CAMPAIGN MANAGER CODEs
		if(missionM->getMissionList().size()>0)
		{
		if(missionM->getMissionList()[0]->getMissionComplete()==true)
		{
			if(display_tut==false)
			{
			CDialogueTree *complete = new CDialogueTree(graphics, L"Admiral Brennan", L"Excellent work! Destroying enemy ships is vital our operations in this system. If you haven't already noticed, destroyed enemy ships often leave behind cargo containers, which you can pick up for loot and then sell at the nearest station.");
			complete->addText(L"Now, things here have changed significantly since the time you left Earth. The Free-Traders Union is the most powerful group in this part of the galaxy, and we're in desperate need of their support");
			complete->addText(L"We don't have the resources here to provide you with all supplies and weaponry that you'll need. You'll have to work as a semi-freelancer in order to get the money to equip your ship. You can make credits here by trading with different space stations, and doing jobs for the different corporations around this sector");
			complete->addText(L"You'll still be needed for the war effort though. Terran Cruisers, such as your own, are in high demand around here. Our objectives in this system is to push the aliens back by capturing their planets. You can get to different planets by targetting them in the Navigations Console and then Warping towards that target.");
			complete->addText(L"That's not all though. There are some mysterious things occuring in this sector, and we might need you to check out some events. Stay safe, Captain");
			dialogueM->addTree(complete);
			display_tut=true;
			}
		}
		}
		//
		 
		//constantly update position of turning circle GUI
		XYcircle->setPosition(CPlayer->getPos());
		XYcircle->setRotation(core::vector3df(0,CPlayer->turn.Y,0));
		ZYcircle->setPosition(CPlayer->getPos());
		ZYcircle->setRotation(core::vector3df(CPlayer->getRot().X,CPlayer->turn.Y,0));

		TurningArrow->setRotation(core::vector3df(CPlayer->turn.X,CPlayer->turn.Y,0));
		TurningArrow->setPosition(CPlayer->getPos());
	

		//run functions
		alertBox->run();																	//show alert texts
		Manager->gameManagerLoop(frameDeltaTime,sound, CPlayer,alertBox,callback);			//update nodes and game scene ai
		CPlayer->manageTurrets(player_target,frameDeltaTime);								//gives player target to turrets
		cameraControl(receiver,frameDeltaTime);												//camera movement input
		dockMenu->menuLoop(CPlayer,docked_station);											//dockmenu loop
		dialogueM->loop();																	//communications
		missionM->loop(CPlayer,current_mission, Manager->ship_manager,alertBox);			//loop through missions
		gameMenu->menuLoop(CPlayer,missionM);												//menu loop
		CPlayer->playerRun(frameDeltaTime);													//run through player funcs
		playerControl(receiver,frameDeltaTime,sound);										//player key input
		CHud->updateHud(player_target,jump_target);											//continually update hud
		targetMenu->loop(player_target);													//update target info

		//dust is very very simple
		//simple moves around the dust cloud so the camera is always immersed in it
		for(int i=0; i<dust_manager.size();i++)
		{
			if(dust_manager[i]->getPosition().getDistanceFrom(pCam->getPosition())>1000)
			{
				vector3df pos(pCam->getPosition().X+rand()%2000-1000,pCam->getPosition().Y+rand()%2000-1000,pCam->getPosition().Z+rand()%2000-1000);
				dust_manager[i]->setPosition(pos);
			}
		}

	}
	else
	{
		//IF GAME PAUSED

		//escape menu
		escapeMenu->setVisible(true);
		
		//save game
		if(escapeMenu->getSaveButtonPressed())
		{
			saveGame();
		}

		if(escapeMenu->getQuitButtonPressed())
		{
			GameExit=true;
		}
		
	}
}



//turns the turning circle on or off
void GameLoop::setTurnCircleGUI(bool is_visible)
{
	TurningArrow->setVisible(is_visible);
	XYcircle->setVisible(is_visible);
	ZYcircle->setVisible(is_visible);
}

//big func to move player and stuff
//going to comment it more
void GameLoop::playerControl(KeyListener *receiver, f32 frameDeltaTime,irrklang::ISoundEngine *sound)
{
	//Used to constantly check whether the player is shooting or not
	//And pass on the target the player is targetting in order to shoot it
	Manager->playerShoot(graphics,CPlayer,sound,player_target,frameDeltaTime, targetMenu->getSelected());
	//If the player clicks with the left mouse button
	//Check the gameManger object if there is any ship that the mouse is near
	//if there is a ship, return the ship object and set player_target to that ship
	//if there is no ship, return 0
	if(CommandView!=true)
	{

		//player can only target other ships in normal view
		//player can only target planets in nav view
		if(NavView!=true)
		{
			//target selection only for non navigation view
			if(receiver->mouseLButtonDown())
			{
				//this is so the player can select subsystems n stuff
				if(targetMenu->getVisible()!=true)
					player_target = Manager->getTarget();
			}
		}
		else
		{
			//only be able to target planets in nav mode
			if(receiver->mouseLButtonDown())
			{
				jump_target = Manager->getTargetPlanet();
			}
			
		}
	}
	//a little bit of safety against crashes
	if(player_target!=0)
	{
		//if the player target is destroyed, set player target to zero so there is no crash
		if(player_target->getHullPoints()<1)
		{
			player_target=0;
		}
		//able to display the target menu
		if(receiver->IsKeyDown(irr::KEY_KEY_U))
		{
			targetMenu->setVisible(true);
		}
		else
			targetMenu->setVisible(false);
	}
	else
	{
		targetMenu->setVisible(false);
	}

	//give the player a current mission
	if(gameMenu->getSelectedMission()!=0)
	{
		current_mission = gameMenu->getSelectedMission();
	}
	else
	{

		current_mission = 0;
	}


	//if the player is alive
	if(CPlayer->getHull()>0)
	{
		//F1 Normal View
		//Shows ship targets but not planet targets
		if(NavView==false)
		{
			Manager->showPlanetTargets(false);
			Manager->showShipTargets(true);
		}
		else
		{
			Manager->showPlanetTargets(true);
			Manager->showShipTargets(false);
		}
		if(receiver->IsKeyDown(irr::KEY_F1))
		{
			CommandView=false;
			NavView=false;

		}
		//F2 Command View
		//Used to order ships around
		if(receiver->IsKeyDown(irr::KEY_F2))
		{
			CommandView=true;
			NavView=false;
		}
		//F3 Navigational View
		//Shows planet targets but not ship targets
		if(receiver->IsKeyDown(irr::KEY_F3))
		{
			CommandView = false;
			NavView = true;
		}

		//if docked open dockmenu
		if(CPlayer->getDocked()==true)
		{
			dockMenu->setMenuOpen(true);
		}
		else
		{
			dockMenu->setMenuOpen(false);
		}

		//show nav lines
		if(NavView==true)
		{
			core::dimension2d<s32> p = graphics->getSceneManager()->getSceneCollisionManager()->getScreenCoordinatesFrom3DPosition(CPlayer->getPos(),pCam);
			player_nav->setVisible(true);
			player_nav->setRelativePosition(vector2d<s32>(p.Width-32,p.Height-32));
			if(jump_target!=0)
			{
				
				core::dimension2d<s32> t = graphics->getSceneManager()->getSceneCollisionManager()->getScreenCoordinatesFrom3DPosition(jump_target->getPos(),pCam);
				graphics->getVideoDriver()->draw2DLine(p,t,SColor(255,255,255,0));

			}
		}
		else
			player_nav->setVisible(false);

		//If the player presses V (to dock) dock player at station
		//station has to be targetted
		if(receiver->IsKeyDown(irr::KEY_KEY_V))
		{
			//dock_open === key release
			//temporary
			if(dock_open==false)
			{
				dock_open=true;
				if(CPlayer->getDocked()==false)
				{
					if(player_target!=0)
					{
						//Ensure that the target is within a certain distance in order to dock
						if(player_target->getPos().getDistanceFrom(CPlayer->getPos())<SHIP_DOCK_DISTANCE)
						{
							if(player_target->getIsStation())
							{
								docked_station = player_target;
								velocity=0;
								CPlayer->setVelocity(velocity);
								CPlayer->setDocked(true);
								CPlayer->setPos(player_target->getPos());
								CPlayer->setRot(vector3df(0,0,0));
							}
						}
					}
				}
				else
				{
					CPlayer->setDocked(false);
				}
			}
			
		}
		else
		{
			dock_open=false;
		}

		//warp code
		//If the Player presses J (to warp)
		if(receiver->IsKeyDown(irr::KEY_KEY_J))
		{
			if(jump_target!=0)
			{
				float x(jump_target->getPos().X - CPlayer->getPos().X);
				float y(jump_target->getPos().Y - CPlayer->getPos().Y);
				float z(jump_target->getPos().Z - CPlayer->getPos().Z);

				float angleY = std::atan2(x,z)*180/3.14159265;
				float tmp = sqrt(x*x+ z*z);
				float angleX = std::atan2(tmp,y)*180/3.14159265;
				angleX-=90;

				CPlayer->turn.X = angleX;
				CPlayer->turn.Y = angleY;


				Warp=true;

			}
		}

		//if warp button pressed
		if(Warp == true)
		{
			//ensure that player hasn't deselected the target, or else game will crash
			if(jump_target!=0)
			{

				if(CPlayer->getRot().Y+3>CPlayer->turn.Y && CPlayer->getRot().Y-3<CPlayer->turn.Y &&CPlayer->getRot().X+3>CPlayer->turn.X && CPlayer->getRot().X-3<CPlayer->turn.X)
				{
					if(CPlayer->getPos().getDistanceFrom(jump_target->getPos())>SHIP_WARP_DISTANCE)
					{
						velocity = SHIP_WARP_SPEED;
						CPlayer->setVelocity(velocity);
						if(distance<CAMERA_DISTANCE_MAX)
							distance+=50;

					}
					else
					{
						Warp = false;
						velocity = 0;
						CPlayer->setVelocity(velocity);
						if(distance>CAMERA_DISTANCE_MAX/2)
						{
							distance-=50;
						}
					}
				}
			}
			else
			{

				//if the player deselects planet, cancel warp
				Warp = false;
			}
		}
		//end warp code

		//fighter code
		if(gameMenu->getSendFighters() || receiver->IsKeyDown(irr::KEY_KEY_N))
		{
			LaunchFighters=true;
		}
		if(LaunchFighters==true)
		{
			//deploy fighters
			if(CPlayer->getNumFighters()>0)
			{
				if(CPlayer->getFighterCreationTime() < graphics->getTimer()->getTime())
				{
					vector3df pos = CPlayer->getPos();
					pos+=vector3df(rand()%100-50,rand()%100-50,rand()%100-50);
					fighter *f = new fighter(graphics,sound,CPlayer->getPos(),CPlayer->getRot(),CPlayer->getFighterType(),FACTION_TERRAN_FEDERATION,CPlayer->ship);
					f->setTarget(player_target);
					if(player_target!=0)
						f->moveToPoint(player_target->getPos());
					Manager->addFighter(f);
					CPlayer->addFighterCount(-1);
					CPlayer->setFighterCreationTime(graphics->getTimer()->getTime()+CPlayer->getFighterLaunchTime());
					CPlayer->fighter_manager.push_back(f);
				}
			}
			else
				LaunchFighters=false;
		}

		if(gameMenu->getReturnFighters())
		{
			/*
			for(int i=0; i<Manager->fighter_manager.size();i++)
			{
				//find player fighters
				if(Manager->fighter_manager[i]->getHomeBase()==CPlayer->ship)
				{
					Manager->fighter_manager[i]->setReturnHome(true);
				}
			}
			*/
			CPlayer->setFighterCount(10);

		}
		

		//movement code
		//ensure movement code is done only when not docked
		//and not during warps
		if(CPlayer->getDocked()!=true)
		{
			if(Warp!=true)
			{
				//get input and do stuff with it
				if(receiver->IsKeyDown(irr::KEY_KEY_A))
				{
					CPlayer->turn.Y-=35*frameDeltaTime;
				}
				if(receiver->IsKeyDown(irr::KEY_KEY_D))
				{
					CPlayer->turn.Y += 35*frameDeltaTime;
				}
				if(receiver->IsKeyDown(irr::KEY_KEY_W))
				{
					if(CPlayer->turn.X<70)
						CPlayer->turn.X+=35*frameDeltaTime;
				}
				if(receiver->IsKeyDown(irr::KEY_KEY_S))
				{
					if(CPlayer->turn.X>-70)
						CPlayer->turn.X-=35*frameDeltaTime;
				}

				if(receiver->IsKeyDown(irr::KEY_KEY_Q))
				{
					CPlayer->turn.Y=CPlayer->getRot().Y;
					CPlayer->turn.X=CPlayer->getRot().X;
					CPlayer->turn.Z=CPlayer->getRot().Z;
				}
	
				//get input to add velocity to player
				if(receiver->IsKeyDown(irr::KEY_KEY_X))
				{
					if(CPlayer->getVelocity() < CPlayer->getMaxSpeed())	//dont let velocity get above maxspeed
					{
						velocity+=(5+abs(velocity)/2)*frameDeltaTime;
						CPlayer->setVelocity(velocity);
					}
				}
				if(receiver->IsKeyDown(irr::KEY_KEY_Z))
				{
					if(CPlayer->getVelocity() > -10)
					{
						velocity-=5*frameDeltaTime;
						CPlayer->setVelocity(velocity);
					}

				}


				//self explainatory
				if(receiver->IsKeyDown(irr::KEY_SPACE))
				{
					if(player_target!=0)
						CPlayer->shoot();
				}
				else
					CPlayer->resetCannons();
			}
		}


		if(CPlayer->getHull()<1)
		{
			//if player died
			//fade out the screen
			gui::IGUIInOutFader *fader =graphics->getGUIEnvironment()->addInOutFader(0,0);
			fader->setColor(SColor(0,0,0,0));
			fader->fadeOut(400);
			GamePaused=true;
		}

		//show turning circle only when turning
		if(CPlayer->getRot().Y-3>CPlayer->turn.Y | CPlayer->getRot().Y+3<CPlayer->turn.Y | CPlayer->getRot().X-3>CPlayer->turn.X | CPlayer->getRot().X+3<CPlayer->turn.X)
			setTurnCircleGUI(true);
		else
			setTurnCircleGUI(false);


		// open the crew/cargo/stats/loadout menu
		if(receiver->IsKeyDown(irr::KEY_KEY_C))
		{
			if(CPlayer->getDocked()==false)
			{
				if(menuop==true)
				{
					menuop=false;
					if(gameMenu->getMenuOpen()==true)
					{
						gameMenu->setMenuOpen(false);
					}
					else
					{
						gameMenu->setMenuOpen(true);
					}
				}
			}
		}
		else
		{
			menuop=true;
		}
	}


}

void GameLoop::cameraControl(KeyListener *receiver, f32 frameDeltaTime)
{
	tMouseW = receiver->mouseWheel();
	//different camera for nav view
	if(NavView!=true)
	{
		//pCam->bindTargetAndRotation(true);


		//constantly update camera position
		//to track player
		//TODO::add camera sway like in BSGO and EvE
		pCam->setTarget(CPlayer->getPos());
		//use sine?
		//vector3df pos = CPlayer->getPos();
		//float tmp = camera_sway*frameDeltaTime;
		//pos.Y += sin(tmp);
		//pCam->setTarget(pos);


		//move camera by determining if mouse moved
		//check pixels the mouse moved
		if(receiver->mouseRButtonDown()==true)
		{
			if(receiver->mouseX()!=tMouseX)
			{
				int i=receiver->mouseX()-tMouseX;
				tMouseX=receiver->mouseX();
				angle+=i;
			}
			if(receiver->mouseY()!=tMouseY)
			{
				int t=receiver->mouseY()-tMouseY;
				tMouseY=receiver->mouseY();
				angle2+=t;
			}
		}

		//change distance of camera to player
		if(receiver->IsKeyDown(irr::KEY_MINUS))
		{
			if(distance<CAMERA_DISTANCE_MAX)
				distance+=10;
		}
		if(receiver->IsKeyDown(irr::KEY_PLUS))
		{
			if(distance>CAMERA_DISTANCE_MIN)
				distance-=10;
		}

		//mousewheel
		if(tMouseW < tMouseW_old)
		{
			if(distance > CAMERA_DISTANCE_MIN)
			{
				distance+=(tMouseW-tMouseW_old)*CAMERA_MOUSE_WHEEL;
				tMouseW_old = tMouseW;
			}
		}
		if(tMouseW > tMouseW_old)
		{
			if(distance < CAMERA_DISTANCE_MAX)
			{
				distance+=(tMouseW-tMouseW_old)*CAMERA_MOUSE_WHEEL;
				tMouseW_old = tMouseW;
			}
		}
		//if angle is too big or too small
		//put it back in 360 limit
		if (angle > 360) 
			angle -= 360;
		else if (angle < 0)
			angle += 360;

		if(angle2>360)
			angle-=360;
		if(angle<0)
			angle+=360;

		//distance cannot be less than zero, or else there will be inversion
		if(distance<0)
			distance=0;

 
		tMouseX=receiver->mouseX();
		tMouseY=receiver->mouseY();    

		core::vector3df targ = pCam->getTarget();
		core::vector3df playerCameraPos;

		//3d trig
		//this math is to determine the location of orbiting camera
		playerCameraPos.Y = sin(angle2*3.14/180)*distance;
		playerCameraPos.Y += targ.Y;

		float temp;
		temp=cos(angle2*3.14/180);


		//some code to calculate position
		core::vector3df old;
		playerCameraPos.X = (sin((angle) * 3.141 / 180) * (distance));
		playerCameraPos.X = playerCameraPos.X*temp;
		playerCameraPos.X  += targ.X;

		playerCameraPos.Z  = (cos((angle) * 3.141/ 180) * (distance));
		playerCameraPos.Z = playerCameraPos.Z*temp;
		playerCameraPos.Z += targ.Z;


		//smooth out camera motion
		old = pCam->getPosition();
		old=old*0.8+playerCameraPos*0.2f;
		pCam->setPosition(old);

	}
	else
	{
		//nav view
		//pCam->bindTargetAndRotation(false);
		pCam->setPosition(core::vector3df(CPlayer->getPos().X,nav_distance,CPlayer->getPos().Z));
		if(tMouseW < tMouseW_old)
		{
			nav_distance+=(tMouseW-tMouseW_old)*500;
			tMouseW_old = tMouseW;
		}
		if(tMouseW > tMouseW_old)
		{
			nav_distance+=(tMouseW-tMouseW_old)*500;
			tMouseW_old = tMouseW;
		}

	}


}

