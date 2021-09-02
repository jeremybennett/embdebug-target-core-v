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
#include "Utils.h"

using std::cout;
using std::endl;
using std::unique_ptr;
using std::vector;

/// \brief A vector of all the standard user CSRs
static const vector<uint16_t> userCsrList{
  Dmi::Csr::FFLAGS,        Dmi::Csr::FRM,           Dmi::Csr::FCSR,
  Dmi::Csr::CYCLE,         Dmi::Csr::INSTRET,       Dmi::Csr::HPMCOUNTER3,
  Dmi::Csr::HPMCOUNTER4,   Dmi::Csr::HPMCOUNTER5,   Dmi::Csr::HPMCOUNTER6,
  Dmi::Csr::HPMCOUNTER7,   Dmi::Csr::HPMCOUNTER8,   Dmi::Csr::HPMCOUNTER9,
  Dmi::Csr::HPMCOUNTER10,  Dmi::Csr::HPMCOUNTER11,  Dmi::Csr::HPMCOUNTER12,
  Dmi::Csr::HPMCOUNTER13,  Dmi::Csr::HPMCOUNTER14,  Dmi::Csr::HPMCOUNTER15,
  Dmi::Csr::HPMCOUNTER16,  Dmi::Csr::HPMCOUNTER17,  Dmi::Csr::HPMCOUNTER18,
  Dmi::Csr::HPMCOUNTER19,  Dmi::Csr::HPMCOUNTER20,  Dmi::Csr::HPMCOUNTER21,
  Dmi::Csr::HPMCOUNTER22,  Dmi::Csr::HPMCOUNTER23,  Dmi::Csr::HPMCOUNTER24,
  Dmi::Csr::HPMCOUNTER25,  Dmi::Csr::HPMCOUNTER26,  Dmi::Csr::HPMCOUNTER27,
  Dmi::Csr::HPMCOUNTER28,  Dmi::Csr::HPMCOUNTER29,  Dmi::Csr::HPMCOUNTER30,
  Dmi::Csr::HPMCOUNTER31,  Dmi::Csr::CYCLEH,        Dmi::Csr::INSTRETH,
  Dmi::Csr::HPMCOUNTERH3,  Dmi::Csr::HPMCOUNTERH4,  Dmi::Csr::HPMCOUNTERH5,
  Dmi::Csr::HPMCOUNTERH6,  Dmi::Csr::HPMCOUNTERH7,  Dmi::Csr::HPMCOUNTERH8,
  Dmi::Csr::HPMCOUNTERH9,  Dmi::Csr::HPMCOUNTERH10, Dmi::Csr::HPMCOUNTERH11,
  Dmi::Csr::HPMCOUNTERH12, Dmi::Csr::HPMCOUNTERH13, Dmi::Csr::HPMCOUNTERH14,
  Dmi::Csr::HPMCOUNTERH15, Dmi::Csr::HPMCOUNTERH16, Dmi::Csr::HPMCOUNTERH17,
  Dmi::Csr::HPMCOUNTERH18, Dmi::Csr::HPMCOUNTERH19, Dmi::Csr::HPMCOUNTERH20,
  Dmi::Csr::HPMCOUNTERH21, Dmi::Csr::HPMCOUNTERH22, Dmi::Csr::HPMCOUNTERH23,
  Dmi::Csr::HPMCOUNTERH24, Dmi::Csr::HPMCOUNTERH25, Dmi::Csr::HPMCOUNTERH26,
  Dmi::Csr::HPMCOUNTERH27, Dmi::Csr::HPMCOUNTERH28, Dmi::Csr::HPMCOUNTERH29,
  Dmi::Csr::HPMCOUNTERH30, Dmi::Csr::HPMCOUNTERH31
};

