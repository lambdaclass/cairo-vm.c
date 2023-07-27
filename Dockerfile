FROM rust:latest

RUN apt update && \
    apt install cmake -y && \
    apt install cpputest -y && \
    apt install valgrind -y

WORKDIR /usr/cairo-vm_in_C

#RUN make docker_build_collections_lib

CMD bash
