// gcc odbc_example.c -o odbc_example -lodbc

#include <stdio.h>
#include <stdlib.h>

#include "odbc.h"

#define BUFFERSIZE 1024
#define NUMCOLS 5

// 실패시 -1 성공시 0

int odbc_init(db_info_t *p_db)
{
    int ret = 0;

    p_db->henv = SQL_NULL_HENV;
    p_db->hdbc = SQL_NULL_HDBC;
    p_db->hstmt = SQL_NULL_HSTMT;

    // 성공시 0 or 1
    ret = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &p_db->henv);
    printf("\n1 %d", ret);
    if (ret < 0)
    {
        p_db->conn = -1;
        return -1;
    }

    // 성공시 0 or 1
    ret = SQLSetEnvAttr(p_db->henv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER *)SQL_OV_ODBC3, 0);
    printf("\n2 %d", ret);
    if (ret < 0)
    {
        p_db->conn = -1;
        return -1;
    }

    // 성공시 0 or 1
    ret = SQLAllocHandle(SQL_HANDLE_DBC, p_db->henv, &p_db->hdbc);
    printf("\n3 %d", ret);
    if (ret < 0)
    {
        p_db->conn = -1;
        return -1;
    }

    // 연결 타임 아웃 설정, 성공시 0 or 1
    ret = SQLSetConnectAttr(p_db->hdbc, SQL_LOGIN_TIMEOUT, (SQLPOINTER)5, 0);
    printf("\n4 %d", ret);
    if (ret < 0)
    {
        p_db->conn = -1;
        return -1;
    }

    // SQL 서버 접속 성공시 0 or 1
    ret = odbc_connect(p_db);

    return ret;
}

// 실패시 -1 성공시 0
int odbc_connect(db_info_t *p_db)
{
    int ret = 0;

    // SQL 서버 접속 성공시 0 or 1
    ret = SQLConnect(p_db->hdbc, (SQLCHAR *)"ttt", SQL_NTS, (SQLCHAR *)"t", 6, "t", 6);
    printf("\n@@ %d", ret);
    if (ret < 0)
    {
        p_db->conn = -1;
        return -1;
    }
    
    return ret;
}

// 실패시 -1 성공시 열의 개수 출력
int odbc_query(db_info_t *p_db, char *sql_query)
{
    int ret = 0;
    
    // 핼들 할당
    ret = SQLAllocHandle(SQL_HANDLE_STMT, p_db->hdbc, &p_db->hstmt);
    printf("\n 1 %d", ret);
    if (ret < 0)
    {
        p_db->conn = -1;
        return -1;
    }

    // 쿼리문 실행
    ret = SQLExecDirect(p_db->hstmt, (SQLCHAR *)sql_query, SQL_NTS);
    printf("\n 2 %d", ret);
    if (ret < 0)
    {
        p_db->conn = -1;
        return -1;
    }

    // 쿼리문 행 개수 가져오기
    SQLNumResultCols(p_db->hstmt, &ret);

    return ret;
}

// 모든 접속 종료
void odbc_release(db_info_t *p_db)
{
    SQLFreeHandle(SQL_HANDLE_STMT, p_db->hstmt);
    SQLFreeHandle(SQL_HANDLE_ENV, p_db->henv);
    SQLDisconnect(p_db->hdbc);
}

// 예제 r
int test_read(db_info_t *p_db, int *a, float *b, char (*c)[10])
{
    SQLLEN t1 = 0;
    SQLREAL t2 = 0;
    SQLCHAR t3[30] = {
        0,
    };
    SQLFLOAT t4 = 0;
    SQLDATE t5[30] = {
        0,
    };

    int l1, l2, l3, l4, l5;
    int ret = 0;
    int count = 0;
    char query[1024];

    sprintf(query, "SELECT sum(t1), sum(t2) FROM dbo.sun");
    ret = odbc_query(p_db, query);
    
    printf("\nselect %d\n", ret);
    // SQLRowCount(p_db->hstmt, &a);
    // printf("\nselect2 %d\n", a);

    // SQL_C_CHAR   /* CHAR, VARCHAR, DECIMAL, NUMERIC */
    // SQL_C_LONG   /* INTEGER                      */
    // SQL_C_SHORT  /* SMALLINT                     */
    // SQL_C_FLOAT  /* REAL                         */
    // SQL_C_DOUBLE /* FLOAT, DOUBLE                */

    p_db->cnt=0;

    ret = SQLBindCol(p_db->hstmt, 1, SQL_C_SHORT, &t1, sizeof(t1), &l1);
    printf("\n s1 %d", ret);
    ret = SQLBindCol(p_db->hstmt, 2, SQL_C_FLOAT, &t2, sizeof(t1), &l2);
    printf("\n s2 %d", ret);
    // ret = SQLBindCol(p_db->hstmt, 3, SQL_C_CHAR, t3, sizeof(t3), &l3);
    // printf("\n s3 %d", ret);
    // ret = SQLBindCol(p_db->hstmt, 4, SQL_C_DOUBLE, &t4, sizeof(t4), &l4);
    // printf("\n s4 %d", ret);
    // ret = SQLBindCol(p_db->hstmt, 5, SQL_C_CHAR, t5, sizeof(t5), &l5);
    // printf("\n s5 %d", ret);

    for(;;)
    {
        ret = SQLFetch(p_db->hstmt);
        if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
        {
            count++;
            p_db->cnt++;
            *a = t1;
            *b = t2;
            // sprintf(c[count-1], "%s", t3);
            printf("\n[%d] %d, %f, %d \n", (count-1), t1, t2, a[count-1]);
        }
        else if (ret == SQL_NO_DATA)
        {
            break;
        }
        else if (p_db->conn < 0)
        {
            break;
        }
    }

    SQLFreeHandle(SQL_HANDLE_STMT, p_db->hstmt);

    printf("### %d", count);
    return count;
}

// 예제
int test_add(db_info_t *p_db)
{
    int ret = 0;

    char query[1024];

    sprintf(query, "INSERT INTO dbo.sun(t1, t2, t3, t4, t5) VALUES(50, 5.5, 'aaa', 4.3, '2019-10-20 11:11:11')");

    ret = odbc_query(p_db, query);
    printf("\ninsert %d\n", ret);

    SQLRowCount(p_db->hstmt, &ret);
    printf("\ninsert %d\n", ret);

    return ret;
}
