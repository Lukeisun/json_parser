SRCS += \
	main.cpp \
	Lexer.cpp \
	Parser.cpp
OBJECTS=$(patsubst %.cpp,%.o,$(SRCS))


CFLAGS = --std=c++23 -g -fsanitize=address
OUT=jsonp

all: $(OUT)

$(OUT):$(OBJECTS)
	g++ ${CFLAGS} -ljsoncpp -lfmt -o $@ $^

%.o:%.cpp
	g++ ${CFLAGS} -c -o $@ $^

clean:
	rm -rf $(OUT) $(OBJECTS)

