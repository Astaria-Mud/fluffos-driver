#####################################################
#       Standard Makefile for the MudOS driver      #
#####################################################
 
SHELL=/bin/sh
 
###########################
# setup and configuration #
###########################
 
# Note: this Makefile may need modified depending upon which type of machine
# you are using to compile the driver.  Be sure to read all of the comments
# in this file since they may pertain to your particular machine.
#
# If you get some weird compile-time error message from "make" about not being
# able to find some .h file, then try one of the following two things:
#
# 1) type "make depend"
# 2) deleting all of the lines below the comment that mentions "make depend"
#    (near the bottom of this file).
 
# change this if you wish the driver binary to be named something else
DRIVER_BIN = driver

# uncomment PROOF if using CenterLine's TestCenter to debug the driver.
#PROOF=proof
 
# RESOLV: define this to be -lresolv if your machine has it.
# HP and NeXT don't have it.
#RESOLV=-lresolv
 
# uncomment MALLOC* if you wish to use BSD malloc or Smalloc.
#   Both of these require the sbrk() system call.  Be sure to read the
#   comments in the bsdmalloc.c file before deciding on BSDMALLOC.
#   note: These mallocs may not work correctly on the NeXT or other systems
#   that don't fully support sbrk().  It is okay not to use either of these
#   two mallocs in which case you will get system malloc (read the comments
#   in options.h for choices on malloc statistics and debugging).
#
# uncomment this line for BSDmalloc
# (fast but high overhead)
#MALLOC=bsdmalloc.o
#
# uncomment this line for SMALLOC
# (slower than bsdmalloc, faster than most system mallocs, least overhead)
#MALLOC=smalloc.o
#
# Archimede's memory allocator works with all 32-bit systems, NeXT included.
# This is the default and optimal choice.
MALLOC=amalloc.o
 
# uncomment STRFUNCS if your machine is missing memcpy(), memset(), strtol(),
# and strcspn(). (needed by Sequent Dynix/3)
#STRFUNCS=strfuncs.o
 
# uncomment UALARM if your machine is missing the ualarm() system call.
# needed by HP, A/UX (Apple Mac), EP/IX (MIPS), Sequent Dynix/3, Ultrix
#UALARM=ualarm.o
 
# Command used to install executables in the INSTALL_DIR
# Use the -f version for HP/UX
#INSTALL = install -f
#INSTALL = install -c
# Use cp if all else fails
INSTALL = cp
 
# Set INSTALL_DIR to the directory where you want to install the executables.
INSTALL_DIR = ../bin

#Enable warnings from the compiler (gcc), if wanted.
WARN=-Wall
 
#Use this with gcc if you have lots of RAM, speeds up compile
PIPE=-pipe

# define profiling if you want it
# note: the gmon.out file will likely be written in the mudlib dir.
# PROFILE_ON controls whether or not monitoring is active at driver
# startup.  Comment PROFILE_ON to make profiling not active at startup.
# Use moncontrol(1) efun to enable profiling and moncontrol(0) to turn
# it off.
#PROFILE_ON=-DPROFILE_ON
# Uncomment this if you want to enable profiling of the driver (gcc)
#PROFIL=-pg -DPROFILING $(PROFILE_ON)
 
# Enable run time debugging
DEBUG=-g3 -O -m32 # -march=pentium3 # -DDEBUG # -fno-inline
# With extra driver debug code
#DEBUG=-g -DDEBUG
# Prevent -DDEBUG from aborting the driver (when in -DDEBUG mode)
#DEBUG_NON_FATAL=-DDEBUG_NON_FATAL
# Compile in debug() macro code
#DEBUG_MACRO=-DDEBUG_MACRO
 
# If you don't have strchr() and strrchr(), then uncomment the following line
#STR=-Dstrchr=index -Dstrrchr=rindex
 
