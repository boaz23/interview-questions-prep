CXXFLAGS=\
	-Wall\
	-Wpedantic\
	-std=gnu++17
INC_DIRS=\
	utils

DEBUG ?= true
ifeq ($(DEBUG),false)
	CXXFLAGS += -s -O2
else
ifeq ($(DEBUG),true)
	CXXFLAGS += -g -O0
else
$(error "Invalid value for `DEBUG`. Must be either `true` or `false`.")
endif
endif

no_target:
	@echo "No target chosen" >&2 && exit 1

clean:
	rm -rf -- out/* build/*

.PHONY: no_target clean

INHOUSE_TARGETS=$(foreach src,$(wildcard *.cpp),$(basename $(src)))
INHOUSE_OBJS=$(INHOUSE_TARGETS:%=build/%.o)

LEETCODE_SRC_DIR_PATH=../leetcode/cpp
LEETCODE_FILE_PATHS=$(wildcard $(LEETCODE_SRC_DIR_PATH)/*.cpp)
LEETCODE_FILE_BASE_NAMES=$(foreach src,$(LEETCODE_FILE_PATHS),$(basename $(notdir $(src))))
LEETCODE_TARGETS=$(foreach name,$(LEETCODE_FILE_BASE_NAMES),leetcode_$(name))
LEETCODE_OBJS=$(LEETCODE_TARGETS:%=build/%.o)

QUESTIONS_TARGETS=$(INHOUSE_TARGETS) $(LEETCODE_TARGETS)
$(QUESTIONS_TARGETS): %: out/% ;
.PHONY: $(QUESTIONS_TARGETS)

CXXFLAGS += $(foreach dir,$(INC_DIRS),-I$(dir))

out/%: build/%.o utils/utils.h
	$(CXX) -o $@ $^

$(INHOUSE_OBJS): build/%.o: %.cpp
$(LEETCODE_OBJS): build/leetcode_%.o: $(LEETCODE_SRC_DIR_PATH)/%.cpp

build/%.o:
	@mkdir -p $(@D)
	$(CXX) -c -o $@ $< $(CXXFLAGS)
