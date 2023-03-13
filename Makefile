##########   BASE   ##########
# File name
NAME = ircserv

# Sources
#SRCS = ./server.cpp
SRCS = ./server_old.cpp


# Header/s
#HEADER = ./server.hpp

OBJS = ${SRCS:.cpp=.o}

# Compilateurs
CC = c++

CFLAGS = -g -Wall -Wextra #-Werror #-std=c++98

##########   RULES   ##########
all: $(NAME)

$(NAME): $(OBJS)
	${CC} ${SRCS} -o ${NAME} ${CFLAGS}

%.o: %.cpp $(HEADER)
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f ${NAME}

re: fclean all

.PHONY: all clean fclean re
