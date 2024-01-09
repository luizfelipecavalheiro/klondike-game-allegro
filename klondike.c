/**
 * @file klondike.c
 *
 * @brief Jogo klondike (paciência) utilizando Allegro.
 *
 * Este programa implementa o jogo klondike utlizando a biblioteca Allegro, para isso é necessário possuir
 * a biblioteca a Allegro 5 instalada em seu computador.
 *
 * @author Luiz Felipe Cavalheiro
 *
 * @note Para rodar o jogo, digite: gcc -Wall -o klondike klondike.c telag.c -lallegro_font -lallegro_color -lallegro_ttf -lallegro_primitives -lallegro && ./klondike
 */

//Para rodar o jogo: gcc -Wall -o klondike klondike.c telag.c -lallegro_font -lallegro_color -lallegro_ttf -lallegro_primitives -lallegro && ./klondike
#include "funcoes.h"

/**
 * @brief Calcula o bônus com base no tempo da última jogada e pontos obtidos.
 *
 * Esta função calcula um bônus com base no tempo decorrido desde a última jogada
 * e nos pontos obtidos na jogada atual. O bônus é calculado de acordo com a fórmula:
 *   pontuacao = (7.0 - tempo_jogada) / 7.0 * 3.0 * pontos_da_jogada;
 * Se o tempo_jogada for maior ou igual a 7 segundos, o bônus é zero.
 *
 * @param j Ponteiro para a estrutura de dados do jogo.
 * @param pontos_da_jogada Número de pontos obtidos na jogada atual.
 * @return Valor do bônus calculado.
 */
double bonus(jogo_t *j, int pontos_da_jogada)
{
  double pontuacao;
  double tempo_jogada = tela_relogio() - j->tempo_ultima_jogada;
  if (tempo_jogada < 7)
    pontuacao = (7.0 - tempo_jogada) / 7.0 * 3.0 * pontos_da_jogada;
  else
    pontuacao = 0;
  j->tempo_ultima_jogada = tela_relogio();
  
  return pontuacao;
}

// verifica se a pilha está vazia
bool pilha_vazia(pilha_t *p)
{
  return p->n_cartas == 0;
}

// verifica se a pilha está cheia
bool pilha_cheia(pilha_t *p)
{
  return p->n_cartas == N_MAX_CARTAS;
}

// faça uma função que empilha uma carta em uma pilha. 
void empilha_carta(pilha_t *p, carta_t carta) 
{
  int n_cartas = p->n_cartas;
  // se a pilha está vazia, insere a carta aberta
  if (n_cartas == 0) {
    p->cartas[n_cartas] = carta;
    p->n_cartas++;
  } else { // se a pilha não está vazia
    // se a carta do topo esta fechada, adiciona carta e deixa fechada
    if (n_cartas == p->n_cartas_fechadas) {
      p->cartas[n_cartas] = carta;
      p->n_cartas++;
      p->n_cartas_fechadas++;

    } else { //se a carta do topo está aberta, adiciona carta e deixa aberta
      p->cartas[n_cartas] = carta;
      p->n_cartas++;
    }
  }
}

// retorna a carta no topo de uma pilha. 
// Quem chama precisa verificar se a pilha nao é vazia
carta_t retorna_carta_topo(pilha_t *p)
{
  assert(!pilha_vazia(p));
  return p->cartas[p->n_cartas - 1];
}

// retorna a carta no topo da pilha, removendo essa carta da pilha.
// Quem chama precisa verificar se a pilha nao esta vazia
carta_t remove_carta_topo(pilha_t *p)
{ 
  assert(!pilha_vazia(p));
  carta_t c = retorna_carta_topo(p);
  if (p->n_cartas == p->n_cartas_fechadas && p->n_cartas_fechadas > 0)
    p->n_cartas_fechadas--;
  p->n_cartas--;
  return c;
}

// verifica se a bilha está fechada
bool pilha_fechada(pilha_t *p)
{
  return p->n_cartas == p->n_cartas_fechadas;
}

// abre a carta do topo de uma pilha.
void abre_carta_topo_pilha(pilha_t *p)
{
  assert(pilha_fechada(p));
  p->n_cartas_fechadas--;
}

// fecha todas as cartas da pilha.
void fecha_todas_cartas_pilha(pilha_t *p)
{
  assert(!pilha_vazia(p));
  p->n_cartas_fechadas = p->n_cartas;
}

// esvazia a pilha
void esvazia_pilha(pilha_t *p)
{
  p->n_cartas = 0;
  p->n_cartas_fechadas = 0;
}

// gera baralho inteiro
void gera_baralho_inteiro(pilha_t *p)
{
  for (naipe_t n = ouros; n <= paus; n++) {
    for (valor_t v = as; v <= rei; v++) {
      carta_t c = cria_carta(v,n);
      p->cartas[p->n_cartas] = c;
      p->n_cartas++;
      p->n_cartas_fechadas++;
    }
  }
}

