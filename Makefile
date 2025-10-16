# Manic Miner Improved Makefile

# Project info
PROJECT = manicminer
VERSION_MAJOR = 1
VERSION_MINOR = 0
YEAR = $(shell date +'%Y')
BUILD_VERSION = "v$(VERSION_MAJOR).$(VERSION_MINOR).$(YEAR)"

# Platform detection and configuration
UNAME_S := $(shell uname -s)
UNAME_M := $(shell uname -m)

# Default compiler and flags (for native builds)
CC = gcc
TARGET = $(PROJECT)
BASE_LDFLAGS = -lSDL2 -lm
CFLAGS = -std=c99 -pedantic -Wall -Wextra -Wshadow -Wpointer-arith -Wcast-align \
         -Wmissing-prototypes -Wmissing-declarations -Wredundant-decls -Wnested-externs \
         -g -MMD -MP

# Initialize variables
OBJ_DIR =
LDFLAGS =
CPPFLAGS =

# Platform-specific configuration
ifneq ($(CROSS_COMPILE),)
    # Cross-compilation mode (set by cross-* targets)
    ifeq ($(CROSS_COMPILE), windows)
        CC = x86_64-w64-mingw32-gcc
        TARGET = $(PROJECT).exe
        OBJ_DIR = windows-cross
        SDL2_WIN_PATH ?= /usr/x86_64-w64-mingw32
        CFLAGS += -I$(SDL2_WIN_PATH)/include/SDL2
        # Windows libraries required for SDL2
        LDFLAGS = -L$(SDL2_WIN_PATH)/lib $(BASE_LDFLAGS) -static \
                  -lmingw32 -mwindows -mconsole \
                  -lsetupapi -lwinmm -limm32 -lversion -luuid -loleaut32 -lole32 -lshlwapi -lgdi32 -luser32 -lshell32 -ldinput8 -ldxguid -ldxerr8 -lsetupapi -lwinmm
        CPPFLAGS = -DBUILD=\"v$(VERSION_MAJOR).$(VERSION_MINOR).$(YEAR)\" -DPROJECT_NAME=\"$(PROJECT)\" -DPLATFORM_Windows=1
    else ifeq ($(CROSS_COMPILE), linux-arm64)
        CC = aarch64-linux-gnu-gcc
        TARGET = $(PROJECT)
        OBJ_DIR = linux-arm64-cross
        CFLAGS += -I/usr/include/SDL2
        LDFLAGS = $(BASE_LDFLAGS) -ldl -lpthread
        CPPFLAGS = -DBUILD=\"v$(VERSION_MAJOR).$(VERSION_MINOR).$(YEAR)\" -DPROJECT_NAME=\"$(PROJECT)\" -DPLATFORM_Linux=1
    endif
else ifneq ($(origin OS), undefined)
    # Windows (explicitly set via OS environment variable)
    CC = x86_64-w64-mingw32-gcc
    TARGET = $(PROJECT).exe
    OBJ_DIR = windows
    SDL2_WIN_PATH ?= /usr/x86_64-w64-mingw32
    CFLAGS += -I$(SDL2_WIN_PATH)/include/SDL2
    LDFLAGS = -L$(SDL2_WIN_PATH)/lib $(BASE_LDFLAGS) -static \
              -lmingw32 -mwindows -mconsole \
              -lsetupapi -lwinmm -limm32 -lversion -luuid -loleaut32 -lole32 -lshlwapi -lgdi32 -luser32 -lshell32 -ldinput8 -ldxguid -ldxerr8 -lsetupapi -lwinmm
    CPPFLAGS = -DBUILD=\"v$(VERSION_MAJOR).$(VERSION_MINOR).$(YEAR)\" -DPROJECT_NAME=\"$(PROJECT)\" -DPLATFORM_Windows=1