# define this if you want (compiler) optimization enabled.
#
# *WARNING* using high levels of optimization (e.g. -O3) can cause some
# compilers to produce incorrect code.  If the driver is behaving
# inexplicably, try using a lower level of optimization (or none).
#
# Uncomment one or none of the following optimization lines.
#
# -O is usually a safe level of optimization for most compilers
#OPTIMIZE=-O
# high optimization for gcc:
#OPTIMIZE=-O6 -fomit-frame-pointer -fstrength-reduce -funroll-loops -fexpensive-optimizations
#OPTIMIZE=-O3 -fstrength-reduce -fomit-frame-pointer -funroll-loops -fexpensive-optimizations

#OPTIMIZE=-O2 -fstrength-reduce -march=i686 # -funroll-loops -fexpensive-optimizations

#OPTIMIZE=-O
# uncomment below for RS/6000(AIX) xlc compiler only.
# remove the -Q if xlc complains.
#OPTIMIZE=-O -Q
# might need this one with newer versions of AIX (ie 3.2.4)
#OPTIMIZE=-O -Q -qMAXMEM=16000
# high optimization for HP-UX 7.x/8.x's cc (don't use with 9.x)
#OPTIMIZE=+O3 +Obb3000
# MIPS R3000 running EP/IX Version 1.4.3. Compiler is RISCompiler C 2.11 (cc).
#OPTIMIZE=-Olimit 1802
# DEC Ultrix's cc's highest optimization:
#OPTIMIZE=-O2 -Olimit 5000
# high optimization for cc on SGI
#OPTIMIZE=-Olimit 2500

# Kilian's set:
#OPTIMIZE=-march=athlon64 -mtune=athlon64 -O2 -msse3
OPTIMIZE=-O2 -msse3

# use this for NeXTSTEP 3.1+ (at least)
#CPP=$(CC) -E -traditional-cpp
# the usual
CPP=$(CC) -E

# System V Release 4 (386/486 or if using Sun's cc under Solaris 2.x)
#OSFLAGS=-DSVR4

# MIPS R3000 running EP/IX Version 1.4.3. Compiler is RISCompiler C 2.11 (cc).
#OSFLAGS=-I/usr/include/bsd
 
# RS/6000 AIX: use this OSFLAGS line (required).
#OSFLAGS=-D_BSD -D_ALL_SOURCE
 
# Solaris (SunOS 5): for BSD compatible ioctl()
#OSFLAGS=-DBSD_COMPS

# try uncommenting this if you are using gcc and at runtime you see socket
# errors saying that the "set socket nonblocking" operation is not supported.
# That error is caused by old-style macros (that gcc doesn't normally grok)
# used by ioctl on some systems.
#NEED_OLD_CPP=-traditional-cpp

# May be needed by SunOS for LPC->C support
#STATIC=-static
 
CFLAGS= $(STATIC) $(OSFLAGS) $(OPTIMIZE) $(WARN) $(PROFIL) $(DEBUG) \
   $(DEBUG_MACRO) $(DEBUG_NON_FATAL) $(STR) $(PIPE) $(NEED_OLD_CPP)

# Flags to be used by the LPC->C compiler
# remove optimization or make other modifications here
CFLAGS2= $(STATIC) $(OSFLAGS) $(OPTIMIZE) $(WARN) $(DEBUG) \
   $(DEBUG_MACRO) $(DEBUG_NON_FATAL) $(STR) $(PIPE) $(NEED_OLD_CPP)
 
# set CC=cc on a NeXT and to gcc on other machines (if possible)
# You may want to use xlc on an RS/6000 (produces faster code).
#
# NOTE: If you are using gcc (especially on a sun4), and the driver gives
# a runtime error regarding "operation not supported on socket", then
# try using cc instead (some UNIX's use unsupported hacks in some of
# the system #include (.h) files which prevents some (socket) ioctl operations
# from working correctly when using a less crufty compiler than that provided
# by default on the system in question).
#
#CC=cc
CC=gcc
# CenterLine cc
#CC=clcc
#CC=xlc
 
# define this to be bison if you have it, and yacc otherwise.
YACC=bison
YFLAGS=-d -y
#YACC=obyacc
#YFLAGS = -d
 
