#OBJS specifies which files to compile as part of the project
SOURCES = $(wildcard src/*.cpp)
OBJS = $(patsubst %.cpp, %.o, $(SOURCES))

LIB_DIRECTORY = ./JAGE
LIB_INCLUDE = ./JAGE/include
LIB_FILE = ./JAGE/jage.a

HEADER_DIR = ./headers

#CC specifies which compiler we're using
CC = g++
LINKER = g++

#COMPILER_FLAGS specifies the additional compilation options we're using
COMPILER_FLAGS = -O2 -g -Wall

#LINKER_FLAGS specifies the libraries we're linking against
LINKER_FLAGS = -lglfw -lGLEW -lGL

#RESULT specifies the name of our exectuable
RESULT = double_pendulum

all: $(RESULT)

#This is the target that compiles our executable
$(RESULT) : $(LIB_FILE) $(OBJS)
	$(LINKER) $(OBJS) $(LIB_FILE) $(LINKER_FLAGS) -o $(RESULT)

%.o: %.cpp
	$(CC) $(addprefix -I, $(LIB_INCLUDE)) $(addprefix -I, $(HEADER_DIR)) $(COMPILER_FLAGS) -c -o $@ $<

$(LIB_FILE):
	make -C $(LIB_DIRECTORY)

.PHONY: clean
clean:
	rm -f $(RESULT)
	rm -f $(OBJS)
	make -C $(LIB_DIRECTORY) clean

run: $(RESULT)
	./$(RESULT)

