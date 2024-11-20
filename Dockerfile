# Use the official Ubuntu base image
FROM ubuntu:latest

# Update the package list and install GCC
RUN apt update && apt install -y gcc gdb make perl

# Install the Getopt::Std module for Perl
# Required for datalab/driver.pl
RUN perl -MCPAN -e 'install Getopt::Std'

# Default command to run when the container starts
CMD ["bash"]