else ifeq ($(findstring MINGW,$(UNAME_S))$(findstring MSYS,$(UNAME_S)), MINGWMSYS)
    # Native Windows (MinGW or MSYS)
    CC = gcc
    TARGET = $(PROJECT).exe
    OBJ_DIR = windows-native
    LDFLAGS = $(BASE_LDFLAGS) -lmingw32 -mwindows -mconsole \
              -lsetupapi -lwinmm -limm32 -lversion -luuid -loleaut32 -lole32 -lshlwapi -lgdi32 -luser32 -lshell32
    CPPFLAGS = -DBUILD=\"v$(VERSION_MAJOR).$(VERSION_MINOR).$(YEAR)\" -DPROJECT_NAME=\"$(PROJECT)\" -DPLATFORM_Windows=1
else ifeq ($(UNAME_S), Darwin)
    # macOS
    ifeq ($(UNAME_M), arm64)
        OBJ_DIR = macos-arm64
        ifeq ($(shell [ -d /opt/homebrew ] && echo exists), exists)
            CFLAGS += -I/opt/homebrew/include/SDL2
            LDFLAGS = -L/opt/homebrew/lib $(BASE_LDFLAGS)
        else ifeq ($(shell [ -d /usr/local/include/SDL2 ] && echo exists), exists)
            CFLAGS += -I/usr/local/include/SDL2
            LDFLAGS = -L/usr/local/lib $(BASE_LDFLAGS)
        else
            CFLAGS += -I/usr/include/SDL2
            LDFLAGS = $(BASE_LDFLAGS)
        endif
    else
        OBJ_DIR = macos-x86_64
        ifeq ($(shell [ -d /usr/local/include/SDL2 ] && echo exists), exists)
            CFLAGS += -I/usr/local/include/SDL2
            LDFLAGS = -L/usr/local/lib $(BASE_LDFLAGS)
        else
            CFLAGS += -I/usr/include/SDL2
            LDFLAGS = $(BASE_LDFLAGS)
        endif
    endif
    CFLAGS += -D_DARWIN_C_SOURCE
    LDFLAGS += -framework CoreAudio -framework AudioToolbox -framework ForceFeedback -framework CoreVideo -framework Cocoa -framework IOKit -framework Carbon
    CPPFLAGS = -DBUILD=\"v$(VERSION_MAJOR).$(VERSION_MINOR).$(YEAR)\" -DPROJECT_NAME=\"$(PROJECT)\" -DPLATFORM_Darwin=1
else ifeq ($(UNAME_S), Linux)
    # Linux
    ifeq ($(UNAME_M), aarch64)
        OBJ_DIR = linux-arm64
    else ifeq ($(UNAME_M), x86_64)
        OBJ_DIR = linux-x86_64
    else
        OBJ_DIR = linux-$(UNAME_M)
    endif
    CFLAGS += -I/usr/include/SDL2
    LDFLAGS = $(BASE_LDFLAGS) -ldl -lpthread
    CPPFLAGS = -DBUILD=\"v$(VERSION_MAJOR).$(VERSION_MINOR).$(YEAR)\" -DPROJECT_NAME=\"$(PROJECT)\" -DPLATFORM_Linux=1
else
    # Fallback
    OBJ_DIR = build-$(UNAME_S)-$(UNAME_M)
    CFLAGS += -I/usr/include/SDL2
    LDFLAGS = $(BASE_LDFLAGS)
    CPPFLAGS = -DBUILD=\"v$(VERSION_MAJOR).$(VERSION_MINOR).$(YEAR)\" -DPROJECT_NAME=\"$(PROJECT)\" -DPLATFORM_$(UNAME_S)=1
endif

# Set defaults if not set by platform-specific blocks
OBJ_DIR ?= linux-native
LDFLAGS ?= $(BASE_LDFLAGS)
CPPFLAGS ?= -DBUILD=\"v$(VERSION_MAJOR).$(VERSION_MINOR).$(YEAR)\" -DPROJECT_NAME=\"$(PROJECT)\" -DPLATFORM_$(UNAME_S)=1

# Directory structure
SRC_DIR = src
BUILD_DIR = build
OBJ_PATH = $(BUILD_DIR)/$(OBJ_DIR)

# Source files
SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c,$(OBJ_PATH)/%.o,$(SRCS))
DEPS = $(OBJS:.o=.d)

# Default target
all: $(TARGET)

