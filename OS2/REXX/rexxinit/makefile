# $Id: makefile,v 1.4 2004/08/16 04:51:07 prokushev Exp $
#
#
#

PROJ = rexxinit
TRGT = $(PROJ).dll
DESC = REXX initializer DLL
DIRS = exe
srcfiles     = $(p)rexxinit$(e)
ADD_COPT     = -i=. -sg -3r # -d3
ADD_LINKOPT  = segment type DATA shared # lib clib3r.lib,math387r.lib,emu387.lib,rexx.lib # debug all 
DLL          = 1
DLLOPT       = initinstance
OPTIONS      = manyautodata, caseexact, heapsize=40960, start=dllinit
STACKSIZE    = 81920
EXPORTS      = RxApiInit.1, &
               dllinit.2

DEST         = os2$(SEP)dll
UNI2H        = 1

!include $(%ROOT)tools/mk/appsos2_cmd.mk

#TARGETS = $(PATH)$(PROJ).exe # subdirs
#TARGET = all
