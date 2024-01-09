#ifndef FUNCOES_H
#define FUNCOES_H

/**
 * @file funcoes.h
 *
 * @brief Interface de funções do jogo klondike (paciência) utilizando Allegro.
 *
 * @author Luiz Felipe Cavalheiro
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <time.h>
#include <ctype.h>
#include "telag.h"

#define ALTURA 480
#define LARGURA 720
#define CARTA_ALTURA ALTURA/5
#define CARTA_LARGURA LARGURA/11
#define ESPACO_ENTRE_CARTAS LARGURA/36
#define N_MAX_CARTAS 52
#define N_PILHAS 13
#define N_PILHAS_SAIDA 4
#define N_PILHAS_PRINCIPAIS 7
#define MAX_CHAR_CMD 2

// enums para dar nomes a valores constantes, de forma organizada
typedef enum {
  ouros,
  copas,
  espadas,
  paus
} naipe_t;

typedef enum { 
  as = 1,
  // os números 2-10 se auto representam
  valete = 11,
  dama,
  rei
} valor_t;

typedef enum {
  naipe_vermelho,
  naipe_preto
} cor_t;

// registro que representa uma carta de baralho
typedef struct {
  valor_t valor;
  naipe_t naipe;
} carta_t;

typedef struct {
    int lin;
    int col;
} coordenadas_t;

// registro que representa uma pilha de cartas
typedef struct {
  int n_cartas;
  int n_cartas_fechadas;
  carta_t cartas[N_MAX_CARTAS];
} pilha_t;

typedef struct {
  pilha_t monte;
  pilha_t descarte;
  pilha_t pilhas_saida[N_PILHAS_SAIDA];
  pilha_t pilhas_principais[N_PILHAS_PRINCIPAIS];
  coordenadas_t coordenadas_pilhas[N_PILHAS];
  char comando[MAX_CHAR_CMD+1];
  double pontos;
  double tempo_ultima_jogada;
  bool sair;
} jogo_t;


/**
 * @brief Cria uma carta com um determinado valor e naipe.
 * 
 * Esta função cria uma carta que contém um valor e um naipe
 * 
 * @param valor Valor da carta.
 * @param naipe Naipe da carta.
 * @return A carta criada.
 */

carta_t cria_carta(valor_t valor, naipe_t naipe);

/**
 * @brief Calcula o bônus com base no tempo da última jogada e pontos obtidos.
 *
 * Esta função calcula um bônus com base no tempo decorrido desde a última jogada
 * e nos pontos obtidos na jogada atual. O bônus é calculado de acordo com a fórmula:
 * pontuacao = (7.0 - tempo_jogada) / 7.0 * 3.0 * pontos_da_jogada;
 * Se o tempo_jogada for maior ou igual a 7 segundos, o bônus é zero.
 *
 * @param j Ponteiro para a estrutura de dados do jogo.
 * @param pontos_da_jogada Número de pontos obtidos na jogada atual.
 * @return Valor do bônus calculado.
 */
double bonus(jogo_t *j, int pontos_da_jogada);

/**
 * @brief Verifica se uma pilha de cartas está vazia.
 * 
 * Esta função verifica se determinada pilha está vazia.
 *
 * @param p Ponteiro para a pilha.
 * @return true se a pilha estiver vazia, false caso contrário.
 */
bool pilha_vazia(pilha_t *p);

/**
 * @brief Verifica se uma pilha de cartas está cheia.
 * 
 * Esta função verifica se determinada pilha está cheia.
 *
 * @param p Ponteiro para a pilha.
 * @return true se a pilha estiver cheia, false caso contrário.
 */
bool pilha_cheia(pilha_t *p);

/**
 * @brief Empilha uma carta na pilha.
 * 
 * Esta função empilha uma carta em determinada pilha
 *
 * @param p Ponteiro para a pilha.
 * @param carta Carta a ser empilhada.
 */
void empilha_carta(pilha_t *p, carta_t carta);

