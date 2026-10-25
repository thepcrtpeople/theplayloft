#!/usr/bin/env bash
set -euo pipefail

if [[ -z "${DEVKITPPC:-}" ]]; then
  echo "Installing devkitPro toolchain..."
  export DEVKITPPC="/opt/devkitpro"
fi

mkdir -p "$DEVKITPPC"

if [[ ! -d "$DEVKITPPC/devkitPPC" ]]; then
  echo "Downloading devkitPro pacman installer..."
  wget -q https://github.com/devkitPro/pacman/releases/latest/download/devkitpro-pacman.amd64.deb -O /tmp/devkitpro-pacman.deb
  apt-get update
  apt-get install -y /tmp/devkitpro-pacman.deb
fi

if [[ ! -d "$DEVKITPPC/devkitPPC" ]]; then
  echo "Installing toolchain packages..."
  /usr/bin/pacman -S --noconfirm --needed devkitPPC
fi

if [[ -d "$DEVKITPPC/devkitPPC" ]]; then
  echo "devkitPro installed at $DEVKITPPC/devkitPPC"
else
  echo "devkitPro installation did not complete; please install manually."
  exit 1
fi

echo "Setup complete. Use:"
echo "  export DEVKITPPC=/opt/devkitpro"
echo "  make"
