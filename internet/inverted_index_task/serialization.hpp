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

    virtual void serialize() = 0;
    virtual reference deserialize( const std::string& file_name ) = 0;
};

