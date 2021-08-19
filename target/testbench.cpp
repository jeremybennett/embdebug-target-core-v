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

/// \brief Enumeration of the CSRs

enum Csrs
{
  FFLAGS = 0x1,
  FRM = 0x2,
  FCSR = 0x3,
  CYCLE = 0xc00,
  INSTRET = 0xc02,
  HPMCOUNTER3 = 0xc03,
  HPMCOUNTER4 = 0xc04,
  HPMCOUNTER5 = 0xc05,
  HPMCOUNTER6 = 0xc06,
  HPMCOUNTER7 = 0xc07,
  HPMCOUNTER8 = 0xc08,
  HPMCOUNTER9 = 0xc09,
  HPMCOUNTER10 = 0xc0a,
  HPMCOUNTER11 = 0xc0b,
  HPMCOUNTER12 = 0xc0c,
  HPMCOUNTER13 = 0xc0d,
  HPMCOUNTER14 = 0xc0e,
  HPMCOUNTER15 = 0xc0f,
  HPMCOUNTER16 = 0xc10,
  HPMCOUNTER17 = 0xc11,
  HPMCOUNTER18 = 0xc12,
  HPMCOUNTER19 = 0xc13,
  HPMCOUNTER20 = 0xc14,
  HPMCOUNTER21 = 0xc15,
  HPMCOUNTER22 = 0xc16,
  HPMCOUNTER23 = 0xc17,
  HPMCOUNTER24 = 0xc18,
  HPMCOUNTER25 = 0xc19,
  HPMCOUNTER26 = 0xc1a,
  HPMCOUNTER27 = 0xc1b,
  HPMCOUNTER28 = 0xc1c,
  HPMCOUNTER29 = 0xc1d,
  HPMCOUNTER30 = 0xc1e,
  HPMCOUNTER31 = 0xc1f,
  CYCLEH = 0xc80,
  INSTRETH = 0xc82,
  HPMCOUNTERH3 = 0xc83,
  HPMCOUNTERH4 = 0xc84,
  HPMCOUNTERH5 = 0xc85,
  HPMCOUNTERH6 = 0xc86,
  HPMCOUNTERH7 = 0xc87,
  HPMCOUNTERH8 = 0xc88,
  HPMCOUNTERH9 = 0xc89,
  HPMCOUNTERH10 = 0xc8a,
  HPMCOUNTERH11 = 0xc8b,
  HPMCOUNTERH12 = 0xc8c,
  HPMCOUNTERH13 = 0xc8d,
  HPMCOUNTERH14 = 0xc8e,
  HPMCOUNTERH15 = 0xc8f,
  HPMCOUNTERH16 = 0xc90,
  HPMCOUNTERH17 = 0xc91,
  HPMCOUNTERH18 = 0xc92,
  HPMCOUNTERH19 = 0xc93,
  HPMCOUNTERH20 = 0xc94,
  HPMCOUNTERH21 = 0xc95,
  HPMCOUNTERH22 = 0xc96,
  HPMCOUNTERH23 = 0xc97,
  HPMCOUNTERH24 = 0xc98,
  HPMCOUNTERH25 = 0xc99,
  HPMCOUNTERH26 = 0xc9a,
  HPMCOUNTERH27 = 0xc9b,
  HPMCOUNTERH28 = 0xc9c,
  HPMCOUNTERH29 = 0xc9d,
  HPMCOUNTERH30 = 0xc9e,
  HPMCOUNTERH31 = 0xc9f,
  LPSTART0 = 0x800,
  LPEND0 = 0x801,
  LPCOUNT0 = 0x802,
  LPSTART1 = 0x804,
  LPEND1 = 0x805,
  LPCOUNT1 = 0x806,
  UHARTID = 0xcc0,
  PRIVLV = 0xcc1,
  MSTATUS = 0x300,
  MISA = 0x301,
  MIE = 0x304,
  MTVEC = 0x305,
  MCOUNTINHIBIT = 0x320,
  MHPMEVENT3 = 0x323,
  MHPMEVENT4 = 0x324,
  MHPMEVENT5 = 0x325,
  MHPMEVENT6 = 0x326,
  MHPMEVENT7 = 0x327,
  MHPMEVENT8 = 0x328,
  MHPMEVENT9 = 0x329,
  MHPMEVENT10 = 0x32a,
  MHPMEVENT11 = 0x32b,
  MHPMEVENT12 = 0x32c,
  MHPMEVENT13 = 0x32d,
  MHPMEVENT14 = 0x32e,
  MHPMEVENT15 = 0x32f,
  MHPMEVENT16 = 0x330,
  MHPMEVENT17 = 0x331,
  MHPMEVENT18 = 0x332,
  MHPMEVENT19 = 0x333,
  MHPMEVENT20 = 0x334,
  MHPMEVENT21 = 0x335,
  MHPMEVENT22 = 0x336,
  MHPMEVENT23 = 0x337,
  MHPMEVENT24 = 0x338,
  MHPMEVENT25 = 0x339,
  MHPMEVENT26 = 0x33a,
  MHPMEVENT27 = 0x33b,
  MHPMEVENT28 = 0x33c,
  MHPMEVENT29 = 0x33d,
  MHPMEVENT30 = 0x33e,
  MHPMEVENT31 = 0x33f,
  MSCRATCH = 0x340,
  MEPC = 0x341,
  MCAUSE = 0x342,
  MTVAL = 0x343,
  MIP = 0x344,
  TSELECT = 0x7a0,
  TDATA1 = 0x7a1,
  TDATA2 = 0x7a2,
  TDATA3 = 0x7a3,
  TINFO = 0x7a4,
  MCONTEXT = 0x7a8,
  SCONTEXT = 0x7aa,
  DCSR = 0x7b0,
  DPC = 0x7b1,
  DSCRATCH0 = 0x7b2,
  DSCRATCH1 = 0x7b3,
  MCYCLE = 0xb00,
  MINSTRET = 0xb02,
  MHPMCOUNTER3 = 0xb03,
  MHPMCOUNTER4 = 0xb04,
  MHPMCOUNTER5 = 0xb05,
  MHPMCOUNTER6 = 0xb06,
  MHPMCOUNTER7 = 0xb07,
  MHPMCOUNTER8 = 0xb08,
  MHPMCOUNTER9 = 0xb09,
  MHPMCOUNTER10 = 0xb0a,
  MHPMCOUNTER11 = 0xb0b,
  MHPMCOUNTER12 = 0xb0c,
  MHPMCOUNTER13 = 0xb0d,
  MHPMCOUNTER14 = 0xb0e,
  MHPMCOUNTER15 = 0xb0f,
  MHPMCOUNTER16 = 0xb10,
  MHPMCOUNTER17 = 0xb11,
  MHPMCOUNTER18 = 0xb12,
  MHPMCOUNTER19 = 0xb13,
  MHPMCOUNTER20 = 0xb14,
  MHPMCOUNTER21 = 0xb15,
  MHPMCOUNTER22 = 0xb16,
  MHPMCOUNTER23 = 0xb17,
  MHPMCOUNTER24 = 0xb18,
  MHPMCOUNTER25 = 0xb19,
  MHPMCOUNTER26 = 0xb1a,
  MHPMCOUNTER27 = 0xb1b,
  MHPMCOUNTER28 = 0xb1c,
  MHPMCOUNTER29 = 0xb1d,
  MHPMCOUNTER30 = 0xb1e,
  MHPMCOUNTER31 = 0xb1f,
  MCYCLEH = 0xb80,
  MINSTRETH = 0xb82,
  MHPMCOUNTERH3 = 0xb83,
  MHPMCOUNTERH4 = 0xb84,
  MHPMCOUNTERH5 = 0xb85,
  MHPMCOUNTERH6 = 0xb86,
  MHPMCOUNTERH7 = 0xb87,
  MHPMCOUNTERH8 = 0xb88,
  MHPMCOUNTERH9 = 0xb89,
  MHPMCOUNTERH10 = 0xb8a,
  MHPMCOUNTERH11 = 0xb8b,
  MHPMCOUNTERH12 = 0xb8c,
  MHPMCOUNTERH13 = 0xb8d,
  MHPMCOUNTERH14 = 0xb8e,
  MHPMCOUNTERH15 = 0xb8f,
  MHPMCOUNTERH16 = 0xb90,
  MHPMCOUNTERH17 = 0xb91,
  MHPMCOUNTERH18 = 0xb92,
  MHPMCOUNTERH19 = 0xb93,
  MHPMCOUNTERH20 = 0xb94,
  MHPMCOUNTERH21 = 0xb95,
  MHPMCOUNTERH22 = 0xb96,
  MHPMCOUNTERH23 = 0xb97,
  MHPMCOUNTERH24 = 0xb98,
  MHPMCOUNTERH25 = 0xb99,
  MHPMCOUNTERH26 = 0xb9a,
  MHPMCOUNTERH27 = 0xb9b,
  MHPMCOUNTERH28 = 0xb9c,
  MHPMCOUNTERH29 = 0xb9d,
  MHPMCOUNTERH30 = 0xb9e,
  MHPMCOUNTERH31 = 0xb9f,
  MVENDORID = 0xf11,
  MARCHID = 0xf12,
  MIMPID = 0xf13,
  MHARTID = 0xf14,
};

