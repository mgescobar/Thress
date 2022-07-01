#ifndef _tela_h_
#define _tela_h_

//
// tela.h
// ------
//
// fun��es de acesso � tela
//
// para uso na disciplina Laborat�rio de Programa��o I - 1o sem. 2019 - UFSM
//


#include <stdbool.h>

// inicializa��o da tela
// cria uma janela com o tamanho dado em pixels
// deve ser executada antes do uso de qualquer outra fun��o da tela
void tela_inicio(int largura, int altura, char nome[]);


// finaliza��o da tela
// deve ser chamada no final da utiliza��o da tela, nenhuma outra fun��o da
// tela deve ser chamada depois desta.
void tela_fim(void);


// inicia o desenho de uma imagem
// prepara o necess�rio para que se possa desenhar uma nova imagem.
// as fun��es de desenho s� devem ser chamadas ap�s esta fun��o.
// o que for desenhado ser� mantido em mem�ria, e s� ser� mostrado na tela
// quando for chamada a fun��o tela_termina_desenho().
void tela_inicia_desenho(void);


// atualiza a tela
// faz com o que foi desenhado na tela desde a �ltima chamada � 
// fun��o tela_inicia_desenho() realmente apare�a.
// caso necess�rio, espera at� que a imagem anterior tenha ficado
// na tela pelo tempo adequado.
// antes da chamada a esta fun��o a imagem anterior fica sendo exibida, 
// o conte�do da nova imagem fica s� na mem�ria.
// ap�s esta fun��o ser chamada, as fun��es de desenho n�o devem ser usadas
// enquanto a fun��o tela_inicia_desenho() n�o for chamada novamente.
void tela_termina_desenho(void);


// DESENHO

// desenha um c�rculo
// x e y s�o as coordenadas do centro do c�rculo, r � o raio
// l � a largura da linha, corl a cor da linha e corint � a cor
// do interior do c�rculo
void tela_circulo(float x, float y, float r, float l, int corl, int corint);

// desenha uma linha reta
// x1, y1 e x2, y2 s�o as coordenadas das pontas, l a largura e corl a cor
void tela_linha(float x1, float y1, float x2, float y2, float l, int corl);

// desenha um retangulo
// x1, y1 s�o as coordenadas do canto superior esquerdo, 
// x2, y2 as do inferior direito
// l � a largura da linha, corl a cor da linha e corint � a cor
// do interior do retangulo
void tela_retangulo(float x1, float y1, float x2, float y2, float l,
                    int corl, int corint);


// desenha texto centrado
// x,y coordenadas do meio do texto, tam tamanho das letras, c cor, t texto
void tela_texto(float x, float y, int tam, int c, char t[]);

// desenha texto � esquerda
// x,y coordenadas do fim do texto, tam tamanho das letras, c cor, t texto
void tela_texto_esq(float x, float y, int tam, int c, char t[]);

// desenha texto � direita
// x, y coordenadas do inicio do texto, tam tamanho das letras, c cor, t texto
void tela_texto_dir(float x, float y, int tam, int c, char t[]);



// CORES

// valores para representar cores pr�-definidas
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

// cria uma nova cor
// vm, az, vd componentes vermelho, azul e verde (entre 0 e 1) da nova cor
// retorna um valor inteiro que deve ser usado nas fun��es de desenho
int tela_cria_cor(float vm, float az, float vd);



// ENTRADA DE DADOS

// retorna a posi��o x, y do mouse
void tela_cade_o_rato(float *x, float *y);

// retorna se o bot�o do mouse est� apertado
bool tela_rato_apertado(void);

// retorna se o bot�o do mouse foi clicado (apertado e solto) desde a �ltima
// chamada a esta fun��o
bool tela_rato_clicado(void);

// retorna a posi��o do mouse na �ltima vez que ele foi clicado
void tela_rato_clique(float *x, float *y);

// retorna uma tecla digitada
// pode retornar um caractere imprim�vel ou '\b' para backspace ou '\n' para
// enter ou '\0' para qualquer outra coisa ou se n�o tiver sido digitado nada.
char tela_le_tecla(void);

// TEMPO

// retorna quantos segundos transcorreram desde o in�cio do programa
double tela_relogio(void);

#endif
