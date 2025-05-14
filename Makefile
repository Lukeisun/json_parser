SRCS += \
	main.cpp \
	Lexer.cpp \
	Parser.cpp
CFLAGS = --std=c++23 -g -fsanitize=address

all:
	g++ -lfmt ${CFLAGS} -I. ${SRCS}

gen_cc:
	bear -- g++ -lfmt ${CFLAGS} ${SRCS}

