#!/bin/bash
ps -e| grep rf_bt
# if not found - equals to 1, start it
if [ $? -eq 1 ]
then
sudo ./rf_bt
else
echo "eq 0 - rf running - do nothing"
fi