# A/UX: may be required for Apple's UNIX (and possibly others).  Try
# uncommenting this if the make results in link errors seemingly related
# to yacc (e.g. undefined symbol "yyerror").
#EXTRALIBS=-ly
 
# HP-UX: use this EXTRALIBS line for HP-UX
#EXTRALIBS=-lBSD
 
# RS/6000 AIX: use this EXTRALIBS line 
#EXTRALIBS=-lbsd
 
# NetBSD 386: due to export restrictions...you may be missing or
#  have a crippled crypt package
EXTRALIBS=-lcrypt
 
# NeXT: link with MallocDebug if you have a NeXT with NeXTOS 2.1 or later and
# you wish to search for memory leaks (see /NextDeveloper/Apps/MallocDebug).
# Note: linking with MallocDebug will cause the virtual size of the
# driver process to reach appoximately 40MB however the amount of real memory
# used will remain close to normal.
#EXTRALIBS=-lMallocDebug -lsys_s
 
# Sequent DYNIX/ptx: use this EXTRALIBS line
#EXTRALIBS=-lsocket -linet -lnsl -lseq -lmalloc
 
# System V Release 4 (386/486)
#EXTRALIBS=-lsocket -lnsl
 
# Solaris (SunOS 5.1)
#EXTRALIBS=-lnsl -lsocket -lresolv
 
# MIPS R3000 running EP/IX Version 1.4.3. Compiler is RISCompiler C 2.11 (cc).
#EXTRALIBS=-lbsd
 
# Don't change this line.  Define EXTRALIBS before this line if you
# wish to add any libraries.
LIBS=-lm $(EXTRALIBS)

# Define these for LPC->C support
#EXTRA_EFUNS=cfuns.o cfuns_main.o cfuns_port.o cfuns_math.o cfuns_sock.o
#EXTRA_OBJS=lpc_compiler.tab.o compile_file.o interface.o

#################################################
# the meat of things                            #
# don't change anything below this section      #
#################################################

EFUNS=efuns_main.o efuns_port.o efuns_math.o efuns_matrix.o efuns_sock.o \
    $(EXTRA_EFUNS)

OBJ=compiler.tab.o lex.o main.o rc.o interpret.o simulate.o file.o object.o \
  backend.o array.o mapping.o comm.o ed.o regexp.o swap.o buffer.o crc32.o \
  malloc.o call_out.o otable.o dumpstat.o stralloc.o hash.o mudlib_stats.o \
  port.o reclaim.o parse.o simul_efun.o sprintf.o uid.o \
  avltree.o \
  socket_efuns.o socket_ctrl.o qsort.o eoperators.o md.o \
  gridgen.o \
  strstr.o disassembler.o binaries.o $(UALARM) $(STRFUNCS) \
  $(MALLOC) $(EFUNS) replace_program.o functab_tree.o $(EXTRA_OBJS)

all: $(DRIVER_BIN) addr_server


##-----------------------------------
##-- Start Gridgen (Rag)
##-----------------------------------
GRIDGEN_DEPENDS=gridgen/caverns.c gridgen/grid.c gridgen/line.c gridgen/point.c gridgen/gridgen.c gridgen/maze.c gridgen/pointpool.c gridgen/twist.c gridgen/circle.c gridgen/gridgen.h gridgen/rectangle.c gridgen/dungeon.c gridgen/mines.c gridgen/row.c

gridgen.o: $(GRIDGEN_DEPENDS)
	$(CC) $(OPTIMIZE) -c $(DEBUG) $(PIPE) gridgen/gridgen.c
##-----------------------------------


# For Sequent DYNIX/ptx compile $(OBJ) is parallel:
# $(DRIVER_BIN):& $(OBJ)
$(DRIVER_BIN): $(OBJ)
	-mv $(DRIVER_BIN) $(DRIVER_BIN).old
	$(PROOF) $(CC) $(CFLAGS) $(OBJ) -o $(DRIVER_BIN) $(LIBS)

all: $(DRIVER_BIN) addr_server lpc_to_c.h