// embaralha cartas da pilha
void embaralha_cartas_pilha(pilha_t *p)
{
  assert(!pilha_vazia(p));
  srand(time(NULL));
  int i = 0, j = 0, metade = N_MAX_CARTAS/2;
  
  for (i = 0; i < N_MAX_CARTAS; i++) {
    // Gera um índice aleatório entre 0 e n_cartas (exclusivo)
    // Troca apenas se o índice i for par
    if (i % 2 == 0) {
      // Gera um índice aleatório entre i e n_cartas (exclusivo)
      //numero entre metade e N_MAX_CARTAS
      if (i < metade)
         j = (rand() % (N_MAX_CARTAS - metade)) + metade;
      //numero entre 0 e N_MAX_CARTAS / 2
      else
        j = (rand() % (N_MAX_CARTAS - metade));
    } else {
      // Não realiza troca, mantém o índice atual
      j = i;
    }
    // Troca as cartas nas posições i e j
    carta_t temp = p->cartas[i];
    p->cartas[i] = p->cartas[j];
    p->cartas[j] = temp;
  }

  for (i = 0; i < N_MAX_CARTAS; i++) {
    // Gera um índice aleatório entre 0 e n_cartas (exclusivo)
    // Troca apenas se o índice i for impar
    if (i % 2 != 0) {
      if (i < metade)
        j = (rand() % (N_MAX_CARTAS - metade));
      else
        j = (rand() % (N_MAX_CARTAS - metade)) + metade;
    } else {
      // Não realiza troca, mantém o índice atual
      j = i;
    }
    // Troca as cartas nas posições i e j
    carta_t temp = p->cartas[i];
    p->cartas[i] = p->cartas[j];
    p->cartas[j] = temp;
  }
  
  i = 0;
  while (i < N_MAX_CARTAS) {
      if (i < metade)
         j = (rand() % (N_MAX_CARTAS - metade)) + metade;
      //numero entre 0 e N_MAX_CARTAS / 2
      else
        j = (rand() % (N_MAX_CARTAS - metade));
      carta_t temp = p->cartas[j];
      p->cartas[j] = p->cartas[i];
      p->cartas[i] = temp;

      i++;
  }
  
}

// retorna numero de cartas da pilha
int numero_cartas_pilha(pilha_t *p)
{
  return p->n_cartas;
}

// retorna numero de cartas fechadas da pilha
int numero_cartas_fechadas_pilha(pilha_t *p)
{
  return p->n_cartas_fechadas;
}

// retorna numero de cartas abertas da pilha
int numero_cartas_abertas_pilha(pilha_t *p)
{
  return p->n_cartas - p->n_cartas_fechadas;
}

// verifica se posicao da carta na pilha é valida (não pode ser maior nem menor que o tamanho da pilha)
bool posicao_valida(pilha_t *p, int pos)
{
  if (pos >= 0) {
    if (pos < p->n_cartas) 
      return true;
    else 
      return false;
  } else {
    if (p->n_cartas + pos >= 0)
      return true;
    else
      return false;
  }
}

// retorna carta de uma determinada posicao na pilha
carta_t retorna_carta(pilha_t *p, int pos, bool *aberta)
{
  assert(posicao_valida(p,pos));
  if (pos >= 0) {
    if (aberta != NULL) {
      if (pos >= p->n_cartas_fechadas)
        *aberta = true;
      else 
        *aberta = false;
    }
    return p->cartas[pos];

  } else { // numero negativo
    if (aberta != NULL) {
      if (p->n_cartas + pos >= p->n_cartas_fechadas)
        *aberta = true;
      else
        *aberta = false;
    }
    return p->cartas[p->n_cartas + pos];
  }
}

// cria a carta atribuindo valor e naipe
carta_t cria_carta(valor_t valor, naipe_t naipe)
{
  carta_t c = {valor, naipe};
  return c;
}

// retorna naipe da carta
naipe_t naipe_carta(carta_t c)
{
  return c.naipe;
}

// retorna valor da carta
valor_t valor_carta(carta_t c)
{
  return c.valor;
}

// retorna a cor de uma carta
cor_t cor_carta(carta_t c)
{
  if (c.naipe == ouros || c.naipe == copas) {
    return naipe_vermelho;
  } else {
    return naipe_preto;
  }
}

// preenche a descricao da carta
void descricao_carta(carta_t c, char *descricao)
{
  // Inicializa a string vazia
  descricao[0] = '\0';

  // Adiciona o valor da carta à string
  switch (c.valor) {
    case as:     sprintf(descricao, "A"); break;
    case valete: sprintf(descricao, "J"); break;
    case dama:   sprintf(descricao, "Q"); break;
    case rei:    sprintf(descricao, "K"); break;
    default:     sprintf(descricao, "%d", c.valor);
  }

  // Adiciona o naipe da carta à string
  switch (c.naipe) {
    case copas:   strcat(descricao, "\u2665"); break;
    case ouros:   strcat(descricao, "\u2666"); break;
    case paus:    strcat(descricao, "\u2663"); break;
    case espadas: strcat(descricao, "\u2660"); break;
  }
}

// compara se duas cartas são iguais
bool compara_cartas(carta_t c1, carta_t c2)
{
  if (naipe_carta(c1) == naipe_carta(c2) && valor_carta(c1) == valor_carta(c2))
    return true;
  else 
    return false;
}

// testa se a carta c pode ser empilhada na pilha p, no jogo "solitaire"
bool pode_empilhar(carta_t c, pilha_t p)
{
  if (pilha_vazia(&p)) {
    return c.valor == rei;
  } else {
    carta_t topo = p.cartas[p.n_cartas - 1];
    if (cor_carta(c) == cor_carta(topo)) return false;
    return c.valor == topo.valor - 1;
  }
}

// Verifica as condicoes para mover n cartas 
bool pode_mover(pilha_t *origem, pilha_t *destino, int n_cartas_a_mover) 
{
  if (origem->n_cartas - n_cartas_a_mover >= 0 && destino->n_cartas + n_cartas_a_mover <= N_MAX_CARTAS )
    return true;
  else
    return false;
}

// Move uma quantidade de cartas em ordem
void move_cartas_em_ordem(pilha_t *origem, pilha_t *destino, int n_cartas_a_mover)
{
  assert(pode_mover(origem,destino,n_cartas_a_mover));
  int n_cartas_origem = origem->n_cartas;
  int pos = n_cartas_origem - n_cartas_a_mover;

  for (int i = pos; i < n_cartas_origem; i++) {
    destino->cartas[destino->n_cartas] = origem->cartas[i];
    destino->n_cartas++;
  }   
  origem->n_cartas -= n_cartas_a_mover;

}

