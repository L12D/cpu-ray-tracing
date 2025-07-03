# Fichiers
TARGET = main
SRCS = src/main.cpp src/utils.cpp src/Camera.cpp src/Scene.cpp src/Sphere.cpp src/Object.cpp src/Shape.cpp src/Application.cpp src/TriangleSet.cpp
OBJS = $(SRCS:.cpp=.o)
OBJS := $(patsubst %.o, build/%.o, $(OBJS))

# Compilation
CC = g++
CFLAGS = -std=c++17 -O3 -g
LDFLAGS = 

# Libs
CFLAGS += $(shell pkg-config --cflags opencv4)
LDFLAGS += $(shell pkg-config --libs opencv4)

all: build/$(TARGET)

build/$(TARGET): $(OBJS)
	@mkdir -p $(dir $@)
	$(CC) $^ $(LDFLAGS) -o $@

build/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

test: override CFLAGS += -g -Wall -Wextra -pedantic -fsanitize=address -fno-omit-frame-pointer -Wno-unused-but-set-variable
test: override LDFLAGS += -fsanitize=address
test: build/$(TARGET)
	./build/$(TARGET)

run: 
	make
	./build/$(TARGET)

clean:
	rm -rf build
