# Makefile to build ImageFilters program
# --- macros

CPP       = g++
CXXFLAGS  = -Wall
OBJECTS   = Image.o Main.o
RM        = rm -f
PRJ       = ImageFilters.exe

# --- targets
all: ImageFilters

ImageFilters: Image.o Main.o
	$(CPP) $(CXXFLAGS) -o $(PRJ) $(OBJECTS)

$(BIN): $(OBJ)
	$(CPP) $(CXXFLAGS) $(OBJECTS) -o $(PRJ)

Image.o: Image.cpp Image.h
	$(CPP) $(CXXFLAGS) -c Image.cpp -o Image.o

Main.o: Main.cpp Image.h FiltersAdj.h FiltersPunctual.h FiltersConvolution.h FiltersStripes.h FFT.h FT.h 
	$(CPP) $(CXXFLAGS) -c Main.cpp -o Main.o

# --- remove binary and executable files
clean:
	$(RM) $(OBJECTS) $(PRJ)
