#include <stdio.h>
#include "odbc.h"

db_info_t db;

int main(int argc, char **argv)
{
    int len, i;
    SQLLEN a;
    SQLREAL b;
    SQLCHAR c[10][10];

    db.server = "ttt";
    db.user = "pmgrow";
    db.password = "pmgrow";

    odbc_init(&db);

    printf("\n1 %d\n", db.conn);

    len = test_read(&db, &a, &b, c);

    printf("\n!!!%d\n", db.cnt);
    for(i=0; i<len; i++){
        printf("\n ## %d %d %lf ##", i, a, b );
    }

    // test_add(&db);


    return 0;
}