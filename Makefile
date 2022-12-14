#---------------------------------- VARIABLES ---------------------------------#

# Environment.

RM			= rm -rf
CC			= clang

# Executable / Libraries.

NAME		= ft_ping

# Color codes.

RESET		= \033[0m
GREEN		= \033[32m
YELLOW	= \033[33m

#--------------------------------- DIRECTORIES  -------------------------------#

# I/O Compilation.

SRCS_DIR			= srcs
OBJS_DIR			= objs

# Location of all header files used in the project.

INCS_DIR			= incs

# All the subdirectories used in the project.

OBJS_SUBDIRS	:= core
OBJS_SUBDIRS	+= display
OBJS_SUBDIRS	+= utils

#------------------------------------ FILES -----------------------------------#

# Used header at each compilation to check file integrity.

INCS	:= incs/enums.h
INCS	+= incs/display.h
INCS	+= incs/macros.h
INCS	+= incs/stdheaders.h
INCS	+= incs/utils.h

# Source files.

SRCS	:= core/main.c
SRCS	+= core/packets.c
SRCS	+= core/parsing.c
SRCS	+= core/setup.c
SRCS	+= core/signals.c
SRCS  += display/statistics.c
SRCS	+= utils/errors.c
SRCS	+= utils/maths.c
SRCS  += utils/network.c
SRCS	+= utils/time.c

#-------------------------------- MISCELANEOUS --------------------------------#

# Some tricks in order to get the makefile doing his job.

D_SRCS			= $(addsuffix /, $(SRCS_DIR))
D_OBJS			= $(addsuffix /, $(OBJS_DIR))
D_DEPS			= $(addsuffix /, $(SRCS_DIR))

C_DEPS			= $(addprefix $(D_OBJS), $(DEPS))
C_OBJS			= $(addprefix $(D_OBJS), $(OBJS))
C_INCS			= $(foreach include, $(INCS_DIR), -I$(include))

C_SUBDIRS		= $(foreach dir, $(OBJS_SUBDIRS), $(D_OBJS)$(dir))

#--------------------------------- COMPILATION --------------------------------#

# How files should be compiled.

OBJS		= $(SRCS:.c=.o)
DEPS 		= $(SRCS:.c=.d)

# Linked libraries at compile time.

LIBS			:= -lm

# Compilation flags.

CFLAGS	= $(C_INCS) -Wall -Wextra -Werror -MMD

#------------------------------------ RULES -----------------------------------#

# Redefinition of the implicit compilation rule to prompt some informations.

$(D_OBJS)%.o: $(D_SRCS)%.c $(INCS)
	@echo "$(YELLOW)      - Compiling :$(RESET)" $<
	@$(CC) $(CFLAGS) -c $< -o $@

# Implicit make rule simply using dependancies to compile our project.

all: $(OBJS_DIR) $(C_SUBDIRS) $(NAME)

$(NAME): $(C_OBJS)
	@echo "$(YELLOW)\n      - Building $(RESET)$(NAME) $(YELLOW)...\n$(RESET)"
	@$(CC) $(CFLAGS) -o $(NAME) $(C_OBJS) $(LIBS)
	@echo "$(GREEN)***   Project $(NAME) successfully compiled   ***\n$(RESET)"

# Rules used to create folders if they aren't already existing.

$(OBJS_DIR):
	@mkdir -p $(OBJS_DIR)

$(C_SUBDIRS):
	@mkdir -p $(C_SUBDIRS)

# Deleting all .o files.

clean:
	@echo "$(GREEN)***   Deleting all object from $(NAME)   ...   ***\n$(RESET)"
	@$(RM) $(C_OBJS)

# Deleting all executables and libraries after cleaning up all .o files.

fclean: clean
	@echo "$(GREEN)***   Deleting executable file from $(NAME)   ...   ***\n$(RESET)"
	@$(RM) $(NAME)

# Re-compile everything.

re: fclean all

# Avoid unexpected behaviour when regular files get the same name
# as the following variables.

.PHONY: all clean fclean re

-include ${C_DEPS}