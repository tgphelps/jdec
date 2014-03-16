#!/bin/sh

OPTS=$*

for C in *.class ; do
	../jdec $OPTS $C
	if [ $? = 0 ] ; then
		echo OK $C
	else
		echo BAD $C
	fi
done