/// \brief A vector of all the custom user CSRs
static const vector<uint16_t> customUserCsrList{
  Dmi::Csr::LPSTART0, Dmi::Csr::LPEND0, Dmi::Csr::LPCOUNT0,
  Dmi::Csr::LPSTART1, Dmi::Csr::LPEND1, Dmi::Csr::LPCOUNT1,
  Dmi::Csr::UHARTID,  Dmi::Csr::PRIVLV
};

/// \brief A vector of all the standard machine CSRs
static const vector<uint16_t> machineCsrList{
  Dmi::Csr::MSTATUS,        Dmi::Csr::MISA,           Dmi::Csr::MIE,
  Dmi::Csr::MTVEC,          Dmi::Csr::MCOUNTINHIBIT,  Dmi::Csr::MHPMEVENT3,
  Dmi::Csr::MHPMEVENT4,     Dmi::Csr::MHPMEVENT5,     Dmi::Csr::MHPMEVENT6,
  Dmi::Csr::MHPMEVENT7,     Dmi::Csr::MHPMEVENT8,     Dmi::Csr::MHPMEVENT9,
  Dmi::Csr::MHPMEVENT10,    Dmi::Csr::MHPMEVENT11,    Dmi::Csr::MHPMEVENT12,
  Dmi::Csr::MHPMEVENT13,    Dmi::Csr::MHPMEVENT14,    Dmi::Csr::MHPMEVENT15,
  Dmi::Csr::MHPMEVENT16,    Dmi::Csr::MHPMEVENT17,    Dmi::Csr::MHPMEVENT18,
  Dmi::Csr::MHPMEVENT19,    Dmi::Csr::MHPMEVENT20,    Dmi::Csr::MHPMEVENT21,
  Dmi::Csr::MHPMEVENT22,    Dmi::Csr::MHPMEVENT23,    Dmi::Csr::MHPMEVENT24,
  Dmi::Csr::MHPMEVENT25,    Dmi::Csr::MHPMEVENT26,    Dmi::Csr::MHPMEVENT27,
  Dmi::Csr::MHPMEVENT28,    Dmi::Csr::MHPMEVENT29,    Dmi::Csr::MHPMEVENT30,
  Dmi::Csr::MHPMEVENT31,    Dmi::Csr::MSCRATCH,       Dmi::Csr::MEPC,
  Dmi::Csr::MCAUSE,         Dmi::Csr::MTVAL,          Dmi::Csr::MIP,
  Dmi::Csr::TSELECT,        Dmi::Csr::TDATA1,         Dmi::Csr::TDATA2,
  Dmi::Csr::TDATA3,         Dmi::Csr::TINFO,          Dmi::Csr::MCONTEXT,
  Dmi::Csr::SCONTEXT,       Dmi::Csr::DCSR,           Dmi::Csr::DPC,
  Dmi::Csr::DSCRATCH0,      Dmi::Csr::DSCRATCH1,      Dmi::Csr::MCYCLE,
  Dmi::Csr::MINSTRET,       Dmi::Csr::MHPMCOUNTER3,   Dmi::Csr::MHPMCOUNTER4,
  Dmi::Csr::MHPMCOUNTER5,   Dmi::Csr::MHPMCOUNTER6,   Dmi::Csr::MHPMCOUNTER7,
  Dmi::Csr::MHPMCOUNTER8,   Dmi::Csr::MHPMCOUNTER9,   Dmi::Csr::MHPMCOUNTER10,
  Dmi::Csr::MHPMCOUNTER11,  Dmi::Csr::MHPMCOUNTER12,  Dmi::Csr::MHPMCOUNTER13,
  Dmi::Csr::MHPMCOUNTER14,  Dmi::Csr::MHPMCOUNTER15,  Dmi::Csr::MHPMCOUNTER16,
  Dmi::Csr::MHPMCOUNTER17,  Dmi::Csr::MHPMCOUNTER18,  Dmi::Csr::MHPMCOUNTER19,
  Dmi::Csr::MHPMCOUNTER20,  Dmi::Csr::MHPMCOUNTER21,  Dmi::Csr::MHPMCOUNTER22,
  Dmi::Csr::MHPMCOUNTER23,  Dmi::Csr::MHPMCOUNTER24,  Dmi::Csr::MHPMCOUNTER25,
  Dmi::Csr::MHPMCOUNTER26,  Dmi::Csr::MHPMCOUNTER27,  Dmi::Csr::MHPMCOUNTER28,
  Dmi::Csr::MHPMCOUNTER29,  Dmi::Csr::MHPMCOUNTER30,  Dmi::Csr::MHPMCOUNTER31,
  Dmi::Csr::MCYCLEH,        Dmi::Csr::MINSTRETH,      Dmi::Csr::MHPMCOUNTERH3,
  Dmi::Csr::MHPMCOUNTERH4,  Dmi::Csr::MHPMCOUNTERH5,  Dmi::Csr::MHPMCOUNTERH6,
  Dmi::Csr::MHPMCOUNTERH7,  Dmi::Csr::MHPMCOUNTERH8,  Dmi::Csr::MHPMCOUNTERH9,
  Dmi::Csr::MHPMCOUNTERH10, Dmi::Csr::MHPMCOUNTERH11, Dmi::Csr::MHPMCOUNTERH12,
  Dmi::Csr::MHPMCOUNTERH13, Dmi::Csr::MHPMCOUNTERH14, Dmi::Csr::MHPMCOUNTERH15,
  Dmi::Csr::MHPMCOUNTERH16, Dmi::Csr::MHPMCOUNTERH17, Dmi::Csr::MHPMCOUNTERH18,
  Dmi::Csr::MHPMCOUNTERH19, Dmi::Csr::MHPMCOUNTERH20, Dmi::Csr::MHPMCOUNTERH21,
  Dmi::Csr::MHPMCOUNTERH22, Dmi::Csr::MHPMCOUNTERH23, Dmi::Csr::MHPMCOUNTERH24,
  Dmi::Csr::MHPMCOUNTERH25, Dmi::Csr::MHPMCOUNTERH26, Dmi::Csr::MHPMCOUNTERH27,
  Dmi::Csr::MHPMCOUNTERH28, Dmi::Csr::MHPMCOUNTERH29, Dmi::Csr::MHPMCOUNTERH30,
  Dmi::Csr::MHPMCOUNTERH31, Dmi::Csr::MVENDORID,      Dmi::Csr::MARCHID,
  Dmi::Csr::MIMPID,         Dmi::Csr::MHARTID
};

