RESET		=	\033[0m
GREEN		=	\033[32m
YELLOW		=	\033[33m
BLUE		=	\033[34m
RED			=	\033[31m
MAGENTA		=	\033[35m
CYAN		=	\033[36m
UP			=	\033[A
CUT			=	\033[K

SRC_DIR		=	./src
INC_DIR		=	./inc
OBJ_DIR		=	./obj
OBJ_DIRS	=	./obj \
				./obj/main \
				./obj/Header \
				./obj/Request \
				./obj/Response \
				./obj/Server \
				./obj/Socket \
				./obj/utils \

FILES 		=	main/main \
				Header/AHeader \
				Header/RequestHeader \
				Header/ResponseHeader \
				Request/ARequest \
				Request/DELETERequest \
				Request/GETRequest \
				Request/POSTRequest \
				Response/Response \
				Server/Server \
				Socket/ClientSocket \
				utils/utils \

HEADER		=	Header/AHeader \
				Header/RequestHeader \
				Header/ResponseHeader \
				Request/DELETERequest \
				Request/GETRequest \
				Request/POSTRequest \
				Response/Response \
				Response/responseCodes \
				Server/Server \
				Socket/ClientSocket \
				utils/utils \

TEMPLATES	=	utils/utils \

INCL		=	$(addsuffix .hpp, $(addprefix $(INC_DIR)/, $(HEADER)))
TEMP		=	$(addsuffix .tpp, $(addprefix $(INC_DIR)/, $(TEMPLATES)))
SRCS 		= 	$(addsuffix .cpp, $(addprefix $(SRC_DIR)/, $(FILES)))
OBJS 		= 	$(addsuffix .o, $(addprefix $(OBJ_DIR)/, $(FILES)))

NAME		=	webserv
FLAGS		=	-Wall -Werror -Wextra -pedantic -std=c++98 -g
INC			=	-I $(INC_DIR)

all: $(NAME)

$(NAME): $(OBJS)
	@echo "$(YELLOW)Compiling [$(NAME)]...$(RESET)"
	@c++ $(FLAGS) -o $(NAME) $(OBJS)
	@echo "$(GREEN)Finished [$(NAME)]$(RESET)"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(INCL)
	@mkdir -p $(OBJ_DIRS)
	@echo "$(YELLOW)Compiling [$@]...$(RESET)"
	@c++ $(FLAGS) -I $(INC_DIR) -o $@ -c $<
	@printf "$(UP)$(CUT)"
	@echo "$(GREEN)Finished [$@]$(RESET)"
	@printf "$(UP)$(CUT)"

clean:
	@echo "$(BLUE)[$(NAME)] Deleting all objects $(RESET)"
	@rm -rf $(OBJ_DIR)

fclean: clean
	@echo "$(BLUE)Deleting $(NAME) $(RESET)"
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re

coffee:
	@clear
	@echo ""
	@echo "                   ("
	@echo "	                     )     ("
	@echo "               ___...(-------)-....___"
	@echo '           .-""       )    (          ""-.'
	@echo "      .-''''|-._             )         _.-|"
	@echo '     /  .--.|   `""---...........---""`   |'
	@echo "    /  /    |                             |"
	@echo "    |  |    |          :::      ::::::::  |"
	@echo "     \  \   |        :+:      :+:    :+:  |"
	@echo "      '\ '\ |      +:+ +:+         +:+    |"
	@echo "        '\ '|    +#+  +:+       +#+       |"
	@echo "        _/ /\  +#+#+#+#+#+   +#+          /"
	@echo "       (__/  \      #+#    #+#           /"
	@echo '    _..---""` \    ###   ########.fr    /`""---.._'
	@echo " .-'           \                       /          '-."
	@echo ":               '-.__             __.-'              :"
	@echo ':                  ) ""---...---"" (                :'
	@echo "\'._                '"--...___...--"'              _.'"
	@echo '   \""--..__                              __..--""/'
	@echo "     '._     """----.....______.....----"""         _.'"
	@echo '         ""--..,,_____            _____,,..--"""'''
	@echo '                      """------"""'
	@sleep 0.5
	@clear
	@echo ""
	@echo "                 ("
	@echo "	                  )      ("
	@echo "               ___..(.------)--....___"
	@echo '           .-""       )   (           ""-.'
	@echo "      .-''''|-._      (       )        _.-|"
	@echo '     /  .--.|   `""---...........---""`   |'
	@echo "    /  /    |                             |"
	@echo "    |  |    |          :::      ::::::::  |"
	@echo "     \  \   |        :+:      :+:    :+:  |"
	@echo "      '\ '\ |      +:+ +:+         +:+    |"
	@echo "        '\ '|    +#+  +:+       +#+       |"
	@echo "        _/ /\  +#+#+#+#+#+   +#+          /"
	@echo "       (__/  \      #+#    #+#           /"
	@echo '    _..---""` \    ###   ########.fr    /`""---.._'
	@echo " .-'           \                       /          '-."
	@echo ":               '-.__             __.-'              :"
	@echo ':                  ) ""---...---"" (                :'
	@echo "\'._                '"--...___...--"'              _.'"
	@echo '   \""--..__                              __..--""/'
	@echo "     '._     """----.....______.....----"""         _.'"
	@echo '         ""--..,,_____            _____,,..--"""'''
	@echo '                      """------"""'
	@sleep 0.5
	@clear
	@echo ""
	@echo "               ("
	@echo "	                  )     ("
	@echo "               ___..(.------)--....___"
	@echo '           .-""      )    (           ""-.'
	@echo "      .-''''|-._      (       )        _.-|"
	@echo '     /  .--.|   `""---...........---""`   |'
	@echo "    /  /    |                             |"
	@echo "    |  |    |          :::      ::::::::  |"
	@echo "     \  \   |        :+:      :+:    :+:  |"
	@echo "      '\ '\ |      +:+ +:+         +:+    |"
	@echo "        '\ '|    +#+  +:+       +#+       |"
	@echo "        _/ /\  +#+#+#+#+#+   +#+          /"
	@echo "       (__/  \      #+#    #+#           /"
	@echo '    _..---""` \    ###   ########.fr    /`""---.._'
	@echo " .-'           \                       /          '-."
	@echo ":               '-.__             __.-'              :"
	@echo ':                  ) ""---...---"" (                :'
	@echo "\'._                '"--...___...--"'              _.'"
	@echo '   \""--..__                              __..--""/'
	@echo "     '._     """----.....______.....----"""         _.'"
	@echo '         ""--..,,_____            _____,,..--"""'''
	@echo '                      """------"""'
	@sleep 0.5
	@clear
	@echo ""
	@echo "             (         ) "
	@echo "	              )        ("
	@echo "               ___)...----)----....___"
	@echo '           .-""      )    (           ""-.'
	@echo "      .-''''|-._      (       )        _.-|"
	@echo '     /  .--.|   `""---...........---""`   |'
	@echo "    /  /    |                             |"
	@echo "    |  |    |          :::      ::::::::  |"
	@echo "     \  \   |        :+:      :+:    :+:  |"
	@echo "      '\ '\ |      +:+ +:+         +:+    |"
	@echo "        '\ '|    +#+  +:+       +#+       |"
	@echo "        _/ /\  +#+#+#+#+#+   +#+          /"
	@echo "       (__/  \      #+#    #+#           /"
	@echo '    _..---""` \    ###   ########.fr    /`""---.._'
	@echo " .-'           \                       /          '-."
	@echo ":               '-.__             __.-'              :"
	@echo ':                  ) ""---...---"" (                :'
	@echo "\'._                '"--...___...--"'              _.'"
	@echo '   \""--..__                              __..--""/'
	@echo "     '._     """----.....______.....----"""         _.'"
	@echo '         ""--..,,_____            _____,,..--"""'''
	@echo '                      """------"""'
	@sleep 0.5
	@clear
	@echo ""
	@echo "               ("
	@echo "	                  )     ("
	@echo "               ___..(.------)--....___"
	@echo '           .-""      )    (           ""-.'
	@echo "      .-''''|-._      (       )        _.-|"
	@echo '     /  .--.|   `""---...........---""`   |'
	@echo "    /  /    |                             |"
	@echo "    |  |    |          :::      ::::::::  |"
	@echo "     \  \   |        :+:      :+:    :+:  |"
	@echo "      '\ '\ |      +:+ +:+         +:+    |"
	@echo "        '\ '|    +#+  +:+       +#+       |"
	@echo "        _/ /\  +#+#+#+#+#+   +#+          /"
	@echo "       (__/  \      #+#    #+#           /"
	@echo '    _..---""` \    ###   ########.fr    /`""---.._'
	@echo " .-'           \                       /          '-."
	@echo ":               '-.__             __.-'              :"
	@echo ':                  ) ""---...---"" (                :'
	@echo "\'._                '"--...___...--"'              _.'"
	@echo '   \""--..__                              __..--""/'
	@echo "     '._     """----.....______.....----"""         _.'"
	@echo '         ""--..,,_____            _____,,..--"""'''
	@echo '                      """------"""'
	@sleep 0.5
	@clear
	@echo ""
	@echo "                 ("
	@echo "	                  )      ("
	@echo "               ___..(.------)--....___"
	@echo '           .-""       )   (           ""-.'
	@echo "      .-''''|-._      (       )        _.-|"
	@echo '     /  .--.|   `""---...........---""`   |'
	@echo "    /  /    |                             |"
	@echo "    |  |    |          :::      ::::::::  |"
	@echo "     \  \   |        :+:      :+:    :+:  |"
	@echo "      '\ '\ |      +:+ +:+         +:+    |"
	@echo "        '\ '|    +#+  +:+       +#+       |"
	@echo "        _/ /\  +#+#+#+#+#+   +#+          /"
	@echo "       (__/  \      #+#    #+#           /"
	@echo '    _..---""` \    ###   ########.fr    /`""---.._'
	@echo " .-'           \                       /          '-."
	@echo ":               '-.__             __.-'              :"
	@echo ':                  ) ""---...---"" (                :'
	@echo "\'._                '"--...___...--"'              _.'"
	@echo '   \""--..__                              __..--""/'
	@echo "     '._     """----.....______.....----"""         _.'"
	@echo '         ""--..,,_____            _____,,..--"""'''
	@echo '                      """------"""'
