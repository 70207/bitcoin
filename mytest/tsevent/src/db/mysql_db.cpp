
#include <stdio.h>
#include <mysql.h>
#include <errmsg.h>
#include <mysql_db.h>


static BtchDB* g_btch_db = 0;

BtchDB* BtchDB::GetInstance()
{
    if(!g_btch_db){
        g_btch_db = new BtchDB();
    }

    return g_btch_db;
}

BtchDB::BtchDB():
_addConn(0), _enableConn(0), _disableConn(0), _ucAddConn(0)
{

	
}

bool BtchDB::Init(const char* db, const char* table, const char* ucTable)
{
    _db = db;
    _table = table;
    _ucTable = ucTable;
    _addConn = Connect();
    _enableConn = Connect();
    _disableConn = Connect();
    _ucAddConn = Connect();

}


bool BtchDB::ReConnect(void** conn)
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

void* BtchDB::Connect()
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



bool BtchDB::Execute(void** conn, const char* sql)
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

bool BtchDB::AddAddress(int type, const char *ip, int port)
{
    char buffer[4096];
    const char* sql = "insert into %s(`type`, `ip`,`port`,`status`) values(%d, '%s', %d, 0) on duplicate \
                    key update `status`=0";

    sprintf(buffer, sql, _table, type, ip, port);
    return Execute( &_addConn, buffer);
}

bool BtchDB::EnableAddress(int type, const char *ip, int port)
{
    char buffer[4096];
    const char* sql = "insert into %s(`type`, `ip`,`port`,`status`) values(%d, '%s', %d, 1) on duplicate \
                    key update `status`=1";

    sprintf(buffer, sql, _table, type, ip, port);
    return Execute( &_enableConn, buffer);

}

bool BtchDB::DisableAddress(int type, const char *ip, int port)
{
    char buffer[4096];
    const char* sql = "insert into %s(`type`,`ip`,`port`,`status`) values(%d, '%s', %d, 1) on duplicate \
                    key update `status`=2";

    sprintf(buffer, sql, _table, type, ip, port);
    return Execute(&_disableConn ,buffer);

}

bool BtchDB::AddUnCheckedAddress(int type, const char *ip, int port)
{
    char buffer[4096];
    const char* sql = "insert into %s(`type`, `ip`,`port`,`status`) values(%d, '%s', %d, 0) on duplicate \
                    key update `status`=0";

    sprintf(buffer, sql, _ucTable, type, ip, port);
    return Execute(&_ucAddConn ,buffer);
}



bool BtchDB::AddBlock(BtchBlock* block)
{
    char buffer[4096];
    const char* sql = "insert into %s("

}

bool BtchDB::AddTransaction(BtchTransaction* tx)
{

}