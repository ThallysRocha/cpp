#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char cor[11][10] = {"especial","vermelho", "verde", "azul", "amarelo", "roxo", "rosa", "marrom", "preto", "branco", "cinza"};

struct casa{
  int id;
  char nome[50];
  int preco;
  int dono;
  int aluguel;
  int cor_indice;
};
struct jogador{
int id;
char nome[50];
int dinheiro;
int posicao;
int pena;
struct casa propriedades[40];
int num_propriedades;
};

struct casa tabuleiro [40];

void popula_tabuleiro(){
  for(int i = 0; i < 40; i++){
    tabuleiro[i].id = i;
    tabuleiro[i].dono = -1;
    
    if(i==0){
      strcpy(tabuleiro[i].nome, "inicio");
      tabuleiro[i].preco = 0;
      tabuleiro[i].aluguel = 0;
      tabuleiro[i].cor_indice = 0;
    }
    else if(i==10){
      strcpy(tabuleiro[i].nome, "prisão");
      tabuleiro[i].preco = 0;
      tabuleiro[i].aluguel = 0;
      tabuleiro[i].cor_indice = 0;
    }
    else if(i==20){
      strcpy(tabuleiro[i].nome, "estacionamento");
      tabuleiro[i].preco = 0;
      tabuleiro[i].aluguel = 0;
      tabuleiro[i].cor_indice = 0;
    }
    else if (i==30){
      strcpy(tabuleiro[i].nome, "vá para a prisão");
      tabuleiro[i].preco = 0;
      tabuleiro[i].aluguel = 0;
      tabuleiro[i].cor_indice = 0;
    }
    else {
      tabuleiro[i].cor_indice = i%10;
      tabuleiro[i].preco = 60 + rand() % 200;
      tabuleiro[i].aluguel = 10 + rand() % 50;
    }
}
}
void imprime_casa(struct casa c){
  printf("casa %d:\n", c.id);
  printf("dono: %d\n", c.dono);
  printf("preco: %d\n", c.preco);
  printf("aluguel: %d\n", c.aluguel);
  printf("cor: %s\n", cor[c.cor_indice]);
}
void imprime_tabuleiro(){
  for(int i = 0; i < 40; i++){
    imprime_casa(tabuleiro[i]);
  }
}

void inicializa_jogadores(struct jogador *jogadores, int num_jogadores){
  for(int i = 0; i < num_jogadores; i++){
    jogadores[i].id = i;
    jogadores[i].dinheiro = 1000;
    jogadores[i].posicao = 0;
    jogadores[i].pena = 0;
    jogadores[i].num_propriedades = 0;
  }
}
void imprime_jogador(struct jogador jogador){
  printf("jogador %d:\n", jogador.id);
  printf("dinheiro: %d\n", jogador.dinheiro);
  printf("posicao: %d\n", jogador.posicao);
  printf("pena: %d\n", jogador.pena);
  printf("propriedades:\n");
  for(int i = 0; i < jogador.num_propriedades; i++){
    imprime_casa(jogador.propriedades[i]);
    printf("\n");
  }
}
void imprime_jogadores(struct jogador *jogadores, int num_jogadores){
  for(int i = 0; i < num_jogadores; i++){
    imprime_jogador(jogadores[i]);
  }
}

int joga_dado(){
  int valor_dado = rand() % 6 + 1;
  printf("tirou %d no dado\n" , valor_dado);
  return valor_dado;
}

void comprar_casa(struct jogador *jogador, int casa_id){
  jogador->dinheiro -= tabuleiro[casa_id].preco;
  tabuleiro[casa_id].dono = jogador->id;
  jogador->propriedades[jogador->num_propriedades] = tabuleiro[casa_id];
  jogador->num_propriedades++;
}
void pagar_aluguel(struct jogador *jogador, struct jogador *dono,int casa_id){
  jogador->dinheiro -= tabuleiro[casa_id].aluguel;
  dono->dinheiro += tabuleiro[casa_id].aluguel;
}

void entregar_bens(struct jogador *jogador,struct jogador *dono){
  dono->dinheiro += jogador->dinheiro;
  jogador->dinheiro = 0;
  for(int i = 0; i < jogador->num_propriedades; i++){
    tabuleiro[jogador->propriedades[i].id].dono = dono->id;
    dono->propriedades[dono->num_propriedades+1] = jogador->propriedades[i];
    dono->num_propriedades++;
    
  }
  jogador->num_propriedades = 0;
}

