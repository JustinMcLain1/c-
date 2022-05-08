#include <iostream>
#include <string>
#include <mysql.h>

using namespace std;

int main()
{

    char Input;
    cout << "A for Author Select, B for Book List, T for Title Select";
    cin >> Input;

    mysql_library_init(0, NULL, NULL);
    if (mysql_library_init(0, NULL, NULL))
    {
        fprintf(stderr, "could not initialize MySQL client library\n");

        exit(1);
    }

    MYSQL_RES *result;
    MYSQL_ROW row;
    MYSQL *mysql = new MYSQL;
    mysql_init(mysql);

    if (Input == 'B')
    {
        if (!mysql_real_connect(mysql, "courses", "z1910087", "1999Oct08", "henrybooks", 0, NULL, 0)) //if connect do these
        {
            fprintf(stderr, "Failed to connect to database: Error: %s\n",
                    mysql_error(mysql));
            exit(1);
        }
        else
        {

            if (mysql_query(mysql, "SELECT * FROM Book")) //run the statement
            {
                fprintf(stderr, "Failed to connect query: Error: %s\n",
                        mysql_error(mysql));
                exit(1);
            }
            else
            {
                result = mysql_store_result(mysql); //store results from query
                if (result == NULL)
                {
                    fprintf(stderr, "Failed to store: Error: %s\n",
                            mysql_error(mysql));
                    exit(1);
                }
                else
                {
                    cout << "BookCode"  //formatting
                             << " Title                         "
                             << " Author           "
                             << "Price" << endl;

                        cout << "========"
                             << " =============================="
                             << " ================="
                             << " =======" << endl;
                    while ((row = mysql_fetch_row(result))) //fetch results from store
                    {

                        cout << row[0] << "\t " << row[1] << "\t" << row[2] << "\t" << row[3] << "\t\t" << "$" <<row[4] << endl;
                    }
                }
            }
        }

        mysql_free_result(result);

        mysql_close(mysql);

        return 0;
    }
    //--------------------------------------------------------------------------
    if (Input == 'A')
    {
        string Author;
        cout << "Which Author? ";  // obtain author
        cin >> Author;
        if (!mysql_real_connect(mysql, "courses", "z1910087", "1999Oct08", "henrybooks", 0, NULL, 0)) //if connect do these
        {
            fprintf(stderr, "Failed to connect to database: Error: %s\n", //error checking
                    mysql_error(mysql));
            exit(1);
        }
        else
        {

            if (mysql_query(mysql, "SELECT BookCode, Title, AuthorFirst,AuthorLast,Price FROM Book,Author;")) //run the statement
            {
                fprintf(stderr, "Failed to connect query: Error: %s\n",
                        mysql_error(mysql));
                exit(1);
            }
            else
            {
                result = mysql_store_result(mysql);
                if (result == NULL)
                {
                    fprintf(stderr, "Failed to store: Error: %s\n",
                            mysql_error(mysql));
                    exit(1);
                }
                else
                {


                    cout << "BookCode"
                            << " Title                         "
                            << " Author           "
                            << "Price" << endl;

                        cout << "========"
                            << " =============================="
                            << " ================="
                            << " =======" << endl;
                    while ((row = mysql_fetch_row(result)))
                    {
                        
                        cout << row[0] << "\t " << row[1] << "\t" << row[2] << "\t" << row[3] << "\t\t" << "$" <<row[4] << endl;
                    }
                }
            }
        }

        mysql_free_result(result);

        mysql_close(mysql);

        return 0;
    }
    //------------------------------------------------------------------------------------------------------------------------------
    if (Input == 'T')
    {
        string Title2;
        cout << "Which Title?  ";  // obtain title
        cin >> Title2;
        if (!mysql_real_connect(mysql, "courses", "z1910087", "1999Oct08", "henrybooks", 0, NULL, 0)) //if connect do these
        {
            fprintf(stderr, "Failed to connect to database: Error: %s\n",
                    mysql_error(mysql)); //error checking
            exit(1);
        }
        else
        {

            if (mysql_query(mysql, "SELECT * FROM Book;")) //run the statement
            {
                fprintf(stderr, "Failed to connect query: Error: %s\n",
                        mysql_error(mysql)); //error checking
                exit(1);
            }
            else
            {
                result = mysql_store_result(mysql); //store the results from the query
                if (result == NULL) //check if the results are empty
                {
                    fprintf(stderr, "Failed to store: Error: %s\n",
                            mysql_error(mysql)); //error checking
                    exit(1);
                }
                else
                {
                        cout << "BookCode"  //formatting
                             << " Title                         "
                             << " Author           "
                             << "Price" << endl;

                        cout << "========"
                             << " =============================="
                             << " ================="
                             << " =======" << endl;

                    while ((row = mysql_fetch_row(result))) // fetch the results from store
                    {

                        cout << row[0] << "\t " << row[1] << "\t" << row[2] << "\t" << row[3] << "\t\t" << "$" <<row[4] << endl;

                        printf("\n");
                    }
                }
            }
        }

        mysql_free_result(result);

        mysql_close(mysql);

        return 0;
    }

}