// inicia as pilhas do jogo, distribuindo as cartas
void inicia_pilhas_jogo(jogo_t *j)
{
  j->sair = false;
  j->comando[0] = '\0';
  // esvazia pilhas
  esvazia_pilha(&j->monte);
  esvazia_pilha(&j->descarte);
  for (int i = 0; i < N_PILHAS_SAIDA; i++) {
    esvazia_pilha(&j->pilhas_saida[i]);
  }
  for (int i = 0; i < N_PILHAS_PRINCIPAIS; i++) {
    esvazia_pilha(&j->pilhas_principais[i]);
  }

  // gera baralho
  gera_baralho_inteiro(&j->monte);

  // embaralha as cartas
  embaralha_cartas_pilha(&j->monte);

  // distribui cartas na pilha
  // nao precisa verificar se pode empilhar, pois é o inicio do jogo
  for (int i = 0; i < N_PILHAS_PRINCIPAIS; i++) {
    for (int k = 0; k < i+1; k++) {
      empilha_carta(&j->pilhas_principais[i], remove_carta_topo(&j->monte));
    }

    fecha_todas_cartas_pilha(&j->pilhas_principais[i]);
    abre_carta_topo_pilha(&j->pilhas_principais[i]);
  }

  j->tempo_ultima_jogada = tela_relogio();
  j->pontos = 0.0;

}

// verifica se pode mover carta para pilha de saída
bool pode_mover_para_pilha_saida(jogo_t *j, int n_pilha, carta_t c)
{
  if(n_pilha < 0 || n_pilha >= N_PILHAS_SAIDA) return false;

  if ((pilha_vazia(&j->pilhas_saida[n_pilha]) && valor_carta(c) == as) || 
      (!pilha_vazia(&j->pilhas_saida[n_pilha]) && naipe_carta(c) == naipe_carta(retorna_carta_topo(&j->pilhas_saida[n_pilha])) &&
      valor_carta(c) == valor_carta(retorna_carta_topo(&j->pilhas_saida[n_pilha])) + 1)) {
        return true;
  } else {
    return false;
  }
}

// verifica se pode mover carta para pilha principal
bool pode_mover_para_pilha_principal(jogo_t *j, int n_pilha, carta_t c)
{
  if(n_pilha < 0 || n_pilha >= N_PILHAS_PRINCIPAIS) return false;

  if (pode_empilhar(c,j->pilhas_principais[n_pilha])) {
      return true;
  } else {
    return false;
  }
}

// verifica se pode mover cartas de uma pilha, só pode mover cartas abertas
bool pode_mover_cartas_pilha(jogo_t *j, int n_pilha, int n_cartas_a_mover)
{
  if (n_pilha < 0 || n_pilha >= N_PILHAS_PRINCIPAIS) return false;

  int n_cartas_abertas = numero_cartas_abertas_pilha(&j->pilhas_principais[n_pilha]);

  if (n_cartas_a_mover <= n_cartas_abertas)
    return true;
  else
    return false;

}

// verifica se venceu o jogo
bool venceu_jogo(jogo_t *j)
{
  int total_cartas = 0;
  for (int i = 0; i < N_PILHAS_SAIDA; i++){
    total_cartas += numero_cartas_pilha(&j->pilhas_saida[i]);
  }

  return total_cartas == N_MAX_CARTAS;
}

// move a carta do topo do monte para o topo do descarte aberta
bool abre_carta(jogo_t *j)
{
  if (!pilha_vazia(&j->monte)) {
    empilha_carta(&j->descarte,remove_carta_topo(&j->monte));
    return true;
  } else {
    return false;
  }
}

// move todas as cartas do descarte para o monte
bool recicla_descarte(jogo_t *j)
{
  if (pilha_vazia(&j->monte) && !pilha_vazia(&j->descarte)) {
    int num_cartas_descarte = numero_cartas_pilha(&j->descarte);
    for (int i = 0; i < num_cartas_descarte; i++) {
      empilha_carta(&j->monte,remove_carta_topo(&j->descarte));
    }
    fecha_todas_cartas_pilha(&j->monte);
    // reciclagem do descarte zera os pontos
    j->pontos = 0;
    return true;
  } else {
    return false;
  }
}

// move carta do descarte para saida
bool move_carta_descarte_para_saida(jogo_t *j, int n_pilha)
{
  if (!pilha_vazia(&j->descarte) && pode_mover_para_pilha_saida(j,n_pilha,retorna_carta_topo(&j->descarte))) {
    empilha_carta(&j->pilhas_saida[n_pilha],remove_carta_topo(&j->descarte));
    // carta colocada na pilha de saida da 15 pontos;
    j->pontos += 15;
    j->tempo_ultima_jogada = tela_relogio();
    return true;
  } else {
    return false;
  }
}

// move carta do descarte para jogo
bool move_carta_descarte_para_jogo(jogo_t *j, int n_pilha)
{
  if (!pilha_vazia(&j->descarte) && pode_mover_para_pilha_principal(j,n_pilha,retorna_carta_topo(&j->descarte))) {
    empilha_carta(&j->pilhas_principais[n_pilha],remove_carta_topo(&j->descarte));
    // carta movida do descarte para pilha de jogo dá 10 pontos + bonus
    j->pontos = j->pontos + 10 + bonus(j,10);
    return true;
  } else {
    return false;
  }
}

