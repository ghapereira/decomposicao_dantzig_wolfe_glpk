/* *****************************************************************************
 * @project     Decomposição de Dantzig-Wolfe
 * @file        leituraArquivos.c
 * @brief       Funções de leitura de arquivos para o projeto
 * @author      Gustavo H. Alves Pereira
 * ****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include "definicoes.h"

/**
 * @function    leArquivoGau3
 * @brief       Lê problema do arquivo Gau3
 * @note        Os outros problemas foram convertidos para o formato
 *              do gau3. Mantenho o nome por motivos históricos
 * @param       cortes: TipoCorte *, vetor onde serão armazenados os tipos de
 *              corte
 * @return      DescProblema contendo dados de tamanho do problema
 */
void leArquivoGau3(DescProblema * problema, const char * nomeArquivo) {
        printf("Lendo do arquivo %s\n", nomeArquivo);
        FILE* fp;
        char line[TAM_MAX_LINHA_PROB];

        // Contador
        size_t i = 0;

        fp = fopen(nomeArquivo, "r");
        if(fp == NULL) {
                printf("\n>>>Erro! Arquivo %s não pode ser aberto.\n",
                        nomeArquivo);
                exit(1);
        }

        // Le primeira linha, com cabeçalho
        fgets(line, TAM_MAX_LINHA_PROB, fp);
        sscanf(line, "%d %ld", &problema->tamanhoTotal,
               &problema->numeroDeInstancias);
        problema->cortes = (TipoCorte *) calloc (problema->numeroDeInstancias,
                                                 sizeof(TipoCorte));

        // Lê as demais linhas
        for(i = 0; i < problema->numeroDeInstancias; i++) {
                if (fgets(line, TAM_MAX_LINHA_PROB, fp) == NULL) {
                        printf("\n[ERRO] Arquivo acabou antes do tamanho declarado.\n");
                        free(problema->cortes);
                        exit(1);
                }
                sscanf(line, "%d %d",
                       &problema->cortes[i].largura,
                       &problema->cortes[i].qtdCortes);
        }

        fclose(fp);
}

