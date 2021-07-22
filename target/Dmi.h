// Declaration of the Debug Module Interface
//
// This file is part of the Embecosm Debug Server target for CORE-V MCU
//
// Copyright (C) 2021 Embecosm Limited
// SPDX-License-Identifier: Apache-2.0

#ifndef DMI_H
#define DMI_H

#include <cstdint>
#include <memory>

#include "IDtm.h"

/// \brief The class modeling the Debug Module Interface
///
/// This sits on top of the Debug Transport Module
///
/// Within this class we provide classes to represent each of the registers.
class Dmi
{
public:

  class Dmcontrol
  {
  public:

    // Constructors & destructor
    Dmcontrol () = delete;
    Dmcontrol (std::unique_ptr<IDtm> & dtm_);
    ~Dmcontrol () = default;

    // Delete copy operator
    Dmcontrol& operator=(const Dmcontrol&) = delete;

    // API
    void read ();
    void reset ();
    void write ();
    void haltreq (const bool flag);
    void resumereq ();
    bool hartreset () const;
    void hartreset (const bool flag);
    void ackhavereset ();
    bool hasel () const;
    void hasel (const bool flag);
    uint32_t hartsel () const;
    void hartsel (const uint32_t hartselVal);
    void hartselMax ();
    void setresethaltreq ();
    void clrresethaltreq ();
    bool ndmreset () const;
    void ndmreset (const bool flag);
    bool dmactive () const;
    void dmactive (const bool flag);


  private:

    /// \brief Masks for flag bits in \c dmcontrol
    enum Masks
      {
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

    /// \brief Offsets for flag bits in \c dmcontrol
    ///
    /// How much to shift right to get the mask into the LS bits of the word.
    enum Offsets
      {
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

    /// \brief Sizes of flag bits in \c dmcontrol
    ///
    /// How many bits are set in the flag.
    enum Sizes
      {
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

    /// \brief The address of the \c dmcontrol register in the DMI.
    static const uint64_t DMI_ADDR = 0x10;

    /// \brief The reset value of the \c dmcontrol register in the DMI.
    static const uint32_t RESET_VALUE = 0x0;

    /// \brief A reference to the DTM we are using.
    std::unique_ptr<IDtm> & mDtm;

    /// \brief the value of the Dmcontrol register.
    uint32_t mDmcontrolReg;
  };

  class Dmstatus
  {
  public:

    // Constructors & destructor
    Dmstatus () = delete;
    Dmstatus (std::unique_ptr<IDtm> & dtm_);
    ~Dmstatus () = default;

    // Delete copy operator
    Dmstatus& operator=(const Dmstatus&) = delete;

    // API
    void read ();
    bool impebreak () const;
    bool havereset () const;
    bool resumeack () const;
    bool nonexistent () const;
    bool unavail () const;
    bool running () const;
    bool halted () const;
    bool authenticated () const;
    bool authbusy () const;
    bool hasresethaltreq () const;
    bool confstrptrvalid () const;
    uint8_t version () const;

  private:

    /// \brief Masks for flag bits in \c dmstatus
    enum Masks
      {
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
      };

    /// \brief Offsets for flag bits in \c dmstatus
    ///
    /// How much to shift right to get the mask into the LS bits of the word.
    enum Offsets
      {
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
      };

    /// \brief Sizes of flag bits in \c dmstatus
    ///
    /// How many bits are set in the flag.
    enum Sizes
      {
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
      };

    /// \brief The address of the \c dmstatus register in the DMI
    static const uint64_t DMI_ADDR = 0x11;

    /// \brief A reference to the DTM we are using
    std::unique_ptr<IDtm> & mDtm;

    /// \brief The value of the Dmstatus register
    uint32_t mDmstatusReg;
  };

  // Constructor and destructor
  Dmi (std::unique_ptr<IDtm> dtm);
  Dmi () = delete;
  ~Dmi () = default;

  // API for the underlying DTM
  void dtmReset ();

  // Accessors for registers
  std::unique_ptr<Dmcontrol> & dmcontrol ();
  std::unique_ptr<Dmstatus> & dmstatus ();

private:

  /// \brief the Debug Transport Module we use
  std::unique_ptr<IDtm> mDtm;

  /// \brief The Dmcontrol register
  std::unique_ptr<Dmcontrol> mDmcontrol;

  /// \brief The Dmstatus register
  std::unique_ptr<Dmstatus> mDmstatus;
};

#endif	// DMI_H