void jogar(struct jogador *jogadores, int num_jogadores){
  int jogadores_ativos = num_jogadores;
  int rodar = 1;
  while(rodar){
    for(int i = 0; i < num_jogadores; i++){
      if(jogadores[i].dinheiro > 0){
        if(jogadores_ativos == 1){
          printf("o jogador %d venceu!", i);
          rodar = 0;
          break;
        }
        printf("\n");
        printf("jogador %d, é a sua vez de jogar!\n", i);
        imprime_jogador(jogadores[i]);
        printf("\n");
        if(jogadores[i].pena == 0){
          
          int nova_posicao = jogadores[i].posicao + joga_dado();
          jogadores[i].posicao = nova_posicao;
          printf("O jogador %d está na posição %d\n",i, nova_posicao);
          if(nova_posicao >= 40) {
            nova_posicao = nova_posicao - 40;
            jogadores[i].posicao = nova_posicao;
            jogadores[i].dinheiro += 200;
            printf("jogador %d passou pelo inicio e recebeu 200, seu novo saldo é: %d\n",i,jogadores[i].dinheiro);
          }
          else if(nova_posicao == 30){
            jogadores[i].pena = 3;
            printf("jogador %d foi preso, e deverá esperá três turnos para poder jogar novamente.",i);
            jogadores[i].posicao = 10;
          }
          else if(nova_posicao != 20 && nova_posicao != 10){
              if(tabuleiro[nova_posicao].dono == -1 
                && jogadores[i].dinheiro >= tabuleiro[nova_posicao].preco){
                
                printf("jogador %d deseja comprar a casa %d por %d?[Y/N]\n", 
                  jogadores[i].id,tabuleiro[nova_posicao].id,
                  tabuleiro[nova_posicao].preco);
                imprime_casa(tabuleiro[nova_posicao]);
                char resposta = '-';
                scanf(" %c",&resposta);
                if(resposta == 'Y' || resposta == 'y'){
                  comprar_casa(&jogadores[i], nova_posicao);
                  printf("O jogador %d comprou a casa %d, seu saldo agora é %d\n",i,nova_posicao,jogadores[i].dinheiro);
                }
              }
              else if(tabuleiro[nova_posicao].dono == -1 
                  && jogadores[i].dinheiro < tabuleiro[nova_posicao].preco){
                    printf("O jogador %d não tem dinheiro para comprar a casa %d, o preço é %d\n",
                      i,tabuleiro[nova_posicao].id,
                      tabuleiro[nova_posicao].preco);
                  }
                
              
              else if(tabuleiro[nova_posicao].dono != i 
                && jogadores[i].dinheiro >= tabuleiro[nova_posicao].aluguel){              
                jogadores[i].dinheiro -= tabuleiro[nova_posicao].aluguel;
                printf("O jogador %d parou na casa do jogador %d e pagou o aluguel de %d, seu saldo agora é %d\n",
                  i,
                  tabuleiro[nova_posicao].dono,
                  tabuleiro[nova_posicao].aluguel,jogadores[i].dinheiro);
              }
              else if(tabuleiro[nova_posicao].dono != i 
                && jogadores[i].dinheiro < tabuleiro[nova_posicao].aluguel){
                  printf("O jogador %d não tem dinheiro para pagar o aluguel de %d, decretou falência e perdeu o jogo\n todos os seus bens serão entregues ao jogador %d\n",i,tabuleiro[nova_posicao].aluguel,tabuleiro[nova_posicao].dono);
                  entregar_bens(&jogadores[i],&jogadores[tabuleiro[nova_posicao].dono]);
                  jogadores_ativos--;
                }
        }
      }
        else{
          jogadores[i].pena--;
          printf("jogador %d está preso e não pode jogar, faltam %d turnos para ele sair.\n", i,jogadores[i].pena);
        }        
      }
  }
}
}
int main(void) {
  popula_tabuleiro();
  printf("escolha o número de jogadores: [2-4]\n");
  int num_jogadores;
  scanf(" %d",&num_jogadores);
  while(num_jogadores>4 && num_jogadores<2){
    printf("escolha o número de jogadores: [2-4]\n");
    scanf(" %d",&num_jogadores);
  }
  struct jogador jogadores[num_jogadores];
  inicializa_jogadores(jogadores, num_jogadores);
  jogar(jogadores, num_jogadores);
  return 0;
}
