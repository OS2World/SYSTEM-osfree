# $Id: makefile,v 1.3 2004/08/21 14:49:03 prokushev Exp $
#
# Makefile for Watcom/LIBC interface layer

PROJ = mm_shared
TRGT = $(PROJ).lib
ADD_COPT = -i=$(MYDIR)..$(SEP)include # -i=$(%WATCOM)$(SEP)h$(SEP)os2 # until mmos2 .uni modules will be ready

# For performance/compatibility reasons, we build a "_System" and
# "_Optlink"-like libraries, corresponding to "-5r" and "-5s" options of
# Watcom compiler.
srcfiles = $(p)file$(e) $(p)globstr$(e) &
           $(p)mciini$(e) $(p)msg$(e) $(p)dll$(e) $(p)crc$(e)

TARGETS  = $(PATH)$(PROJ).lib

!include $(%ROOT)tools/mk/libsos2.mk

$(PATH)$(PROJ).lib: $(OBJS)
 @$(MAKE) $(MAKEOPT) library=$(PATH)$(PROJ).lib library

