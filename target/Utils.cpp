// Definition of some useful static utils
//
// This file is part of the Embecosm Debug Server target for CORE-V MCU
//
// Copyright (C) 2021 Embecosm Limited
// SPDX-License-Identifier: Apache-2.0

#include <iomanip>
#include <iostream>
#include <sstream>

#include "Utils.h"

using std::hex;
using std::ostringstream;
using std::setfill;
using std::setw;
using std::size_t;
using std::string;

/// Instantiate the output string stream.
ostringstream Utils::oss;

/// \brief Generate the hexadecimal representation of an octet.
///
/// \note No leading "0x".
///
/// \param[in] val  The value to print.
/// \param[in] len  The number of digits to print (default 2).
/// \return  A string with the hexadecimal representation of \p val, left
///          padded with zeros to at least \p len chars.
string
Utils::hexStr (const uint8_t val, const size_t len)
{
  oss.str ("");
  oss << hex << setfill ('0') << setw (len) << val;
  return oss.str ();
}

/// \brief Generate the hexadecimal representation of a 16-bit word.
///
/// \note No leading "0x".
///
/// \param[in] val  The value to print.
/// \param[in] len  The number of digits to print (default 4).
/// \return  A string with the hexadecimal representation of \p val, left
///          padded with zeros to at least \p len chars.
string
Utils::hexStr (const uint16_t val, const size_t len)
{
  oss.str ("");
  oss << hex << setfill ('0') << setw (len) << val;
  return oss.str ();
}

/// \brief Generate the hexadecimal representation of a 32-bit word.
///
/// \note No leading "0x".
///
/// \param[in] val  The value to print.
/// \param[in] len  The number of digits to print (default 8).
/// \return  A string with the hexadecimal representation of \p val, left
///          padded with zeros to at least \p len chars.
string
Utils::hexStr (const uint32_t val, const size_t len)
{
  oss.str ("");
  oss << hex << setfill ('0') << setw (len) << val;
  return oss.str ();
}

/// \brief Generate the hexadecimal representation of a 64-bit word.
///
/// \note No leading "0x".
///
/// \param[in] val  The value to print.
/// \param[in] len  The number of digits to print (default 16).
/// \return  A string with the hexadecimal representation of \p val, left
///          padded with zeros to at least \p len chars.
string
Utils::hexStr (const uint64_t val, const size_t len)
{
  oss.str ("");
  oss << hex << setfill ('0') << setw (len) << val;
  return oss.str ();
}

/// \brief String representation of booleans.
///
/// \param[in] flag  Value to represent.
/// \return \c "true if \p flag is true, \c "false" otherwise.
string
Utils::boolStr (bool flag)
{
  oss.str ("");

  if (flag)
    oss << "true";
  else
    oss << "false";

  return oss.str ();
}

/// \brief String representation of numbers as boolean values.
///
/// \param[in] val  Value to represent.
/// \return  \c "true" if \p val is non-zero, \c "false" otherwise.
string
Utils::nonZero (uint64_t val)
{
  oss.str ("");

  if (val != 0)
    oss << "true";
  else
    oss << "false";

  return oss.str ();
}
