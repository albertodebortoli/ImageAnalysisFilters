# Makefile to build ImageFilters program
# --- macros

CPP       = g++
CXXFLAGS  = -Wall
OBJECTS   = Image.o Main.o FiltersAdj.o FiltersPunctual.o FiltersConvolution.o FiltersStripes.o FFT.o FT.o
RM        = rm -f
PRJ       = ImageFilters

# --- targets
all: ImageFilters

ImageFilters: Image.o Main.o FiltersAdj.o FiltersPunctual.o FiltersConvolution.o FiltersStripes.o FFT.o FT.o
	$(CPP) $(CXXFLAGS) -o $(PRJ) $(OBJECTS)

$(BIN): $(OBJ)
	$(CPP) $(CXXFLAGS) $(OBJECTS) -o $(PRJ)

Image.o: Image.cpp Image.h
	$(CPP) $(CXXFLAGS) -c Image.cpp -o Image.o

FiltersAdj.o: FiltersAdj.cpp FiltersAdj.h
	$(CPP) $(CXXFLAGS) -c FiltersAdj.cpp -o FiltersAdj.o

FiltersPunctual.o: FiltersPunctual.cpp FiltersPunctual.h
	$(CPP) $(CXXFLAGS) -c FiltersPunctual.cpp -o FiltersPunctual.o

FiltersConvolution.o: FiltersConvolution.cpp FiltersConvolution.h
	$(CPP) $(CXXFLAGS) -c FiltersConvolution.cpp -o FiltersConvolution.o

FiltersStripes.o: FiltersStripes.cpp FiltersStripes.h
	$(CPP) $(CXXFLAGS) -c FiltersStripes.cpp -o FiltersStripes.o

FFT.o: FFT.cpp FFT.h
	$(CPP) $(CXXFLAGS) -c FFT.cpp -o FFT.o

FT.o: FT.cpp FT.h
	$(CPP) $(CXXFLAGS) -c FT.cpp -o FT.o

Main.o: Main.cpp Image.h FiltersAdj.h FiltersPunctual.h FiltersConvolution.h FiltersStripes.h FFT.h FT.h 
	$(CPP) $(CXXFLAGS) -c Main.cpp -o Main.o

# --- remove binary and executable files
clean:
	$(RM) $(OBJECTS) $(PRJ)
