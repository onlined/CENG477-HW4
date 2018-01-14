#!/bin/bash

GLFW3_PATH='/usr/lib/libglfw3.so'

if [ ! -f $GLFW3_PATH ]; then
    result=$(ldconfig -p | grep libglfw*\.so$)
    if [ $? ]; then
        path=$(awk -F'=> ' '{print $2}' <<< $result | head -n 1)
        sudo ln -s $path $GLFW3_PATH
    else
        echo "You don't seem to have glfw installed."
    fi
else
    echo "It seems that you don't need this fix."
fi
