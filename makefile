CC        := gcc
CFLAGS    := -std=c11 -Wall
SRC       := main.c fila.c
OBJ       := $(SRC:.c=.o)
TARGET    := programa-fila.exe

.PHONY: all clean run

# Alvo padrão: compila tudo
all: $(TARGET)

# Linka o executável com todos os objetos
$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET)

# Compila cada .c em .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Removedor de arquivos objeto e executável
clean:
	del $(OBJ) $(TARGET)  # Windows
	# rm -f $(OBJ) $(TARGET)  # Linux/macOS

# Alvo run: build + execução
run: all
	@echo Executando $(TARGET)...
	@./$(TARGET)
