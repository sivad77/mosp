#CFLAGS=    -DSMALL -Wall -O3 -march=pentium4 -fomit-frame-pointer
#CFLAGS=  -DNDEBUG  -DSMALL -Wall -O3 -march=pentium4 -fomit-frame-pointer
#CFLAGS= -Wall -g -DSMALL
CFLAGS= -Wall -O2 -DSMALL
#CFLAGS= -Wall -g -pg -DSMALL
CC=gcc
DIRS = list set matrix hash stats
OTHEROBJS = list/aList.o set/bitSet.o matrix/matrix.o hash/hash.o stats/stats.o
EXEC = mosp

export CC
export CFLAGS

SRCS := $(wildcard *.c)
OBJS := $(patsubst %.c,%.o,$(wildcard *.c))

all: $(EXEC)

subdirs: 
	@for dir in $(DIRS);  do $(MAKE) -C $$dir; done

$(EXEC): subdirs $(OBJS)
	$(CC) $(CFLAGS) -o $(EXEC) $(OBJS) $(OTHEROBJS)

clean:
	rm -f gmon.out $(OBJS) $(OTHEROBJS) $(EXEC)
