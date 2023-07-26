FROM rust:latest

RUN apt update && \
    apt install cmake -y && \
    apt install cpputest -y && \
    apt install valgrind -y

WORKDIR /usr/cairo-vm_in_C
COPY . .

RUN rm -r Collections-C && \
    make docker_build_collections_lib

CMD bash
