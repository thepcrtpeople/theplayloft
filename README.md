# ThePlayLoft Wii Kiosk

This repository contains a Wii homebrew kiosk app that acts as a payment/session gate for a public game booth.

The app does not try to launch a game by itself. Instead:

- the Wii boots into a full-screen waiting page
- the receptionist opens the kiosk page locally over Wi-Fi
- payment/session info is entered and approved
- when the request is approved, the app exits back to the Wii menu/home screen
- the user then manually launches Wiiflow or the game they want

This is intentionally designed to be safe and simple.

## Behavior

1. User sees a waiting screen
2. Receptionist approves session/payment
3. App returns to the Wii System Menu
4. User manually launches the game through Wiiflow

## Safety

This project intentionally avoids direct binary title launch logic. That is risky and unnecessary for your use case.

Instead, the app acts as a lock / gate screen and exits cleanly when the session is approved.

## Build

```bash
bash scripts/setup-devkitpro.sh
make
```
