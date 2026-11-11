#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

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

/* Le uma linha e garante que o usuario digitou apenas numeros inteiros (rejeita letras misturadas como "12a") */
int lerInteiro(void) {
    char buffer[100];
    int i, inicio, fimTexto, valido;
    long valor;
    char *fimConversao;

    while (1) {
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            printf("\nEntrada encerrada.\n");
            exit(0);
        }

        buffer[strcspn(buffer, "\r\n")] = '\0';

        /* Ignora espacos em branco no inicio */
        inicio = 0;
        while (buffer[inicio] == ' ' || buffer[inicio] == '\t') {
            inicio++;
        }

        /* Remove espacos em branco no final */
        fimTexto = (int)strlen(buffer) - 1;
        while (fimTexto >= inicio && (buffer[fimTexto] == ' ' || buffer[fimTexto] == '\t')) {
            buffer[fimTexto] = '\0';
            fimTexto--;
        }

        if (buffer[inicio] == '\0') {
            printf("Formato invalido! Nada foi digitado. Digite um numero inteiro: ");
            continue;
        }

        valido = 1;
        i = inicio;
        if (buffer[i] == '-' || buffer[i] == '+') {
            i++;
            if (buffer[i] == '\0') {
                valido = 0;
            }
        }

        for (; buffer[i] != '\0'; i++) {
            if (!isdigit((unsigned char)buffer[i])) {
                valido = 0;
                break;
            }
        }

        if (!valido) {
            printf("Formato invalido! O que foi digitado ('%s') esta errado. Digite apenas numeros inteiros: ", buffer + inicio);
            continue;
        }

        valor = strtol(buffer + inicio, &fimConversao, 10);
        return (int)valor;
    }
}

/* Garante que o numero inteiro digitado seja maior que zero (> 0) */
int lerInteiroPositivo(void) {
    int valor;
    while (1) {
        valor = lerInteiro();
        if (valor <= 0) {
            printf("Valor invalido! O numero digitado (%d) esta errado. Digite um valor maior que zero: ", valor);
        } else {
            return valor;
        }
    }
}

/* Garante que o numero inteiro digitado seja zero ou maior (>= 0) */
int lerInteiroNaoNegativo(void) {
    int valor;
    while (1) {
        valor = lerInteiro();
        if (valor < 0) {
            printf("Valor invalido! O numero digitado (%d) esta errado. Nao sao permitidos valores negativos: ", valor);
        } else {
            return valor;
        }
    }
}

/* Le textos validando se nao estao vazios e se possuem letras (impede digitar apenas numeros ou simbolos) */
void lerTextoValidado(char *destino, int tamanho) {
    int i, temLetra;

    while (1) {
        if (fgets(destino, tamanho, stdin) == NULL) {
            printf("\nEntrada encerrada.\n");
            exit(0);
        }

        destino[strcspn(destino, "\r\n")] = '\0';

        if (strlen(destino) < 2) {
            printf("Formato invalido! O texto digitado e muito curto ou vazio. Digite novamente: ");
            continue;
        }

        temLetra = 0;
        for (i = 0; destino[i] != '\0'; i++) {
            if (isalpha((unsigned char)destino[i]) || (unsigned char)destino[i] >= 128) {
                temLetra = 1;
                break;
            }
        }

        if (!temLetra) {
            printf("Formato invalido! O que foi digitado ('%s') esta errado. Este campo exige texto com letras: ", destino);
            continue;
        }

        return;
    }
}

