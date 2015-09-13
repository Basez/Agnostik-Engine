# Agnostik-Engine
Cross-platform Game Engine created as a specialization project by Bas van Zutphen.
It is currently still in development but you can follow my progress here :)

# Third-party Licenses / References
- GLM: http://glm.g-truc.net/copying.txt
- SDL2: https://www.libsdl.org/license.php
- Assimp: http://assimp.sourceforge.net/main_license.html
- Glew: http://glew.sourceforge.net/credits.html
- stb https://github.com/nothings/stb

# Dependencies
TODO:

# Installation Guide Linux
TODO:

# Installation Guide Windows

- download & run Cygwin64 package manager;
	- include all g++ (C++ only) compilers during the installation process; they are located under DEVIL
	- click continue and let it install the packages. 
- download & install Python 2.7 (Please Note, make sure its version 2.7, and select the install option: just for me, otherwise scons will not work)  https://stackoverflow.com/questions/3008509/python-version-2-6-required-which-was-not-found-in-the-registry)
- Add 'PATH=$PATH:/c/Python27'  to bottom of all .bash_profile files inside cygwin folder structure so cygwin can find python
- Add "C:\Python27" to the PATH environment variable
- Python should now be working on cygwin64, test by opening cygwin and type  python -V (should display the version number)
- Download and extract scons 2.3.6
- Install scons by navigating to the extracted folder inside cygwin, and calling the "python setup.py install" command
- Create file called 'scons' (no extensions, just "scons") in C:/Python27 (python installation folder)
- add 'exec scons.bat "$@"' to it to be able to run scons from cygwin. check this link: http://www.scons.org/wiki/SConsAndCygwin. Also make sure scons.bat is inside the Python27 folder!
- now try the scons -v command, it should display the scons version if it was installed successfully 

#######################################
Setup Agnostik build chain Linux:

TODO:

# How to build
After following the installation guide, navigate to the repository, where the config.ini file is located.
Inside the config.ini, you can change certain properties of the build process and of the application itself. 
Change the top property called 'buildname' to whichever build you wish to peform (choices are win64gl/linux64gl/win64dx)
After this is setup, simply navigate to the folder inside Cygwin64, and type "scons".
This will generate the debug & release build and if you are on windows (win64gl/win64dx) it will generate vx2013 project files.
If you have a different Visual Studio version, you can change those in the build scripts (located under /project_files)
If you want to clean all the build output files, use the command scons -c to cleanup everything.