/**
 * @brief Retorna a carta no topo da pilha sem removê-la.
 *
 * Esta função retorna a carta que está no topo de uma pilha.
 * 
 * @param p Ponteiro para a pilha.
 * @return A carta no topo da pilha.
 */
carta_t retorna_carta_topo(pilha_t *p);

/**
 * @brief Remove e retorna a carta no topo da pilha.
 * 
 * Esta função remove e reetorna a carta que está no topo de determinada pilha.
 *
 * @param p Ponteiro para a pilha.
 * @return A carta removida.
 */
carta_t remove_carta_topo(pilha_t *p);

/**
 * @brief Verifica se uma pilha de cartas está fechada.
 *
 * Esta função verifica se uma pilha está fechada, ou seja, todas as cartas viradas para baixo.
 * 
 * @param p Ponteiro para a pilha.
 * @return true se a pilha estiver fechada, false caso contrário.
 */
bool pilha_fechada(pilha_t *p);

/**
 * @brief Abre a carta no topo da pilha.
 *
 * Esta função abre a carta que está no topo de uma pilha.
 * 
 * @param p Ponteiro para a pilha.
 */
void abre_carta_topo_pilha(pilha_t *p);

/**
 * @brief Fecha todas as cartas na pilha.
 *
 * Esta função fecha todas as cartas de uma pilha, ou seja, vira todas para baixo.
 * 
 * @param p Ponteiro para a pilha.
 */
void fecha_todas_cartas_pilha(pilha_t *p);

/**
 * @brief Esvazia completamente uma pilha de cartas.
 *
 * Esta função esvazia, retira todas as cartas de uma pilha.
 * 
 * @param p Ponteiro para a pilha.
 */
void esvazia_pilha(pilha_t *p);

/**
 * @brief Gera um baralho completo em uma pilha.
 *
 * Esta função gera o baralho completo com 52 cartas.
 * 
 * @param p Ponteiro para a pilha.
 */
void gera_baralho_inteiro(pilha_t *p);

/**
 * @brief Embaralha as cartas em uma pilha.
 *
 * Esta função embaralha as cartas.
 * 
 * @param p Ponteiro para a pilha.
 */
void embaralha_cartas_pilha(pilha_t *p);

/**
 * @brief Retorna o número total de cartas em uma pilha.
 *
 * Esta função retorna a quantidade de cartas que uma pilha possui.
 * 
 * @param p Ponteiro para a pilha.
 * @return O número total de cartas.
 */
int numero_cartas_pilha(pilha_t *p);

/**
 * @brief Retorna o número de cartas fechadas em uma pilha.
 *
 * Esta função retorna a quantidade de cartas fechadas que uma pilha possui.
 * 
 * @param p Ponteiro para a pilha.
 * @return O número de cartas fechadas.
 */
int numero_cartas_fechadas_pilha(pilha_t *p);

/**
 * @brief Retorna o número de cartas abertas em uma pilha.
 *
 * Esta função retorna a quantidade de cartas abertas que uma pilha possui.
 * 
 * @param p Ponteiro para a pilha.
 * @return O número de cartas abertas.
 */
int numero_cartas_abertas_pilha(pilha_t *p);

/**
 * @brief Verifica se uma posição é válida para uma pilha de cartas.
 *
 * @param p Ponteiro para a pilha.
 * @param pos Posição a ser verificada.
 * @return true se a posição for válida, false caso contrário.
 */
bool posicao_valida(pilha_t *p, int pos);

/**
 * @brief Retorna a carta em uma posição específica da pilha.
 *
 * @param p Ponteiro para a pilha.
 * @param pos Posição desejada.
 * @param aberta Ponteiro para armazenar se a carta está aberta.
 * @return A carta na posição especificada.
 */
carta_t retorna_carta(pilha_t *p, int pos, bool *aberta);

/**
 * @brief Retorna o naipe de uma carta.
 *
 * @param c Carta a ser analisada.
 * @return O naipe da carta.
 */
naipe_t naipe_carta(carta_t c);

/**
 * @brief Retorna o valor de uma carta.
 *
 * @param c Carta a ser analisada.
 * @return O valor da carta.
 */
