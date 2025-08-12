#pragma once

#include "my.h"

class WalletManager {
    WalletManager();

    public:
        static std::string record_transaction(std::string &to_record);
        static std::string record_transfer(std::string &to_record);
        static std::string create_wallet(std::string &to_create);
        static std::string update_record(std::string &to_update);
        static std::string update_transfer(std::string &to_update);
        static std::string update_wallet(std::string &to_update);
        static std::string get_wallets(std::optional<std::string> &criteria);
        static std::string get_records(std::optional<std::string> &criteria);
        static std::string delete_record(std::string &to_delete);
        static std::string delete_wallet(std::string &to_delete);
    private:
        static void prepareStmt(sqlite3 *db, const char *Query, sqlite3_stmt *stmt);
        static sqlite3 *init_db(void);
        static void closedb(sqlite3 *db);
        static void init_tables(sqlite3 *db);
};