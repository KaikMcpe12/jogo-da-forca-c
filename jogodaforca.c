#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>

// constantes de cores
#define RESET "\033[0m"
#define BOLD "\033[1m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN "\033[36m"
#define WHITE "\033[37m"

// utils
void limparbuffer(){
    while (getchar()!= '\n');
}

// função que limpar a tela
void limparTela(){
    system("cls") && system("clear");
}

// pausar a tela
void pausar() {
    printf("\n" YELLOW "Pressione ENTER para continuar..." RESET);
    getchar();
}

// scanf para tipo inteiro
int input_int(int min, int max)
{
    int num;

    do
    {
        scanf("%d", &num);

        if(num < min || num > max) {
            printf("\033[31mDigite um número entre %d e %d:\033[m ", min, max);
            sleep(2);   
            limparbuffer();
        }
    } while (num < min || num > max);

    return num;
}

// remove acentos de uma palavra
char removerAcento(char c) {
    switch(c) {
        case 'á': case 'à': case 'ã': case 'â': return 'a';
        case 'é': case 'ê': return 'e';
        case 'í': return 'i';
        case 'ó': case 'ô': case 'õ': return 'o';
        case 'ú': return 'u';
        case 'ç': return 'c';
        default: return c;
    }
}

// verifica se a letra está certa
int letraCorreta(char letra, char letraPalavra) {
    return (letra == letraPalavra) || 
           (removerAcento(letra) == removerAcento(letraPalavra));
}

void imprimir_boneco(char head, char body, char lf_bra, char rh_bra, char lf_per, char rh_per)
{
    printf(MAGENTA" +---------------+\n");
    printf(" |               |\n");
    printf(" |               %c\n", head);
    printf(" |              %c%c%c\n", lf_bra, body, rh_bra);
    printf(" |              %c %c\n", lf_per, rh_per);
    printf(" |\n");
    printf(" |\n");
    printf(" |\n");
    printf("===\n\n" RESET);
}

void boneco(int erros)
{
    if(erros == 0) {
        imprimir_boneco(' ', ' ', ' ', ' ', ' ', ' ');
    } else if(erros == 1) {
        imprimir_boneco('0', ' ', ' ', ' ', ' ', ' ');
    } else if(erros == 2) {
        imprimir_boneco('0', '|', ' ', ' ', ' ', ' ');
    } else if(erros == 3) {
        imprimir_boneco('0', '|', '/', ' ', ' ', ' ');
    } else if(erros == 4) {
        imprimir_boneco('0', '|', '/', '\\', ' ', ' ');
    } else if(erros == 5) {
        imprimir_boneco('0', '|', '/', '\\', '/', ' ');
    } else if(erros == 6) {
        imprimir_boneco('0', '|', '/', '\\', '/', '\\');
    }
}

void cabecalho()
{
    printf(YELLOW "+-------------------------------------------------------+\n");
    printf("|                                                       |\n");
    printf("|      █ ▄▀▄ ▄▀  ▄▀▄   █▀▄ ▄▀▄   █▀ ▄▀▄ █▀▄ ▄▀▀ ▄▀▄     |\n");
    printf("|    ▀▄█ ▀▄▀ ▀▄█ ▀▄▀   █▄▀ █▀█   █▀ ▀▄▀ █▀▄ ▀▄▄ █▀█     |\n");
    printf("|                                                       |\n");
    printf("+-------------------------------------------------------+\n\n" RESET);
}

// aosrtea a palavra
char* sortear_palavra_arquivo(const char* nome_arquivo) {
    FILE* arquivo = fopen(nome_arquivo, "r");
    
    fseek(arquivo, 0, SEEK_END);
    long tamanho_arquivo = ftell(arquivo);
    
    long posicao_aleatoria = rand() % tamanho_arquivo;
    fseek(arquivo, posicao_aleatoria, SEEK_SET);
    
    if (posicao_aleatoria > 0) {
        char c;
        while ((c = fgetc(arquivo)) != '\n' && c != EOF) {}
    }
    
    char* palavra = malloc(100 * sizeof(char));
    
    if (fgets(palavra, 100, arquivo) == NULL) {
        fseek(arquivo, 0, SEEK_SET);
        fgets(palavra, 100, arquivo);
    }
    
    palavra[strcspn(palavra, "\n")] = '\0';
    
    fclose(arquivo);
    return palavra;
}

char* sortear(int escolha, int nivel) {
    char nome_arquivo[50];
    char* categorias[] = {"animais", "fruta", "pais"};
    
    // arq
    sprintf(nome_arquivo, "palavras/%s-v%d.txt", categorias[escolha - 1], nivel);
    
    char* palavra_sorteada = sortear_palavra_arquivo(nome_arquivo);
    
    return palavra_sorteada;
}

int menu_opcoes() 
{
    int opcao;

    cabecalho();

    printf("Escolha o tema da palavra abaixo:\n\n");

    printf(MAGENTA"1-Animal\n");
    printf("2-Fruta\n");
    printf("3-País\n" RESET);

    // aqui o usuario escola 1 das 3 opções disponiveis
    printf("\n=> ");
    opcao = input_int(1, 3);
}

int menu_niveis()
{
    int nivel;

    cabecalho();

    printf("Escolha o nível que deseja abaixo:\n\n");

    printf(MAGENTA"1 - Fácil\n");
    printf("2 - Médio\n");
    printf("3 - Dificil\n" RESET);

    // aqui o usuario escola 1 das 3 opções disponivel
    printf("\n=> ");
    nivel = input_int(1, 3);
}

