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
///////////////////////////////////////////////////////////////////////s

#include "stdafx.h"
#include "init.h"




//basic settings go here
#define _BLOOMSIZE_ 256
#define DEFAULT_X 1024
#define DEFAULT_Y 768
#define DEFAULT_VSYNC false
#define DEFAULT_QUALITY true



//add irrlicht and irrklang libraries
#ifdef _IRR_WINDOWS_
#pragma comment(lib, "Irrlicht.lib")
#pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")
#pragma comment(lib,"irrKlang.lib")
#endif


//temporary
//theres a memory leak with this music file
irrklang::ISound *music;


//video global variables
int gWindowY;
int gWindowX;
bool gFullscreen;
bool gVsync;
bool gQuality;
char gDriver;

video::SColor colors[] =
{
	video::SColor(255,96,96,96),
	video::SColor(255,96,96,96),
	video::SColor(255,96,96,96),
	video::SColor(255,96,96,96)
};

video::SColor colors1[] =
{
	video::SColor(255,224,224,254),
	video::SColor(255,234,234,234),
	video::SColor(255,224,224,254),
	video::SColor(255,234,234,234)
};

video::SColor colors2[] =
{
	video::SColor(255,32,32,32),
	video::SColor(255,32,32,32),
	video::SColor(255,32,32,32),
	video::SColor(255,32,32,32)
};





//---------------------------------------------------------------------------------------
//isolate from main loop
//function to generate a new config file
//simply deletes old file and creates a new one
//BEGIN FUNC
void CInit::createNewVideoConfig(int x, int y, bool windowed, bool vsync, bool quality)
{
	//check that there isnt already a file there
	//so we can overwrite
	ifstream vfileReader;
	vfileReader.open("config/video.ini");
	if(vfileReader)
	{
		remove("config/video.ini");
	}

	//generate a new text file
	ofstream newfile;

	//default variables
	gWindowX = x;
	gWindowY = y;
	gFullscreen = windowed;

	//use ofstream to add lines to the ini
	newfile.open("config/video.ini");
	newfile<<"[VIDEO]"<<endl;
	newfile<<"X=" << x << ";" <<endl;
	newfile<<"Y="<< y << ";" <<endl;
	if(windowed==true)
		newfile<<"Fullscreen=1;"<<endl;
	else
		newfile<<"Fullscreen=0;"<<endl;
	if(vsync==true)
		newfile<<"Vsync=1;"<<endl;
	else 
		newfile<<"Vsync=0;"<<endl;
	if(quality==true)
		newfile << "Quality=1"<<endl;
	else
		newfile << "Quality=0"<<endl;
	newfile.close();


}
//end function
//-------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------
//readVideoConfig()
//just reads information from the video.ini
//begin func
void CInit::readVideoConfig()
{
	//so ini reading is pretty much a giant hack
	//Everything in the ini file is saved into a string, which can only store a maximum of 256 characters
	//The program reads through the string and looks for the X= and the ; parts
	//once the program saves the value between these two points, this used up part is then deleted from the string

	//create video.ini file reader
	ifstream vfileReader;
	vfileReader.open("config/video.ini");
	if(vfileReader)
	{
		//if video.ini exists
		if(vfileReader.is_open())
		{
			std::string vfile;
			//256 characters max
			char tmp[256];	
			//while loaded file
			while(vfileReader.good())
			{
				//have vfile contain everything in the config as a string
				//256 character max
				vfileReader.getline(tmp,256);
				vfile = tmp;

				//X res
				//Find the location of X=
				size_t x_pos = vfile.find("X=");
				//And the ; which ends the line
				size_t newline_x_pos = vfile.find(";");

				//Ensures that the line is being properly read in the right direction
				if(newline_x_pos > x_pos)
				{
					//Find the size of the variable declaration inside of the file
					int xsize = newline_x_pos - x_pos;
					char *x = new char [xsize];
					//Copy over the variable declartion into the new char variable
					//Then delete the X= since we no longer need that
					vfile.copy(x,xsize,int(x_pos)+2);
					//Get the number from the x variable and convert into an integer
					gWindowX = std::atoi(x);
					//delete the x res part of the string so the other part doesnt get confused
					vfile.erase(x_pos,newline_x_pos);
				}

				//Y res
				size_t y_pos = vfile.find("Y=");
				size_t newline_y_pos = vfile.find(";");
				if(newline_y_pos > y_pos)
				{
					int ysize = newline_y_pos - y_pos;
					char *y = new char[ysize];
					vfile.copy(y,ysize,int(y_pos)+2);
					gWindowY=std::atoi(y);
					vfile.erase(y_pos,newline_y_pos);
				}

				//fullscreen
				size_t full_pos = vfile.find("Fullscreen=");
				size_t newline_full_pos = vfile.find(";");
				if(newline_full_pos > full_pos)
				{
					int fsize = newline_full_pos - full_pos;
					char *f = new char[fsize];
					//The full_pos+11 is used in order to delete the fullscreen= part of the config since we no longer need it
					vfile.copy(f,fsize,int(full_pos)+11);
					gFullscreen=std::atoi(f);
					vfile.erase(full_pos,newline_full_pos);
				}

				//VSYNC
				size_t sync_pos = vfile.find("Vsync=");
				size_t newline_sync_pos = vfile.find(";");
				if(newline_sync_pos > sync_pos)
				{
					int vsize = newline_sync_pos - sync_pos;
					char *v = new char[vsize];
					vfile.copy(v,vsize,int(sync_pos)+6);
					gVsync = std::atoi(v);
					vfile.erase(sync_pos,newline_sync_pos);
				}

				//GRAPHICS QUAL
				size_t qual_pos = vfile.find("Quality=");
				size_t newline_qual_pos = vfile.find(";");
				if(newline_qual_pos > qual_pos)
				{
					int qsize = newline_qual_pos - qual_pos;
					char *q = new char[qsize];
					vfile.copy(q,qsize, int(qual_pos)+8);
					gQuality = std::atoi(q);
					vfile.erase(qual_pos,newline_qual_pos);
				}
			}
		}
	}
	else
	{
		//just use default settings if no video.ini exists
		createNewVideoConfig(1024,768,true,false,true);
	}

}
//end func
//--------------------------------------------------------------------------------------------


