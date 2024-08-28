#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

// Estrutura para representar um produto
typedef struct {
    char nome[50];
    int quantidade;
    char dataValidade[11]; // DD/MM/AAAA
    char doador[50];
} Produto;

// Funções
void cadastrarProduto(Produto produtos[], int *numProdutos);
void imprimirEstoque(Produto produtos[], int numProdutos);
void salvarProdutos(Produto produtos[], int numProdutos);
void carregarProdutos(Produto produtos[], int *numProdutos);
void removerProduto(Produto produtos[], int *numProdutos);

void limparTela() {
    system("clear");
}

void esperarTecla() {
    printf("Pressione Enter para continuar...");
    getchar(); // Aguarda a entrada do usuário
    getchar(); // Aguarda a entrada do usuário
}

void exibirLogin() {
    printf("========================================\n");
    printf("               TELA DE LOGIN            \n");
    printf("========================================\n");
}

void exibirMenu() {
    printf("\n========================================\n");
    printf("                MENU PRINCIPAL          \n");
    printf("========================================\n");
    printf("1. Cadastrar produto\n");
    printf("2. Imprimir estoque\n");
    printf("3. Remover produto\n");
    printf("4. Sair\n");
    printf("Escolha uma opcao: ");
}

int main() {
    Produto produtos[100]; // Array para armazenar os produtos
    int numProdutos = 0;
    int opcao;
    char usuario[20], senha[20];
    const char usuarioValido[] = "admin";
    const char senhaValida[] = "12345";
    int tentativas = 3;

    setlocale(LC_ALL, "Portuguese");

    // Carregar produtos do arquivo
    carregarProdutos(produtos, &numProdutos);

    while (tentativas > 0) {
        limparTela();
        exibirLogin();

        printf("Digite o nome de usuario: ");
        scanf("%19s", usuario); // Limita o tamanho da entrada
        printf("Digite a senha: ");
        scanf("%19s", senha); // Limita o tamanho da entrada

        if (strcmp(usuario, usuarioValido) == 0 && strcmp(senha, senhaValida) == 0) {
            printf("\nLogin bem-sucedido!\n");
            break;
        } else {
            tentativas--;
            if (tentativas > 0) {
                printf("\nERRO! Senha/Login inválido.\n");
                printf("Você tem %d tentativas restantes.\n", tentativas);
                esperarTecla();
            } else {
                printf("\nLimite de tentativas excedido.\n");
                esperarTecla();
            }
        }
    }

    if (tentativas == 0) {
        return 1; // Encerra o programa com erro
    }

    do {
        limparTela();
        exibirMenu();
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                cadastrarProduto(produtos, &numProdutos);
                salvarProdutos(produtos, numProdutos); // Salvar após o cadastro
                esperarTecla();
                break;
            case 2:
                imprimirEstoque(produtos, numProdutos);
                esperarTecla();
                break;
            case 3:
                removerProduto(produtos, &numProdutos);
                salvarProdutos(produtos, numProdutos); // Salvar após a remoção
                esperarTecla();
                break;
            case 4:
                printf("Saindo...\n");
                break;
            default:
                printf("ERRO! Código inserido inválido.\n");
                esperarTecla();
                break;
        }
    } while (opcao != 4);

    return 0;
}

