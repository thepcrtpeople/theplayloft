# ThePlayLoft Wii Kiosk Launcher

This is a starter project for a Wii homebrew app that hosts a local webpage and accepts launch commands.

## Why this app exists

The booking/payment layer lives in your website or backend. The Wii app is the trusted local launcher that enforces which games are allowed to run and for how long.

## Runtime model

1. Homebrew app boots and shows waiting screen
2. Browser connects to the Wii over Wi-Fi
3. Receptionist clicks a launch button
4. Browser sends POST `/launch`
5. Wii app validates the request
6. Wii app launches the selected game
7. Timer expires and app returns to waiting state

## Production integration notes

Replace the stubbed `launchGameById()` function with the exact method your setup uses to launch a USB game with Wiiflow or a direct loader.
