# Variables
NAME		:=	minirc
FILES			:=	main.cpp \
					Socket.cpp \
					Server.cpp \
					ServerService.cpp \
					ServerException.cpp \
					EventHandler.cpp \
					Poller.cpp

all:
	@c++ -Wall -Wextra -Werror -std=c++98 $(FILES) -I ./ -o $(NAME)

clean:
	@echo "Nothing to be done."

fclean:
	@rm $(NAME)

re: fclean all