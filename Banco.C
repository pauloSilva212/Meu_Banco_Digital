
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "meu_banco.h"

// Encontra uma conta pelo tipo
int encontrarConta(Cliente cliente, int tipo) {

    int i;

    for (i = 0; i < 3; i++) {
        if (cliente.contas[i].tipo == tipo) {
            return i;
        }
    }

    return -1;
}

// Calcula o saldo total do cliente
float calcularSaldo(Cliente cliente) {

    int i;
    float total = 0;

    for (i = 0; i < 3; i++) {
        total += cliente.contas[i].saldo;
    }

    return total;
}

// Registra uma movimentacao no historico
void registrarHistorico(Cliente clientes[], int c, char texto[]) {

    if (strlen(clientes[c].historico) + strlen(texto) < 1000) {
        strcat(clientes[c].historico, texto);
    }
}

// CADASTRO DOS CLIENTES
void abrirBanco(Cliente clientes[], int quantidade) {

    int i, j, k, tipo;
    int numeroConta = 1000;
    int existe;

    for (i = 0; i < quantidade; i++) {

        printf("\nNome do cliente: ");
        scanf(" %49[^\n]", clientes[i].nome);

        clientes[i].possuiEmprestimo = 0;
        clientes[i].possuiTitulo = 0;
        clientes[i].premio = 0;

        strcpy(clientes[i].historico, "");

        for (j = 0; j < 3; j++) {
            clientes[i].contas[j].tipo = 0;
            clientes[i].contas[j].numero = 0;
            clientes[i].contas[j].saldo = 0;
        }

        j = 0;

        while (j < 3) {

            printf("\n1 - Corrente");
            printf("\n2 - Salario");
            printf("\n3 - Poupanca");
            printf("\n0 - Encerrar cadastro");
            printf("\nEscolha: ");

            scanf("%d", &tipo);

            if (tipo == 0) {
                break;
            }

            if (tipo < 1 || tipo > 3) {
                printf("Opcao invalida!\n");
                continue;
            }

            existe = 0;

            for (k = 0; k < j; k++) {

                if (clientes[i].contas[k].tipo == tipo) {
                    existe = 1;
                }

                if ((tipo == 1 && clientes[i].contas[k].tipo == 2) ||
                    (tipo == 2 && clientes[i].contas[k].tipo == 1)) {
                    existe = 1;
                }
            }

            if (existe == 1) {
                printf("Essa combinacao de contas nao e permitida!\n");
                continue;
            }

            clientes[i].contas[j].tipo = tipo;
            clientes[i].contas[j].numero = numeroConta;
            clientes[i].contas[j].saldo = 0;

            printf("Conta criada: %04d-0%d\n", numeroConta, tipo);

            numeroConta++;
            j++;
        }
    }

    printf("\nBanco aberto com sucesso!\n");
}

// DEPOSITO
void depositar(Cliente clientes[], int quantidade) {

    int cliente, tipo, conta;
    float valor;
    char texto[100];

    printf("Numero do cliente: ");
    scanf("%d", &cliente);

    if (cliente < 1 || cliente > quantidade) {
        printf("Cliente invalido!\n");
        return;
    }

    cliente--;

    printf("Tipo da conta (1, 2 ou 3): ");
    scanf("%d", &tipo);

    conta = encontrarConta(clientes[cliente], tipo);

    if (conta == -1) {
        printf("Conta nao encontrada!\n");
        return;
    }

    printf("Valor do deposito: R$ ");
    scanf("%f", &valor);

    if (valor <= 0) {
        printf("Valor invalido!\n");
        return;
    }

    clientes[cliente].contas[conta].saldo += valor;

    sprintf(texto, "Deposito: R$ %.2f\n", valor);
    registrarHistorico(clientes, cliente, texto);

    printf("Deposito realizado!\n");
}

// SAQUE
void sacar(Cliente clientes[], int quantidade) {

    int cliente, tipo, conta;
    float valor;
    char texto[100];

    printf("Numero do cliente: ");
    scanf("%d", &cliente);

    if (cliente < 1 || cliente > quantidade) {
        printf("Cliente invalido!\n");
        return;
    }

    cliente--;

    printf("Tipo da conta (1, 2 ou 3): ");
    scanf("%d", &tipo);

    conta = encontrarConta(clientes[cliente], tipo);

    if (conta == -1) {
        printf("Conta nao encontrada!\n");
        return;
    }

    printf("Valor do saque: R$ ");
    scanf("%f", &valor);

    if (valor <= 0) {
        printf("Valor invalido!\n");
        return;
    }

    clientes[cliente].contas[conta].saldo -= valor;

    sprintf(texto, "Saque: R$ %.2f\n", valor);
    registrarHistorico(clientes, cliente, texto);

    printf("Saque realizado!\n");
}

