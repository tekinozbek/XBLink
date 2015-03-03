INCL = -I include/ -I libxbee/

OBJ = 

compile: $(OBJ)
	g++ --std=c++11 $(INCL) $^ main.cpp

%.o: src/%.cpp
	g++ --std=c++11 $(INCL) -c -o $@ $<

clean:
	rm -f *.o *.so *.a a.out
