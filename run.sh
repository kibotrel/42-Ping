#/usr/bin/env bash

docker build -t ft_ping-docker .
docker run --network host -it ft_ping-docker:latest
