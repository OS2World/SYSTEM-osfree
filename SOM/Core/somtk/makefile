
!ifdef HOSTBLD
OS2TK = $(%OS2TK)
PLATFORM = host$(SEP)$(%HOST)$(SEP)
!else
OS2TK = $(%ROOT)build$(SEP)bin$(SEP)os2tk45
!endif

DEST = none

!include $(%ROOT)tools/mk/all.mk

TARGETS= pre $(OS2TK)$(SEP)som$(SEP)lib$(SEP)somtk.lib

$(OS2TK)$(SEP)som$(SEP)lib$(SEP)somtk.lib: $(OS2TK)$(SEP)som$(SEP)common$(SEP)dll$(SEP)som.dll
	$(verbose)$(LIB) -b $^@ +$< $(BLACKHOLE)

pre: .symbolic
 @$(MDHIER) $(OS2TK)$(SEP)som$(SEP)lib