# Main executable
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)
	@echo "Build complete: $(TARGET) (v$(VERSION_MAJOR).$(VERSION_MINOR).$(YEAR))"
	@echo "Platform: $(UNAME_S) $(UNAME_M)"
	@echo "Build directory: $(OBJ_DIR)"
	@echo "Compiler: $(CC)"

# Standard compilation rule for ALL source files
$(OBJ_PATH)/%.o: $(SRC_DIR)/%.c | $(OBJ_PATH)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

# Create build directory
$(OBJ_PATH):
	@mkdir -p $(OBJ_PATH)

# Debug build with extra warnings
debug: CFLAGS += -DDEBUG -O0 -Werror
debug: $(TARGET)
	@echo "Debug build"

# Release build
release: CFLAGS += -O2 -DNDEBUG -Wno-unused-parameter
release: $(TARGET)
	@echo "Release build: $(TARGET)"

# Universal macOS build (if on macOS)
macos-universal:
	@if [ "$(UNAME_S)" != "Darwin" ]; then \
		echo "Error: This target is only available on macOS"; \
		exit 1; \
	fi
	$(MAKE) clean
	@echo "Building universal binary for macOS..."
	@mkdir -p $(BUILD_DIR)/macos-universal
	# Build for x86_64
	$(MAKE) CC="gcc -arch x86_64" OBJ_DIR=macos-x86_64-temp
	mv $(TARGET) $(BUILD_DIR)/macos-universal/$(PROJECT)-x86_64
	# Build for arm64
	$(MAKE) CC="gcc -arch arm64" OBJ_DIR=macos-arm64-temp
	mv $(TARGET) $(BUILD_DIR)/macos-universal/$(PROJECT)-arm64
	# Create universal binary
	lipo -create \
		$(BUILD_DIR)/macos-universal/$(PROJECT)-x86_64 \
		$(BUILD_DIR)/macos-universal/$(PROJECT)-arm64 \
		-output $(TARGET)
	@rm -rf $(BUILD_DIR)/macos-x86_64-temp $(BUILD_DIR)/macos-arm64-temp
	@echo "Universal binary created: $(TARGET)"

# Cross-compilation targets - use environment variables for complete isolation
cross-windows:
	@echo "Cross-compiling for Windows..."
	@echo "Note: Ensure SDL2 development libraries for MinGW are installed"
	@echo "On Ubuntu/Debian: sudo apt-get install mingw-w64 libsdl2-dev"
	@if ! which x86_64-w64-mingw32-gcc >/dev/null 2>&1; then \
		echo "Error: MinGW cross-compiler not found"; \
		exit 1; \
	fi
	+$(MAKE) CROSS_COMPILE=windows

cross-linux-arm64:
	@echo "Cross-compiling for Linux ARM64..."
	@echo "Note: Ensure aarch64 cross-compiler is installed"
	@echo "On Ubuntu/Debian: sudo apt-get install gcc-aarch64-linux-gnu libsdl2-dev"
	@if ! which aarch64-linux-gnu-gcc >/dev/null 2>&1; then \
		echo "Error: ARM64 cross-compiler not found"; \
		exit 1; \
	fi
	+$(MAKE) CROSS_COMPILE=linux-arm64

# Native Windows build (if actually running on Windows)
windows-native:
	$(MAKE) OBJ_DIR=windows-native

# Install to appropriate locations
install: $(TARGET)
ifeq ($(UNAME_S), Darwin)
	@mkdir -p /usr/local/bin
	cp $(TARGET) /usr/local/bin/
	@echo "Installed to /usr/local/bin/$(TARGET)"
else ifneq (,$(findstring MINGW,$(UNAME_S))$(findstring MSYS,$(UNAME_S)))
	@echo "Windows installation - copy $(TARGET) to desired location manually"
else
	@mkdir -p ~/.local/bin
	cp $(TARGET) ~/.local/bin/
	@echo "Installed to ~/.local/bin/$(TARGET)"
endif

# Uninstall
uninstall:
ifeq ($(UNAME_S), Darwin)
	rm -f /usr/local/bin/$(TARGET) 2>/dev/null || true
	@echo "Uninstalled from /usr/local/bin/$(TARGET)"
