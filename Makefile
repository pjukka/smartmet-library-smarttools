LIB = smarttools

MAINFLAGS = -Wall -W -Wno-unused-parameter

EXTRAFLAGS = -pedantic -Wpointer-arith -Wcast-qual \
	-Wcast-align -Wwrite-strings -Wconversion -Winline \
	-Wctor-dtor-privacy -Wnon-virtual-dtor -Wno-pmf-conversions \
	-Wsign-promo -Wchar-subscripts -Wold-style-cast -Wunreachable-code

DIFFICULTFLAGS = -Weffc++ -Wredundant-decls -Wshadow -Woverloaded-virtual

CC = g++
CFLAGS0 = -DUNIX -O0 -g
CFLAGS = -DUNIX -O0 -g $(MAINFLAGS) $(EXTRAFLAGS) -Werror
CFLAGS_RELEASE = -DUNIX -O2 -DNDEBUG $(MAINFLAGS)
LDFLAGS = -s
ARFLAGS = -r
INCLUDES = -I $(includedir)/newbase
LIBS = -L ../../../../lib -lnewbase

# Common library compiling template

include ../../makefiles/makefile.lib

# stl_algo warning, nothing to do
NFmiDrawParamList.o: NFmiDrawParamList.cpp
	$(CC) $(CFLAGS) -Wno-error $(INCLUDES) -c -o $(objdir)/$@ $<

# overloading warning
NFmiSmartInfo.o: NFmiSmartInfo.cpp
	$(CC) $(CFLAGS0) -Wno-error $(INCLUDES) -c -o $(objdir)/$@ $<

# lots of isspace warnings
NFmiSmartToolIntepreter.o: NFmiSmartToolIntepreter.cpp
	$(CC) $(CFLAGS0) $(INCLUDES) -c -o $(objdir)/$@ $<
