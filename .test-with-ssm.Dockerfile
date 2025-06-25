FROM alpine:3.22

RUN apk add --no-cache \
    build-base \
    cmake \
    git

WORKDIR /work

RUN git clone --depth=1 https://github.com/openspur/ssm.git
RUN cd ssm \
  && ./configure \
  && make -j \
  && make install

COPY . /work/yp-spur
RUN cd yp-spur \
  && mkdir build \
  && cd build \
  && cmake .. \
  && make -j \
  && make install

ENV LD_LIBRARY_PATH=/usr/local/lib:/usr/local/lib64
RUN ldd /usr/local/bin/ypspur-coordinator

COPY --chmod=755 <<EOF /entrypoint.sh
#!/bin/sh

set -eu

ssm-coordinator &

sleep 1

ypspur-coordinator --without-device --param /work/yp-spur/test/vehicle.param &

sleep 1

lsssm | tee lsssm.log

if ! cat lsssm.log | grep spur_odometry >/dev/null; then
  echo "SSM not connected" >&2
  exit 1
fi
EOF

ENTRYPOINT ["/entrypoint.sh"]
