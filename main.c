/* *****************************************************************************
 * @project     Decomposição de Dantzig-Wolfe
 * @file        main.c
 * @brief       Arquivo principal do projeto, reunindo as chamadas
 *              para a resolução dos problemas
 * @author      Gustavo H. Alves Pereira
 * ****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <glpk.h>
#include <time.h>
#include "definicoes.h"
#include "leituraArquivos.h"
#include "decomposicaoDW.h"

int main() {
        DescProblema problema;
        clock_t launch, done;

        // Problema 'gau3'
        leArquivoGau3(&problema, "dat/gau3");
        launch = clock();
        decomposicaoDantzigWolfe(&problema, "gau3");
        done = clock();
        printf("Tempo gasto: %lds\n", (done - launch)/CLOCKS_PER_SEC);
        free(problema.cortes);
        printf("-------------------------\n\n");

        // O problema 'e3' teme ntrada similar ao 'gau3'
        leArquivoGau3(&problema, "dat/e3");
        launch = clock();
        decomposicaoDantzigWolfe(&problema, "e3");
        done = clock();
        printf("Tempo gasto: %lds\n", (done - launch)/CLOCKS_PER_SEC);
        free(problema.cortes);
        printf("-------------------------\n\n");

        // Decompus o problema 'bin3' em HARD0 a HARD9
        size_t i = 0;
        const size_t instanciasBIN3 = 9;
        // dat/bin3HARDX -> 13 caracteres, + '\0': 14
        const size_t tamNomeArquivoBIN3 = 14;
        char arquivoBin3[tamNomeArquivoBIN3];
        for(i = 0; i <= instanciasBIN3; i++) {
                sprintf(arquivoBin3, "dat/bin3HARD%ld", i);
                leArquivoGau3(&problema, arquivoBin3);
                launch = clock();
                decomposicaoDantzigWolfe(&problema, arquivoBin3);
                done = clock();
                printf("Tempo gasto: %lds\n", (done - launch)/CLOCKS_PER_SEC);
                free(problema.cortes);
                printf("-------------------------\n\n");
        }

        return 0;
}

