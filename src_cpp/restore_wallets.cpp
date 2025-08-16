#include "../include.cpp/my.h"

std::string WalletManager::restore_wallet(std::string &to_restore)
{
    sqlite3 *db = WalletManager::init_db();
    sqlite3_stmt *restoreStmt = nullptr;
    std::string result = "Failed to restore wallet!";
    

    const char* restoreWalletSQL = "UPDATE wallets SET is_deleted = 0 WHERE name = ?;";
    
    if (sqlite3_prepare_v2(db, restoreWalletSQL, -1, &restoreStmt, nullptr) != SQLITE_OK) {
        std::cerr << "SQL prepare error: " << sqlite3_errmsg(db) << std::endl;
        goto cleanup;
    }
    
    sqlite3_bind_text(restoreStmt, 1, to_restore.c_str(), -1, SQLITE_STATIC);
    
    if (sqlite3_step(restoreStmt) != SQLITE_DONE) {
        std::cerr << "Failed to complete the query: " << sqlite3_errmsg(db) << std::endl;
        goto cleanup;
    }
    
    result = "Wallet restored successfully";
    
cleanup:
    if (restoreStmt) sqlite3_finalize(restoreStmt);
    WalletManager::closedb(db);
    return result;
}
