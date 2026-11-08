# Widescreen investigation

Initial source review, 8 November 2026. The investigation below describes the
original pipeline. A first implementation now exists on branch `widescreen`.

## First implementation

Run `./build/crusn --window --widescreen` from this checkout, with the ROM at
`roms/crusnusa.zip`. Browser mode defaults to widescreen; use `?widescreen=0`
(or append `&widescreen=0` to an existing query) for the original aspect ratio.
Native mode defaults to the original aspect ratio.
MAME validation forces original mode.

The world viewport uses 712×400 with menus and results in a centered 512×400
area. The race HUD uses edge anchors, as described below. It preserves original
projected coordinates, expands horizontal rejection/rasterization bounds to
-100 through 611, and translates X only at framebuffer addressing. This preserves
rasterization/texture interpolation in the original center region. Separate wide
buffers follow the original hardware pages; direct screen writes are mirrored
into their centers, and full-page fills extend across the new margins. Existing
sky and water strips use the expanded raster bounds; the road-height sampling
check also accepts the extended horizontal range. No assets or physics changed.

Native targets build successfully. The portable-boundary, C3X translation, and
assembly-source-sync checks pass. The changed game modules also compile without
`CRUISN_PORTABLE`. Original-mode hashes match a pre-change deterministic run over
1,900 ticks, and a captured wide frame at tick 1,900 has a pixel-identical center
crop with additional scenery in both margins. `game-probe` now accepts
`TICKS --widescreen --dump PATH.bmp` and `0 --widescreen --check-render`; the latter
checks both pages, full/partial fills, direct writes, margin rasterization, page
selection, and decoded output. Run it without `--widescreen` to check original
mode too. AddressSanitizer instrumentation is available for the probe through
`CRUSN_ENABLE_ASAN=ON` in a Debug build. The margin/page checks and a 5,700-tick
widescreen attract run completed without AddressSanitizer errors.

Interactive SDL presentation could not be tested in this execution environment:
SDL reports that no displays are available. Browser compilation/runtime and all
track/camera combinations remain unverified. Live aspect changes and individual
full-screen artwork/wipe policies remain follow-up work.
This is an initial implementation, not a claim of complete widescreen coverage.

On macOS, CMake restricts curl's include directories to C compilation, since
only `native_download.c` uses curl. This prevents an SDK-wide curl include path
from interfering with libc++ header lookup in the C++ sound-board code. No
temporary header copies or custom `CURL_INCLUDE_DIR` setting are required.

## Race HUD anchors

Only `HUD()` and its radar move to the viewport edges: elapsed time and speed
shift left by half the added width; position, RPM/gear, and radar shift right by
the same amount. Artwork, unit/transmission labels, digits, shadows, and radar
markers move together. Countdown and central messages retain their centered
coordinates. Entrance/exit animation offsets remain intact, including while the
race HUD slides out in `MBONUS`; separate bonus/results elements are unchanged.
All game-side changes are guarded by `CRUISN_PORTABLE`.

`./build/game-probe 1900 --check-hud --dump /tmp/hud.bmp` renders the actual HUD
against a blank background and checks its pixel placement at original and wide
widths. It covers 240 race/finish, transmission, MPH/KPH, position, and animation
combinations. Text is rendered through `TEXT_OUTPUT()`, so the checks include
deferred text and shadows rather than just immediate artwork.

True widescreen is feasible, but requires changes to game projection, background
coverage, render-target storage, and UI placement. SDL scaling alone can stretch
or crop the existing image; it cannot expose additional scenery.

## Current pipeline and constraints

| Area | Evidence | Implication |
| --- | --- | --- |
| Presentation | `src/platform/sdl_video.c:37` sets a 512×400 SDL logical size and creates a texture of that size. | Resizing/fullscreen preserves the existing aspect and adds unused space. |
| Storage | `src/portable/machine.h:13` defines 512×400, with two 0x40000-word screen pages. `machine.c` allocates contiguous hardware screen memory and derives page pointers from that fixed page size. | Dimensions, row pitch, allocation, and emulated memory addresses must be separated. |
| Rasterization | `src/portable/output.c` clips scanlines against compile-time dimensions and indexes pixels as `y * CRUSN_SCREEN_WIDTH + x`. | Wider output needs a target with explicit width, height, and stride. Wireframe needs the same treatment. |
| World projection | `src/game/dirq.c:535` computes projected X as `world_x * INVTAB[z >> 4] + SCRNHXI`. Y uses a 1.04 correction and `SCRNHYI`. Dynamic objects and shadows have additional projection sites. | Preserve projection scale and vertical framing; move the horizontal center and widen accepted screen bounds. |
| Geometry rejection | `dirq.c:1967` and `dirq.c:2108` reject polygons whose vertices are all beyond X=511. `CLIPCK` also has hardware coordinate/span checks. | Extra scenery can be discarded before reaching the portable rasterizer. Audit each executable path, including recursive subdivision. |
| Backgrounds | `src/game/infin.c` has its own projection, panorama strips, water geometry, and a 512-wide road-height visibility check. | World projection changes alone do not establish correct sky, horizon, or water coverage. |
| UI | `src/game/text.c:682` emits glyph quads through `_stuff_fpga`; `totala.c:124` submits those to the same `port_output_fpga` used by world geometry. `hud.c` contains positions such as X=256. | The rasterizer cannot reliably infer whether a quad is scenery, HUD, or a full-screen effect. Explicit placement/pass context is needed. |
| Direct screen access | `src/game/font1a.c:620` addresses pixels using `y << 9`. `util.c` clears screens using original addresses and counts through `SCREEN_FILL`. | Changing a width constant leaves direct writes and screen clears using the old layout. |
| Browser | `src/platform/web/shell.html:11` fixes CSS aspect to 512/400; the canvas also starts at 512×400. | Browser sizing and argument/configuration plumbing must follow the selected mode. |

