#ifndef SHARED_EAN8_H
#define SHARED_EAN8_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define DigitosEAN8 8 //digitos totais do codigo de barra
#define AreaCodigo 67 //numero de areas necessario pra multiplicar pelos pixels

typedef enum{
    LCODE,
    RCODE
} TipoCodigos;
int CalcOitavoDig (const char *Digitos_7);//gera o oitavo digito
int Verif8dig (const char *verif8); //verifica se os 8 digitos sao validos
char* gerarsequencia (const char *digitos8); //pega os 8 digitos e transforme em 0 e 1
int decodificador (const char *bloco); //converte um bloco de 7 areas para seu digito numerico
#endif
