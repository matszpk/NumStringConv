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
/*! \file NumStringConv.h
 * \brief Number from/to string utilities
 */

#ifndef __NUMSTRINGCONV_H__
#define __NUMSTRINGCONV_H__

#include <exception>
#include <string>
#include <cstdlib>
#include <cstring>
#include <cstdint>
#include <mutex>

/** HAVE_INT128 - enable GCC __int128 support for faster multiplicartion */
#if defined(__SIZEOF_POINTER__) && __SIZEOF_POINTER__==8 && defined(__GNUC__)
#  define HAVE_INT128 1
#endif

/* types and defs */

typedef signed char cxchar;
typedef unsigned char cxuchar;
typedef unsigned char cxbyte;
typedef signed short cxshort;
typedef unsigned short cxushort;
typedef signed int cxint;
typedef unsigned int cxuint;
typedef signed long cxlong;
typedef unsigned long cxulong;
typedef signed long long cxllong;
typedef unsigned long long cxullong;

/// main namespace
namespace CLRX
{

/// exception class
class Exception: public std::exception
{
protected:
    std::string message;
public:
    Exception() = default;
    explicit Exception(const std::string& message);
    virtual ~Exception() throw() = default;
    
    const char* what() const throw();
};

/// parse exception class
class ParseException: public Exception
{
public:
    ParseException() = default;
    explicit ParseException(const std::string& message);
    ParseException(size_t lineNo, const std::string& message);
    ParseException(size_t lineNo, size_t charNo, const std::string& message);
    virtual ~ParseException() throw() = default;
};

/// counts leading zeroes for 32-bit unsigned integer. For zero behavior is undefined
inline cxuint CLZ32(uint32_t v);
/// counts leading zeroes for 64-bit unsigned integer. For zero behavior is undefined
inline cxuint CLZ64(uint64_t v);

inline cxuint CLZ32(uint32_t v)
{
#ifdef __GNUC__
    return __builtin_clz(v);
#else
    cxuint count = 0;
    for (uint32_t t = 1U<<31; t > v; t>>=1, count++);
    return count;
#endif
}

inline cxuint CLZ64(uint64_t v)
{
#ifdef __GNUC__
    return __builtin_clzll(v);
#else
    cxuint count = 0;
    for (uint64_t t = 1ULL<<63; t > v; t>>=1, count++);
    return count;
#endif
}

/// parse unsigned integer regardless locales
/** parses unsigned integer in decimal form from str string. inend can points
 * to end of string or can be null. Function throws ParseException when number in string
 * is out of range, when string does not have number or inend points to string.
 * \param str input string pointer
 * \param inend pointer points to end of string or null if not end specified
 * \param outend returns end of number in string
 * \return parsed integer value
 */
extern cxuint cstrtoui(const char* str, const char* inend, const char*& outend);

/// parse 8-bit unsigned formatted looks like C-style
/** parses 8-bit unsigned integer from str string. inend can points
 * to end of string or can be null. Function throws ParseException when number in string
 * is out of range, when string does not have number or inend points to string.
 * Function accepts decimal format, octal form (with prefix '0'), hexadecimal form
 * (prefix '0x' or '0X'), and binary form (prefix '0b' or '0B').
 * \param str input string pointer
 * \param inend pointer points to end of string or null if not end specified
 * \param outend returns end of number in string
 * \return parsed integer value
 */
extern uint8_t cstrtou8CStyle(const char* str, const char* inend, const char*& outend);

/// parse 16-bit unsigned formatted looks like C-style
/** parses 16-bit unsigned integer from str string. inend can points
 * to end of string or can be null. Function throws ParseException when number in string
 * is out of range, when string does not have number or inend points to string.
 * Function accepts decimal format, octal form (with prefix '0'), hexadecimal form
 * (prefix '0x' or '0X'), and binary form (prefix '0b' or '0B').
 * \param str input string pointer
 * \param inend pointer points to end of string or null if not end specified
 * \param outend returns end of number in string
 * \return parsed integer value
 */
extern uint16_t cstrtou16CStyle(const char* str, const char* inend, const char*& outend);

/// parse 32-bit unsigned formatted looks like C-style
/** parses 32-bit unsigned integer from str string. inend can points
 * to end of string or can be null. Function throws ParseException when number in string
 * is out of range, when string does not have number or inend points to string.
 * Function accepts decimal format, octal form (with prefix '0'), hexadecimal form
 * (prefix '0x' or '0X'), and binary form (prefix '0b' or '0B').
 * \param str input string pointer
 * \param inend pointer points to end of string or null if not end specified
 * \param outend returns end of number in string
 * \return parsed integer value
 */
extern uint32_t cstrtou32CStyle(const char* str, const char* inend, const char*& outend);

/// parse 64-bit unsigned formatted looks like C-style
/** parses 64-bit unsigned integerfrom str string. inend can points
 * to end of string or can be null. Function throws ParseException when number in string
 * is out of range, when string does not have number or inend points to string.
 * Function accepts decimal format, octal form (with prefix '0'), hexadecimal form
 * (prefix '0x' or '0X'), and binary form (prefix '0b' or '0B').
 * \param str input string pointer
 * \param inend pointer points to end of string or null if not end specified
 * \param outend returns end of number in string
 * \return parsed integer value
 */
extern uint64_t cstrtou64CStyle(const char* str, const char* inend, const char*& outend);

/// parse half float formatted looks like C-style
/** parses half floating point from str string. inend can points
 * to end of string or can be null. Function throws ParseException when number in string
 * is out of range, when string does not have number or inend points to string.
 * Function accepts decimal format and binary format. Result is rounded to nearest even
 * (if two values are equally close will be choosen a even value).
 * Currently only IEEE-754 format is supported.
 * \param str input string pointer
 * \param inend pointer points to end of string or null if not end specified
 * \param outend returns end of number in string
 * \return parsed floating point value
 */
extern cxushort cstrtohCStyle(const char* str, const char* inend, const char*& outend);

/// parse single float formatted looks like C-style
/** parses single floating point from str string. inend can points
 * to end of string or can be null. Function throws ParseException when number in string
 * is out of range, when string does not have number or inend points to string.
 * Function accepts decimal format and binary format. Result is rounded to nearest even
 * (if two values are equally close will be choosen a even value).
 * Currently only IEEE-754 format is supported.
 * \param str input string pointer
 * \param inend pointer points to end of string or null if not end specified
 * \param outend returns end of number in string
 * \return parsed floating point value
 */
extern float cstrtofCStyle(const char* str, const char* inend, const char*& outend);

/// parse double float formatted looks like C-style
/** parses double floating point from str string. inend can points
 * to end of string or can be null. Function throws ParseException when number in string
 * is out of range, when string does not have number or inend points to string.
 * Function accepts decimal format and binary format. Result is rounded to nearest even
 * (if two values are equally close will be choosen a even value).
 * Currently only IEEE-754 format is supported.
 * \param str input string pointer
 * \param inend pointer points to end of string or null if not end specified
 * \param outend returns end of number in string
 * \return parsed floating point value
 */
extern double cstrtodCStyle(const char* str, const char* inend, const char*& outend);

/// format 32-bit unsigned integer
/** format 32-bit unsigned integer in C-style formatting.
 * \param value integer value
 * \param str output string
 * \param maxSize max size of string (including null-character)
 * \param radix radix of digits (2, 8, 10, 16)
 * \param width max number of digits in number
 * \param prefix adds required prefix if true
 * \return length of output string (excluding null-character)
 */
extern size_t u32tocstrCStyle(uint32_t value, char* str, size_t maxSize, cxuint radix = 10,
       cxuint width = 0, bool prefix = true);

/// format 64-bit unsigned integer
/** format 64-bit unsigned integer in C-style formatting.
 * \param value integer value
 * \param str output string
 * \param maxSize max size of string (including null-character)
 * \param radix radix of digits (2, 8, 10, 16)
 * \param width max number of digits in number
 * \param prefix adds required prefix if true
 * \return length of output string (excluding null-character)
 */
extern size_t u64tocstrCStyle(uint64_t value, char* str, size_t maxSize, cxuint radix = 10,
        cxuint width = 0, bool prefix = true);

/// format half float in C-style
/** format to string the half float in C-style formatting. This function handles 2 modes
 * of printing value: human readable and scientific. Scientific mode forces form with
 * decimal exponent.
 * Currently only IEEE-754 format is supported.
 * \param value float value
 * \param str output string
 * \param maxSize max size of string (including null-character)
 * \param scientific enable scientific mode
 * \return length of output string (excluding null-character)
 */
extern size_t htocstrCStyle(cxushort value, char* str, size_t maxSize,
                            bool scientific = false);

/// format single float in C-style
/** format to string the single float in C-style formatting. This function handles 2 modes
 * of printing value: human readable and scientific. Scientific mode forces form with
 * decimal exponent.
 * Currently only IEEE-754 format is supported.
 * \param value float value
 * \param str output string
 * \param maxSize max size of string (including null-character)
 * \param scientific enable scientific mode
 * \return length of output string (excluding null-character)
 */
extern size_t ftocstrCStyle(float value, char* str, size_t maxSize,
                            bool scientific = false);

/// format double float in C-style
/** format to string the double float in C-style formatting. This function handles 2 modes
 * of printing value: human readable and scientific. Scientific mode forces form with
 * decimal exponent.
 * Currently only IEEE-754 format is supported.
 * \param value float value
 * \param str output string
 * \param maxSize max size of string (including null-character)
 * \param scientific enable scientific mode
 * \return length of output string (excluding null-character)
 */
extern size_t dtocstrCStyle(double value, char* str, size_t maxSize,
                            bool scientific = false);

};

#endif
