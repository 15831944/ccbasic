#ͨ��makefile
#���
#����׼��: make config
#����debug�棺make 
#����release�棺make -e DEBUG=0

linux : MakeLinux

UpdateSubModuleLinux : 
	 git submodule update --init
	
MakeLinux : UpdateSubModuleLinux
	cd lib/linux && $(MAKE)


LIBEVENT_PATH := 3rd/libevent

libevent : 
	cd $(LIBEVENT_PATH) && ./autogen.sh && ./configure && make && make install

clean : 
	cd lib/linux && $(MAKE) clean

