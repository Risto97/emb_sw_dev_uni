CXX      := -g++
CXXFLAGS := -Wall -Wextra -std=c++11 -lrt
LDFLAGS  := -L/usr/lib -lstdc++
BUILD    := ./build
OBJ_DIR  := $(BUILD)/objects
APP_DIR  := .
TARGET   := program
INCLUDE  := -Iinclude/ -I../../z1/ -I./
SRC      :=  $(wildcard *.cc)
SRC			 += ../../z1/frame.cc
SRC      +=  $(wildcard ../common/*.cc)
INCLUDE  += -I../common/

OBJECTS := $(SRC:%.cc=$(OBJ_DIR)/%.o)

all: build $(APP_DIR)/$(TARGET)

$(OBJ_DIR)/%.o: %.cc
	echo $(SRC)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(INCLUDE) -o $@ -c $<

$(APP_DIR)/$(TARGET): $(OBJECTS)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(OBJECTS) $(INCLUDE) $(LDFLAGS) -o $(APP_DIR)/$(TARGET)

.PHONY: all build clean debug release

build:
	@mkdir -p $(APP_DIR)
	@mkdir -p $(OBJ_DIR)

debug: CXXFLAGS += -DDEBUG -g
debug: all

release: CXXFLAGS += -O2
release: all

clean:
	-@rm -rvf $(OBJ_DIR)/*
	-@rm -rvf $(TARGET)
	-@rm -rvf *.ppm
