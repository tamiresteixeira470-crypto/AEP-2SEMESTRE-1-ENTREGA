#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int codigo;
    char escola[100];
    char tema[100];
    char data[20];
    char publico[100];
    char responsavel[100];
    int qtdPrevista;
    int qtdEfetiva;
    int situacao;
} Acao;

void cadastrarAcao(Acao acoes[], int *total) {
    if (*total >= 100) {
        printf("Limite de acoes atingido!\n");
        return;
    }

    printf("Codigo da acao (numerico): ");
    scanf("%d", &acoes[*total].codigo);

    printf("Escola: ");
    scanf(" %[^\n]", acoes[*total].escola);

    printf("Tema: ");
    scanf(" %[^\n]", acoes[*total].tema);

    printf("Data Prevista: ");
    scanf(" %[^\n]", acoes[*total].data);

    printf("Publico-alvo: ");
    scanf(" %[^\n]", acoes[*total].publico);

    printf("Responsavel: ");
    scanf(" %[^\n]", acoes[*total].responsavel);

    printf("Qtd Prevista de Participantes: ");
    scanf("%d", &acoes[*total].qtdPrevista);

    acoes[*total].situacao = 1;
    acoes[*total].qtdEfetiva = 0;
    *total = *total + 1;

    printf("Acao cadastrada com sucesso!\n");
}

void listarAcoes(Acao acoes[], int total) {
    if (total == 0) {
        printf("Nenhuma acao cadastrada.\n");
        return;
    }
    
    for (int i = 0; i < total; i++) {
        printf("\n[Codigo: %d] Escola: %s | Tema: %s", acoes[i].codigo, acoes[i].escola, acoes[i].tema);
        printf("\nSituacao: %d (1-Plan, 2-Realizada, 3-Cancelada)\n", acoes[i].situacao);
    }
}

void pesquisarAcao(Acao acoes[], int total) {
    int buscaCod;
    int achou = 0;

    printf("Digite o codigo da acao: ");
    scanf("%d", &buscaCod);

    for (int i = 0; i < total; i++) {
        if (acoes[i].codigo == buscaCod) {
            printf("\nAcao encontrada! Escola: %s - Tema: %s\n", acoes[i].escola, acoes[i].tema);
            achou = 1;
        }
    }

    if (achou == 0) {
        printf("Nao encontrada.\n");
    }
}

void atualizarSituacao(Acao acoes[], int total) {
    int codAtualiza;
    int achou = 0;

    printf("Codigo da acao a ser atualizada: ");
    scanf("%d", &codAtualiza);

    for (int i = 0; i < total; i++) {
        if (acoes[i].codigo == codAtualiza) {
            achou = 1;
            printf("Nova Situacao (1-Planejada, 2-Realizada, 3-Cancelada): ");
            scanf("%d", &acoes[i].situacao);

            if (acoes[i].situacao == 2) {
                printf("Quantidade efetiva de participantes: ");
                scanf("%d", &acoes[i].qtdEfetiva);
            }
            printf("Situacao atualizada!\n");
        }
    }

    if (achou == 0) {
        printf("Nao encontrada.\n");
    }
}

void resumoGeral(Acao acoes[], int total) {
    int cPlan = 0, cReal = 0, cCanc = 0;
    int sPrev = 0, sEfet = 0;

    // Protecao: se tentar abrir o resumo sem dados, avisa o usuario
    if (total == 0) {
        printf("\n--- RESUMO ---");
        printf("\nNenhuma acao cadastrada para gerar resumo.\n");
        return;
    }

    for (int i = 0; i < total; i++) {
        if (acoes[i].situacao == 1) { 
            cPlan++; 
        }
        else if (acoes[i].situacao == 2) {
            cReal++;
            sEfet += acoes[i].qtdEfetiva;
        }
        else if (acoes[i].situacao == 3) { 
            cCanc++; 
        }
        
        // As acoes canceladas saem da soma de previsao para nao quebrar o percentual final
        if (acoes[i].situacao != 3) { 
            sPrev += acoes[i].qtdPrevista; 
        }
    }

    printf("\n--- RESUMO GERAL ---");
    printf("\nPlanejadas: %d | Realizadas: %d | Canceladas: %d", cPlan, cReal, cCanc);
    printf("\nParticipantes Previstos (acoes validas): %d", sPrev);
    printf("\nParticipantes Efetivos: %d", sEfet);

    if (sPrev > 0) {
        // Variavel float dedicada para evitar qualquer problema no calculo da divisao
        float percentual = (sEfet * 100.0) / (float)sPrev;
        printf("\nPercentual Alcancado: %.2f%%\n", percentual);
    } else {
        printf("\nPercentual Alcancado: 0.00%%\n");
    }
}

int main() {
    Acao acoes[100];
    int totalAcoes = 0;
    int opcao = 0;

    while (opcao != 6) {
        printf("\n=== SISTEMA DE ACOES PSE - SAUDE MENTAL ===");
        printf("\n1. Cadastrar Acao");
        printf("\n2. Listar Acoes");
        printf("\n3. Pesquisar Acao por Codigo");
        printf("\n4. Atualizar Situacao da Acao");
        printf("\n5. Resumo Geral");
        printf("\n6. Sair");
        printf("\nEscolha uma opcao: ");
        scanf("%d", &opcao);

        if (opcao == 1) {
            cadastrarAcao(acoes, &totalAcoes);
        } 
        else if (opcao == 2) {
            listarAcoes(acoes, totalAcoes);
        } 
        else if (opcao == 3) {
            pesquisarAcao(acoes, totalAcoes);
        } 
        else if (opcao == 4) {
            atualizarSituacao(acoes, totalAcoes);
        } 
        else if (opcao == 5) {
            resumoGeral(acoes, totalAcoes);
        }
    }

    return 0;
}