// CONSULTA DE SALDO
void consultarSaldo(Cliente clientes[], int quantidade) {

    int cliente, i;

    printf("Numero do cliente: ");
    scanf("%d", &cliente);

    if (cliente < 1 || cliente > quantidade) {
        printf("Cliente invalido!\n");
        return;
    }

    cliente--;

    printf("\nCliente: %s\n", clientes[cliente].nome);

    for (i = 0; i < 3; i++) {

        if (clientes[cliente].contas[i].tipo != 0) {

            printf("Conta: %04d-0%d\n",
                clientes[cliente].contas[i].numero,
                clientes[cliente].contas[i].tipo);

            printf("Saldo: R$ %.2f\n",
                clientes[cliente].contas[i].saldo);
        }
    }

    printf("Saldo total: R$ %.2f\n",
        calcularSaldo(clientes[cliente]));
}

// EMPRESTIMO
void solicitarEmprestimo(Cliente clientes[], int quantidade) {

    int cliente, corrente, poupanca;
    float limite, valor;
    char texto[100];

    printf("Numero do cliente: ");
    scanf("%d", &cliente);

    if (cliente < 1 || cliente > quantidade) {
        printf("Cliente invalido!\n");
        return;
    }

    cliente--;

    if (clientes[cliente].possuiEmprestimo == 1) {
        printf("Cliente ja recebeu emprestimo!\n");
        return;
    }

    corrente = encontrarConta(clientes[cliente], 1);
    poupanca = encontrarConta(clientes[cliente], 3);

    if (corrente == -1 || poupanca == -1) {
        printf("Precisa ter conta corrente e poupanca!\n");
        return;
    }

    if (clientes[cliente].contas[poupanca].saldo <= 0) {
        printf("Precisa ter saldo na poupanca!\n");
        return;
    }

    limite = calcularSaldo(clientes[cliente]) * 2.5;

    if (limite <= 0) {
        printf("Nao possui limite para emprestimo!\n");
        return;
    }

    printf("Limite disponivel: R$ %.2f\n", limite);

    printf("Valor desejado: R$ ");
    scanf("%f", &valor);

    if (valor <= 0 || valor > limite) {
        printf("Valor fora do limite!\n");
        return;
    }

    clientes[cliente].contas[corrente].saldo += valor;
    clientes[cliente].possuiEmprestimo = 1;

    sprintf(texto, "Emprestimo: R$ %.2f\n", valor);
    registrarHistorico(clientes, cliente, texto);

    printf("Emprestimo aprovado!\n");
}

// COMPRA DE TITULO
void comprarTitulo(Cliente clientes[], int quantidade) {

    int cliente, salario;

    printf("Numero do cliente: ");
    scanf("%d", &cliente);

    if (cliente < 1 || cliente > quantidade) {
        printf("Cliente invalido!\n");
        return;
    }

    cliente--;

    if (clientes[cliente].possuiTitulo == 1) {
        printf("Cliente ja possui titulo!\n");
        return;
    }

    salario = encontrarConta(clientes[cliente], 2);

    if (salario == -1) {
        printf("Cliente nao possui conta salario!\n");
        return;
    }

    if (clientes[cliente].contas[salario].saldo < 1000 ||
        clientes[cliente].contas[salario].saldo > 1500) {

        printf("Saldo fora do limite permitido!\n");
        return;
    }

    clientes[cliente].possuiTitulo = 1;

    registrarHistorico(clientes, cliente, "Titulo de capitalizacao adquirido.\n");

    printf("Titulo adquirido com sucesso!\n");
}

// ALERTA DE SALDO NEGATIVO
void verificarSaldoNegativo(Cliente clientes[], int quantidade) {

    int i, conta;

    for (i = 0; i < quantidade; i++) {

        conta = encontrarConta(clientes[i], 1);

        if (conta != -1 &&
            clientes[i].contas[conta].saldo < 0) {

            printf("\nATENCAO: %s possui saldo negativo!\n",
                clientes[i].nome);
        }
    }
}

// HISTORICO DO BANCO
void consultarHistorico(Cliente clientes[], int quantidade) {

    int i, corrente, poupanca, salario;
    float totalBanco = 0;

    for (i = 0; i < quantidade; i++) {
        totalBanco += calcularSaldo(clientes[i]);
    }

    printf("\n===== HISTORICO DO BANCO =====\n");

    printf("\nSALDO TOTAL DO BANCO\n");
    printf("%d clientes | R$ %.2f\n", quantidade, totalBanco);

    printf("\nCLIENTES APTOS A EMPRESTIMOS\n");

    for (i = 0; i < quantidade; i++) {

        corrente = encontrarConta(clientes[i], 1);
        poupanca = encontrarConta(clientes[i], 3);

        if (corrente != -1 && poupanca != -1 &&
            clientes[i].contas[poupanca].saldo > 0 &&
            clientes[i].possuiEmprestimo == 0) {

            printf("%s | %04d-01 | R$ %.2f\n",
                clientes[i].nome,
                clientes[i].contas[corrente].numero,
                calcularSaldo(clientes[i]) * 2.5);
        }
    }

    printf("\nCLIENTES COM SALDO NEGATIVO NA CONTA CORRENTE\n");

    for (i = 0; i < quantidade; i++) {

        corrente = encontrarConta(clientes[i], 1);

        if (corrente != -1 &&
            clientes[i].contas[corrente].saldo < 0) {

            printf("%s | R$ %.2f\n",
                clientes[i].nome,
                clientes[i].contas[corrente].saldo);
        }
    }

    printf("\nCLIENTES APTOS A COMPRAR TITULOS\n");

    for (i = 0; i < quantidade; i++) {

        salario = encontrarConta(clientes[i], 2);

        if (salario != -1 &&
            clientes[i].contas[salario].saldo >= 1000 &&
            clientes[i].contas[salario].saldo <= 1500 &&
            clientes[i].possuiTitulo == 0) {

            printf("%s | R$ %.2f\n",
                clientes[i].nome,
                clientes[i].contas[salario].saldo);
        }
    }

    printf("\nCLIENTES CONTEMPLADOS NO SORTEIO\n");

    for (i = 0; i < quantidade; i++) {

        if (clientes[i].premio > 0) {

            printf("%s | Premio: R$ %.2f\n",
                clientes[i].nome,
                clientes[i].premio);
        }
    }

    printf("\nMOVIMENTACOES DOS CLIENTES\n");

    for (i = 0; i < quantidade; i++) {

        printf("\n%s:\n", clientes[i].nome);

        if (strlen(clientes[i].historico) == 0) {
            printf("Nenhuma movimentacao.\n");
        } else {
            printf("%s", clientes[i].historico);
        }
    }
}

