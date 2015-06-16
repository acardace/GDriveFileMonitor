#-!/bin/bash
gcc -Wall -Wextra -ggdb *.c ./jsmn/libjsmn.a  -lcurl -o autoUploader
