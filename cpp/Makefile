MY_MAKEFLAGS =\
	-r\
	-f makefiles/Makefile\
	-Imakefiles/

TARGETS = $(shell ./make_targets.sh $(MY_MAKEFLAGS))

.PHONY: no_target all compile_commands

no_target:
	@echo "No target chosen" >&2 && exit 1

all: $(TARGETS)

compile_commands:
	compiledb -nv -o build/compile_commands.json --command-style $(MAKE) all

$(TARGETS):
	$(MAKE) $(MY_MAKEFLAGS) $@
