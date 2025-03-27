FROM ubuntu:22.04

RUN apt-get update && \
    apt-get install -y build-essential cmake pkgconf

RUN apt-get update && apt-get install -y --no-install-recommends --no-install-suggests \
    libx11-xcb1 \
    libxcb-xfixes0 \
    libxcb-shape0 \
    libxcb-shm0 \
    libxcb-randr0 \
    libxcb-image0 \
    libxcb-keysyms1 \
    libxcb-xtest0 \
    libdbus-1-3 \
    libglib2.0-0 \
    libgbm1 \
    libxfixes3 \
    libgl1 \
    libdrm2 \
    libgssapi-krb5-2 \
    openssl \
    ca-certificates \
    pkg-config \
    libegl-mesa0 \
    libsdl2-dev \
    g++-multilib \
    libcurl4-openssl-dev \
    libasound2 libasound2-plugins alsa alsa-utils alsa-oss \
    pulseaudio pulseaudio-utils \
    dbus dos2unix

WORKDIR /app

COPY zoom-linux-sdk/demo/ /app/demo/
COPY zoom-linux-sdk/speechsdk/ /app/speechsdk/

# Setup PulseAudio and Zoom config
RUN mkdir -p /var/run/dbus && \
    dbus-uuidgen > /var/lib/dbus/machine-id && \
    adduser root pulse-access && \
    adduser root audio && \
    rm -rf /var/run/pulse /var/lib/pulse /root/.config/pulse && \
    mkdir -p /root/.config/pulse && \
    cp -r /etc/pulse/* /root/.config/pulse/ && \
    mkdir -p /root/.config && \
    echo -e "[General]\nsystem.audio.type=default" > /root/.config/zoomus.conf

# Debug: Verify setup
RUN ls -la /app/speechsdk/lib/x64/

RUN cd /app/demo && rm -rf bin && rm -rf build && cmake -B build && cd build && make

WORKDIR /app/demo/bin

RUN echo '#!/bin/bash' > /app/demo/run.sh && \
    echo 'dbus-daemon --system &' >> /app/demo/run.sh && \
    echo 'pulseaudio --start --exit-idle-time=-1 --verbose &' >> /app/demo/run.sh && \
    echo 'sleep 2' >> /app/demo/run.sh && \
    echo 'pactl load-module module-null-sink sink_name=SpeakerOutput' >> /app/demo/run.sh && \
    echo 'pactl set-default-sink SpeakerOutput' >> /app/demo/run.sh && \
    echo 'pactl set-default-source SpeakerOutput.monitor' >> /app/demo/run.sh && \
    echo 'pactl info' >> /app/demo/run.sh && \
    echo './meetingSDKDemo' >> /app/demo/run.sh

RUN chmod +x /app/demo/run.sh

CMD ["/app/demo/run.sh"]