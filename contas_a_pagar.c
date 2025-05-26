#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <time.h>

#define MAX_CONTAS 100

typedef struct {
    int documento;
    char descricao[30];
    char beneficio[20];
    float valor;
    char vencimento[11];
} TConta;

FILE *pconta;
TConta conta_aux, conta_nula = {0};

/*void linha() {
   // for (int i = 1; i <= 40; i++)
        printf("_");
    printf("\n");}*/


void cabec(const char *titulo) {
    system("cls"); // Para Windows, use system("clear") para Linux.
    //linha();
    printf("\nXYZ COMERCIO DE PRODUTOS LTDA.\n");
    printf("SISTEMA DE CONTAS A PAGAR - %s\n", titulo);
    //linha();
}

void abre_arquivo() {
    pconta = fopen("contas.txt", "r+b");
    if (pconta == NULL) {
        pconta = fopen("contas.txt", "w+b");
        if (pconta == NULL) {
            perror("Erro ao abrir arquivo");
            exit(EXIT_FAILURE);
        }
    }
}

int procura(int docp) {
    int pos = 0;
    rewind(pconta);
    while (fread(&conta_aux, sizeof(TConta), 1, pconta)) {
        if (conta_aux.documento == docp)
            return pos;
        pos++;
    }
    return -1;
}

void mostrar_conta(int pos) {
    fseek(pconta, pos * sizeof(TConta), SEEK_SET);
    fread(&conta_aux, sizeof(TConta), 1, pconta);

   // linha();
    printf("Documento    Descrição    Beneficiário    Valor     Vencimento\n");
   // linha();
    printf("%5d %10s %15s %13.2f %15s\n",
           conta_aux.documento, conta_aux.descricao, conta_aux.beneficio,
           conta_aux.valor, conta_aux.vencimento);
    //linha();
}

void incluir_conta() {
    char resp;
    do {
        cabec("INCLUSÃO DE CONTA");
        printf("\nDocumento: ");
        scanf("%d", &conta_aux.documento);

        printf("Descrição: ");
        getchar(); // Limpar buffer.
        fgets(conta_aux.descricao, sizeof(conta_aux.descricao), stdin);
        conta_aux.descricao[strcspn(conta_aux.descricao, "\n")] = '\0';

        printf("Beneficiário: ");
        fgets(conta_aux.beneficio, sizeof(conta_aux.beneficio), stdin);
        conta_aux.beneficio[strcspn(conta_aux.beneficio, "\n")] = '\0';

        printf("Valor: ");
        scanf("%f", &conta_aux.valor);

        printf("Vencimento (dd/mm/yyyy): ");
        getchar();
        fgets(conta_aux.vencimento, sizeof(conta_aux.vencimento), stdin);
        conta_aux.vencimento[strcspn(conta_aux.vencimento, "\n")] = '\0';

        printf("\nConfirma inclusão (S/N)? ");
        getchar();
        resp = getchar();
        if (resp == 'S' || resp == 's') {
            fseek(pconta, 0, SEEK_END);
            fwrite(&conta_aux, sizeof(TConta), 1, pconta);
            printf("\nConta incluída com sucesso!\n");
        }
        printf("\nNova inclusão (S/N)? ");
        getchar();
        resp = getchar();
    } while (resp == 'S' || resp == 's');
}

void consultar_conta() {
    int doccon, posicao;
    char resp;
    do {
        cabec("\nCONSULTA DE CONTA\n");
        printf("\nDocumento: ");
        scanf("%d", &doccon);

        posicao = procura(doccon);
        if (posicao == -1)
            printf("\n\nDocumento não encontrado!\n\n");
        else
            mostrar_conta(posicao);

        printf("\nNova consulta (S/N)? ");
        getchar();
        resp = getchar();
    } while (resp == 'S' || resp == 's');
}

void excluir_conta() {
    int documento_excluir, posicao;
    char resp;
    do {
        cabec("EXCLUSÃO DE CONTA");
        printf("\nDocumento da conta: ");
        scanf("%d", &documento_excluir);

        posicao = procura(documento_excluir);
        if (posicao == -1) {
            printf("\nDocumento não encontrado!\n");
        } else {
            mostrar_conta(posicao);
            printf("\nConfirma exclusão (S/N)? ");
            getchar();
            resp = getchar();
            if (resp == 'S' || resp == 's') {
                fseek(pconta, posicao * sizeof(TConta), SEEK_SET);
                fwrite(&conta_nula, sizeof(TConta), 1, pconta);
                printf("\nConta excluída com sucesso!\n");
            } else {
                printf("\nExclusão cancelada!\n");
            }
        }
        printf("\nExcluir outra conta (S/N)? ");
        getchar();
        resp = getchar();
    } while (resp == 'S' || resp == 's');
}

