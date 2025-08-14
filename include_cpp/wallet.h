#pragma once

#include "my.h"

struct wallet
{
    std::string name;
    std::string currency;
    std::string source;
    size_t      initial_amount;
    size_t      balance;
    std::string color;
    std::string created_at;
    std::string updated_at;
};

template <>
struct glz::meta<wallet> {
    static constexpr auto value = glz::object(
        "name",           &wallet::name,
        "currency",       &wallet::currency,
        "source",         &wallet::source,
        "initial_amount", &wallet::initial_amount,
        "balance",        &wallet::balance,
        "color",          &wallet::color,
        "created_at",     &wallet::created_at,
        "updated_at",     &wallet::updated_at
    );
};