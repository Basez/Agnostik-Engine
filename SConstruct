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
            if len(splitLine) != 2 or splitLine[0].startswith('#') or splitLine[0].startswith(';'):
			    continue
            key = splitLine[0]
            value = splitLine[1][splitLine[1].index('[')+1:splitLine[1].index(']')] # store as variable, the content is stripped of its first and last character (the [ and ])
            configDict[key] = value

# display config variables
for key in configDict.iterkeys():
	print "configval %s = %s" % (key, configDict[key])

# check selected compiler	
buildname = configDict['buildname']
Export('buildname')

if buildname == 'win64gl':    
    SConscript('project_files/SConscript_win64gl')
elif buildname == 'linux64gl':
	SConscript('project_files/SConscript_linux64gl')
elif buildname == 'win64dx11':
	SConscript('project_files/SConscript_win64dx11')
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
	Execute(Delete('project_files/agnostik_' + buildname + '.sdf', must_exist=0))
	Execute(Delete('project_files/agnostik_' + buildname + '.v12.suo', must_exist=0))
	Execute(Delete('project_files/agnostik_' + buildname + '.opensdf', must_exist=0))
	Execute(Delete('project_files/agnostik_' + buildname + '.vcxproj.user', must_exist=0))
	Execute(Delete('project_files/.vs/', must_exist=0))
	Execute(Delete('project_files/release/', must_exist=0))
	Execute(Delete('project_files/debug/', must_exist=0))
	Execute(Delete('project_files/obj/', must_exist=0))
	Execute(Delete('genfiles/', must_exist=0))
	# delete shaders
	Execute(Delete('shaders/output_dx11', must_exist=0))
	Execute(Delete('shaders/output_gl/', must_exist=0))
	# delete visual studio output files
	Execute(Delete('build/' + buildname + '/agnostik_debug.ilk', must_exist=0))
	Execute(Delete('build/' + buildname + '/agnostik_debug.pdb', must_exist=0))
	Execute(Delete('build/' + buildname + '/agnostik_release.ilk', must_exist=0))
	Execute(Delete('build/' + buildname + '/agnostik_release.pdb', must_exist=0))
	

