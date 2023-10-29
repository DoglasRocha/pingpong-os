# Nome do seu arquivo de saída
OUTPUT = output

# Lista de arquivos de origem
SRCS = ppos-core-aux.c pingpong-scheduler-srtf.c

# Bibliotecas estáticas
LIBS = libppos_static.a

# Compilador e opções
CC = gcc
CFLAGS = -Wall

# Regra para criar o executável
$(OUTPUT): $(SRCS) $(LIBS)
	$(CC) $(CFLAGS) -o $(OUTPUT) $(SRCS) $(LIBS)

# Regra para limpar arquivos temporários e o executável
clean:
	rm -f $(OUTPUT)

# Regra phony para evitar conflitos com arquivos chamados "clean"
.PHONY: clean