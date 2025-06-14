FROM ubuntu:latest

RUN apt-get update -y && apt-get upgrade -y
RUN apt-get -y install nano
RUN apt-get -y install xorg

RUN apt-get -y install cmake
RUN apt-get -y install clang
RUN apt-get -y install ninja-build
RUN apt-get -y install freeglut3-dev

RUN useradd cmake-container -s /bin/bash -d /home/cmake-container -m && groupadd -g 963 docker && usermod -aG docker cmake-container
RUN echo 'export CC=clang CXX=clang++ CMAKE_GENERATOR=Ninja' >> /home/cmake-container/.bashrc
