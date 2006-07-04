
INST_DIR = 	/usr/local/fdo-3.2.0
INCLUDE =	-I$(FDO)/Unmanaged/inc

LIBS =	-L$(INST_DIR)/lib -lFDO.so -lxalan-c -lxerces-c

rtest:	rtest.cpp
	$(CXX) -g $(INCLUDE) rtest.cpp -o rtest $(LIBS) -lgdal

ConnectTests:	ConnectTests.cpp
	$(CXX) -g $(INCLUDE) ConnectTests.cpp -o ConnectTests $(LIBS) 

clean:
	rm -f *.o
	rm rtest ConnectTests