// move carta do jogo para a saida
bool move_carta_jogo_para_saida(jogo_t *j, int n_pilha_jogo, int n_pilha_saida) 
{
  
  if (n_pilha_jogo < 0 || n_pilha_jogo >= N_PILHAS_PRINCIPAIS) return false;

  if (!pilha_vazia(&j->pilhas_principais[n_pilha_jogo]) &&
      pode_mover_para_pilha_saida(j,n_pilha_saida,retorna_carta_topo(&j->pilhas_principais[n_pilha_jogo]))) {
    
    empilha_carta(&j->pilhas_saida[n_pilha_saida],remove_carta_topo(&j->pilhas_principais[n_pilha_jogo]));
    // carta colocada na pilha de saida da 15 pontos;
    j->pontos += 15;

    //abre a carta do topo se ela nao estover aberta
    if (numero_cartas_pilha(&j->pilhas_principais[n_pilha_jogo]) > 0 && 
        numero_cartas_fechadas_pilha(&j->pilhas_principais[n_pilha_jogo]) > 0 &&
        pilha_fechada(&j->pilhas_principais[n_pilha_jogo])) {

      abre_carta_topo_pilha(&j->pilhas_principais[n_pilha_jogo]);
      // abertura de carta na pilha de jogo dá 20 pontos + bônus;
      j->pontos = j->pontos + 20 + bonus(j,20);
    }
    
    return true;
  } else {
    return false;
  }
}

// move carta da saida para o jogo
bool move_carta_saida_para_jogo(jogo_t *j, int n_pilha_saida, int n_pilha_jogo) 
{
  
  if (n_pilha_saida < 0 || n_pilha_saida >= N_PILHAS_SAIDA) return false;

  // adicionar verificacao de a pilha nao estar vazia
  if (!pilha_vazia(&j->pilhas_saida[n_pilha_saida]) &&
      pode_mover_para_pilha_principal(j,n_pilha_jogo,retorna_carta_topo(&j->pilhas_saida[n_pilha_saida]))) {
    empilha_carta(&j->pilhas_principais[n_pilha_jogo],remove_carta_topo(&j->pilhas_saida[n_pilha_saida]));
    // carta retirada de pilha de saída retira 15 pontos.
    if (j->pontos - 15 < 0)
      j->pontos = 0;
    else
      j->pontos -= 15;
    j->tempo_ultima_jogada = tela_relogio();
    return true;
  } else {
    return false;
  }
}

// move tantas cartas de uma pilha do jogo para outra pilha do jogo
bool move_cartas_entre_pilhas_jogo_com_qtde(jogo_t *j, int n_pilha1, int n_pilha2, int n_cartas_a_mover)
{
  if(n_pilha1 < 0 || n_pilha1 >= N_PILHAS_PRINCIPAIS || n_pilha2 < 0 || n_pilha2 >= N_PILHAS_PRINCIPAIS)
    return false;

  if (pode_mover_cartas_pilha(j,n_pilha1,n_cartas_a_mover) &&
      pode_mover(&j->pilhas_principais[n_pilha1],&j->pilhas_principais[n_pilha2],n_cartas_a_mover)) {
    move_cartas_em_ordem(&j->pilhas_principais[n_pilha1],&j->pilhas_principais[n_pilha2],n_cartas_a_mover);
    
    // esse trecho estava em move cartas em ordem
    if (numero_cartas_pilha(&j->pilhas_principais[n_pilha1]) > 0 && 
        numero_cartas_fechadas_pilha(&j->pilhas_principais[n_pilha1]) > 0 && 
        pilha_fechada(&j->pilhas_principais[n_pilha1])) {
      abre_carta_topo_pilha(&j->pilhas_principais[n_pilha1]);
      j->pontos = j->pontos + 20 + bonus(j,20);
    }

    return true;
  } else {
    return false;
  }
}

// descobre quantas cartas devem ser movidas de uma pilha para outra
bool move_cartas_entre_pilhas_jogo(jogo_t *j, int n_pilha1, int n_pilha2)
{
  if(n_pilha1 < 0 || n_pilha1 >= N_PILHAS_PRINCIPAIS || n_pilha2 < 0 || n_pilha2 >= N_PILHAS_PRINCIPAIS)
    return false;
  
  int n_cartas_a_mover = numero_cartas_abertas_pilha(&j->pilhas_principais[n_pilha1]);
  int lim_max = numero_cartas_pilha(&j->pilhas_principais[n_pilha1]);
  int lim_min = numero_cartas_fechadas_pilha(&j->pilhas_principais[n_pilha1]);
  int i = 0;

  for (i = lim_min; i < lim_max; i++) {
    if (pode_empilhar(retorna_carta(&j->pilhas_principais[n_pilha1],i,NULL),j->pilhas_principais[n_pilha2])) {
      break;
    } else{
      n_cartas_a_mover--;
    }
  }

  if (n_cartas_a_mover > 0) {
    move_cartas_entre_pilhas_jogo_com_qtde(j,n_pilha1,n_pilha2,n_cartas_a_mover);
    return true;
  } else {
    return false;
  }
}

