/*
################################################################################
#                                INCLUSÕES                                     #
################################################################################
*/

#include "tela.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

/*
################################################################################
#                                 Definições                                   #
################################################################################
*/

#define transparente 0
#define azul 1
#define vermelho 2
#define verde 3
#define amarelo 4
#define preto 5
#define laranja 6
#define rosa 7
#define branco 8
#define marrom 9

#define LARG_TELA 600
#define ALT_TELA  800


typedef struct {
  int pontuacao;
  char nome[10];
}top;


/*
################################################################################
#                                DECLARAÇÕES                                   #
################################################################################
*/


//Funções principais:
void inicializa(int tab[4][4]);
void recebe_jogada(int tab[4][4], int *pp, bool *stp);
void tela_jogo(int tab[4][4], int *pp);
void adiciona_peca(int tab[4][4], int v_aux[4], int *pp, char mov);
bool fim(int tab[4][4], bool *stp);
void tela_melhores(int tab[4][4], top topscore[100], int corFundo);
void salva_jogo(int tab[4][4], top topscore[100]);

//Funções auxiliares:
int numero_aleatorio(int max);
bool pode_somar(int a, int b);
int pode_vertical(int tab[4][4]);
int pode_horizontal(int tab[4][4]);
void jog_cima(int tab[4][4], int v_aux[4], bool *movi);
void jog_baixo(int tab[4][4], int v_aux[4], bool *movi);
void jog_dir(int tab[4][4], int v_aux[4], bool *movi);
void jog_esq(int tab[4][4], int v_aux[4], bool *movi);
int nova_peca();
int pontuacao(int tab[4][4]);
void fogos(float x, float y, int corFundo, int tab[4][4], top topscore[100]);
void desenhatrofeu(float x, float y, int corFundo);


/*
################################################################################
#                                    MAIN                                      #
################################################################################
*/


int main(void)
{
  top topscore[1000];
  int tab[4][4], proxima_peca = nova_peca();
  int *pp = &proxima_peca;
  int c = 0;
  bool parei = false;
  bool *stp = &parei;

  srand(time(NULL));

  inicializa(tab);
  tela_inicio(LARG_TELA, ALT_TELA, "Threes");

  while (!fim(tab, stp)) {
    recebe_jogada(tab, pp, stp);
    tela_jogo(tab, pp);
  }

  salva_jogo(tab, topscore);

  do {
    fogos(10, 50, tela_cria_cor(0.0, 0.0, 0.1), tab, topscore);
    fogos(450, 50, tela_cria_cor(0.0, 0.0, 0.1), tab, topscore);
    c++;
  } while (c < 7);
  sleep(20);

  tela_fim();
  return 0;
}


/*
################################################################################
#                           FUNÇÕES PRINCIPAIS                                 #
################################################################################
*/


// Escolhe 9 espaços do tabuleiro aleatóriamente e os inicializa conforme as
//regras do jogo.
void inicializa(int tab[4][4])
{
  int lin, col, aux;

  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      tab[i][j] = 0;                        // Inicializa o tabuleiro com 0's.
    }
  }

  for (int i = 0; i < 9; i++) {
    lin = numero_aleatorio(4);
    col = numero_aleatorio(4);
    if (tab[lin][col] == '\0') {
      aux = numero_aleatorio(100);
      if (aux <= 35 ) {                     // 35% de chance de ser 1;
        tab[lin][col] = 1;
      } else if (aux > 35 && aux <= 70) {   // 35% de chance de ser 2;
        tab[lin][col] = 2;
      } else if (aux > 70 && aux <= 98) {   // 28% de chance de ser 3;
        tab[lin][col] = 3;
      } else {                              // 2% de chance de ser 6;
        tab[lin][col] = 6;
      }
    } else {
      i--;
    }
  }

  return;
}

