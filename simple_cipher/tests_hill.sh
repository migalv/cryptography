#!/bin/bash

# Aqui va el texto que quieres cifrar sin la opcion -i
TEXTO_A_CIFRAR="ABCD"
# Aqui va el texto que se espera una vez se haya cifrado
TEXTO_CIFRADO="EYFXDR"
# Variable que se usa para decir si utilizar valgrind o no
VALGRIND=""

while test $# -gt 0
do
    case "$1" in
        --verbose) echo "Verbose 1"
            ;;
        -v) VALGRIND="valgrind --leak-check=full"
            ;;
        -h) echo "Usage:"
			echo "\t\t $0 [-v]"
			echo "OPTIONS"
			echo " -v\t\t\t Used to pass valgrind tests"
			;;
        --*) echo "BAD OPTION use tests_hill.sh -h to show help."
            ;;
        *) echo "BAD ARGUMENT use tests_hill.sh -h to show help."
            ;;
    esac
    shift
done

echo "${TEXTO_A_CIFRAR}" > foobar.txt

OUTPUT="$($VALGRIND bin/hill -C 26 3 matriz_inyectiva.txt < foobar.txt)"

rm foobar.txt

# Ahora comprobamos que la salida del programa es la esperada
if [ $(echo ${OUTPUT} | grep -c ${TEXTO_CIFRADO}) -gt 0 ]
then
  echo "Success"
else
  echo "Fail";
fi
