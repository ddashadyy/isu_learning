#pragma once

#include <string>

template< class T >
struct IByteSerialize;

// в планах сделать
template< class T >
struct IJsonSerialize;


template< class T >
struct IByteSerialize
{
    using reference = T&;

    IByteSerialize() = default;
    virtual ~IByteSerialize() = default;

    virtual void serialize( const std::string& destination ) = 0;
    virtual reference deserialize( const std::string& source ) = 0;
};