// Recebe a jogada e a interpreta. Se for uma jogada válida adiciona uma nova
//peça e chama a função que descobre qual será a próxima. Se for 'p' finaliza o
//jogo.
void recebe_jogada(int tab[4][4], int *pp, bool *stp)
{
  int v_aux[4] = {0,0,0,0};
  bool movimentou = false;
  bool *movi = &movimentou;

  switch (tela_le_tecla()) {
    case 'w': case 'W':
      if (pode_vertical(tab) == 1 || pode_vertical(tab) == 3) {
        jog_cima(tab, v_aux, movi);
        if (*movi) {
          adiciona_peca(tab, v_aux, pp, 'w');
          *pp = nova_peca();
        }
      }
      break;
    case 's': case 'S':
      if (pode_vertical(tab) == 2 || pode_vertical(tab) == 3) {
        jog_baixo(tab, v_aux, movi);
        if (*movi) {
          adiciona_peca(tab, v_aux, pp, 's');
          *pp = nova_peca();
        }
      }
      break;
    case 'a': case 'A':
      if (pode_horizontal(tab) == 2 || pode_horizontal(tab) == 3) {
        jog_esq(tab, v_aux, movi);
        if (*movi) {
          adiciona_peca(tab, v_aux, pp, 'a');
          *pp = nova_peca();
        }
      }
      break;
    case 'd': case 'D':
      if (pode_horizontal(tab) == 1 || pode_horizontal(tab) == 3) {
        jog_dir(tab, v_aux, movi);
        if (*movi) {
          adiciona_peca(tab, v_aux, pp, 'd');
          *pp = nova_peca();
        }
      }
      break;
    case 'p': case 'P':
      *stp = true;
      break;
  }
  return;
}

// Mostra o jogo na tela.
void tela_jogo(int tab[4][4], int *pp)
{
  char tabc[10], num[5], pts[10], relogio[20];
  int q = 140;
  sprintf(num, "%d", *pp);
  sprintf(pts, "%d", pontuacao(tab));
  sprintf(relogio,"%.lf", tela_relogio());


  tela_inicia_desenho();
  tela_retangulo(0, 0, LARG_TELA, ALT_TELA, 5, transparente,
                 tela_cria_cor(0.6, 0.7, 0.8));

  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      sprintf(tabc, "%d", tab[i][j]);
      if (tab[i][j] == 0) {
        tela_circulo(90 + (j*q),90 + (i*q),60 , 2, branco,
                     tela_cria_cor(0.7, 0.8, 1));
        tela_texto(90 + (j*q),90 + (i*q),60 , transparente, tabc);
      } else if (tab[i][j] == 1) {
        tela_circulo(90 + (j*q),90 + (i*q),60 , 2, branco,
                     tela_cria_cor(0.4, 0.8, 0.9));
        tela_texto(90 + (j*q),90 + (i*q),60 , preto, tabc);
      } else if (tab[i][j] == 2) {
        tela_circulo(90 + (j*q),90 + (i*q),60 , 2, branco,
                     tela_cria_cor(0.8, 0.5, 0.7));
        tela_texto(90 + (j*q),90 + (i*q),60 , preto, tabc);
        tela_circulo(90, 735, 45, 1, preto,
                     tela_cria_cor(0.4, 0.5, 0.6));
      } else {
        tela_circulo(90 + (j*q),90 + (i*q),60 , 2, branco,
                     tela_cria_cor(0.5, 0.4, 0.8));
        tela_texto(90 + (j*q),90 + (i*q),60 , preto, tabc);
      }
    }
  }

  // Pontuação atual.
  tela_linha(0, 600, 600, 600, 4, preto);
  tela_texto(300, 650, 50, preto, pts);
  // Próxima peça.
  tela_texto(90, 660, 30, preto, "Próxima:");
  if (*pp == 1) {
    tela_circulo(90, 735, 45, 1, branco, tela_cria_cor(0.4, 0.8, 0.9));
    tela_texto(90, 735, 45, preto, num);
  } else if (*pp == 2) {
    tela_circulo(90, 735, 45, 1, branco, tela_cria_cor(0.8, 0.5, 0.7));
    tela_texto(90, 735, 45, preto, num);
  } else {
    tela_circulo(90, 735, 45, 1, branco, tela_cria_cor(0.5, 0.4, 0.8));
    tela_texto(90, 735, 45, preto, num);
  }
  // Informativo.
  tela_texto(480, 770, 20, preto, "Pressione P para sair.");
  // Tempo.
  tela_texto(520, 630, 30, preto, "Tempo:");
  tela_texto(520, 670, 30, preto, relogio);

  tela_termina_desenho();
  return;
}

