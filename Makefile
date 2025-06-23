# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: nmonzon <nmonzon@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/05/13 16:17:26 by nmonzon           #+#    #+#              #
<<<<<<< HEAD
#    Updated: 2025/06/06 12:28:11 by nmonzon          ###   ########.fr        #
=======
#    Updated: 2025/06/23 10:29:32 by jgraf            ###   ########.fr        #
>>>>>>> master
#                                                                              #
# **************************************************************************** #

NAME = webserv

CMP = c++
FLAGS = -Wall -Wextra -Werror -std=c++17 -Iinclude

<<<<<<< HEAD
SRC = src/main.cpp \
	  src/parse/read_config.cpp \
	  src/class/WebServ.cpp src/class/Server.cpp src/class/Location.cpp src/class/SocketManager.cpp src/class/Socket.cpp
=======
SCR_PARSE = $(addprefix parse/, read_config.cpp tokenize.cpp)
SCR_CLASS = $(addprefix class/, WebServ.cpp Server.cpp Location.cpp Socket.cpp SocketManager.cpp)
SCR_LOG = $(addprefix log/, logging.cpp)
SRC = $(addprefix src/, main.cpp $(SCR_PARSE) $(SCR_CLASS) $(SCR_LOG))
>>>>>>> master
OBJ_DIR = _obj
OBJ = $(SRC:%.cpp=$(OBJ_DIR)/%.o)

all: info $(OBJ_DIR) $(NAME)

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)/src/
	@mkdir -p $(OBJ_DIR)/src/parse/
	@mkdir -p $(OBJ_DIR)/src/class/
<<<<<<< HEAD
=======
	@mkdir -p $(OBJ_DIR)/src/log/
>>>>>>> master

$(OBJ_DIR)/%.o: %.cpp
	@$(CMP) $(FLAGS) -c $< -o $@

$(NAME): $(OBJ)
	@$(CMP) $(FLAGS) $(OBJ) -o $(NAME)
	@printf "Succesfully compiled $(NAME).\n"

info:
	@printf "Compiling $(NAME)...\n"

clean:
	@printf "Cleaned object files.\n"
	@rm -rf $(OBJ_DIR)

fclean: clean
	@printf "Cleaned executable.\n"
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re info