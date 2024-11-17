#!/bin/bash

docker build -t csapp-datalab . && \
docker run --rm -v "$(pwd):/datalab" csapp-datalab perl /datalab/driver.pl