// Adiciona a nova peça na linha/coluna que moveu mais peças.
void adiciona_peca(int tab[4][4], int v_aux[4], int *pp, char mov)
{
  int posmaior = 0;

  // Encontra a linha que moveu mais peças.
  for (int j = 1; j < 4; j++) {
    if (v_aux[j] > v_aux[posmaior]) {
      posmaior = j;
    }
  }

  // Adiciona a nova peça nessa linha.
  switch (mov) {
    case 'w':
      tab[3][posmaior] = *pp;
      break;
    case 's':
      tab[0][posmaior] = *pp;
      break;
    case 'a':
      tab[posmaior][3] = *pp;
      break;
    case 'd':
      tab[posmaior][0] = *pp;
      break;
  }

  return;
}

// Informa se o jogo acabou.
bool fim(int tab[4][4], bool *stp)
{

  if ((pode_horizontal(tab) == 0 && pode_vertical(tab) == 0) || *stp) {
    return true;
  } else {
    return false;
  }
}

// Mostra o ranking dos melhores na tela pós-jogo.
void tela_melhores(int tab[4][4], top topscore[100], int corFundo)
{
  {
    char pontosAux[100]={0}, nr[3]={0}, score[100]={0};
    int constante=30, c=0;
    sprintf(score, "%d", pontuacao(tab));

    tela_inicia_desenho();

    tela_retangulo(0, 0, LARG_TELA, ALT_TELA, 5, preto, corFundo);
    tela_texto(LARG_TELA/2, 50, 25, branco, "FIM DE JOGO!");
    tela_texto(LARG_TELA/2, 300, 35, branco, "SUA PONTUAÇÃO:");
    tela_texto(LARG_TELA/2, 350, 35, branco, score);
    tela_retangulo(40, 430, 560, 475, 5, amarelo, transparente);
    tela_texto(LARG_TELA/2, 450, 35, branco, "RANKING DOS 10 MELHORES");
    desenhatrofeu(0, 0, corFundo);
    desenhatrofeu(435, 0, corFundo);

    // Mostra na tela as 10 melhores jogadas.
    for (c=0;c<10;c++) {
      sprintf(nr, "%d", c+1);
      if (topscore[c].pontuacao != -404 && strcmp(topscore[c].nome, "zerar")
          != 0) {
        sprintf(pontosAux, "%d", topscore[c].pontuacao);
        tela_texto(170, 500+(c*constante), 20, branco, nr);
        tela_texto(190, 500+(c*constante), 20, branco, ")");
        tela_texto_dir(200, 490+(c*constante), 20, branco, topscore[c].nome);
        tela_texto(355, 500+(c*constante), 30, branco, "-");
        tela_texto_dir(380, 490+(c*constante), 20, branco, pontosAux);
      } else {
        tela_texto(170, 500+(c*constante), 20, branco, nr);
        tela_texto(190, 500+(c*constante), 20, branco, ")");
        tela_texto_dir(200, 490+(c*constante), 20, branco, "");
        tela_texto(355, 500+(c*constante), 30, branco, "-");
        tela_texto_dir(380, 490+(c*constante), 20, branco, "");
      }
    }
  }

  return;
}

