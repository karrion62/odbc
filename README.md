참고 URL : https://docs.microsoft.com/ko-kr/sql/connect/odbc/linux-mac/installing-the-microsoft-odbc-driver-for-sql-server?view=sql-server-ver15
https://idlecomputer.tistory.com/190


$ sudo su
# curl https://packages.microsoft.com/keys/microsoft.asc | apt-key add -
# curl https://packages.microsoft.com/config/ubuntu/18.04/prod.list > tee /etc/apt/sources.list.d/mssql-release.list
# apt-get update
# apt-get install mssql-tools

$ sudo apt-get install freetds-dev
$ sudo apt-get install tdsodbc

#### sudo ACCEPT_EULA=Y apt-get install msodbcsql17

### 설치확인
$ odbcinst -q -d -n "ODBC Driver 17 for SQL Server"
[ODBC Driver 17 for SQL Server]
Description=Microsoft ODBC Driver 17 for SQL Server
Driver=/opt/microsoft/msodbcsql17/lib64/libmsodbcsql-17.4.so.2.1
UsageCount=1

$ cat /etc/odbcinst.ini


$ odbcinst -j
unixODBC 2.3.7
DRIVERS............: /etc/odbcinst.ini
SYSTEM DATA SOURCES: /etc/odbc.ini
FILE DATA SOURCES..: /etc/ODBCDataSources
USER DATA SOURCES..: /home/plus/.odbc.ini
SQLULEN Size.......: 8
SQLLEN Size........: 8
SQLSETPOSIROW Size.: 8


$ sudo vi /etc/odbc.ini
[DB2016]
driver=SQL Server
server=121.185.138.99
port=20041
database=_dy_solar_ess
client_charset = UTF-8
tds_version = 8.0


### 제거 방법
# rm -f /usr/bin/sqlcmd
# rm -f /usr/bin/bcp
# rm -rf /opt/microsoft/msodbcsql
# odbcinst -u -d -n "ODBC Driver 17 for SQL Server"



$ /opt/mssql-tools/bin/sqlcmd -S 121.185.138.99,20041 -U pmgrow -P pmgrow
1> select @@version
2> go

------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Microsoft SQL Server 2014 - 12.0.2269.0 (X64)
	Jun 10 2015 03:35:45
	Copyright (c) Microsoft Corporation
	Express Edition (64-bit) on Windows NT 6.3 <X64> (Build 18362: )


(1 rows affected)
1> use _dy_solar_ess
2> go
데이터베이스 컨텍스트가 '_dy_solar_ess'(으)로 변경되었습니다.
1>
1>
1>
1> select * from tr_bms
2> go

1> select * from tr_inverter;
2> go

$ sudo apt-get install unixodbc-dev