valor_t valor_carta(carta_t c);

/**
 * @brief Retorna a cor de uma carta.
 *
 * @param c Carta a ser analisada.
 * @return A cor da carta.
 */
cor_t cor_carta(carta_t c);

/**
 * @brief Gera uma descrição textual de uma carta.
 *
 * @param c Carta a ser descrita.
 * @param descricao Ponteiro para armazenar a descrição.
 */
void descricao_carta(carta_t c, char *descricao);

/**
 * @brief Compara duas cartas para determinar se são iguais.
 *
 * @param c1 Primeira carta.
 * @param c2 Segunda carta.
 * @return true se as cartas forem iguais, false caso contrário.
 */
bool compara_cartas(carta_t c1, carta_t c2);

/**
 * @brief Verifica se é possível empilhar uma carta em cima de outra.
 *
 * @param c Carta a ser empilhada.
 * @param p Pilha de destino.
 * @return true se a carta pode ser empilhada, false caso contrário.
 */
bool pode_empilhar(carta_t c, pilha_t p);

/**
 * @brief Verifica se é possível mover um número específico de cartas entre pilhas.
 *
 * @param origem Pilha de origem.
 * @param destino Pilha de destino.
 * @param n_cartas_a_mover Número de cartas a serem movidas.
 * @return true se o movimento é possível, false caso contrário.
 */
bool pode_mover(pilha_t *origem, pilha_t *destino, int n_cartas_a_mover);

/**
 * @brief Move um número específico de cartas entre pilhas, mantendo a ordem.
 *
 * @param origem Pilha de origem.
 * @param destino Pilha de destino.
 * @param n_cartas_a_mover Número de cartas a serem movidas.
 */
void move_cartas_em_ordem(pilha_t *origem, pilha_t *destino, int n_cartas_a_mover);

/**
 * @brief Inicializa as pilhas do jogo com cartas.
 *
 * @param j Ponteiro para a estrutura de dados do jogo.
 */
void inicia_pilhas_jogo(jogo_t *j);

/**
 * @brief Verifica se é possível mover uma carta para uma pilha de saída.
 *
 * @param j Ponteiro para a estrutura de dados do jogo.
 * @param n_pilha Índice da pilha de saída.
 * @param c Carta a ser movida.
 * @return true se o movimento é possível, false caso contrário.
 */
bool pode_mover_para_pilha_saida(jogo_t *j, int n_pilha, carta_t c);

/**
 * @brief Verifica se é possível mover uma carta para uma pilha principal.
 *
 * @param j Ponteiro para a estrutura de dados do jogo.
 * @param n_pilha Índice da pilha principal.
 * @param c Carta a ser movida.
 * @return true se o movimento é possível, false caso contrário.
 */
bool pode_mover_para_pilha_principal(jogo_t *j, int n_pilha, carta_t c);

/**
 * @brief Verifica se é possível mover um número específico de cartas de uma pilha.
 *
 * @param j Ponteiro para a estrutura de dados do jogo.
 * @param n_pilha Índice da pilha.
 * @param n_cartas_a_mover Número de cartas a serem movidas.
 * @return true se o movimento é possível, false caso contrário.
 */
bool pode_mover_cartas_pilha(jogo_t *j, int n_pilha, int n_cartas_a_mover);

/**
 * @brief Verifica se o jogador venceu o jogo.
 *
 * @param j Ponteiro para a estrutura de dados do jogo.
 * @return true se o jogador venceu, false caso contrário.
 */
bool venceu_jogo(jogo_t *j);

/**
 * @brief Abre a carta no topo da pilha de descarte.
 *
 * @param j Ponteiro para a estrutura de dados do jogo.
 * @return true se a carta foi aberta, false caso contrário.
 */
bool abre_carta(jogo_t *j);

/**
 * @brief Recicla as cartas da pilha de descarte para o monte.
 *
 * @param j Ponteiro para a estrutura de dados do jogo.
 * @return true se as cartas foram recicladas, false caso contrário.
 */
