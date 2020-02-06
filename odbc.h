#include <sql.h>
#include <sqlext.h>
#include <unistd.h>

// typedef struct data_binding_s
// {
//     SQLSMALLINT TargetType;
//     SQLPOINTER TargetValuePtr;
//     SQLINTEGER BufferLength;
//     SQLLEN StrLen_or_Ind;
// } data_binding_t;

typedef struct db_info_s
{
    char *server;
    char *user;
    char *password;
    char *database;

    SQLHENV henv;
    SQLHDBC hdbc;
    SQLHSTMT hstmt;

    int conn;
    int last_error;
    int cnt;

    // data_binding_t *data;
    // pthread_mutex_t mutex;
} db_info_t;

int odbc_init(db_info_t *p_db);
int odbc_connect(db_info_t *p_db);
int odbc_query(db_info_t *p_db, char *sql_query);
void odbc_release(db_info_t *p_db);

int test_read(db_info_t *p_db, int *a, float *b, char (*c)[10]);

