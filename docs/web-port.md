# Web port

The native SDL application and the Emscripten/WebAssembly application share the
translated game and a startup flow. No changes in `src/game` are required.
`CRUISN_PORTABLE` is defined for both builds; future game divergences must use
that guard, and game code must access host services through portable headers.

## Source layout

- `src/portable/startup.c`: check the cache, asynchronously download or import a
  ZIP, validate its game/sound contents, commit the cache, then signal readiness.
- `src/portable/startup_ui.c`: original embedded 5x7 glyphs and a host-independent
  ARGB loading/error framebuffer. No ROM fonts or external assets are needed.
- `src/portable/rom.c`: the existing ROM lane loader, also usable for validation
  without writing the machine's ROM memory.
- `src/portable/download.h`: download progress, cancellation, and cache interfaces.
- `src/platform/native_download.c`: libcurl multi transfers, disk caching, and
  atomic replacement (including Windows replacement semantics).
- `src/platform/web_download.c`: Emscripten Fetch, the in-memory filesystem, and
  asynchronous IndexedDB cache saves.
- `src/platform/web/shell.html`: canvas, user activation, local ZIP selection,
  sound/fullscreen controls, and IndexedDB restore before C startup.
- `src/platform/sdl_main.c`: persistent application state and one-step scheduling.
  The native executable loops over it; the browser uses requestAnimationFrame
  through `emscripten_set_main_loop_arg`. Game IRQ timing remains 57 Hz.

SDL creates the renderer before startup begins. Machine/game/audio initialization
happens only after ROM validation and cache installation. The same texture and
renderer display both the startup framebuffer and the game's decoded framebuffer.
The browser selects SDL's OpenGL ES 2 renderer, backed by WebGL.

## Downloads and storage

The default source is `https://archive.org/download/crusnusa/crusnusa.zip`.
This ZIP contains the ROM entries directly. CMake's `CRUSN_ROM_URL`, the native
`CRUSN_ROM_URL` environment variable, native `--rom-url`, and the web `rom-url`
query parameter allow overrides. Set the URL to an empty string to disable
automatic downloading. Existing valid ROMs take precedence over the URL.

The browser backend maps Internet Archive's `/download/` route to its `/cors/`
route, which serves the same file with cross-origin access headers. Other hosts
must provide appropriate CORS headers themselves. No external proxy is used.

Transfers are limited to 64 MiB and use a temporary `.part` file. Required 4.5
entry names, sizes, ZIP CRCs, and (when sound is enabled) the DCS catalog and
soft-boot image are checked before replacing `roms/crusnusa.zip`. The current
loader does not authenticate the file against an independent cryptographic ROM
manifest. A failed transfer/import leaves an existing cached ROM intact. R or
Retry download starts a replacement download; local files can be selected in the
web shell or dropped onto either startup UI.

Native caching uses the current working directory, or the parent of a macOS app
bundle. Browser caching mounts IDBFS at `/roms`. Restoration completes before
`main`; a save completes before starting the game. If browser persistence fails
(for example, a storage quota is exhausted), a validated ROM can still be used
for the current session and a warning is logged. Scores/CMOS persistence is a
separate feature and is not added by this change.

## Building and hosting

Use Emscripten 4.0.19, the SDK version pinned in CI:

```sh
emcmake cmake -S . -B build-web -DCMAKE_BUILD_TYPE=Release -DCRUSN_TRACE=OFF
cmake --build build-web --parallel
python3 -m http.server 8000 --directory build-web
```

Open `http://localhost:8000/crusn.html`. Deploy `crusn.html`, `crusn.js`, and
`crusn.wasm` together. The server must serve `.wasm` as `application/wasm`.
Use HTTPS for public hosting. This build is single-threaded and does not require
COOP/COEP headers. No ROM data is included in the generated files.

The native build requires SDL2 and libcurl 7.68+. Windows release packages bundle
curl's runtime dependencies and a CA certificate bundle. Linux needs its normal
libcurl runtime dependencies. Native TLS verification stays enabled.

## GitHub Pages

Every push builds the WebAssembly target in `.github/workflows/release.yml`.
Pushes to `main` also publish the successful web build to
https://jeff-1amstudios.github.io/cruisin-usa/. Manual workflow runs on `main`
can redeploy the site. Pull requests and other branches build without publishing.

The Pages artifact contains only `index.html`, `crusn.js`, and `crusn.wasm`.
The build's `crusn.html` becomes `index.html` when staging the Pages artifact;
relative JavaScript/Wasm URLs continue to work under the repository subpath.
Desktop/release archives are separate from the deployed site.

Repository Settings → Pages must use GitHub Actions as its publishing source.
The `web` job uploads the site; the `deploy-pages` job receives only the Pages
and OIDC permissions needed to deploy it to the `github-pages` environment.
Deployment does not wait for unrelated desktop packaging jobs.

## Verification

The startup integration tests use synthetic ZIP data and a local HTTP server;
they do not require arcade ROMs or call translated game functions:

```sh
cmake --build build --target startup-probe check-game-portable-boundary
python3 tools/test_startup.py build/startup-probe
```

The optional core probe runs the same translated code with neutral host-service
stubs and prints framebuffer hashes every 57 interrupts. With a supplied ROM in
`roms/crusnusa.zip`, compare native and Wasm execution:

```sh
cmake --build build --target game-probe
cmake --build build-web --target game-probe
./build/game-probe > /tmp/crusn-native-core.txt
node build-web/game-probe.js > /tmp/crusn-wasm-core.txt
diff -u /tmp/crusn-native-core.txt /tmp/crusn-wasm-core.txt
```

The Wasm probe uses Node's filesystem access and is for local testing only; it
is not part of the deployable browser files. Matching hashes are a smoke test,
not a replacement for MAME validation or browser graphics/audio tests.

Browser verification should cover fresh download, visible progress, invalid URL
and retry, local import, reload with a restored cache, keyboard input, mute,
fullscreen, and returning from a background tab. Chrome, Firefox, and Safari
should be checked before a public release. Touch/gamepad input is not added.

Use `?widescreen=1` to opt into the wider world view with centered UI. The SDL
texture switches from the original startup dimensions to 712×400 when the game
starts. Original aspect remains the default.
