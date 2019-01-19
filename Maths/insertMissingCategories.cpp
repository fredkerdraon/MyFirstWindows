//To compile this we need: g++ -Wall -Wl,--no-as-needed -o simple_call_scilab.exe $(pkg-config --libs --cflags scilab) $(ls /usr/lib/scilab/ | sed 's/^lib/-l/g' | sed "s/\.so.*$//g" | sort | uniq | grep -v disable) simple_call_scilab.c

//-lsciaction_binding -lsciarnoldi -lscicall_scilab -lscicommons -lscicompletion -lsciconsole -lsciconsole-minimal -lsciexternal_objects -lsciexternal_objects_java -lscifunctions -lscigraphic_export -lscigraphic_objects -lscigraphics -lscigui -lscihdf5 -lscihelptools -lscihistory_browser -lscihistory_manager -lsciinterpolation -lscijvm -lscilab -lscilab-cli -lscilocalization -lscimatio -lscioptimization -lsciparallel -lscipreferences -lscipreferences-cli -lscirandlib -lscirenderer -lsciscicos -lsciscicos_blocks -lsciscicos_blocks-cli -lsciscicos-cli -lsciscinotes -lscisignal_processing -lscisound -lscispecial_functions -lscispreadsheet -lscistatistics -lscisundials -lscisymbolic -lscitclsci -lscitypes -lsciui_data -lsciumfpack -lscixcos -lscixml

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <stdlib.h>
#include "call_scilab.h"
#include "api_scilab.h"
#include "Scierror.h"
#include "BOOL.h"
#include "api_common.h"
#include "sciprint.h"
#include "string.h"

//#include "my_global.h" // Include this file first to avoid problems
#include "mysql.h" // MySQL Include File
#define SERVER "localhost"
#define USER "username"
#define PASSWORD "password"
#define DATABASE "databasename"


using namespace std;

int main( int argc, char* argv[])
{
	setenv("START","/usr/share/scilab/etc/scilab.start",1);
	setenv("SCI","/usr/share/scilab",1);

	printf("Connection to the database...\n");

 	MYSQL *connect; // Create a pointer to the MySQL instance
 	connect=mysql_init(NULL); // Initialise the instance
 	/* This If is irrelevant and you don't need to show it. I kept it in for Fault Testing.*/
    	if(!connect)    /* If instance didn't initialize say so and exit with fault.*/
    	{
        	fprintf(stderr,"MySQL Initialization Failed");
        	return 1;
    	}
    	/* Now we will actually connect to the specific database.*/

    	connect=mysql_real_connect(connect,"localhost","root","fred1301","managementoverview",0,NULL,0);
    	/* Following if statements are unneeded too, but it's worth it to show on your
    	first app, so that if your database is empty or the query didn't return anything it
    	will at least let you know that the connection to the mysql server was established. */

    	if(connect)
	{
        	printf("Connection Succeeded\n");
    	}
    	else
	{
        	printf("Connection Failed!\n");
    	}
	
	printf("\n");
	printf("Running the query...\n");

	unsigned int numrows = 0;
	string Category;
	string Year;
	string Month;

	if(strcmp(argv[1],"insertMissingCategories") == 0)
	{
			printf("Second param: %s\n",argv[2]);				
			printf("Third param: %s\n",argv[3]);				

			MYSQL_RES *res_set; /* Create a pointer to recieve the return value.*/
			MYSQL_RES *res_set_months; /* Create a pointer to recieve the return value.*/
    			MYSQL_ROW row;  /* Assign variable for rows. */
    			MYSQL_ROW row_months;  /* Assign variable for rows. */
    			//mysql_query(connect,"SELECT * FROM cashflows");
			unsigned int a = 0;
    			
			string query = "SELECT distinct(Category) FROM cashflows"; 
			printf("%s \n",query.c_str());
			mysql_query(connect,query.c_str());
    			res_set = mysql_store_result(connect); /* Receive the result and store it in res_set */
    			numrows = mysql_num_rows(res_set); /* Create the count to print all rows */
			printf("Number of categories: %d\n",numrows);
			

    			/*This while is to print all rows and not just the first row found, */
    			while ((row = mysql_fetch_row(res_set)) != NULL)
			{
    				unsigned int i = 0; /* Create a counter for the rows */
				Category = row[0];
				printf("%s\n",Category.c_str());
				a++;


				string months="select year(MyDate), month(MyDate) from cashflows group by month(MyDate), year(MyDate) order by year(MyDate), month(MyDate)";
				//printf("%s \n",months.c_str());
				mysql_query(connect,months.c_str());
    				res_set_months = mysql_store_result(connect); /* Receive the result and store it in res_set */
    				numrows = mysql_num_rows(res_set_months); /* Create the count to print all rows */
				//printf("Number of months: %d\n",numrows);

				while((row_months = mysql_fetch_row(res_set_months)) != NULL) {
					//printf("%s,%d,%d,%d\n",Date.c_str(),i,a,Debit);
					string Year = row_months[0];
					string Month = row_months[1];
					string instrument="INSERT INTO cashflows (MyDate, GroupLib, Libelle, Debit, Credit, Category, Weekly, Monthly, Yearly) VALUES ( '" + Year + "-" + Month + "-01'" + ",'" + Category + "', '" + Category + "', 0.0000001, 0.0000001,'" + Category + "',0,0,0)";
					//printf("Added: %s\n",instrument.c_str());
					mysql_query(connect,instrument.c_str());
					/*if(query_state!=0)
   					{
				      		cout<<mysql_error(conn)<<endl<<endl;
				      		return 1;
   					}*/
					i++;
				}
    			}

	}
	
	printf("On referme la connection...\n");
    	mysql_close(connect);   /* Close and shutdown */

    	return 0;
}
