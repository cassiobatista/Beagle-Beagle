#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <my_global.h>
#include <mysql.h>

#define HOST "localhost" 
#define USER "root"  
#define PWD   "beagle1234" 
#define DB    "ir_db"

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

int main(int argc, char **argv)
{
	return setup();
}

/* Get element from table, pass the column and the row ("vol_mais","philips",con) */
char *get_element(char *field, char *branch, MYSQL *con) 
{
	char *str_value; // pedro: no malloc?
	char *cmd_arr[] = {"SELECT ", field, " FROM tv WHERE marca='", branch, "'"};

	/* compute size of each string on vector that'll compose the command for calloc */
	uint8_t i, len = 0;
	for(i=0; i<sizeof(cmd_arr)/sizeof(cmd_arr[0]); i++)
		len += strlen(cmd_arr[i]);

	/* allocate space to and compose the string command */
	char *cmd = (char *) calloc ((len+1), sizeof(char));
	for(i=0; i<sizeof(cmd_arr)/sizeof(cmd_arr[0]); i++)
		sprintf(cmd, "%s%s", cmd, cmd_arr[i]);

	if(mysql_query(con, cmd))
		finish_with_error(con);
   
	/* pedro: comenta aqui em uma linha soh */
	MYSQL_RES *result = mysql_store_result(con);
	if(result == NULL)
		finish_with_error(con);

	/* pedro: funciona sem alloc pra str_value? row[0] é string? */
	MYSQL_ROW row;
	while((row = mysql_fetch_row(result)))
		str_value = (row[0] ? row[0] : "NULL");

	mysql_free_result(result);
	free(cmd);

	return (char *) str_value;
}

/* Insert element into table, pass the column and the row ("volume+", "11111", "lg", con); */
int set_element(char *field, char *value, char *branch, MYSQL *con) 
{
	char *cmd_arr[] = {"UPDATE tv SET `", field ,"`=", value, " WHERE marca='", branch, "'"};

	/* compute size of each string on vector that'll compose the command for calloc */
	uint8_t i, len = 0;
	for(i=0; i<sizeof(cmd_arr)/sizeof(cmd_arr[0]); i++)
		len += strlen(cmd_arr[i]);

	/* allocate space to and compose the string command */
	char *cmd = (char *) calloc ((len+1), sizeof(char));
	for(i=0; i<sizeof(cmd_arr)/sizeof(cmd_arr[0]); i++)
		sprintf(cmd, "%s%s", cmd, cmd_arr[i]);

	if(mysql_query(con, cmd))
		finish_with_error(con);
	free(cmd);

	return 0;
}

/* Insert row in the table ("marca", "lg", con) */
int insert_table(char *field, char *value, MYSQL *con) 
{
	char *cmd_arr[] = {"INSERT INTO tv(", field, ") VALUES ('", value, "')"};

	/* compute size of each string on vector that'll compose the command for calloc */
	uint8_t i, len = 0;
	for(i=0; i<sizeof(cmd_arr)/sizeof(cmd_arr[0]); i++)
		len += strlen(cmd_arr[i]);

	/* allocate space to and compose the string command */
	char *cmd = (char *) calloc((len+1), sizeof(char));
	for(i=0; i<sizeof(cmd_arr)/sizeof(cmd_arr[0]); i++)
		sprintf(cmd, "%s%s", cmd, cmd_arr[i]);

	if(mysql_query(con, cmd))
		finish_with_error(con);
	free(cmd);

	return 0;
}

int grant_access(char *user, char *db, MYSQL *con) 
{
	char *cmd_arr[] = {"GRANT ALL ON ", db, " TO GUEST IDENTIFIED BY \"", user, "\""};

	/* compute size of each string on vector that'll compose the command for calloc */
	uint8_t i, len = 0;
	for(i=0; i<sizeof(cmd_arr)/sizeof(cmd_arr[0]); i++)
		len += strlen(cmd_arr[i]);

	/* allocate space to and compose the string command */
	char *cmd = (char *) calloc((len+1), sizeof(char));
	for(i=0; i<sizeof(cmd_arr)/sizeof(cmd_arr[0]); i++)
		sprintf(cmd, "%s%s", cmd, cmd_arr[i]);

	if(mysql_query(con, cmd))
		finish_with_error(con);
	free(cmd);

	if(mysql_query(con, "FLUSH PRIVILEGES"))
		finish_with_error(con);

	return 0;
}

