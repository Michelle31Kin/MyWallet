#pragma once

#include "my.h"
#include "wallet.h"
#include "transaction.h"

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
        /*|||||_______________________________________________________[___UPDATE RECORD___]__________________________________________________|||||*/
        
                //THE NEGATIVE OF A STATEMENT WILL BE PRECEEDED BY '_' 
                    // toUpdate.wallet_name == previous_wallet_name ====> A && _A <=== toUpdate.wallet_name != previous_wallet_name
                    //           toUpdate.amount == previous_amount ====> B && _B <=== toUpdate.amount != previous_amount
                    //               toUpdate.type == previous_type ====> C && _C <=== toUpdate.type != previous_type
                //_____SAME WALLET
            static std::string ABC(sqlite3 *db, transaction &toUpdate);
            static std::string AB_C(sqlite3 *db, transaction &toUpdate);
            static std::string A_BC(sqlite3 *db, transaction &toUpdate, sqlite3_int64 previous_amount);
            static std::string A_B_C(sqlite3 *db, transaction &toUpdate, sqlite3_int64 previous_amount);
                //_____DIFFERENT WALLET
            static std::string _ABC(sqlite3 *db, transaction &toUpdate, std::string &previous_wallet_name);
            static std::string _AB_C(sqlite3 *db, transaction &toUpdate, std::string &previous_wallet_name);
            static std::string _A_BC(sqlite3 *db, transaction &toUpdate, std::string &previous_wallet_name, sqlite3_int64 previous_amount);
            static std::string _A_B_C(sqlite3 *db, transaction &toUpdate, std::string &previous_wallet_name, sqlite3_int64 previous_amount);

        /*|||||________________________________________________________[_____THE END_____]___________________________________________________|||||*/
        static sqlite3 *init_db(void);
        static void closedb(sqlite3 *db);
        static void init_tables(sqlite3 *db);
};