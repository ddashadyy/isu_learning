#pragma once

#include <string>
#include <nlohmann/json.hpp>


template< class T >
struct IJsonSerialize
{
    using reference = T&;

    IJsonSerialize() = default;
    virtual ~IJsonSerialize() = default;

    virtual void serialize( const std::string& destination ) = 0;
    virtual reference deserialize( const std::string& source ) = 0;
};
