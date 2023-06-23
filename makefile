CC=g++
CFLAGS=-c
PREPROCESS=-DSFML_STATIC
VERSION=c++17
WARNING=-Wall
OPT=-O0
objects=HanoiTowersSFML.o
INCLUDE=-IC:\SFML_64bit\SFML-2.5.1\include
LIBS=-LC:\SFML_64bit\SFML-2.5.1\lib
DEPENDENCY=-lopengl32 -lwinmm -lgdi32 -lsfml-graphics-s-d -lsfml-window-s-d -lsfml-system-s-d -lkernel32

all: HanoiTowersSFML 

HanoiTowersSFML: $(objects)
	$(CC) -o HanoiTowersSFML $(objects) $(LIBS) $(DEPENDENCY)
	@echo Build succeeded
	
HanoiTowersSFML.o : HanoiTowersSFML.cpp
	$(CC) $(WARNING) $(PREPROCESS) $(INCLUDE) $(CFLAGS) HanoiTowersSFML.cpp -o HanoiTowersSFML.o -std=$(VERSION) $(OPT)
	@echo Compilation succeeeded
	
.PHONY:
	clean
	
clean:
	rm -f $(objects)
	@echo Cleanup done