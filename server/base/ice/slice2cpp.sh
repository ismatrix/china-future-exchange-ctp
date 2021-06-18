#########################################################################
# File Name: slice.sh
# Author: 
# mail:
# description: create the source file using ice
# Created Time: Wed 23 Mar 2016 07:49:55 PM CST
#########################################################################
#!/bin/bash


include="-I./ -I/usr/local/ice-3.6.2/slice"

for param in $@
do
	include+=" -I$param"
done

slice2cpp ${include} ./*.ice

