CXX=g++

#BOOST_INCLUDE=-I/usr/local/boost/include
#BOOST_LIB=-L/usr/local/boost/lib

CXXFLAGS=-std=c++0x -O3 -I. $(BOOST_INCLUDE)
#CXXFLAGS=-std=c++0x -stdlib=libc++ -O3 -I. $(BOOST_INCLUDE)

LDLIBS=$(BOOST_LIB) -lboost_iostreams -lboost_system -lboost_filesystem

SRC=faster2

all: $(SRC)

clean:
	rm -rf $(SRC)

.cpp:
	$(CXX) $(CXXFLAGS) $< -o $@ $(LDLIBS)
