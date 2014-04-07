#!/usr/bin/python

import sys,os.path

input_file = None
new_file = None

def copyFile():
	for line in input_file:
		new_file.write(line)

path = "/home/alexio/software/Galaxy_Project/galaxy-dist/tools/my_tools"
myfile = open(path+"/test.txt",a)
myfile.write("testing")
myfile.close()

file_num = 1
for var in sys.argv[1:]:
	input_file = open(var)
	new_fname = path+"/data/file"+str(file_num) + ".fa"
	new_file = open(new_fname, "w")
	copyfile()
    new_file.close()
	input_file.close()

#create faster2 index
subprocess.call([path+'/faster2/faster2', path+'/data','index','nt'])
