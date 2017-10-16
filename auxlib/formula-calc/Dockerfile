FROM ubuntu:xenial

RUN apt-get -qq update && \
  apt-get install -y -qq --no-install-recommends sudo build-essential cmake git ca-certificates valgrind && \
  apt-get clean -qq && \
  rm -rf /var/lib/apt/lists/*

RUN cd /usr/src && \
  git clone -b release-1.8.0 https://github.com/google/googletest.git --depth=1 && \
  cd googletest && \
  mkdir build && cd build && \
  cmake .. && \
  make && make install && \
  rm -rf /usr/src/googletest/build

COPY ./ /usr/src/formula-calc/

RUN mkdir -p /usr/src/formula-calc/build && \
  cd /usr/src/formula-calc/build && \
  cmake .. && \
  make && (make test || (cat Testing/Temporary/LastTest.log; false)) && \
  sudo make install && \
  rm -rf /usr/src/formula-calc/build
