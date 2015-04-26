#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <my_global.h>
#include <mysql.h>

#define DEBUG 0

int get_element(char *column, char *target, MYSQL *con); // Get element from table and store in an INT variable, pass the column and the row ("volume+","sony",con)
int insert_element(char *column, char *value, char *target, MYSQL *con); // Insert element in the table, pass the column and and the row ("volume+","11111","lg", con);
int insert_table(char *column, char *field ,MYSQL *con); // Insert row in the table, pass the column and the field ("marca","lg",con)
void finish_with_error(MYSQL *con); // Default exit if mysql_query returns an error
char *handle_string(char *user); // Function that transforms string user into "user", not used now (was used by grant_db to grant database access to a user)
int create_db(char *name, char *user, char *pwd, MYSQL *con);// Creates the database, pass the name, user accessing and pwd of the user ("phi_project","root","pwd",con);
int grant_db(char *user, char *db, MYSQL *con); // Grant database access to a specific user, not being used right now due to seg fault.
int create_table(char *table, char *db, char *user, char *pwd, MYSQL *con); // Create a table in the database, right now is a default table defined by the strings on the function

int main(int argc, char **argv)
{
	MYSQL *con = mysql_init(NULL);
	int i =0;

	if(con == NULL){
		fprintf(stderr, "%s\n", mysql_error(con));
		exit(1);
	}

	/* Executar apenas uma vez para criar o db */
	create_db("y","root","pwd",con);
	
	create_table("tv","y","root","pwd",con);
	insert_table("marca","sony",con);
	insert_table("marca","lg",con);
	insert_table("marca","phillips",con);
	insert_table("marca","samsung",con);
	insert_element("volume+","11101","lg",con);
	int vol_lg = get_element("volume+","lg",con);
	insert_element("on","100000111","sony",con);
	int on_sony = get_element("on","sony",con);

	printf("volume+ of lg = %d\non of sony = %d\n", vol_lg,on_sony);
	mysql_close(con);
	exit(0);
}

int get_element(char *column, char *target, MYSQL *con)
{
	char *cmd_arr[] = {"SELECT `", column, "` FROM tv WHERE marca='", target, "'"};
	char *str_value;

	int i;
	size_t len = 0;
	for(i=0; i<sizeof(cmd_arr)/(sizeof(char)*8); i++)
		len+= strlen(cmd_arr[i]);

	char *cmd = (char *) malloc ((len+1) * sizeof(char));
	for(i=0; i<sizeof(cmd_arr)/(sizeof(char)*8); i++)
		sprintf(cmd, "%s%s", cmd, cmd_arr[i]);

	if(DEBUG)
		printf("%s\n", cmd);

	if(mysql_query(con, cmd))
		finish_with_error(con);
   
	MYSQL_RES *result = mysql_store_result(con);
	if(result == NULL)
		finish_with_error(con);

	MYSQL_FIELD *field;
	int num_fields = mysql_num_fields(result);
	MYSQL_ROW row;
	
	while((row = mysql_fetch_row(result)))
		str_value = (row[0] ? row[0] : "NULL");

	mysql_free_result(result);

	free(cmd);
	return atoi(str_value);
}

int insert_element(char *column, char *value, char *target, MYSQL *con)
{
	char *cmd_arr[] = {"UPDATE tv SET `", column ,"`=", value, " WHERE marca='", target, "'"};
	char *cmd = "";

	int i;
	size_t len = 0;
	for(i=0; i<sizeof(cmd_arr)/(sizeof(char)*8); i++)
		len+= strlen(cmd_arr[i]);

	cmd = (char *) malloc ((len+1) * sizeof(char));
	for(i=0; i<sizeof(cmd_arr)/(sizeof(char)*8); i++)
		sprintf(cmd, "%s%s", cmd, cmd_arr[i]);

	if(DEBUG)
		printf("%s\n", cmd);

	if(mysql_query(con, cmd))
		finish_with_error(con);

	free(cmd);
	return 0;
}

