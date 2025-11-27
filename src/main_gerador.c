#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shared_ean8.h"

//parametros opcionais macros
#define padrao_espacamento_lateral 4 // pixels
#define padrao_pixels_por_area 3     // pixels
#define altura_padrao_codigo 50      // pixels
#define nome_padrao_do_arquivo "codigo_de_barras.pbm"

typedef struct {
    char *identificador;
    int espacamento_lateral;
    int pixels_por_area;
    int altura_codigo;
    char *nome_arquivo;
} ParametrosGerador;

int main (int argc, char *argv[]){
ParametrosGerador parametros;
    if (argc <2){
        fprintf(stderr, "Erro: Faltou o argumento obrigatorio.\n");
        fprintf(stderr, "Uso correto: %s <identificador_8_digitos> [espacamento_lateral] [pixels_por_area] [altura] [nome_arquivo]\n", argv[0]);
        return 1; //retorna erro
    }

    parametros.identificador = argv[1]; //definindo o argv1 como identificado

    //verificando se oq foi inserido eh valido como identificador 
    if (verificador_oitavo_digito(parametros.identificador) == 0){
        fprintf(stderr, "Erro: identificador invalido"); //erro se os 8 digitos nao forem validos
        return 1;
    }
    //se houver mais q dois argumentos, o terceiro vai ser o espacamento lateral, se nao, use oq foi definido como padrao.
    /*                                condicao  |   caso true     |    caso false*/
    parametros.espacamento_lateral = (argc > 2) ? atoi(argv[2]) : padrao_espacamento_lateral;
    //fazendo o mesmo pro resto dos argumentos
    parametros.pixels_por_area = (argc > 3) ? atoi(argv[3]) : padrao_pixels_por_area;
    parametros.altura_codigo = (argc > 4) ? atoi (argv[4]) : altura_padrao_codigo;
    parametros.nome_arquivo = (argc > 5) ? argv [5] : nome_padrao_do_arquivo;


    int largura_codigo;
    int largura_total_imagem;
    int altura_total_imagem;

    //espacamento vertical vai ser igual ao lateral
    const int espacamento_vertical =  parametros.espacamento_lateral;

    //calculo 67 Areas x Pixels
    largura_codigo = AreaCodigo * parametros.pixels_por_area;

    //largura total calculo largura do codigo + espcamento direito + espacaamento esquerdo
    largura_total_imagem = largura_codigo + (2 * parametros.espacamento_lateral);

    //altura total parecido com a largura mas verticalmente.
    altura_total_imagem = parametros.altura_codigo + (2 * espacamento_vertical);

    printf("A imagem tera %d x %d pixels. Largura do codigo sera: %d\n",
    largura_total_imagem, altura_total_imagem, largura_codigo);

    FILE *fp = NULL;
    fp = fopen(parametros.nome_arquivo, "r"); //tenta abrir o arquivo pra ver se ja existe
    if (fp != NULL){
        //Arquivo ja existe!
        fclose(fp); //fecha oq tentou ler
        char resposta;

        printf ("O arquivo '%s' ja existe, voce deseja sobrescrever ele? (s para sim / n para nao)", parametros.nome_arquivo);
        scanf(" %c", &resposta);

        if (resposta != 's'){
            //caso o usuario negue sobrescrever
            fprintf (stderr, "Erro: o arquivo ja existe. a execucao foi interrompida.");
            return 1; //retorna erro
        }
    }
    fp = fopen (parametros.nome_arquivo, "w");

    if (fp == NULL){
        //ERRO ao abrir ou conseguir permissao
        fprintf (stderr, "Erro: nao foi possivel abrir o arquivo");
        return 1;
    }

    //gerando a sequencia binario de 67 areas
    //usando a funcao
    char *sequencia67 = gerarsequencia(parametros.identificador);

    if (sequencia67 == NULL) {
        //erro de alocacao
        fclose(fp);
        return 1;
    }

    //escrever cabecalho PBM
    fprintf(fp, "P1\n");
    fprintf(fp, "%d %d\n", largura_total_imagem, altura_total_imagem);

                               /* MATRIZ*/
    //definindo limites
    //espacamento vertical = lateral

    const int linha_inicio_codigo = espacamento_vertical;
    const int linha_fim_codigo = altura_total_imagem - espacamento_vertical;

    //desenhando a matriz loop

    for (int y = 0; y < altura_total_imagem; y++){
        //logica vertical espacamento em cima e em baixo
        // se tiver na area do espacamento, escreve uma linha de 0
        if (y < linha_inicio_codigo || y >= linha_fim_codigo){
            for (int x = 0; x < largura_total_imagem; x++){
                fprintf(fp, "0");
            }
        }
        else {
            //          loop q percorre as colunas da imagem
           for (int x = 0; x < largura_total_imagem; x++){
            //logica horizontal
            //
            if (x < parametros.espacamento_lateral || x >= (largura_total_imagem - parametros.espacamento_lateral)){
                fprintf (fp, "0"); //escreve branco
            }
            //             CODIGO DE BARRAS DE VERDADE
            else {
                //zera o indice x no ponto onde o codigo de barras comeca
                int indice_relativo = x - parametros.espacamento_lateral;

                //determina qual das 67 areas estamos desenhas
                //o indice da area eh o indice relativo dividido pelos pixels por area
                int indice_area = indice_relativo / parametros.pixels_por_area;

                //pega o caractere 0 ou 1 da sequencia binaria
                char pixel_cor = sequencia67[indice_area];
                fprintf(fp, "%c", pixel_cor);
            }
           }
        }
        fprintf (fp, "\n"); //uma nova linha eh escriva depois de cada pixel
    }
    //limpeza e fechamento
    free(sequencia67); //libera malloc
    fclose(fp); //fecha o arquivo
    printf ("arquivo de imagem: '%s' foi gerado\n", parametros.nome_arquivo);
    return 0;
}
