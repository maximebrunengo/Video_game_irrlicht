TARGET  := Video_game
BUILD   := build
SOURCES := src
######################################################################################
.SUFFIXES:
.SECONDARY:
CPPFLAGS = -I/usr/include/irrlicht
CXXFLAGS += -Wall -Wextra -O2 -ggdb3 -std=c++11 -Wno-unused-parameter
LDFLAGS += -lIrrlicht -pthread
ifneq ($(BUILD),$(notdir $(CURDIR)))
export OUTPUT   :=      $(CURDIR)/$(BUILD)/$(TARGET)
export VPATH    :=      $(foreach dir,$(SOURCES),$(CURDIR)/$(dir))
export DEPSDIR  :=      $(CURDIR)/$(BUILD)
CXXFILES        :=      $(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.cpp)))
export OFILES   :=      $(CXXFILES:.cpp=.o)
export LD       :=      $(CXX)
export OUTPUT   :=      $(CURDIR)/$(TARGET)
.PHONY: $(BUILD) clean
$(BUILD):
	@[ -d $@ ] || mkdir -p $@
	@$(MAKE) --no-print-directory -C $(BUILD) -f $(CURDIR)/Makefile
clean:
	@echo clean ...
	@rm -fr $(BUILD) $(OUTPUT)
else
$(OUTPUT): $(OFILES)
	@echo '[1m[[35mLD[37m][0m' $(notdir $@)
	@$(LD) $^ -o $@ $(LIBPATHS) $(LDFLAGS)
-include $(DEPSDIR)/*.d
endif
%.o: %.cpp
	@echo '[1m[[32mCC[37m][0m' $(notdir $<)
	@$(CXX) -MMD -MP -MF $(DEPSDIR)/$*.d $(CXXFLAGS) $(CPPFLAGS) $(INCLUDE) -c $< -o $@