int insert_table(char *column, char *field, MYSQL *con)
{
	char *cmd_arr[] = {"INSERT INTO tv(", column, ") VALUES ('", field, "')"};
	char *cmd = "";

	int i;
	size_t len = 0;
	for(i=0; i<sizeof(cmd_arr)/(8*sizeof(char)); i++)
		len+= strlen(cmd_arr[i]);

	cmd = (char *) malloc ((len+1) * sizeof(char));
	for(i=0; i<sizeof(cmd_arr)/(8*sizeof(char)); i++)
		sprintf(cmd, "%s%s", cmd, cmd_arr[i]);
	
	if(DEBUG)
		printf("%s\n", cmd);

	if(mysql_query(con, cmd))
		finish_with_error(con);
	
	free(cmd);
	return 0;
}

int create_table(char *table, char *db, char *user, char *pwd, MYSQL *con)
{
	char d_command[22] = "DROP TABLE IF EXISTS ";
	char c_command[13] = "CREATE TABLE ";
	char table1[50] = "(`marca` VARCHAR(40) NOT NULL,`volume+` INT,";
	char table2[50] = "`volume-` INT,`canal+` INT,`canal-` INT,`on` INT,";
	char table3[50] = "`off` INT,`address` INT, UNIQUE (`marca`))";

	size_t create_len = strlen(c_command)+strlen(table)+strlen(table1)+strlen(table2)+strlen(table3);
	size_t drop_len = strlen(d_command)+strlen(table);

	char *drop_command = (char *) malloc ((drop_len+1) * sizeof(char));
	char *create_command = (char*) malloc ((create_len+1) * sizeof(char));

	sprintf(drop_command, "%s%s",d_command, table);
	sprintf(create_command,"%s%s%s%s%s",c_command,table,table1,table2,table3);

	if(mysql_real_connect(con,"localhost",user,pwd,db,0,NULL,0) == NULL)
		finish_with_error(con);

	if(mysql_query(con, drop_command))
		finish_with_error(con);

	if(mysql_query(con,create_command))
		finish_with_error(con);

	if(DEBUG)
		printf("%s\n%s\n",drop_command,create_command);

	free(drop_command);
	free(create_command);
	return 0;
}

int create_db(char *name, char *user, char *pwd,  MYSQL *con)
{
	char *create_command = "CREATE DATABASE ";

	if(mysql_real_connect(con,"localhost",user,pwd,NULL,0,NULL,0) == NULL)
		finish_with_error(con);

	if(mysql_query(con, strcat(create_command, name)))
		finish_with_error(con);

	return 0;
}

void finish_with_error(MYSQL *con)
{
	fprintf(stderr, "%s\n", mysql_error(con));
	mysql_close(con);
	exit(1);
}

char *handle_string(char *user)
{
	char *old  = user;
	char *handle = "\"";
	char *result;

	result = (char *) malloc (strlen(old)+3 * sizeof(char));
	sprintf(result, "%s%s%s",handle,old,handle);

	printf("%s\n",result);

	return result;
}

int grant_db(char *user, char *db, MYSQL *con){
/*  char *grantall = "GRANT ALL ON y.* TO GUEST IDENTIFIED BY ";
	char *flush_command = "FLUSH PRIVILEGES";

	char *user_handle = handle_string(user);

	size_t grant_len = strlen(grantall)+strlen(user_handle);

	char *grant_command = (char *) malloc((grant_len+1)* sizeof (char));

	sprintf(grant_command, "%s%s", grantall,user_handle);

	printf("user_handle = %s\n",user_handle);
	printf("%s\n",grant_command);
*/
	if(mysql_query(con, "GRANT ALL ON y TO GUEST IDENTIFIED BY \"phy_dev\""))
		finish_with_error(con);

	printf("Grant Sucess!\n");
	
	if(mysql_query(con,"FLUSH PRIVILEGES"))
		finish_with_error(con);

	//free(result);
	//free(grant_command);	
	return 0;
	//exit(0);
}