#!/bin/bash
#
# $Id$
# Purpose: Start ntripclient

# change these 3 according to your needs
Stream='CQ-F9P'
User='guest'
Password='guest'

DateStart=`date -u '+%s'`
SleepMin=10     # Wait min sec for next reconnect try
SleepMax=10000  # Wait max sec for next reconnect try
(while true; do
  ./ntripclient -s 160.16.134.72 -r 80 -d $Stream -u $User -p $Password -D /dev/ttyACM0 -B 115200
  if test $? -eq 0; then DateStart=`date -u '+%s'`; fi
  DateCurrent=`date -u '+%s'`
  SleepTime=`echo $DateStart $DateCurrent | awk '{printf("%d",($2-$1)*0.02)}'`
  if test $SleepTime -lt $SleepMin; then SleepTime=$SleepMin; fi
  if test $SleepTime -gt $SleepMax; then SleepTime=$SleepMax; fi
  # Sleep 2 percent of outage time before next reconnect try
  sleep $SleepTime
done) 

