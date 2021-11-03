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
  Args (int argc, char *argv[]);
  Args (const Args &) = delete;
  ~Args ();

  // API calls
  uint64_t clkPeriodNs () const;
  uint64_t durationNs () const;
  std::string vcd () const;
  bool testStatus () const;
  bool testGprs () const;
  bool testFprs () const;
  bool testCsrs () const;
  bool testFpuCsrs () const;

  // Delete the copy assignment operator
  Args &operator= (const Args &) = delete;

private:
  /// \brief the Period of the clock in nanoseconds
  uint64_t mClkPeriodNs;

  /// \brief the duration of execution in nanoseconds
  uint64_t mDurationNs;

  /// \brief Name of the VCD file (empty if not specified)
  std::string mVcd;

  /// \brief True if we should test hart status
  bool mTestStatus;

  /// \brief True if we should test GPRs
  bool mTestGprs;

  /// \brief True if we should test FPRs and FPU related CSRs
  bool mTestFprs;

  /// \brief True if we should test CSRs
  bool mTestCsrs;

  /// \brief True if we should test FPU related CSRs
  bool mTestFpuCsrs;
};

#endif // ARGS_H