// Salva a pontuação da jogada atual e o nome do jogador. Compara com os
//resultado anteriores e salva em um arquivo as 10 melhoes pontuações.
void salva_jogo(int tab[4][4], top topscore[1000])
{
  char nome[20]="zerar", auxnome[20]="zerar";
  int tam=0, i=0, auxpontos=0, player=0;
  FILE *placar;

  // Teste zerando a struct.
  for (int y=0;y<1000;y++) {
    strcpy(topscore[y].nome, "zerar");
    topscore[y].pontuacao = -404;
  }

  // Abre o arquivo com "a" e salva a jogada atual.
  placar = fopen("placar.txt", "a");
  if (placar == NULL) {
    printf("Não foi possivel abrir o arquivo. \n");
  } else {
    printf("Digite seu nome: ");
    scanf(" %s", nome);
    fprintf(placar, "%s %d", nome, pontuacao(tab));
    fclose(placar);

    // Abre o arquivo com "r" somente para ler o conteúdo.
    placar = fopen("placar.txt", "r");
    // Lê o arquivo e adiciona na struct a pontuação.
    do {
      fscanf(placar, "%s %d", topscore[i].nome, &topscore[i].pontuacao);
      tam++;
      i++;
    } while (!feof(placar));
    fclose(placar);

    // Ordena a struct.
    while (player+1 != tam) {
      for (int i=player+1;i<=tam;i++) {
        if (topscore[player].pontuacao < topscore[i].pontuacao) {
          auxpontos = topscore[player].pontuacao;
          strcpy(auxnome,topscore[player].nome);
          topscore[player].pontuacao = topscore[i].pontuacao;
          strcpy(topscore[player].nome, topscore[i].nome);
          topscore[i].pontuacao = auxpontos;
          strcpy(topscore[i].nome, auxnome);
        }
      }
    player++;
    }

    // Abre em "w" para sobrescrever o arquivo.
    placar = fopen("placar.txt", "w");
    // Preenche o arquivo com a struct devidamente ordenada de 1 à 10.
    for (int j=0;j<tam;j++) {
      fprintf(placar, "%s %d\n", topscore[j].nome, topscore[j].pontuacao);
    }
  }

  fclose(placar);
  return;
}


/*
################################################################################
#                            FUNÇÕES AUXILIARES                                #
################################################################################
*/


// Sorteia de forma aleatória um numero no intervalo [0,max].
int numero_aleatorio(int max)
{
    return (rand() % max);
}

// Diz se os dois valores podem ou não ser somados de acordo com as regras do
//jogo.
bool pode_somar(int a, int b)
{
  bool pode = false;

  if (a + b == 3 && (a != 0 && b != 0)) {
    pode = true;
  } else if (a == b && a > 2) {
    pode = true;
  }

  return pode;
}

// Retorna 0 se não puder mover verticalmente, 1 se puder mover somente para
//cima, 2 se puder mover somente para baixo e 3 se puder mover para cima e para
//baixo.
int pode_vertical(int tab[4][4])
{
  bool pode_cima = false, pode_baixo = false;

  // CIMA.
  for (int i = 1; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if (tab[i-1][j] == 0 && tab[i][j] != 0) {
        pode_cima = true;
      } else if (pode_somar(tab[i][j], tab[i-1][j])) {
        pode_cima = true;
      }
    }
  }

  // BAIXO.
  for (int i = 2; i >= 0; i--) {
    for (int j = 0; j < 4; j++) {
      if (tab[i+1][j] == 0 && tab[i][j] != 0) {
        pode_baixo = true;
      } else if (pode_somar(tab[i][j], tab[i+1][j])) {
        pode_baixo = true;
      }
    }
  }

  if (pode_cima && pode_baixo) {
    return 3;
  } else if (pode_cima && !pode_baixo) {
    return 1;
  } else if (pode_baixo && !pode_cima) {
    return 2;
  } else {
    return 0;
  }

}

// Retorna 0 se não puder mover horizontalmente, 1 se puder mover somente para a
//direita, 2 se puder mover somenta para a esquerda e 3 se puder mover para a
//direita e esquerda.
int pode_horizontal(int tab[4][4])
{
  bool pode_esq = false, pode_dir = false;

  // ESQUERDA.
  for (int i = 0; i < 4; i++) {
    for (int j = 1; j < 4; j++) {
      if (tab[i][j-1] == 0 && tab[i][j] != 0) {
        pode_esq = true;
      } else if (pode_somar(tab[i][j], tab[i][j-1])) {
        pode_esq = true;
      }
    }
  }

  // DIREITA.
  for (int i = 0; i < 4; i++) {
    for (int j = 2; j >= 0; j--) {
      if (tab[i][j+1] == 0 && tab[i][j] != 0) {
        pode_dir = true;
      } else if (pode_somar(tab[i][j], tab[i][j+1])) {
        pode_dir = true;
      }
    }
  }

  if (pode_esq && pode_dir) {
    return 3;
  } else if (pode_dir && !pode_esq) {
    return 1;
  } else if (pode_esq && !pode_dir) {
    return 2;
  } else {
    return 0;
  }

}

