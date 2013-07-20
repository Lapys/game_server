// This is the current Techdemo of the Irrlicht Engine (c) 2003 by N.Gebhardt.
// This file is not documentated.

#include "CDemo.h"
#include <stdio.h>

CDemo::CDemo(bool f, bool m, video::EDriverType d)
: fullscreen(f), driverType(d), currentScene(0), quakeLevel(0),
 lightMapNode(0), dynamicNode(0), camera(0), model1(0), model2(0),
 billboard(0), scenePrepared(false), music(m)
{
}


void CDemo::run()
{
	device = createDevice(driverType,
		core::dimension2d<s32>(640, 480), 16, fullscreen, this);

	device->getFileSystem()->addZipFileArchive("irrlicht.dat");
	device->getFileSystem()->addZipFileArchive("..\\..\\media\\irrlicht.dat");
	device->getFileSystem()->addZipFileArchive("map-20kdm2.pk3");
	device->getFileSystem()->addZipFileArchive("..\\..\\media\\map-20kdm2.pk3");

	video::IVideoDriver* driver = device->getVideoDriver();
	scene::ISceneManager* smgr = device->getSceneManager();
	gui::IGUIEnvironment* guienv = device->getGUIEnvironment();

	device->setWindowCaption(L"Irrlicht Engine Techdemo");

	while(device->run() && driver)
	{
		if (device->isWindowActive())
		{
			doSceneSetup();

			driver->beginScene(true, true, backColor);

			smgr->drawAll();
			guienv->drawAll();

			driver->endScene();

			wchar_t tmp[255];
			swprintf(tmp, L"%s fps:%d triangles:%d", text.c_str(), driver->getFPS(),
				driver->getPrimitiveCountDrawed());

			statusText->setText(tmp);
		}
	}

	device->drop();
}


