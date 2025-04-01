# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ncastell <ncastell@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/03/22 20:09:04 by sguzman           #+#    #+#              #
#    Updated: 2025/04/01 11:33:01 by sguzman          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

################################################################################
#                                     CONFIG                                   #
################################################################################

NAME        = ircserv
NAME_BOT    = ircbot
CC          = c++
CFLAGS      = -Wall -Wextra -Werror -std=c++98
DFLAGS      = -MMD -MF $(@:.o=.d)

ifneq ($(SANITIZER),)
    CFLAGS += -fsanitize=$(SANITIZER)
endif

################################################################################
#                                 PROGRAM'S SRCS                               #
################################################################################

SRCS_PATH    = ./src
OBJS_PATH    = ./build
INCLUDE_PATH = ./include

SRCS         = channel.cc \
			   channelmanager.cc \
			   client.cc \
			   clientmanager.cc \
			   command.cc \
			   event.cc \
			   eventmanager.cc \
			   listener.cc \
			   logging.cc \
			   parser.cc \
			   server.cc \
			   tool.cc
BOT          = bot.cc  \
			   logging.cc \
			   tool.cc
MAIN         = main.cc

################################################################################
#                                  Makefile  objs                              #
################################################################################

OBJS        = $(addprefix $(OBJS_PATH)/, ${SRCS:.cc=.o})
OBJS_BOT	= $(addprefix $(OBJS_PATH)/, ${BOT:.cc=.o})
OBJS_MAIN   = $(addprefix $(OBJS_PATH)/, ${MAIN:.cc=.o})

DEPS        = $(addprefix $(OBJS_PATH)/, ${SRCS:.cc=.d})
DEPS_BOT    = $(addprefix $(OBJS_PATH)/, ${BOT:.cc=.d})
DEPS_MAIN   = $(addprefix $(OBJS_PATH)/, ${MAIN:.cc=.d})

################################################################################
#                                 Makefile logic                               #
################################################################################

SHELL := /bin/bash
RED    = \033[0;31m
GREEN  = \033[0;32m
YELLOW = \033[0;33m
BLUE   = \033[0;34m
PURPLE = \033[0;35m
CYAN   = \033[0;36m
RESET  = \033[m

define compile
    printf "%b%-46b" "$(BLUE)$(2) " "$(CYAN)$(@F)$(RESET)"; \
    OUTPUT=$$($(1) 2>&1); \
    RESULT=$$?; \
    if [ $$RESULT -ne 0 ]; then \
        printf "%b\n" "$(RED)[✗]$(RESET)"; \
	printf "%b\n" "$$OUTPUT"; \
    else  \
        printf "%b\n" "$(GREEN)[✓]$(RESET)"; \
    fi; \
    [ $$RESULT -eq 0 ]
endef

################################################################################
#                                 Makefile rules                               #
################################################################################

all: banner $(NAME)

bonus: banner $(NAME_BOT)

banner:
	@printf "%b" "$(GREEN)\n"
	@echo
	@echo "   _______  _____"
	@echo "  /  _/ _ \/ ___/"
	@echo " _/ // , _/ /__  "
	@echo "/___/_/|_|\___/  "
	@echo
	@printf "%b" "$(YELLOW)Name:           $(CYAN)$(NAME)\n$(RESET)"
	@printf "%b" "$(YELLOW)CC:             $(CYAN)$(CC)\n$(RESET)"
	@printf "%b" "$(YELLOW)Flags:          $(CYAN)$(CFLAGS)\n$(RESET)"
	@printf "%b" "\n$(RESET)"

-include $(DEPS) $(DEPS_MAIN)

$(NAME): $(OBJS) $(OBJS_MAIN)
	@$(call compile,$(CC) $(CFLAGS) $^ -o $@)

$(NAME_BOT): $(OBJS_BOT)
	@$(call compile,$(CC) $(CFLAGS) $^ -o $@)

$(OBJS_PATH)/%.o: $(SRCS_PATH)/%.cc
	@mkdir -p $(dir $@)
	@$(call compile,$(CC) $(CFLAGS) $(DFLAGS) -c $< -o $@ -I $(INCLUDE_PATH))

clean: banner
	@rm -rf $(OBJS_PATH)
	@printf "%-53b%b" "$(CYAN)$(@):" "$(GREEN)[✓]$(RESET)\n"

fclean: banner clean
	@rm -rf $(NAME)
	@rm -rf $(NAME_BOT)
	@printf "%-53b%b" "$(CYAN)$(@):" "$(GREEN)[✓]$(RESET)\n"

re: fclean all

.PHONY: all bonus banner clean fclean re 
