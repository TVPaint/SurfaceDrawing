FROM debian:sid-slim as builder

RUN apt update && apt install --no-install-recommends -y git cmake build-essential qtbase5-dev ca-certificates && apt clean

ARG BRANCH=network

ADD https://api.github.com/repos/TVPDamienL/SurfaceDrawing/git/refs/heads/$BRANCH /tmp/version.json
RUN git clone -b $BRANCH https://github.com/TVPDamienL/SurfaceDrawing /tmp/SurfaceDrawing

RUN cd /tmp/SurfaceDrawing && \
    cmake . -DBUILD_CLIENT=OFF -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/install && \
    ls -l &&\
    cat Makefile &&\
    make install -j 4

RUN mkdir -p /install/server/lib && \
    for i in $(ldd /install/server/SurfaceDrawing_SERVER | grep -o "=> .* " | sed "s/=>//"); do \
        cp $i /install/server/lib/; \
    done


FROM debian:sid-slim

COPY --from=builder /install/server/SurfaceDrawing_SERVER /usr/bin
COPY --from=builder /install/server/lib/* /usr/lib/x86_64-linux-gnu/

RUN chmod +x /usr/bin/SurfaceDrawing_SERVER

EXPOSE 55666

ENTRYPOINT SurfaceDrawing_SERVER
