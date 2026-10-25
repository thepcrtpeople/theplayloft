#---------------------------------------------------------------------------------
# Clear the implicit built in rules
#---------------------------------------------------------------------------------
.SUFFIXES:

ifeq ($(strip $(DEVKITPPC)),)
$(error "Please set DEVKITPPC to the devkitPro install directory, e.g. export DEVKITPPC=/opt/devkitpro")
endif

include $(DEVKITPPC)/wii_rules

#---------------------------------------------------------------------------------
# Target name
#---------------------------------------------------------------------------------
TARGET := boot
BUILD  := build
SOURCES := src
DATA := data
INCLUDES := include

#---------------------------------------------------------------------------------
# Object files and ldlists
#---------------------------------------------------------------------------------
CFILES := $(foreach dir,$(SOURCES),$(wildcard $(dir)/*.c))
CPPFILES := $(foreach dir,$(SOURCES),$(wildcard $(dir)/*.cpp))
SFILES := $(foreach dir,$(SOURCES),$(wildcard $(dir)/*.s))

OBJS := $(patsubst %.c,$(BUILD)/%.o,$(CFILES)) \
        $(patsubst %.cpp,$(BUILD)/%.o,$(CPPFILES)) \
        $(patsubst %.s,$(BUILD)/%.o,$(SFILES))

#---------------------------------------------------------------------------------
# Include libraries
#---------------------------------------------------------------------------------
LIBS := -logc -lm

#---------------------------------------------------------------------------------
# Build flags
#---------------------------------------------------------------------------------
CFLAGS := -O2 -Wall -Wextra -DGEKKO -DHW_RVL -mrvl -mcpu=750 -meabi -mhard-float
CPPFLAGS := -O2 -Wall -Wextra -DGEKKO -DHW_RVL
LDFLAGS := -g -Wl,-Map,$(TARGET).map

#---------------------------------------------------------------------------------
# Build rules
#---------------------------------------------------------------------------------
.PHONY: all clean

all: $(TARGET).dol

$(TARGET).dol: $(TARGET).elf
	@echo "\n[build] converting ELF to DOL"
	@elf2dol $< $@

$(TARGET).elf: $(OBJS)
	@echo "\n[link] $@"
	@$(CXX) $(OBJS) $(LDFLAGS) -o $@ $(LIBS)

$(BUILD)/%.o: %.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(BUILD)/%.o: %.cpp
	@mkdir -p $(dir $@)
	@$(CXX) $(CPPFLAGS) $(INCLUDES) -c $< -o $@

$(BUILD)/%.o: %.s
	@mkdir -p $(dir $@)
	@$(CC) $(ASFLAGS) -c $< -o $@

clean:
	@rm -rf $(BUILD) $(TARGET).elf $(TARGET).dol $(TARGET).map

#---------------------------------------------------------------------------------
# End
#---------------------------------------------------------------------------------
