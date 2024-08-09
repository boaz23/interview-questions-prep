$(DEPS):
	@$(MKDIR) $(@D)

.PHONY: clean

clean:
	$(RM) --\
		$(DIR_PATH_OUT)/*\
		$(DIR_PATH_BUILD)/$(DIR_NAME_OBJS)/*\
		$(DIR_PATH_BUILD)/$(DIR_NAME_DEPS)/*

include $(wildcard $(DEPS))
