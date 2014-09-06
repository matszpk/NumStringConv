/*
 *  NumStringConv - number from/to string conversion utilities
 *  Copyright (C) 2014 Mateusz Szpakowski
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include <cstdio>
#include <exception>
#include <cstring>
#include <NumStringConv.h>

using namespace CLRX;

union FloatUnion
{
    float f;
    uint32_t u;
};

union DoubleUnion
{
    double d;
    uint64_t u;
};

float halfToFloat(uint16_t halfVal)
{
    FloatUnion v;
    if ((halfVal & 0x7c00) != 0x7c00 && (halfVal & 0x7c00) != 0)
        v.u = ((halfVal&0x8000)<<16) | ((((halfVal>>10)&31)-15 + 127)<<23) |
            ((halfVal&0x3ff)<<13);
    else if ((halfVal & 0x7c00) == 0x7c00) // if inf/nan
        v.u = ((halfVal&0x8000)<<16) | 0x7f800000 | ((halfVal&0x3ff)<<13);
    else
    {
        if ((halfVal & 0x7fff) == 0) // if zero
            v.u = ((halfVal&0x8000)<<16);
        else
        {   // denormalized
            uint16_t mant = halfVal&0x3ff;
            uint16_t expf = 128-15;
            for (; mant < 0x400; mant<<=1, expf--);
            v.u = ((halfVal&0x8000)<<16) | (expf<<23) | ((mant&0x3ff)<<13);
        }
    }
    return v.f;
}

int main(int argc, const char** argv)
{
    if (argc < 3)
    {
        puts("Usage: cstrtofXCStyle number_with_delim h|f|d [numchars]");
        return 0;
    }
    
    size_t clen = 0;
    if (argc == 4)
    {
        char* endptr;
        errno = 0;
        clen = strtoul(argv[3], &endptr, 10);
    }
    
    if (clen == 0)
        clen = ::strlen(argv[1]);
    const char* outEnd;
    char* endptr;
    try
    {
        switch(argv[2][0])
        {
            case 'h':
            {
                uint16_t v = cstrtohCStyle(argv[1], argv[1] + clen, outEnd);
                printf("NumStr: %04hx,%1.4e\n", v, halfToFloat(v));
                break;
            }
            case 'f':
            {
                FloatUnion v, ov;
                v.f = cstrtofCStyle(argv[1], argv[1] + clen, outEnd);
                ov.f = strtof(argv[1], &endptr);
                printf("NumStr: %08x,%1.7e\nSystem: %08x,%1.7e\n", v.u, v.f, ov.u, ov.f);
                break;
            }
            case 'd':
            {
                DoubleUnion v, ov;
                v.d = cstrtodCStyle(argv[1], argv[1] + clen, outEnd);
                ov.d = strtod(argv[1], &endptr);
                printf("NumStr: %016llx,%1.16e\nSystem: %016llx,%1.16e\n", v.u, v.d, ov.u, ov.d);
                break;
            }
            default:
                throw Exception("Unknown type");
                break;
        }
        puts(outEnd);
    }
    catch(const std::exception& ex)
    {
        fputs("Failed: ",stderr);
        fputs(ex.what(), stderr);
        fputs("\n",stderr);
        return 1;
    }
    return 0;
}
