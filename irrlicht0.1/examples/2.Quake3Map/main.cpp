/*
This Tutorial shows how to load a Quake 3 map into the
engine, create a SceneNode for optimizing the speed of
rendering and how to create a user controlled camera.

Lets start like the HelloWorld example: We include
the irrlicht header files. This time we also include 
the stdio.h for the swprintf function.
*/
#include <irrlicht.h>
#include <stdio.h>

/*
As already written in the HelloWorld example, in the Irrlicht
Engine, everything can be found in the namespace 'irr'. 
To get rid of the irr:: in front of the name of every class,
we tell the compiler that we use that namespace from now on, 
and we will not have to write that 'irr::'.
There are 5 other sub namespaces 'core', 'scene', 'video',
'io' and 'gui'. Unlike in the HelloWorld example,
we do not a 'using namespace' for these 5 other namespaces
because in this way you will see what can be found in which
namespace. But if you like, you can also include the namespaces
like in the previous example. Code just like you want to.
*/
using namespace irr;

/*
Again, to be able to use the Irrlicht.DLL file, we need to link with the 
Irrlicht.lib. We could set this option in the project settings, but
to make it easy, we use a pragma comment lib:
*/
#pragma comment(lib, "Irrlicht.lib")

/*
To store a pointer to our camera, we create this global variable.
As you can see, the Camera scene node classis located in the namespace
'scene', like everything which has to do with the 3d scene: Meshes,
Cameras, Animators and stuff like that.
*/
scene::ICameraSceneNode* camera = 0;

/*
To get events like mouse and keyboard input, or GUI events like 
"the OK button has been clicked", we need an object wich is derived
from the IEventReceiver object. There is only one method to 
override: OnEvent. This method will be called by the engine when
an event happened. 
In our example we simply delegate every event to our camera object,
because we want the camera to be moved by the input of the user.
*/
class MyEventReceiver : public IEventReceiver
{
public:
	virtual bool OnEvent(SEvent event)
	{
		if (camera)
			return camera->OnEvent(event);

		return false;
	}
};


/*
Ok, lets start. Again, we use the main() method as start, not the
WinMain(), because its shorter to write.
*/
void main()
{
	/*
	Like in the HelloWorld example, we create an IrrlichtDevice with 
	createDevice(). The difference now is that we use the DirectX8 driver
	because the Software device would be too slow to draw a huge Quake 3 map.
	Also, we create an EventReceiver we declared above and set a pointer to
	it as last parameter of the createDevice() function. In this way,
	the Irrlicht Engine will send all events to this event Receiver.
	*/

	MyEventReceiver receiver;

	IrrlichtDevice *device =
		createDevice(video::DT_DIRECTX8, core::dimension2d<s32>(640, 480), 16, false, &receiver);

	/*
	Get a pointer to the video driver and the SceneManager so that
	we do not always have to write device->getVideoDriver() and
	device->getSceneManager().
	*/
	video::IVideoDriver* driver = device->getVideoDriver();
	scene::ISceneManager* smgr = device->getSceneManager();

	/*
	To display the Quake 3 map, we first need to load it. Quake 3 maps
	are packed into .pk3 files wich are nothing other than .zip files.
	So we add the .pk3 file to our FileSystem. After it was added,
	we are able to read from the files in that archive as they would
	directly be stored on disk.
	*/
	device->getFileSystem()->addZipFileArchive("..\\..\\media\\map-20kdm2.pk3");

	/* 
	Now we can load the mesh by calling getMesh(). We get a pointer returned
	to a IAnimatedMesh. As you know, Quake 3 maps are not really animated,
	they are only a huge chunk of static geometry with some materials
	attached. Hence the IAnimated mesh consists of only one frame,
	so we get the "first frame" of the "animation", which is our quake level
	and create an OctTree scene node with it, using addOctTreeSceneNode().
	The OctTree optimizes the scene a little bit, trying to draw only geometry
	which is currently visible. An alternative to the OctTree would be a 
	AnimatedMeshSceneNode, which would draw always the complete geometry of 
	the mesh, without optimization. Try it out: Write addAnimatedMeshSceneNode
	instead of addOctTreeSceneNode and compare the primitives drawed by the
	video driver. (There is a getPrimitiveCountDrawed() method in the 
	IVideoDriver class). Note that this optimization with the Octree is only
	useful when drawing huge meshes consiting of lots of geometry.
	*/
	scene::IAnimatedMesh* mesh = smgr->getMesh("20kdm2.bsp");
	scene::ISceneNode* node = 0;
	
	if (mesh)
		node = smgr->addOctTreeSceneNode(mesh->getMesh(0));

	/*
	Because the level was modelled not around the origin (0,0,0), we translate
	the whole level a little bit.
	*/
	if (node)
		node->setRelativePosition(core::vector3df(-1300,-144,-1249));

	/*
	Now we only need a Camera to look at the Quake 3 map.
	And we want to create a user controlled camera. There is a camera
	available, which can be controlled compareable to the camera in Maya:
	Rotate with left mouse button pressed, Zoom with both buttons pressed,
	translate with right mouse button pressed.
	*/
	camera = smgr->addCameraSceneNodeMaya();

	/*
	We have done everything, so lets draw it. We also write the current
	frames per second and the drawn primitives to the caption of the
	window.
	*/
	int lastFPS = -1;

	while(device->run())
	{
		driver->beginScene(true, true, video::Color(0,100,100,100));
		smgr->drawAll();
		driver->endScene();

		int fps = driver->getFPS();

		if (lastFPS != fps)
		{
			wchar_t tmp[255];
			swprintf(tmp, L"Quake 3 Map Example - Irrlicht Engine (fps:%d) Triangles:%d", 
				fps, driver->getPrimitiveCountDrawed());

			device->setWindowCaption(tmp);
			lastFPS = fps;
		}
	}

	/*
	In the end, delete the Irrlicht device.
	*/
	device->drop();
}

