#ifndef __LEITURA_ARQUIVOS_DW_H__
#define __LEITURA_ARQUIVOS_DW_H__

/**
 * @function    leArquivoGau3
 * @brief       Lê problema do arquivo Gau3
 * @param       cortes: TipoCorte *, vetor onde serão armazenados os tipos de
 *              corte
 * @return      DescProblema contendo dados de tamanho do problema
 */
void leArquivoGau3(DescProblema * problema, const char * nomeArquivo);

#endif // __LEITURA_ARQUIVOS_DW_H__