//
void imprimir_palavra(char* palavra)
{   
    for(int i = 0; i < strlen(palavra); i++)
    {
        printf("\033[32m%c\033[m ", palavra[i]);
    }
    printf("\n");
}
// aqui sera verificado a letra digitada pelo usario
int verificar_palavra(char letra, char palavra[], char* palavra_sorteada){
    int encontrado = 0;

    for(int i = 0; i < strlen(palavra_sorteada); i++)
    {
        if(letra == palavra_sorteada[i])
        {
            palavra[i] = palavra_sorteada[i];
            encontrado = 1;
        }
    }

    return encontrado;
}

//função que contem o codigo da taça
void taca(){
    printf("\n\033[32mParabens, voce ganhou!\033[m\n\n");

    printf("\033[33m          \n");
    printf("      '.=========.'     \n");
    printf("      .-\\:      /-.    \n");
    printf("     | (|:.     |) |    \n");
    printf("      '-|:.     |-'     \n");
    printf("        \\::.    /      \n");
    printf("         '::. .'        \n");
    printf("           ) (          \n");
    printf("         .'   '.        \n");
    printf("        '-------'       \n\n\033[m");
}

//função que contem o codigo da derrota
void caveira(){
    printf("\033[31m    _______________                                 \n");
    printf("   /               \\                               \n");
    printf("  /                 \\                              \n");
    printf(" /     __      __    \\                             \n");
    printf(" |    /  \\    /  \\   |                            \n");
    printf(" |    \\__/    \\__/   |                            \n");
    printf(" |                   |                              \n");
    printf(" |         _         |              +-----------   \n");
    printf("  \\_      / \\      _/               |           |   \n");
    printf("   |\\    /___\\    /|                |           O   \n");
    printf("   | |           | |                |          /|\\  \n");
    printf("   |  XXXXXXXXXXX  |                |          / \\   \n");
    printf("   |   XXXXXXXXX   |                |           \n");
    printf("   \\_             _/                |               \n");
    printf("     \\_   __    _/                  |    matou o cara \n");
    printf("       \\/    \\/                    ===               \033[m\n");
}

void mostras_letras_digitadas(char letras[], int tamanho)
{
    printf("\033[1mLETRAS DIGITADAS:\033[m");
    for(int i = 0; i < tamanho; i++)
    {
        printf("\033[33m%c\033[m - ", letras[i]);
    }
    printf("\n\n");
}

int letras_repetidas(char letra, char letras_digitadas[], int tamanho)
{
    for(int i = 0; i < tamanho; i++)
    {
        if(letra == letras_digitadas[i])
        {
            return 1;
        }
    }

    return 0;
}

// função principal
int main()
{   
    srand(time(NULL));

    int tentativas = 0,opcao, nivel;
    char* palavra_sorteada;
    char palavra[100];
    
    opcao = menu_opcoes();
    limparTela();
    nivel = menu_niveis();

    palavra_sorteada = sortear(opcao, nivel);

    for(int i = 0; i < strlen(palavra_sorteada); i++) // aqui estão imprimindo os espaços da palavra sorteadas  
    {
        if(palavra_sorteada[i] == '_')
        {
            palavra[i] = ' ';
        } else {
            palavra[i] = '_';
        }
    }
    palavra[strlen(palavra_sorteada)] = '\0';
    
    limparTela();

    // inicio do jogo

    int tamanho = 0;
    char letras_digitadas[100];

   // printf("palavra sorteada: %s\n", palavra_sorteada);
    while (tentativas < 6 && strcmp(palavra, palavra_sorteada) != 0)
    {
        char letra;

        cabecalho();

        printf(BOLD"A opção escolhida foi:" RESET);
        if(opcao == 1)
        {
            printf(YELLOW"Animal"RESET"\n\n");
        } else if(opcao == 2)
        {
            printf(RED"Fruta"RESET"\n\n");
        } else {
            printf(GREEN"Pais"RESET"\n\n");
        }

        mostras_letras_digitadas(letras_digitadas, tamanho);

        boneco(tentativas);

        imprimir_palavra(palavra); 
        printf("\n" CYAN"Dígite uma letra: " RESET);
        scanf(" %c", &letra);

        if(letra < 65 || letra > 122 | (letra >= 91 && letra <= 96))
        {
            printf(RED"Comando inválido, digite novamente\n"RESET);
            sleep(2);   
            limparbuffer();
            limparTela();
            continue;
        }
        
        if(letras_repetidas(letra, letras_digitadas, tamanho) == 1)
        {
            printf(RED"Letra repetida, digite novamente\n"RESET);
            sleep(2);
            limparbuffer();
            limparTela();
            continue;
        } else {
            letras_digitadas[tamanho] = letra;
            tamanho++;
        }
        
        if(verificar_palavra(letra, palavra, palavra_sorteada) == 0)
        {
            tentativas++;
        }

        limparbuffer();
        limparTela();
    }

    if (strcmp(palavra, palavra_sorteada) == 0)
    {
        taca();
    }
    else 
    {
        caveira();
    }
    printf("\n\n" BOLD "A palavra sorteada foi:" RESET " " RED "%s" RESET "\n", palavra_sorteada);

    return 0;
}
