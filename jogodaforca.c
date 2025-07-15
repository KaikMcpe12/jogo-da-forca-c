#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <wchar.h>
#include <locale.h>

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

// estrutura para jogador
typedef struct {
    char nome[50];
    int vitorias;
    int partidas;
    int tentativas;
} Jogador;

// mapeamento de caracteres acentuados
typedef struct {
    char* com_acento;
    char sem_acento;
} MapaAcento;

MapaAcento mapa_acentos[] = {
    {"á", 'a'}, {"à", 'a'}, {"ã", 'a'}, {"â", 'a'},
    {"é", 'e'}, {"è", 'e'}, {"ê", 'e'},
    {"í", 'i'}, {"ì", 'i'}, {"î", 'i'},
    {"ó", 'o'}, {"ò", 'o'}, {"õ", 'o'}, {"ô", 'o'},
    {"ú", 'u'}, {"ù", 'u'}, {"û", 'u'},
    {"ç", 'c'}
};

// utils
void limpar_buffer(){
    while (getchar()!= '\n');
}

void limpar_tela(){
    system("cls") && system("clear");
}

void pausar() {
    limpar_buffer();
    printf("\n" YELLOW "Pressione ENTER para continuar..." RESET);
    getchar();
}

int input_int(int min, int max) {
    int num;
    do {
        scanf("%d", &num);
        if(num < min || num > max) {
            printf(RED "Digite um número entre %d e %d: " RESET, min, max);
            limpar_buffer();
        }
    } while (num < min || num > max);
    return num;
}

char remover_acento(char *c) {
    int num_acentos = sizeof(mapa_acentos) / sizeof(MapaAcento);
    for (int i = 0; i < num_acentos; i++) {
        if (strncmp(c, mapa_acentos[i].com_acento, strlen(mapa_acentos[i].com_acento)) == 0) {
            return mapa_acentos[i].sem_acento;
        }
    }
    return c[0];
}

int letras_equivalentes(char a, char *b) {
    char char_sem_acento = remover_acento(b);
    return a == char_sem_acento;
}

int tamanho_bytes(const char* str, int pos) {
    unsigned char primeiro_byte = str[pos];
    if (primeiro_byte < 128) {
        return 1;
    } else if ((primeiro_byte & 0xE0) == 0xC0) {
        return 2;
    } else if ((primeiro_byte & 0xF0) == 0xE0) {
        return 3;
    }
    return 1;
}

