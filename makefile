all:
	gcc main.c leituraArquivos.c matrizes.c decomposicaoDW.c -lglpk -lm -Wall -o dwolfe.out
clean:
	rm dwolfe.out

