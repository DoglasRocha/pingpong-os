#include <stdio.h>
#include "ppos.h"
#include "disk.h"
#include "ppos_disk.h"
#include "ppos-core-globals.h"

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
    create_task(diskManager, )

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