# About Agnostik-Engine
Agnostik is a platform agnostic stateless renderer which I made for my specialization project during the 3rd year of my study. It currently runs on Windows and Linux, using DirectX and OpenGL respectively. I am planning to port it to the PS4 and Vulkan in the future.

It is currently still in development but you can follow my progress here :)

# Third-party Licenses / References
- GLM: http://glm.g-truc.net/copying.txt
- SDL2: https://www.libsdl.org/license.php
- Assimp: http://assimp.sourceforge.net/main_license.html
- Glew: http://glew.sourceforge.net/credits.html
- stb: https://github.com/nothings/stb
- MMGR: http://www.paulnettle.com/

# Installation Guide Windows using Cygwin64

- Install Visual C++ Redistributable for Visual Studio 2015 (located in the dependencies folder)
- Download & install Cygwin64
- Download & install Python 2.7 (Please Note, make sure its version 2.7, and select the install option: just for me, otherwise scons will not work)  https://stackoverflow.com/questions/3008509/python-version-2-6-required-which-was-not-found-in-the-registry)
- Add "C:\Python27" to the PATH environment variable
- Python should now be working on cygwin64, test by opening cygwin and type  python -V (should display the version number)
- Download and extract scons 2.5.0
- Open Cygwin and navigate to the folder where you extracted scons. Scons is then installed by executing the "python setup.py install" command inside the Cygwin terminal.
- Create file called 'scons' (no file extension, just "scons") in C:/Python27 (python installation folder)
- Add 'exec scons.bat "$@"' to it to be able to run scons from cygwin. check this link: http://www.scons.org/wiki/SConsAndCygwin. Also make sure scons.bat is inside the Python27 folder!
- Now try the scons -v command, it should display the scons version if it was installed successfully. You can now use the scons command to build Agnostik!

# Installation Guide Linux

- Make sure G++ and Python 2.7 are installed
- Download and extract scons 2.4.1 
- Execute "sudo python setup.py install" in the extracted scons folder
- type scons -v to confirm the installation was successful
- Install these Dependencies (Through apt-get):
	- libglew-dev
	- libsdl2-dev
- Download/build/install assimp 3.2 manually (since the package is out of date)

# How to build

Open Cygwin (or an ordinary terminal on Linux). Navigate to the root directory of the project, its where the config.ini file is located.
Inside the config.ini file you can change certain properties of the build process and of the engine itself. They both share this file.
Type 'scons build=buildname' to generate a Visual Studio project (Windows Only) and build the engine, make sure you call this from the root directory.
To see a list of possible builds / features, type "scons -h"