//---------------------------------------------------------------------------------------------
// main menu graphics loop goes here
//initializer
//create the initial program which will eventuall create everything else
CInit::CInit():graphics(0), time(0.f), sound(0), exit(false)
{
	StartMenu();
}

//main menu graphics loop goes here
void CInit::StartMenu()
{
	//setup varialbes based on whats read from video.ini
	CInit::readVideoConfig();

	//initialize the graphics device and the sound engine
	if(exit!=true)
	{
	graphics		=		createDevice(video::EDT_DIRECT3D9, core::dimension2d<u32>(gWindowX,gWindowY),32,gFullscreen,gVsync,false,0);
	sound			=		createIrrKlangDevice();
	}
	

	//create key receiver
	receiver = new KeyListener;
	graphics->setEventReceiver(receiver);

	core::dimension2d<u32> t = graphics->getVideoDriver()->getScreenSize();
	graphics->getVideoDriver()->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS,true);



	//setup sound
	sound->setSoundVolume(1.f);
	sound->setDefault3DSoundMinDistance(500);

	bool start;
	start=false;
	load=false;
	

	//create menu
	CMainMenu *menu = new CMainMenu(graphics,sound);
	COptionMenu *option = new COptionMenu(graphics);

	music = sound->play2D("res/sounds/music/ambient1.mp3",true,false,true);
	music->drop();


	//create the textures used by the render pipeline bloom
	video::ITexture* mainTarget = graphics->getVideoDriver()->addRenderTargetTexture(graphics->getVideoDriver()->getScreenSize(),"mainTarget");
	video::ITexture* overlay = graphics->getVideoDriver()->addRenderTargetTexture(graphics->getVideoDriver()->getScreenSize(),"overlay");
	video::ITexture* rtt0;
	video::ITexture* temp;

	rtt0 = graphics->getVideoDriver()->addRenderTargetTexture(core::dimension2du(_BLOOMSIZE_,_BLOOMSIZE_),"rtt0");

	CScreenQuadSceneNode* screenQuad = new CScreenQuadSceneNode(graphics->getSceneManager()->getRootSceneNode(),graphics->getSceneManager(),10);
	screenQuad->getMaterial(0).setTexture(0,mainTarget);
	screenQuad->getMaterial(0).setTexture(1,rtt0);

	int timetoload=0;
	fader = new CFader(graphics);

	graphics->getCursorControl()->setVisible(false);

	//This is NOT the main graphics loop
	//just the menu loop
	//this will lead into the main graphics loop
	while(graphics->run())
	{

		graphics->getVideoDriver()->beginScene(true, true, video::SColor(255,100,100,140));




		//if the new game button is pressed
		if(menu->getNewButton()->isPressed())
		{
			start = true;
		}
		//load game
		if(menu->getLoadgame()->isPressed())
		{
			//loadgame only works if the save file exists
			ifstream ifile("saves/save.lsav");
			if(ifile)
			{
				start=true;
				load=true;
			}
			else
			{
				graphics->getGUIEnvironment()->addMessageBox(L"No Savegame exists",L"You must save a game before loading a game.",true);
			}
		}
		//open options menu
		if(menu->getOptions()->isPressed())
		{
			option->setVisible(true);
		}
		//quit game
		if(menu->getQuit()->isPressed())
		{
			break;
		}


		//run through the options menu as well
		option->loop();
		//if the apply button get pressed
		//update init file changes
		if(option->getApplyButton()->isPressed())
		{
			createNewVideoConfig(option->getResolutionX(),option->getResolutionY(),option->getWindowed(),false,option->getHighQuality());
		}

		if(gQuality==true)
			graphics->getVideoDriver()->setRenderTarget(mainTarget,true,true,video::SColor(255,128,160,160));
		graphics->getSceneManager()->drawAll();

		if(gQuality==true)
		{
			graphics->getVideoDriver()->setRenderTarget(rtt0,true,true,video::SColor(0,0,0,0));
			graphics->getVideoDriver()->draw2DImage(mainTarget,core::rect<s32>(0,0,_BLOOMSIZE_,_BLOOMSIZE_),core::rect<s32>(0,0,gWindowX,gWindowY),
				0,colors
				);


			graphics->getVideoDriver()->setRenderTarget(overlay,true,true);
			screenQuad->render();

			graphics->getVideoDriver()->setRenderTarget(video::ERT_FRAME_BUFFER,true,true);		
			graphics->getVideoDriver()->draw2DImage(overlay,core::rect<s32>(0,0,gWindowX,gWindowY),core::rect<s32>(0,0,gWindowX,gWindowY),
				0,colors1
				);
		}


			
		//create borders so the menu loops more spiffy
		//also programmer art

		graphics->getVideoDriver()->draw2DRectangle(core::rect<s32>(t.Width/2-90,t.Height/2-20,t.Width/2+90,t.Height/2+210),SColor(90,0,0,0),SColor(90,0,0,0),SColor(90,0,0,0),SColor(90,0,0,0));
		graphics->getVideoDriver()->draw2DRectangle(core::rect<s32>(0,0,t.Width,50),SColor(90,0,0,0),SColor(90,0,0,0),SColor(90,0,0,0),SColor(90,0,0,0));
		graphics->getVideoDriver()->draw2DRectangle(core::rect<s32>(0,t.Height-50,t.Width,t.Height),SColor(90,0,0,0),SColor(90,0,0,0),SColor(90,0,0,0),SColor(90,0,0,0));
		graphics->getVideoDriver()->draw2DRectangle(core::rect<s32>(t.Width/2-80,t.Height/2-10,t.Width/2+80,t.Height/2+200),SColor(90,125,135,148),SColor(90,125,135,148),SColor(90,125,135,148),SColor(90,125,135,148));
		graphics->getGUIEnvironment()->drawAll();
		graphics->getVideoDriver()->draw2DImage(graphics->getVideoDriver()->getTexture("res/menu/cursor.png"),
			position2d<s32>(graphics->getCursorControl()->getPosition().X,graphics->getCursorControl()->getPosition().Y),
			rect<s32>(0,0,32,32), 
			0, 
			SColor(255,255,255,255), 
			true);
		//ok
		//so the loop doesnt break until after 4.5 seconds because we want the screen to fade to black first
		//transitions and all that extra crap
		if(start==true)
		{
			if(timetoload<graphics->getTimer()->getTime())
			{
				break;
			}
			else
			{
				fader->update(5,graphics->getTimer()->getTime(),true);
			}
		}
		else
		{
			timetoload=graphics->getTimer()->getTime()+4500;
		}


		graphics->getVideoDriver()->endScene();


	}


	menu->remove();
	
	//create game object
	//the start bool is necessary
	//so when the player exits from the menu
	//the game doesn't load everything into memory
	if(start==true)
	{
		graphics->getGUIEnvironment()->setFocus(0);
		Game = new GameLoop(graphics,receiver, sound, load, gQuality);

		StartGame();
	}
	else
	{
		//delete everything
		drop();
	}

}


