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
#include "mainmenu.h"

//annyonig to read
CMainMenu::CMainMenu(irr::IrrlichtDevice *graphics,irrklang::ISoundEngine *sound)
{

	//setup GUI font and other stuff
	gui::IGUISkin *skin = graphics->getGUIEnvironment()->createSkin(gui::EGST_WINDOWS_METALLIC);
	graphics->getGUIEnvironment()->setSkin(skin);
	gui::IGUIFont *micro = graphics->getGUIEnvironment()->getFont("res/font/verdana_micro.xml");
	gui::IGUIFont *menu_font= graphics->getGUIEnvironment()->getFont("res/font/large.xml");
	gui::IGUIFont *astro= graphics->getGUIEnvironment()->getFont("res/font/system.xml");
	graphics->getGUIEnvironment()->getSkin()->setFont(astro);

	//setup colors for gui
	for (s32 i=0; i<gui::EGDC_COUNT ; i++)
	{
		video::SColor col = graphics->getGUIEnvironment()->getSkin()->getColor((gui::EGUI_DEFAULT_COLOR)i);
		col.setAlpha(208);
		col.setBlue(128);
		col.setGreen(118);
		col.setRed(108);
		graphics->getGUIEnvironment()->getSkin()->setColor((gui::EGUI_DEFAULT_COLOR)i, col);
	}

	graphics->getGUIEnvironment()->getSkin()->setColor(gui::EGDC_BUTTON_TEXT,video::SColor(255,255,255,255));
	graphics->getGUIEnvironment()->getSkin()->setColor(gui::EGDC_HIGH_LIGHT_TEXT,video::SColor(255,255,255,255));
	graphics->getGUIEnvironment()->getSkin()->setColor(gui::EGDC_3D_DARK_SHADOW,video::SColor(128,40,50,60));
	graphics->getGUIEnvironment()->getSkin()->setColor(gui::EGDC_3D_SHADOW,video::SColor(128,80,90,100));
	graphics->getGUIEnvironment()->getSkin()->setColor(gui::EGDC_ACTIVE_BORDER,video::SColor(255,145,155,165));
	graphics->getGUIEnvironment()->getSkin()->setColor(gui::EGDC_INACTIVE_BORDER,video::SColor(128,80,90,100));
	graphics->getGUIEnvironment()->getSkin()->setColor(gui::EGDC_GRAY_TEXT,video::SColor(128,40,50,60));
	graphics->getGUIEnvironment()->getSkin()->setColor(gui::EGDC_WINDOW_SYMBOL,video::SColor(255,255,255,255));
	graphics->getGUIEnvironment()->getSkin()->setColor(gui::EGDC_INACTIVE_CAPTION, video::SColor(255,200,200,200));
	graphics->getGUIEnvironment()->getSkin()->setColor(gui::EGDC_ACTIVE_CAPTION, video::SColor(255,250,250,250));	


	//important to set up menu
	core::dimension2d<u32> t = graphics->getVideoDriver()->getScreenSize();


	//Create logo
	logo = graphics->getGUIEnvironment()->addImage(graphics->getVideoDriver()->getTexture("res/menu/lost_horizons_logo.png"),core::position2d<s32>(t.Width/2-256,0));
	graphics->setWindowCaption(L"Lost Horizons");

	//create menu buttons
	control=graphics->getGUIEnvironment()->addGUIElement("context",0);
	newgame = graphics->getGUIEnvironment()->addButton(core::rect<int>(t.Width/2-50,t.Height/2+20,t.Width/2+50,t.Height/2+40),control,0,L"New Game");
	loadgame = graphics->getGUIEnvironment()->addButton(core::rect<int>(t.Width/2-50,t.Height/2+60,t.Width/2+50,t.Height/2+80),control,0,L"Load Game");
	options = graphics->getGUIEnvironment()->addButton(core::rect<int>(t.Width/2-50,t.Height/2+100,t.Width/2+50,t.Height/2+120),control,0,L"Options");
	quit = graphics->getGUIEnvironment()->addButton(core::rect<int>(t.Width/2-50,t.Height/2+140,t.Width/2+50,t.Height/2+160),control,0,L"Quit");

	//setup camera for menu scene
	cam = graphics->getSceneManager()->addCameraSceneNode();
	cam->setPosition(vector3df(0,0,0));
	cam->setFarValue(5000000);



	//sun* menustar = new sun(graphics,core::vector3df(-20000,500,70000));
	//create sun for menu background
	corona=graphics->getSceneManager()->addBillboardSceneNode(0,dimension2d<f32>(50000,50000),vector3df(-20000,500,70000));
	corona->setMaterialTexture(0,graphics->getVideoDriver()->getTexture("res/particlewhite.bmp"));
	corona->setMaterialFlag(video::EMF_LIGHTING, false);
	corona->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);

	scene::IBillboardSceneNode *corona3 = graphics->getSceneManager()->addBillboardSceneNode(corona,dimension2d<f32>(130000,110000),vector3df(0,0,0));
	corona3->setMaterialTexture(0,graphics->getVideoDriver()->getTexture("res/textures/engine_corona.png"));
	corona3->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);
	//setup menu background
	scene::ISceneNode *skybox = graphics->getSceneManager()->addSkyBoxSceneNode(
		graphics->getVideoDriver()->getTexture("res/textures/skyboxes/3/space_top3.jpg"),
		graphics->getVideoDriver()->getTexture("res/textures/skyboxes/3/space_bottom4.jpg"),
		graphics->getVideoDriver()->getTexture("res/textures/skyboxes/3/space_left2.jpg"),
		graphics->getVideoDriver()->getTexture("res/textures/skyboxes/3/space_right1.jpg"),
		graphics->getVideoDriver()->getTexture("res/textures/skyboxes/3/space_front5.jpg"),
		graphics->getVideoDriver()->getTexture("res/textures/skyboxes/3/space_back6.jpg"));
	graphics->getSceneManager()->setAmbientLight(video::SColor(64,64,64,64));	

	nebula = graphics->getSceneManager()->addParticleSystemSceneNode(false,cam);
	scene::IParticleSphereEmitter *em = nebula->createSphereEmitter(vector3df(-800,0,100),10,vector3df(0.02,0,0),1,1,SColor(255,200,220,225),SColor(255,200,220,225),15000,25000,0,dimension2df(500,500),dimension2df(2000,2000));
	nebula->setEmitter(em);
	em->drop();
	nebula->setMaterialFlag(video::EMF_LIGHTING,false);
	nebula->setMaterialTexture(0,graphics->getVideoDriver()->getTexture("res/textures/fog.pcx"));
	nebula->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);

	scene::IParticleAffector *af = nebula->createFadeOutParticleAffector();
	nebula->addAffector(af);
	af->drop();

	asteroids = graphics->getSceneManager()->addAnimatedMeshSceneNode(graphics->getSceneManager()->getMesh("res/models/planets/asteroid.x"));
	asteroids->setMaterialTexture(0,graphics->getVideoDriver()->getTexture("res/roid.jpg"));
	asteroids->setPosition(vector3df(-20000,0,60000));
	asteroids->setScale(vector3df(8,8,8));


}

void CMainMenu::remove()
{
	delete this;
}

//delete everything
CMainMenu::~CMainMenu()
{
	corona->remove();
	cam->remove();
	newgame->remove();
	loadgame->remove();
	options->remove();
	quit->remove();

	logo->remove();
	nebula->remove();
	asteroids->remove();
}

//used in the graphics loop in order to detect if buttons were pressed
gui::IGUIButton *CMainMenu::getNewButton()
{
	return newgame;
}

gui::IGUIButton *CMainMenu::getLoadgame()
{
	return loadgame;
}

gui::IGUIButton *CMainMenu::getOptions()
{
	return options;
}

gui::IGUIButton *CMainMenu::getQuit()
{
	return quit;
}
