
#include <stdio.h>
#include "meu_banco.h"

int main() {

    Cliente clientes[MAX_CLIENTES];

    int quantidade = 0;
    int bancoAberto = 0;
    int opcao;

    do {

        if (bancoAberto == 1) {
            verificarSaldoNegativo(clientes, quantidade);
        }

        printf("\n===== BANCO DIGITAL =====\n");
        printf("1 - Abrir banco / Operacoes\n");
        printf("2 - Consultar historico\n");
        printf("3 - Efetuar sorteio\n");
        printf("4 - Sair\n");
        printf("Escolha: ");

        scanf("%d", &opcao);

        switch (opcao) {

            case 1:

                if (bancoAberto == 0) {

                    printf("Quantidade de clientes: ");
                    scanf("%d", &quantidade);

                    if (quantidade < 1 || quantidade > MAX_CLIENTES) {
                        printf("Quantidade invalida!\n");
                        quantidade = 0;
                        break;
                    }

                    abrirBanco(clientes, quantidade);
                    bancoAberto = 1;
                }

                menuOperacoes(clientes, quantidade);

                break;

            case 2:

                if (bancoAberto == 1) {
                    consultarHistorico(clientes, quantidade);
                } else {
                    printf("Abra o banco primeiro!\n");
                }

                break;

            case 3:

                if (bancoAberto == 1) {
                    efetuarSorteio(clientes, quantidade);
                } else {
                    printf("Abra o banco primeiro!\n");
                }

                break;

            case 4:
                printf("Encerrando o programa...\n");
                break;

            default:
                printf("Opcao invalida!\n");
        }

    } while (opcao != 4);

    return 0;
}
