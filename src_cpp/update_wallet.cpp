#include "../include_cpp/my.h"

string WalletManager::update_wallet(string &to_update)
{
    sqlite3 *db = WalletManager::init_db();
    wallet to_edit{};
    sqlite3_stmt *stmt;
    char *updateSQL = "UPDATE INTO WALLETS"
                    "(currency, source, initial_amount,"
                    "balance ,color,created_at,updated_at)"
                    "VALUES(?,?,?,?,?,?,?);"
    ;

    glz::read_json(to_edit,to_update);

    if (sqlite3_prepare_v2(db, updateSQL,-1 , &stmt, nullptr)!=SQLITE_OK){
        cerr << "SQL prepare error:" << sqlite3_errmsg(db) <<endl;
        WalletManager::closedb(db);
        return "Failed to update wallet !!";
    }
    sqlite3_bind_text(stmt , 1,to_edit.currency.c_str() ,-1 ,SQLITE_STATIC);
    sqlite3_bind_text(stmt , 1,to_edit.source.c_str() ,-1 ,SQLITE_STATIC);
    sqlite3_bind_text(stmt , 1,to_edit.initial_amount);
    sqlite3_bind_text(stmt , 1,to_edit.balance);
    sqlite3_bind_text(stmt , 1,to_edit.color.c_str() ,-1 ,SQLITE_STATIC);
    sqlite3_bind_text(stmt , 1,to_edit.created_at.c_str() ,-1 ,SQLITE_STATIC);
    sqlite3_bind_text(stmt , 1,to_edit.updated_at.c_str() ,-1 ,SQLITE_STATIC);

    if (!(sqlite3_step(stmt) == SQLITE_DONE)){
        cerr << "SQL step error : " << sqlite3_errmsg(db) <<endl;
        sqlite3_finalize(stmt);
        WalletManager::closedb(db);
        return "Failed to update wallet !!";
    }
    sqlite3_finalize(stmt);
    WalletManager::close(db);
    return "Wallet updated succesfully!";
}

