#!/bin/bash
gcc Main.c -pthread -lm -o AES5 
./AES5 >> data
rm AES5