void alterar_conta() {
    int documento_alterar, posicao;
    char resp;
    do {
        cabec("ALTERAÇÃO DE CONTA");
        printf("\nDocumento da conta: ");
        scanf("%d", &documento_alterar);

        posicao = procura(documento_alterar);
        if (posicao == -1) {
            printf("\nDocumento não encontrado!\n");
        } else {
            mostrar_conta(posicao);

            printf("\nConfirma alteração (S/N)? ");
            getchar();
            resp = getchar();
            if (resp == 'S' || resp == 's') {
                printf("\nNova descrição: ");
                getchar();
                fgets(conta_aux.descricao, sizeof(conta_aux.descricao), stdin);
                conta_aux.descricao[strcspn(conta_aux.descricao, "\n")] = '\0';

                 printf("\nBeneficio: ");
                getchar();
                fgets(conta_aux.beneficio, sizeof(conta_aux.beneficio), stdin);
                conta_aux.beneficio[strcspn(conta_aux.beneficio, "\n")] = '\0';

                printf("Novo valor: ");
                scanf("%f", &conta_aux.valor);

                printf("Novo vencimento (dd/mm/yyyy): ");
                getchar();
                fgets(conta_aux.vencimento, sizeof(conta_aux.vencimento), stdin);
                conta_aux.vencimento[strcspn(conta_aux.vencimento, "\n")] = '\0';

                fseek(pconta, posicao * sizeof(TConta), SEEK_SET);
                fwrite(&conta_aux, sizeof(TConta), 1, pconta);
                printf("\nConta alterada com sucesso!\n");
            } else {
                printf("\nAlteração cancelada!\n");
            }
        }
        printf("\nAlterar outra conta (S/N)? ");
        getchar();
        resp = getchar();
    } while (resp == 'S' || resp == 's');
}

void menu_cadastro() {
    char opcao;
    do {
        cabec("CADASTRO DE CONTAS");
        printf("1 - Inclusão\n");
        printf("2 - Alteração\n");
        printf("3 - Consulta\n");
        printf("4 - Exclusão\n");
        printf("0 - Retornar ao menu principal\n");
        printf("Opção: ");
        scanf(" %c", &opcao);

        switch (opcao) {
            case '1':
                incluir_conta();
                break;
            case '2':
                alterar_conta();
                break;
            case '3':
                consultar_conta();
                break;
            case '4':
                excluir_conta();
                break;
            case '0':
                break;
            default:
                printf("\nOpção inválida!\n");
        }
    } while (opcao != '0');
}
void contas_a_pagar() {
    cabec("CONTAS A PAGAR");
    printf("Documento    Descrição    Beneficiário    Valor    Vencimento\n");
    //linha();
    rewind(pconta);
    while (fread(&conta_aux, sizeof(TConta), 1, pconta)) {
        if (conta_aux.documento != 0 && conta_aux.valor > 0) {
            printf("%5d %12s %13s %14.2f %13s\n", conta_aux.documento, conta_aux.descricao, conta_aux.beneficio, conta_aux.valor, conta_aux.vencimento);
        }
    }
   // linha();
    printf("\nTecle enter para voltar...");
    getchar(); getchar();
}

