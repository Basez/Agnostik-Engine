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

# Return all files in dir, and all its subdirectories, ending in pattern
def listAllFilesWithExtention(dir, pattern):
   for dirname, subdirs, files in os.walk(dir):
      for f in files:
         if f.endswith(pattern):
            yield os.path.join(dirname, f)
			
# if cleaning, make sure its cleaning everything that an IDE could generate
if env.GetOption('clean'):
	#call sconscripts to make sure the cleaner knows about the output files these create, and can clean them
	# Remove all files in the current dir matching *.config
	for f in listAllFilesWithExtention('.', '.sln'): print("files with extension: " + f);
	for f in listAllFilesWithExtention('.', '.sln'): Execute(Delete(f, must_exist=0))
	for f in listAllFilesWithExtention('.', '.sdf'): Execute(Delete(f, must_exist=0))
	for f in listAllFilesWithExtention('.', '.suo'): Execute(Delete(f, must_exist=0))
	for f in listAllFilesWithExtention('.', '.opensdf'): Execute(Delete(f, must_exist=0))
	for f in listAllFilesWithExtention('.', '.user'): Execute(Delete(f, must_exist=0))
	for f in listAllFilesWithExtention('.', '.ilk'): Execute(Delete(f, must_exist=0))
	for f in listAllFilesWithExtention('.', '.pdb'): Execute(Delete(f, must_exist=0))
	for f in listAllFilesWithExtention('.', '.vcxproj'): Execute(Delete(f, must_exist=0))
	for f in listAllFilesWithExtention('.', '.filters'): Execute(Delete(f, must_exist=0))
	for f in listAllFilesWithExtention('.', '.exe'): Execute(Delete(f, must_exist=0))	
#	for f in listAllFilesWithExtention('.', '.exe'): Execute(os.remove(f))
	# delete shaders and other folders
	Execute(Delete('shaders/output_dx11', must_exist=0))
	Execute(Delete('shaders/output_gl/', must_exist=0))
	Execute(Delete('genfiles/', must_exist=0))
	
	
	




	
	
