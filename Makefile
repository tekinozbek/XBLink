INCL = -I include/ -I libxbee/

OBJ = XBeeModule.o XBeeMessage.o XBeeMessageHandler.o tunnel.o

compile: $(OBJ)
	g++ --std=c++11 $(INCL) -o xblink $^ main.cpp -lxbee

%.o: src/%.cpp
	g++ --std=c++11 $(INCL) -c -o $@ $< -lxbee

clean:
	rm -f *.o *.so *.a a.out
