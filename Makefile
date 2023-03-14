##########   BASE   ##########
# File name
NAME = ircserv

# Sources
SRCS = ./src/20_class/server.cpp
SRCS += ./src/30_Parsing_argv/parsing.cpp
SRCS += ./src/main.cpp


# Header/s
HEADER = ./src/10_tools/colors.hpp
HEADER += ./src/20_class/server.hpp
HEADER += ./src/30_Parsing_argv/parsing.hpp
HEADER += ./src/main.hpp


OBJS = ${SRCS:.cpp=.o}

# Compilateurs
CC = c++

CFLAGS = -g -Wall -Wextra -Werror -std=c++98

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
