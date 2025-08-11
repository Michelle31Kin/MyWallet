#include <saucer/smartview.hpp>
#include "include_cpp/my.h"

int main()
{
    auto app = saucer::application::init({      // Instantiate application
        .id = "myWallet",                    // Choose a suitable ID for your program
    });

    saucer::smartview smartview{{        // Instantiate smartview
        .application = app,
    }};

    smartview.set_title("My Wallet"); // Set the window title to "Hello World!"

    smartview.set_url("http://localhost:5173/"); // Navigate to google.com
    smartview.show();                        // Show the smartview
    
    app->run();                             // And finally enter the run-loop.

    return 0;
}