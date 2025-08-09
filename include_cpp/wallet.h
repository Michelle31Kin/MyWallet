#pragma once

#include "my.h"

struct wallet
{
    int id;
    std::string name;
    std::string currency;
    std::string source;
    size_t initial_amount;
    size_t balance;
    std::string color;
    std::string created_at;
    std::string updated_at;
};