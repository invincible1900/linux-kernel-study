
PKG_CONFIG=/usr/bin/pkg-config
NL_LIBNAME=libnl-genl-3.0

NL_LIB_FLAGS=$(shell $(PKG_CONFIG) --cflags $(NL_LIBNAME))
NL_LIBS_L=$(shell $(PKG_CONFIG) --libs-only-L $(NL_LIBNAME))
NL_LIBS_l=$(shell $(PKG_CONFIG) --libs-only-l $(NL_LIBNAME))

CC=${CROSS_COMPILE}gcc

.PHONY:all
all: main.c 
	$(CC) -Wextra -Wall -Werror -Wno-unused-parameter main.c $(NL_LIB_FLAGS) $(NL_LIBS_L) $(NL_LIBS_l) -o gnl -g



