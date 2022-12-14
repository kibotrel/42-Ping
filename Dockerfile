FROM silkeh/clang:14-stretch


COPY ./srcs /ft_ping/srcs
COPY ./incs /ft_ping/incs
COPY Makefile /ft_ping/Makefile

WORKDIR /ft_ping/

RUN echo "stty -echoctl" >> ~/.bashrc
RUN apt-get update -y
RUN apt-get upgrade -y
RUN apt-get install iputils-ping valgrind -y
RUN make re

CMD ["bash"]
