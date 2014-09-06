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

#include <iostream>
#include <exception>
#include <cstring>
#include <cerrno>
#include <cstdint>
#include <NumStringConv.h>

using namespace CLRX;

int main(int argc, const char** argv)
{
    if (argc < 3)
    {
        std::cout << "Usage: cstrtouXCStyle number_with_delim b|h|i|l [numchars]" << std::endl;
        return 0;
    }
    
    size_t clen = 0;
    if (argc == 4)
    {
        char* endptr;
        errno = 0;
        clen = strtoul(argv[3], &endptr, 10);
        if (errno != 0 || *endptr != 0 || endptr == argv[3])
        {
            std::cerr << "Cant parse numchars" << std::endl;
            return 1;
        }
    }
    
    if (clen == 0)
        clen = ::strlen(argv[1]);
    const char* outEnd;
    try
    {
        uint64_t v;
        switch(argv[2][0])
        {
            case 'b':
                v = cstrtou8CStyle(argv[1], argv[1] + clen, outEnd);
                break;
            case 'h':
                v = cstrtou16CStyle(argv[1], argv[1] + clen, outEnd);
                break;
            case 'i':
                v = cstrtou32CStyle(argv[1], argv[1] + clen, outEnd);
                break;
            case 'l':
                v = cstrtou64CStyle(argv[1], argv[1] + clen, outEnd);
                break;
            default:
                throw Exception("Unknown type");
                break;
        }
            
        std::cout << v << std::endl;
        std::cout << outEnd << std::endl;
    }
    catch(const std::exception& ex)
    {
        std::cerr << "Failed: " << ex.what() << std::endl;
        return 1;
    }
    return 0;
}