void imprimir_boneco(char head, char body, char lf_bra, char rh_bra, char lf_per, char rh_per) {
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

void desenhar_boneco(int erros) {
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

void cabecalho() {
    printf(YELLOW "+-------------------------------------------------------+\n");
    printf("|                                                       |\n");
    printf("|      █ ▄▀▄ ▄▀  ▄▀▄   █▀▄ ▄▀▄   █▀ ▄▀▄ █▀▄ ▄▀▀ ▄▀▄     |\n");
    printf("|    ▀▄█ ▀▄▀ ▀▄█ ▀▄▀   █▄▀ █▀█   █▀ ▀▄▀ █▀▄ ▀▄▄ █▀█     |\n");
    printf("|                                                       |\n");
    printf("+-------------------------------------------------------+\n\n" RESET);
}

char* sortear_palavra_arquivo(const char* nome_arquivo) {
    FILE* arquivo = fopen(nome_arquivo, "r");
    if (!arquivo) return NULL;
    
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

char* sortear_palavra(int escolha, int nivel) {
    char nome_arquivo[50];
    char* categorias[] = {"animais", "fruta", "pais"};
    sprintf(nome_arquivo, "palavras/%s-v%d.txt", categorias[escolha - 1], nivel);
    return sortear_palavra_arquivo(nome_arquivo);
}

int menu_opcoes() {
    int opcao;
    cabecalho();
    printf("Escolha o tema da palavra abaixo:\n\n");
    printf(MAGENTA"1 - Animal\n");
    printf("2 - Fruta\n");
    printf("3 - País\n" RESET);
    printf("\n=> ");
    opcao = input_int(1, 3);
    return opcao;
}

int menu_niveis() {
    int nivel;
    cabecalho();
    printf("Escolha o nível que deseja abaixo:\n\n");
    printf(MAGENTA"1 - Fácil\n");
    printf("2 - Médio\n");
    printf("3 - Difícil\n" RESET);
    printf("\n=> ");
    nivel = input_int(1, 3);
    return nivel;
}

// configurar jogadores para multiplayer
int configurar_multiplayer(Jogador **jogadores) {
    int num_jogadores;
    cabecalho();
    printf(BOLD CYAN"=== MODO MULTIPLAYER ===" RESET "\n\n");
    printf("Quantos jogadores? (2-4): ");
    num_jogadores = input_int(2, 4);

    *jogadores = realloc(*jogadores, num_jogadores * sizeof(Jogador));

    limpar_buffer();
    
    for (int i = 0; i < num_jogadores; i++) {
        printf("\nNome do jogador %d: ", i + 1);
        fgets((*jogadores)[i].nome, 50, stdin);
        (*jogadores)[i].nome[strcspn((*jogadores)[i].nome, "\n")] = '\0';
        (*jogadores)[i].vitorias = 0;
        (*jogadores)[i].partidas = 0;
        (*jogadores)[i].tentativas = 0;
    }
    
    return num_jogadores;
}

// configurar jogador solo
int configurar_solo(Jogador jogadores[]) {
    cabecalho();
    printf(BOLD CYAN"=== MODO SOLO ===" RESET "\n\n");
    
    limpar_buffer();
    printf("Digite seu nome: ");
    fgets(jogadores[0].nome, 50, stdin);
    jogadores[0].nome[strcspn(jogadores[0].nome, "\n")] = '\0';
    jogadores[0].vitorias = 0;
    jogadores[0].partidas = 0;
    jogadores[0].tentativas = 0;
    
    return 1;
}

// verificar se palavra está completa
int palavra_completa(char* palavra, char* palavra_sorteada) {
    return strcmp(palavra, palavra_sorteada) == 0;
}

// verificar se palavra digitada está correta
int verificar_palavra_completa(char* tentativa, char* palavra_sorteada) {
    // converter para minúsculas para comparação
    char tentativa_lower[100], palavra_lower[100];
    strcpy(tentativa_lower, tentativa);
    strcpy(palavra_lower, palavra_sorteada);
    
    for (int i = 0; tentativa_lower[i]; i++) {
        if (tentativa_lower[i] >= 'A' && tentativa_lower[i] <= 'Z') {
            tentativa_lower[i] += 32;
        }
    }
    
    for (int i = 0; palavra_lower[i]; i++) {
        if (palavra_lower[i] >= 'A' && palavra_lower[i] <= 'Z') {
            palavra_lower[i] += 32;
        }
    }
    
    return strcmp(tentativa_lower, palavra_lower) == 0;
}

int verificar_letra_na_palavra(char letra, char palavra[], char* palavra_sorteada) {
    int encontrou = 0;
    int pos = 0;
    int pos_palavra = 0;
    
    while (palavra_sorteada[pos] != '\0') {
        int bytes = tamanho_bytes(palavra_sorteada, pos);
        
        if (letras_equivalentes(letra, &palavra_sorteada[pos])) {
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

void imprimir_palavra_progresso(char* palavra, char* palavra_sorteada) {   
    int pos = 0;
    
    while (palavra_sorteada[pos] != '\0') {
        int bytes = tamanho_bytes(palavra_sorteada, pos);
        
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

void mostrar_letras_digitadas(char letras[], int tamanho) {
    printf("\033[1mLETRAS DIGITADAS:\033[m ");
    for(int i = 0; i < tamanho; i++) {
        printf("\033[33m%c\033[m ", letras[i]);
    }
    printf("\n\n");
}

void mostrar_placar(Jogador jogadores[], int num_jogadores) {
    printf(BOLD "\n=== PLACAR ===" RESET "\n");
    for (int i = 0; i < num_jogadores; i++) {
        printf("%s: %d vitórias / %d partidas\n", 
               jogadores[i].nome, jogadores[i].vitorias, jogadores[i].partidas);
    }
    printf("\n");
}

int letras_repetidas(char letra, char letras_digitadas[], int tamanho) {
    for(int i = 0; i < tamanho; i++) {
        if(letra == letras_digitadas[i]) {
            return 1;
        }
    }
    return 0;
}

void taca() {
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

void caveira() {
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

// menu inicial
int menu_inicial() {
    int opcao;
    cabecalho();
    printf(BOLD YELLOW"Bem-vindo ao Jogo da Forca!\n\n" RESET);
    
    printf("Escolha uma opção:\n\n");
    printf(MAGENTA"1 - Jogar Solo\n");
    printf("2 - Multiplayer\n");
    printf("3 - Instruções\n");
    printf("4 - Sair\n" RESET);
    
    printf("\n=> ");
    opcao = input_int(1, 4);
    return opcao;
}

// mostrar instruções
void mostrar_instrucoes() {
    cabecalho();
    printf(BOLD CYAN"=== INSTRUÇÕES ===" RESET "\n\n");
    
    printf("• O objetivo é adivinhar a palavra secreta\n");
    printf("• Você pode digitar uma letra ou a palavra completa\n");
    printf("• Cada letra errada adiciona uma parte ao boneco\n");
    printf("• Com 6 erros, o boneco fica completo e você perde\n");
    printf("• Acerte a palavra antes disso para ganhar!\n\n");
    
    printf(BOLD YELLOW"MODO SOLO:" RESET "\n");
    printf("• Jogue sozinho contra o computador\n");
    printf("• 6 tentativas para adivinhar a palavra\n\n");
    
    printf(BOLD YELLOW"MODO MULTIPLAYER:" RESET "\n");
    printf("• 2 a 4 jogadores se revezam\n");
    printf("• Cada jogador tem suas próprias tentativas\n");
    printf("• Pressione ENTER para pular sua vez\n");
    printf("• Ganha quem acertar a palavra primeiro\n");
    printf("• Placar de vitórias é mantido entre partidas\n\n");
    
    pausar();
}

int jogar_partida(Jogador jogadores[], int num_jogadores, int opcao, char** palavra_resultado) {
    int nivel = menu_niveis();
    limpar_buffer();

    char* palavra_sorteada = sortear_palavra(opcao, nivel);
    *palavra_resultado = palavra_sorteada;
    
    char palavra[100];
    char letras_digitadas[100];
    int tamanho_letras = 0;
    int jogador_atual = 0;
    int jogo_terminado = 0;
    int vencedor = -1;
    
    // inicializar palavra com underscores
    int tamanho_palavra = mbstowcs(NULL, palavra_sorteada, 0);
    for(int i = 0; i < tamanho_palavra; i++) {
        if(palavra_sorteada[i] == ' ') {
            palavra[i] = ' ';
        } else if (palavra_sorteada[i] == '-') {
            palavra[i] = '-';
        } else {
            palavra[i] = '_';
        }
    }
    palavra[strlen(palavra_sorteada)] = '\0';
    
    // inicializar tentativas dos jogadores
    for (int i = 0; i < num_jogadores; i++) {
        jogadores[i].tentativas = 0;
    }
    
    limpar_tela();
    
    // loop principal do jogo
    while (!jogo_terminado) {
        // verificar se todos os jogadores esgotaram suas tentativas
        int todos_perderam = 1;
        for (int i = 0; i < num_jogadores; i++) {
            if (jogadores[i].tentativas < 6) {
                todos_perderam = 0;
                break;
            }
        }
        
        if (todos_perderam) {
            jogo_terminado = 1;
            break;
        }
        
        // pular jogadores que já perderam
        while (jogadores[jogador_atual].tentativas >= 6) {
            jogador_atual = (jogador_atual + 1) % num_jogadores;
        }
        
        cabecalho();
        
        // mostrar categoria
        printf(BOLD"A opção escolhida foi: " RESET);
        char* categorias[] = {"Animal", "Fruta", "País"};
        char* cores[] = {YELLOW, RED, GREEN};
        printf("%s%s%s\n\n", cores[opcao-1], categorias[opcao-1], RESET);
        
        // mostrar placar (apenas no multiplayer)
        if (num_jogadores > 1) {
            mostrar_placar(jogadores, num_jogadores);
        }
        
        // mostrar letras digitadas
        mostrar_letras_digitadas(letras_digitadas, tamanho_letras);
        
        // mostrar boneco baseado nas tentativas do jogador atual
        desenhar_boneco(jogadores[jogador_atual].tentativas);
        
        // mostrar palavra
        imprimir_palavra_progresso(palavra, palavra_sorteada);
        
        // verificar se palavra foi completada
        if (palavra_completa(palavra, palavra_sorteada)) {
            vencedor = jogador_atual;
            jogo_terminado = 1;
            break;
        }
        
        // vez do jogador
        if (num_jogadores == 1) {
            printf(CYAN"\nDigite uma letra ou palavra completa: " RESET);
        } else {
            printf(CYAN"\n=== VEZ DE %s ===" RESET "\n", jogadores[jogador_atual].nome);
            printf("Digite uma letra ou palavra completa (ENTER para pular): ");
        }
        
        char entrada[100];
        fgets(entrada, 100, stdin);
        entrada[strcspn(entrada, "\n")] = '\0';
        
        // pular vez (apenas no multiplayer)
        if (strlen(entrada) == 0 && num_jogadores > 1) {
            printf(YELLOW"%s pulou a vez!\n" RESET, jogadores[jogador_atual].nome);
            sleep(1);
            jogador_atual = (jogador_atual + 1) % num_jogadores;
            limpar_tela();
            continue;
        }
        
        // forçar entrada no modo solo
        if (strlen(entrada) == 0 && num_jogadores == 1) {
            printf(RED"Digite uma letra ou palavra!\n" RESET);
            sleep(1);
            limpar_tela();
            continue;
        }
        
        // tentativa de palavra completa
        if (strlen(entrada) > 1) {
            if (verificar_palavra_completa(entrada, palavra_sorteada)) {
                strcpy(palavra, palavra_sorteada);
                if (num_jogadores > 1) {
                    printf(GREEN"%s acertou a palavra!\n" RESET, jogadores[jogador_atual].nome);
                } else {
                    printf(GREEN"Parabéns! Você acertou a palavra!\n" RESET);
                }
                vencedor = jogador_atual;
                jogo_terminado = 1;
                sleep(2);
                break;
            } else {
                if (num_jogadores > 1) {
                    printf(RED"Palavra incorreta! %s perdeu uma tentativa.\n" RESET, jogadores[jogador_atual].nome);
                } else {
                    printf(RED"Palavra incorreta! Você perdeu uma tentativa.\n" RESET);
                }
                jogadores[jogador_atual].tentativas++;
                sleep(2);
            }
        } else {
            // tentativa de letra
            char letra = entrada[0];
            
            // validar entrada
            if(letra < 65 || letra > 122 || (letra >= 91 && letra <= 96)) {
                printf(RED"Comando inválido!\n" RESET);
                sleep(2);
                limpar_tela();
                continue;
            }
            
            // verificar letra repetida
            if(letras_repetidas(letra, letras_digitadas, tamanho_letras)) {
                printf(RED"Letra já digitada!\n" RESET);
                sleep(2);
                limpar_tela();
                continue;
            }
            
            // adicionar letra às digitadas
            letras_digitadas[tamanho_letras] = letra;
            tamanho_letras++;
            
            // verificar se letra está na palavra
            if(!verificar_letra_na_palavra(letra, palavra, palavra_sorteada)) {
                if (num_jogadores > 1) {
                    printf(RED"Letra não encontrada! %s perdeu uma tentativa.\n" RESET, jogadores[jogador_atual].nome);
                } else {
                    printf(RED"Letra não encontrada! Você perdeu uma tentativa.\n" RESET);
                }
                jogadores[jogador_atual].tentativas++;
                sleep(2);
            } else {
                if (num_jogadores > 1) {
                    printf(GREEN"Boa %s! Letra encontrada!\n" RESET, jogadores[jogador_atual].nome);
                } else {
                    printf(GREEN"Boa! Letra encontrada!\n" RESET);
                }
                sleep(1);
                
                // verificar se completou a palavra
                if (palavra_completa(palavra, palavra_sorteada)) {
                    vencedor = jogador_atual;
                    jogo_terminado = 1;
                    break;
                }
            }
        }
        
        // no modo solo, não muda jogador
        if (num_jogadores > 1) {
            jogador_atual = (jogador_atual + 1) % num_jogadores;
        }
        
        limpar_tela();
    }
    
    return vencedor;
}

int main() {   
    srand(time(NULL));
    setlocale(LC_ALL, "");
    
    Jogador *jogadores = calloc(1, sizeof(Jogador));
    int num_jogadores = 0, opcao_inicial, opcao_tema;
    char jogar_novamente;
    char* palavra_sorteada = NULL;
    
    do {
        opcao_inicial = menu_inicial();
        limpar_tela();
        
        if (opcao_inicial == 1) {
            // modo solo
            num_jogadores = configurar_solo(jogadores);
        } else if (opcao_inicial == 2) {
            // modo multiplayer
            num_jogadores = configurar_multiplayer(&jogadores);
        } else if (opcao_inicial == 3) {
            // instruções
            mostrar_instrucoes();
            limpar_tela();
            continue;
        } else if (opcao_inicial == 4) {
            // sair
            printf(BOLD YELLOW"Obrigado por jogar!\n" RESET);
            return 0;
        }
        
        limpar_tela();
        
        // loop de partidas
        do {
            opcao_tema = menu_opcoes();
            limpar_tela();
            
            // incrementar partidas para todos
            for (int i = 0; i < num_jogadores; i++) {
                jogadores[i].partidas++;
            }
            
            int vencedor = jogar_partida(jogadores, num_jogadores, opcao_tema, &palavra_sorteada);
            limpar_tela();
            
            if (vencedor >= 0) {
                taca();
                if (num_jogadores == 1) {
                    printf(BOLD GREEN"🏆 PARABÉNS! VOCÊ VENCEU! 🏆\n" RESET);
                } else {
                    printf(BOLD GREEN"🏆 VENCEDOR: %s! 🏆\n" RESET, jogadores[vencedor].nome);
                }
                jogadores[vencedor].vitorias++;
            } else {
                caveira();
                if (num_jogadores == 1) {
                    printf(BOLD RED"Você não conseguiu adivinhar a palavra!\n" RESET);
                } else {
                    printf(BOLD RED"Ninguém conseguiu adivinhar a palavra!\n" RESET);
                }
            }
            
            printf("\n" BOLD "A palavra sorteada foi: " RESET RED "%s" RESET "\n", palavra_sorteada);
            
            // mostrar placar final
            if (num_jogadores > 1) {
                mostrar_placar(jogadores, num_jogadores);
            } else {
                printf("\n" BOLD "Seu placar: " RESET "%d vitórias / %d partidas\n", 
                       jogadores[0].vitorias, jogadores[0].partidas);
            }
            
            printf("\nDeseja jogar novamente? (s/n): ");
            scanf(" %c", &jogar_novamente);
            limpar_buffer();
            limpar_tela();
        } while (jogar_novamente == 's' || jogar_novamente == 'S');
        
        printf(BOLD YELLOW "Voltando ao menu principal...\n" RESET);
        sleep(1);
        limpar_tela();
        
    } while (1);
    
    return 0;
}
