FROM ubuntu:20.04

RUN mkdir nxi
COPY qt_downloader /opt

RUN apt-get update \
    && apt-get install -y -qq --no-install-recommends \
        gcc-8 g++-8 cmake make \
        python3.7-dev python3-pip xml2 p7zip-full libxslt1-dev libgl1-mesa-dev \
        libssl-dev libxcursor-dev libxcomposite-dev libxdamage-dev libxrandr-dev \
        libfontconfig1-dev libxss-dev libwebp-dev libjsoncpp-dev libopus-dev libminizip-dev \
        libavutil-dev libavformat-dev libavcodec-dev libevent-dev \
        bison build-essential gperf flex ruby python libasound2-dev libbz2-dev libcap-dev \
        libcups2-dev libdrm-dev libegl1-mesa-dev libgcrypt11-dev libnss3-dev libpci-dev libpulse-dev libudev-dev \
        libxtst-dev gyp ninja-build

RUN pip3 install requests semantic_version lxml
RUN chmod +x /opt/qt_downloader
RUN /opt/qt_downloader linux desktop 5.13.1 gcc_64

WORKDIR /nxi