// verifica qual é a jogada e chama a respectiva funcao que move as cartas
bool realiza_jogada(jogo_t *j, char *jogada)
{
  if (jogada == NULL || jogada[0] == '\0') {
    return false;
  }

  char origem_char = jogada[0];
  char destino_char;
  if (jogada[1] == '\0'){
    if (jogada[0] == 'm')
      destino_char = 'p';
    else if (jogada[0] == 'p')
      destino_char = 'm';
    else if (jogada[0] == 'f')
      j->sair = true;
  } else destino_char = jogada[1];

  origem_char = toupper(origem_char);
  destino_char = toupper(destino_char);

  int origem, destino;
  
  // Mapeia os caracteres para índices numéricos
  if (origem_char >= 'A' && origem_char <= 'D') {
    origem = origem_char - 'A';
  } else if (origem_char >= '1' && origem_char <= '7') {
    origem = origem_char - '1';
  } else if (origem_char != 'M' && origem_char != 'P') {
    return false;
  }

  if (destino_char >= 'A' && destino_char <= 'D') {
    destino = destino_char - 'A';
  } else if (destino_char >= '1' && destino_char <= '7') {
    destino = destino_char - '1';
  } else if (destino_char != 'P' && destino_char != 'M') {
    return false;
  }
  
  //só pode mover da saida para pilha do jogo
  if (origem_char >= 'A' && origem_char <= 'D') {
    if (destino_char >= '1' && destino_char <= '7') 
      return move_carta_saida_para_jogo(j,origem,destino);
    else 
      return false;
  } else if (origem_char >= '1' && origem_char <= '7') { 
    //só pode mover da pilha do jogo, para outra do jogo ou para saida
    if (destino_char >= '1' && destino_char <= '7') 
      return move_cartas_entre_pilhas_jogo(j,origem,destino);
    else if (destino_char >= 'A' && destino_char <= 'D') 
      return move_carta_jogo_para_saida(j,origem,destino);
    else 
      return false;
  
  } else if (origem_char == 'M') {
    // só pode mover para descarte
    if (destino_char == 'P')
      return abre_carta(j);
    else 
      return false;
  
  } else if (origem_char == 'P') {
    // pode reciclar descarte, mover para jogo ou saida
    if (destino_char == 'M') 
      return recicla_descarte(j);
    else if (destino_char >= '1' && destino_char <= '7')
      return move_carta_descarte_para_jogo(j,destino);
    else if (destino_char >= 'A' && destino_char <= 'D')
      return move_carta_descarte_para_saida(j,destino);
    else
      return false;
  } else {
    return false;
  }

}

// lê o caractere digitado pelo usuário e armazena na "string" comando 
void processa_teclado(jogo_t * j)
{
  char tecla;

  tecla = tela_tecla();
  if (tecla == '\0') return;

  int nchar = strlen(j->comando);

  switch (tecla) {
    case '\b':
      if (nchar > 0) {
        j->comando[--nchar] = '\0';
      }
      break;
    case '\n':
      if (nchar > 0) {
        realiza_jogada(j, j->comando);
        j->comando[0] = '\0';
      }
      break;
    default:
      if (nchar < MAX_CHAR_CMD) {
        j->comando[nchar] = tecla;
        nchar++;
        j->comando[nchar] = '\0';
      }
      break;
  }

}

// desenha local da pilha
void desenho_de_local(int lin, int col)
{
  tela_retangulo(col, lin, col+CARTA_LARGURA, lin+CARTA_ALTURA, 1, branco, transparente);
}

// desenha carta fechada
void desenho_de_carta_fechada(int lin, int col)
{
  tela_retangulo(col, lin, col+CARTA_LARGURA, lin+CARTA_ALTURA, 2, branco, marrom);
  char txt[30];
  txt[0] = '\0';
  strcat(txt, "\u2592");
  int tam_borda_carta = 2;
  int tam_letra = CARTA_LARGURA - 4;
  int posXtexto = col + CARTA_LARGURA / 2 + tam_borda_carta/2;
  int posYtexto = lin + CARTA_ALTURA / 3;
  tela_texto(posXtexto, posYtexto, tam_letra, branco, txt);
}

// desenha carta aberta
void desenho_de_carta_aberta(int lin, int col, carta_t carta)
{
  char txt[50];
  int cor;
  int tam_borda_carta = 2;
  int tam_letra = CARTA_LARGURA / 5;
  int posXtexto = col + CARTA_LARGURA / 4;
  int posYtexto = lin + CARTA_ALTURA / 15 + tam_borda_carta;
  descricao_carta(carta,txt);
  if (cor_carta(carta) == naipe_vermelho)
    cor = vermelho;
  else
    cor = preto;
  tela_retangulo(col, lin, col+CARTA_LARGURA, lin+CARTA_ALTURA, tam_borda_carta, cor, branco);
  tela_texto(posXtexto, posYtexto, tam_letra, cor,txt);
  char naipe[5];
  naipe[0] = '\0';
  switch (naipe_carta(carta)) {
    case copas:   strcat(naipe, "\u2665"); break;
    case ouros:   strcat(naipe, "\u2666"); break;
    case paus:    strcat(naipe, "\u2663"); break;
    case espadas: strcat(naipe, "\u2660"); break;
  }
  tela_texto(col+CARTA_LARGURA/2, lin + CARTA_ALTURA/2, CARTA_LARGURA/2, cor,naipe);
  tela_texto_esq(col+CARTA_LARGURA - tam_borda_carta, lin + CARTA_ALTURA - CARTA_ALTURA/6 - tam_borda_carta, tam_letra,cor,txt);
}

// desenha pilha fechada
void desenho_de_pilha_fechada(int lin, int col, pilha_t *p)
{
  if (pilha_vazia(p))
    desenho_de_local(lin,col);
  else if (pilha_fechada(p))
    desenho_de_carta_fechada(lin,col);
  else
    desenho_de_carta_aberta(lin,col,retorna_carta_topo(p));
}

// desenha pilha aberta
void desenho_de_pilha_aberta(int lin, int col, pilha_t *p)
{
  int num_cartas_pilha = numero_cartas_pilha(p);
  int num_cartas_fechadas = numero_cartas_fechadas_pilha(p);

  if (!pilha_vazia(p)) {
    for (int i = 0; i < num_cartas_pilha; i++) {
      if (i > 0) {
        //atualizar os valores de lin
        lin += CARTA_ALTURA/5;
      }
      if (i < num_cartas_fechadas) 
        desenho_de_carta_fechada(lin,col);
      else
        desenho_de_carta_aberta(lin,col,retorna_carta(p,i,NULL));
    }
  }
  else 
    desenho_de_local(lin,col);
}

