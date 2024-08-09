ifeq ($(OS),Windows_NT)
	ifeq ($(shell uname -s),) # not in a bash-like shell
		RM = del /F /Q
		MKDIR = mkdir
	else # in a bash-like shell, like msys
		RM    += -r
		MKDIR = mkdir -p
	endif

	TARGET_EXTENSION_EXE        = .exe
	TARGET_EXTENSION_LIB_STATIC = .lib
else
	RM    += -r
	MKDIR = mkdir -p

	TARGET_EXE_EXTENSION        = 
	TARGET_EXTENSION_LIB_STATIC = .a
endif

CFLAGS_BASE +=\
	-Wall\
	-Wextra\
	-Wpedantic\
	-Wcast-align\
	-Wwrite-strings\
	-Wmissing-field-initializers\
	-Wno-unknown-pragmas\
	-Wundef

ifeq ($(NDEBUG),)
	CFLAGS_BASE += -g -O0
else
	CFLAGS_BASE += -DNDEBUG -s -O2
endif

CFLAGS +=\
	-std=gnu-17\
	$(CFLAGS_BASE)
CXXFLAGS +=\
	-std=gnu++17\
	$(CFLAGS_BASE)

SRC_EXT = cpp

DIR_PATH_BUILD = build
DIR_NAME_OBJS  = objs
DIR_NAME_DEPS  = deps
DIR_PATH_OUT   = out

DEPS =

print_var = $(info VAR $(1) = $($(1)))