/* Create a table in the database */
int create_table(char *table, MYSQL *con) 
{
	char *cmd_arr[] = {
		"CREATE TABLE IF NOT EXISTS ", table, " (",
		"marca VARCHAR(40) NOT NULL,",
		"vol_mais VARCHAR(40),",
		"vol_menos VARCHAR(40),",
		"ch_mais VARCHAR(40),",
		"ch_menos VARCHAR(40),",
		"on_off VARCHAR(40),",
		"primary key(marca)",
		");"
	};

	/* compute size of each string on vector that'll compose the command for calloc */
	uint8_t i, len = 0;
	for(i=0; i<sizeof(cmd_arr)/sizeof(cmd_arr[0]); i++)
		len += strlen(cmd_arr[i]);

	/* allocate space to and compose the string command */
	char *cmd = (char *) calloc((len+1), sizeof(char));
	for(i=0; i<sizeof(cmd_arr)/sizeof(cmd_arr[0]); i++)
		sprintf(cmd, "%s%s", cmd, cmd_arr[i]);

	if(mysql_query(con, cmd))
		finish_with_error(con);
	free(cmd);

	return 0;
}

/* Drop a table in the database */
int drop_table(char *table, MYSQL *con) 
{
	char *cmd_arr[] = {"DROP TABLE IF EXISTS ", table};

	/* compute size of each string on vector that'll compose the command for calloc */
	uint8_t i, len = 0;
	for(i=0; i<sizeof(cmd_arr)/sizeof(cmd_arr[0]); i++)
		len += strlen(cmd_arr[i]);

	/* allocate space to and compose the string command */
	char *cmd = (char *) calloc((len+1), sizeof(char));
	for(i=0; i<sizeof(cmd_arr)/sizeof(cmd_arr[0]); i++)
		sprintf(cmd, "%s%s", cmd, cmd_arr[i]);

	if(mysql_query(con, cmd))
		finish_with_error(con);
	free(cmd);

	return 0;
}

/* Create a table in the database ("TV", con) */
int create_db(char *db, MYSQL *con)
{
	char cmd[60] = "CREATE DATABASE IF NOT EXISTS ";
	if(mysql_query(con, strcat(cmd, db)))
		finish_with_error(con);

	return 0;
}

/* Drop a table in the database ("TV", con) */
int drop_db(char *db, MYSQL *con)
{
	char cmd[60] = "DROP DATABASE IF EXISTS ";
	if(mysql_query(con, strcat(cmd, db)))
		finish_with_error(con);

	return 0;
}

/* Default exit if mysql_query returns an error */
void finish_with_error(MYSQL *con) 
{
	fprintf(stderr, "%s\n", mysql_error(con));
	mysql_close(con);
	exit(1);
}

/* Configure databased used in this work */
int setup()  
{
	/* init connection */
	MYSQL *con = mysql_init(NULL);   
	if(con == NULL)  
		finish_with_error(con);  
	 
	/* connect to MySQL (mysql -u root -p [beagle1234]) */   
	if(mysql_real_connect(con, HOST, USER, PWD, NULL, 0, NULL, 0) == NULL)   
		finish_with_error(con);  

	drop_db(DB, con); //drop/delete database
	create_db(DB, con); //create database
	 
	/* change database (USE ir_db) */
	if(mysql_select_db(con, DB)) 
		finish_with_error(con);  

	drop_table("tv", con); //drop/delete table
	create_table("tv", con); //create table
	/* set branch on primary key and values for remaining fields */
	insert_table("marca", "samsung", con);
	set_element("vol_mais", "1111000001110000011100000000111110", "samsung", con);
	set_element("vol_menos","1111000001110000011010000001011110", "samsung", con);
	set_element("ch_mais",  "1111000001110000001001000101101110", "samsung", con);
	set_element("ch_menos", "1111000001110000000001000111101110", "samsung", con);
	set_element("on_off",   "1111000001110000001000000101111110", "samsung", con);

	/* fetch bistream: just checking out (select) */
	printf("Samsung Vol+ = '%s'\n", get_element("vol_menos", "samsung", con));
	 
	/* close MySQL connection */
	mysql_close(con);

	return EXIT_SUCCESS;
}
