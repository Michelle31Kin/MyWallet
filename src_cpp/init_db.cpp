#include "include_cpp/my.h"

static const char *walletsSQL = "CREATE TABLE IF NOT EXISTS wallets ("
                                      "name TEXT PRIMARY KEY,"   
                                      "currency TEXT,"
                                      "source TEXT,"
                                      "initial_amount INTEGER,"
                                      "balance INTEGER,"
                                      "color TEXT,"
                                      "created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,"
                                      "updated_at TIMESTAMP, "
                                      "is_active INTEGER DEFAULT TRUE);"
                                      ;
static const char *transactionSQL = "CREATE TABLE IF NOT EXISTS transactions ("
                                          "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                                          "wallet_name TEXT NOT NULL,"
                                          "type TEXT CHECK (type IN ('INCOME', 'EXPENSE', 'TRANSFER')),"
                                          "recorded_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,"
                                          "updated_at TIMESTAMP,"
                                          "category TEXT,"
                                          "description TEXT,"
                                          "amount INTEGER, "
                                          "is_archived INTEGER DEFAULT FALSE, "
                                          "related_wallet_id INTEGER,"
                                          "FOREIGN KEY (wallet_name) REFERENCES wallets(name));"
                                    ;

sqlite3 *WalletManager::init_db(void)
{
    namespace fs = std::filesystem;
    sqlite3 *db = nullptr;
    fs::path dbPath1;
    fs::path dbPath2;
    fs::path dbfullPath;
    char *appdata;
    char *home;
    char *errmsg;

    #if _WIN32
        appdata = std::getenv("APPDATA");
        dbPath1 = appdata ? fs::path(appdata) : fs::temp_directory_path();
    #else
        home = std::getenv("HOME");
        dbPath1 = home ? fs::path(home) / ".local" / "share" : fs::temp_directory_path();
    #endif

    dbPath2 = dbPath1 / "myWallet";
    fs::create_directories(dbPath2);
    dbfullPath = dbPath2 / "myWallet.sqlite";

    if (sqlite3_open(dbfullPath.string().c_str(), &db) != SQLITE_OK) {
        std::cerr << "Failed to open the database: " << sqlite3_errmsg(db) << "!!" << endl;
        WalletManager::closedb(db);
        throw std::runtime_error("Failed to open the database");
    }

    WalletManager::init_tables(db);    
    return db;
}

void WalletManager::init_tables(sqlite3 *db)
{
    char *errmsg;
    if (sqlite3_exec(db, walletsSQL, 0, 0, &errmsg) != SQLITE_OK) {
        std::cerr << "Failed to create table: " << errmsg << " !!" << endl;
        sqlite3_free(errmsg);
        WalletManager::closedb(db);
        throw std::runtime_error("Failed to create table!");
    }
    if (sqlite3_exec(db, transactionSQL, 0, 0, &errmsg) != SQLITE_OK) {
        std::cerr << "Failed to create table: " << errmsg << " !!" << endl;
        sqlite3_free(errmsg);
        WalletManager::closedb(db);
        throw std::runtime_error("Failed to create table!");
    }
}

void WalletManager::closedb(sqlite3 *db)
{
    if (db != nullptr) {
        sqlite3_close(db);
    }
}