#syntax=docker/dockerfile:1
FROM ubuntu:20.04
LABEL authors="202422900232王冉恒"
# copy sourcefile to image
WORKDIR /root
COPY . .

# install python3 and protobuf compiler
RUN apt-get update
RUN apt-get install python3 -y
RUN apt-get install python3-pip -y

# install grpc plugin for python
RUN python3 -m pip install grpcio
RUN python3 -m pip install grpcio-tools