# Manic Miner - AI Coding Guide

## Project Overview
C port of the 1983 ZX Spectrum game "Manic Miner" using SDL2. 256×192 pixel resolution with retro aesthetics: per-pixel coloring, 16-color palette, polyphonic square-wave audio with stereo panning.

## Architecture: Event-Driven State Machine

The core architecture uses function pointers for state transitions ([main.c](../src/main.c)):
- **Action**: State initialization (e.g., `Loader_Action`, `Game_Action`, `Title_Action`)
- **Responder**: Input handling 
- **Ticker**: Game logic updates (50ms per frame)
- **Drawer**: Rendering
- **Flasher**: Periodic effects (every 20 frames)

States transition by reassigning these function pointers. Example: `Action = Game_Action;` switches to gameplay.

## Critical Code Patterns

### State Transitions
State modules follow naming: `<State>_Action()` initializes, then assigns `Do<State>Ticker()`, `Do<State>Drawer()`, etc. See [game.c](../src/game.c) lines 349-362 or [title.c](../src/title.c).

### Video System
[video.c](../src/video.c) uses a custom character set and pixel buffer (`videoPixel[WIDTH * HEIGHT]`). Colors use ZX Spectrum palette (0-7 standard, 8-15 bright). Draw via `Video_Draw*()` functions, never direct pixel manipulation.

### Audio System
[audio.c](../src/audio.c) implements square-wave synthesis with 8 channels (5 music, 3 SFX). Stereo panning uses `panTable` for pseudo-spatial audio. Music data stored as event streams with timing/note info.

### Level Data
[levels.c](../src/levels.c) defines 20 levels with tile types (T_ITEM, T_SOLID, T_CONVEYL, etc.). Each level has robot/enemy patterns, conveyor belts, and collapsible blocks. Special levels: Eugene (4), Skylab (13), SPG (18-19) have custom logic in [robots.c](../src/robots.c) and [spg.c](../src/spg.c).

### Game Loop
50ms fixed timestep in [main.c](../src/main.c):
1. Call `Action()` (state setup if needed)
2. Poll events → `Responder()`
3. Update logic → `Ticker()`
4. Render → `Drawer()`
5. Flash update every 20 frames

## Build System

Cross-platform Makefile with platform detection and cross-compilation:

```bash
make                    # Native build (Linux/macOS/Windows)
make debug              # Debug build with -O0 -DDEBUG
make release            # Optimized -O2 build
make cross-windows      # Cross-compile for Windows (needs MinGW)
make cross-linux-arm64  # Cross-compile for ARM64
```

Build outputs to `build/<platform>/` with separate object directories per target. Dependencies auto-generated via `-MMD -MP`.

## Key Conventions

- **Fixed-width types**: Use `BYTE` (uint8), `WORD` (uint16), `UINT` (uint32) from [common.h](../src/common.h)
- **Coordinates**: Origin (0,0) is top-left. WIDTH=256, HEIGHT=192 pixels
- **Timing**: All game logic assumes 50ms ticks (20 FPS, matching PAL refresh)
- **Memory layout**: Sparse use of dynamic allocation; mostly static buffers
- **No external assets**: Graphics/audio data embedded in C arrays

## Development Workflow

1. **Testing changes**: Run `./manicminer` after `make`. No test suite; manual playtesting required.
2. **Debugging**: Use `make debug` for symbol info. Check `gameInput` for key states (KEY_* enums in [common.h](../src/common.h)).
3. **Adding features**: 
   - New states: Create `<name>_Action()` and `Do<Name>*()` functions
   - New graphics: Add to `charSet` arrays or sprite buffers
   - New audio: Extend music/SFX event streams in [audio.c](../src/audio.c)

## Game-Specific Logic

- **Cheat mode**: Activated by typing cheat codes (see [cheat.c](../src/cheat.c)). Changes `Responder` to `Cheat_Responder`.
- **Collision detection**: Tile-based via `Level_GetTileType()`. Miner position uses `minerX`/`minerY` (pixels) and `minerTile` (tile grid).
- **Air depletion**: Game timer in [game.c](../src/game.c). Reduced by collisions/time. Visual meter updates in `Game_DrawAir()`.

## Dependencies

- **SDL2**: Only external dependency. Platform-specific paths in Makefile.
- **Math library**: `-lm` for audio frequency calculations.
- **Platform libs**: MinGW requires additional Windows libs (user32, gdi32, etc.) - see Makefile line 35-37.

## File Organization

- [src/*.c](../src/): All implementation files (24 modules)
- [src/*.h](../src/): Headers (common.h, game.h, system.h, video.h, audio.h, misc.h)
- [build/](../build/): Platform-specific object files and dependency tracking
- Root: Makefile, README.md, COMPILE.md, LICENCE