// Função para validar a data no formato DD/MM/AAAA
int validarData(const char* data) {
    int dia, mes, ano;

    // Verifica o formato básico
    if (sscanf(data, "%2d/%2d/%4d", &dia, &mes, &ano) != 3)
        return 0;

    // Verifica se os valores de dia, mês e ano são válidos
    if (ano < 1 || mes < 1 || mes > 12 || dia < 1)
        return 0;

    // Dias máximos em cada mês
    int diasNoMes[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

    // Ajuste para anos bissextos
    if ((ano % 4 == 0 && ano % 100 != 0) || (ano % 400 == 0))
        diasNoMes[1] = 29;

    // Verifica o número de dias no mês
    if (dia > diasNoMes[mes - 1])
        return 0;

    return 1;
}

// Função para cadastrar um produto
void cadastrarProduto(Produto produtos[], int *numProdutos) {
    if (*numProdutos >= 100) {
        printf("Estoque cheio. Não é possível cadastrar mais produtos.\n");
        return;
    }

    Produto p;
    printf("Digite o nome do produto: ");
    scanf("%49s", p.nome);
    printf("Digite a quantidade: ");
    scanf("%d", &p.quantidade);

    // Valida a data de validade
    do {
        printf("Digite a data de validade (DD/MM/AAAA): ");
        scanf("%10s", p.dataValidade);
        if (!validarData(p.dataValidade)) {
            printf("Data inválida! Tente novamente.\n");
        }
    } while (!validarData(p.dataValidade));

    printf("Digite o nome do doador: ");
    scanf("%49s", p.doador);

    produtos[*numProdutos] = p;
    (*numProdutos)++;
}

// Função para imprimir o estoque
void imprimirEstoque(Produto produtos[], int numProdutos) {
    printf("========================================\n");
    printf("              ESTOQUE ATUAL            \n");
    printf("========================================\n");
    for (int i = 0; i < numProdutos; i++) {
        printf("Produto %d:\n", i + 1);
        printf("Nome: %s\n", produtos[i].nome);
        printf("Quantidade: %d\n", produtos[i].quantidade);
        printf("Data de Validade: %s\n", produtos[i].dataValidade);
        printf("Doador: %s\n", produtos[i].doador);
        printf("----------------------------------------\n");
    }
}

// Função para salvar produtos em um arquivo
void salvarProdutos(Produto produtos[], int numProdutos) {
    FILE *file = fopen("produtos.txt", "w");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo para escrita.\n");
        return;
    }

    for (int i = 0; i < numProdutos; i++) {
        fprintf(file, "%s,%d,%s,%s\n", produtos[i].nome, produtos[i].quantidade, produtos[i].dataValidade, produtos[i].doador);
    }

    fclose(file);
}

// Função para carregar produtos de um arquivo
void carregarProdutos(Produto produtos[], int *numProdutos) {
    FILE *file = fopen("produtos.txt", "r");
    if (file == NULL) {
        printf("Nenhum arquivo de produtos encontrado. Iniciando com estoque vazio.\n");
        return;
    }

    while (fscanf(file, "%49[^,],%d,%10[^,],%49[^\n]\n", produtos[*numProdutos].nome, &produtos[*numProdutos].quantidade, produtos[*numProdutos].dataValidade, produtos[*numProdutos].doador) == 4) {
        (*numProdutos)++;
    }

    fclose(file);
}

// Função para remover uma quantidade específica de um produto
void removerProduto(Produto produtos[], int *numProdutos) {
    char nome[50];
    int quantidadeRemover;
    int i;

    // Solicita o nome do produto a ser removido e a quantidade
    printf("Digite o nome do produto: ");
    scanf("%49s", nome);
    printf("Digite a quantidade a ser removida: ");
    scanf("%d", &quantidadeRemover);

    // Procura o produto pelo nome
    for (i = 0; i < *numProdutos; i++) {
        if (strcmp(produtos[i].nome, nome) == 0) {
            // Verifica se a quantidade a ser removida é maior do que a disponível
            if (quantidadeRemover > produtos[i].quantidade) {
                printf("Quantidade a ser removida é maior do que a disponível.\n");
                return;
            } else if (quantidadeRemover == produtos[i].quantidade) {
                // Remove o produto se a quantidade a ser removida for igual à disponível
                for (int j = i; j < *numProdutos - 1; j++) {
                    produtos[j] = produtos[j + 1];
                }
                (*numProdutos)--;  // Reduz o número de produtos
                printf("Produto removido com sucesso.\n");
                return;
            } else {
                // Reduz a quantidade do produto
                produtos[i].quantidade -= quantidadeRemover;
                printf("Quantidade removida com sucesso. Nova quantidade: %d\n", produtos[i].quantidade);
                return;
            }
        }
    }

    // Se o produto não for encontrado
    printf("Produto não encontrado.\n");
}
