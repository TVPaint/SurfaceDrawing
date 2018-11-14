FROM debian:sid-slim as builder

RUN apt update && apt install --no-install-recommends -y git cmake build-essential qtbase5-dev ca-certificates && apt clean

RUN  echo "w" && git clone -b network https://github.com/TVPDamienL/SurfaceDrawing /tmp/SurfaceDrawing

RUN cd /tmp/SurfaceDrawing && \
    cmake . -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/install && \
    make install -j 4

FROM debian:sid-slim

COPY --from=builder /install/server/SurfaceDrawing_SERVER /usr/bin
COPY --from=builder /usr/lib/x86_64-linux-gnu/libQt5Widgets.so.5 /usr/lib/x86_64-linux-gnu/libQt5Widgets.so.5
COPY --from=builder /usr/lib/x86_64-linux-gnu/libQt5Network.so.5 /usr/lib/x86_64-linux-gnu/libQt5Network.so.5
COPY --from=builder /usr/lib/x86_64-linux-gnu/libQt5Gui.so.5 /usr/lib/x86_64-linux-gnu/libQt5Gui.so.5
COPY --from=builder /usr/lib/x86_64-linux-gnu/libQt5Core.so.5 /usr/lib/x86_64-linux-gnu/libQt5Core.so.5
COPY --from=builder /usr/lib/x86_64-linux-gnu/libGL.so.1 /usr/lib/x86_64-linux-gnu/libGL.so.1
COPY --from=builder /usr/lib/x86_64-linux-gnu/libpng16.so.16 /usr/lib/x86_64-linux-gnu/libpng16.so.16
COPY --from=builder /usr/lib/x86_64-linux-gnu/libharfbuzz.so.0 /usr/lib/x86_64-linux-gnu/libharfbuzz.so.0
COPY --from=builder /usr/lib/x86_64-linux-gnu/libicui18n.so.60 /usr/lib/x86_64-linux-gnu/libicui18n.so.60
COPY --from=builder /usr/lib/x86_64-linux-gnu/libicuuc.so.60 /usr/lib/x86_64-linux-gnu/libicuuc.so.60
COPY --from=builder /usr/lib/x86_64-linux-gnu/libpcre2-16.so.0 /usr/lib/x86_64-linux-gnu/libpcre2-16.so.0
COPY --from=builder /usr/lib/x86_64-linux-gnu/libdouble-conversion.so.1 /usr/lib/x86_64-linux-gnu/libdouble-conversion.so.1
COPY --from=builder /usr/lib/x86_64-linux-gnu/libglib-2.0.so.0 /usr/lib/x86_64-linux-gnu/libglib-2.0.so.0
COPY --from=builder /usr/lib/x86_64-linux-gnu/libGLX.so.0 /usr/lib/x86_64-linux-gnu/libGLX.so.0
COPY --from=builder /usr/lib/x86_64-linux-gnu/libGLdispatch.so.0 /usr/lib/x86_64-linux-gnu/libGLdispatch.so.0
COPY --from=builder /usr/lib/x86_64-linux-gnu/libfreetype.so.6 /usr/lib/x86_64-linux-gnu/libfreetype.so.6
COPY --from=builder /usr/lib/x86_64-linux-gnu/libgraphite2.so.3 /usr/lib/x86_64-linux-gnu/libgraphite2.so.3
COPY --from=builder /usr/lib/x86_64-linux-gnu/libicudata.so.60 /usr/lib/x86_64-linux-gnu/libicudata.so.60
COPY --from=builder /usr/lib/x86_64-linux-gnu/libX11.so.6 /usr/lib/x86_64-linux-gnu/libX11.so.6
COPY --from=builder /usr/lib/x86_64-linux-gnu/libXext.so.6 /usr/lib/x86_64-linux-gnu/libXext.so.6
COPY --from=builder /usr/lib/x86_64-linux-gnu/libxcb.so.1 /usr/lib/x86_64-linux-gnu/libxcb.so.1
COPY --from=builder /usr/lib/x86_64-linux-gnu/libXau.so.6 /usr/lib/x86_64-linux-gnu/libXau.so.6
COPY --from=builder /usr/lib/x86_64-linux-gnu/libXdmcp.so.6 /usr/lib/x86_64-linux-gnu/libXdmcp.so.6
COPY --from=builder /usr/lib/x86_64-linux-gnu/libbsd.so.0 /usr/lib/x86_64-linux-gnu/libbsd.so.0



RUN chmod +x /usr/bin/SurfaceDrawing_SERVER

EXPOSE 55666

ENTRYPOINT SurfaceDrawing_SERVER
