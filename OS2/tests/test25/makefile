#
#
#
#

PROJ = mini33
TRGT = $(PROJ).exe
DESC = Protshell stub
srcfiles  = $(p)mini33$(e)
# defines additional options for C compiler
ADD_COPT  = -db -d2 -s -g=DGROUP -i=$(MYDIR)..$(SEP)..$(SEP)include -i=$(%WATCOM)$(SEP)h -i=$(%WATCOM)$(SEP)h$(SEP)os2
OPTIONS   = start=startup,internalrelocs
#IMPORTS   = DosPutMessage MSG.5
IMPORTS   = DosPutMessage DOSCALLS.387
STACKSIZE = 32k
DEST        = os2$(SEP)test

!include $(%ROOT)tools/mk/appsos2_cmd.mk