void contas_atrasadas() {
    time_t t = time(NULL);
    struct tm *local = localtime(&t);
    char data_atual[11];
    strftime(data_atual, sizeof(data_atual), "%d/%m/%Y", local);

    cabec("CONTAS ATRASADAS");
    printf("Documento    Descrição    Beneficiário    Valor    Vencimento\n");
   // linha();
    rewind(pconta);
    while (fread(&conta_aux, sizeof(TConta), 1, pconta)) {
        if (conta_aux.documento != 0 && conta_aux.valor > 0 && strcmp(conta_aux.vencimento, data_atual) < 0) {
            printf("%5d %12s %13s %14.2f %13s\n", conta_aux.documento, conta_aux.descricao, conta_aux.beneficio, conta_aux.valor, conta_aux.vencimento);
        }
    }
    //linha();
    printf("\nTecle enter para voltar...");
    getchar(); getchar();
}
void balanco_despesas() {
    float total_despesas[12] = {0};
    char beneficiario_[12][20] = {0};
    float maior_valor[12] = {0};
    int dia, mes, ano;

    cabec("BALANÇO DE DESPESAS NO PERÍODO (JANEIRO A DEZEMBRO)");
    rewind(pconta);

    while (fread(&conta_aux, sizeof(TConta), 1, pconta)) {
        if (conta_aux.documento != 0 && conta_aux.valor > 0) {
            sscanf(conta_aux.vencimento, "%d/%d/%d", &dia, &mes, &ano);
            mes--;
            total_despesas[mes] += conta_aux.valor;
        if (conta_aux.valor > maior_valor[mes]) {
                maior_valor[mes] = conta_aux.valor;
            if (conta_aux.beneficio[0] != '\0') {
                    strcpy(beneficiario_[mes], conta_aux.beneficio);
              } else {
                    beneficiario_[mes][0] = '\0';
        }
     }
   }
}

    printf("MÊS       TOTAL DE DESPESAS        BENEFICIÁRIO\n");
    for (int i = 0; i < 12; i++) {
        char *mes_nome;
        switch(i) {
            case 0: mes_nome = "Janeiro"; break;
            case 1: mes_nome = "Fevereiro"; break;
            case 2: mes_nome = "Março"; break;
            case 3: mes_nome = "Abril"; break;
            case 4: mes_nome = "Maio"; break;
            case 5: mes_nome = "Junho"; break;
            case 6: mes_nome = "Julho"; break;
            case 7: mes_nome = "Agosto"; break;
            case 8: mes_nome = "Setembro"; break;
            case 9: mes_nome = "Outubro"; break;
            case 10: mes_nome = "Novembro"; break;
            case 11: mes_nome = "Dezembro"; break;
        }

        printf("%-10s R$ %.2f %20s\n", mes_nome, total_despesas[i], beneficiario_[i][0] == '\0' ? "" : beneficiario_[i]);
    }
    printf("\nTecle enter para voltar...");
    getchar(); getchar();
}

void relatorio() {
    char opcao;
    do {
        cabec("RELATÓRIOS");
    printf("1 - Contas a Pagar\n");
    printf("2 - Contas Atrasadas\n");
    printf("3 - Balanço de Despesas\n");
    printf("0 - Retornar\n");
    printf("Opção: ");
    scanf(" %c", &opcao);

        switch (opcao) {
            case '1':
                contas_a_pagar();
                break;
            case '2':
                contas_atrasadas();
                break;
            case '3':
                balanco_despesas();
                break;
            case '0':
                break;
            default:
                printf("\nOpção inválida!\n");
        }
    } while (opcao != '0');
}
void baixa_conta() {
    int documento_baixa, posicao;
    float valor_pago;
    char resp;

    do {
        cabec("BAIXA DE CONTAS");
    printf("\nDocumento da conta: ");
    scanf("%d", &documento_baixa);

      posicao = procura(documento_baixa);
    if (posicao == -1) {
            printf("\nDocumento não encontrado!\n");
    } else {
        mostrar_conta(posicao);

    printf("\nDigite o valor pago: ");
    scanf("%f", &valor_pago);

    if (valor_pago <= 0 || valor_pago > conta_aux.valor) {
    printf("\nValor pago inválido!\n");
    } else {
    printf("\nConfirma baixa (S/N)? ");
    getchar();
        resp = getchar();
    if (resp == 'S' || resp == 's') {
        conta_aux.valor -= valor_pago;
        fseek(pconta, posicao * sizeof(TConta), SEEK_SET);
        fwrite(&conta_aux, sizeof(TConta), 1, pconta);
        printf("\nBaixa realizada com sucesso!\n");
    } else {
        printf("\nOperação cancelada.\n");
 }
}
}
    printf("\nRealizar outra baixa (S/N)? ");
    getchar();
        resp = getchar();
    } while (resp == 'S' || resp == 's');
}

int main() {
    char opcao;

    abre_arquivo();
    setlocale(LC_ALL, "Portuguese");

    do {
        cabec("MENU PRINCIPAL");
        printf("\n1 - Cadastro de contas\n");
        printf("\n2 - baixa de contas\n");
        printf("\n3 - relatorio\n");
        printf("\n0 - Finalizar\n");
        printf("\n\nOpção:");
        scanf(" %c", &opcao);

        switch (opcao) {
            case '1':
                menu_cadastro();
                break;
            case '2':
                baixa_conta();
                break;
            case '3':
                relatorio();
                break;
            case '0':
                fclose(pconta);
                printf("\nSaindo...\n");
                break;
            default:
                printf("\nOpção inválida!\n");
        }
    } while (opcao != '0');

    return 0;
}