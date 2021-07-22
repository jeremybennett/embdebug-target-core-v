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
main(int   argc,
     char *argv[])
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
  unique_ptr<Dmi::Dmcontrol> & dmcontrol = dmi->dmcontrol ();
  unique_ptr<Dmi::Dmstatus> & dmstatus = dmi->dmstatus ();
  dmcontrol->reset ();
  dmcontrol->hartselMax ();
  dmcontrol->dmactive (true);
  dmcontrol->write ();
  uint32_t hartselLen = dmcontrol->hartsel ();
  cout << "Max HARTS: 0x" << hex << (hartselLen + 1) << dec << endl;

  uint32_t numHarts;
  for (numHarts = 0; numHarts <= hartselLen; numHarts++)
    {
      dmcontrol->reset ();
      dmcontrol->hartsel (numHarts);
      dmcontrol->dmactive (true);
      dmcontrol->write ();

      dmstatus->read ();
      if (dmstatus->unavail ())
	break;
    }

  cout << "Num HARTS: " << numHarts << endl;

  // Delete the DMI, and hence DTM and TAP, which will save the VCD
  dmi.reset(nullptr);
  return EXIT_SUCCESS;
}