bool recicla_descarte(jogo_t *j);

/**
 * @brief Move uma carta da pilha de descarte para uma pilha de saída.
 *
 * @param j Ponteiro para a estrutura de dados do jogo.
 * @param n_pilha Índice da pilha de saída.
 * @return true se o movimento foi realizado, false caso contrário.
 */
bool move_carta_descarte_para_saida(jogo_t *j, int n_pilha);

/**
 * @brief Move uma carta da pilha de descarte para uma pilha principal.
 *
 * @param j Ponteiro para a estrutura de dados do jogo.
 * @param n_pilha Índice da pilha principal.
 * @return true se o movimento foi realizado, false caso contrário.
 */
bool move_carta_descarte_para_jogo(jogo_t *j, int n_pilha);

/**
 * @brief Move uma carta de uma pilha principal para uma pilha de saída.
 *
 * @param j Ponteiro para a estrutura de dados do jogo.
 * @param n_pilha_jogo Índice da pilha principal.
 * @param n_pilha_saida Índice da pilha de saída.
 * @return true se o movimento foi realizado, false caso contrário.
 */
bool move_carta_jogo_para_saida(jogo_t *j, int n_pilha_jogo, int n_pilha_saida);

/**
 * @brief Move uma carta da pilha de saída para a pilha de jogo.
 *
 * Esta função move uma carta da pilha de saída para a pilha de jogo,
 * se a jogada for válida.
 *
 * @param j Ponteiro para a estrutura de dados do jogo.
 * @param n_pilha_saida Índice da pilha de saída.
 * @param n_pilha_jogo Índice da pilha de jogo.
 * @return true se a carta foi movida com sucesso, false caso contrário.
 */
bool move_carta_saida_para_jogo(jogo_t *j, int n_pilha_saida, int n_pilha_jogo);

/**
 * @brief Move um número específico de cartas entre duas pilhas de jogo.
 *
 * Esta função move um número específico de cartas da pilha1 para a pilha2,
 * se a jogada for válida.
 *
 * @param j Ponteiro para a estrutura de dados do jogo.
 * @param n_pilha1 Índice da primeira pilha de jogo.
 * @param n_pilha2 Índice da segunda pilha de jogo.
 * @param n_cartas_a_mover Número de cartas a serem movidas.
 * @return true se as cartas foram movidas com sucesso, false caso contrário.
 */
bool move_cartas_entre_pilhas_jogo_com_qtde(jogo_t *j, int n_pilha1, int n_pilha2, int n_cartas_a_mover);

/**
 * @brief Move todas as cartas de uma pilha para outra pilha de jogo.
 *
 * Esta função move todas as cartas da pilha1 para a pilha2, se a jogada for válida.
 *
 * @param j Ponteiro para a estrutura de dados do jogo.
 * @param n_pilha1 Índice da primeira pilha de jogo.
 * @param n_pilha2 Índice da segunda pilha de jogo.
 * @return true se as cartas foram movidas com sucesso, false caso contrário.
 */
bool move_cartas_entre_pilhas_jogo(jogo_t *j, int n_pilha1, int n_pilha2);

/**
 * @brief Realiza uma jogada com base no comando fornecido.
 *
 * Esta função realiza uma jogada com base no comando fornecido,
 * se a jogada for válida.
 *
 * @param j Ponteiro para a estrutura de dados do jogo.
 * @param jogada Comando da jogada a ser realizada.
 * @return true se a jogada foi bem-sucedida, false caso contrário.
 */
bool realiza_jogada(jogo_t *j, char *jogada);

/**
 * @brief Processa as entradas do teclado para interação com o jogo.
 *
 * Esta função processa as entradas do teclado para interação com o jogo.
 *
 * @param j Ponteiro para a estrutura de dados do jogo.
 */
void processa_teclado(jogo_t * j);

/**
 * @brief Desenha local onde a carta vai aparecer.
 * 
 * @param lin Linha onde a carta será desenhada.
 * @param col Coluna onde a carta será desenhada.
 */
