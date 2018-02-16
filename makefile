all:
	g++ --std=c++14 loc_dispatch.cpp -o loc_dispatch
clean:
	rm -f *.o loc_dispatch
