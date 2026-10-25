# ThePlayLoft Wii Kiosk Launcher

This repository is a starter project for a Wii homebrew kiosk that hosts a local waiting screen and accepts commands from a receptionist page.

It is designed to compile in a devkitPro environment and to serve as a base for a Wiiflow-compatible launcher.

## Requirements

- GitHub Codespaces or a Unix-like environment
- `git`, `make`, `wget`, `tar`, `unzip`
- devkitPro PowerPC toolchain for Wii homebrew

## Typical build command

```bash
make
```

This produces a homebrew ELF target that can be converted to a `.dol` or launched via your chosen loader.
