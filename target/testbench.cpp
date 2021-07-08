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

#include "Args.h"
#include "IDtm.h"
#include "DtmJtag.h"

using std::cout;
using std::dec;
using std::endl;
using std::hex;
using std::setfill;
using std::setw;
using std::unique_ptr;

/// \brief Population count of ones.
///
/// From the Aggregate MAGIC algorithms (see aggregate.org).
///
/// \param[in] x  The number whose number of ones is to be counted.
/// \returns Number of bits set to 1 in \p x.
static uint32_t
ones32(uint32_t x)
{
  // 32-bit recursive reduction using SWAR...
  //   but first step is mapping 2-bit values
  //   into sum of 2 1-bit values in sneaky way
  x -= ((x >> 1) & 0x55555555);
  x = (((x >> 2) & 0x33333333) + (x & 0x33333333));
  x = (((x >> 4) + x) & 0x0f0f0f0f);
  x += (x >> 8);
  x += (x >> 16);

  return(x & 0x0000003f);
}

/// \brief Floor of the log to base 2 of an integer.
///
/// From the Aggregate MAGIC algorithms (see aggregate.org).
///
/// \param[in] x  The number whose log is to be computed.
/// \returns log2 of \p x, or -1 if \px is zero.
static int
floor_log2 (uint32_t x)
{
  x |= (x >> 1);
  x |= (x >> 2);
  x |= (x >> 4);
  x |= (x >> 8);
  x |= (x >> 16);

  return (ones32(x) - 1);
}

/// \brief Ceiling of the log to base 2 of an integer.
///
/// From the Aggregate MAGIC algorithms (see aggregate.org).
///
/// \param[in] x  The number whose log is to be computed.
/// \returns log2 of \p x, or -1 if \px is zero.
static int
ceil_log2 (uint32_t x)
{
	int y = (x & (x - 1));

	y |= -y;
	y >>= 31;
        x |= (x >> 1);
        x |= (x >> 2);
        x |= (x >> 4);
        x |= (x >> 8);
        x |= (x >> 16);

        return (ones32 (x) - 1 - y);
}


int
main(int   argc,
     char *argv[])
{
  unique_ptr<Args> args (new Args (argc, argv));
  unique_ptr<IDtm> dtm (new DtmJtag (args->clkPeriodNs (), args->durationNs (),
				     args->vcd ().c_str ()));
  // Reset the processor
  dtm->reset ();

  // Explore the target
  uint32_t dmcontrol = IDtm::HARTSELLO_MASK | IDtm::HARTSELHI_MASK;
  // cout << "dmcontrol = 0x" << setw (8) << setfill ('0') << hex << dmcontrol
  //      << dec << setfill (' ') << setw (0) << endl;
  dmcontrol = dtm->dmiWrite (static_cast<uint64_t> (IDtm::DMCONTROL), dmcontrol);
  // cout << "dmcontrol = 0x" << setw (8) << setfill ('0') << hex << dmcontrol
  //      << dec << setfill (' ') << setw (0) << endl;
  uint32_t hartselhi = (dmcontrol & IDtm::HARTSELHI_MASK)
    >> IDtm::HARTSELHI_OFFSET;
  uint32_t hartsello = (dmcontrol & IDtm::HARTSELLO_MASK)
    >> IDtm::HARTSELLO_OFFSET;
  uint32_t hartsel = (hartselhi << IDtm::HARTSELLO_SIZE) | hartsello;
  uint32_t numHarts;
  for (numHarts = 0; numHarts <= hartsel; numHarts++)
    {
      hartselhi = numHarts >> IDtm::HARTSELLO_SIZE;
      hartsello = numHarts & ((1U << IDtm::HARTSELLO_SIZE) - 1U);
      dmcontrol = (hartselhi << IDtm::HARTSELHI_OFFSET)
	| (hartsello << IDtm::HARTSELLO_OFFSET);
      cout << "hart = " << setw (7) << numHarts << ", dmcontrol = 0x" << setw (8)
	   << setfill ('0') << hex << dmcontrol;
      dmcontrol = dtm->dmiWrite (static_cast<uint64_t> (IDtm::DMCONTROL),
				 dmcontrol);
      uint32_t dmstatus = dtm->dmiRead (static_cast<uint64_t> (IDtm::DMSTATUS));
      cout << ", dmstatus = 0x" << setw (8) << setfill ('0') << hex << dmstatus
	   << dec << setfill (' ') << setw (0) << endl;
      if ((dmstatus & IDtm::ANYNONEXISTENT_MASK) != 0)
	break;
    }

  cout << "Num HARTS: " << numHarts << endl;

  // Delete the DTM, which will save the VCD
  dtm.reset(nullptr);
  return EXIT_SUCCESS;
}
