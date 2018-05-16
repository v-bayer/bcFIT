#!/bin/sh

JAR=PigMapReduceBaseball-1.0-SNAPSHOT-jar-with-dependencies.jar
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
	hadoop fs -mkdir -p /user/hue/PigMapReduceBaseball;\
	hadoop fs -rm -f -R /user/hue/PigMapReduceBaseball/output;\
	hadoop jar ${JAR} hdfs:///user/hue/Fielding/adminDB/Fielding.txt /user/hue/PigMapReduceBaseball/output;\
	mapred job -list all;\
	hadoop fs -ls /user/hue/PigMapReduceBaseball/output;\
	echo Done.\
"
