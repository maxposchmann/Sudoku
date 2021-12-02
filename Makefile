# CFLAGS: we're using this variable to define the flags we are passing to
#CFLAGS = -O3
CFLAGS = -g -std=gnu++0x
CXX = g++

OBJ_DIR = obj
SRC_DIR = src
INC_DIR = include

#OBJS: variable that we are storing the list of object files that are shared across
#multiple executables.
OBJS = $(OBJ_DIR)/Board.o $(OBJ_DIR)/Cell.o

#Rule for building all of the .o files corresponding to the .cpp files.

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp Makefile
	$(CXX) -c $(CFLAGS) -I $(INC_DIR) $< -o $@
	$(CXX) -MM $(CFLAGS) -I $(INC_DIR) $< > $(OBJ_DIR)/$*.d

sudoku: $(OBJS) $(OBJ_DIR)/Sudoku.o Makefile
	$(CXX) $(CFLAGS) -o sudoku $(OBJS) $(OBJ_DIR)/Sudoku.o

#Rule named clean - gets rid of all remnants of the compile.
clean:
	rm $(OBJ_DIR)/*.o $(OBJ_DIR)/*.d sudoku

# dependency info for *existing* .o files
-include $(OBJS:.o=.d)
