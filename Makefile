CXX      = g++

LIBS     = gtkmm-3.0 gdkmm-3.0

CXXFLAGS = '-std=c++11' '-std=gnu++11' '-Wall' `pkg-config --cflags $(LIBS)`
LDFLAGS  = `pkg-config --libs $(LIBS)`

OBJ = main.o default_clipboard_controller.o clipboard_model.o gtk_clipboard_model.o

main: $(OBJ)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(OBJ) -o main

%.o: %.c
	$(CXX) $(CXXFLAGS) -c $<

