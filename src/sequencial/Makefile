CC = gcc
CFLAGS = -Wall -O3

UTILS_DIR = ../utils
INCLUDES = -I$(UTILS_DIR)
# Executa o main sequencial com os dois arquivos utilitários de IO e Math
SOURCES = main.c $(UTILS_DIR)/io_utils.c $(UTILS_DIR)/math_utils.c

all: kmeans_seq
kmeans_seq:
	$(CC) $(CFLAGS) $(INCLUDES) $(SOURCES) -o kmeans_seq -lm
clean:
	rm -f kmeans_seq