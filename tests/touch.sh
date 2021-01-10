#!/bin/bash
# =======================================================================
#  FileName:     touch.sh
#  Author:       dingfang
#  CreateDate:   2021-01-09 22:01:17
#  ModifyAuthor: dingfang
#  ModifyDate:   2021-01-10 19:03:38
# =======================================================================

# 生成的日志文件名为
# 1. "example" + "_" + "时间" + ".log"
# 2. "example" + "_" + "时间" + ".log." + "序号"
FILENAME="example"
# 生成日志文件的天数
DAYS=10
# 每天生成日志最大数量(随机数的最大值)
MAX_FILES=10

if [[ ${#} -eq 3 ]]; then
    DAYS=$1
    MAX_FILES=$2
    FILENAME=$3
fi


genLogFile()
{
    d=${1}
    touch ${FILENAME}_${d}.log
    num=$((${RANDOM} % ${MAX_FILES}))
    while [[ ${num} -ne 0 ]];
    do
        touch ${FILENAME}_${d}.log.${num}
        num=$((num - 1))
    done
}

testnum=${DAYS}
touch ${FILENAME}.log
while true
do
    days=`date -d "${testnum} day ago" +"%Y-%m-%d"`
    genLogFile ${days}
    if [ ${testnum} -eq 0 ]; then
        break
    fi
    testnum=$((testnum - 1))
done


