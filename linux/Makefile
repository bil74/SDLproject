#Copyright Notice:
#The files within this zip file are copyrighted by Lazy Foo' Productions (2004-2014)
#and may not be redistributed without written permission.

#OBJS specifies which files to compile as part of the project
OBJS = main.c control.c display.c movement.c render.c timer.c utils.c config.c

#CC specifies which compiler we're using
CC = gcc

#COMPILER_FLAGS specifies the additional compilation options we're using
# -w suppresses all warnings
COMPILER_FLAGS = -O3 -Wall -I.

#LINKER_FLAGS specifies the libraries we're linking against
LINKER_FLAGS = -lSDL2 -lSDL2_ttf -lm

#OBJ_NAME specifies the name of our exectuable
OBJ_NAME = sdlproject

#This is the target that compiles our executable
all : $(OBJS)
	$(CC) $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)
