NAME = webserv

CMP = c++
FLAGS = -Wall -Wextra -Werror -std=c++17 -Iinclude

SCR_PARSE = $(addprefix parse/, read_config.cpp tokenize.cpp)
SCR_CLASS = $(addprefix class/, WebServ.cpp Server.cpp Location.cpp Socket.cpp SocketManager.cpp)
SCR_LOG = $(addprefix log/, logging.cpp)
SRC = $(addprefix src/, main.cpp $(SCR_PARSE) $(SCR_CLASS) $(SCR_LOG))
OBJ_DIR = _obj
OBJ = $(SRC:%.cpp=$(OBJ_DIR)/%.o)

all: info $(OBJ_DIR) $(NAME)

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)/src/
	@mkdir -p $(OBJ_DIR)/src/parse/
	@mkdir -p $(OBJ_DIR)/src/class/
	@mkdir -p $(OBJ_DIR)/src/log/

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