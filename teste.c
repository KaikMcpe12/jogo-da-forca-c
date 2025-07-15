#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <wchar.h>
#include <locale.h>
#include <ctype.h>

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

// Estrutura para mapeamento de caracteres UTF-8
typedef struct {
    char* com_acento;
    char sem_acento;
} MapaAcentoUTF8;

// Mapeamento de caracteres acentuados em UTF-8
MapaAcentoUTF8 mapa_acentos_utf8[] = {
    {"á", 'a'}, {"à", 'a'}, {"ã", 'a'}, {"â", 'a'},
    {"é", 'e'}, {"è", 'e'}, {"ê", 'e'},
    {"í", 'i'}, {"ì", 'i'}, {"î", 'i'},
    {"ó", 'o'}, {"ò", 'o'}, {"õ", 'o'}, {"ô", 'o'},
    {"ú", 'u'}, {"ù", 'u'}, {"û", 'u'},
    {"ç", 'c'},
    {"Á", 'A'}, {"À", 'A'}, {"Ã", 'A'}, {"Â", 'A'},
    {"É", 'E'}, {"È", 'E'}, {"Ê", 'E'},
    {"Í", 'I'}, {"Ì", 'I'}, {"Î", 'I'},
    {"Ó", 'O'}, {"Ò", 'O'}, {"Õ", 'O'}, {"Ô", 'O'},
    {"Ú", 'U'}, {"Ù", 'U'}, {"Û", 'U'},
    {"Ç", 'C'}
};

// utils
void limpar_buffer(){
    while (getchar()!= '\n');
}

// função que limpar a tela
void limpar_tela(){
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
            printf(RED "Digite um número entre %d e %d: " RESET, min, max);
            limpar_buffer();
        }
    } while (num < min || num > max);

    return num;
}

// Função para verificar se uma string UTF-8 corresponde a um caractere acentuado
char obter_char_sem_acento_utf8(const char* str) {
    int num_acentos = sizeof(mapa_acentos_utf8) / sizeof(MapaAcentoUTF8);
    
    for (int i = 0; i < num_acentos; i++) {
        if (strncmp(str, mapa_acentos_utf8[i].com_acento, strlen(mapa_acentos_utf8[i].com_acento)) == 0) {
            return mapa_acentos_utf8[i].sem_acento;
        }
    }
    
    return str[0]; // Retorna o primeiro byte se não encontrar
}

// Função para verificar se um caractere digitado corresponde a um caractere da palavra
int chars_equivalentes(char char_digitado, const char* palavra_pos) {
    // Converter char digitado para minúscula
    char_digitado = tolower(char_digitado);
    
    // Se o caractere da palavra é ASCII simples
    if ((unsigned char)palavra_pos[0] < 128) {
        return char_digitado == tolower(palavra_pos[0]);
    }
    
    // Se o caractere da palavra é UTF-8 (acentuado)
    char char_sem_acento = obter_char_sem_acento_utf8(palavra_pos);
    return char_digitado == tolower(char_sem_acento);
}

