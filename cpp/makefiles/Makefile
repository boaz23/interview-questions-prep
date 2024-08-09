include Common.mk
include CommonObjects.mk

INC_DIRS=\
	utils
define_object_targets=$(eval $(call _define_object_targets,$(1),$(2),..$(3:%=/%)/cpp,$(INC_DIRS) $(4)))

$(call define_object_targets,free,,,)
$(call define_object_targets,leetcode,leetcode,leetcode,)

include CommonSecondaryTargets.mk

out/%: $(DIR_PATH_BUILD)/$(DIR_NAME_OBJS)/%.o
	@$(MKDIR) $(@D)
	$(CXX) -o $@ $^