/// \bried An enumeration of the groups of CPUs
enum CsrType
{
  ANY,  ///< All configurations
  FP,   ///< Only if FPU is present
  HWLP, ///< Only if hardware loop is present
};

/// \brief A structure representing CSRs
struct CsrId
{
  const char *name;    ///< The printable name of the CSR
  const uint16_t addr; ///< The address of the CSR
  const bool readOnly; ///< True if the CSR is read only
  const CsrType type;  ///< Which CSR group
};

/// \brief Base address of the GPRs when reading/writing
static const uint16_t GPR_BASE = 0x1000;

/// \brief Base address of the FPRs when reading/writing
static const uint16_t FPR_BASE = 0x1000;

/// \brief A vector of all the standard user CSRs
static const CsrId userCsrList[]
    = { { "fflags", FFLAGS, false, FP },
        { "frm", FRM, false, FP },
        { "fcsr", FCSR, false, FP },
        { "cycle", CYCLE, true, ANY },
        { "instret", INSTRET, true, ANY },
        { "hpmcounter3", HPMCOUNTER3, true, ANY },
        { "hpmcounter4", HPMCOUNTER4, true, ANY },
        { "hpmcounter5", HPMCOUNTER5, true, ANY },
        { "hpmcounter6", HPMCOUNTER6, true, ANY },
        { "hpmcounter7", HPMCOUNTER7, true, ANY },
        { "hpmcounter8", HPMCOUNTER8, true, ANY },
        { "hpmcounter9", HPMCOUNTER9, true, ANY },
        { "hpmcounter10", HPMCOUNTER10, true, ANY },
        { "hpmcounter11", HPMCOUNTER11, true, ANY },
        { "hpmcounter12", HPMCOUNTER12, true, ANY },
        { "hpmcounter13", HPMCOUNTER13, true, ANY },
        { "hpmcounter14", HPMCOUNTER14, true, ANY },
        { "hpmcounter15", HPMCOUNTER15, true, ANY },
        { "hpmcounter16", HPMCOUNTER16, true, ANY },
        { "hpmcounter17", HPMCOUNTER17, true, ANY },
        { "hpmcounter18", HPMCOUNTER18, true, ANY },
        { "hpmcounter19", HPMCOUNTER19, true, ANY },
        { "hpmcounter20", HPMCOUNTER20, true, ANY },
        { "hpmcounter21", HPMCOUNTER21, true, ANY },
        { "hpmcounter22", HPMCOUNTER22, true, ANY },
        { "hpmcounter23", HPMCOUNTER23, true, ANY },
        { "hpmcounter24", HPMCOUNTER24, true, ANY },
        { "hpmcounter25", HPMCOUNTER25, true, ANY },
        { "hpmcounter26", HPMCOUNTER26, true, ANY },
        { "hpmcounter27", HPMCOUNTER27, true, ANY },
        { "hpmcounter28", HPMCOUNTER28, true, ANY },
        { "hpmcounter29", HPMCOUNTER29, true, ANY },
        { "hpmcounter30", HPMCOUNTER30, true, ANY },
        { "hpmcounter31", HPMCOUNTER31, true, ANY },
        { "cycleh", CYCLEH, true, ANY },
        { "instreth", INSTRETH, true, ANY },
        { "hpmcounterh3", HPMCOUNTERH3, true, ANY },
        { "hpmcounterh4", HPMCOUNTERH4, true, ANY },
        { "hpmcounterh5", HPMCOUNTERH5, true, ANY },
        { "hpmcounterh6", HPMCOUNTERH6, true, ANY },
        { "hpmcounterh7", HPMCOUNTERH7, true, ANY },
        { "hpmcounterh8", HPMCOUNTERH8, true, ANY },
        { "hpmcounterh9", HPMCOUNTERH9, true, ANY },
        { "hpmcounterh10", HPMCOUNTERH10, true, ANY },
        { "hpmcounterh11", HPMCOUNTERH11, true, ANY },
        { "hpmcounterh12", HPMCOUNTERH12, true, ANY },
        { "hpmcounterh13", HPMCOUNTERH13, true, ANY },
        { "hpmcounterh14", HPMCOUNTERH14, true, ANY },
        { "hpmcounterh15", HPMCOUNTERH15, true, ANY },
        { "hpmcounterh16", HPMCOUNTERH16, true, ANY },
        { "hpmcounterh17", HPMCOUNTERH17, true, ANY },
        { "hpmcounterh18", HPMCOUNTERH18, true, ANY },
        { "hpmcounterh19", HPMCOUNTERH19, true, ANY },
        { "hpmcounterh20", HPMCOUNTERH20, true, ANY },
        { "hpmcounterh21", HPMCOUNTERH21, true, ANY },
        { "hpmcounterh22", HPMCOUNTERH22, true, ANY },
        { "hpmcounterh23", HPMCOUNTERH23, true, ANY },
        { "hpmcounterh24", HPMCOUNTERH24, true, ANY },
        { "hpmcounterh25", HPMCOUNTERH25, true, ANY },
        { "hpmcounterh26", HPMCOUNTERH26, true, ANY },
        { "hpmcounterh27", HPMCOUNTERH27, true, ANY },
        { "hpmcounterh28", HPMCOUNTERH28, true, ANY },
        { "hpmcounterh29", HPMCOUNTERH29, true, ANY },
        { "hpmcounterh30", HPMCOUNTERH30, true, ANY },
        { "hpmcounterh31", HPMCOUNTERH31, true, ANY } };

