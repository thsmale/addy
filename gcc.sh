
# First one is ideal because easiest
gcc client.c 
gcc client.c util.c
gcc client.c util.c addy.c 
gcc client.c addy.c
gcc addy.c

gcc addy.h
gcc client.c addy.h
gcc client.c addy.c addy.h
gcc addy.h client.c
gcc client.c add.h

# Answer
main.c, addy.h addy.c
gcc main.c addy.c
