// Declaration of the interface to the Debug Transport Module
//
// This file is part of the Embecosm Debug Server target for CORE-V MCU
//
// Copyright (C) 2021 Embecosm Limited
// SPDX-License-Identifier: Apache-2.0

#ifndef IDTM_H
#define IDTM_H

#include <cstdint>

/// \brief Abstract class for a Debug Transport Module
///
/// This must be subclassed by an actual implementation such as JTAG or USB.
class IDtm
{
public:

  // Registers
  enum DmiRegs
    {
     DMCONTROL = 0x10,
     DMSTATUS = 0x11,
    };

  // Flag bits
  enum Flags
    {
     // dmstatus flags
     IMPEBREAK       = 0x00400000,
     ALLHAVERESET    = 0x00080000,
     ANYHAVERESET    = 0x00040000,
     ALLRESUMEACK    = 0x00020000,
     ANYRESUMEACK    = 0x00010000,
     ALLNONEXISTENT  = 0x00008000,
     ANYNONEXISTENT  = 0x00004000,
     ALLUNAVAIL      = 0x00002000,
     ANYUNAVAIL      = 0x00001000,
     ALLRUNNING      = 0x00000800,
     ANYRUNNING      = 0x00000400,
     ALLHALTED       = 0x00000200,
     ANYHALTED       = 0x00000100,
     AUTHENTICATED   = 0x00000080,
     AUTHBUSY        = 0x00000040,
     HASRESETHALTREQ = 0x00000020,
     CONFSTRPTRVALID = 0x00000010,
     VERSION         = 0x0000000f,

     // dmcontrol flags
     HALTREQ         = 0x80000000,
     RESUMEREQ       = 0x40000000,
     HARTRESET       = 0x20000000,
     ACKHAVERESET    = 0x10000000,
     HASEL           = 0x04000000,
     HARTSELLO       = 0x03ff0000,
     HARTSELHI       = 0x0000ffc0,
     SETRESETHALTREQ = 0x00000008,
     CLRRESETHALTREQ = 0x00000004,
     NDMRESET        = 0x00000002,
     DMACTIVE        = 0x00000001,
    };

  // Constructor and destructor
  IDtm () = default;
  IDtm (const IDtm&) = delete;
  virtual ~IDtm () = default;

  // API
  virtual bool reset () = 0;
  virtual uint32_t dmiRead (uint64_t address) = 0;
  virtual uint32_t dmiWrite (uint64_t address, uint32_t wdata) = 0;

  // Delete the copy assignment operator
  IDtm& operator=(const IDtm&) = delete;

private:

};

#endif	// IDTM_H
