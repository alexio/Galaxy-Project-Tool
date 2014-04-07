#!/usr/bin/python

import subprocess, sys, os.path

#print 'Args',str(sys.argv)
#ls_output = subprocess.check_output(['ls'])
#print ls_output

#subprocess.call(['mv','/home/alexio/software/Galaxy_Project/galaxy-dist/tools/my_tools/testerFolder', '/home/alexio/software/Galaxy_Project/galaxy-dist/tools/my_tools/testing'])

input_file = None
new_file = None

def copyFile():
	for line in input_file:
		print line
		new_file.write(line)


path ="/home/alexio/software/Galaxy_Project/galaxy-dist/tools/my_tools"
myfile =  open(path + "/test.txt", "a")
myfile.write("testing")
myfile.close()

file_num = 1
for var in sys.argv[1:]:
	print var
	input_file = open(var)
	new_fname = path+"/data/file"+str(file_num)+".fa"
	while os.path.isfile(new_fname):
		file_num+=1
		new_fname = path + "/data/file" + str(file_num) + ".fa"
	new_file = open(new_fname, "w")
	copyFile()
	new_file.close()
	input_file.close()

#call faster2 now
subprocess.call([path+'/faster2/faster2', path+"/data/", "index", "nt"])