void CDemo::doSceneSetup()
{
	if (currentScene>6)
		currentScene = 2;

	switch(currentScene)
	{
	case 0:
		{
			core::dimension2d<int> size = device->getVideoDriver()->getScreenSize();

			// setup loading screen
			backColor.set(255,90,90,156);
			gui::IGUIImage* img = device->getGUIEnvironment()->addImage(
				core::rectEx<int>(10,10,98,41));

			img->setImage(
				device->getVideoDriver()->getTexture("..\\..\\media\\irrlichtlogo.bmp"));

			const int lwidth = 150;
			const int lheight = 16;

			core::rectEx<int> pos(10, size.Height-lheight-10, 10+lwidth, size.Height-10);
			
			device->getGUIEnvironment()->addImage(pos);
			statusText = device->getGUIEnvironment()->addStaticText(L"Loading...", true, pos);

			++currentScene;
		}
		break;
	case 1:
		{
			// load quake levels 

			video::IVideoDriver* driver = device->getVideoDriver();

			quakeLevel = device->getSceneManager()->getMesh("20kdm2.bsp");
			
			if (quakeLevel)
			{
				dynamicNode = device->getSceneManager()->addOctTreeSceneNode(quakeLevel->getMesh(0));
				if (dynamicNode)
				{
					dynamicNode->setMaterialType(video::EMT_SOLID);
					dynamicNode->setMaterialFlag(video::EMF_LIGHTING, true);
					dynamicNode->setRelativePosition(core::vector3df(-1450,-144,-1350));
					dynamicNode->setVisible(false);
				}

				lightMapNode = device->getSceneManager()->addOctTreeSceneNode(quakeLevel->getMesh(0));
				if (lightMapNode)
				{
					lightMapNode->setRelativePosition(core::vector3df(-1450,-144,-1350));
					lightMapNode->setVisible(true);
				}
			}

			// load sydney

			scene::IAnimatedMesh* mesh = 0;
			mesh = device->getSceneManager()->getMesh("..\\..\\media\\sydney.md2");
			if (mesh)
			{
				model1 = device->getSceneManager()->addAnimatedMeshSceneNode(mesh,
					0, -1, core::vector3df(0, -50, 0));
				if (model1)
				{
					model1->setMaterialTexture(0, 
						driver->getTexture("..\\..\\media\\sydney.bmp"));

					model1->setMaterialFlag(video::EMF_LIGHTING, true);
					model1->setFrameLoop(0, 310);
				}
			}

			// load faerie

			mesh = device->getSceneManager()->getMesh("..\\..\\media\\faerie.md2");
			if (mesh)
			{
				model2 = device->getSceneManager()->addAnimatedMeshSceneNode(mesh,
					0, -1, core::vector3df(0, -50, -100));
				if (model2)
				{
					model2->setMaterialTexture(0, 
						driver->getTexture("..\\..\\media\\Faerie5.BMP"));

					model2->setMaterialFlag(video::EMF_LIGHTING, true);
					model2->setFrameLoop(0, 310);

					scene::ISceneNodeAnimator* anim = 0;
					anim = device->getSceneManager()->createRotationAnimator(
						core::vector3df(0.0f, 0.3f, 0.0f));
					model2->addAnimator(anim);
					anim->drop();
				}
			}

			// load music

			#ifdef USE_AUDIERE
			if (music)
				startAudiere();
			#endif
	
			// create lights

			scene::ILightSceneNode* light = device->getSceneManager()->addLightSceneNode(0,
			core::vector3df(0, -30, -100), video::Colorf(3.0f, 1.0f, 1.0f, 0.0f), 450.0f);

			scene::ISceneNodeAnimator* anim = 
				device->getSceneManager()->createFlyCircleAnimator(core::vector3df(0.0f, 1.0f, 0.0f), 50.0f, -0.0005f);
			light->addAnimator(anim);
			anim->drop();

			billboard = device->getSceneManager()->addBillboardSceneNode(
				light, core::dimension2d<f32>(20,20));
			billboard->setMaterialFlag(video::EMF_LIGHTING, false);
			billboard->setMaterialTexture(0, driver->getTexture("..\\..\\media\\particle.tga"));
			billboard->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);


			scene::IBillboardSceneNode* bill = device->getSceneManager()->addBillboardSceneNode(
				billboard, core::dimension2d<f32>(10,10));
			bill->setMaterialFlag(video::EMF_LIGHTING, false);
			bill->setMaterialTexture(0, driver->getTexture("..\\..\\media\\particle.bmp"));
			bill->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);


			anim = device->getSceneManager()->createFlyCircleAnimator(core::vector3df(0.0f, 1.0f, 0.0f), 8.0f, 0.005f);
			light->addAnimator(anim);
			anim->drop();
			bill->addAnimator(anim);

			backColor.set(0,0,0,0);

			++currentScene;
			scenePrepared = false;
		}
		// FALLTHROUGH
	case 2:
		{
			// cycle in türen, lightmap
			if (!scenePrepared)
			{
				lightMapNode->setVisible(true);
				dynamicNode->setVisible(false);
				billboard->setVisible(false);
				model1->setVisible(false);
				model2->setVisible(false);		

				device->getSceneManager()->setActiveCamera(0);
				if (camera)
					camera->remove();

				// grosser circle um türen herum
				camera = device->getSceneManager()->addCameraSceneNode(0,
				core::vector3df(0,150,0), core::vector3df(0,0,0));
				scene::ISceneNodeAnimator* anim = 0;
				anim = device->getSceneManager()->createFlyCircleAnimator(
					core::vector3df(0.0f, 1.0f, 0.0f), 550.0f, 0.0002f);
				camera->addAnimator(anim);
				anim->drop();

				text = L"Lightmaps";

				time = GetTickCount();
				scenePrepared = true;
			}

			DWORD now = GetTickCount();

			if (now - time > 20000)
			{
				++currentScene;
				scenePrepared = false;
			}
		}
		break;
	case 4:
		{
			// schwenk im innerem raum

			if (!scenePrepared)
			{
				lightMapNode->setVisible(false);
				dynamicNode->setVisible(true);
				billboard->setVisible(true);
				model1->setVisible(true);
				model2->setVisible(true);	

				device->getSceneManager()->setActiveCamera(0);
				if (camera)
					camera->remove();

				// schwenk im innerem raum
				camera = device->getSceneManager()->addCameraSceneNode(0,
				core::vector3df(0,-20,0), core::vector3df(0,-20,-100));
				scene::ISceneNodeAnimator* anim = 0;
				anim = device->getSceneManager()->createFlyCircleAnimator(
					core::vector3df(0.0f, 1.0f, 0.0f), 70.0f, 0.0005f);
				camera->addAnimator(anim);
				anim->drop();

				text = L"Dynamic lights";

				time = GetTickCount();
				scenePrepared = true;
			}

			DWORD now = GetTickCount();

			if (now - time > 20000)
			{
				++currentScene;
				scenePrepared = false;
			}
		}
		break;
	case 5:
		{
			if (!scenePrepared)
			{
				// schwenk um türen herum, dynamic light.

				lightMapNode->setVisible(false);
				dynamicNode->setVisible(true);
				billboard->setVisible(true);
				model1->setVisible(true);
				model2->setVisible(true);			

				device->getSceneManager()->setActiveCamera(0);
				if (camera)
					camera->remove();

				// langsamer circle um die türen herum
				camera = device->getSceneManager()->addCameraSceneNode(0,
					core::vector3df(0,150,0), core::vector3df(0,0,0));
				scene::ISceneNodeAnimator* anim = 0;
				anim = device->getSceneManager()->createFlyCircleAnimator(
					core::vector3df(0.0f, 1.0f, 0.0f), 550.0f, 0.0002f);
				camera->addAnimator(anim);
				anim->drop();

				text = L"Quake 3 Maps";

				time = GetTickCount();
				scenePrepared = true;
			}

			DWORD now = GetTickCount();

			if (now - time > 15000)
			{
				++currentScene;
				scenePrepared = false;
			}
		}
		break;
	case 6:
		{
			// sydney mitte 

			if (!scenePrepared)
			{
				lightMapNode->setVisible(false);
				dynamicNode->setVisible(true);
				billboard->setVisible(true);
				model1->setVisible(true);
				model2->setVisible(true);	

				device->getSceneManager()->setActiveCamera(0);
				if (camera)
					camera->remove();

				//kleiner circle um sydney in der mitte
				camera = device->getSceneManager()->addCameraSceneNode(0,
					core::vector3df(0,-20,0), core::vector3df(0,-25,0));
				scene::ISceneNodeAnimator* anim = 0;
				anim = device->getSceneManager()->createFlyCircleAnimator(
					core::vector3df(0.0f, 1.0f, 0.0f), 40.0f, 0.0003f);
				camera->addAnimator(anim);
				anim->drop();

				text = L"Characters anims";

				time = GetTickCount();
				scenePrepared = true;
			}

			DWORD now = GetTickCount();

			if (now - time > 20000)
			{
				++currentScene;
				scenePrepared = false;
			}

		}
		break;
	case 3:
		{
			// cylce über dächern, lightmaps

			if (!scenePrepared)
			{
				lightMapNode->setVisible(true);
				dynamicNode->setVisible(false);
				billboard->setVisible(false);
				model1->setVisible(false);
				model2->setVisible(false);			

				device->getSceneManager()->setActiveCamera(0);
				if (camera)
					camera->remove();

				// grosser circle über den dächern
				camera = device->getSceneManager()->addCameraSceneNode(0,
					core::vector3df(0,600,0), core::vector3df(0,-300,0));
				scene::ISceneNodeAnimator* anim = 0;
				anim = device->getSceneManager()->createFlyCircleAnimator(
					core::vector3df(0.0f, 1.0f, 0.0f), 800.0f, 0.0002f);
				camera->addAnimator(anim);
				anim->drop();

				text = L"Huge Levels";

				time = GetTickCount();
				scenePrepared = true;
			}

			DWORD now = GetTickCount();

			if (now - time > 30000)
			{
				++currentScene;
				scenePrepared = false;
			}
		}
		break;
	}
}


bool CDemo::OnEvent(SEvent event)
{
	if (event.EventType == EET_KEY_INPUT_EVENT)
	{
		if (event.KeyInput.Key == KEY_ESCAPE &&
			event.KeyInput.PressedDown == false)
			device->closeDevice();
	}

	return false;
}

#ifdef USE_AUDIERE
void CDemo::startAudiere()
{
	audiereDevice = OpenDevice();
	if (!audiereDevice) 
		return;

	stream = OpenSound(audiereDevice.get(), "IrrlichtTheme.mp3", true);
	if (!stream) 
	{
		stream = OpenSound(audiereDevice.get(), "..\\..\\media\\IrrlichtTheme.mp3", true);
		if (!stream)
			return;
	}
		

	stream->setRepeat(true);
	//stream->setVolume(0.5f); // 50% volume
	stream->play();
}
#endif