The current executable `DISPLAY` implementation does not reproduce every
object-level X/Y rejection shown in its assembly comments. Distinguish active C
from commented assembly when auditing culling; the hardcoded polygon rejection
paths above are executable.

## Recommended design

Keep original mode as the default/reference path, and make widescreen an opt-in
portable enhancement. Guard every change in `src/game` with `CRUISN_PORTABLE`,
including new includes, declarations, and changed behavior. Preserve the original
path when the macro is undefined. Retain
the assembly comments and function boundaries, and expose host-independent
configuration through `src/portable`. SDL/window queries belong in
`src/platform`.

Introduce separate concepts for the original 512×400 game/UI coordinates, the
expanded world viewport, the software render target, and the presentation
rectangle. Avoid globally redefining `SCREENHX`, `SCRNHX`, or hardware screen
page sizes: those values encode different responsibilities.

For a first implementation, use a fixed aspect selected at startup. Preserve the
existing vertical projection and object scale and reveal additional scenery to
the left and right. For example, under the port's current square-pixel
presentation, a 712×400 target approximates 16:9 and shifts the world center from
256 to 356. An equivalent implementation could retain original projected
coordinates, allow X from -100 through 611, and add 100 at rasterization. Choose
one coordinate convention and apply it consistently to clipping and backgrounds.
Do not apply both offsets.

The port currently presents 512/400 = 1.28, rather than 4:3, and has an existing
1.04 vertical projection correction. Decide separately whether to reproduce a
4:3 arcade display with non-square pixels. That changes the internal width needed
for 16:9; 712×400 is an example preserving current presentation, not an assertion
about the intended CRT aspect.

Keep hardware-addressed screen memory at its original layout. Add separate
expanded page storage for wide rendering, with explicit handling of page flips,
clears, fills, and direct pixel writes. Original-mode rendering can continue to
use the original buffers. A 712×400 image requires 284,800 words, exceeding the
262,144 words in one hardware page; simply increasing dimensions causes writes
to cross page/allocation boundaries. Define which operations affect the full
wide target and which affect the centered legacy region; arbitrary hardware
address writes cannot automatically be interpreted as wide-layout operations.

Initially retain a centered 512×400 safe area for HUD and menus. Apply the same
offset to their text and graphics, preserving sizes and relative alignment.
Later, explicitly anchor selected HUD groups to the outer edges. Camera-relative
3D menu models, screen wipes, flashes, and full-screen artwork need individual
policies. Object flags such as `O_NOUNIV` alone are not sufficient proof that an
object is UI.

## Implementation sequence

1. Add original/wide configuration and an explicit render-target description.
   Update software rasterization, decoding, dumps, clears, page lifecycle, and SDL
   texture sizing. Preserve startup/diagnostic rendering in original coordinates.
2. Widen projection and rejection bounds for regular, dynamic, shadow, and
   illuminated geometry. Cover all camera paths. Retain depth behavior and the
   inverse table initially; widening the viewport does not require changing
   camera distance or physics.
3. Adapt infinity-plane sky/water coverage and road-height sampling. Verify
   panorama wrap, pitch/roll, turns, tunnels, and water transitions. Existing
   strips may cover the new field of view, but that requires runtime evidence.
4. Add explicit UI placement context and full-screen effect coverage. Ship
   centered UI first; treat edge anchoring as a subsequent polish step.
5. Expose the mode in native arguments and web configuration/sizing. Add live
   aspect changes only after fixed-mode resizing and buffer lifetimes are sound.

No GPU renderer is required for the first version: the current quad rasterizer
can target a wider buffer. At 712×400, framebuffer area grows by about 39%, with
additional geometry and overdraw beyond that. Measure native and browser frame
times before deciding whether renderer optimization is necessary.

## Verification and remaining unknowns

- Run the existing MAME validation flow in original mode. Wide screen positions,
  framebuffer contents, and potentially render-call counts intentionally differ
  from the original and cannot use the unmodified replay as their visual oracle.
  Additional palette lookups/calls may also affect replay ordering.
- Capture matching original/wide frames from deterministic runs. Compare the
  original image with the centered wide crop, allowing for deliberate effects
  changes and checking dither parity when translating coordinates.
- Inspect all tracks, camera views, menus, HUD, attract scenes, wipes, tunnels,
  water, and steep/rolled views for gaps and premature rejection. Source review
  cannot establish whether track assets and scenery activation cover newly
  visible areas; inspect activation/lifetime rules if objects pop at the edges.
- Exercise screen clears and page flips with memory bounds checking, then test
  fullscreen/windowed native output and browser canvas sizing/performance.
- After implementation, run `check-game-portable-boundary`,
  `check-asm-source-sync`, and, for changed translated floating-point code,
  `check-c3x-translation`, plus relevant build/runtime checks.

This is a cross-cutting rendering feature. The smallest useful milestone is one
fixed 16:9 world view with centered legacy UI, demonstrated through a complete
attract/race sequence including sky and water. Broad aspect support, automatic
window-driven FOV, and a fully rearranged HUD can follow that proof of coverage.
