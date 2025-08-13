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
    sqlite3_int64 previous_amount;
    const unsigned char* previous_type_from_SQL;
    std::string previous_type;
    const unsigned char* previous_wallet_name_from_SQL;
    std::string previous_wallet_name;
    sqlite3_stmt *get_previous_amount_of_transaction_to_be_updated_Stmt;
    sqlite3_stmt *get_previous_type_of_transaction_to_be_updated_Stmt;
    sqlite3_stmt *get_previous_wallet_name_of_transaction_to_be_updated_Stmt;
    const char *get_previous_amount_of_transaction_to_be_updated_SQL = "SELECT amount FROM transactions WHERE id = ?;";
    const char *get_previous_type_of_transaction_to_be_updated_SQL = "SELECT type FROM transactions where id = ?;";
    const char *get_previous_wallet_name_of_transaction_to_be_updated_SQL = "SELECT wallet_name FROM transactions WHERE id = ?;";

    glz::read_json(toUpdate, to_update);
    sqlite3_exec(db, "BEGIN;", nullptr, nullptr, nullptr);
    if (sqlite3_prepare_v2(db, get_previous_amount_of_transaction_to_be_updated_SQL, -1, &get_previous_amount_of_transaction_to_be_updated_Stmt, nullptr) != SQLITE_OK) {
        std::cerr << "SQL prepare error: " << sqlite3_errmsg(db) << endl;
        WalletManager::closedb(db);
        return "Failed to update record";
    }
    if (sqlite3_prepare_v2(db, get_previous_type_of_transaction_to_be_updated_SQL, -1, &get_previous_type_of_transaction_to_be_updated_Stmt, nullptr) != SQLITE_OK) {
        std::cerr << "SQL prepare error: " << sqlite3_errmsg(db) << endl;
        WalletManager::closedb(db);
        return "Failed to update record";
    }
    if (sqlite3_prepare_v2(db, get_previous_wallet_name_of_transaction_to_be_updated_SQL, -1, &get_previous_wallet_name_of_transaction_to_be_updated_Stmt, nullptr) != SQLITE_OK) {
        std::cerr << "SQL prepare error: " << sqlite3_errmsg(db) << endl;
        WalletManager::closedb(db);
        return "Failed to update record";
    }
    sqlite3_bind_int(get_previous_amount_of_transaction_to_be_updated_Stmt, 1, toUpdate.id);
    sqlite3_bind_int(get_previous_type_of_transaction_to_be_updated_Stmt, 1, toUpdate.id);
    sqlite3_bind_int(get_previous_wallet_name_of_transaction_to_be_updated_Stmt, 1, toUpdate.id);
    if (sqlite3_step(get_previous_amount_of_transaction_to_be_updated_Stmt) == SQLITE_ROW) {
        sqlite3_int64 amount = sqlite3_column_int64(get_previous_amount_of_transaction_to_be_updated_Stmt, 0);
    } else {
        std::cerr << "No transaction found with that ID.\n";
    }
    if (sqlite3_step(get_previous_type_of_transaction_to_be_updated_Stmt) == SQLITE_ROW) {
        previous_type_from_SQL = sqlite3_column_text(get_previous_type_of_transaction_to_be_updated_Stmt, 0);
        previous_type = reinterpret_cast<const char *>(previous_type_from_SQL);
    } else {
        std::cerr << "No type found with that ID.\n";
    }
    if (sqlite3_step(get_previous_wallet_name_of_transaction_to_be_updated_Stmt) == SQLITE_ROW) {
        previous_wallet_name_from_SQL = sqlite3_column_text(get_previous_wallet_name_of_transaction_to_be_updated_Stmt, 0);
        previous_wallet_name = reinterpret_cast<const char *>(previous_wallet_name_from_SQL);
    }
    

    //THE NEGATIVE OF A STATEMENT WILL BE PRECEEDED BY '_' 
    // toUpdate.wallet_name == previous_wallet_name ====> A && _A <=== toUpdate.wallet_name != previous_wallet_name
    //           toUpdate.amount == previous_amount ====> B && _B <=== toUpdate.amount != previous_amount
    //               toUpdate.type == previous_type ====> C && _C <=== toUpdate.type != previous_type

    if ((toUpdate.wallet_name == previous_wallet_name) && (toUpdate.amount == previous_amount) && (toUpdate.type == previous_type)) return WalletManager::ABC(db, toUpdate);                                          //CASE 1 ABC
    if ((toUpdate.wallet_name == previous_wallet_name) && (toUpdate.amount != previous_amount) && (toUpdate.type == previous_type)) return WalletManager::A_BC(db, toUpdate, previous_amount);                        //CASE 2 A_BC
    if ((toUpdate.wallet_name == previous_wallet_name) && (toUpdate.amount == previous_amount) && (toUpdate.type != previous_type)) return WalletManager::AB_C(db, toUpdate);                                         //CASE 3 AB_C
    if ((toUpdate.wallet_name == previous_wallet_name) && (toUpdate.amount != previous_amount) && (toUpdate.type != previous_type)) return WalletManager::A_B_C(db, toUpdate, previous_amount);                       //CASE 4 A_B_C

    if ((toUpdate.wallet_name != previous_wallet_name) && (toUpdate.amount == previous_amount) && (toUpdate.type == previous_type)) return WalletManager::_ABC(db, toUpdate, previous_wallet_name);                    //CASE 5 _ABC
    if ((toUpdate.wallet_name != previous_wallet_name) && (toUpdate.amount != previous_amount) && (toUpdate.type == previous_type)) return WalletManager::_A_BC(db, toUpdate, previous_wallet_name, previous_amount);  //CASE 6 _A_BC
    if ((toUpdate.wallet_name != previous_wallet_name) && (toUpdate.amount == previous_amount) && (toUpdate.type != previous_type)) return WalletManager::_AB_C(db, toUpdate, previous_wallet_name);                   //CASE 7 _AB_C
    if ((toUpdate.wallet_name != previous_wallet_name) && (toUpdate.amount != previous_amount) && (toUpdate.type != previous_type)) return WalletManager::_A_B_C(db, toUpdate, previous_wallet_name, previous_amount); //CASE 8 _A_B_C
}