// Move o tabuleiro para cima.
void jog_cima(int tab[4][4], int v_aux[4], bool *movi)
{
  for (int i = 1; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if (tab[i-1][j] == 0){
        tab[i-1][j] = tab[i][j];
        tab[i][j] = 0;
        v_aux[j]++;
        *movi = true;
      } else if (pode_somar(tab[i][j], tab[i-1][j])) {
        tab[i-1][j] += tab[i][j];
        tab[i][j] = 0;
        v_aux[j]++;
        *movi = true;
      }
    }
  }

  return;
}

// Move o tabuleiro para baixo.
void jog_baixo(int tab[4][4], int v_aux[4], bool *movi)
{
  for (int i = 2; i >= 0; i--) {
    for (int j = 0; j < 4; j++) {
      if (tab[i+1][j] == 0){
        tab[i+1][j] = tab[i][j];
        tab[i][j] = 0;
        v_aux[j]++;
        *movi = true;
      } else if (pode_somar(tab[i][j], tab[i+1][j])) {
        tab[i+1][j] += tab[i][j];
        tab[i][j] = 0;
        v_aux[j]++;
        *movi = true;
      }
    }
  }

  return;
}

// Move o tabuleiro para a direita.
void jog_dir(int tab[4][4], int v_aux[4], bool *movi)
{
  for (int i = 0; i < 4; i++) {
    for (int j = 2; j >= 0; j--) {
      if (tab[i][j+1] == 0){
        tab[i][j+1] = tab[i][j];
        tab[i][j] = 0;
        v_aux[i]++;
        *movi = true;
      } else if (pode_somar(tab[i][j], tab[i][j+1])) {
        tab[i][j+1] += tab[i][j];
        tab[i][j] = 0;
        v_aux[i]++;
        *movi = true;
      }
    }
  }

  return;
}

// Move o tabuleiro para a esquerda.
void jog_esq(int tab[4][4], int v_aux[4], bool *movi)
{
  for (int i = 0; i < 4; i++) {
    for (int j = 1; j < 4; j++) {
      if (tab[i][j-1] == 0){
        tab[i][j-1] = tab[i][j];
        tab[i][j] = 0;
        v_aux[i]++;
        *movi = true;
      } else if (pode_somar(tab[i][j], tab[i][j-1])) {
        tab[i][j-1] += tab[i][j];
        tab[i][j] = 0;
        v_aux[i]++;
        *movi = true;
      }
    }
  }

  return;
}

// Escolhe, de acordo com as regras, o valor da nova peça.
int nova_peca()
{
  int nrmaior = 0, aux = 0;
  aux = numero_aleatorio(100);
  if (aux <= 32) {
    return 1;
  } else if (aux > 32 && aux <=64) {
    return 2;
  } else if (aux > 64 && aux <= 96) {
    return 3;
  } else if (aux > 96 && aux <= 100) {
    nrmaior = numero_aleatorio(100);
    if (nrmaior <= 50) {
      return 6;
    } else if (nrmaior > 50 && nrmaior <= 80) {
      return 12;
    } else if (nrmaior > 80 && nrmaior <= 95) {
      return 24;
    } else if (nrmaior > 95 && nrmaior <= 100) {
      return 48;
    }
  }
  return 0;
}

// Informa a pontuação atual.
int pontuacao(int tab[4][4])
{
  int score = 0;

  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if (tab[i][j] == 3) {                 // 3 = 3pts;
        score += 3;
      } else if (tab[i][j] > 3) {           // > 3 = 3 vezes o valor da metade.
        score += 3 * (tab[i][j] / 2);
      }
    }
  }

  return score;
}

