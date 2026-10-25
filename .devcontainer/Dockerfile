FROM mcr.microsoft.com/devcontainers/base:ubuntu

RUN apt-get update \
    && DEBIAN_FRONTEND=noninteractive apt-get install -y \
        build-essential \
        git \
        wget \
        curl \
        unzip \
        ca-certificates \
        pkg-config \
        libzstd-dev \
        python3 \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /workspaces
CMD ["bash"]