// desenha pilha compactada
void desenho_compacto_de_pilha_aberta(int lin, int col, pilha_t *p)
{
  int num_cartas_fechadas = numero_cartas_fechadas_pilha(p);
  int num_cartas_abertas = numero_cartas_abertas_pilha(p);
  if (!pilha_vazia(p)) {
    char txt[50];
    txt[0] = '\0';
    // desenha cartas fechadas
    if (num_cartas_fechadas > 2) {
      // desenha a primeira fechada
      desenho_de_carta_fechada(lin,col);
      lin += CARTA_ALTURA / 5;

      // desenha a fechada especial
      sprintf(txt, "%d fechada(s)",num_cartas_fechadas - 2);
      int tam_borda_carta = 2;
      int tam_letra = CARTA_LARGURA / 6;
      int posXtexto = col + CARTA_LARGURA / 3 + tam_borda_carta + tam_letra;
      int posYtexto = lin + CARTA_ALTURA / 15;
      tela_retangulo(col, lin, col+CARTA_LARGURA, lin+CARTA_ALTURA, tam_borda_carta, branco, marrom);
      tela_texto(posXtexto, posYtexto, tam_letra, branco,txt);
      lin += CARTA_ALTURA / 5;

      //desenha a ultima fechada
      desenho_de_carta_fechada(lin,col);
      lin += CARTA_ALTURA / 5;

    } else if (num_cartas_fechadas > 0 && num_cartas_fechadas <= 2) {
      for (int i = 0; i < num_cartas_fechadas; i++) {
        desenho_de_carta_fechada(lin,col);
        lin += CARTA_ALTURA / 5;
      }
    } else {
      desenho_de_local(lin,col);
    }

    // desenha cartas abertas
    if (num_cartas_abertas > 2) {
      //desenha a primeira aberta, a especial e a carta do topo
      desenho_de_carta_aberta(lin,col,retorna_carta(p,num_cartas_fechadas,NULL));
      lin += CARTA_ALTURA / 5;
      //carta especial abaixo
      txt[0] = '\0';
      sprintf(txt, "%d aberta(s)",num_cartas_abertas-2);
      int tam_borda_carta = 2;
      int tam_letra = CARTA_LARGURA/6;
      int posXtexto = col + CARTA_LARGURA / 3 + tam_borda_carta + tam_letra;
      int posYtexto = lin + CARTA_ALTURA / 15;
      int cor;
      if (cor_carta(retorna_carta_topo(p)) == naipe_vermelho)
        cor = preto;
      else
        cor = vermelho;
      tela_retangulo(col, lin, col+CARTA_LARGURA, lin+CARTA_ALTURA, tam_borda_carta, cor, branco);
      tela_texto(posXtexto, posYtexto, tam_letra, cor,txt);
      // carta especial acima
      lin += CARTA_ALTURA / 5;
      desenho_de_carta_aberta(lin,col,retorna_carta_topo(p));
      lin += CARTA_ALTURA / 5;
    } else if (num_cartas_abertas > 0 && num_cartas_abertas <= 2) {
      for (int i = 0; i < num_cartas_abertas; i++) {
        desenho_de_carta_aberta(lin,col,retorna_carta(p,num_cartas_fechadas+i,NULL));
        lin += CARTA_ALTURA / 5;
      }
    } else {
      desenho_de_local(lin,col);
    }

  }
  else
    desenho_de_local(lin,col);
}

// define as coordenadas x, y iniciais de cada pilha
void inicializa_coordenadas(jogo_t *j)
{
  int deslocamento_horizontal = LARGURA / 10;
  int deslocamento_vertical = ESPACO_ENTRE_CARTAS * 2;

  for (int i = 0; i < N_PILHAS; i++) {
    if (i < 2) {
      j->coordenadas_pilhas[i].col = deslocamento_horizontal;
      j->coordenadas_pilhas[i].lin = deslocamento_vertical;
      deslocamento_horizontal += CARTA_LARGURA + ESPACO_ENTRE_CARTAS;
    } else if (i >= 2 && i <= 5) {
      deslocamento_horizontal += CARTA_LARGURA + ESPACO_ENTRE_CARTAS;
      j->coordenadas_pilhas[i].col = deslocamento_horizontal;
      j->coordenadas_pilhas[i].lin = deslocamento_vertical;
    } else {
      if (i == 6) {
        deslocamento_horizontal = LARGURA/10;
        deslocamento_vertical += CARTA_ALTURA + ESPACO_ENTRE_CARTAS*3;
      }
      j->coordenadas_pilhas[i].col = deslocamento_horizontal;
      j->coordenadas_pilhas[i].lin = deslocamento_vertical;
      deslocamento_horizontal += CARTA_LARGURA + ESPACO_ENTRE_CARTAS;
    }
  }

}