void desenho_de_local(int lin, int col);

/**
 * @brief Desenha uma carta fechada em uma determinada posição na tela.
 *
 * Esta função desenha uma carta fechada em uma determinada posição na tela.
 *
 * @param lin Linha onde a carta será desenhada.
 * @param col Coluna onde a carta será desenhada.
 */
void desenho_de_carta_fechada(int lin, int col);

/**
 * @brief Desenha uma carta aberta em uma determinada posição na tela.
 *
 * Esta função desenha uma carta aberta em uma determinada posição na tela.
 *
 * @param lin Linha onde a carta será desenhada.
 * @param col Coluna onde a carta será desenhada.
 * @param carta Carta a ser desenhada.
 */
void desenho_de_carta_aberta(int lin, int col, carta_t carta);

/**
 * @brief Desenha uma pilha fechada em uma determinada posição na tela.
 *
 * Esta função desenha uma pilha fechada em uma determinada posição na tela.
 *
 * @param lin Linha onde a pilha será desenhada.
 * @param col Coluna onde a pilha será desenhada.
 * @param p Ponteiro para a pilha a ser desenhada.
 */
void desenho_de_pilha_fechada(int lin, int col, pilha_t *p);

/**
 * @brief Desenha uma pilha aberta em uma determinada posição na tela.
 *
 * Esta função desenha uma pilha aberta em uma determinada posição na tela.
 *
 * @param lin Linha onde a pilha será desenhada.
 * @param col Coluna onde a pilha será desenhada.
 * @param p Ponteiro para a pilha a ser desenhada.
 */
void desenho_de_pilha_aberta(int lin, int col, pilha_t *p);

/**
 * @brief Desenha uma pilha aberta de forma compacta em uma determinada posição na tela.
 *
 * Esta função desenha uma pilha aberta de forma compacta em uma determinada posição na tela.
 *
 * @param lin Linha onde a pilha será desenhada.
 * @param col Coluna onde a pilha será desenhada.
 * @param p Ponteiro para a pilha a ser desenhada.
 */
void desenho_compacto_de_pilha_aberta(int lin, int col, pilha_t *p);

/**
 * @brief Inicializa as coordenadas das pilhas no jogo.
 *
 * Esta função inicializa as coordenadas das pilhas no jogo.
 *
 * @param j Ponteiro para a estrutura de dados do jogo.
 */
void inicializa_coordenadas(jogo_t *j);

/**
 * @brief Desenha as pilhas na tela do jogo.
 *
 * Esta função desenha as pilhas na tela do jogo.
 *
 * @param j Ponteiro para a estrutura de dados do jogo.
 */
void desenho_das_pilhas(jogo_t *j);

/**
 * @brief Desenha elementos adicionais na tela do jogo.
 *
 * Esta função desenha elementos adicionais na tela do jogo.
 *
 * @param j Ponteiro para a estrutura de dados do jogo.
 */
void desenhos_de_extras(jogo_t *j);

/**
 * @brief Desenha o fundo da tela do jogo.
 *
 * Esta função desenha o fundo da tela do jogo.
 *
 * @param j Ponteiro para a estrutura de dados do jogo.
 */
void desenho_do_fundo(jogo_t *j);

/**
 * @brief Desenha a tela do jogo.
 *
 * Esta função desenha a tela do jogo.
 *
 * @param j Ponteiro para a estrutura de dados do jogo.
 */
void desenho_da_tela(jogo_t *j);

/**
 * @brief Apresenta informações sobre o jogo.
 *
 * Esta função apresenta informações sobre o jogo.
 */
void apresentacao();

/**
 * @brief Função principal que inicia e executa o jogo.
 *
 * @return A pontuação obtida pelo jogador.
 */
double jogo();

/**
 * @brief Pergunta ao jogador se deseja jogar novamente.
 *
 * @param pontos Pontuação obtida no jogo anterior.
 * @return true se o jogador deseja jogar novamente, false caso contrário.
 */
bool quer_jogar_de_novo(double pontos);

#endif // FUNCOES_H