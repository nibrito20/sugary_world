TARGET = sugary_world

SRC = main.c fase1.c fase2.c

# Diretórios da Raylib (Homebrew)
INCLUDE_PATH = /opt/homebrew/include
LIB_PATH = /opt/homebrew/lib

# Flags de compilação
CFLAGS = -I$(INCLUDE_PATH) -Iinclude
LDFLAGS = -L$(LIB_PATH) -lraylib -lm -ldl -lpthread -framework OpenGL -framework Cocoa -framework IOKit

all:
	gcc $(SRC) -o $(TARGET) $(CFLAGS) $(LDFLAGS)

run: all
	./$(TARGET)

clean:
	rm -f $(TARGET)
