#ifndef BEAGLE_IR_DB_H
#define BEAGLE_IR_DB_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <my_global.h>
#include <mysql.h>

#define HOST "localhost" 
#define USER "root"  
#define PWD  "beagle1234" 
#define DB   "ir_db"

/* Get element from table ("vol_mais", "samsung", con) */
char *get_element(char *field, char *branch, MYSQL *con); 

/* Insert element into table ("ch_mais", "010101010", "lg", con); */
int set_element(char *field, char *value, char *branch, MYSQL *con);

/* Insert row in the table ("marca", "samsung", con) */
int insert_table(char *field, char *value, MYSQL *con); 

/* Grant database access to a specific user ("root", "ir_db", con) */
int grant_access(char *user, char *db, MYSQL *con); 

/* Create a table in the database ("TV", con) */
int create_table(char *table, MYSQL *con); 

/* Drop a table in the database ("TV", con) */
int drop_table(char *table, MYSQL *con); 

/* Create a table in the database ("TV", con) */
int create_db(char *db, MYSQL *con); 

/* Drop a table in the database ("TV", con) */
int drop_db(char *db, MYSQL *con); 

/* Default exit if mysql_query() returns an error */
void finish_with_error(MYSQL *con);

/* Configure databased used in this work */
int setup();

#endif
