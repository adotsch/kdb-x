FROM alpine:3.19

RUN apk add --no-cache curl xz libc-dev mingw-w64-binutils make patchelf

# Install Zig 0.13.0
RUN curl -L https://ziglang.org/download/0.13.0/zig-linux-x86_64-0.13.0.tar.xz | \
    tar -xJ -C /usr/local && \
    ln -s /usr/local/zig-linux-x86_64-0.13.0/zig /usr/local/bin/zig

WORKDIR /app
