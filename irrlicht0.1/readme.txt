==========================================================================
The Irrlicht Engine SDK version 0.1
==========================================================================

  Welcome the Irrlicht Engine SDK.

  Content of this file:

  1. Where to find what
  2. How to start
  3. Release Notes
  4. License
  5. Contact



==========================================================================
1. Where to find what
==========================================================================

  You will find some directories after decompressing the archive in which
  came the SDK. These are:
  
  \bin       The compiled library Irrlicht.DLL and some compiled demo 
             and example applications, just start them to see the 
             Irrlicht Engine in action.
  \doc       Documentation of the Irrlicht Engine.
  \examples  Examples and tutorials showing how to use the engine. 
  \include   Header files to include when using the engine.
  \lib       Lib to link with your programs when using the engine.
  \media     Graphics and sound data for the demo applications and
             examples.
  \source    The source code of the Irrlicht Engine. This code is
             not needed to develop applications with the engine,
             but it is included to let you be able to recompile and
             debug it, if necessary.             



==========================================================================
2. How to start
==========================================================================

  To see the engine in action, just go to the \bin directory, and start
  some applications. There should also be an application named 
  TechDemo.exe, this should show the most interesting things.
  
  To start developing own applications and games with the engine take 
  a look at the 1.HelloWorld example in the \examples directory.
  There will also be a .html file with an easy to understand tutorial.
  


==========================================================================
3. Release Notes
==========================================================================

  This is an alpha version of the SDK. So please note that there are
  features missing in the engine and maybe also some bugs. The following
  list is a complete list of parts of this alpha SDK which do not work
  100% correct. All other parts which can be found in the documentation
  and accessed with the Irrlicht header files work and can already be 
  used. 
  
  OpenGL Device
  The OpenGL Device is currently only able to load textures and draw basic 
  3D primitives. No 2D functions, complex materials and dynamic light are
  implemented. Please use the DirectX8 or the Software Device instead. 
  In the next release (0.2) the OpenGL device will be complete.
  
  Software Device
  Dynamic lighting, multitexturing, 3d clipping and bilinear filtering are
  not implemented because the Software Device was intented to to only 2d 
  functions, the primitive 3d functions are only an addition. Until the
  first non beta release (1.0) of the Engine, some more 3d functionality
  will be added.
  
  BspTree
  The binary space partition scene node does not work correctly and has 
  some bugs in it, which might lead to crashes. Please use the OctTree
  scene node instead. There is nearly no difference between the interface
  of them.

  MeshViewerGUIElement
  The camera control of this element is currently disabled.
  
  FileOpenDialog
  Not all functions are implemented.
  
  Render to Texture support
  Is not implemented yet.
  
  MS3D Skeletal Animation
  A loaded mesh is currently not animated and only drawn static.
  
  Quake 3 maps
  No support for curved surfaces yet.
  
  Linux Support
  The engine currently only runs with Windows platforms. A linux version
  will be available somewhere between the versions 0.2 and 0.5, it depends
  on the need of the users.

==========================================================================
4. License
==========================================================================

  The license of the Irrlicht Engine is based on the zlib/libpng license.
  Even though this license does not require you to mention that you are
  using the Irrlicht Engine in your product, an acknowledgement somewhere
  would be highly appreciated.


  The Irrlicht Engine License
  ===========================

  Copyright (C) 2002-2003 Nikolaus Gebhardt

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
  

  
==========================================================================
5. Contact
==========================================================================

  If you have problems, questions or suggestions, please visit the 
  official homepage of the Irrlicht Engine:
  
  http://irrlicht.sourceforge.net
  
  You will find forums, bugtrackers, patches, tutorials, and other stuff
  which will help you out.
  
  If want to contact the author of the engine, please send an email to:
  
  niko@code3d.com


  
    