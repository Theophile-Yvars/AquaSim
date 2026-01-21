# ========================
# Project configuration
# ========================

CXX      := g++
CXXFLAGS := -Wall -Wextra -Wpedantic -std=c++20 -Iinclude
LDFLAGS  :=

TARGET   := app
SRC_DIR  := src
BUILD_DIR:= build

# ========================
# Source / Object files
# ========================

SRCS := $(wildcard $(SRC_DIR)/*.cpp)
OBJS := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRCS))

# ========================
# Rules
# ========================

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR) $(TARGET)

re: clean all

.PHONY: all clean re
