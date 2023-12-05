// PingPongOS - PingPong Operating System
// Prof. Carlos A. Maziero, DINF UFPR
// Versão 1.2 -- Julho de 2017

// interface do gerente de disco rígido (block device driver)

#ifndef __DISK_MGR__
#define __DISK_MGR__
#include "ppos.h"

// estruturas de dados e rotinas de inicializacao e acesso
// a um dispositivo de entrada/saida orientado a blocos,
// tipicamente um disco rigido.

// estrutura que representa um disco no sistema operacional
typedef struct diskrequest_t
{
  int cmd;
  int block;
  void *buffer;
  task_t *task;
  struct diskrequest_t *next, *prev;
} diskrequest_t;

// estrutura que representa um disco no sistema operacional
typedef struct
{
  semaphore_t acesso;
  int num_blocks;
  int block_size;
  int ocupado;
  int cabeca;
  long int distperc;
  task_t *filaDisco;
  diskrequest_t *filaSolicitacoes;
  short disparado;
} disk_t;

// inicializacao do gerente de disco
// retorna -1 em erro ou 0 em sucesso
// numBlocks: tamanho do disco, em blocos
// blockSize: tamanho de cada bloco do disco, em bytes
int disk_mgr_init(int *numBlocks, int *blockSize);

// leitura de um bloco, do disco para o buffer
int disk_block_read(int block, void *buffer);

// escrita de um bloco, do buffer para o disco
int disk_block_write(int block, void *buffer);

#endif