/// \brief A vector of all the custom user CSRs
static const CsrId userCustomCsrList[] = {
  { "lpstart0", LPSTART0, false, HWLP }, { "lpend0", LPEND0, false, HWLP },
  { "lpcount0", LPCOUNT0, false, HWLP }, { "lpstart1", LPSTART1, false, HWLP },
  { "lpend1", LPEND1, false, HWLP },     { "lpcount1", LPCOUNT1, false, HWLP },
  { "uhartid", UHARTID, true, ANY },     { "privlv", PRIVLV, true, ANY }
};

/// \brief A vector of all the standard machine CSRs
static const CsrId machineCsrList[]
    = { { "mstatus", MSTATUS, false, ANY },
        { "misa", MISA, false, ANY },
        { "mie", MIE, false, ANY },
        { "mtvec", MTVEC, false, ANY },
        { "mcountinhibit", MCOUNTINHIBIT, false, ANY },
        { "mhpmevent3", MHPMEVENT3, false, ANY },
        { "mhpmevent4", MHPMEVENT4, false, ANY },
        { "mhpmevent5", MHPMEVENT5, false, ANY },
        { "mhpmevent6", MHPMEVENT6, false, ANY },
        { "mhpmevent7", MHPMEVENT7, false, ANY },
        { "mhpmevent8", MHPMEVENT8, false, ANY },
        { "mhpmevent9", MHPMEVENT9, false, ANY },
        { "mhpmevent10", MHPMEVENT10, false, ANY },
        { "mhpmevent11", MHPMEVENT11, false, ANY },
        { "mhpmevent12", MHPMEVENT12, false, ANY },
        { "mhpmevent13", MHPMEVENT13, false, ANY },
        { "mhpmevent14", MHPMEVENT14, false, ANY },
        { "mhpmevent15", MHPMEVENT15, false, ANY },
        { "mhpmevent16", MHPMEVENT16, false, ANY },
        { "mhpmevent17", MHPMEVENT17, false, ANY },
        { "mhpmevent18", MHPMEVENT18, false, ANY },
        { "mhpmevent19", MHPMEVENT19, false, ANY },
        { "mhpmevent20", MHPMEVENT20, false, ANY },
        { "mhpmevent21", MHPMEVENT21, false, ANY },
        { "mhpmevent22", MHPMEVENT22, false, ANY },
        { "mhpmevent23", MHPMEVENT23, false, ANY },
        { "mhpmevent24", MHPMEVENT24, false, ANY },
        { "mhpmevent25", MHPMEVENT25, false, ANY },
        { "mhpmevent26", MHPMEVENT26, false, ANY },
        { "mhpmevent27", MHPMEVENT27, false, ANY },
        { "mhpmevent28", MHPMEVENT28, false, ANY },
        { "mhpmevent29", MHPMEVENT29, false, ANY },
        { "mhpmevent30", MHPMEVENT30, false, ANY },
        { "mhpmevent31", MHPMEVENT31, false, ANY },
        { "mscratch", MSCRATCH, false, ANY },
        { "mepc", MEPC, false, ANY },
        { "mcause", MCAUSE, false, ANY },
        { "mtval", MTVAL, false, ANY },
        { "mip", MIP, false, ANY },
        { "tselect", TSELECT, false, ANY },
        { "tdata1", TDATA1, false, ANY },
        { "tdata2", TDATA2, false, ANY },
        { "tdata3", TDATA3, false, ANY },
        { "tinfo", TINFO, true, ANY },
        { "mcontext", MCONTEXT, false, ANY },
        { "scontext", SCONTEXT, false, ANY },
        { "dcsr", DCSR, false, ANY },
        { "dpc", DPC, false, ANY },
        { "dscratch0", DSCRATCH0, false, ANY },
        { "dscratch1", DSCRATCH1, false, ANY },
        { "mcycle", MCYCLE, false, ANY },
        { "minstret", MINSTRET, false, ANY },
        { "mhpmcounter3", MHPMCOUNTER3, false, ANY },
        { "mhpmcounter4", MHPMCOUNTER4, false, ANY },
        { "mhpmcounter5", MHPMCOUNTER5, false, ANY },
        { "mhpmcounter6", MHPMCOUNTER6, false, ANY },
        { "mhpmcounter7", MHPMCOUNTER7, false, ANY },
        { "mhpmcounter8", MHPMCOUNTER8, false, ANY },
        { "mhpmcounter9", MHPMCOUNTER9, false, ANY },
        { "mhpmcounter10", MHPMCOUNTER10, false, ANY },
        { "mhpmcounter11", MHPMCOUNTER11, false, ANY },
        { "mhpmcounter12", MHPMCOUNTER12, false, ANY },
        { "mhpmcounter13", MHPMCOUNTER13, false, ANY },
        { "mhpmcounter14", MHPMCOUNTER14, false, ANY },
        { "mhpmcounter15", MHPMCOUNTER15, false, ANY },
        { "mhpmcounter16", MHPMCOUNTER16, false, ANY },
        { "mhpmcounter17", MHPMCOUNTER17, false, ANY },
        { "mhpmcounter18", MHPMCOUNTER18, false, ANY },
        { "mhpmcounter19", MHPMCOUNTER19, false, ANY },
        { "mhpmcounter20", MHPMCOUNTER20, false, ANY },
        { "mhpmcounter21", MHPMCOUNTER21, false, ANY },
        { "mhpmcounter22", MHPMCOUNTER22, false, ANY },
        { "mhpmcounter23", MHPMCOUNTER23, false, ANY },
        { "mhpmcounter24", MHPMCOUNTER24, false, ANY },
        { "mhpmcounter25", MHPMCOUNTER25, false, ANY },
        { "mhpmcounter26", MHPMCOUNTER26, false, ANY },
        { "mhpmcounter27", MHPMCOUNTER27, false, ANY },
        { "mhpmcounter28", MHPMCOUNTER28, false, ANY },
        { "mhpmcounter29", MHPMCOUNTER29, false, ANY },
        { "mhpmcounter30", MHPMCOUNTER30, false, ANY },
        { "mhpmcounter31", MHPMCOUNTER31, false, ANY },
        { "mcycleh", MCYCLEH, false, ANY },
        { "minstreth", MINSTRETH, false, ANY },
        { "mhpmcounterh3", MHPMCOUNTERH3, false, ANY },
        { "mhpmcounterh4", MHPMCOUNTERH4, false, ANY },
        { "mhpmcounterh5", MHPMCOUNTERH5, false, ANY },
        { "mhpmcounterh6", MHPMCOUNTERH6, false, ANY },
        { "mhpmcounterh7", MHPMCOUNTERH7, false, ANY },
        { "mhpmcounterh8", MHPMCOUNTERH8, false, ANY },
        { "mhpmcounterh9", MHPMCOUNTERH9, false, ANY },
        { "mhpmcounterh10", MHPMCOUNTERH10, false, ANY },
        { "mhpmcounterh11", MHPMCOUNTERH11, false, ANY },
        { "mhpmcounterh12", MHPMCOUNTERH12, false, ANY },
        { "mhpmcounterh13", MHPMCOUNTERH13, false, ANY },
        { "mhpmcounterh14", MHPMCOUNTERH14, false, ANY },
        { "mhpmcounterh15", MHPMCOUNTERH15, false, ANY },
        { "mhpmcounterh16", MHPMCOUNTERH16, false, ANY },
        { "mhpmcounterh17", MHPMCOUNTERH17, false, ANY },
        { "mhpmcounterh18", MHPMCOUNTERH18, false, ANY },
        { "mhpmcounterh19", MHPMCOUNTERH19, false, ANY },
        { "mhpmcounterh20", MHPMCOUNTERH20, false, ANY },
        { "mhpmcounterh21", MHPMCOUNTERH21, false, ANY },
        { "mhpmcounterh22", MHPMCOUNTERH22, false, ANY },
        { "mhpmcounterh23", MHPMCOUNTERH23, false, ANY },
        { "mhpmcounterh24", MHPMCOUNTERH24, false, ANY },
        { "mhpmcounterh25", MHPMCOUNTERH25, false, ANY },
        { "mhpmcounterh26", MHPMCOUNTERH26, false, ANY },
        { "mhpmcounterh27", MHPMCOUNTERH27, false, ANY },
        { "mhpmcounterh28", MHPMCOUNTERH28, false, ANY },
        { "mhpmcounterh29", MHPMCOUNTERH29, false, ANY },
        { "mhpmcounterh30", MHPMCOUNTERH30, false, ANY },
        { "mhpmcounterh31", MHPMCOUNTERH31, false, ANY },
        { "mvendorid", MVENDORID, true, ANY },
        { "marchid", MARCHID, true, ANY },
        { "mimpid", MIMPID, true, ANY },
        { "mhartid", MHARTID, true, ANY } };

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

