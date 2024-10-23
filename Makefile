CC=gcc
CF=-Iinclude -I/usr/include/freetype2 -g
LF=-lglfw -lGL -ldl -lm -lrt -lfreetype -lGLU -lOpenGL
TARGET=bin/TextRender
OBJ_DIR=obj/
SRC_DIR=src/
SHADER_DIR=$(SRC_DIR)shaders/
SHADER_DEF_FILE=$(SHADER_DIR)shader_definitions.txt

OBJECT_FILES:=$(subst .c,.o,$(wildcard $(SRC_DIR)*.c))
OBJECT_FILES:=$(subst $(SRC_DIR),$(OBJ_DIR), $(OBJECT_FILES))

SHADER_SCRIPT=$(SHADER_DIR)generate_shader_defines.sh

all: $(TARGET) 

clean:
	@rm -f bin/* obj/* $(SHADER_DEF_FILE)

.PHONY: all clean shaders

# Step to generate shader definitions
shaders: $(SHADER_DEF_FILE)

$(SHADER_DEF_FILE): $(SHADER_DIR)/*.glsl $(SHADER_SCRIPT)
	@echo "[*] Generating shader definitions"
	@bash $(SHADER_SCRIPT)

# Compile the shader_program.c with shader definitions
obj/shader_program.o: src/shader_program.c $(SHADER_DEF_FILE)
	@echo "[*] Compiling $<"
	@$(CC) $(CF) -c -o $@ $< $(shell cat $(SHADER_DEF_FILE))

# Compile all other .c files
obj/%.o: src/%.c 
	@echo "[*] Compiling $<"
	@$(CC) -c $(CF) -o $@ $<

# Link everything together
$(TARGET): $(OBJECT_FILES)
	@echo "[*] Linking all together"
	@$(CC) -o $@ $^ $(LF) $(CF)
