CFLAGS = -O2 -Wall -MMD -MP
LDFLAGS = -lvulkan -lxcb -lxcb-xkb
BUILDDIR = build

SOURCES  = $(wildcard *.c)
SOURCES += $(wildcard **/*.c)
OBJECTS  = $(SOURCES:%.c=$(BUILDDIR)/%.o)
SHADERS  = $(wildcard shaders/*.vert shaders/*.frag)
SPV      = $(SHADERS:%.vert=%.vert.spv) $(SHADERS:%.frag=%.frag.spv)

.PHONY: clean all

all: $(BUILDDIR)/engine shaders

-include $(OBJECTS:.o=.d)

$(BUILDDIR)/%.o: %.c
	@mkdir -p $(BUILDDIR)
	@mkdir -p $(BUILDDIR)/vulkan
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILDDIR)/engine: $(OBJECTS)
	$(CXX) $^ $(LDFLAGS) -o $@

.PHONY: test clean

test: all
	@$(BUILDDIR)/engine

clean:
	@rm -Rf build shaders/*.spv

shaders/%.vert.spv: shaders/%.vert
	glslc $< -o $@

shaders/%.frag.spv: shaders/%.frag
	glslc $< -o $@

shaders: $(SPV)
