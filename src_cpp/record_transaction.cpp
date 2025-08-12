#include "../include_cpp/my.h"

std::string WalletManager::record_transaction(std::string &to_record)
{
    sqlite3 *db = WalletManager::init_db();
    transaction to_insert{};
    sqlite3_stmt *insertTransactionStmt;
    sqlite3_stmt *updateWalletStmt;
    const char *createSQL = "INSERT INTO transactions "
                    "(wallet_name, type, recorded_at, "
                    "updated_at, category, description, "
                    "amount, related_wallet_id) "
                    "VALUES (?, ?, ?, ?, ?, ?, ?, ?);"
    ;
    const char *updateWalletSQL_income = "UPDATE wallets SET balance = balance + ?  WHERE name = ?;";
    const char *updateWalletSQL_expense = "UPDATE wallets SET balance = balance - ? WHERE name = ?;";

    glz::read_json(to_insert, to_record);
    sqlite3_exec(db, "BEGIN;", nullptr, nullptr, nullptr); // <=== START TRANSACTION
    if (sqlite3_prepare_v2(db, createSQL, -1, &insertTransactionStmt, nullptr) != SQLITE_OK) {
        std::cerr << "SQL prepare error: " << sqlite3_errmsg(db) << endl;
        WalletManager::closedb(db);
        return "Failed to record transaction!!";
    }
    if (to_insert.type == "INCOME") {
        if (sqlite3_prepare_v2(db, updateWalletSQL_income, -1, &updateWalletStmt, nullptr) != SQLITE_OK) {
            std::cerr << "SQL prepare error: " << sqlite3_errmsg(db) << endl;
            WalletManager::closedb(db);
            return "Failed to record transaction!!";
        }
    } else {
        if (sqlite3_prepare_v2(db, updateWalletSQL_expense, -1, &updateWalletStmt, nullptr) != SQLITE_OK) {
            std::cerr << "SQL prepare error: " << sqlite3_errmsg(db) << endl;
            WalletManager::closedb(db);
            return "Failed to record transaction!!";
        }
    }
    
    sqlite3_bind_text(insertTransactionStmt, 1, to_insert.wallet_name.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(insertTransactionStmt, 2, to_insert.type.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(insertTransactionStmt, 3, to_insert.recorded_at.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(insertTransactionStmt, 4, to_insert.updated_at.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(insertTransactionStmt, 5, to_insert.category.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(insertTransactionStmt, 6, to_insert.description.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int64(insertTransactionStmt, 7, to_insert.amount);
    sqlite3_bind_int(insertTransactionStmt, 8, to_insert.related_wallet_id);
    //----------------------------------------------------------------------
    sqlite3_bind_int64(updateWalletStmt, 1, to_insert.amount);
    sqlite3_bind_text(updateWalletStmt, 2, to_insert.wallet_name.c_str(), -1, SQLITE_STATIC);

    if (!(sqlite3_step(insertTransactionStmt) == SQLITE_DONE)) {
        std::cerr << "SQL step error: " << sqlite3_errmsg(db) << endl;
        sqlite3_finalize(insertTransactionStmt);
        WalletManager::closedb(db);
        return "Failed to record transaction!!";
    }
    if (!(sqlite3_step(updateWalletStmt) == SQLITE_DONE)) {
        std::cerr << "SQL step error: " << sqlite3_errmsg(db) << endl;
        sqlite3_finalize(insertTransactionStmt);
        WalletManager::closedb(db);
        return "Failed to record transaction!!";
    }
    
    sqlite3_exec(db, "COMMIT;", nullptr, nullptr, nullptr); // <=== CLOSE TRANSACTION
    sqlite3_finalize(insertTransactionStmt);
    sqlite3_finalize(updateWalletStmt);
    WalletManager::closedb(db);
    return "Transaction recorded successfully!";
}