// desenha todas as pilhas 
void desenho_das_pilhas(jogo_t *j)
{
  int i = 0;
  // desenho do monte
  if (numero_cartas_pilha(&j->monte) > 0) {
    desenho_de_pilha_fechada(j->coordenadas_pilhas[i].lin,j->coordenadas_pilhas[i].col,&j->monte);
  } else
    desenho_de_local(j->coordenadas_pilhas[i].lin,j->coordenadas_pilhas[i].col);
  i++;
  
  // desenho do descarte
  if (numero_cartas_pilha(&j->descarte) > 0)
    desenho_de_carta_aberta(j->coordenadas_pilhas[i].lin,j->coordenadas_pilhas[i].col,retorna_carta_topo(&j->descarte));
  else
    desenho_de_local(j->coordenadas_pilhas[i].lin,j->coordenadas_pilhas[i].col);
  i++;

  // desenho das pilhas de saida
  for (int k = 0; k < N_PILHAS_SAIDA; k++) {
    if (numero_cartas_pilha(&j->pilhas_saida[k]) > 0) 
      desenho_de_carta_aberta(j->coordenadas_pilhas[i].lin,j->coordenadas_pilhas[i].col,retorna_carta_topo(&j->pilhas_saida[k]));
    else 
      desenho_de_local(j->coordenadas_pilhas[i].lin,j->coordenadas_pilhas[i].col);
    i++;
  }

  // desenho das pilhas principais
  for (int k = 0; k < N_PILHAS_PRINCIPAIS; k++) {
    if (numero_cartas_pilha(&j->pilhas_principais[k]) > 0) {
      
      int num_cartas_pilha = numero_cartas_pilha(&j->pilhas_principais[k]);
      int lim_maximo_tela = ALTURA - CARTA_ALTURA / 2;
      int espaco_da_pilha = num_cartas_pilha * (CARTA_ALTURA / 5) + j->coordenadas_pilhas[i].lin + CARTA_ALTURA;

      if (espaco_da_pilha < lim_maximo_tela) 
        desenho_de_pilha_aberta(j->coordenadas_pilhas[i].lin,j->coordenadas_pilhas[i].col,&j->pilhas_principais[k]); 
      else
        desenho_compacto_de_pilha_aberta(j->coordenadas_pilhas[i].lin,j->coordenadas_pilhas[i].col,&j->pilhas_principais[k]); 
    } else {
      desenho_de_local(j->coordenadas_pilhas[i].lin,j->coordenadas_pilhas[i].col);
    }
    i++;
  }
}

// desenhas coisas extras na tela
void desenhos_de_extras(jogo_t *j)
{
  // pontuacao
  char pontuacao [30];
  sprintf(pontuacao,"Pontos: %.2f",j->pontos);
  tela_texto_esq(LARGURA - LARGURA/10,ALTURA - ALTURA/10,LARGURA/40,amarelo,pontuacao);

  // comando 
  char jogada [30];
  sprintf(jogada,"Digite sua jogada: %s",j->comando);
  tela_texto_dir(LARGURA/10,ALTURA - ALTURA/10,LARGURA/40,amarelo,jogada);

  // mouse
  int rx, ry;
  tela_rato_pos(&rx, &ry);

  // desenha um círculo na posição do mouse
  tela_circulo(rx, ry, 5, 2, naipe_vermelho, verde);
}

// desenho de fundo do jogo
void desenho_do_fundo(jogo_t *j)
{
  inicializa_coordenadas(j);
  //identificacao da pilha
  char c[1];
  c[0] = '\0';
  for (int i = 0; i < N_PILHAS; i++) {
    if (i == 0) {
      tela_texto_esq(j->coordenadas_pilhas[i].col + CARTA_LARGURA / 2 + ESPACO_ENTRE_CARTAS / 4 + 2, j->coordenadas_pilhas[i].lin - ESPACO_ENTRE_CARTAS - ESPACO_ENTRE_CARTAS / 4, CARTA_LARGURA / 3, amarelo, "M");
    } else if (i == 1) {
      tela_texto_esq(j->coordenadas_pilhas[i].col + CARTA_LARGURA / 2 + ESPACO_ENTRE_CARTAS / 4, j->coordenadas_pilhas[i].lin - ESPACO_ENTRE_CARTAS - ESPACO_ENTRE_CARTAS / 4, CARTA_LARGURA / 3, amarelo, "P");
    } else if (i >= 2 && i <= 5) {
      c[0] = 'A' - 2 + i;
      tela_texto_esq(j->coordenadas_pilhas[i].col + CARTA_LARGURA / 2 + ESPACO_ENTRE_CARTAS / 4, j->coordenadas_pilhas[i].lin - ESPACO_ENTRE_CARTAS - ESPACO_ENTRE_CARTAS / 4, CARTA_LARGURA / 3, amarelo, c);
    } else {
      c[0] = '1' - 6 + i;
      tela_texto_esq(j->coordenadas_pilhas[i].col + CARTA_LARGURA / 2 + ESPACO_ENTRE_CARTAS / 4, j->coordenadas_pilhas[i].lin - ESPACO_ENTRE_CARTAS - ESPACO_ENTRE_CARTAS / 4, CARTA_LARGURA / 3, amarelo, c);
    }
  }
  c[0] = '\0';
  
}

// desenha a tela, funcao que chama os desenhos mais específicos de cada parte
void desenho_da_tela(jogo_t *j)
{
  desenho_do_fundo(j);
  desenho_das_pilhas(j);
  desenhos_de_extras(j);
  tela_atualiza();
}

