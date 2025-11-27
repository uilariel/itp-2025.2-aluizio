#include "shared_ean8.h"
//matriz contendo os codigos L & R pros numeros
static const char *CODIGOS_EAN8[10][2] = {
    // Digito | Lcode  | Rcode
     {"0001101", "1110010"},//0
     {"0011001", "1100110"},//1
     {"0010011", "1101100"},//2
     {"0111101", "1000010"},//3
     {"0100011", "1011100"},//4
     {"0110001", "1001110"},//5
     {"0101111", "1010000"},//6
     {"0111011", "1000100"},//7
     {"0110111", "1001000"},//8
     {"0001011", "1110100"}//9
};

int calcular_digito_verificador (const char *Digitos_7){
    int somapond = 0;
    int peso;
//loop pra passar por todos os digitos
    for (int i = 0; i < 7; i++){
        int digito = Digitos_7[i] - '0'; //converte char pra int
        if ((i+1)%2 != 0){
            peso = 3;
        }
        else {
            peso = 1;
        }
        somapond = somapond + (digito*peso); //acumula a soma ponderada
    }
    int resto =  somapond % 10; //dps de terminar o loop for, divide por 10 e retorna o modulo
    if (resto == 0){
        return 0; //se for 0, o ultimo digito sera 0
    }
    else {
        return 10 - resto; // se for != 0, o ultimo digito eh 10 - o resto
         }
    }


int verificador_oitavo_digito (const char *codigo_8_digitos){
    //verifica se o tamanho eh 8
    if (codigo_8_digitos == NULL || strlen(codigo_8_digitos) != DigitosEAN8){

        return 0;
    }
    //verifica se so tem numeros
    for (int i = 0; i < DigitosEAN8; i++){
        if (codigo_8_digitos[i] < '0' || codigo_8_digitos[i] > '9'){
            return 0; //invalido
        }
    }
    //verifica o digito verificador !
    // passo 1: separar os 7 digitos
    char Digitos_7[8]; //buffer para 7 digitos + o nulo
    strncpy(Digitos_7, codigo_8_digitos, 7);
    Digitos_7[7] = '\0';

    //passo 2: calcular o digito verificador
    int dv_calculado = calcular_digito_verificador(Digitos_7);

    //passo 3: obter o digito que foi fornecido na entrada
    int dv_entrada = codigo_8_digitos[7] - '0';

    //passo 4: comparar com os dois
    if (dv_entrada != dv_calculado){
        return 0; //invalido
    }

    return 1; //se nao foi barrado em nada retorna true (1) :D!!!!!!!
}


//recebe os 8 digitos e retorna uma string com 67 digitos + o nulo 
char* gerarsequencia (const char *digitos8){
    char *sequencia = (char*)malloc(AreaCodigo + 1); //alocamos 67 + 1
    if (sequencia == NULL){
        printf (" nao foi possivel alocar a memoria");
        return NULL; //VERIFICA ERRO
    }
    char *p = sequencia; //ponteiro que rastreia onde escrever na string
    //passo 1: 3 digitos iniciais fixos
    strcpy(p, "101");
    p += 3; //move p

    //passo 2: 4 digitos do LCODE
    for (int i = 0; i < 4 ; i++){
        int digito = digitos8[i] - '0'; //converte string digitos8 pra int
        strcpy(p, CODIGOS_EAN8[digito][LCODE]); //pega o digito e 0 ou 1 pra se localizar na matriz
        p += 7; //avanca 7 posicoes
    }
    //passo 3: 5 digitos do meio
    strcpy(p, "01010");
    p += 5; //avanca 5 posicoes

    //passo 4:
    for (int i = 4; i < 8; i++){
        int digito = digitos8[i] - '0'; //converte de string pra int
        strcpy(p, CODIGOS_EAN8[digito][RCODE]);
        p += 7; //avanca 7 posicoes 
    }
    //passo 5: 3 digitos do final
    strcpy(p, "101");
    p += 3; //ja acabou mas vo avancar 3 so pra ficar igual

    sequencia[AreaCodigo] = '\0'; // garante q o ultimo eh nulo
    return sequencia; //lembrar de chamar free no programa
}

int decodificador(const char *bloco, TipoDeCodificacao tipo){
    int coluna = (int)tipo; //determina a coluna q vai ser buscada R OU L
    //loop que itera sobre as 10 linhas da matriz (0 ate 9)
    for(int digito = 0; digito <10; digito++){
        //compara a entrada com o numero na matriz
        //retorna 0 se forem iguais
        if (strcmp(bloco, CODIGOS_EAN8[digito][coluna]) == 0){
            return digito; //retorna o indice que vai ser o digito se for igual
        }
    }
    printf ("erro de leitura");
    return -1; //se o loop terminar sem encontrar nada da erro de leitura
}