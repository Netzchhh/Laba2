FROM gcc:14

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y \
    cmake \
    git \
    build-essential \
    libgtest-dev \
    && rm -rf /var/lib/apt/lists/* \
    && cd /usr/src/googletest && mkdir build && cd build && cmake .. && make && cp lib/*.a /usr/lib

WORKDIR /workspace

COPY . .

RUN rm -rf build && mkdir build && cd build && cmake .. && make -j$(nproc)

CMD ["/bin/bash", "-c", "cd build && ctest --output-on-failure || true; ./main"]