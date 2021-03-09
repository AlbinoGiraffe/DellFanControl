CXX = g++
CXXFLAGS = -Wall

OBJECTS = fancontrol.o

fancontrol: $(OBJECTS)
	$(CXX) -g $(CXXFLAGS) -o fancontrol fancontrol.o

fancontrol.o: fancontrol.cpp fancontrol.h
	$(CXX) -g $(CXXFLAGS) -c fancontrol.cpp

clean: 
	rm -f *.o
	rm fancontrol
