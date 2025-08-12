#include "../include_cpp/my.h"

    /*----------------------------------------------------------------------------------------------------------------------------------*/
/*|||||                                             [___TO DELETE INCOME AND EXPENSE RECORDS___]                                       |||||*/
    /*----------------------------------------------------------------------------------------------------------------------------------*/

std::string WalletManager::delete_record(std::string &to_delete)
{
    /*|||||_______________________________________________________[___VARIABLE DECLARATION___]__________________________________________________|||||*/

        sqlite3 *db = WalletManager::init_db(); // <--- TO CONNECT TO THE DATABASE
        transaction toDelete{};
        sqlite3_stmt *deleteStatement;
        sqlite3_stmt *walletBalanceUpdateStatement;
        const char *deleteSQL = "DELETE FROM transactions WHERE id = ?;";
        const char *updateWalletSQL_forExpense = "UPDATE wallets SET balance = balance + ? WHERE name = ?";
        const char *updateWalletSQL_forIncome = "UPDATE wallets SET balance = balance - ? WHERE name = ?";

    /*|||||____________________________________________________________[_____THE END_____]______________________________________________________|||||*/

        glz::read_json(toDelete, to_delete); //<--- TO PARSE THE JSON INFORMATION INTO THE TRANSACTION STRUCT
        
         /*----------------------------------------------------------------------------------------------------------------------------------*/
    /*|||||                                                        [___BEGIN TRANSACTION___]                                                 |||||*/
         /*----------------------------------------------------------------------------------------------------------------------------------*/

        sqlite3_exec(db, "BEGIN;", nullptr, nullptr, nullptr); // <--- STARTING TRANSACTION      

    /*|||||_______________________________________________________[___PREPARING STATEMENTS___]__________________________________________________|||||*/

        if (sqlite3_prepare_v2(db, deleteSQL, -1, &deleteStatement, nullptr) != SQLITE_OK) {
            std::cerr << "SQL prepare failed: " << sqlite3_errmsg(db) << "!!" << endl;
            sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr); // <--- ROLLING BACK STATEMENT
            WalletManager::closedb(db);
            return "Failed to delete record";
        }
        if (toDelete.type == "INCOME") {
            if (sqlite3_prepare_v2(db, updateWalletSQL_forIncome, -1, &walletBalanceUpdateStatement, nullptr) != SQLITE_OK) {
                std::cerr << "SQL prepare failed: " << sqlite3_errmsg(db) << "!!" << endl;
                sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr); // <--- ROLLING BACK STATEMENT
                WalletManager::closedb(db);
                return "Failed to delete record";
            }
        } else if (toDelete.type == "EXPENSE") {
            if (sqlite3_prepare_v2(db, updateWalletSQL_forExpense, -1, &walletBalanceUpdateStatement, nullptr) != SQLITE_OK) {
                std::cerr << "SQL prepare failed: " << sqlite3_errmsg(db) << "!!" << endl;
                sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr); // <--- ROLLING BACK STATEMENT
                WalletManager::closedb(db);
                return "Failed to delete record";
            }
        }

    /*|||||__________________________________________________________[_____THE END_____]________________________________________________________|||||*/    


    /*|||||_________________________________________[___BINDING PARAMETERS TO THE STATEMENT OBJECTS___]_________________________________________|||||*/

        sqlite3_bind_int(deleteStatement, 1, toDelete.id);
        sqlite3_bind_int64(walletBalanceUpdateStatement, 1, toDelete.amount);
        sqlite3_bind_text(walletBalanceUpdateStatement, 2, toDelete.wallet_name.c_str(), -1, SQLITE_STATIC);

    /*|||||__________________________________________________________[_____THE END_____]________________________________________________________|||||*/


    /*|||||______________________________________________________[___EXECUTING STATEMENTS___]__________________________________________________|||||*/

        if (sqlite3_step(deleteStatement) != SQLITE_DONE) {
            std::cerr << "SQL step failed: " << sqlite3_errmsg(db) << "!!" << endl;
            sqlite3_finalize(deleteStatement);
            sqlite3_finalize(walletBalanceUpdateStatement);
            sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr); // <--- ROLLING BACK STATEMENT
            WalletManager::closedb(db);
            return "Failed to delete record";
        }
        if (sqlite3_step(walletBalanceUpdateStatement) != SQLITE_DONE) {
            std::cerr << "SQL step failed: " << sqlite3_errmsg(db) << "!!" << endl;
            sqlite3_finalize(deleteStatement);
            sqlite3_finalize(walletBalanceUpdateStatement);
            sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr); // <--- ROLLING BACK STATEMENT
            WalletManager::closedb(db);
            return "Failed to delete record";
        }

    /*|||||___________________________________________________________[_____THE END_____]______________________________________________________|||||*/

    
         /*----------------------------------------------------------------------------------------------------------------------------------*/
    /*|||||                                                        [___CLOSING TRANSACTION___]                                                 |||||*/
         /*----------------------------------------------------------------------------------------------------------------------------------*/
        
         sqlite3_exec(db, "COMMIT;", nullptr, nullptr, nullptr); //  <--- CLOSING TRANSACTION


    /*|||||_________________________________________[___CLEANING STATEMENTS___]________________________________________|||||*/

        sqlite3_finalize(deleteStatement); //<--- TO FREE THE VARIABLE, REQUIRED BY SQLITE3
        sqlite3_finalize(walletBalanceUpdateStatement);
        WalletManager::closedb(db); //<--- TO CLOSE THE CONNECTION TO THE DATABASE

    /*|||||_____________________________________________[_____THE END_____]_____________________________________________|||||*/
    return "Record deleted successfully!";
}