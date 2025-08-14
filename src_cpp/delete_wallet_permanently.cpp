#include "../include_cpp/my.h"

std::string WalletManager::delete_wallet_permanently(std::string &to_delete)
{
    sqlite3 *db = WalletManager::init_db();
    wallet toDelete{};
    sqlite3_stmt *deleteWalletStmt = nullptr;
    const char *deleteWalletSQL = "UPDATE wallets SET is_active = FALSE WHERE name = ?;";
    std::string result = "Failed to delete record!!";

    glz::read_json(toDelete, to_delete);
    if (sqlite3_prepare_v2(db, deleteWalletSQL, -1, &deleteWalletStmt, nullptr) != SQLITE_OK) {
        std::cerr << "SQL prepare error: " << sqlite3_errmsg(db) << endl;
        goto cleanup;
    }
    sqlite3_bind_text(deleteWalletStmt, 1, toDelete.name.c_str(), -1, SQLITE_STATIC);
    if (sqlite3_step(deleteWalletStmt) != SQLITE_DONE ) {
        std::cerr << "Failed to complete the query: " << sqlite3_errmsg(db) << endl;
        goto cleanup;
    }
    result = "Wallet deleted successfully";
    cleanup:
        if (deleteWalletStmt) sqlite3_finalize(deleteWalletStmt);
        WalletManager::closedb(db);
        return result;
}