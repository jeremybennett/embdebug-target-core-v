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

#include <iomanip>
#include <iostream>
#include <sstream>

#include "Args.h"
#include "Dmi.h"
#include "DtmJtag.h"
#include "IDtm.h"

using std::cout;
using std::dec;
using std::endl;
using std::hex;
using std::setfill;
using std::setw;
using std::unique_ptr;

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

  // Explore the target
  unique_ptr<Dmi::Dmcontrol> &dmcontrol = dmi->dmcontrol ();
  unique_ptr<Dmi::Dmstatus> &dmstatus = dmi->dmstatus ();
  dmcontrol->reset ();
  dmcontrol->hartselMax ();
  dmcontrol->dmactive (true);
  dmcontrol->write ();
  dmcontrol->reset ();
  dmcontrol->read ();
  uint32_t hartselLen = dmcontrol->hartsel ();
  cout << "Max HARTS: 0x" << hex << (hartselLen + 1) << dec << endl;

  cout << "Testing HART availability" << endl << endl;
  uint32_t numHarts;
  for (numHarts = 0; numHarts <= hartselLen; numHarts++)
    {
      dmcontrol->reset ();
      dmcontrol->hartsel (numHarts);
      dmcontrol->dmactive (true);
      dmcontrol->write ();

      dmstatus->read ();
      dmstatus->prettyPrint (false);
      cout << "Hart " << numHarts << ", dmstatus: 0x" << dmstatus;
      dmstatus->prettyPrint (true);
      cout << " " << dmstatus << endl << endl;

      if (dmstatus->unavail ())
        break;
    }

  cout << "Num HARTS: " << numHarts << endl << endl;

  // Read the Hartinfo for each hart
  unique_ptr<Dmi::Hartinfo> &hartinfo = dmi->hartinfo ();
  for (uint32_t h = 0; h < numHarts; h++)
    {
      dmcontrol->reset ();
      dmcontrol->hartsel (h);
      dmcontrol->dmactive (true);
      dmcontrol->write ();

      hartinfo->read ();
      cout << "Hart " << numHarts << ", hartinfo: 0x" << hartinfo;
      hartinfo->prettyPrint (true);
      cout << " " << hartinfo << endl << endl;
    }

  // Delete the DMI, and hence DTM and TAP, which will save the VCD
  dmi.reset (nullptr);
  return EXIT_SUCCESS;
}
