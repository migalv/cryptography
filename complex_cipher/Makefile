
################################################################################
# Makefile de la práctica 2 de Cripto
# Grupo 2312, Pareja 13
# Alberto Garcia Redero, Miguel Álvarez Lesmes
################################################################################

# compilador
CC = gcc

#banderas de compilacion
CFLAGS = -Wall -g
LDLIBS = -lm -Iincludes -L lib

vpath %.c src/
vpath %.h include/

all: desCBC avalancha criterion linealidad_SBoxes_DES genera_sbox tripleDESCBC seg-perf linealidad_SBoxes_AES sbox_aes clear

desCBC: main_des.o desCBC.o

avalancha: avalancha.o desCBC.o

criterion: main_criterion.o criterion.o

linealidad_SBoxes_DES: main_linealidadDES.o linealidad_SBoxes_DES.o

genera_sbox: genera_sbox.o criterion.o linealidad_SBoxes_DES.o

tripleDESCBC: tripleDESCBC.o desCBC.o

seg-perf: main_seg-perf.c seg-perf.o parser.o

linealidad_SBoxes_AES: main_linealidadAES.c linealidad_SBoxes_AES.o

sbox_aes: main_sbox_aes.c sbox_aes.o

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
	tar cvzf F-CSI_p2_g13.tgz src/ includes/ Makefile remove_metadata.py images/
	@ echo "Cleanup Completed!"

# ayuda
help:
	@echo "Uso: make <tag>"
	@echo "Tags válidos:"
	@$(MAKE) --print-data-base --question | sed -e "s:makefile::g" |\
	awk '/^[^.%][-A-Za-z0-9_]*:/	\
		{ print "   " substr($$1, 1, length($$1)-1) }'
