ARG UBUNTU_VERSION=bionic
FROM ubuntu:${UBUNTU_VERSION}

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get -qq update \
  && apt-get install -y --no-install-recommends \
    build-essential \
    cmake \
    git \
    libgtest-dev
# don't clean apt-cache to reuse later

ENV CTEST_OUTPUT_ON_FAILURE=1

RUN mkdir -p /tmp/gtest \
  && cd /tmp/gtest \
  && cmake /usr/src/gtest \
  && make -j \
  && cp $(find . -name "*.a") /usr/lib/ \
  && rm -rf /tmp/gtest

ENV CTEST_OUTPUT_ON_FAILURE=1

COPY ./ /yp-spur
RUN mkdir -p /yp-spur/build
WORKDIR /yp-spur/build
RUN cmake .. && make -j && make test && make install
# check that ypspur-interpreter is built without readline
RUN ldd ypspur-interpreter | grep libreadline.so; test $? -eq 1

RUN apt-get install -y --no-install-recommends libreadline-dev

RUN mkdir -p /yp-spur/build-readline
WORKDIR /yp-spur/build-readline
RUN cmake .. && make -j && make test && make install && ldconfig
# check that ypspur-interpreter is built with readline
RUN ldd ypspur-interpreter | grep libreadline.so
RUN ypspur-coordinator --version
