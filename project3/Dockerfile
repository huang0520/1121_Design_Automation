FROM ubuntu:20.04

# Set environment variables
ARG DEBIAN_FRONTEND=noninteractive
ARG TZ=Asia/Taipei

# Set path
ENV PATH="/build/bin:${PATH}"

RUN apt update && apt install -y \
    build-essential \
    git \
    automake

WORKDIR /root

RUN git clone https://github.com/ivmai/cudd.git

WORKDIR /root/cudd

RUN touch configure.ac aclocal.m4 configure Makefile.am Makefile.in
RUN ./configure --enable-silent-rules --enable-shared --enable-obj
RUN make -j4 check
