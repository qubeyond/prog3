#!/bin/bash

EXE="./build/main_app"
DB_FILE="./build/test_data.bin"

rm -f $DB_FILE

echo "1. МАССИВ"
$EXE --file $DB_FILE --query 'MPUSH my_arr Alpha'
$EXE --file $DB_FILE --query 'MPUSH my_arr Beta'
$EXE --file $DB_FILE --query 'MINSERT my_arr 1 Gamma'
$EXE --file $DB_FILE --query 'PRINT my_arr'
$EXE --file $DB_FILE --query 'MLEN my_arr'

echo -e "\n2. СТЕК"
$EXE --file $DB_FILE --query 'SPUSH my_stack Bottom'
$EXE --file $DB_FILE --query 'SPUSH my_stack Middle'
$EXE --file $DB_FILE --query 'SPUSH my_stack Top'
$EXE --file $DB_FILE --query 'PRINT my_stack'
$EXE --file $DB_FILE --query 'SPOP my_stack'
$EXE --file $DB_FILE --query 'PRINT my_stack'

echo -e "\n3. ОЧЕРЕДЬ"
$EXE --file $DB_FILE --query 'QPUSH my_queue Client1'
$EXE --file $DB_FILE --query 'QPUSH my_queue Client2'
$EXE --file $DB_FILE --query 'PRINT my_queue'
$EXE --file $DB_FILE --query 'QPOP my_queue'

echo -e "\n4. СПИСКИ"
$EXE --file $DB_FILE --query 'FPUSH_HEAD s_list Node1'
$EXE --file $DB_FILE --query 'FPUSH_TAIL s_list Node2'
$EXE --file $DB_FILE --query 'LPUSH_HEAD d_list DNode1'
$EXE --file $DB_FILE --query 'PRINT s_list'
$EXE --file $DB_FILE --query 'PRINT d_list'

echo -e "\n5. ХЕШ-ТАБЛИЦА"
$EXE --file $DB_FILE --query 'HSET users admin 1234'
$EXE --file $DB_FILE --query 'HSET users guest qwerty'
$EXE --file $DB_FILE --query 'HGET users admin'
$EXE --file $DB_FILE --query 'PRINT users'

echo -e "\n6. ДЕРЕВО"
$EXE --file $DB_FILE --query 'TINSERT my_tree 50'
$EXE --file $DB_FILE --query 'TINSERT my_tree 30'
$EXE --file $DB_FILE --query 'TINSERT my_tree 70'
$EXE --file $DB_FILE --query 'TINSERT my_tree 20'
$EXE --file $DB_FILE --query 'PRINT my_tree'

echo -e "\n7. ДАМП"
hexdump -C $DB_FILE | head -n 5