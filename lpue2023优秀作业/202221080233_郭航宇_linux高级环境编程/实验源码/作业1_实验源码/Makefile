CXX = g++
CXXFLAGS = -std=c++17 -Wall
LINK = -lstdc++fs
DEBUG = -g -D DEBUG
SOURCE_DIR = src
BUILD_DIR = build
TARGET_DIR = bin
SOURCES = $(wildcard $(SOURCE_DIR)/*.cpp)
OBJECTS = $(patsubst $(SOURCE_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SOURCES))
COM_OBJS = $(BUILD_DIR)/filesystem.o
TARGETS = terminal create list read write delete copyFrom copyTo
EXECUTABLES = $(addprefix $(TARGET_DIR)/, $(TARGETS))

.PHONY: all clean

all: $(EXECUTABLES)

$(TARGET_DIR)/terminal: $(BUILD_DIR)/terminal.o $(COM_OBJS)
	$(CXX) $^ -o $@ $(CXXFLAGS) $(LINK) $(DEBUG)

$(TARGET_DIR)/create: $(BUILD_DIR)/create.o $(COM_OBJS)
	$(CXX) $^ -o $@ $(CXXFLAGS) $(LINK) $(DEBUG)

$(TARGET_DIR)/list: $(BUILD_DIR)/list.o $(COM_OBJS)
	$(CXX) $^ -o $@ $(CXXFLAGS) $(LINK) $(DEBUG)

$(TARGET_DIR)/read: $(BUILD_DIR)/read.o $(COM_OBJS)
	$(CXX) $^ -o $@ $(CXXFLAGS) $(LINK) $(DEBUG)

$(TARGET_DIR)/write: $(BUILD_DIR)/write.o $(COM_OBJS)
	$(CXX) $^ -o $@ $(CXXFLAGS) $(LINK) $(DEBUG)

$(TARGET_DIR)/delete: $(BUILD_DIR)/delete.o $(COM_OBJS)
	$(CXX) $^ -o $@ $(CXXFLAGS) $(LINK) $(DEBUG)

$(TARGET_DIR)/copyFrom: $(BUILD_DIR)/copyFrom.o $(COM_OBJS)
	$(CXX) $^ -o $@ $(CXXFLAGS) $(LINK) $(DEBUG)

$(TARGET_DIR)/copyTo: $(BUILD_DIR)/copyTo.o $(COM_OBJS)
	$(CXX) $^ -o $@ $(CXXFLAGS) $(LINK) $(DEBUG)

$(BUILD_DIR)/%.o: $(SOURCE_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR)/*.o $(EXECUTABLES)