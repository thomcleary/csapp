#!/bin/bash

# Run the container
docker run -it --rm -v "$(pwd):/csapp" -w /csapp csapp
