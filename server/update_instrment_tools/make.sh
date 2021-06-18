#########################################################################
# File Name: make.sh
# Created Time: Fri 02 Dec 2016 10:59:58 AM CST
# Author: 
# description: 
#########################################################################
#!/bin/bash

type=$1

usage ()
{
	echo "ctp           compile the original version"
	echo "rohon         compile support rohon"
	echo "sungard17     compile support sungard17"
	echo "sungard18     compile support sungard18"
	echo "all           compile all version"
	echo ""
}

case  $type in
	rohon)
		make clean  -f Makefile.rohon && make debug -f Makefile.rohon
		;;
	ctp)
		make clean  -f Makefile && make debug -f Makefile
		;;
	sungard17)
		make clean  -f Makefile.sungard17 && make debug -f Makefile.sungard17
		;;
	sungard18)
		make clean  -f Makefile.sungard18 && make debug -f Makefile.sungard18
		;;
	all)
		make clean  -f Makefile.rohon && make debug -f Makefile.rohon
		make clean  -f Makefile && make debug -f Makefile
		make clean  -f Makefile.sungard17 && make debug -f Makefile.sungard17
		make clean  -f Makefile.sungard18 && make debug -f Makefile.sungard18
		;;
	*)
		usage;
		;;
esac