/* Le nomes de pessoas garantindo que contenham letras e nenhum numero */
void lerNomeResponsavel(char *destino, int tamanho) {
    int i, temLetra, temNumero;

    while (1) {
        if (fgets(destino, tamanho, stdin) == NULL) {
            printf("\nEntrada encerrada.\n");
            exit(0);
        }

        destino[strcspn(destino, "\r\n")] = '\0';

        if (strlen(destino) < 2) {
            printf("Formato invalido! O nome digitado e muito curto ou vazio. Digite novamente: ");
            continue;
        }

        temLetra = 0;
        temNumero = 0;
        for (i = 0; destino[i] != '\0'; i++) {
            if (isdigit((unsigned char)destino[i])) {
                temNumero = 1;
            }
            if (isalpha((unsigned char)destino[i]) || (unsigned char)destino[i] >= 128) {
                temLetra = 1;
            }
        }

        if (temNumero || !temLetra) {
            printf("Formato invalido! O nome digitado ('%s') esta errado (use apenas letras, sem numeros): ", destino);
            continue;
        }

        return;
    }
}

/* Verifica se a string esta no formato DD/MM/AAAA e se representa uma data real no calendario */
int validarData(const char *data) {
    int dia, mes, ano, i;
    int diasNoMes[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    if (strlen(data) != 10) return 0;
    if (data[2] != '/' || data[5] != '/') return 0;

    for (i = 0; i < 10; i++) {
        if (i == 2 || i == 5) continue;
        if (!isdigit((unsigned char)data[i])) return 0;
    }

    dia = (data[0] - '0') * 10 + (data[1] - '0');
    mes = (data[3] - '0') * 10 + (data[4] - '0');
    ano = (data[6] - '0') * 1000 + (data[7] - '0') * 100 + (data[8] - '0') * 10 + (data[9] - '0');

    if (ano < 1900 || ano > 2100) return 0;
    if (mes < 1 || mes > 12) return 0;

    /* Ajuste para ano bissexto */
    if ((ano % 4 == 0 && ano % 100 != 0) || (ano % 400 == 0)) {
        diasNoMes[2] = 29;
    }

    if (dia < 1 || dia > diasNoMes[mes]) return 0;

    return 1;
}

/* Le e valida uma data obrigatoriamente no padrao DD/MM/AAAA */
void lerDataValidada(char *destino, int tamanho) {
    while (1) {
        if (fgets(destino, tamanho, stdin) == NULL) {
            printf("\nEntrada encerrada.\n");
            exit(0);
        }

        destino[strcspn(destino, "\r\n")] = '\0';

        if (!validarData(destino)) {
            printf("Formato de data invalido! O que foi digitado ('%s') esta errado. Use DD/MM/AAAA (ex: 15/08/2026): ", destino);
            continue;
        }

        return;
    }
}

/* Verifica se um codigo ja existe no vetor de acoes */
int codigoExiste(Acao acoes[], int total, int codigo) {
    int i;
    for (i = 0; i < total; i++) {
        if (acoes[i].codigo == codigo) {
            return 1;
        }
    }
    return 0;
}

const char* obterTextoSituacao(int situacao) {
    if (situacao == 1) return "Planejada";
    if (situacao == 2) return "Realizada";
    if (situacao == 3) return "Cancelada";
    return "Desconhecida";
}

void cadastrarAcao(Acao acoes[], int *total) {
    int novoCodigo;

    if (*total >= 100) {
        printf("Limite de acoes atingido!\n");
        return;
    }

    /* Le o codigo e impede codigos repetidos ou invalidos */
    while (1) {
        printf("Codigo da acao (numerico): ");
        novoCodigo = lerInteiroPositivo();

        if (codigoExiste(acoes, *total, novoCodigo)) {
            printf("Erro: O codigo %d ja esta cadastrado! Digite um codigo diferente.\n", novoCodigo);
        } else {
            acoes[*total].codigo = novoCodigo;
            break;
        }
    }

    printf("Escola: ");
    lerTextoValidado(acoes[*total].escola, sizeof(acoes[*total].escola));

    printf("Tema: ");
    lerTextoValidado(acoes[*total].tema, sizeof(acoes[*total].tema));

    printf("Data Prevista (DD/MM/AAAA): ");
    lerDataValidada(acoes[*total].data, sizeof(acoes[*total].data));

    printf("Publico-alvo: ");
    lerTextoValidado(acoes[*total].publico, sizeof(acoes[*total].publico));

    printf("Responsavel: ");
    lerNomeResponsavel(acoes[*total].responsavel, sizeof(acoes[*total].responsavel));

    printf("Qtd Prevista de Participantes: ");
    acoes[*total].qtdPrevista = lerInteiroPositivo();

    acoes[*total].situacao = 1;
    acoes[*total].qtdEfetiva = 0;
    *total = *total + 1;

    printf("Acao cadastrada com sucesso!\n");
}

void listarAcoes(Acao acoes[], int total) {
    int i;

    if (total == 0) {
        printf("Nenhuma acao cadastrada.\n");
        return;
    }

    for (i = 0; i < total; i++) {
        printf("\n[Codigo: %d] Escola: %s | Tema: %s",
               acoes[i].codigo, acoes[i].escola, acoes[i].tema);
        printf("\nSituacao: %d (%s)\n",
               acoes[i].situacao, obterTextoSituacao(acoes[i].situacao));
    }
}

void visualizarAcaoIndice(Acao acoes[], int i) {
    printf("\n--- Detalhes da Acao [Codigo: %d] ---", acoes[i].codigo);
    printf("\nEscola: %s", acoes[i].escola);
    printf("\nTema: %s", acoes[i].tema);
    printf("\nData Prevista: %s", acoes[i].data);
    printf("\nPublico-alvo: %s", acoes[i].publico);
    printf("\nResponsavel: %s", acoes[i].responsavel);
    printf("\nSituacao: %d (%s)", acoes[i].situacao, obterTextoSituacao(acoes[i].situacao));
    printf("\nParticipantes Previstos: %d", acoes[i].qtdPrevista);
    printf("\nParticipantes Efetivos: %d\n", acoes[i].qtdEfetiva);
}

void editarAcaoIndice(Acao acoes[], int i) {
    printf("\n--- Editando Acao [Codigo: %d] ---\n", acoes[i].codigo);

    printf("Nova Escola (Atual: %s): ", acoes[i].escola);
    lerTextoValidado(acoes[i].escola, sizeof(acoes[i].escola));

    printf("Novo Tema (Atual: %s): ", acoes[i].tema);
    lerTextoValidado(acoes[i].tema, sizeof(acoes[i].tema));

    printf("Nova Data Prevista DD/MM/AAAA (Atual: %s): ", acoes[i].data);
    lerDataValidada(acoes[i].data, sizeof(acoes[i].data));

    printf("Novo Publico-alvo (Atual: %s): ", acoes[i].publico);
    lerTextoValidado(acoes[i].publico, sizeof(acoes[i].publico));

    printf("Novo Responsavel (Atual: %s): ", acoes[i].responsavel);
    lerNomeResponsavel(acoes[i].responsavel, sizeof(acoes[i].responsavel));

    printf("Nova Qtd Prevista de Participantes (Atual: %d): ", acoes[i].qtdPrevista);
    acoes[i].qtdPrevista = lerInteiroPositivo();

    printf("Acao editada com sucesso!\n");
}

void excluirAcaoIndice(Acao acoes[], int *total, int i) {
    int confirma;
    int j;

    while (1) {
        printf("\nConfirmar exclusao da acao [Codigo: %d]? (1-Sim / 0-Nao): ", acoes[i].codigo);
        confirma = lerInteiro();

        if (confirma == 1) {
            for (j = i; j < *total - 1; j++) {
                acoes[j] = acoes[j + 1];
            }
            *total = *total - 1;
            printf("Acao excluida com sucesso!\n");
            break;
        } else if (confirma == 0) {
            printf("Exclusao cancelada.\n");
            break;
        } else {
            printf("Opcao invalida! O valor digitado (%d) esta errado. Digite 1 para Sim ou 0 para Nao.\n", confirma);
        }
    }
}

void pesquisarAcao(Acao acoes[], int *total) {
    int buscaCod;
    int achou = 0;
    int opcaoSub = -1;
    int i;

    if (*total == 0) {
        printf("Nenhuma acao cadastrada.\n");
        return;
    }

    printf("Digite o codigo da acao: ");
    buscaCod = lerInteiroPositivo();

    for (i = 0; i < *total; i++) {
        if (acoes[i].codigo == buscaCod) {
            achou = 1;
            printf("\nAcao [Codigo: %d] encontrada!\n", acoes[i].codigo);

            do {
                printf("\nO que deseja fazer com esta acao?");
                printf("\n1. Visualizar Acao");
                printf("\n2. Editar Acao");
                printf("\n3. Excluir Acao");
                printf("\n0. Voltar ao menu principal");
                printf("\nEscolha uma opcao: ");
                opcaoSub = lerInteiro();

                if (opcaoSub == 1) {
                    visualizarAcaoIndice(acoes, i);
                } else if (opcaoSub == 2) {
                    editarAcaoIndice(acoes, i);
                } else if (opcaoSub == 3) {
                    excluirAcaoIndice(acoes, total, i);
                } else if (opcaoSub != 0) {
                    printf("Opcao invalida! O valor digitado (%d) esta errado. Digite 1, 2, 3 ou 0.\n", opcaoSub);
                }
            } while (opcaoSub < 0 || opcaoSub > 3);

            break;
        }
    }

    if (achou == 0) {
        printf("Acao com codigo %d nao encontrada.\n", buscaCod);
    }
}

void atualizarSituacao(Acao acoes[], int total) {
    int codAtualiza;
    int novaSit;
    int achou = 0;
    int i;

    if (total == 0) {
        printf("Nenhuma acao cadastrada.\n");
        return;
    }

    printf("Codigo da acao a ser atualizada: ");
    codAtualiza = lerInteiroPositivo();

    for (i = 0; i < total; i++) {
        if (acoes[i].codigo == codAtualiza) {
            achou = 1;
            do {
                printf("Nova Situacao (1-Planejada, 2-Realizada, 3-Cancelada): ");
                novaSit = lerInteiro();
                if (novaSit < 1 || novaSit > 3) {
                    printf("Opcao invalida! O valor digitado (%d) esta errado. Escolha 1, 2 ou 3.\n", novaSit);
                }
            } while (novaSit < 1 || novaSit > 3);

            acoes[i].situacao = novaSit;

            if (acoes[i].situacao == 2) {
                printf("Quantidade efetiva de participantes: ");
                acoes[i].qtdEfetiva = lerInteiroNaoNegativo();
            } else {
                acoes[i].qtdEfetiva = 0;
            }
            printf("Situacao atualizada!\n");
            break;
        }
    }

    if (achou == 0) {
        printf("Acao com codigo %d nao encontrada.\n", codAtualiza);
    }
}

void exportarPlanilha(Acao acoes[], int total, int cPlan, int cReal, int cCanc, int sPrev, int sEfet, float percentual) {
    FILE *fp = fopen("resumo_acoes.csv", "w");
    int i;

    if (fp == NULL) {
        printf("Erro ao criar o arquivo de planilha!\n");
        return;
    }

    fprintf(fp, "Codigo;Escola;Tema;Data;Publico-Alvo;Responsavel;Qtd Prevista;Qtd Efetiva;Situacao\n");

    for (i = 0; i < total; i++) {
        fprintf(fp, "%d;%s;%s;%s;%s;%s;%d;%d;%s\n",
                acoes[i].codigo,
                acoes[i].escola,
                acoes[i].tema,
                acoes[i].data,
                acoes[i].publico,
                acoes[i].responsavel,
                acoes[i].qtdPrevista,
                acoes[i].qtdEfetiva,
                obterTextoSituacao(acoes[i].situacao));
    }

    fprintf(fp, "\nRESUMO GERAL;VALORES\n");
    fprintf(fp, "Acoes Planejadas;%d\n", cPlan);
    fprintf(fp, "Acoes Realizadas;%d\n", cReal);
    fprintf(fp, "Acoes Canceladas;%d\n", cCanc);
    fprintf(fp, "Participantes Previstos (acoes validas);%d\n", sPrev);
    fprintf(fp, "Participantes Efetivos;%d\n", sEfet);
    fprintf(fp, "Percentual Alcancado;%.2f%%\n", percentual);

    fclose(fp);
    printf("\nPlanilha 'resumo_acoes.csv' exportada com sucesso!\n");
}

void gerarPDF(Acao acoes[], int total, int cPlan, int cReal, int cCanc, int sPrev, int sEfet, float percentual) {
    FILE *fp = fopen("resumo_acoes.pdf", "wb");
    char conteudo[16384];
    char linha[512];
    long offsets[6];
    long xrefPos;
    int i;

    if (fp == NULL) {
        printf("Erro ao gerar o arquivo PDF!\n");
        return;
    }

    strcpy(conteudo, "BT\n/F1 14 Tf\n50 790 Td\n18 TL\n");
    strcat(conteudo, "(SISTEMA DE ACOES PSE - RESUMO GERAL) Tj\nT*\n");
    strcat(conteudo, "/F1 11 Tf\n15 TL\n");
    strcat(conteudo, "(------------------------------------------------------------) Tj\nT*\n");

    snprintf(linha, sizeof(linha), "(Planejadas: %d | Realizadas: %d | Canceladas: %d) Tj\nT*\n", cPlan, cReal, cCanc);
    strcat(conteudo, linha);

    snprintf(linha, sizeof(linha), "(Participantes Previstos: %d) Tj\nT*\n", sPrev);
    strcat(conteudo, linha);

    snprintf(linha, sizeof(linha), "(Participantes Efetivos: %d) Tj\nT*\n", sEfet);
    strcat(conteudo, linha);

    snprintf(linha, sizeof(linha), "(Percentual Alcancado: %.2f%%) Tj\nT*\n", percentual);
    strcat(conteudo, linha);

    strcat(conteudo, "(------------------------------------------------------------) Tj\nT*\nT*\n");
    strcat(conteudo, "(LISTA DE ACOES CADASTRADAS:) Tj\nT*\n");

    for (i = 0; i < total; i++) {
        snprintf(linha, sizeof(linha),
                 "([Cod: %d] Escola: %s | Tema: %s | Sit: %s | Prev: %d | Efet: %d) Tj\nT*\n",
                 acoes[i].codigo, acoes[i].escola, acoes[i].tema,
                 obterTextoSituacao(acoes[i].situacao),
                 acoes[i].qtdPrevista, acoes[i].qtdEfetiva);

        if (strlen(conteudo) + strlen(linha) + 10 < sizeof(conteudo)) {
            strcat(conteudo, linha);
        }
    }

    strcat(conteudo, "ET");

    fprintf(fp, "%%PDF-1.4\n");

    offsets[1] = ftell(fp);
    fprintf(fp, "1 0 obj\n<< /Type /Catalog /Pages 2 0 R >>\nendobj\n");

    offsets[2] = ftell(fp);
    fprintf(fp, "2 0 obj\n<< /Type /Pages /Kids [3 0 R] /Count 1 >>\nendobj\n");

    offsets[3] = ftell(fp);
    fprintf(fp, "3 0 obj\n<< /Type /Page /Parent 2 0 R /Resources << /Font << /F1 4 0 R >> >> /MediaBox [0 0 595 842] /Contents 5 0 R >>\nendobj\n");

    offsets[4] = ftell(fp);
    fprintf(fp, "4 0 obj\n<< /Type /Font /Subtype /Type1 /BaseFont /Helvetica >>\nendobj\n");

    offsets[5] = ftell(fp);
    fprintf(fp, "5 0 obj\n<< /Length %lu >>\nstream\n%s\nendstream\nendobj\n",
            (unsigned long)strlen(conteudo), conteudo);

    xrefPos = ftell(fp);
    fprintf(fp, "xref\n0 6\n0000000000 65535 f \n");
    for (i = 1; i <= 5; i++) {
        fprintf(fp, "%010ld 00000 n \n", offsets[i]);
    }

    fprintf(fp, "trailer\n<< /Size 6 /Root 1 0 R >>\nstartxref\n%ld\n%%%%EOF\n", xrefPos);

    fclose(fp);
    printf("\nArquivo 'resumo_acoes.pdf' gerado com sucesso!\n");
}

void resumoGeral(Acao acoes[], int total) {
    int cPlan = 0, cReal = 0, cCanc = 0;
    int sPrev = 0, sEfet = 0;
    int i;
    int opcaoExportar = -1;
    float percentual = 0.0f;

    if (total == 0) {
        printf("\n--- RESUMO ---");
        printf("\nNenhuma acao cadastrada para gerar resumo.\n");
        return;
    }

    for (i = 0; i < total; i++) {
        if (acoes[i].situacao == 1) {
            cPlan++;
        } else if (acoes[i].situacao == 2) {
            cReal++;
            sEfet += acoes[i].qtdEfetiva;
        } else if (acoes[i].situacao == 3) {
            cCanc++;
        }

        if (acoes[i].situacao != 3) {
            sPrev += acoes[i].qtdPrevista;
        }
    }

    if (sPrev > 0) {
        percentual = (sEfet * 100.0f) / (float)sPrev;
    }

    printf("\n--- RESUMO GERAL ---");
    printf("\nPlanejadas: %d | Realizadas: %d | Canceladas: %d", cPlan, cReal, cCanc);
    printf("\nParticipantes Previstos (acoes validas): %d", sPrev);
    printf("\nParticipantes Efetivos: %d", sEfet);
    printf("\nPercentual Alcancado: %.2f%%\n", percentual);

    do {
        printf("\nDeseja exportar o resumo?");
        printf("\n1. Exportar em Planilha (.csv)");
        printf("\n2. Gerar PDF (.pdf)");
        printf("\n0. Voltar ao menu principal");
        printf("\nEscolha uma opcao: ");
        opcaoExportar = lerInteiro();

        if (opcaoExportar == 1) {
            exportarPlanilha(acoes, total, cPlan, cReal, cCanc, sPrev, sEfet, percentual);
        } else if (opcaoExportar == 2) {
            gerarPDF(acoes, total, cPlan, cReal, cCanc, sPrev, sEfet, percentual);
        } else if (opcaoExportar != 0) {
            printf("Opcao invalida! O valor digitado (%d) esta errado. Digite 1, 2 ou 0.\n", opcaoExportar);
        }
    } while (opcaoExportar < 0 || opcaoExportar > 2);
}

int main(void) {
    Acao acoes[100];
    int totalAcoes = 0;
    int opcao = 0;

    while (opcao != 6) {
        printf("\n=== SISTEMA DE ACOES PSE - SAUDE MENTAL ===");
        printf("\n1. Cadastrar Acao");
        printf("\n2. Listar Acoes");
        printf("\n3. Pesquisar Acao por Codigo (Visualizar / Editar / Excluir)");
        printf("\n4. Atualizar Situacao da Acao");
        printf("\n5. Resumo Geral (Exportar Planilha / PDF)");
        printf("\n6. Sair");
        printf("\nEscolha uma opcao: ");
        opcao = lerInteiro();

        if (opcao == 1) {
            cadastrarAcao(acoes, &totalAcoes);
        } else if (opcao == 2) {
            listarAcoes(acoes, totalAcoes);
        } else if (opcao == 3) {
            pesquisarAcao(acoes, &totalAcoes);
        } else if (opcao == 4) {
            atualizarSituacao(acoes, totalAcoes);
        } else if (opcao == 5) {
            resumoGeral(acoes, totalAcoes);
        } else if (opcao != 6) {
            printf("Opcao invalida! O valor digitado (%d) esta errado. Escolha de 1 a 6.\n", opcao);
        }
    }

    printf("\nEncerrando o sistema...\n");
    return 0;
}