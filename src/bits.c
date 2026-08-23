/*
bits.c -- bits editing for Vinora Engine
Copyright (c) 2026 Evgeniy Parfenyuk <parthen@riseup.net>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.
*/

#include "bits.h"

void SetBit(unsigned char *mem, int memSize, int bitId)
{
    if ((bitId < 0) || ((bitId / 8) >= memSize))
        return;
    int byteIndex = bitId / 8;
    int bitOffset = bitId % 8;
    mem[byteIndex] = mem[byteIndex] | (1 << bitOffset);
}

bool IsBitSet(const unsigned char *mem, int memSize, int bitId)
{
    if ((bitId < 0) || ((bitId / 8) >= memSize))
        return false;
    int byteIndex = bitId / 8;
    int bitOffset = bitId % 8;

    return mem[byteIndex] & (1 << bitOffset);
}