static const char *
abiName (uint8_t regno)
{
  static const char *regname[] = {
    "zero", "ra",  "sp",   "gp",   "tp",  "t0",  "t1",  "t2",  "s0/fp", "s1",
    "a0",   "a1",  "a2",   "a2",   "a4",  "a5",  "a6",  "a7",  "s2",    "s3",
    "s4",   "s5",  "s6",   "s7",   "s8",  "s9",  "s10", "s11", "t3",    "t4",
    "t5",   "t6",  "ft0",  "ft1",  "ft2", "ft3", "ft4", "ft5", "ft6",   "ft6",
    "fs0",  "fs1", "fa0",  "fa1",  "fa2", "fa3", "fa4", "fa5", "fa6",   "fa7",
    "fs2",  "fs3", "fs4",  "fs5",  "fs6", "fs7", "fs8", "fs9", "fs10",  "fs11",
    "ft8",  "ft9", "ft10", "ft11",
  };

  return (regno < 64) ? regname[regno] : "(unknown)";
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

  // Explore the target
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

  cout << "Num HARTS: " << numHarts << endl << endl;

  // Read hartinfo and abstractcs for each hart.
  unique_ptr<Dmi::Hartinfo> &hartinfo = dmi->hartinfo ();
  unique_ptr<Dmi::Abstractcs> &abstractcs = dmi->abstractcs ();

  for (uint32_t h = 0; h < numHarts; h++)
    {
      cout << "Hart " << h << endl;
      dmi->selectHart (h);

      hartinfo->read ();
      hartinfo->prettyPrint (false);
      cout << "  hartinfo: 0x" << hartinfo;
      hartinfo->prettyPrint (true);
      cout << " " << hartinfo << endl;

      abstractcs->read ();
      abstractcs->prettyPrint (false);
      cout << "  abstractcs: 0x" << abstractcs;
      abstractcs->prettyPrint (true);
      cout << " " << abstractcs << endl << endl;
    }

  // Test accessing the first Hart. First halt it.
  dmi->haltHart (0);

  dmstatus->read ();
  if (dmstatus->halted ())
    cout << "Hart 0 halted" << endl;
  else
    {
      dmstatus->prettyPrint (false);
      cout << "Hart 0 failed to halt, dmstatus: 0x" << dmstatus;
      dmstatus->prettyPrint (true);
      cout << " " << dmstatus << endl << endl;
    }

  // PC
  cout << "PC = 0x" << Utils::hexStr (dmi->readCsr (Dmi::Csr::DPC)) << endl
       << endl;

  // Read GPRs
  for (size_t r = 0; r < 0x20; r++)
    cout << "GPR " << abiName (r) << " (x" << r << ") = 0x"
         << Utils::hexStr (dmi->readGpr (r)) << endl;
  cout << endl;

  // Writing GPRs and reading back
  for (size_t r = 0; r < 0x20; r++)
    {
      dmi->writeGpr (r, 0xdeadbeef);
      cout << "GPR " << abiName (r) << " (x" << r << ") = 0x"
           << Utils::hexStr (dmi->readGpr (r));
      dmi->writeGpr (r, 0x00000000);
      cout << ", 0x" << Utils::hexStr (dmi->readGpr (r));
      dmi->writeGpr (r, 0xffffffff);
      cout << ", 0x" << Utils::hexStr (dmi->readGpr (r)) << endl;
    }
  cout << endl;

  // FPRs (for now disabled, should be based on MISA indication of FPU
  // availability).
  if (false)
    {
      for (size_t r = 0; r < 0x20; r++)
        {
          cout << "FPR " << abiName (0x20 + r) << " (f" << r << ") = 0x"
               << Utils::hexStr (dmi->readFpr (r)) << endl;
        }
      cout << endl;
    }

  // Standard user CSRs
  for (auto it = userCsrList.begin (); it != userCsrList.end (); it++)
    if (dmi->csrType (*it) != Dmi::FP)
      {
        uint32_t val = dmi->readCsr (*it);
        if (val != 0)
          cout << "Standard user CSR " << dmi->csrName (*it) << " (0x"
               << Utils::hexStr (*it, 3) << ") = 0x" << Utils::hexStr (val)
               << endl;
      }
  cout << endl;

  // Custom user CSRs
  for (auto it = customUserCsrList.begin (); it != customUserCsrList.end ();
       it++)
    if (dmi->csrType (*it) != Dmi::FP)
      {
        uint32_t val = dmi->readCsr (*it);
        if (val != 0)
          cout << "Custom user CSR " << dmi->csrName (*it) << " (0x"
               << Utils::hexStr (*it, 3) << ") = 0x" << Utils::hexStr (val)
               << endl;
      }
  cout << endl;

  // Standard machine CSRs
  for (auto it = machineCsrList.begin (); it != machineCsrList.end (); it++)
    if (dmi->csrType (*it) != Dmi::FP)
      {
        uint32_t val = dmi->readCsr (*it);
        if (val != 0)
          cout << "Standard machine CSR " << dmi->csrName (*it) << " (0x"
               << Utils::hexStr (*it, 3) << ") = 0x" << Utils::hexStr (val)
               << endl;
      }
  cout << endl;

  // Delete the DMI, and hence DTM and TAP, which will save the VCD
  dmi.reset (nullptr);
  return EXIT_SUCCESS;
}
