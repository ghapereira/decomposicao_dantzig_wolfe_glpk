# Método de Decomposição de Dantzig-Wolfe

O problema de corte unidimensional é uma utilização clássica da pesquisa
operacional, especialmente no que se refere à geração de colunas. Nele,
se tem um tamanho total de folha, ou chapa de aço, que pode ser subdividido.
Além disso, se fazem necessários um número de envios de determinados tamanhos
de corte. O problema então estabelece a necessidade de se minimizar o número
de padrões de corte, isto é, qual número de cortes de cada comprimento dentro
da placa original.

Este problema é assim descrito em [1]:

\begin{align}
min z = \sum_{j} x_j
        s. a. \sum_{j} a_j^i * x_j \geq b_i  \hspace{0.5cm}    \forall_i\\
        x_j \in \N
\end{align}

A utilidade da geração de colunas é notada quando se percebe a necessidade da
enumeração dos padrões de corte para a resolução do problema. Esta enumeração
compreende tantos padrões que se torna logo inviável. A ideia do Dantzig-Wolfe,
então, é de gerar novos padrões para serem avaliados. Tais padrões são
representados por uma nova coluna na matriz.

  A geração dos padrões advém da solução de um problema da mochila inteira, isto é,
um problema da maximização dos lucros através da alocação de bens num invólucro,
chamado mochila. A solução deste subproblema gera a nova coluna a ser avaliada
pelo problema mestre:

\begin{align}
max z = \sum_{i} \pi_{i} a_{j}^{i}\
        s.a. \sum_{i} l_{i} * a_{j}^{i} \leq L
        a_{i}^{j} \geq 0
\end{align}

A coluna gerada, que são as soluções do subproblema, é adicionada ao mestre, e
se repete o procedimento enquanto o valor da função objetivo do subproblema
for maior do que 1.

### Implementação
Para a implementação, utilizei o  GLPK, um pacote de programação
linear e inteira do GNU. Instruções de instalação podem ser
encontradas [aqui](https://en.wikibooks.org/wiki/GLPK/Linux_OS#Install).
A linguagem definida para o trabalho é o C, com um makefile incluso.
O programa foi desenvolvido em Linux. Uma ferramenta fundamental utilizada
para checagem de vazamentos de memória foi o [valgrind](http://valgrind.org/).

Por questões de praticidade algumas decisões foram tomadas:

* Para a maioria dos casos de teste de maior tamanho que encontrei não consegui
alcançar a condição de parada. Inicialmente percebi uma melhoria no objetivo do
mestre, mas então o programa parou de melhorar por uma grande quantidade de tempo.
Provavelmente incorri num caso de degeneração, restando
esperar a saída deste passo. Para obter resultados de teste, limitei a um número definido de iterações a busca, de modo que eu consigo uma solução viável e melhor que a mais ingênua, porém sub-ótima.
Uma exceção foi o pequeno caso de teste
*e3*, que foi rapidamente resolvido segundo a condição de parada.

* O primeiro arquivo que utilizei de teste, *gau3*, tinha a melhor
formatação para leitura. O arquivo *e3* possuía formatação similar.
Como o professor informou que não haveria problema na modificação do formato
da entrada, subdividi o arquivo *bin3* em arquivos *bin3HARD0*... *bin3HARD9*, além de passar o formato das entradas para um formato similar ao
do *gau3*

### Referências

[1] STIDSEN, Thomas. [*Column Generation: Cutting Stock – A very
  applied method*](http://www.columbia.edu/~cs2035/courses/ieor4600.S07/columngeneration.pdf).

[2] MAKHORIN, Andrew. [*GNU Linear Programming Kit - Reference Manual*](Disponível online em http://kam.mff.cuni.cz/~elias/glpk.pdf).

[3] MARTIN, Richard Kipp. *Large Scale Linear and Integer Optimization*. Kluwer Academic Publishers, 1999.


### Links
* [Página com instâncias Dantzig-Wolfe](http://www.math.tu-dresden.de/~capad/)

Todas as instâncias porventura adicionadas ao repositório são provenientes das localidades
citadas, com todos os direitos a eles reservados.

