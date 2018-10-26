#pragma once

#include <stdint.h>

constexpr
static
uint32_t
compute_hash( const char*  iStr )
{
    constexpr  uint32_t  FNV_OFFSET_BASIS = 0x811c9dc5;
    constexpr  uint32_t  FNV_PRIME = 16777619;
    uint32_t  hash = FNV_OFFSET_BASIS;
    while( *iStr )
    {
        hash = hash ^ static_cast< uint32_t >( *iStr++);
        hash = hash * FNV_PRIME;
    }

    return  hash;
}

