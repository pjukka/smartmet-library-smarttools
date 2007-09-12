LIB = smarttools

MAINFLAGS = -Wall -W -Wno-unused-parameter

EXTRAFLAGS = -pedantic -Wpointer-arith -Wcast-qual \
	-Wcast-align -Wwrite-strings -Wconversion -Winline \
	-Wnon-virtual-dtor -Wno-pmf-conversions \
	-Wsign-promo -Wchar-subscripts -Wold-style-cast

DIFFICULTFLAGS = -Weffc++ -Wredundant-decls -Wshadow -Woverloaded-virtual -Wunreachable-code

CC = g++
CFLAGS0 = -DUNIX -O0 -g
CFLAGS = -DUNIX -O0 -g $(MAINFLAGS) $(EXTRAFLAGS) -Werror
CFLAGS_RELEASE = -DUNIX -O2 -DNDEBUG $(MAINFLAGS)
LDFLAGS = -s
ARFLAGS = -r
INCLUDES = -I $(includedir)/smartmet/newbase
LIBS = -L $(libdir) -lsmartmet-newbase

# Common library compiling template

# Installation directories

prosessor := $(shell uname -p)

ifeq ($(origin PREFIX), undefined)
  PREFIX = /usr
else
  PREFIX = $(PREFIX)
endif

ifeq ($(prosessor), x86_64)
  libdir = $(PREFIX)/lib64
else
  libdir = $(PREFIX)/lib
endif

bindir = $(PREFIX)/bin
includedir = $(PREFIX)/include
objdir = obj

# rpm variables
rpmsourcedir = /smartmet/src/redhat/SOURCES
rpmerr = "There's no spec file ($(LIB).spec). RPM wasn't created. Please make a spec file or copy and rename it into $(LIB).spec"

# What to install

LIBFILE = libsmartmet_$(LIB).a

# How to install

INSTALL_PROG = install -m 775
INSTALL_DATA = install -m 664

# CFLAGS

ifneq (,$(findstring release,$(MAKECMDGOALS)))
  CFLAGS = $(CFLAGS_RELEASE)
endif

# Compilation directories

vpath %.cpp source
vpath %.h include
vpath %.o $(objdir)

# The files to be compiled

SRCS = $(patsubst source/%,%,$(wildcard source/*.cpp))
HDRS = $(patsubst include/%,%,$(wildcard include/*.h))
OBJS = $(SRCS:%.cpp=%.o)

OBJFILES = $(OBJS:%.o=obj/%.o)

INCLUDES := -I include $(INCLUDES)

.PHONY: test rpm

# The rules

all: objdir $(LIBFILE)
debug: all
release: all

$(LIBFILE): $(OBJS)
	$(AR) $(ARFLAGS) $(LIBFILE) $(OBJFILES)

clean:
	rm -f $(LIBFILE) $(OBJFILES) *~ source/*~ include/*~

install:
	@mkdir -p $(includedir)/$(LIB)
	@list='$(HDRS)'; \
	for hdr in $$list; do \
	  if [ include/$$hdr -nt $(includedir)/$(LIB)/$$hdr ]; \
	  then \
	    echo $(INSTALL_DATA) include/$$hdr $(includedir)/$(LIB)/$$hdr; \
	  fi; \
	  $(INSTALL_DATA) include/$$hdr $(includedir)/$(LIB)/$$hdr; \
	done
	@mkdir -p $(libdir)
	$(INSTALL_DATA) $(LIBFILE) $(libdir)/$(LIBFILE)

depend:
	makedepend $(INCLUDES)

test:
	cd test && make test

html::
	mkdir -p /data/local/html/lib/$(LIB)
	doxygen $(LIB).dox

objdir:
	@mkdir -p $(objdir)

rpm: clean depend
	if [ -e $(LIB).spec ]; \
	then \
	  tar -C ../ -cf $(rpmsourcedir)/libsmartmet-$(LIB).tar $(LIB) ; \
	  gzip -f $(rpmsourcedir)/libsmartmet-$(LIB).tar ; \
	  rpmbuild -ta $(rpmsourcedir)/libsmartmet-$(LIB).tar.gz ; \
	else \
	  echo $(rpmerr); \
	fi;

headertest:
	@echo "Checking self-sufficiency of each header:"
	@echo
	@for hdr in $(HDRS); do \
	echo $$hdr; \
	echo "#include \"$$hdr\"" > /tmp/$(LIB).cpp; \
	echo "int main() { return 0; }" >> /tmp/$(LIB).cpp; \
	$(CC) $(CFLAGS) $(INCLUDES) -o /dev/null /tmp/$(LIB).cpp $(LIBS); \
	done

.SUFFIXES: $(SUFFIXES) .cpp

.cpp.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c -o $(objdir)/$@ $<

# stl_algo warning, nothing to do
NFmiDrawParamList.o: NFmiDrawParamList.cpp
	$(CC) $(CFLAGS) -Wno-error $(INCLUDES) -c -o $(objdir)/$@ $<

# overloading warning
NFmiSmartInfo.o: NFmiSmartInfo.cpp
	$(CC) $(CFLAGS0) -Wno-error $(INCLUDES) -c -o $(objdir)/$@ $<

# lots of isspace warnings
NFmiSmartToolIntepreter.o: NFmiSmartToolIntepreter.cpp
	$(CC) $(CFLAGS0) $(INCLUDES) -c -o $(objdir)/$@ $<

# -include Dependencies
# DO NOT DELETE THIS LINE -- make depend depends on it.
