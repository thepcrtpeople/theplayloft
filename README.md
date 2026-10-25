# ThePlayLoft Wii Kiosk Launcher

This repository contains a starter Wii homebrew project for a kiosk-style game launcher. The app hosts a small local web page on the Wii, and the receptionist can click a button to send a launch command to the Wii app. The Wii app then decides whether to launch the selected game from USB.

The project is intentionally structured as a template so you can adapt it to your own Wiiflow-compatible launch flow.

## What this includes

- A minimal Wii homebrew app skeleton
- Local HTTP server running on the Wii
- HTML/JS waiting screen served from the Wii app
- Launch endpoint (`/launch`) for the receptionist UI
- A safe allow-list of allowed game IDs
- Timer-based session flow (`/stop`, `state tracking`)
- Codespaces-ready setup for installing the Wii toolchain

## Project structure

- `src/main.c` - app entry point and main loop
- `src/http_server.c` / `http_server.h` - tiny HTTP server implementation
- `src/launcher.c` / `launcher.h` - launch logic and session state
- `src/page.h` - embedded waiting page HTML
- `Makefile` - devkitPro build rules
- `scripts/setup-devkitpro.sh` - downloads and configures the Wii toolchain
- `.devcontainer/` - Codespaces environment

## Build flow

1. Open the repo in GitHub Codespaces.
2. Run the setup script:

   ```bash
   bash scripts/setup-devkitpro.sh
   ```

3. Build the project:

   ```bash
   make
   ```

4. Output will be a Wii homebrew ELF, which can then be converted to a bootable `.dol` or wrapped into your preferred loader flow.

## Important note

This is a template. It does not include proprietary Wiiflow internals or a private launcher API. In production, you will replace the example `launchGameById()` implementation with the actual Wiiflow-compatible or direct USB-loader launch method your Wii setup uses.

## Network flow

```text
Browser -> Wii app /launch -> Wii app validates command -> Wii app launches game -> Timer ends -> return to waiting screen
```

## Example launch request

```http
POST /launch
Content-Type: application/json

{
  "game": "game1",
  "minutes": 30
}
```

The app validates that `game1` is allowed, then launches the game and starts a session timer.

## Why this is a good starting point

It gives you a clean separation between:

- payment / admin logic in your web app or backend
- launch decisions in the Wii homebrew app
- the actual game launch in the Wii loader stack

That keeps the Wii app as the local authority for what is allowed to run.
