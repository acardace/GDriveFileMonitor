#!/bin/bash
gcc -Wall -O2 *.c ./jsmn/libjsmn.a  -lcurl -o autoUploader
