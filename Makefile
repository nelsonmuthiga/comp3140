CXX = g++
CC = gcc
CXXFLAGS = -I include -std=c++11 -Wall -Wextra
CFLAGS = -I include
SRC_DIR = src
OBJ_DIR = obj
INCLUDE_DIR = include

# OS detection
ifeq ($(OS),Windows_NT)
	RM = rmdir /s /q
	RM_FILE = del /q
	MKDIR = if not exist $(OBJ_DIR) mkdir $(OBJ_DIR)
	TARGET = ticket_system.exe
	RUN = $(TARGET)
else
	RM = rm -rf
	RM_FILE = rm -f
	MKDIR = mkdir -p $(OBJ_DIR)
	TARGET = ticket_system
	RUN = ./$(TARGET)
endif

SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS = $(SOURCES:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)
# Add sqlite3.c object
OBJECTS += $(OBJ_DIR)/sqlite3.o

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET)
	@echo "Build successful! Run with: $(RUN)"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Compile sqlite3.c from include folder
$(OBJ_DIR)/sqlite3.o: $(INCLUDE_DIR)/sqlite3.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR):
	$(MKDIR)

clean:
ifeq ($(OS),Windows_NT)
	@if exist $(OBJ_DIR) $(RM) $(OBJ_DIR)
	@if exist $(TARGET) $(RM_FILE) $(TARGET)
else
	$(RM) $(OBJ_DIR) $(TARGET)
endif
	@echo "Cleaned build files"

run: $(TARGET)
	$(RUN)

rebuild: clean all

.PHONY: all clean run rebuild
