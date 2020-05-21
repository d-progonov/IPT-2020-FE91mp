CC = g++
DEBUG = -g
CFLAGS = -Wall -c $(DEBUG)
LFLAGS = -Wall $(DEBUG)

BOOST_H = /usr/include/boost/
DBOOST_FILESYSTEM = -DBOOST_FILESYSTEM_VERSION=2
OBJS = jet.o clparser.o setup.o cimage.o cfeature.o cfeaturevector.o dct_m.o idct_m.o
BOOST_OBJ = -lboost_filesystem
LOCAL_LIB_L = /usr/local/lib

jet: $(OBJS)
	$(CC) $(LFLAGS) -o jet $(OBJS) $(BOOST_OBJ) -L$(LOCAL_LIB_L) -L$(BOOST_FILESYSTEM_L) -L$(BOOST_SYSTEM_L) -ljpeg

clparser.o: setup.h texception.h macros.h clparser.h clparser.cpp
	$(CC) $(CFLAGS) -I $(BOOST_H) $(DBOOST_FILESYSTEM) -L$(LOCAL_LIB_L) clparser.cpp

jet.o: clparser.h setup.h texception.h macros.h jet.cpp
	$(CC) $(CFLAGS) jet.cpp

setup.o: setup.h setup.cpp
	$(CC) $(CFLAGS) setup.cpp

cimage.o: dct_m.h idct_m.h cfeaturevector.h cimage.h jpeglib.h platform.h cimage.cpp
	$(CC) $(CFLAGS) -I $(BOOST_H) $(DBOOST_FILESYSTEM) -L$(LOCAL_LIB_L) cimage.cpp

cfeature.o: cfeature.h cfeature.cpp
	$(CC) $(CFLAGS) cfeature.cpp

cfeaturevector.o: cfeature.h cfeaturevector.h cfeaturevector.cpp
	$(CC) $(CFLAGS) cfeaturevector.cpp

dct_m.o: dct_m.h dct_m.c
	$(CC) $(CFLAGS) dct_m.c

idct_m.o: idct_m.h idct_m.c
	$(CC) $(CFLAGS) idct_m.c

clean:
	rm -f *.o *~ jet src/*.o src/*~
