#pragma once

#include <string>
#include <unordered_map>

#include <saucer/webview.hpp>

#include "_app/env.js.hpp"
#include "_app/immutable/assets/style.DerPdVYS.css.hpp"
#include "_app/immutable/bundle.jqqEe9hw.js.hpp"
#include "_app/version.json.hpp"
#include "index.html.hpp"

namespace saucer::embedded
{
    inline auto all()
    {
        std::unordered_map<std::string, embedded_file> rtn;

        rtn.emplace("_app/env.js", embedded_file{saucer::stash<>::view(foWb6UOVUrJL2LKbRk62olXudePQFdRamao9WxaQmhCxrz5AUmbd8QElhvILZOvNVxtxHoU5KWTjLhofBPlE3yk8PSRuFCIWthnRHlMOMQVEKhhQkajLtoNF4q3aKOPtZb34n47), "application/javascript"});
        rtn.emplace("_app/immutable/assets/style.DerPdVYS.css", embedded_file{saucer::stash<>::view(fhZOODdbeTVJGvs9skJ8bTaveEZkWe8dyG8Viq3MFYaazExopo8EU6kErfUl6GuaKZCbkaDGUCKcnmIk7pACeqnPmyUgBEK5ZdKwjr2KzzKx3jSWCjxvUMOCKRFhX1ukkTHuS5MD8A6ORhQGFbqfPTC4TZ071fqrTmfq2boy3TJ2ZP), "text/css"});
        rtn.emplace("_app/immutable/bundle.jqqEe9hw.js", embedded_file{saucer::stash<>::view(f8BUnaADmncPMNMtPISmgJFdMnD7lEVyROjfOU4swrMli0uioj90vaEmSvPdPplqsgqn2w4YsSc6CWPGubP21oA1xag6xIr29icaIQcwKAiIGYKa3eq3Ahg71pqjlYKLc0cD2EL26QKQgIzvBDKtf7t1vs62eyFs7iVj1), "application/javascript"});
        rtn.emplace("_app/version.json", embedded_file{saucer::stash<>::view(f138PlP4zGwfteMh1m8XUbyuFH9TFQMMkKyvT8JFnBpT85kOz3qCx60A9AyXlhr9UJtF1w9WVuqGTFRM3LhSzP2ImrnzMu31VYv8SlrnlF5XU29DrEBSsKfDe49BvpAMtoOIQSbvKsu20hwM), "application/json"});
        rtn.emplace("index.html", embedded_file{saucer::stash<>::view(fCEfvH31oaA0jc6RTU3Ott6l8dKggXHp336KiJH3a0mBao9U9OMfvk4XpoCg2HAzOh5JgZxb8MOdYnqTLed2v9NlZE9fEKtpYnqAy0KRDBGHW8e43X76kwDNVvLic4mtAiaLWu), "text/html"});

        return rtn;
    }
} // namespace saucer::embedded
