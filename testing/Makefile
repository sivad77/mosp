CFLAGS=-pg -g
CC=gcc

all: bitSet hashTbl #aList

#aList: aListTest.c ../list/aList.c
#	$(CC) $(CFLAGS) -o aListTest aListTest.c ../list/aList.c

bitSet: bitSetTest.c ../set/bitSet.c
	$(CC) $(CFLAGS) -DSMALL -o bitSetTest bitSetTest.c ../set/bitSet.c

hashTbl: hashTblTest.c ../hash/hash.c
	$(CC) $(CFLAGS) -o hashTblTest hashTblTest.c ../hash/hash.c

clean:
	rm -f aListTest bitSetTest
