#!/bin/sh

JAR=MapReduceCurrency-2-3-1.0-SNAPSHOT-jar-with-dependencies.jar
SSHPORT=2222
DIR=$(dirname ${0})

# build
mvn package

# deploy
echo 'Enter password "hadoop".'
scp -o UserKnownHostsFile=/dev/null -o StrictHostKeyChecking=no -P ${SSHPORT} \
	"${DIR}/target/${JAR}" hue@localhost:/tmp/

# run
echo 'Enter password "hadoop".'
ssh -o UserKnownHostsFile=/dev/null -o StrictHostKeyChecking=no -p ${SSHPORT} hue@localhost "\
	set -x;
	cd /tmp;\
	hadoop fs -mkdir -p /user/hue/MapReduceCurrency-2-3;\
	hadoop fs -rm -f -R /user/hue/MapReduceCurrency-2-3/output;\
	hadoop jar ${JAR} hdfs:///user/hue/Fielding/adminDB/eurofxref-sdmx.csv /user/hue/MapReduceCurrency-2-3/output;\
	mapred job -list all;\
	hadoop fs -ls /user/hue/MapReduceCurrency-2-3/output;\
	echo Done.\
"
