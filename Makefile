# Compilador
CC = gcc

# Flags de compilação
CFLAGS = -std=c99 -Wall -Wextra -O2

# Bibliotecas
LIBS = -lmysqlclient -lcurl

# Diretórios
SRC_DIR = src
BIN_DIR = .
OBJ_DIR = obj

# Alvo principal
TARGET = sge-integration

# Arquivos fonte
SOURCES = $(SRC_DIR)/main.c $(SRC_DIR)/database.c $(SRC_DIR)/gemini_api.c $(SRC_DIR)/ui.c
OBJECTS = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SOURCES))

# Alvo padrão
all: $(TARGET)

# Criar diretório de objetos
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Compilar arquivo objeto
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Linkar programa final
$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(BIN_DIR)/$(TARGET) $(OBJECTS) $(LIBS)
	@echo "✅ Compilação concluída: $(TARGET)"

# Limpar arquivos compilados
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)/$(TARGET)
	@echo "✅ Limpeza concluída"

# Executar programa
run: $(TARGET)
	./$(TARGET)

# Recompilar tudo
rebuild: clean all

# Help
help:
	@echo "Comandos disponíveis:"
	@echo "  make          - Compila o programa"
	@echo "  make run      - Compila e executa"
	@echo "  make clean    - Remove arquivos compilados"
	@echo "  make rebuild  - Limpa e recompila"
	@echo "  make help     - Mostra esta mensagem"

.PHONY: all clean run rebuild help
