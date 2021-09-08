// Trivial Verilator test bench interface implementation
//
// This file is part of the Embecosm Debug Server target for CORE-V MCU
//
// Copyright (C) 2021 Embecosm Limited
// SPDX-License-Identifier: Apache-2.0

/// \mainpage Embdebug CORE-V Verilator target
///
/// This is a standard Embdebug for a Verilator model.  The point of
/// engagement with the Verilator model is the model of the JTAG TAP, and thus
/// the Verilator model sits underneath this.

#include <iostream>
#include <sstream>

#include "Args.h"
#include "Dmi.h"
#include "DtmJtag.h"
#include "IDtm.h"
#include "TestJtag.h"
#include "Utils.h"

using std::cout;
using std::endl;
using std::unique_ptr;
using std::vector;

static uint32_t
countHarts (unique_ptr<Dmi> &dmi)
{
  uint32_t hartselLen = dmi->hartsellen ();
  cout << "Max HARTS: 0x" << Utils::hexStr (hartselLen + 1) << endl;
  cout << "Testing HART availability" << endl << endl;
  uint32_t numHarts;
  unique_ptr<Dmi::Dmstatus> &dmstatus = dmi->dmstatus ();
  for (numHarts = 0; numHarts <= hartselLen; numHarts++)
    {
      dmi->selectHart (numHarts);
      dmstatus->read ();
      dmstatus->prettyPrint (false);
      cout << "Hart " << numHarts << ", dmstatus: 0x" << dmstatus;
      dmstatus->prettyPrint (true);
      cout << " " << dmstatus << endl << endl;

      if (dmstatus->nonexistent ())
        break;
    }

  return numHarts;
}

int
main (int argc, char *argv[])
{
  unique_ptr<Args> args (new Args (argc, argv));

  // We create the DTM here, because only at this level do we know what
  // derived class we will instantiate.  But we then pass ownership to the
  // DMI, because that is where it belongs.
  unique_ptr<IDtm> dtm (new DtmJtag (args->clkPeriodNs (), args->durationNs (),
                                     args->vcd ().c_str ()));
  unique_ptr<Dmi> dmi (new Dmi (std::move (dtm)));

  // Reset the processor
  dmi->dtmReset ();

  // Count the harts and create the testsuite
  uint32_t numHarts = countHarts (dmi);
  unique_ptr<TestJtag> testsuite (new TestJtag (dmi, numHarts));

  // Explore the target
  cout << "Num harts: " << numHarts << endl << endl;

  // Test accessing each hart
  for (uint32_t h = 0; h < numHarts; h++)
    {
      cout << "Testing hart " << h << endl;
      cout << "============="
           << Utils::padStr ("", Utils::decStr (h).size (), '=') << endl
           << endl;

      // First halt the hart
      testsuite->haltHart (h);

      // Status of this hart
      if (args->testStatus ())
        testsuite->hartStatus (h);

      // Test the GPRs
      if (args->testGprs ())
        testsuite->testGprs ();

      // FPRs. This test should not be run unless MISA indicates there is a FPU.
      if (args->testFprs ())
        testsuite->testFprs ();

      // CSRs
      if (args->testCsrs ())
        testsuite->testCsrs ();
    }

  // Delete the DMI, and hence DTM and TAP, which will save the VCD
  dmi.reset (nullptr);
  return EXIT_SUCCESS;
}
