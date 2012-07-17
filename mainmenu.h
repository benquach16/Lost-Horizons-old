/////////////////////////////////////////////////////////////////
//2011 Ben Quach 
//MainMenu.h
//Main Menu Class Code
/////////////////////////////////////////////////////////////////

#ifndef _MAINMENU_H_
#define _MAINMENU_H_

#include "irrlicht.h"
#include "irrklang.h"
#include "planet.h"

using namespace irr;
using namespace core;
using namespace video;

class CMainMenu
{
public:
	//Initialize the menu, does not need a mainloop
	CMainMenu(irr::IrrlichtDevice *graphics, irrklang::ISoundEngine *sound);
	//Function to delete the object
	void remove();

	//needed in order to detect what buttons are being pressed
	gui::IGUIButton *getNewButton();
	gui::IGUIButton *getLoadgame();
	gui::IGUIButton *getOptions();
	gui::IGUIButton *getQuit();
	~CMainMenu();

private:
	//List of all the objects created by the class
	scene::ISceneNode *corona;
	scene::ICameraSceneNode *cam;
	scene::IParticleSystemSceneNode *nebula;
	scene::IAnimatedMeshSceneNode *asteroids;
	gui::IGUIImage *logo;
	gui::IGUIElement * control;
	gui::IGUIButton *newgame;
	gui::IGUIButton *loadgame;
	gui::IGUIButton *options;
	gui::IGUIButton *quit;

};


#endif