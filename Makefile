# C++ compiler.
CC=g++

# Compilation and linking flags.
CC_FLAGS = -Wall -c -MMD -MP
LD_FLAGS =

# Target 
TARGET = simulator 

# Header files directory
SRCDIR = ./src/
INCDIR = ./inc/
OBJDIR = ./obj/

# Sources
SRCS = $(wildcard $(SRCDIR)*.cpp)

# Object files
OBJS = $(addprefix $(OBJDIR),$(notdir $(SRCS:.cpp=.o)))

# The dependency file names.
DEPS := $(OBJS:.o=.d)

$(TARGET): $(OBJS)
	@echo "\`\` building: $@"
	@$(CC) $(LD_FLAGS) -o $@ $^

obj/%.o: src/%.cpp | $(OBJDIR)
	@echo "\`\` compiling: $<"
	@$(CC) $(CC_FLAGS) -I$(INCDIR) -o $@ $<

# Read dependency files and handle them
-include $(DEPS)

$(OBJDIR):
	mkdir -p $(OBJDIR)

clean: 
	rm -r $(OBJDIR)
	rm $(TARGET)
