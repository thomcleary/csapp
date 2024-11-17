#!/bin/bash

# Check if the image already exists
if ! docker image inspect csapp-datalab > /dev/null 2>&1; then
  # Build the image if it doesn't exist
  docker build -t csapp-datalab .
fi

# Run the container
docker run --rm -v "$(pwd):/datalab" csapp-datalab perl /datalab/driver.pl