/// \brief Read a CSR.
///
/// \param[in] dmi   The DMI to use.
/// \param[in] addr  Address of the CSR to read.
/// \return  The value of the CSR read.
static uint32_t
readCsr (unique_ptr<Dmi> &dmi, uint16_t addr)
{
  unique_ptr<Dmi::Command> &command = dmi->command ();
  unique_ptr<Dmi::Data> &data = dmi->data ();

  command->reset ();
  command->cmdtype (Dmi::Command::ACCESS_REG);
  command->aarsize (Dmi::Command::ACCESS32);
  command->aatransfer (true);
  command->aawrite (false);
  command->aaregno (addr);
  command->write ();

  data->read (0);
  return data->data (0);
};

/// \brief Write a CSR.
///
/// \note There is no check that the CSR is writable.
///
/// \param[in] dmi   The DMI to use.
/// \param[in] addr  Address of the CSR to write.
/// \param[in] val   The value to write to the CSR.
static void
writeCsr (unique_ptr<Dmi> &dmi, uint16_t addr, uint32_t val)
{
  unique_ptr<Dmi::Command> &command = dmi->command ();
  unique_ptr<Dmi::Data> &data = dmi->data ();

  data->reset (0);
  data->data (0, val);
  data->write (0);

  command->reset ();
  command->cmdtype (Dmi::Command::ACCESS_REG);
  command->aarsize (Dmi::Command::ACCESS32);
  command->aatransfer (true);
  command->aawrite (true);
  command->aaregno (addr);
  command->write ();
};

