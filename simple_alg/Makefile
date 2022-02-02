
################################################################################
# Makefile de la práctica 1 de Cripto
# Grupo 2312, Pareja 13
# Alberto Garcia Redero, Miguel Álvarez Lesmes
################################################################################

# compilador
CC = gcc

#banderas de compilacion
CFLAGS = -Wall -g
LDLIBS = -lm -lgmp -Iincludes -L lib

vpath %.c src/
vpath %.h includes/

all: afin afin_mejorado hill vignere criptoanalisis transposicion permutacion clear

afin: main_afin.o afin.o euclides.o parser.o

afin_mejorado: main_afin_mejorado.o afin_mejorado.o afin.o euclides.o parser.o

hill: main_hill.o hill.o matriz.o modular.o euclides.o parser.o

vignere: main_vignere.o vignere.o parser.o

criptoanalisis: main_criptoanalisis.o criptoanalisis.o euclides.o

transposicion: transposicion.o hill.o matriz.o parser.o modular.o euclides.o

permutacion: main_permutacion.o permutacion.o matriz.o parser.o modular.o euclides.o hill.o

# Regla que genera todos los .o
%.o :	%.c
	@echo -n compilando objeto \'$<\'...
	@$(CC) $(CFLAGS) $< -c
	@echo [OK]

# Regla que genera el ejecutable
% :	%.o
	@echo -n compilando ejecutable \'$@\'...
	@mkdir -p bin
	@$(CC) $(CFLAGS) $^ -o bin/$@ $(LDLIBS)
	@echo [OK]

# limpieza de .o, ejecutables y librerias
clean:
	@echo Limpiando *.o, ejecutables y librerias ...
	@rm -f *.o bin/*

# limpieza solo de .o
clear:
	@echo Limpiando ficheros .o ...
	@rm -f *.o

# información extra
info:
	@echo "Autores: Alberto Garcia Redero, Miguel Álvarez Lesmes"
	@echo "Práctica 1 de CRIPTO, Pareja 13"

# ayuda
help:
	@echo "Uso: make <tag>"
	@echo "Tags válidos:"
	@$(MAKE) --print-data-base --question | sed -e "s:makefile::g" |\
	awk '/^[^.%][-A-Za-z0-9_]*:/	\
		{ print "   " substr($$1, 1, length($$1)-1) }'
