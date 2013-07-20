// This is the current Techdemo of the Irrlicht Engine (c) 2003 by N.Gebhardt.
// This file is not documentated.

#include "CMainMenu.h"

CMainMenu::CMainMenu()
: startButton(0), device(0), start(false), fullscreen(true), selected(1),
	music(true)
{
}



bool CMainMenu::run(bool& outFullscreen, bool& outMusic, video::EDriverType& outDriver)
{
	device = createDevice(video::DT_SOFTWARE,
		core::dimension2d<s32>(512, 384), 16, false, this);

	device->getFileSystem()->addZipFileArchive("irrlicht.dat");
	device->getFileSystem()->addZipFileArchive("..\\..\\media\\irrlicht.dat");

	video::IVideoDriver* driver = device->getVideoDriver();
	scene::ISceneManager* smgr = device->getSceneManager();
	gui::IGUIEnvironment* guienv = device->getGUIEnvironment();

	device->setWindowCaption(L"Irrlicht Engine Techdemo v0.1");

	// add images

	gui::IGUIImage* img = guienv->addImage(core::rectEx<int>(0,0,512,384));
	img->setImage(driver->getTexture("..\\..\\media\\techdemoback.bmp"));

	// add list box

	gui::IGUIListBox* box = guienv->addListBox(core::rectEx<int>(295,23,485,90), 0, 1);
	box->addItem(L"Irrlicht Software Engine [too slow]");
	box->addItem(L"Direct3D 8");
	box->addItem(L"OpenGL [incomplete]");
	box->setSelected(selected);

	// add button

	startButton = guienv->addButton(core::rectEx<int>(420,105,485,130), 0, 2, L"Start Demo");

	// add checkbox

	guienv->addCheckBox(fullscreen, core::rectEx<int>(295,105,400,130), 0, 3, L"Fullscreen");
	guienv->addCheckBox(music, core::rectEx<int>(295,130,400,155), 0, 4, L"Music");

	// add text

	wchar_t* text[] = {		L"Welcome to the Irrlicht Engine v0.1.",
							L"Please select a device to display the demo. What you",
							L"currently see is displayed using the Irrlicht",
							L"Software Engine, but this would be too slow for the",
							L"demo.",
							L"The Irrlicht Engine was written by me, N.Gebhardt.",
							L"The models, maps and textures were placed at my",
							L"disposal by Brian Collins, Michael Cook and",
							L"Jakab Marton. The music was created by M.Rohde",
							L"and is played back by Audiere.",
							0};

	const int textHeight = 15;
	const int top = 180;

	int t=0;
	while(text[t])
	{
		guienv->addStaticText(text[t], false, 
			core::rectEx<int>(295, top + t*textHeight, 485, top + (t+1)*textHeight));
		++t;
	}
	
	// add md2 model

	scene::IAnimatedMesh* mesh = smgr->getMesh("..\\..\\media\\faerie.md2");
	scene::IAnimatedMeshSceneNode* node = smgr->addAnimatedMeshSceneNode(mesh);
	if (node)
	{
		node->setMaterialTexture(0, driver->getTexture("..\\..\\media\\faerie2.bmp"));
		node->setMaterialFlag(video::EMF_LIGHTING, false);
		node->setFrameLoop(0, 310);
	}

	smgr->addCameraSceneNode(0, core::vector3df(60,0,0), core::vector3df(0,0,20));

	while(device->run())
	{
		if (device->isWindowActive())
		{
			driver->beginScene(false, true, video::Color(0,0,0,0));
			guienv->drawAll();
			smgr->drawAll();
			driver->endScene();
		}
	}

	device->drop();

	outFullscreen = fullscreen;
	outMusic = music;

	switch(selected)
	{
	case 0:	outDriver = video::DT_SOFTWARE; break;
	case 1:	outDriver = video::DT_DIRECTX8; break;
	case 2:	outDriver = video::DT_OPENGL; break;
	}

	return start;
}



bool CMainMenu::OnEvent(SEvent event)
{
	if (event.EventType == EET_GUI_EVENT)
	{
		s32 id = event.GUIEvent.Caller->getID();
		switch(id)
		{
		case 1:
			if (event.GUIEvent.EventType == gui::EGET_LISTBOX_CHANGED)
			{
				selected = ((gui::IGUIListBox*)event.GUIEvent.Caller)->getSelected();
				startButton->setEnabled(selected == 1);
			}
			break;
		case 2:
			if (event.GUIEvent.EventType == gui::EGET_BUTTON_CLICKED )
			{
				device->closeDevice();
				start = true;
			}
		case 3:
			if (event.GUIEvent.EventType == gui::EGET_CHECKBOX_CHANGED )
				fullscreen = ((gui::IGUICheckBox*)event.GUIEvent.Caller)->isChecked();
			break;
		case 4:
			if (event.GUIEvent.EventType == gui::EGET_CHECKBOX_CHANGED )
				music = ((gui::IGUICheckBox*)event.GUIEvent.Caller)->isChecked();
			break;
		}
	}

	return false;
}
