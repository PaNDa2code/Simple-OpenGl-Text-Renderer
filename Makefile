CC=gcc
CF=-Iinclude -I/usr/include/freetype2 -g
LF=-lglfw -lGL -ldl -lm -lrt -lfreetype -lGLU -lOpenGL
TARGET=bin/OpenGlText
OBJ_DIR=obj/
SRC_DIR=src/

OBJECT_FILES:=$(subst .c,.o,$(wildcard $(SRC_DIR)*.c))
OBJECT_FILES:=$(subst $(SRC_DIR),$(OBJ_DIR), $(OBJECT_FILES))

SHADER_SRCS_MACROS = \
    -DVERTEX_SHADER_SOURCE="\"$(shell awk '{printf "%s\\n", $$0}' src/shaders/vertex_shader.glsl)\"" \
		-DFRAGMENT_SHADER_SOURCE="\"$(shell awk '{printf "%s\\n", $$0}' src/shaders/fragment_shader.glsl)\""

all: $(TARGET) 

clean:
	@rm -f bin/* obj/*

.PHONY: all clean

obj/shader_program.o: src/shader_program.c
	@echo "[*] Compiling $<"
	@$(CC) $(SHADER_SRCS_MACROS) -c $(CF) -o $@ $<

obj/%.o: src/%.c 
	@echo "[*] Compiling $<"
	@$(CC) -c $(CF) -o $@ $<

$(TARGET): $(OBJECT_FILES)
	@echo "[*] Linking all together"
	@$(CC) -o $@ $^ $(LF) $(CF)
