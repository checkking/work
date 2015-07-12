#!/bin/sh

if [ $# -ne 1 ]; then
	echo "Usage: $0 <# of nodes>"
	exit 1
fi

NUM_NODES=$1

if [ "${HADOOP_HOME}" == "" ]; then
        echo "[ERROR] environment variable HADOOP_HOME is NOT set!"
        exit 2
fi

HADOOP_FS=hdfs://nmg01-mulan-hdfs.dmop.baidu.com:54310
HADOOP_UGI=mpi,hpcinit
HADOOP_OUT=/app/inf/mpi/chenkang02
HADOOP_DATA_DIR=
HDFS_FILES=${HADOOP_DATA_DIR}/file1.txt,${HADOOP_DATA_DIR}/file2.txt
HDFS_ARCHIVES=${HADOOP_DATA_DIR}/archive1.tar,${HADOOP_DATA_DIR}/archive2.tar.gz,${HADOOP_DATA_DIR}/archive3.tgz
HPC_HOME=/home/chenkang/soft/hpc_client

HADOOP_FS_CMD="${HADOOP_HOME}/bin/hadoop fs -Dfs.default.name=${HADOOP_FS} -Dhadoop.job.ugi=${HADOOP_UGI}"
${HADOOP_FS_CMD} -rmr ${HADOOP_OUT}

CURR_DIR=`dirname $0`
rm -rf ${CURR_DIR}/$(basename ${HADOOP_OUT})

sh ${HPC_HOME}/bin/qsub_f --hdfs ${HADOOP_FS} --ugi ${HADOOP_UGI} --pushcore no --core-size-limit 8999 --hpcugi ${HADOOP_UGI} --hout ${HADOOP_OUT} --files ${CURR_DIR}/vectorcompute.o -N chenkang02_practise -l nodes=${NUM_NODES},walltime=72000 ${CURR_DIR}/run_mpi.sh 