/// \brief Read a general purpose register
///
/// \param[in] dmi     The DMI to use.
/// \param[in] regNum  Number of the register to read.
/// \return  The value of the register read.
static uint32_t
readGpr (unique_ptr<Dmi> &dmi, size_t regNum)
{
  return readCsr (dmi, GPR_BASE + static_cast<uint16_t> (regNum));
}

/// \brief Write a general purpose register
///
/// \param[in] dmi     The DMI to use.
/// \param[in] regNum  Number of the register to write.
/// \param[in] val     The value to write to the register.
static void
writeGpr (unique_ptr<Dmi> &dmi, size_t regNum, uint32_t val)
{
  return writeCsr (dmi, GPR_BASE + static_cast<uint16_t> (regNum), val);
}

/// \brief Read a floating point register
///
/// \param[in] dmi     The DMI to use.
/// \param[in] regNum  Number of the register to read.
/// \return  The value of the register read.
static uint32_t
readFpr (unique_ptr<Dmi> &dmi, size_t regNum)
{
  return readCsr (dmi, FPR_BASE + static_cast<uint16_t> (regNum));
}

/// \brief Write a floating point register
///
/// \param[in] dmi     The DMI to use.
/// \param[in] regNum  Number of the register to write.
/// \param[in] val     The value to write to the register.
static void
writeFpr (unique_ptr<Dmi> &dmi, size_t regNum, uint32_t val)
{
  return writeCsr (dmi, FPR_BASE + static_cast<uint16_t> (regNum), val);
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
  unique_ptr<Dmi::Dmcontrol> &dmcontrol = dmi->dmcontrol ();
  unique_ptr<Dmi::Dmstatus> &dmstatus = dmi->dmstatus ();
  dmcontrol->reset ();
  dmcontrol->hartselMax ();
  dmcontrol->dmactive (true);
  dmcontrol->write ();
  dmcontrol->reset ();
  dmcontrol->read ();
  uint32_t hartselLen = dmcontrol->hartsel ();
  cout << "Max HARTS: 0x" << Utils::hexStr (hartselLen + 1) << endl;

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

  // Read hartinfo and abstractcs for each hart.
  unique_ptr<Dmi::Hartinfo> &hartinfo = dmi->hartinfo ();
  unique_ptr<Dmi::Abstractcs> &abstractcs = dmi->abstractcs ();

  for (uint32_t h = 0; h < numHarts; h++)
    {
      cout << "Hart " << h << endl;
      dmcontrol->reset ();
      dmcontrol->hartsel (h);
      dmcontrol->dmactive (true);
      dmcontrol->write ();

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
  dmcontrol->reset ();
  dmcontrol->haltreq (true);
  dmcontrol->hartsel (0);
  dmcontrol->dmactive (true);
  dmcontrol->write ();

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

  // Read registers of Hart 0.
  unique_ptr<Dmi::Command> &command = dmi->command ();
  unique_ptr<Dmi::Data> &data = dmi->data ();

  // PC
  cout << "PC = 0x" << Utils::hexStr (readCsr (dmi, DPC)) << endl << endl;

  // GPRs
  for (size_t r = 0; r < 0x20; r++)
    cout << "GPR " << abiName (r) << " (x" << r << ") = 0x"
         << Utils::hexStr (readGpr (dmi, r)) << endl;
  cout << endl;

  // Writing GPRs and reading back
  for (size_t r = 0; r < 0x20; r++)
    {
      writeGpr (dmi, r, 0xdeadbeef);
      cout << "GPR " << abiName (r) << " (x" << r << ") = 0x"
           << Utils::hexStr (readGpr (dmi, r));
      writeGpr (dmi, r, 0x00000000);
      cout << ", 0x" << Utils::hexStr (readGpr (dmi, r));
      writeGpr (dmi, r, 0xffffffff);
      cout << ", 0x" << Utils::hexStr (readGpr (dmi, r)) << endl;
    }

  // FPRs (for now disabled, should be based on MISA indication of FPU
  // availability).
  if (false)
    {
      for (size_t r = 0; r < 0x20; r++)
        {
          cout << "FPR " << abiName (0x20 + r) << " (f" << r << ") = 0x"
               << Utils::hexStr (readFpr (dmi, r)) << endl;
        }
      cout << endl;
    }

  // Standard user CSRs
  size_t numCsrs = sizeof (userCsrList) / sizeof (userCsrList[0]);

  for (size_t i = 0; i < numCsrs; i++)
    if (userCsrList[i].type == ANY)
      {
        uint32_t val = readCsr (dmi, userCsrList[i].addr);
        if (val != 0)
          cout << "Standard user CSR " << userCsrList[i].name << "(0x"
               << Utils::hexStr (userCsrList[i].addr, 3) << ") = 0x"
               << Utils::hexStr (val) << endl;
      }
  cout << endl;

  // Standard custom user CSRs
  numCsrs = sizeof (userCustomCsrList) / sizeof (userCustomCsrList[0]);

  for (size_t i = 0; i < numCsrs; i++)
    if (userCustomCsrList[i].type == ANY)
      {
        uint32_t val = readCsr (dmi, userCustomCsrList[i].addr);
        if (val != 0)
          cout << "Standard custom user CSR " << userCustomCsrList[i].name
               << "(0x" << Utils::hexStr (userCustomCsrList[i].addr, 3)
               << ") = 0x" << Utils::hexStr (val) << endl;
      }
  cout << endl;

  // Standard machine CSRs
  numCsrs = sizeof (machineCsrList) / sizeof (machineCsrList[0]);

  for (size_t i = 0; i < numCsrs; i++)
    if (machineCsrList[i].type == ANY)
      {
        uint32_t val = readCsr (dmi, machineCsrList[i].addr);
        if (val != 0)
          cout << "Standard machine CSR " << machineCsrList[i].name << "(0x"
               << Utils::hexStr (machineCsrList[i].addr, 3) << ") = 0x"
               << Utils::hexStr (val) << endl;
      }
  cout << endl;

  // Delete the DMI, and hence DTM and TAP, which will save the VCD
  dmi.reset (nullptr);
  return EXIT_SUCCESS;
}
