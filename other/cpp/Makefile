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

QUESTIONS_BINS=$(foreach src,$(wildcard *.cpp),$(basename $(src)))
$(QUESTIONS_BINS): %: out/% ;
.PHONY: $(QUESTIONS_BINS)

CXXFLAGS += $(foreach dir,$(INC_DIRS),-I$(dir))

out/%: build/%.o utils/utils.h
	$(CXX) -o $@ $^

build/%.o: %.cpp
	@mkdir -p $(@D)
	$(CXX) -c -o $@ $< $(CXXFLAGS)