//destructor
CInit::~CInit()
{

}

//This does contain the main graphics loop
//used by the game
void CInit::StartGame()
{

	//create the textures used by the render pipeline bloom
	video::ITexture* mainTarget = graphics->getVideoDriver()->addRenderTargetTexture(graphics->getVideoDriver()->getScreenSize(),"mainTarget");
	video::ITexture* overlay = graphics->getVideoDriver()->addRenderTargetTexture(graphics->getVideoDriver()->getScreenSize(),"overlay");
	video::ITexture* rtt0;
	video::ITexture* temp;




	rtt0 = graphics->getVideoDriver()->addRenderTargetTexture(core::dimension2du(_BLOOMSIZE_,_BLOOMSIZE_),"rtt0");

	CScreenQuadSceneNode* screenQuad = new CScreenQuadSceneNode(graphics->getSceneManager()->getRootSceneNode(),graphics->getSceneManager(),10);
	screenQuad->getMaterial(0).setTexture(0,mainTarget);
	screenQuad->getMaterial(0).setTexture(1,rtt0);


	
	while(graphics->run())
	{
		if(graphics->isWindowActive())
		{
			//main graphics loop
			graphics->getVideoDriver()->beginScene(true, true, video::SColor(255,100,100,140));
			//fake bloom
			if(gQuality==true)
				graphics->getVideoDriver()->setRenderTarget(mainTarget,true,true,video::SColor(255,128,160,160));
			graphics->getSceneManager()->drawAll();

			if(gQuality==true)
			{
				graphics->getVideoDriver()->setRenderTarget(rtt0,true,true,video::SColor(0,0,0,0));
				graphics->getVideoDriver()->draw2DImage(mainTarget,core::rect<s32>(0,0,_BLOOMSIZE_,_BLOOMSIZE_),core::rect<s32>(0,0,gWindowX,gWindowY),
					0,colors
					);


				graphics->getVideoDriver()->setRenderTarget(overlay,true,true);
				screenQuad->render();

				graphics->getVideoDriver()->setRenderTarget(video::ERT_FRAME_BUFFER,true,true);		
				graphics->getVideoDriver()->draw2DImage(overlay,core::rect<s32>(0,0,gWindowX,gWindowY),core::rect<s32>(0,0,gWindowX,gWindowY),
					0,colors1
					);
			}


			graphics->getGUIEnvironment()->drawAll();
			//draw mouse
			graphics->getVideoDriver()->draw2DImage(graphics->getVideoDriver()->getTexture("res/menu/cursor.png"),
				position2d<s32>(graphics->getCursorControl()->getPosition().X,graphics->getCursorControl()->getPosition().Y),
				rect<s32>(0,0,32,32), 
				0, 
				SColor(255,255,255,255), 
				true);
			Game->Run();	//run game




			//disp fps
			int fps = graphics->getVideoDriver()->getFPS();
			core::stringw c(L"Lost Horizons [");
			c+=fps;
			c+="]";
			graphics->setWindowCaption(c.c_str());
			//if(graphics->getGUIEnvironment()->getFocus()!=0)
			//graphics->setWindowCaption(graphics->getGUIEnvironment()->getFocus()->getText());
			//graphics crap

			graphics->getVideoDriver()->endScene();

			//quit game
			if(Game->exitGame()==true)
			{
				break;
			}

		}

	}
	

	//drop

	//drop();
	exit=true;
	Game->drop();
	StartMenu();

}

void CInit::drop()
{
	//remove stuff
	//make sure theres no memory leaks
	graphics->drop();
	sound->drop();
	delete receiver;
}