#pragma once

#include "my.h"

class WalletManager {
    WalletManager();

    public:
        std::string record_transaction(std::string to_record);

    private:
        static sqlite3 *init_db(void);
        static void closedb(sqlite3 *db);
        static void init_tables(sqlite3 *db);
};