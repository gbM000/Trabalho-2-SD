#!/bin/bash

NUM_EXECUCOES=20

NUM_PROCESSOS=4  

PROGRAMA="nome_do_executavel (sem extensão)"

SAIDA="resultados.txt"

> $SAIDA

for i in $(seq 1 $NUM_EXECUCOES); do
    echo "Executando teste $i com $NUM_PROCESSOS processos..." >> $SAIDA
    mpirun -np $NUM_PROCESSOS $PROGRAMA >> $SAIDA
    echo "--------------------" >> $SAIDA
done

echo "Execuções concluídas. As saídas foram salvas em $SAIDA."

