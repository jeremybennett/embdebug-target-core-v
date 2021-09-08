// Declaration of the JTAG Test Interface
//
// This file is part of the Embecosm Debug Server target for CORE-V MCU
//
// Copyright (C) 2021 Embecosm Limited
// SPDX-License-Identifier: Apache-2.0

#ifndef TEST_JTAG_H
#define TEST_JTAG_H

#include <sstream>
#include <vector>

#include "Dmi.h"

/// \brief The class to test the JTAG model interface
class TestJtag
{
public:
  // Constructor and destructor
  TestJtag (std::unique_ptr<Dmi> &dmi_, uint32_t numHarts_);
  TestJtag () = delete;
  ~TestJtag () = default;

  // Delete the copy assignment operator
  TestJtag &operator= (const TestJtag &) = delete;

  // Test APU
  void hartStatus (uint32_t h);
  void reportPC ();
  void haltHart (uint32_t h);
  void testGprs ();
  void testFprs ();
  void testCsrs ();

private:
  /// \brief How many registers to print per row
  static const size_t REGS_PER_ROW = 3;

  /// \brief A vector of all the standard user CSRs
  static const std::vector<uint16_t> userCsrList;

  /// \brief A vector of all the custom user CSRs
  static const std::vector<uint16_t> customUserCsrList;

  /// \brief A vector of all the standard machine CSRs
  static const std::vector<uint16_t> machineCsrList;

  /// \brief Reference to the DMI interface.
  std::unique_ptr<Dmi> &mDmi;

  /// \brief Number of available Harts to debug
  uint32_t mNumHarts;

  /// \brief Reference to the \c dmstatus DMI register
  std::unique_ptr<Dmi::Dmstatus> &mDmstatus;

  /// \brief Reference to the \c hartinfo DMI register
  std::unique_ptr<Dmi::Hartinfo> &mHartinfo;

  /// \brief Reference to the \c abstractcs DMI register
  std::unique_ptr<Dmi::Abstractcs> &mAbstractcs;

  /// \brief General string stream for use by utility methods
  static std::ostringstream sOss;

  /// Utility methods
  static const char *gprAbiName (std::size_t regno);
  static std::string fullGprName (std::size_t regno);
  static const char *fprAbiName (std::size_t regno);
  static std::string fullFprName (std::size_t regno);
};

#endif // TEST_JTAG_H
