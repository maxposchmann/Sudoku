# CFLAGS: we're using this variable to define the flags we are passing to
#CFLAGS = -O3
CFLAGS = -g -std=gnu++0x
CXX = g++

#OBJS: variable that we are storing the list of object files that are shared across
#multiple executables.
OBJS= Board.o Cell.o

#Rule for building all of the .o files corresponding to the .cpp files.

%.o: %.cpp Makefile
	$(CXX) -c $(CFLAGS) $< -o $@
	$(CXX) -MM $(CFLAGS) $< > $*.d

sudoku: $(OBJS) Sudoku.o  Makefile
	$(CXX) $(CFLAGS) -o sudoku $(OBJS) Sudoku.o

#Rule named clean - gets rid of all remnants of the compile.
clean:
	rm *.o *.d sudoku

# dependency info for *existing* .o files
-include $(OBJS:.o=.d)
