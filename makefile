TARGET := dm_wm
BUILD_DIR := dist
SRC_FILES := $(wildcard src/*.cpp)

CXX := g++
CXXFLAGS := -Wall
LDFLAGS := -lX11 -lXcomposite -lXrender

all: build

build:
	@echo "Building $(TARGET)..."
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(SRC_FILES) -o $(BUILD_DIR)/$(TARGET) $(CXXFLAGS) $(LDFLAGS)
	@echo "Build complete. Executable is located in $(BUILD_DIR)/$(TARGET)"

install:
	sudo chmod +x dist/dm_wm

launch:
	@echo "Launching $(TARGET)..."
	export DISPLAY=:1
	sudo xinit ./dist/dm_wm -- :1