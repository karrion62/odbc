// gcc odbc_example.c -o odbc_example -lodbc

#include <stdio.h>
#include <stdlib.h>
#include <sql.h>
#include <sqlext.h>
#include <unistd.h>


int main () {
	SQLHENV   henv  = SQL_NULL_HENV;  
	SQLHDBC   hdbc  = SQL_NULL_HDBC;   
	SQLHSTMT  hstmt = SQL_NULL_HSTMT; 
	SQLRETURN retcode;

	SQLCHAR strName[20];
	SQLLEN strName_len=0;
	SQLLEN nNum=0, nNum_len=0;
	SQLLEN nVal=0, nVal_len=0;
	SQLINTEGER IDate;

	SQLLEN sl_id   = 0;
	SQLLEN item_no = 0;
	SQLCHAR update_date[30] = { 0, };
	SQLREAL solarvoltage = 0;
	SQLREAL solarcurrent = 0;
	SQLREAL solarkw = 0;
	SQLREAL kw      = 0;
	//SQLLEN kwh     = 0;
	SQLCHAR kwh[30] = { 0, };
	SQLREAL todaykwh = 0;
	SQLREAL todaykwh2 = 0;

	int i=0;
    int conn=0;
    int k=0;

	retcode = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &henv);
	retcode = SQLSetEnvAttr(henv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER*)SQL_OV_ODBC3, 0);
	retcode = SQLAllocHandle(SQL_HANDLE_DBC, henv, &hdbc);

	SQLSetConnectAttr(hdbc, SQL_LOGIN_TIMEOUT, (SQLPOINTER)3, 0);
	conn = SQLConnect(hdbc, (SQLCHAR*) "ttt", SQL_NTS, (SQLCHAR*) "t", 6, "t", 6);
    printf("connect %d \n", retcode);

    while(1){
        retcode = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);
        printf("\n 1 %d", retcode);
        if(retcode < 0) 
        {
            conn = SQLConnect(hdbc, (SQLCHAR *)"ttt", SQL_NTS, (SQLCHAR *)"t", 6, "t", 6);
            continue;
        }
        retcode = SQLExecDirect(hstmt, (SQLCHAR *)"SELECT * FROM dbo.pcs", SQL_NTS);
        if (retcode < 0)
        {
            conn = SQLConnect(hdbc, (SQLCHAR *)"ttt", SQL_NTS, (SQLCHAR *)"t", 6, "t", 6);
            continue;
        }
        retcode = SQLNumResultCols(hstmt, &k);

        printf("\n 2 %d, %d", retcode, k);
        retcode = SQLBindCol(hstmt, 1, SQL_C_USHORT, &sl_id,   2, &nNum_len);
        printf("\n 3 %d", retcode);
        retcode = SQLBindCol(hstmt, 2, SQL_C_FLOAT, &solarvoltage, 4, &nNum_len);
        printf("\n 4 %d", retcode);
        retcode = SQLBindCol(hstmt, 3, SQL_C_CHAR, update_date, sizeof(update_date), &IDate);

        for (i=0; ;i++) {
            retcode = SQLFetch(hstmt);
            if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
                printf("[%02d] %i, %i, %s, %6.1f, %6.1f, %6.1f, %6.1f, %s, %6.1f, %6.1f \n", i, (int) sl_id, item_no, update_date
                                    , solarvoltage, solarcurrent, solarkw
                                , kw, kwh
                                , todaykwh, todaykwh2);
            } else if (retcode == SQL_NO_DATA) {
                break;
            } else if(conn < 0) break;
            sleep(1);
        }
        SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
        SQLDisconnect(hdbc);
    }
	return 0;
}
