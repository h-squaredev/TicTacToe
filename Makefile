# Compilador C++ principal.
CXX = g++

# Flags de compilacao (C++17 + warnings + include local).
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic -Iinclude

# Nome do executavel final.
TARGET = jogo_da_velha

# Diretorios padrao do projeto.
SRC_DIR = src
OBJ_DIR = obj

# Descobre todos os fontes e gera lista correspondente de objetos.
SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SOURCES))

# Alvo padrao.
all: $(TARGET)

# Linkagem final.
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Regra de compilacao de cada .cpp para .o.
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Garante existencia do diretorio de objetos.
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Limpeza de artefatos de build.
clean:
	rm -f $(TARGET) $(OBJECTS)

# Declara alvos que nao representam arquivos reais.
.PHONY: all clean
