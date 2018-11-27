CXXFLAGS += -std=c++11
GTKFLAGS = `/usr/bin/pkg-config gtkmm-3.0 --cflags --libs`

ex: choice.cpp choice.h
	$(CXX) $(CXXFLAGS) -o choice choice.cpp $(GTKFLAGS)
