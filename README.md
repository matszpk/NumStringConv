NumStringConv
=============

number from/to string conversion utilities

This simple project contains my private functions to number from/to string conversions.

Source code has been written in C++11, so you must have compiler that supports this standard to compile files.

Sample compilation:
g++ -Wall -std=gnu++11 -I. -c -o NumStringConv.o NumStringConv.cpp

cstrtofXCStyle routine:

  converts string to number in IEEE-754 format (half, float or double). Supports only rounding to nearest even and
  ignores locales (useful for conversion for textual files likes source codes).
  This functions support decimal form and hexadecimal form (as in C language).
  This functions has been designed to be the most accurate and uses big number arithmetic to achieve required precision.
  WARNING: this version does not skip spaces, you must skip before use this function!
  
fXtocstrCStyle routines:

  converts number in IEEE-754 format (half, float or double). Can converts only to decimal form (human readable or scientific).

cstrtouXCStyle routines:
  convert string (in C language literal format) to unsigned integer number. Supports binary (prefix: 0b), octal (prefix: 0)
  and hexadecimal (prefix: 0x).
  WARNING: this version does not skip spaces, you must skip before use this function!

uXtocstrCStyle routines:
  convert unsigned integer number to string.
