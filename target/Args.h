// Declaration of a class to process command line arguments.
//
// This file is part of the Embecosm Debug Server target for CORE-V MCU
//
// Copyright (C) 2021 Embecosm Limited
// SPDX-License-Identifier: Apache-2.0

#ifndef ARGS_H
#define ARGS_H

#include <cstdint>
#include <string>

/// \brief Class to process arguments
class Args
{
public:

  // Constructor and destructor, no default constructor and no copy
  // constructor.
  Args () = delete;
  Args (int argc, char * argv[]);
  Args (const Args&) = delete;
  ~Args ();

  // API calls
  uint64_t clkPeriodNs () const;
  uint64_t durationNs () const;
  std::string vcd () const;

  // Delete the copy assignment operator
  Args& operator=(const Args&) = delete;

private:

  /// \brief the Period of the clock in nanoseconds
  uint64_t mClkPeriodNs;

  /// \brief the duration of execution in nanoseconds
  uint64_t mDurationNs;

  /// \brief Name of the VCD file (empty if not specified)
  std::string mVcd;
};

#endif	// ARGS_H
