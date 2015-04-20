export MAKEFLAGS=--no-print-directory

SOURCES := BlobDetector.cpp \
	HsvThreshold.cpp \
	LineDetector.cpp

# compiler
CXX := g++

# compiler flags
CXXFLAGS := -O2 -Wall -std=c++11

# object directory name
OBJ_DIR := obj

# output directory
OUTPUT_DIR := build

OUTPUT := main

# include all objects
OBJECTS = $(addprefix $(OBJ_DIR)/, $(notdir $(SOURCES:.cpp=.o)))

# library flags
LDFLAGS := `pkg-config --libs opencv`

# include directories
INCLUDES := -I. `pkg-config --cflags opencv`

# file to store dependencies
DEPENDENCIES_FILE := dependencies.d

all: directories dependencies
	@$(MAKE) $(OUTPUT_DIR)/main
	@$(MAKE) $(OUTPUT_DIR)/calib

$(OUTPUT_DIR)/main: $(OBJECTS) $(OBJ_DIR)/main.o
	@echo "[$@]"
	@$(CXX) $(CXXFLAGS) $^ $(INCLUDES) $(LDFLAGS) -o $@

$(OUTPUT_DIR)/calib: $(OBJECTS) $(OBJ_DIR)/calib.o
	@echo "[$@]"
	@$(CXX) $(CXXFLAGS) $^ $(INCLUDES) $(LDFLAGS) -o $@

directories:
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(OUTPUT_DIR)

dependencies:
	@$(CXX) -MM $(SOURCES) $(CXXFLAGS) $(INCLUDES) | sed 's|\([a-zA-Z0-9_-]*\)\.o|$(OBJ_DIR)/\1.o|' > $(DEPENDENCIES_FILE)

$(OBJ_DIR)/%.o: %.cpp
	@echo "[$<]"
	@$(CXX) $(CXXFLAGS) $(INCLUDES) -c -o $(OBJ_DIR)/$(notdir $@)  $<

-include $(DEPENDENCIES_FILE)

clean:
	@echo removing all build files
	@find . -name "*.o" -exec rm -f {} \;
	@find . -name "*.o.*" -exec rm -f {} \;
	@find . -name "*.bin" -exec rm -f {} \;
	@find . -name "*.elf" -exec rm -f {} \;
	@find . -name "*.hex" -exec rm -f {} \;
	@find . -name "*.out" -exec rm -f {} \;
	@find . -name "*.map" -exec rm -f {} \;
	@find . -name "*.pyc" -exec rm -f {} \;
	@find . -name "*.d" -exec rm -f {} \;
	@rm -f $(OUTPUT_DIR)/*
	@echo done

.PHONY: all directories dependencies clean




