#include "../include_cpp/my.h"

std::string WalletManager::ABC(sqlite3 *db, transaction &toUpdate)
{
    sqlite3_stmt *updateRecordStmt;
    const char *updateRecordSQL = "UPDATE transactions SET "
                                "wallet_name = ?, type = ?, updated_at = ?, "
                                "category = ?, description = ?, amount = ? WHERE id = ?;"
    ;
    if (sqlite3_prepare_v2(db, updateRecordSQL, -1, &updateRecordStmt, nullptr) != SQLITE_OK) {
        std::cerr << "SQL prepare error: " << sqlite3_errmsg(db) << endl;
        WalletManager::closedb(db);
        return "Failed to update record";
    }
    sqlite3_bind_text(updateRecordStmt, 1, toUpdate.wallet_name.c_str(), -1, SQLITE_STATIC);

    return std::string();
}

std::string WalletManager::AB_C(sqlite3 *db, transaction &toUpdate)
{
    return std::string();
}

std::string WalletManager::A_BC(sqlite3 *db, transaction &toUpdate, sqlite3_int64 previous_amount)
{
    return std::string();
}

std::string WalletManager::A_B_C(sqlite3 *db, transaction &toUpdate, sqlite3_int64 previous_amount)
{
    return std::string();
}

std::string WalletManager::_ABC(sqlite3 *db, transaction &toUpdate, std::string &previous_wallet_name)
{
    return std::string();
}

std::string WalletManager::_AB_C(sqlite3 *db, transaction &toUpdate, std::string &previous_wallet_name)
{
    return std::string();
}

std::string WalletManager::_A_BC(sqlite3 *db, transaction &toUpdate, std::string &previous_wallet_name, sqlite3_int64 previous_amount)
{
    return std::string();
}

std::string WalletManager::_A_B_C(sqlite3 *db, transaction &toUpdate, std::string &previous_wallet_name, sqlite3_int64 previous_amount)
{
    return std::string();
}