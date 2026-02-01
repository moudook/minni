#!/bin/bash
# Deploy model to connected device
echo "Deploying model to device..."

DEVICE_ID=$1
MODEL_PATH=$2

# adb -s $DEVICE_ID push $MODEL_PATH /sdcard/Android/data/com.minni.framework/files/models/
