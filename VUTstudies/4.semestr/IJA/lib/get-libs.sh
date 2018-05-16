#!/bin/bash

DEST_PATH='./lib'
WEBSITE_URL='http://www.stud.fit.vutbr.cz/~xbayer05/java_obr/Obr.zip'

wget -P ${DEST_PATH} ${WEBSITE_URL}
unzip ${DEST_PATH}/Obr.zip -d ${DEST_PATH}
rm -f ${DEST_PATH}/Obr.zip