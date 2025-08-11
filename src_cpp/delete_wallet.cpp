#include "../include_cpp/my.h"

std::string WalletManager::delete_wallet(std::string &to_delete)
{
    sqlite3 *db = WalletManager::init_db();
    wallet toDelete{};
    sqlite3_stmt *stmt1;
    sqlite3_stmt *stmt2;
    const char *deleteWSQL1 = "UPDATE wallets SET is_active = FALSE WHERE name = ?;";
    const char *deleteWSQL2 = "UPDATE transactions SET is_archived = TRUE WHERE wallet_name = ?;";

    glz::read_json(toDelete, to_delete);
    if (sqlite3_prepare_v2(db, deleteWSQL1, -1, &stmt1, nullptr) != SQLITE_OK) {
        std::cerr << "SQL prepare error: " << sqlite3_errmsg(db) << endl;
        WalletManager::closedb(db);
        return "Failed to delete wallet";
    }
    sqlite3_bind_text(stmt1, 1, toDelete.name.c_str(), -1, SQLITE_STATIC);
    if (sqlite3_step(stmt1) != SQLITE_DONE ) {
        std::cerr << "Failed to complete the query: " << sqlite3_errmsg(db) << endl;
        sqlite3_finalize(stmt1);
        WalletManager::closedb(db);
        return "Failed to delete successfully!!";
    }

    if (sqlite3_prepare_v2(db, deleteWSQL2, -1, &stmt2, nullptr) != SQLITE_OK) {
        std::cerr << "SQL prepare error: " << sqlite3_errmsg(db) << endl;
        WalletManager::closedb(db);
        return "Failed to delete wallet";
    }
    sqlite3_bind_text(stmt2, 1, toDelete.name.c_str(), -1, SQLITE_STATIC);
    if (sqlite3_step(stmt2) != SQLITE_DONE ) {
        std::cerr << "Failed to complete the query: " << sqlite3_errmsg(db) << endl;
        sqlite3_finalize(stmt2);
        WalletManager::closedb(db);
        return "Failed to delete successfully!!";
    }
    
    sqlite3_finalize(stmt1);
    sqlite3_finalize(stmt2);
    WalletManager::closedb(db);
    return "Wallet deleted successfully!";
}