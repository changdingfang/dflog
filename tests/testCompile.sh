#!/bin/bash
# =======================================================================
#  FileName:     testCompile.sh
#  Author:       dingfang
#  CreateDate:   2021-01-10 17:58:33
#  ModifyAuthor: dingfang
#  ModifyDate:   2021-01-10 19:05:40
# =======================================================================

SCRIPT=`pwd`/${0}
ROOTDIR=`dirname ${SCRIPT}`

BINDIR=${ROOTDIR}/../build/bin

build()
{
    cd ${ROOTDIR}/..

    if [[ -d ${build} ]]; then
        rm -r build
    fi

    mkdir build

    cd build

    cmake -DDFLOG_TEST=on ..
    make
}


logLoopTest()
{
    rm -f dflog_loop/*
    ./touch.sh ${1} ${2} "dflog_loop/dflog_loop"
    filenums=`ls dflog_loop/* | wc -l`
    if [[ ${filenums} -lt ${1} ]]; then
        echo "generate log file failed!"
        return
    fi
    
    ./${line} ${3}

    filenums=`ls dflog_loop/* | wc -l`
    if [[ ${filenums} -ne ${3} ]]; then
        echo "log loop test failed!"
        return
    fi
}

checkLogLoop()
{
    if [[ ! -d dflog_loop ]]; then
        mkdir dflog_loop
    fi

    logLoopTest 10 10 5
    logLoopTest 30 1 20
    logLoopTest 30 1 3
    logLoopTest 30 3 3
}


checkNormalSink()
{
    rm test1* test2* test3* log -r
}


runTest()
{
    cd ${BINDIR}
    cp ${ROOTDIR}/touch.sh ${BINDIR}

    files=`ls`
    while read line
    do
        if [[ "${line:0:4}" != "test" ||
            -n "$(echo ${line:4:1} | sed -n "/^[0-9]\+$/p")"
            ]]; then
            continue
        fi

        echo "---- ${line}"
        if [[ "${line}" == "testLogLoop" ]]; then
            checkLogLoop "${line}"
            continue
        fi

        if [[ "${line}" == "testNormalSink" ]]; then
            checkNormalSink "${line}"
            # continue
        fi

        ./${line} > result_${line} 2>&1
    done <<< "${files}"
}

# build
runTest

