#include <stdio.h>
#include <stdlib.h>
// Inclui seu shared_ean8.h com suas constantes e protótipos
#include "shared_ean8.h" 

// --- Definições de Teste ---
#define ID_VALIDO "40170725"
#define SEQUENCIA_ESPERADA "1010100011000110100110010111011010101110010100010011011001001110101"

// Declarações de Funções para o Compilador (pois estão definidas em outro .c)
// Necessário para evitar warnings/erros de linker se o .h não usar exatamente o mesmo nome
int calcular_digito_verificador (const char *Digitos_7);
int verificador_oitavo_digito (const char *codigo_8_digitos);
char* gerarsequencia (const char *digitos8);
int decodificador (const char *bloco, TipoDeCodificacao tipo); 

void rodar_testes_shared() {
    printf("--- Testes do Modulo Compartilhado EAN-8 ---\n");
    char *sequencia_gerada = NULL;

    // 1. Teste de Calculo do Digito Verificador
    int dv_calc = calcular_digito_verificador("4017072");
    printf("1. Calculo DV (4017072): Esperado: 5. Resultado: %d -> %s\n", dv_calc, (dv_calc == 5) ? "OK" : "FALHA");

    // 2. Teste de Validacao (Identificador Completo)
    int valid_ok = verificador_oitavo_digito(ID_VALIDO);
    int valid_fail = verificador_oitavo_digito("40170724"); // DV Invalido (deve ser 5)
    printf("2a. Validacao OK (%s): Resultado: %s\n", ID_VALIDO, (valid_ok == 1) ? "OK" : "FALHA");
    printf("2b. Validacao FALHA (40170724): Resultado: %s\n", (valid_fail == 0) ? "OK" : "FALHA");

    // 3. Teste de Codificacao (Gerar Sequencia)
    sequencia_gerada = gerarsequencia(ID_VALIDO);
    if (sequencia_gerada != NULL) {
        if (strcmp(sequencia_gerada, SEQUENCIA_ESPERADA) == 0) {
            printf("3. Codificacao (67 areas): OK\n");
        } else {
            printf("3. Codificacao (67 areas): FALHA\n");
        }
    } else {
        printf("3. Codificacao (67 areas): Erro de alocacao (NULL)\n");
    }
    
    // 4. Teste de Decodificacao (L-CODE e R-CODE)
    
    // Primeiro Digito (4) -> L-code: 0100011
    int d1 = decodificador("0100011", LCODE);
    printf("4a. Decodificacao L-CODE (0100011): Esperado 4. Resultado: %d -> %s\n", d1, (d1 == 4) ? "OK" : "FALHA");

    // Oitavo Digito (5) -> R-code: 1001110
    int d8 = decodificador("1001110", RCODE);
    printf("4b. Decodificacao R-CODE (1001110): Esperado 5. Resultado: %d -> %s\n", d8, (d8 == 5) ? "OK" : "FALHA");
    
    // 5. Teste de Decodificacao (Bloco Invalido)
    int d_invalido = decodificador("1111111", RCODE);
    printf("5. Decodificacao Invalida: Esperado -1. Resultado: %d -> %s\n", d_invalido, (d_invalido == -1) ? "OK" : "FALHA");


    // 6. Limpeza da memória
    if (sequencia_gerada != NULL) {
        free(sequencia_gerada);
    }
    printf("--------------------------------------------\n");
}

int main() {
    rodar_testes_shared();
    return 0;
}