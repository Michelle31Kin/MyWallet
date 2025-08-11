#include "include_cpp/my.h"

std::string WalletManager::record_transaction(std::string &to_record)
{
    sqlite3 *db = WalletManager::init_db();
    transaction to_insert{};
    sqlite3_stmt *stmt;
    const char *createSQL = "INSERT INTO transactions "
                    "(wallet_name, type, recorded_at"
                    "updated_at, category, description"
                    "amount, related_wallet_id) "
                    "VALUES (?, ?, ?, ?, ?, ?, ?, ?);"
    ;

    glz::read_json(to_insert, to_record);
    if (sqlite3_prepare_v2(db, createSQL, -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "SQL prepare error: " << sqlite3_errmsg(db) << endl;
        WalletManager::closedb(db);
        return "Failed to record transaction!!";
    }
    sqlite3_bind_text(stmt, 1, to_insert.wallet_name.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, to_insert.type.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, to_insert.recorded_at.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, to_insert.updated_at.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 5, to_insert.category.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 6, to_insert.description.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 7, to_insert.amount);
    sqlite3_bind_int(stmt, 8, to_insert.related_wallet_id);
    if (!(sqlite3_step(stmt) == SQLITE_DONE)) {
        std::cerr << "SQL step error: " << sqlite3_errmsg(db) << endl;
        sqlite3_finalize(stmt);
        WalletManager::closedb(db);
        return "Failed to record transaction!!";
    }
    sqlite3_finalize(stmt);
    WalletManager::closedb(db);
    return "Transaction recorded successfully!";
}