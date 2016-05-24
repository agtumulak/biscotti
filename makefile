# Makefile
# Aaron G. Tumulak

# Set options
CC :=g++ #--analyze -Qunused-arguments
CFLAGS :=-std=c++11 -Wall #-DNDEBUG
LFLAGS :=
TARGETNAME :=biscotti

# Set directories
SRCDIR := src
BUILDDIR := build
BINDIR := bin

# Get all object files
SOURCES := $(shell find $(SRCDIR) -type f -name *.cpp)
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.cpp=.o))

# Make target
$(BINDIR)/$(TARGETNAME): $(OBJECTS)
	@echo "Linking $@..."
	$(CC) $^ -o $(BINDIR)/$(TARGETNAME) $(LFLAGS)

-include $(OBJECTS:.o=.d)

$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp setup
	@echo "Creating object file $@..."
	$(CC) $(CFLAGS) -c -o $@ $<
	$(CC) $(CFLAGS) -MM $< -MT '$@' > $(@:.o=.d)

setup:
	@echo "Creating directories..."
	mkdir -p $(BUILDDIR)
	mkdir -p $(BINDIR)

clean:
	@echo "Cleaning..."
	rm -rf $(BUILDDIR) $(BINDIR)
	@echo "Done!"

.PHONY: clean setup
