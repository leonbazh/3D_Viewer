# Install
BIN = nk_num_parser

# Flags
CFLAGS += -std=c11 -Wall -Wextra -pedantic -O2 -g

SRC = main.c model_loader.c transformations.c misc_funcs.c
OBJ = $(SRC:.c=.o)

ifeq ($(OS),Windows_NT)
BIN := $(BIN).exe
LIBS = -mwindows -lglfw3 -lopengl32 -lm -lGLU32 -lGLEW32
else
	UNAME_S := $(shell uname -s)
	GLFW3 := $(shell pkg-config --libs glfw3)
	ifeq ($(UNAME_S),Darwin)
		LIBS := $(GLFW3) -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo -lm -lGLEW -L/usr/local/lib
	else
		LIBS = $(GLFW3) -lGL -lm -lGLU -lGLEW -lrt
	endif
endif

#web: 
#	emcc $(SRC) common/slices.c common/number_parser.c common/custom_strings/custom_string.c -DNKCD=NKC_GLFW -o index.html -s USE_GLFW=3 -Os

$(BIN):
	@mkdir -p bin
	rm -f bin/$(BIN) $(OBJS)
	$(CC) $(SRC) $(CFLAGS) -o bin/$(BIN) $(LIBS) && ./bin/$(BIN)
