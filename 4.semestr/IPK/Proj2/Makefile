#!/bin/bash

THISDIR="./"
REFDIR="./www"

SERVER=ipkhttpserver.py
CLIENT=ipkhttpclient.py

SCRIPT=${SERVER} ${CLIENT}

clean:
	#find ${THISDIR} -type f -name '^ipkHttpServer-' -delete
	rm -f *.log
	rm -f *.payload
	rm -f *.header

pack:
	tar cvzf xbayer05.tgz ${SCRIPT} Makefile readme.txt
