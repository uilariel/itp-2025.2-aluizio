#ifndef SHARED_EAN8_H
#define SHARED_EAN8_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define DigitosEAN8 8 //digitos totais do codigo de barra
#define AreaCodigo 67 //numero de areas necessario pra multiplicar pelos pixels

typedef enum{
    LCODE = 0, //usado na coluna 0
    RCODE = 1  //usado na coluna 1
} TipoDeCodificacao; //R OU L
int calcular_digito_verificador (const char *Digitos_7);//gera o oitavo digito
int verificador_oitavo_digito (const char *verif8); //verifica se os 8 digitos sao validos
char* gerarsequencia (const char *digitos8); //pega os 8 digitos e retorna a string
int decodificador (const char *bloco, TipoDeCodificacao tipo); //converte um bloco de 7 areas para seu digito numerico

#endif
