# Use the official Ubuntu base image
FROM ubuntu:latest

# Update the package list and install GCC
RUN apt update && apt install -y gcc make perl

# Install the Getopt::Std module for Perl
# Required for driver.pl
RUN perl -MCPAN -e 'install Getopt::Std'

# Set the default working directory (optional)
WORKDIR /datalab

# Default command to run when the container starts
CMD ["bash"]
