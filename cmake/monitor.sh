#!/bin/sh

#程序名
name="ipm-cli-control"
path="/home/qasky/set-ip"

cd $path

#清除日志
rm -f ipm-clo.log

echo "integrity checking..."
md5sum -c ipm-cli-control.md5 &>/dev/null

if [ $? -eq  0 ]
then
    while true
       do 
          pid=`ps -A | grep $name | awk '{print $1}'`
          now=`date "+ %Y-%m-%d %H:%M:%S"`
          if [ ! $pid ]
          then
             echo "$now $name is not running,statr is now ..." >> ipm-cli.log
             
             cd $path
             nohup ./$name >/dev/null 2>&1 &
           fi
           sleep 3
           done
 else
    echo "integrity checking failed" >> ipm-cli.log
 fi
