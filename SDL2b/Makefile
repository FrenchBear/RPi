CC = gcc
CXX = g++
CFLAGS = -Wall -std=gnu11 `sdl2-config --cflags --libs`
CXXFLAGS = -Wall `sdl2-config --cflags --libs`
LDLIBS = -lm -lSDL2_image -lSDL2_ttf

PROGS =	00Hello 01_hello_SDL 02_image 03_event 04_key 05_optimized 06_extension 07_texture 08_geometry 09_viewport \
		10_colorkeying 11_cliprendering 12_colormodulation 13_alphablending 14_animated 15_rotation 16_ttf

make:	$(PROGS)

clean:
		rm -f $(PROGS)