// SORTEIO BASEADO EM FIBONACCI
void efetuarSorteio(Cliente clientes[], int quantidade) {

    int ordem[5];
    float saldos[5];

    int i, j, posicao, temp;
    float troca;

    int a = 0, b = 1, proximo;
    int passos;
    int vencedor;
    int contaPremio = -1;

    float premio;

    if (quantidade < 5) {
        printf("Sao necessarios cinco clientes!\n");
        return;
    }

    // Encontra os cinco maiores saldos

    for (i = 0; i < 5; i++) {
        ordem[i] = -1;
        saldos[i] = -999999999;
    }

    for (i = 0; i < quantidade; i++) {

        float total = calcularSaldo(clientes[i]);

        posicao = 5;

        for (j = 0; j < 5; j++) {

            if (total > saldos[j]) {
                posicao = j;
                break;
            }
        }

        if (posicao < 5) {

            for (j = 4; j > posicao; j--) {
                saldos[j] = saldos[j - 1];
                ordem[j] = ordem[j - 1];
            }

            saldos[posicao] = total;
            ordem[posicao] = i;
        }
    }

    // Verifica se os cinco possuem titulo

    for (i = 0; i < 5; i++) {

        if (ordem[i] == -1 ||
            clientes[ordem[i]].possuiTitulo == 0) {

            printf("Sorteio cancelado: um dos cinco maiores saldos nao possui titulo!\n");
            return;
        }
    }

    // Gera um numero de Fibonacci

    passos = (int)(time(NULL) % 10) + 1;

    for (i = 0; i < passos; i++) {

        proximo = (a + b) % 5;
        a = b;
        b = proximo;
    }

    // Escolhe o vencedor

    vencedor = ordem[a % 5];

    // Encontra a conta de maior saldo do vencedor

    for (i = 0; i < 3; i++) {

        if (clientes[vencedor].contas[i].tipo != 0) {

            if (contaPremio == -1 ||
                clientes[vencedor].contas[i].saldo >
                clientes[vencedor].contas[contaPremio].saldo) {

                contaPremio = i;
            }
        }
    }

    if (contaPremio == -1) {
        printf("Vencedor sem conta cadastrada!\n");
        return;
    }

    premio = clientes[vencedor].contas[contaPremio].saldo * 10;

    clientes[vencedor].contas[contaPremio].saldo += premio;
    clientes[vencedor].premio += premio;

    printf("\n===== RESULTADO DO SORTEIO =====\n");
    printf("Vencedor: %s\n", clientes[vencedor].nome);
    printf("Premio: R$ %.2f\n", premio);

    char texto[100];

    sprintf(texto, "Premio do sorteio: R$ %.2f\n", premio);
    registrarHistorico(clientes, vencedor, texto);
}

// MENU DE OPERACOES
void menuOperacoes(Cliente clientes[], int quantidade) {

    int opcao;

    do {

        verificarSaldoNegativo(clientes, quantidade);

        printf("\n===== OPERACOES DO BANCO =====\n");
        printf("1 - Depositar\n");
        printf("2 - Sacar\n");
        printf("3 - Solicitar emprestimo\n");
        printf("4 - Comprar titulo\n");
        printf("5 - Consultar saldo\n");
        printf("0 - Voltar ao menu principal\n");
        printf("Escolha: ");

        scanf("%d", &opcao);

        switch (opcao) {

            case 1:
                depositar(clientes, quantidade);
                break;

            case 2:
                sacar(clientes, quantidade);
                break;

            case 3:
                solicitarEmprestimo(clientes, quantidade);
                break;

            case 4:
                comprarTitulo(clientes, quantidade);
                break;

            case 5:
                consultarSaldo(clientes, quantidade);
                break;

            case 0:
                printf("Voltando...\n");
                break;

            default:
                printf("Opcao invalida!\n");
        }

    } while (opcao != 0);
}
