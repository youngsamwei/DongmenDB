FROM debian:buster-slim

#  Setup Aliyun mirror
RUN sed -i "s@http://deb.debian.org@http://mirrors.aliyun.com@g" /etc/apt/sources.list \
    && rm -Rf /var/lib/apt/lists/* \
    && apt-get update

# Install toolchain
RUN apt-get install -y g++ cmake unzip wget

# Copy src
RUN mkdir /workspace
COPY . /workspace/

# Cmake cache
RUN cd /workspace \
    && mkdir cmake-build-debug \
    && cd cmake-build-debug \
    && cmake ..

# Cmake build
RUN cd /workspace \
    && cmake --build cmake-build-debug

ENTRYPOINT /bin/bash