BIN = 3D_Viewer-1.0
VERSION = 1.0
DIST_DIR = dist
INSTALL_DIR = /usr/local/bin
DOCS_DIR = docs
GCOV_OUTPUT = gcov_report *.gc*

CFLAGS += -std=c11 -Wall -Wextra -pedantic -O2 -g
SRC = main.c ./backend/model_loader.c ./backend/transformations.c ./backend/misc_funcs.c
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

all: $(BIN)

$(BIN): $(OBJ)
	@mkdir -p bin
	$(CC) $(OBJ) $(CFLAGS) -o bin/$(BIN) $(LIBS)
	@echo "Build complete: bin/$(BIN)"

install: $(BIN)
	@mkdir -p $(INSTALL_DIR)
	install -m 755 bin/$(BIN) $(INSTALL_DIR)/$(BIN)
	@echo "Installed: $(INSTALL_DIR)/$(BIN)"

uninstall:
	rm -f $(INSTALL_DIR)/$(BIN)
	@echo "Uninstalled: $(INSTALL_DIR)/$(BIN)"

clean:
	rm -f $(OBJ) bin/$(BIN)
	rm -rf $(GCOV_OUTPUT)
	@echo "Cleaned build files"

dvi:
	doxygen Doxyfile
	@echo "Documentation generated in $(DOCS_DIR)"

dist:
	@mkdir -p $(DIST_DIR)
	@tar -cvzf $(DIST_DIR)/$(BIN)-$(VERSION).tar.gz $(SRC) Makefile
	@echo "Distribution package created: $(DIST_DIR)/$(BIN)-$(VERSION).tar.gz"

test:
	$(CC) ./tests/backend_tests.c ./backend/model_loader.c ./backend/transformations.c -lm -lcheck -lsubunit -o ./tests/test
	./tests/test
	@echo "Tests complete"

gcov_report: CFLAGS += -fprofile-arcs -ftest-coverage
gcov_report: clean $(OBJ)
	$(CC) $(OBJ) $(CFLAGS) -o bin/$(BIN) $(LIBS)
	./bin/$(BIN)
	mkdir -p $(GCOV_OUTPUT)
	gcov $(SRC)
	mv *.gcov $(GCOV_OUTPUT)
	@echo "Coverage report generated in $(GCOV_OUTPUT)"

