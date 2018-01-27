
#include <stdio.h>
#include <mysql.h>
#include <errmsg.h>
#include <address_db.h>


static BtchAddrDB* btch_addr_db = 0;

BtchAddrDB* BtchAddrDB::GetInstance()
{
    if(!btch_addr_db){
        btch_addr_db = new BtchAddrDB();
    }

    return btch_addr_db;
}

BtchAddrDB::BtchAddrDB():
_addConn(0), _enableConn(0), _disableConn(0), _ucAddConn(0)
{

	
}

bool BtchAddrDB::Init(const char* db, const char* table, const char* ucTable)
{
    _db = db;
    _table = table;
    _ucTable = ucTable;
    _addConn = Connect();
    _enableConn = Connect();
    _disableConn = Connect();
    _ucAddConn = Connect();

}


bool BtchAddrDB::ReConnect(void** conn)
{
    if(*conn != 0){
         MYSQL *c = (MYSQL*)(*conn);
         mysql_close(c);
    }

    *conn = 0;

    void* c = Connect();
    if(!c){
        return false;
    }

    *conn = c;
    return true;   
}

void* BtchAddrDB::Connect()
{
    MYSQL *conn;
    MYSQL_RES *res;
	MYSQL_ROW row;
	const char *server = "127.0.0.1";
	const char *user = "root";
	const char *password = "sd-9898w";  
	const char *database = _db;//"bitcoin";
	conn = mysql_init(NULL);


    if(!mysql_real_connect(conn, server, user, password, database, 0, NULL, 0))
	{
		fprintf(stderr, "%s\n", mysql_error(conn));
        return 0;
	}

    return conn;
}



bool BtchAddrDB::Execute(void** conn, const char* sql)
{
    if(*conn == 0){
        if(!ReConnect(conn)){
            return false;
        }
    }
    
    bool toBreak = false;
    while (!toBreak)
    {
        MYSQL* c = (MYSQL*)(*conn);
        if (mysql_query(c, sql))
        {
            int err = mysql_errno(c);
            switch (err)
            {
                case CR_SERVER_GONE_ERROR:
                case CR_SERVER_LOST:
                    {
                         if(ReConnect(conn)){
                             continue;
                         }   
                   }
            }

            fprintf(stderr, "%s\n", mysql_error(c));
            return false;
        }
        else{
            return true;
        }
    }

    return true;
}


long BtchAddrDB::Insert(void** conn, const char* sql)
{
    if(*conn == 0){
        if(!ReConnect(conn)){
            return false;
        }
    }
    
    bool toBreak = false;
    while (!toBreak)
    {
        MYSQL* c = (MYSQL*)(*conn);
        if (mysql_query(c, sql))
        {
            int err = mysql_errno(c);
            switch (err)
            {
                case CR_SERVER_GONE_ERROR:
                case CR_SERVER_LOST: 
                    {
                         if(ReConnect(conn)){
                             continue;
                         }   
                   }
            }

            fprintf(stderr, "%s\n", mysql_error(c));
            return 0;
        }
        else{
            return mysql_insert_id(c);
        }
    }

    return 0;
}

bool BtchAddrDB::AddAddress(int type, const char *ip, int port)
{
    char buffer[4096];
    const char* sql = "insert into %s(`type`, `ip`,`port`,`status`) values(%d, '%s', %d, 0) on duplicate \
                    key update `status`=0";

    sprintf(buffer, sql, _table, type, ip, port);
    return Execute( &_addConn, buffer);
}

bool BtchAddrDB::EnableAddress(int type, const char *ip, int port)
{
    char buffer[4096];
    const char* sql = "insert into %s(`type`, `ip`,`port`,`status`) values(%d, '%s', %d, 1) on duplicate \
                    key update `status`=1";

    sprintf(buffer, sql, _table, type, ip, port);
    return Execute( &_enableConn, buffer);

}

bool BtchAddrDB::DisableAddress(int type, const char *ip, int port)
{
    char buffer[4096];
    const char* sql = "insert into %s(`type`,`ip`,`port`,`status`) values(%d, '%s', %d, 1) on duplicate \
                    key update `status`=2";

    sprintf(buffer, sql, _table, type, ip, port);
    return Execute(&_disableConn ,buffer);

}

bool BtchAddrDB::AddUnCheckedAddress(int type, const char *ip, int port)
{
    char buffer[4096];
    const char* sql = "insert into %s(`type`, `ip`,`port`,`status`) values(%d, '%s', %d, 0) on duplicate \
                    key update `status`=0";

    sprintf(buffer, sql, _ucTable, type, ip, port);
    return Execute(&_ucAddConn ,buffer);
}


