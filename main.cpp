#include <saucer/smartview.hpp>
#include "include_cpp/my.h"

using std::string;

int main()
{
    auto app = saucer::application::init({      // Instantiate application
        .id = "myWallet",                    // Choose a suitable ID for your program
    });

    saucer::smartview smartview{{        // Instantiate smartview
        .application = app,
    }};  

    smartview.set_title("My Wallet"); // Set the window title to "Hello World!"

    //Exposing functions to the frontend
    smartview.expose("record_transaction", WalletManager::record_transaction);
    smartview.expose("record_transfer", WalletManager::record_transfer);
    smartview.expose("create_wallet", WalletManager::create_wallet);
    smartview.expose("update_record", WalletManager::update_record);
    smartview.expose("update_transfer", WalletManager::update_transfer);
    smartview.expose("update_wallet", WalletManager::update_wallet);
    smartview.expose("get_wallets", WalletManager::get_wallets);
    smartview.expose("get_records", WalletManager::get_records);
    smartview.expose("delete_record", WalletManager::delete_record);
    smartview.expose("delete_wallet", WalletManager::delete_wallet);
    smartview.expose("delete_wallet_permanently", WalletManager::delete_wallet_permanently);

    smartview.set_url("http://localhost:5173/");
    smartview.show();                        // Show the smartview
    
    app->run();                             // And finally enter the run-loop.

    return 0;
}