else ifneq (,$(findstring MINGW,$(UNAME_S))$(findstring MSYS,$(UNAME_S)))
	@echo "Windows uninstall - remove $(TARGET) manually"
else
	rm -f ~/.local/bin/$(TARGET) 2>/dev/null || true
	@echo "Uninstalled from ~/.local/bin/$(TARGET)"
endif

# Clean builds
clean:
	rm -rf $(BUILD_DIR) $(TARGET) $(PROJECT).exe 2>/dev/null || true
	@echo "Clean complete"

# Deep clean
distclean: clean
	rm -rf dist 2>/dev/null || true
	@echo "Distribution files cleaned"

# Show build information
info:
	@echo "Project: $(PROJECT)"
	@echo "Version: v$(VERSION_MAJOR).$(VERSION_MINOR).$(YEAR)"
	@echo "Platform: $(UNAME_S) $(UNAME_M)"
	@echo "Compiler: $(CC)"
	@echo "Target: $(TARGET)"
	@echo "Build directory: $(OBJ_DIR)"
	@echo "Source files: $(words $(SRCS))"
	@echo "Compiler flags: $(CFLAGS)"
	@echo "Linker flags: $(LDFLAGS)"
	@echo "Preprocessor flags: $(CPPFLAGS)"

# Check dependencies
check-deps:
	@echo "Checking dependencies..."
ifeq ($(UNAME_S), Darwin)
	@if which sdl2-config >/dev/null; then \
		echo "✓ SDL2 found"; \
	else \
		echo "✗ SDL2 not found. Install with: brew install sdl2"; \
	fi
else ifeq ($(UNAME_S), Linux)
	@if which sdl2-config >/dev/null; then \
		echo "✓ SDL2 found (development)"; \
	elif pkg-config --exists sdl2; then \
		echo "✓ SDL2 found (pkg-config)"; \
	else \
		echo "✗ SDL2 not found. Install with: sudo apt-get install libsdl2-dev"; \
	fi
	@if which x86_64-w64-mingw32-gcc >/dev/null; then \
		echo "✓ MinGW cross-compiler found"; \
	else \
		echo "✗ MinGW cross-compiler not found. Install with: sudo apt-get install mingw-w64"; \
	fi
	@if which aarch64-linux-gnu-gcc >/dev/null; then \
		echo "✓ ARM64 cross-compiler found"; \
	else \
		echo "✗ ARM64 cross-compiler not found. Install with: sudo apt-get install gcc-aarch64-linux-gnu"; \
	fi
else ifneq (,$(findstring MINGW,$(UNAME_S))$(findstring MSYS,$(UNAME_S)))
	@echo "Windows environment detected"
	@if which sdl2-config >/dev/null; then \
		echo "✓ SDL2 found"; \
	else \
		echo "✗ SDL2 not found"; \
	fi
endif

# Include dependencies
-include $(DEPS)

# Phony targets
.PHONY: all debug release install uninstall clean distclean info \
        macos-universal cross-windows cross-linux-arm64 windows-native check-deps

# Help target
help:
	@echo "Manic Miner Build System"
	@echo ""
	@echo "Targets:"
	@echo "  all               - Build the project (default)"
	@echo "  debug             - Build with debug flags"
	@echo "  release           - Build with optimizations"
	@echo "  install           - Install to system bin directory"
	@echo "  uninstall         - Remove from system bin directory"
	@echo "  clean             - Remove build artifacts"
	@echo "  distclean         - Remove all generated files"
	@echo "  info              - Show build information"
	@echo "  check-deps        - Check for required dependencies"
	@echo ""
	@echo "Platform-specific:"
	@echo "  macos-universal   - Create universal binary (macOS only)"
	@echo "  cross-windows     - Cross-compile for Windows"
	@echo "  cross-linux-arm64 - Cross-compile for Linux ARM64"
	@echo "  windows-native    - Native Windows build (if on Windows)"
	@echo ""
	@echo "Current platform: $(UNAME_S) $(UNAME_M)"
	@echo "Build directory: $(OBJ_DIR)"
