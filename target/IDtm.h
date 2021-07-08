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

  // Masks for flag bits
  enum Masks
    {
     // dmstatus masks
     IMPEBREAK_MASK       = 0x00400000,
     ALLHAVERESET_MASK    = 0x00080000,
     ANYHAVERESET_MASK    = 0x00040000,
     ALLRESUMEACK_MASK    = 0x00020000,
     ANYRESUMEACK_MASK    = 0x00010000,
     ALLNONEXISTENT_MASK  = 0x00008000,
     ANYNONEXISTENT_MASK  = 0x00004000,
     ALLUNAVAIL_MASK      = 0x00002000,
     ANYUNAVAIL_MASK      = 0x00001000,
     ALLRUNNING_MASK      = 0x00000800,
     ANYRUNNING_MASK      = 0x00000400,
     ALLHALTED_MASK       = 0x00000200,
     ANYHALTED_MASK       = 0x00000100,
     AUTHENTICATED_MASK   = 0x00000080,
     AUTHBUSY_MASK        = 0x00000040,
     HASRESETHALTREQ_MASK = 0x00000020,
     CONFSTRPTRVALID_MASK = 0x00000010,
     VERSION_MASK         = 0x0000000f,

     // dmcontrol masks
     HALTREQ_MASK         = 0x80000000,
     RESUMEREQ_MASK       = 0x40000000,
     HARTRESET_MASK       = 0x20000000,
     ACKHAVERESET_MASK    = 0x10000000,
     HASEL_MASK           = 0x04000000,
     HARTSELLO_MASK       = 0x03ff0000,
     HARTSELHI_MASK       = 0x0000ffc0,
     SETRESETHALTREQ_MASK = 0x00000008,
     CLRRESETHALTREQ_MASK = 0x00000004,
     NDMRESET_MASK        = 0x00000002,
     DMACTIVE_MASK        = 0x00000001,
    };

  // Offsets for flags (i.e. how much do I shift right to get the mask into
  // the LS bits of the word).
  enum Offsets
    {
     // dmstatus offsets
     IMPEBREAK_OFFSET       = 22,
     ALLHAVERESET_OFFSET    = 19,
     ANYHAVERESET_OFFSET    = 18,
     ALLRESUMEACK_OFFSET    = 17,
     ANYRESUMEACK_OFFSET    = 16,
     ALLNONEXISTENT_OFFSET  = 15,
     ANYNONEXISTENT_OFFSET  = 14,
     ALLUNAVAIL_OFFSET      = 13,
     ANYUNAVAIL_OFFSET      = 12,
     ALLRUNNING_OFFSET      = 11,
     ANYRUNNING_OFFSET      = 10,
     ALLHALTED_OFFSET       =  9,
     ANYHALTED_OFFSET       =  8,
     AUTHENTICATED_OFFSET   =  7,
     AUTHBUSY_OFFSET        =  6,
     HASRESETHALTREQ_OFFSET =  5,
     CONFSTRPTRVALID_OFFSET =  4,
     VERSION_OFFSET         =  0,

     // dmcontrol offsets
     HALTREQ_OFFSET         = 31,
     RESUMEREQ_OFFSET       = 30,
     HARTRESET_OFFSET       = 29,
     ACKHAVERESET_OFFSET    = 28,
     HASEL_OFFSET           = 26,
     HARTSELLO_OFFSET       = 16,
     HARTSELHI_OFFSET       =  6,
     SETRESETHALTREQ_OFFSET =  3,
     CLRRESETHALTREQ_OFFSET =  2,
     NDMRESET_OFFSET        =  1,
     DMACTIVE_OFFSET        =  0,
    };

  // Sizes of flags (i.e. how many bits are set in the flag).
  enum Sizes
    {
     // dmstatus sizes
     IMPEBREAK_SIZE       = 1,
     ALLHAVERESET_SIZE    = 1,
     ANYHAVERESET_SIZE    = 1,
     ALLRESUMEACK_SIZE    = 1,
     ANYRESUMEACK_SIZE    = 1,
     ALLNONEXISTENT_SIZE  = 1,
     ANYNONEXISTENT_SIZE  = 1,
     ALLUNAVAIL_SIZE      = 1,
     ANYUNAVAIL_SIZE      = 1,
     ALLRUNNING_SIZE      = 1,
     ANYRUNNING_SIZE      = 1,
     ALLHALTED_SIZE       = 1,
     ANYHALTED_SIZE       = 1,
     AUTHENTICATED_SIZE   = 1,
     AUTHBUSY_SIZE        = 1,
     HASRESETHALTREQ_SIZE = 1,
     CONFSTRPTRVALID_SIZE = 1,
     VERSION_SIZE         = 4,

     // dmcontrol sizes
     HALTREQ_SIZE         =  1,
     RESUMEREQ_SIZE       =  1,
     HARTRESET_SIZE       =  1,
     ACKHAVERESET_SIZE    =  1,
     HASEL_SIZE           =  1,
     HARTSELLO_SIZE       = 10,
     HARTSELHI_SIZE       = 10,
     SETRESETHALTREQ_SIZE =  1,
     CLRRESETHALTREQ_SIZE =  1,
     NDMRESET_SIZE        =  1,
     DMACTIVE_SIZE        =  1,
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
