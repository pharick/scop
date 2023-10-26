TARGET_EXEC	= scop

BUILD_DIR	= ./build
SRC_DIR		= ./src
INC_DIR		= ./include
SRC_FILES	= main.cpp ShaderProgramCompiler.cpp ObjParser.cpp Matrix.cpp

SRCS		= $(addprefix $(SRC_DIR)/,$(SRC_FILES))
OBJS		= $(SRCS:%=$(BUILD_DIR)/%.o)
DEPS		= $(OBJS:.o=.d)

LIB_FLAGS	= -lglfw -lGLEW -lGL
CPPFLAGS	= -I$(INC_DIR) -std=c++11 -MMD -MP -Wall -Wextra -Werror

all: $(BUILD_DIR)/$(TARGET_EXEC)

$(BUILD_DIR)/$(TARGET_EXEC): $(OBJS)
	$(CXX) $(LIB_FLAGS) $(OBJS) -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.cpp.o: %.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

.PHONY: clean re

clean:
	rm -r $(BUILD_DIR)

re: clean all

-include $(DEPS)
