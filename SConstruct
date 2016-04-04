import os

# (function) parse config file
def parseConfig():
	configDict = {}
	with open('config.ini', 'r') as configFile:
			for line in configFile:
				splitLine = line.split('=')
				if len(splitLine) != 2 or splitLine[0].startswith('#') or splitLine[0].startswith(';'):
					continue
				key = splitLine[0]
				# store as variable, the content is stripped of its first and last character (the [ and ])
				value = splitLine[1][splitLine[1].index('[')+1:splitLine[1].index(']')] 
				#check if value is already an array
				if key in configDict:
					configDict[key].append(value)
				else:
					configDict[key] = [value]
	return configDict

# (function) display config variables
def displayConfigVars():
	for key in configDict.iterkeys():
		print "configval %s = %s" % (key, configDict[key])

# (function) Return all files in dir, and all its subdirectories, ending in pattern
def listAllFilesWithExtention(dir, pattern):
   for dirname, subdirs, files in os.walk(dir):
      for f in files:
         if f.endswith(pattern):
            yield os.path.join(dirname, f)
						
# initalize common variables
full_path = os.path.realpath("__file__")
rootDir = os.path.dirname(full_path)
configDict = parseConfig()
platformName = Platform().name

# make these variables available in deeper sconscripts
Export('rootDir')
Export('configDict')
Export('platformName')

# Check if help was asked
if GetOption('help'):
	print "--"
	print "To build a specific Agnostik build you need to use the build commandline option"
	print "This will also generate visual studio files for the win64 projects on windows"
	print "For example; type 'scons buildname=win64dx11' to build and generate a win64dx11 project"
	print ""
	print "Current supported builds: "
	print "\t win64dx11 \t // Agnostik for 64 bit Windows, using DirectX11"
	print "\t win64gl \t // Agnostik for 64 bit Windows, using OpenGL"
	print "\t linux64gl \t // Agnostik for 64 bit Linux, using OpenGL"
	print "\t assets \t // TODO: Compiles and runs asset parser which converts all assets to a binary format"
	print ""
	print "Its also required to build the assets once, or everytime the assets change"
	print "To build the assets type: 'scons buildname=assets'"
	print ""
	print "Misc options:"
	print "\t -c \t\t // Cleans up all generated build files and assets"
	Exit(0)
	
# if -c or the clean option has been specified, we clean everything that an IDE / compiler could generate
if GetOption('clean'):
	# Remove all files in the current dir matching *.config
	for f in listAllFilesWithExtention('.', '.sln'): Execute(Delete(f, must_exist=0))
	for f in listAllFilesWithExtention('.', '.sdf'): Execute(Delete(f, must_exist=0))
	for f in listAllFilesWithExtention('.', '.suo'): Execute(Delete(f, must_exist=0))
	for f in listAllFilesWithExtention('.', '.opensdf'): Execute(Delete(f, must_exist=0))
	for f in listAllFilesWithExtention('.', '.user'): Execute(Delete(f, must_exist=0))
	for f in listAllFilesWithExtention('.', '.ilk'): Execute(Delete(f, must_exist=0))
	for f in listAllFilesWithExtention('.', '.pdb'): Execute(Delete(f, must_exist=0))
	for f in listAllFilesWithExtention('.', '.vcxproj'): Execute(Delete(f, must_exist=0))
	for f in listAllFilesWithExtention('.', '.filters'): Execute(Delete(f, must_exist=0))
	for f in listAllFilesWithExtention('build', '.exe'): Execute(Delete(f, must_exist=0))
	for f in listAllFilesWithExtention('.', '.log'): Execute(Delete(f, must_exist=0))
	# delete shaders and other folders
	Execute(Delete('shaders/output_dx11', must_exist=0))
	Execute(Delete('shaders/output_gl/', must_exist=0))
	Execute(Delete('genfiles/', must_exist=0))
	Execute(Delete('project_files/debug/', must_exist=0))
	Execute(Delete('project_files/obj/', must_exist=0))
	Execute(Delete('project_files/release/', must_exist=0))
	Execute(Delete('project_files/.vs/', must_exist=0))
	Execute(Delete('project_files/imgui.ini', must_exist=0))
	Execute(Delete(os.getcwd() + configDict['path_models_bin'][0], must_exist=0)) # Deletes generated modelsbin folder
	Exit(0)

buildName = ARGUMENTS.get('build', "")
print "Build selected: " + buildName

if buildName == 'win64dx11':	
	SConscript('project_files/SConscript_win64dx11')
elif buildName == 'win64gl':
	SConscript('project_files/SConscript_win64gl')
elif buildName == 'linux64gl':
	SConscript('project_files/SConscript_linux64gl')
elif buildName == 'assets':	
	print "platform: " + platformName 
	SConscript('project_files/SConscript_assets')
else:
	print (" *** ERROR *** No build or other command was specified, please type 'scons -h' for more information")
	Exit(0)
	
	
#Import('env')

#print("Environment: " + env['PLATFORM']);
#print("TARGET_ARCH: " + env['TARGET_ARCH']);


			

	
	




	
	
