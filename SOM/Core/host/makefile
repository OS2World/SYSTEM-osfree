#
# A Makefile for somFree project
#

SOM_COMPILER = somcpp somipc sc
# SOM Run-time kernel
SOM_KERNEL = somkpub somdtype somkprep som
# Interface Repository Framework
SOM_IR = #somiprep somref

DIRS = $(SOM_COMPILER) $(SOM_KERNEL) $(SOM_IR) somtk

# Below not correct yet
# somcorba somir somd somnmf somcdr somany somu somestrm somirfmt Frameworks

PLATFORM = host$(SEP)$(%HOST)$(SEP)

!include $(%ROOT)tools/mk/all.mk

