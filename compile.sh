#!/bin/bash
gcc -Wall -O2 -ggdb *.c ./jsmn/libjsmn.a  -lcurl -o autoUploader