// Função para obter o próximo caractere UTF-8 e retornar quantos bytes ele ocupa
int obter_proximo_char_utf8(const char* str, int pos, char* buffer) {
    unsigned char primeiro_byte = str[pos];
    
    if (primeiro_byte < 128) {
        // ASCII simples (1 byte)
        buffer[0] = str[pos];
        buffer[1] = '\0';
        return 1;
    } else if ((primeiro_byte & 0xE0) == 0xC0) {
        // UTF-8 de 2 bytes
        buffer[0] = str[pos];
        buffer[1] = str[pos + 1];
        buffer[2] = '\0';
        return 2;
    } else if ((primeiro_byte & 0xF0) == 0xE0) {
        // UTF-8 de 3 bytes
        buffer[0] = str[pos];
        buffer[1] = str[pos + 1];
        buffer[2] = str[pos + 2];
        buffer[3] = '\0';
        return 3;
    }
    
    // Fallback para caracteres não reconhecidos
    buffer[0] = str[pos];
    buffer[1] = '\0';
    return 1;
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

void desenhar_boneco(int erros)
{
    if(erros == 0) {
        imprimir_boneco(' ', ' ', ' ', ' ', ' ', ' ');
    } else if(erros == 1) {
        imprimir_boneco('O', ' ', ' ', ' ', ' ', ' ');
    } else if(erros == 2) {
        imprimir_boneco('O', '|', ' ', ' ', ' ', ' ');
    } else if(erros == 3) {
        imprimir_boneco('O', '|', '/', ' ', ' ', ' ');
    } else if(erros == 4) {
        imprimir_boneco('O', '|', '/', '\\', ' ', ' ');
    } else if(erros == 5) {
        imprimir_boneco('O', '|', '/', '\\', '/', ' ');
    } else if(erros == 6) {
        imprimir_boneco('O', '|', '/', '\\', '/', '\\');
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

// sortear a palavra
char* sortear_palavra_arquivo(const char* nome_arquivo) {
    FILE* arquivo = fopen(nome_arquivo, "r");
    if (!arquivo) {
        return NULL;
    }
    
    fseek(arquivo, 0, SEEK_END);
    long tamanho_arquivo = ftell(arquivo);
    
    long posicao_aleatoria = rand() % tamanho_arquivo;
    fseek(arquivo, posicao_aleatoria, SEEK_SET);
    
    if (posicao_aleatoria > 0) {
        char c;
        while ((c = fgetc(arquivo)) != '\n' && c != EOF) {}
    }
    
    char* palavra = malloc(100 * sizeof(char));
    if (!palavra) {
        fclose(arquivo);
        return NULL;
    }
    
    if (fgets(palavra, 100, arquivo) == NULL) {
        fseek(arquivo, 0, SEEK_SET);
        fgets(palavra, 100, arquivo);
    }
    
    palavra[strcspn(palavra, "\n")] = '\0';
    
    fclose(arquivo);
    return palavra;
}

char* sortear_palavra(int escolha, int nivel) {
    char nome_arquivo[50];
    char* categorias[] = {"animais", "fruta", "pais"};
    
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

    printf("\n=> ");
    opcao = input_int(1, 3);
    
    return opcao; // CORREÇÃO: Adicionado return
}

int menu_niveis()
{
    int nivel;

    cabecalho();

    printf("Escolha o nível que deseja abaixo:\n\n");

    printf(MAGENTA"1 - Fácil\n");
    printf("2 - Médio\n");
    printf("3 - Difícil\n" RESET);

    printf("\n=> ");
    nivel = input_int(1, 3);
    
    return nivel; // CORREÇÃO: Adicionado return
}

// Verificar se a letra digitada está na palavra
int verificar_letra_na_palavra(char letra, char palavra[], const char* palavra_sorteada) {
    int encontrou = 0;
    int pos = 0;
    int pos_palavra = 0;
    
    while (palavra_sorteada[pos] != '\0') {
        char buffer[4];
        int bytes = obter_proximo_char_utf8(palavra_sorteada, pos, buffer);
        
        if (chars_equivalentes(letra, &palavra_sorteada[pos])) {
            // Copiar o caractere original (com acento) para a palavra de progresso
            for (int i = 0; i < bytes; i++) {
                palavra[pos_palavra + i] = palavra_sorteada[pos + i];
            }
            encontrou = 1;
        }
        
        pos += bytes;
        pos_palavra += bytes;
    }
    
    return encontrou;
}

// Função para imprimir a palavra de progresso
void imprimir_palavra_progresso(const char* palavra, const char* palavra_sorteada) {
    int pos = 0;
    
    while (palavra_sorteada[pos] != '\0') {
        char buffer[4];
        int bytes = obter_proximo_char_utf8(palavra_sorteada, pos, buffer);
        
        // Verificar se os bytes foram revelados
        int revelado = 1;
        for (int i = 0; i < bytes; i++) {
            if (palavra[pos + i] == '_') {
                revelado = 0;
                break;
            }
        }
        
        if (revelado) {
            printf(GREEN "%.*s " RESET, bytes, &palavra[pos]);
        } else {
            printf(GREEN "_ " RESET);
        }
        
        pos += bytes;
    }
    printf("\n");
}

void mostras_letras_digitadas(char letras[], int tamanho)
{
    printf(BOLD"LETRAS DIGITADAS:"RESET);
    for(int i = 0; i < tamanho; i++)
    {
        printf(YELLOW"%c"RESET" - ", letras[i]);
    }
    printf("\n\n");
}

// CORREÇÃO: Função para verificar letras repetidas usando equivalência
int letras_repetidas(char letra, char letras_digitadas[], int tamanho)
{
    for(int i = 0; i < tamanho; i++)
    {
        if(tolower(letra) == tolower(letras_digitadas[i]))
        {
            return 1;
        }
    }

    return 0;
}

// Função para verificar se a palavra está completa
int palavra_completa(const char* palavra, const char* palavra_sorteada) {
    int pos = 0;
    
    while (palavra_sorteada[pos] != '\0') {
        char buffer[4];
        int bytes = obter_proximo_char_utf8(palavra_sorteada, pos, buffer);
        
        // Verificar se algum byte ainda está como '_'
        for (int i = 0; i < bytes; i++) {
            if (palavra[pos + i] == '_') {
                return 0;
            }
        }
        
        pos += bytes;
    }
    
    return 1;
}

//função que contem o codigo da taça
void taca(){
    printf("\n" GREEN "Parabéns, você ganhou!" RESET "\n\n");

    printf(YELLOW"          \n");
    printf("      '.=========.'     \n");
    printf("      .-\\:      /-.    \n");
    printf("     | (|:.     |) |    \n");
    printf("      '-|:.     |-'     \n");
    printf("        \\::.    /      \n");
    printf("         '::. .'        \n");
    printf("           ) (          \n");
    printf("         .'   '.        \n");
    printf("        '-------'       \n\n" RESET);
}

//função que contem o codigo da derrota
void caveira(){
    printf(RED "Você perdeu!\n\n");
    printf("    _______________                                 \n");
    printf("   /               \\                               \n");
    printf("  /                 \\                              \n");
    printf(" /     __      __    \\                             \n");
    printf(" |    /  \\    /  \\   |                            \n");
    printf(" |    \\__/    \\__/   |                            \n");
    printf(" |                   |                              \n");
    printf(" |         _         |                              \n");
    printf("  \\_      / \\      _/                             \n");
    printf("   |\\    /___\\    /|                              \n");
    printf("   | |           | |                                \n");
    printf("   |  XXXXXXXXXXX  |                                \n");
    printf("   |   XXXXXXXXX   |                                \n");
    printf("   \\_             _/                               \n");
    printf("     \\_   __    _/                                 \n");
    printf("       \\/    \\/                                   \n" RESET);
}

// função principal
int main()
{   
    srand(time(NULL));
    setlocale(LC_ALL, "");

    int tentativas = 0, opcao, nivel;
    char* palavra_sorteada;
    char palavra[100];
    
    opcao = menu_opcoes();
    limpar_tela();
    nivel = menu_niveis();

    // palavra_sorteada = sortear_palavra(opcao, nivel);
    palavra_sorteada = "olá"; // Para teste

    // Inicializar a palavra de progresso
    memset(palavra, '_', strlen(palavra_sorteada));
    palavra[strlen(palavra_sorteada)] = '\0';

    limpar_tela();

    // início do jogo
    int tamanho = 0;
    char letras_digitadas[100];

    while (tentativas < 6 && !palavra_completa(palavra, palavra_sorteada))
    {
        char letra;

        cabecalho();

        printf(BOLD"A opção escolhida foi: " RESET);
        if(opcao == 1) {
            printf(YELLOW"Animal"RESET"\n\n");
        } else if(opcao == 2) {
            printf(RED"Fruta"RESET"\n\n");
        } else {
            printf(GREEN"País"RESET"\n\n");
        }

        if(tamanho > 0) {
            mostras_letras_digitadas(letras_digitadas, tamanho);
        }

        desenhar_boneco(tentativas);
        imprimir_palavra_progresso(palavra, palavra_sorteada); 

        printf("\n" CYAN"Digite uma letra: " RESET);
        scanf(" %c", &letra);

        if(!isalpha(letra)) {
            printf(RED"Digite apenas letras!\n"RESET);
            sleep(2);   
            limpar_buffer();
            limpar_tela();
            continue;
        }
        
        if(letras_repetidas(letra, letras_digitadas, tamanho)) {
            printf(RED"Letra repetida, digite novamente\n"RESET);
            sleep(2);
            limpar_buffer();
            limpar_tela();
            continue;
        } 

        letras_digitadas[tamanho] = letra;
        tamanho++;
        
        if(verificar_letra_na_palavra(letra, palavra, palavra_sorteada) == 0) {
            tentativas++;
        }

        limpar_buffer();
        limpar_tela();
    }

    if (palavra_completa(palavra, palavra_sorteada)) {
        taca();
    } else {
        caveira();
    }
    
    printf("\n\n" BOLD "A palavra era: " RESET RED "%s" RESET "\n", palavra_sorteada);

    return 0;
}