lpc_to_c.h: efuns.h cfuns.h virtual_architecture.h
	-cat efuns.h cfuns.h virtual_architecture.h > lpc_to_c.h

depend:
	makedepend *.c

addr_server:  addr_server.o socket_ctrl.o port.o addr_server.h
	$(CC) $(CFLAGS) socket_ctrl.o addr_server.o port.o \
	-o addr_server $(RESOLV) $(EXTRALIBS)

.c.o:
	$(CC) $(CFLAGS) -c $*.c

compiler.tab.h: compiler.tab.c

lpc_compiler.tab.h: lpc_compiler.tab.c

cc.h: Makefile
	rm -f cc.h
	echo "/* this file automatically generated by the Makefile */" > cc.h
	echo '#define COMPILER "$(CC)"' >> cc.h
	echo '#define OPTIMIZE "$(OPTIMIZE)"' >> cc.h
	echo '#define CFLAGS   "$(CFLAGS2)"' >> cc.h

lex.o: cc.h lex.c compiler.tab.h

interpret.o: interpret.c patchlevel.h

base.h: efun_defs.c

efun_protos.h: efun_defs.c

efunctions.h: efun_defs.c

opc.h: efun_defs.c

func_spec.cpp: func_spec.c op_spec.c config.h options.h port.h
	$(CPP) $(CFLAGS) func_spec.c > func_spec.cpp

make_func.tab.c: make_func.y
	$(YACC) $(YFLAGS) make_func.y
	-rm -f make_func_tab.c
	mv y.tab.c make_func.tab.c

make_func: make_func.tab.o
	$(CC) $(CFLAGS) make_func.tab.o -o make_func

make_malloc: make_malloc.o
	$(CC) $(CFLAGS) make_malloc.o -o make_malloc

make_malloc.o: make_malloc.c config.h

compiler.tab.o: opcodes.h

lpc_compiler.tab.o: opcodes.h

compiler.tab.c: compiler.y
	$(YACC) $(YFLAGS) compiler.y
	rm -f compiler.tab.*
	mv y.tab.c compiler.tab.c
	mv y.tab.h compiler.tab.h

lpc_compiler.y: comp_warning lpc_compiler/pre token_defs lpc_compiler/grammar lpc_compiler/post
	cat comp_warning lpc_compiler/pre token_defs lpc_compiler/grammar lpc_compiler/post > lpc_compiler.y

lpc_compiler.tab.c: lpc_compiler.y
	$(YACC) $(YFLAGS) lpc_compiler.y
	-rm -f lpc_compiler.tab.*
	sed "s/yy/lpcyy/g" y.tab.c | sed "s/y.tab.c/lpc_compiler.tab.c/g" > lpc_compiler.tab.c
	sed "s/yy/lpcyy/g" y.tab.h > lpc_compiler.tab.h

efun_defs.c opcodes.h: malloc.c func_spec.c make_func config.h func_spec.cpp
	./make_func > efun_defs.c

malloc.c: config.h make_malloc
	./make_malloc $(MALLOC_C)
	touch malloc.c
	$(CC) $(CFLAGS) -c malloc.c

tags: force
	ctags *.c *.y

TAGS: force
	etags *.c *.y

force:

install: $(DRIVER_BIN) addr_server
	-mkdir $(INSTALL_DIR)
	$(INSTALL) $(DRIVER_BIN) $(INSTALL_DIR)
	$(INSTALL) addr_server $(INSTALL_DIR)

clean:
	-rm -rf obj *.o mon.out gmon.out *.tab.c *.tab.h *.orig *.rej
	-rm -f efun_defs.c *.ln tags */*~ *~ TAGS
	-rm -f cc.h
	-rm -f lpc_compiler.y
	-rm -f opcodes.h efunctions.h opc.h efun_protos.h
	-rm -f make_func make_malloc
	-rm -f $(DRIVER_BIN) $(DRIVER_BIN).old addr_server
	-rm -f func_spec.cpp

# DO NOT DELETE THIS LINE -- make depend depends on it.
