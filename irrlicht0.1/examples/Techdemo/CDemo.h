// This is the current Techdemo of the Irrlicht Engine (c) 2003 by N.Gebhardt.
// This file is not documentated.

#ifndef __C_DEMO_H_INCLUDED__
#define __C_DEMO_H_INCLUDED__

#define USE_AUDIERE

#include <windows.h>
#include <irrlicht.h>

#ifdef USE_AUDIERE
#include <audiere.h>
using namespace audiere;
#pragma comment (lib, "audiere.lib")
#endif

using namespace irr;

class CDemo : public IEventReceiver
{
public:

	CDemo(bool fullscreen, bool music, video::EDriverType driver);

	void run();

	virtual bool OnEvent(SEvent event);

private:

	void doSceneSetup();

	bool fullscreen;
	bool music;
	bool scenePrepared;
	video::EDriverType driverType;
	IrrlichtDevice *device;

#ifdef USE_AUDIERE
	void startAudiere();
	AudioDevicePtr audiereDevice;
	OutputStreamPtr stream;
#endif

	int currentScene;
	video::Color backColor;
	gui::IGUIElement* statusText;
	scene::ISceneNode* dynamicNode;
	scene::ISceneNode* lightMapNode;
	scene::IAnimatedMesh* quakeLevel;
	scene::ICameraSceneNode* camera;
	scene::IAnimatedMeshSceneNode* model1;
	scene::IAnimatedMeshSceneNode* model2;
	scene::IBillboardSceneNode* billboard;

	DWORD time;

	core::stringw text;
};

#endif