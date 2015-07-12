#!/bin/bash
export HADOOP_HOME=/home/chenkang/soft/hadoop-client/hadoop

IN_PATH=/app/inf/mpi/chenkang02/input/* 
OUT_PATH=/app/inf/mpi/chenkang02/result

## check output path if exist
${HADOOP_HOME}/bin/hadoop fs -conf ${HADOOP_HOME}/conf/hadoop-site.xml -test -e ${OUT_PATH}
if [$? -eq 0]; then 
   echo "${OUT_PATH}"" is exist!"
   exit 1
fi

${HADOOP_HOME}/bin/hadoop streaming -conf ${HADOOP_HOME}/conf/hadoop-site.xml \
    -D mapred.map.tasks=3 \
    -D mapred.reduce.tasks=1 \
    -D mapred.job.name="log join" \
    -D mapred.job.priority=LOW \
    -input ${IN_PATH} \
    -output ${OUT_PATH} \
    -mapper ./mapper.o \
    -reducer ./reducer.o \
    -file mapper.o \
    -file reducer.o

if [ $? -ne 0 ]; then
    ${HADOOP_HOME}/bin/hadoop fs -conf ${HADOOP_HOME}/conf/hadoop-site.xml -rmr ${OUT_PATH}
    exit 1
fi
