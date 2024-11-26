FROM ubuntu

RUN apt-get update && apt-get -y install unminimize && (echo "y"; echo "y") | unminimize
RUN apt-get -y install gcc make perl man-db manpages curl bat less

# Install the Getopt::Std module for Perl
# Required for datalab/driver.pl
RUN perl -MCPAN -e 'install Getopt::Std'

WORKDIR /root

RUN echo "alias bat='batcat'" >> .bashrc && \
    echo "alias mycc='gcc -std=c11 -Wall -Werror -Wextra -pedantic'" >> .bashrc && \
    echo 'export MANPAGER="sh -c '\''col -bx | batcat -l man -p'\''"' >> .bashrc && \
    echo 'export MANROFFOPT="-c"' >> .bashrc && \
    echo 'export LESS="-R"' >> .bashrc

RUN curl -sS https://starship.rs/install.sh | sh -s -- --yes && \
    echo 'eval "$(starship init bash)"' >> .bashrc && \
    mkdir .config && echo '[container]\nformat = "🐳 "\n' >> .config/starship.toml

WORKDIR /csapp
CMD ["bash"]
