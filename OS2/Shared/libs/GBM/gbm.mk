# $Id: makefile,v 1.1 2004/08/16 06:27:05 prokushev Exp $

PORT_NAME = os2$(SEP)gbm
PORT_TYPE = wget
PORT_URL  = https://www.os2site.com/sw/graphics/gbm/gbmos2pm160_gbm176_lu143_moz147_src.zip
PORT_REV  = 
PORT_PATCHES  = 

!include $(%ROOT)tools/mk/port.mk
