# ============================================================================
# Makefile portable para El Gran Torneo Pokemon
#
# Windows (MinGW):   mingw32-make
# Linux / macOS:     make
#
# Targets:
#   make            compila y deja el ejecutable en bin/
#   make run        compila y ejecuta el programa
#   make clean      borra obj/ y bin/
#   make rebuild    limpia y vuelve a compilar todo
# ============================================================================

CC       := gcc
CFLAGS   := -std=gnu99 -O2 -Wall -Wextra -Iinclude
LDLIBS   :=

SRC_DIR  := src
OBJ_DIR  := obj
BIN_DIR  := bin
INCLUDE  := include

SRC      := $(sort $(wildcard $(SRC_DIR)/*.c))
OBJ      := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC))
DEPS     := $(wildcard $(INCLUDE)/*.h)

ifeq ($(OS),Windows_NT)
    TARGET  := $(BIN_DIR)/torneo.exe
    MKDIR   = if not exist "$(1)" mkdir "$(1)"
    CLEAN_OBJ = -@if exist "$(OBJ_DIR)" rmdir /S /Q "$(OBJ_DIR)"
    CLEAN_BIN = -@if exist "$(BIN_DIR)" rmdir /S /Q "$(BIN_DIR)"
    RUN     := $(BIN_DIR)\torneo.exe
else
    TARGET  := $(BIN_DIR)/torneo
    MKDIR   = mkdir -p "$(1)"
    CLEAN_OBJ = -@rm -rf "$(OBJ_DIR)"
    CLEAN_BIN = -@rm -rf "$(BIN_DIR)"
    RUN     := ./$(TARGET)
endif

.PHONY: all run clean rebuild help

all: $(TARGET)

$(TARGET): $(OBJ)
	@$(call MKDIR,$(BIN_DIR))
	$(CC) $(OBJ) -o $@ $(LDLIBS)
	@echo Ejecutable listo: $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(DEPS)
	@$(call MKDIR,$(OBJ_DIR))
	$(CC) $(CFLAGS) -c $< -o $@

run: $(TARGET)
	@$(RUN)

clean:
	$(CLEAN_OBJ)
	$(CLEAN_BIN)

rebuild: clean all

help:
	@echo Objetivos disponibles:
	@echo   all      Compila el proyecto (por defecto).
	@echo   run      Compila y ejecuta el programa.
	@echo   clean    Elimina obj/ y bin/.
	@echo   rebuild  Limpia y compila de nuevo.
