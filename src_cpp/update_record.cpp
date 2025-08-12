#include "../include_cpp/my.h"

void WalletManager::prepareStmt(sqlite3 *db, const char *Query, sqlite3_stmt *stmt)
{
    if (sqlite3_prepare_v2(db, Query, -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "SQL prepare error: " << sqlite3_errmsg(db) << endl;
        WalletManager::closedb(db);
        throw std::runtime_error("Failed to record transaction!!");
    }
}


std::string WalletManager::update_record(std::string &to_update)
{
    sqlite3 *db = WalletManager::init_db();
    transaction toUpdate{};
    sqlite3_stmt *updateRecordStmt;
    sqlite3_stmt *updatePrevWalletBalanceStmt;
    sqlite3_stmt *updateNewWalletBalanceStmt;
    const char *updateRecordSQL = "UPDATE transactions SET "
                                "wallet_name = ?, type = ?, updated_at = ?, "
                                "category = ?, description = ?, amount = ? WHERE id = ?;"
    ;
    //------------------------------NO CHANGE IN WALLETS---------------------------------------
    const char *updateNoWalletBalanceSQL_expense = "UPDATE wallets SET balance = balance + ? - ? WHERE name = ?;";
    const char *updateNoWalletBalanceSQL_income = "UPDATE wallets SET balance = balance - ? + ? WHERE name = ?;";
    const char *retrievePreviousTransactionAmountFromDBSQL = "SELECT amount FROM transactions WHERE id = ?";
    const char *retrieveWalletNameFromDBSQL = "SELECT amount FROM transactions WHERE id = ?";
    //-----------------------------THE TARGETED WALLET HAS BEEN CHANGED------------------------
    const char *updateYesNewWalletBalanceSQL_expense = "UPDATE wallets SET balance = balance - ? WHERE name = ?;";
    const char *updateYesNewWalletBalanceSQL_income = "UPDATE wallets SET balance = balance + ? WHERE name = ?;";
    const char *updateYesPreviousWalletBalanceSQL_expense = "UPDATE wallets SET balance = balance + ? WHERE name = ?;";
    const char *updateYesPreviousWalletBalanceSQL_income = "UPDATE wallets SET balance = balance - ? WHERE name = ?;";

    glz::read_json(toUpdate, to_update);
    if (sqlite3_prepare_v2(db, updateRecordSQL, -1, &updateRecordStmt, nullptr) != SQLITE_OK) {
        std::cerr << "SQL prepare error: " << sqlite3_errmsg(db) << endl;
        WalletManager::closedb(db);
        return "Failed to update transaction!!";
    }
    if (sqlite3_prepare_v2(db, updateNoWalletBalanceSQL_expense, -1, &updatePrevWalletBalanceStmt, nullptr) != SQLITE_OK) {
        std::cerr << "SQL prepare error: " << sqlite3_errmsg(db) << endl;
        WalletManager::closedb(db);
        return "Failed to update transaction!!";
    }
    if (sqlite3_prepare_v2(db, updateRecordSQL, -1, &updateRecordStmt, nullptr) != SQLITE_OK) {
        std::cerr << "SQL prepare error: " << sqlite3_errmsg(db) << endl;
        WalletManager::closedb(db);
        return "Failed to update transaction!!";
    }
    
}