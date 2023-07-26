FROM rust:latest

RUN apt update && \
    apt install valgrind -y

WORKDIR /usr/cairo-vm_in_C
COPY . .

CMD bash
