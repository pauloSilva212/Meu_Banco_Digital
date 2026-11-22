
#ifndef MEU_BANCO_H
#define MEU_BANCO_H

#define MAX_CLIENTES 100

typedef struct {
    int numero;
    int tipo;
    float saldo;
} Conta;

typedef struct {
    char nome[50];
    Conta contas[3];

    int possuiEmprestimo;
    int possuiTitulo;

    float premio;
    char historico[1000];

} Cliente;

void abrirBanco(Cliente clientes[], int quantidade);

void menuOperacoes(Cliente clientes[], int quantidade);

void consultarHistorico(Cliente clientes[], int quantidade);

void efetuarSorteio(Cliente clientes[], int quantidade);

void verificarSaldoNegativo(Cliente clientes[], int quantidade);

#endif
