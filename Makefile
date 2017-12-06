CXX=g++
#CXX=g++-4.7	# for Mac OS X users
CAPD_DIR=$(HOME)/local/capd-DynSys3
CAPD_BIN=$(CAPD_DIR)/bin

TARGET = ex5

all:	$(TARGET)

ex5:	ex5.cpp
	$(CXX) -O2 $< -o $@ \
	`$(CAPD_BIN)/capd-config --cflags --libs`

clean:
	-rm -f $(TARGET) *~ *.o *.exe


