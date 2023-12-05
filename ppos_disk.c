#include <stdio.h>
#include "disk.h"
#include "ppos_disk.h"
#include <math.h>

disk_t ppos_disco;

// inicializacao do gerente de disco
// retorna -1 em erro ou 0 em sucesso
// numBlocks: tamanho do disco, em blocos
// blockSize: tamanho de cada bloco do disco, em bytes
int disk_mgr_init(int *numBlocks, int *blockSize)
{
    int result;
    if (sem_create(&(ppos_disco.acesso), 1) != 0)
        return -1;
    result = disk_cmd(DISK_CMD_INIT, 0, 0);
    ppos_disco.block_size = *blockSize = disk_cmd(DISK_CMD_BLOCKSIZE, 0, 0);
    ppos_disco.num_blocks = *numBlocks = disk_cmd(DISK_CMD_DISKSIZE, 0, 0);
    ppos_disco.ocupado = 0;
    ppos_disco.filaDisco = NULL;
    ppos_disco.disparado = 0;
    ppos_disco.cabeca = 0;
    ppos_disco.distperc = 0;

    return result;
}

// leitura de um bloco, do disco para o buffer
int disk_block_read(int block, void *buffer)
{
    int result;
    sem_down(&(ppos_disco.acesso));

    result = disk_cmd(DISK_CMD_READ, block, buffer);
    task_sleep(1000);

    sem_up(&(ppos_disco.acesso));

    return result;
}

// escrita de um bloco, do buffer para o disco
int disk_block_write(int block, void *buffer)
{
    int result;
    sem_down(&(ppos_disco.acesso));

    result = disk_cmd(DISK_CMD_WRITE, block, buffer);
    task_sleep(1000);

    sem_up(&(ppos_disco.acesso));

    return result;
}

diskrequest_t* escalonamento_disco(diskrequest_t* fila, char opc) {
  if (fila == NULL) return NULL;
  if (opc == 'f') {
    diskrequest_t* aux = fila;
    fila = fila->next;
    return aux;
  }

  else if (opc == 's') {
    diskrequest_t* aux = fila, *sel = fila;
    int menor = 99999;
    while (aux != NULL && aux != fila) {
         if (abs(ppos_disco.cabeca - aux->block) < menor) {
           menor = abs(ppos_disco.cabeca - aux->block);
           sel = aux;
         }
      aux = aux->next;
    }
    aux = sel->prev;
    aux->next = sel->next;
    sel->next->prev = aux;
    return sel;
  }

  else if (opc == 'c') {
    return NULL;
  }

  else return NULL;
}

void diskDriverBody(void* args) {
  while(1 == 1) {
    sem_down(&(ppos_disco.acesso));

    if (ppos_disco.disparado == 1) { //esse valor nunca é definido como 1.
      task_resume(ppos_disco.filaDisco);
      ppos_disco.filaDisco = ppos_disco.filaDisco->next;
      ppos_disco.disparado = 0;
    }

    if (ppos_disco.ocupado == 0 && ppos_disco.filaDisco != NULL) {
        diskrequest_t* pedido = escalonamento_disco(ppos_disco.filaSolicitacoes, 'f');
      //ppos_disco.distperc += abs(ppos_disco.cabeca - pedido->block); essa linha causa seg fault
      ppos_disco.cabeca = pedido->block;
      
        disk_cmd(pedido->cmd, pedido->block, pedido->buffer);
      
      
    printf("cabeca: %d\n", ppos_disco.cabeca); //essa linha nunca é impressa
    }
    sem_up(&(ppos_disco.acesso));
    task_yield();
  }
  
}

