import os

# initalize common variables
full_path = os.path.realpath("__file__")
rootDir = os.path.dirname(full_path)
Export('rootDir')

# parse config file
configDict = {}
with open('config.ini', 'r') as configFile:
        for line in configFile:
            splitLine = line.split('=')
            if len(splitLine) != 2 or splitLine[0].startswith('#'):
			    continue
            configDict[splitLine[0]] = splitLine[1].rstrip()

# display config variables
for key in configDict.iterkeys():
	print "configval %s = %s" % (key, configDict[key])

# check selected compiler	
buildname = configDict['buildname']
Export('buildname')

if buildname == 'win64gl':    
    SConscript('buildscripts/SConscript_win64gl')
elif buildname == 'win64dx':
	SConscript('buildscripts/SConscript_win64dx')
elif buildname == 'linux64gl':
	SConscript('buildscripts/SConscript_linux64gl')
else:
    print "Warning: No valid Build selected! check config.ini"

Import('env')

print("Environment: " + env['PLATFORM']);
print("TARGET_ARCH: " + env['TARGET_ARCH']);

# if cleaning, make sure its cleaning everything that an IDE could generate
if env.GetOption('clean'):
	#call sconscripts to make sure the cleaner knows about the output files these create, and can clean them
	#SConscript('SConscript_msvc')
	#SConscript('SConscript_g++')
	Execute(Delete('buildscripts/agnostik_' + buildname + '.sdf', must_exist=0))
	Execute(Delete('buildscripts/agnostik_' + buildname + '.v12.suo', must_exist=0))
	Execute(Delete('buildscripts/agnostik_' + buildname + '.opensdf', must_exist=0))
	Execute(Delete('buildscripts/agnostik_' + buildname + '.vcxproj.user', must_exist=0))
	Execute(Delete('buildscripts/release/', must_exist=0))
	Execute(Delete('buildscripts/debug/', must_exist=0))
	Execute(Delete('buildscripts/obj/', must_exist=0))
	Execute(Delete('genfiles/', must_exist=0))
