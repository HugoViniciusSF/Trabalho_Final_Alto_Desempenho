# Compilador NVIDIA
CC = nvcc
# Flags de otimização e definição da arquitetura da GTX 1050 Ti (sm_61)
CFLAGS = -O3 -arch=sm_61
# Nome do ficheiro executável final
TARGET = kmeans_cuda
# Ficheiros fonte necessários para a compilação
SRCS = main.cu cuda_kmeans.cu ../utils/io_utils.c ../utils/math_utils.c

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRCS)

clean:
	rm -f $(TARGET)