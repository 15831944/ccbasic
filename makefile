#ͨ��makefile
#���
#����׼��: make config
#����debug�棺make 
#����release�棺make -e DEBUG=0

PLAT ?= none
PLATS = linux

linux : MakeLinux

UpdateSubModuleLinux :
	PLAT = linux
	git submodule update --init
	
MakeLinux : UpdateSubModuleLinux 
	include lib/linux/makefile




