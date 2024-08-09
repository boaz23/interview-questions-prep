MY_MAKEFLAGS =\
	-r\
	-f makefiles/Makefile\
	-Imakefiles/

.PHONY: no_target

no_target:
	@echo "No target chosen" >&2 && exit 1

$(shell ./.make_targets.sh $(MY_MAKEFLAGS)):
	$(MAKE) $(MY_MAKEFLAGS) $@
