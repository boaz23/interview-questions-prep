DEPFLAGS = -MMD -MP -MT $@ -MF $(word 2,$^)

define _define_object_targets
# parameters
# $(1): Variables namespace. This parameter should be hardcoded into this code
#   here instead of using a variable for it because its value is overwritten if
#   this definition is evaluated again and this affects the commands executed
#   in the receipies.
#
$(1)_targets_prefix = $(strip $(2))
$(1)_dir_path_src = $(strip $(3))
$(1)_inc_dirs = $(strip $(4))
# parameters end

$(1)_SRCS = $$(wildcard $$($(1)_dir_path_src)/*.$(SRC_EXT))
$(1)_SRC_BASE_NAMES = $$(foreach src,$$($(1)_SRCS),$$(basename $$(notdir $$(src))))

$(1)_targets_prefix_post_seped=$$($(1)_targets_prefix:%=%/)
$(1)_TARGETS = $$(foreach name,$$($(1)_SRC_BASE_NAMES),$$($(1)_targets_prefix_post_seped)$$(name))
$(1)_path = $$(1)$$(2:%=/%)$$($(1)_targets_prefix:%=/%)
$$($(1)_TARGETS): $$($(1)_targets_prefix_post_seped)%: $$(call $(1)_path,out,)/% ;
.PHONY: $$($(1)_TARGETS)

$(1)_build_path = $$(call $(1)_path,$(DIR_PATH_BUILD),$$(1))
$(1)_map_build_paths = $$(foreach name,$$($(1)_SRC_BASE_NAMES),$$(call $(1)_build_path,$$(1))/$$(name).$$(2))

$(1)_OBJS = $$(call $(1)_map_build_paths,$(DIR_NAME_OBJS),o)
$(1)_DEPS = $$(call $(1)_map_build_paths,$(DIR_NAME_DEPS),d)
DEPS += $$($(1)_DEPS)

$(1)_CXXFLAGS =\
	$$(CXXFLAGS)\
	$$(DEPFLAGS)\
	$$(patsubst %,-I%,$$($(1)_inc_dirs))\

$$($(1)_OBJS): $$(call $(1)_build_path,$(DIR_NAME_OBJS))/%.o:\
		$$($(1)_dir_path_src)/%.$(SRC_EXT)\
		$$(call $(1)_build_path,$(DIR_NAME_DEPS))/%.d
	@$$(MKDIR) $$(@D)
	$$(CXX) $$($(1)_CXXFLAGS) -c $$< -o $$@
endef
