
################################################################################
# Makefile de la práctica 3 de Cripto
# Grupo 2312, Pareja 13
# Alberto Garcia Redero, Miguel Álvarez Lesmes
################################################################################

# compilador
CC = gcc

#banderas de compilacion
CFLAGS = -Wall -g
LDLIBS = -lm -lgmp -Iincludes -L lib

vpath %.c src/
vpath %.h include/
vpath %.a lib/

all: potencia primo factorizar clear

potencia: potencia.o

primo: primo.o

factorizar: factorizar.o

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
	@echo "Práctica 2 de CRIPTO, Pareja 13"

#=============================================================
# Creates tgz with the code
#=============================================================
dist: clean
	tar cvzf F-CSI_p3_g13.tgz src/ primes.dat Makefile
	@ echo "Cleanup Completed!"

# ayuda
help:
	@echo "Uso: make <tag>"
	@echo "Tags válidos:"
	@$(MAKE) --print-data-base --question | sed -e "s:makefile::g" |\
	awk '/^[^.%][-A-Za-z0-9_]*:/	\
		{ print "   " substr($$1, 1, length($$1)-1) }'
