FROM ubuntu:20.04

ARG DEBIAN_FRONTEND=noninteractive

RUN apt-get -y update
RUN apt-get -y install build-essential
RUN apt-get -y install clang
RUN apt-get -y install clang-format
RUN apt-get -y install clang-tidy
RUN apt-get -y install cmake
RUN apt-get -y install golang
RUN apt-get -y install python-is-python3
RUN apt-get -y install python-dev-is-python3
RUN apt-get -y install python3-pip
RUN apt-get -y install git
RUN apt-get -y upgrade
RUN go get github.com/bazelbuild/bazelisk
RUN ln -s /root/go/bin/bazelisk /usr/local/bin/bazel
RUN pip install -U pip conan cpplint
