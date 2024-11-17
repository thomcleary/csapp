#!/bin/bash

docker build -t csapp-datalab . && \
docker run -it --rm -v "$(pwd):/datalab" csapp-datalab
