FROM ubuntu:latest
LABEL authors="rozc"

RUN apt-get update && \
apt-get install -y build-essential cmake git

ENTRYPOINT ["top", "-b"]