// tela de apresentacao do jogo
void apresentacao() 
{
  int fucsia = 11;
  int num_linhas = 1;
  int tam_letra = LARGURA / 40;
  int posY = ALTURA/6;
  tela_altera_cor(fucsia, 1, 0.2, 0.8, 1);
  // desenha um quadrado no contorno da janela
  tela_retangulo(LARGURA/10, ALTURA/10, LARGURA - LARGURA/10, ALTURA - ALTURA /10, 5, fucsia, transparente);
  
  char texto [100];
  sprintf(texto,"BEM VINDO AO KLONDIKE!!!");
  tela_texto(LARGURA/2,posY,tam_letra,amarelo,texto);
  tam_letra = LARGURA / 50;
  texto[0] = '\0';
  num_linhas++;
  sprintf(texto,"- Cada pilha é representada por uma letra ou número:");
  tela_texto_dir(LARGURA/10+tam_letra,posY + num_linhas*tam_letra+tam_letra,tam_letra,branco,texto);
  texto[0] = '\0';
  num_linhas++;
  sprintf(texto,"   > M: Monte de cartas;");
  tela_texto_dir(LARGURA/10+tam_letra,posY + num_linhas*tam_letra+tam_letra,tam_letra,amarelo,texto);
  texto[0] = '\0';
  num_linhas++;
  sprintf(texto,"   > P: Pilha de descarte;");
  tela_texto_dir(LARGURA/10+tam_letra,posY + num_linhas*tam_letra+tam_letra,tam_letra,amarelo,texto);
  texto[0] = '\0';
  num_linhas++;
  sprintf(texto,"   > A-D: Pilhas de saída;");
  tela_texto_dir(LARGURA/10+tam_letra,posY + num_linhas*tam_letra+tam_letra,tam_letra,amarelo,texto);
  texto[0] = '\0';
  num_linhas++;
  sprintf(texto,"   > 1-7: Pilhas de jogo;");
  tela_texto_dir(LARGURA/10+tam_letra,posY + num_linhas*tam_letra+tam_letra,tam_letra,amarelo,texto);
  texto[0] = '\0';
  num_linhas+=2;
  sprintf(texto,"- Para fazer uma jogada digite a pilha de origem e destino (ex.: '5b');");
  tela_texto_dir(LARGURA/10+tam_letra,posY + num_linhas*tam_letra+tam_letra,tam_letra,branco,texto);
  texto[0] = '\0';
  num_linhas+=2;
  sprintf(texto," - Jogadas (uma jogada possui 1 ou 2 caracteres):");
  tela_texto_dir(LARGURA/10+tam_letra,posY + num_linhas*tam_letra+tam_letra,tam_letra,branco,texto);
  texto[0] = '\0';
  num_linhas++;
  sprintf(texto,"       > 'm' = 'mp';");
  tela_texto_dir(LARGURA/10+tam_letra,posY + num_linhas*tam_letra+tam_letra,tam_letra,amarelo,texto);
  texto[0] = '\0';
  num_linhas++;
  sprintf(texto,"       > 'p' = 'pm';");
  tela_texto_dir(LARGURA/10+tam_letra,posY + num_linhas*tam_letra+tam_letra,tam_letra,amarelo,texto);
  texto[0] = '\0';
  num_linhas++;
  sprintf(texto,"       > '(1-7)(1-7)';");
  tela_texto_dir(LARGURA/10+tam_letra,posY + num_linhas*tam_letra+tam_letra,tam_letra,amarelo,texto);
  texto[0] = '\0';
  num_linhas++;
  sprintf(texto,"       > '(1-7)(a-d)';");
  tela_texto_dir(LARGURA/10+tam_letra,posY + num_linhas*tam_letra+tam_letra,tam_letra,amarelo,texto);
  texto[0] = '\0';
  num_linhas++;
  sprintf(texto,"       > '(a-d)(1-7)'");
  tela_texto_dir(LARGURA/10+tam_letra,posY + num_linhas*tam_letra+tam_letra,tam_letra,amarelo,texto);
  texto[0] = '\0';
  num_linhas+=2;
  sprintf(texto,"Obs.: O mouse não movimenta cartas. Para sair digite 'F' como jogada.");
  tela_texto_dir(LARGURA/10+tam_letra,posY + num_linhas*tam_letra+tam_letra,tam_letra,branco,texto);
  texto[0] = '\0';
  num_linhas+=2;
  tam_letra = LARGURA / 40;
  sprintf(texto,"Tecle <enter> para iniciar!!!");
  tela_texto(LARGURA/2,ALTURA - ALTURA/10 -tam_letra,tam_letra,amarelo,texto);
  tela_atualiza();
  char c;
  while (true) {
    c = tela_tecla();
    if (c == '\n')
      break;
  }
}

// funcao que "gera" o jogo
double jogo() 
{

  jogo_t *j = malloc(sizeof(jogo_t));
  inicia_pilhas_jogo(j);
  
  apresentacao();
  do {
    desenho_da_tela(j);
    processa_teclado(j);
  } while(!venceu_jogo(j) && j->sair == false);
  
  double pontos;
  
  if (j->sair == true)
    pontos = 0;
  else
    pontos = j->pontos;
  
  free(j);

  return pontos;
}

// verifica se o usuário quer jogar novamente
bool quer_jogar_de_novo(double pontos) 
{
  if (pontos == 0)
    return false;
  int fucsia = 11;
  int num_linhas = 0;
  int tam_letra = LARGURA / 40;
  int posY = ALTURA/3;
  tela_altera_cor(fucsia, 1, 0.2, 0.8, 1);
  // desenha um quadrado no contorno da janela
  tela_retangulo(LARGURA/10, ALTURA/10, LARGURA - LARGURA/10, ALTURA - ALTURA /10, 5, fucsia, transparente);
  
  char texto [100];
  sprintf(texto,"PARABÉNS!! VOCÊ VENCEU!!!");
  tela_texto(LARGURA/2,posY,tam_letra,amarelo,texto);
  texto[0] = '\0';
  num_linhas++;
  sprintf(texto,"PONTUAÇÃO: %.2f",pontos);
  tela_texto(LARGURA/2,posY + num_linhas*tam_letra+tam_letra,tam_letra,amarelo,texto);
  texto[0] = '\0';
  num_linhas+=2;
  sprintf(texto,"Digite 's' para jogar de novo ou 'n' para sair");
  tela_texto(LARGURA/2,ALTURA - ALTURA/10*2 - tam_letra,tam_letra,branco,texto);
  tela_atualiza();
  char c;
  while (true) {
    c = tela_tecla();
    if (c == 's' || c == 'S') {
      return true;
    } else if (c == 'n' || c == 'N') {
      return false;
    }
  }
}

int main()
{
  tela_inicio(LARGURA,ALTURA,"klondike");
  double pontos;
 
  do {
    pontos = jogo();
  } while(quer_jogar_de_novo(pontos));
  
  tela_fim();
 
  return 0;
}