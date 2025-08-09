#pragma once

#include "my.h"

struct transaction
{
    int id;
    int wallet_id;
    std::string type;
    std::string recorded_at;
    std::string updated_at;
    std::string category;
    std::string description;
    size_t amount;
    int related_wallet_id;
};
