# Variables
CPPFLAGS = -I include
LDFLAGS = -lpthread
SRC_DIR = ./
OBJ_DIR = obj
SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS = $(SOURCES:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)
TARGET = main


.PHONY: all

all: $(TARGET)

# Rule to link the final executable
$(TARGET): $(OBJECTS)
	g++ $^ $(LDFLAGS) -o $@

#  Rule to compile each source file to an object file
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	mkdir -p $(OBJ_DIR)
	g++ $(CPPFLAGS) -c $< -o $@
