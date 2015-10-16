#!/bin/bash
list=`ls *.sh`
for script in $list
do
./$script
done
