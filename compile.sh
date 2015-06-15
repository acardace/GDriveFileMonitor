#!/bin/bash
gcc -Wall -ggdb *.c ./jsmn/libjsmn.a  -lcurl -o autoUploader
