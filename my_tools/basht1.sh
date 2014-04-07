#!/bin/bash
if [ ! -d /home/alexio/software/Galaxy_Project/galaxy-dist/tools/my_tools/data ]; then
	mkdir -p /home/alexio/software/Galaxy_Project/galaxy-dist/tools/my_tools/data;
fi;

mkdir -p /home/alexio/software/Galaxy_Project/galaxy-dist/tools/my_tools/testfolder;

./faster2/faster2 data/ index nt;

mkdir -p /home/alexio/software/Galaxy_Project/galaxy-dist/tools/my_tools/folder2;
