CXX = g++
CC = gcc
CXXFLAGS = -I include -std=c++17 -Wall -Wextra
CFLAGS = -I include
SRC_DIR = src
OBJ_DIR = obj
INCLUDE_DIR = include

# OS detection
ifeq ($(OS),Windows_NT)
	MKDIR = powershell -Command "if (!(Test-Path '$(OBJ_DIR)')) { New-Item -ItemType Directory -Path '$(OBJ_DIR)' | Out-Null }"
	RM = powershell -Command "Remove-Item -Recurse -Force -ErrorAction SilentlyContinue '$(OBJ_DIR)', '$(TARGET)'; exit 0"
	TARGET = ticket_system.exe
	RUN = ./$(TARGET)
else
	MKDIR = mkdir -p $(OBJ_DIR)
	RM = rm -rf $(OBJ_DIR) $(TARGET)
	TARGET = ticket_system
	RUN = ./$(TARGET)
endif

SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS = $(SOURCES:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)
OBJECTS += $(OBJ_DIR)/sqlite3.o

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET)
	@echo "Build successful! Run with: $(RUN)"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)/sqlite3.o: $(INCLUDE_DIR)/sqlite3.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR):
	$(MKDIR)

clean:
	-$(RM)
	@echo "Cleaned build files"

run: $(TARGET)
	$(RUN)

rebuild: clean all

.PHONY: all clean run rebuild