// Tela de pós-jogo. Função que faz tela explodir em felicidade.
// Por causa da forma como são desenhados na tela, essa função deve ser chamada
//ao invés de 'tela_melhores'.
void fogos(float x, float y, int corFundo, int tab[4][4], top topscore[100])
{

  // Fogos sobem...
  for (int i=0;i<100;i+=4) {
    tela_melhores(tab, topscore, corFundo);

    tela_circulo(70+x, (150+y)-i, 3, 1.3, branco, 2);
    tela_termina_desenho();
  }
    tela_inicia_desenho();

  // Fogos explodem.
  for (int j=0;j<50; j+=4) {
    tela_inicia_desenho();

    tela_melhores(tab, topscore, corFundo);

    // Direita/Esquerda.
    tela_circulo((70+x)-(j+15), 50+y, 4, 1, branco, vermelho);
    tela_circulo((70+x)+(j+15), 50+y, 4, 1, branco, vermelho);

    // Em cima/Em baixo.
    tela_circulo((70+x), (50+y)-(j+15), 4, 1, branco, vermelho);
    tela_circulo((70+x), (50+y)+(j+15), 4, 1, branco, vermelho);

    // Na diagonal direita em cima/Na diagonal esquerda em baixo.
    tela_circulo((70+x)+j, (50+y)-j, 4, 1, branco, amarelo);
    tela_circulo((70+x)-j, (50+y)+j, 4, 1, branco, amarelo);

    // Na diagonal esquerda em cima/Na diagonal direita em baixo.
    tela_circulo((70+x)-j, (50+y)-j, 4, 1, branco, amarelo);
    tela_circulo((70+x)+j, (50+y)+j, 4, 1, branco, amarelo);

    // Direita/Esquerda.
    tela_circulo((70+x)-(j+10), 50+y, 4, 1, branco, rosa);
    tela_circulo((70+x)+(j+10), 50+y, 4, 1, branco, rosa);

    // Em cima/Em baixo.
    tela_circulo((70+x), (50+y)-(j-10), 4, 1, branco, rosa);
    tela_circulo((70+x), (50+y)+(j-10), 4, 1, branco, rosa);

    // Na diagonal direita em cima/Na diagonal esquerda em baixo.
    tela_circulo((70+x)+(j-10), (50+y)-(j-10), 4, 1, branco, branco);
    tela_circulo((70+x)-(j-10), (50+y)+(j-10), 4, 1, branco, branco);

    // Na diagonal esquerda em cima/Na diagonal direita em baixo.
    tela_circulo((70+x)-(j-10), (50+y)-(j-10), 4, 1, branco, branco);
    tela_circulo((70+x)+(j-10), (50+y)+(j-10), 4, 1, branco, branco);

    tela_termina_desenho();
    tela_inicia_desenho();
  }

  tela_inicia_desenho();
  tela_melhores(tab, topscore, corFundo);
  tela_termina_desenho();

  return;
}

// Desenha um trofeu bem bonito e super útil na tela de pós-jogo.
void desenhatrofeu(float x, float y, int corFundo)
{
  //                                                             dx / dy
  tela_retangulo(40+x,760+y, 120+x,780+y, 5, preto , amarelo); // x=80 y=20
  tela_retangulo(65+x,730+y, 95+x,760+y, 5, preto , amarelo);// x=30 y=30
  tela_circulo(80+x,670+y,50, 3, preto, amarelo); // x=80 y= 670
  tela_retangulo(75+x,720+y, 85+x,730+y, 5, preto , amarelo); // x= 10 y=10
  tela_retangulo(30+x,620+y,150+x,650+y, 5, corFundo, corFundo); // x=120 y=100
  tela_linha(30+x, 657+y, 130+x, 657+y, 4, preto); // x=100 y=0
  tela_linha(15+x, 670+y, 15+x, 690+y, 4, amarelo); // x=0 y=20
  tela_linha(145+x, 670+y, 145+x, 690+y, 4, amarelo);// x=0 y=20
  tela_linha(30+x, 650+y, 130+x, 650+y, 4, preto);// x= 100 y=0
  tela_linha(15+x, 670+y, 145+x, 670+y, 4, amarelo);// x= 130 y=0
  tela_linha(125+x, 690+y, 145+x, 690+y, 4, amarelo); // x= 20 y=0
  tela_linha(15+x, 690+y, 35+x, 690+y, 4, amarelo); // x = 20 y = 0

  return;
}
