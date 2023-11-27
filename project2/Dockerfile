FROM ubuntu:20.04

# Set environment variables
ARG DEBIAN_FRONTEND=noninteractive
ARG TZ=Asia/Taipei

# Set path
ENV PATH="/build/bin:${PATH}"

# Install required packages
RUN apt update && apt install -y \
    build-essential \
    wget \
    git \
    libx11-dev \
    tcl-dev \
    tk-dev \
    flex \
    swig \
    bison \
    libjpeg-dev \
    libboost-all-dev \
    cmake \
    libtool \
    zlib1g-dev

# Download RePlAce
WORKDIR /root
RUN git clone --recursive --branch standalone https://github.com/The-OpenROAD-Project/RePlAce.git

# Build RePlAce
WORKDIR /root/RePlAce
RUN mkdir build
WORKDIR /root/RePlAce/build
RUN cmake -DCMAKE_INSTALL_PREFIX=/build .. \
    && make \
    && make install
