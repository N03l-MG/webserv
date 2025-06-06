# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jgraf <jgraf@student.42heilbronn.de>       +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/05/13 16:17:26 by nmonzon           #+#    #+#              #
#    Updated: 2025/06/06 15:04:24 by jgraf            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = webserv

CMP = c++
FLAGS = -Wall -Wextra -Werror -std=c++17 -Iinclude

SCR_PARSE = $(addprefix parse/, read_config.cpp tokenize.cpp)
SCR_CLASS = $(addprefix class/, WebServ.cpp Server.cpp Location.cpp)
SRC = $(addprefix src/, main.cpp $(SCR_PARSE) $(SCR_CLASS))
OBJ_DIR = _obj
OBJ = $(SRC:%.cpp=$(OBJ_DIR)/%.o)

all: info $(OBJ_DIR) $(NAME)

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)/src/
	@mkdir -p $(OBJ_DIR)/src/parse/
	@mkdir -p $(OBJ_DIR)/src/class/

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