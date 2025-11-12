CXX = g++
CXXFLAGS = -I include -std=c++11 -Wall -Wextra
SRC_DIR = src
OBJ_DIR = obj
INCLUDE_DIR = include

SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS = $(SOURCES:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)
TARGET = ticket_system

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET)
	@echo "Build successful! Run with: ./$(TARGET)"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	rm -rf $(OBJ_DIR) $(TARGET)
	@echo "Cleaned build files"

run: $(TARGET)
	./$(TARGET)

rebuild: clean all

.PHONY: all clean run rebuild
