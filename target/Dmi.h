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
  /// \brief The class modeling the abstract \c data registers.
  class Data
  {
  public:
    /// \brief The number of abstract \c data registers
    static const std::size_t NUM_REGS = 12;

    // Constructors & destructor
    Data () = delete;
    Data (std::unique_ptr<IDtm> &dtm_);
    ~Data () = default;

    // Delete copy operator
    Data &operator= (const Data &) = delete;

    // API
    void read (const std::size_t n);
    void reset (const std::size_t n);
    void write (const std::size_t n);
    uint32_t data (const std::size_t n) const;
    void data (const std::size_t n, const uint32_t dataVal);

    // Output operator is a friend
    friend std::ostream &operator<< (std::ostream &s,
                                     const std::unique_ptr<Data> &p);

  private:
    /// \brief The address of the \c data registers in the DMI
    static constexpr uint64_t DMI_ADDR[NUM_REGS]
        = { 0x04, 0x05, 0x06, 0x07, 0x08, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f };

    /// \brief The reset value of the \c data registers in the DMI.
    static const uint32_t RESET_VALUE = 0x0;

    /// \brief A reference to the DTM we are using
    std::unique_ptr<IDtm> &mDtm;

    /// \brief The value of the Data registers
    uint32_t mDataReg[NUM_REGS];
  };

  /// \brief The class modeling the \c dmcontrol register.
  class Dmcontrol
  {
  public:
    // Constructors & destructor
    Dmcontrol () = delete;
    Dmcontrol (std::unique_ptr<IDtm> &dtm_);
    ~Dmcontrol () = default;

    // Delete copy operator
    Dmcontrol &operator= (const Dmcontrol &) = delete;

    // API
    void read ();
    void reset ();
    void write ();
    void prettyPrint (const bool flag);
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

    // Output operator is a friend
    friend std::ostream &operator<< (std::ostream &s,
                                     const std::unique_ptr<Dmcontrol> &p);

  private:
    /// \brief Masks for flag bits in \c dmcontrol
    enum Masks
    {
      HALTREQ_MASK = 0x80000000,
      RESUMEREQ_MASK = 0x40000000,
      HARTRESET_MASK = 0x20000000,
      ACKHAVERESET_MASK = 0x10000000,
      HASEL_MASK = 0x04000000,
      HARTSELLO_MASK = 0x03ff0000,
      HARTSELHI_MASK = 0x0000ffc0,
      SETRESETHALTREQ_MASK = 0x00000008,
      CLRRESETHALTREQ_MASK = 0x00000004,
      NDMRESET_MASK = 0x00000002,
      DMACTIVE_MASK = 0x00000001,
    };

    /// \brief Offsets for flag bits in \c dmcontrol
    ///
    /// How much to shift right to get the mask into the LS bits of the word.
    enum Offsets
    {
      HALTREQ_OFFSET = 31,
      RESUMEREQ_OFFSET = 30,
      HARTRESET_OFFSET = 29,
      ACKHAVERESET_OFFSET = 28,
      HASEL_OFFSET = 26,
      HARTSELLO_OFFSET = 16,
      HARTSELHI_OFFSET = 6,
      SETRESETHALTREQ_OFFSET = 3,
      CLRRESETHALTREQ_OFFSET = 2,
      NDMRESET_OFFSET = 1,
      DMACTIVE_OFFSET = 0,
    };

    /// \brief Sizes of flag bits in \c dmcontrol
    ///
    /// How many bits are set in the flag.
    enum Sizes
    {
      HALTREQ_SIZE = 1,
      RESUMEREQ_SIZE = 1,
      HARTRESET_SIZE = 1,
      ACKHAVERESET_SIZE = 1,
      HASEL_SIZE = 1,
      HARTSELLO_SIZE = 10,
      HARTSELHI_SIZE = 10,
      SETRESETHALTREQ_SIZE = 1,
      CLRRESETHALTREQ_SIZE = 1,
      NDMRESET_SIZE = 1,
      DMACTIVE_SIZE = 1,
    };

    /// \brief The address of the \c dmcontrol register in the DMI.
    static const uint64_t DMI_ADDR = 0x10;

    /// \brief The reset value of the \c dmcontrol register in the DMI.
    static const uint32_t RESET_VALUE = 0x0;

    /// \brief Whether pretty printing is enabled for the \c dmcontrol
    /// register.
    bool mPrettyPrint;

    /// \brief A reference to the DTM we are using.
    std::unique_ptr<IDtm> &mDtm;

    /// \brief the value of the Dmcontrol register.
    uint32_t mDmcontrolReg;
  };

  /// \brief The class modeling the \c dmstatus register.
  class Dmstatus
  {
  public:
    // Constructors & destructor
    Dmstatus () = delete;
    Dmstatus (std::unique_ptr<IDtm> &dtm_);
    ~Dmstatus () = default;

    // Delete copy operator
    Dmstatus &operator= (const Dmstatus &) = delete;

    // API
    void read ();
    void prettyPrint (const bool flag);
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

    // Output operator is a friend
    friend std::ostream &operator<< (std::ostream &s,
                                     const std::unique_ptr<Dmstatus> &p);

  private:
    /// \brief Masks for flag bits in \c dmstatus
    enum Masks
    {
      IMPEBREAK_MASK = 0x00400000,
      ALLHAVERESET_MASK = 0x00080000,
      ANYHAVERESET_MASK = 0x00040000,
      ALLRESUMEACK_MASK = 0x00020000,
      ANYRESUMEACK_MASK = 0x00010000,
      ALLNONEXISTENT_MASK = 0x00008000,
      ANYNONEXISTENT_MASK = 0x00004000,
      ALLUNAVAIL_MASK = 0x00002000,
      ANYUNAVAIL_MASK = 0x00001000,
      ALLRUNNING_MASK = 0x00000800,
      ANYRUNNING_MASK = 0x00000400,
      ALLHALTED_MASK = 0x00000200,
      ANYHALTED_MASK = 0x00000100,
      AUTHENTICATED_MASK = 0x00000080,
      AUTHBUSY_MASK = 0x00000040,
      HASRESETHALTREQ_MASK = 0x00000020,
      CONFSTRPTRVALID_MASK = 0x00000010,
      VERSION_MASK = 0x0000000f,
    };

    /// \brief Offsets for flag bits in \c dmstatus
    ///
    /// How much to shift right to get the mask into the LS bits of the word.
    enum Offsets
    {
      IMPEBREAK_OFFSET = 22,
      ALLHAVERESET_OFFSET = 19,
      ANYHAVERESET_OFFSET = 18,
      ALLRESUMEACK_OFFSET = 17,
      ANYRESUMEACK_OFFSET = 16,
      ALLNONEXISTENT_OFFSET = 15,
      ANYNONEXISTENT_OFFSET = 14,
      ALLUNAVAIL_OFFSET = 13,
      ANYUNAVAIL_OFFSET = 12,
      ALLRUNNING_OFFSET = 11,
      ANYRUNNING_OFFSET = 10,
      ALLHALTED_OFFSET = 9,
      ANYHALTED_OFFSET = 8,
      AUTHENTICATED_OFFSET = 7,
      AUTHBUSY_OFFSET = 6,
      HASRESETHALTREQ_OFFSET = 5,
      CONFSTRPTRVALID_OFFSET = 4,
      VERSION_OFFSET = 0,
    };

    /// \brief Sizes of flag bits in \c dmstatus
    ///
    /// How many bits are set in the flag.
    enum Sizes
    {
      IMPEBREAK_SIZE = 1,
      ALLHAVERESET_SIZE = 1,
      ANYHAVERESET_SIZE = 1,
      ALLRESUMEACK_SIZE = 1,
      ANYRESUMEACK_SIZE = 1,
      ALLNONEXISTENT_SIZE = 1,
      ANYNONEXISTENT_SIZE = 1,
      ALLUNAVAIL_SIZE = 1,
      ANYUNAVAIL_SIZE = 1,
      ALLRUNNING_SIZE = 1,
      ANYRUNNING_SIZE = 1,
      ALLHALTED_SIZE = 1,
      ANYHALTED_SIZE = 1,
      AUTHENTICATED_SIZE = 1,
      AUTHBUSY_SIZE = 1,
      HASRESETHALTREQ_SIZE = 1,
      CONFSTRPTRVALID_SIZE = 1,
      VERSION_SIZE = 4,
    };

    /// \brief The address of the \c dmstatus register in the DMI
    static const uint64_t DMI_ADDR = 0x11;

    /// \brief Whether pretty printing is enabled for the \c dmstatus
    /// register.
    bool mPrettyPrint;

    /// \brief A reference to the DTM we are using
    std::unique_ptr<IDtm> &mDtm;

    /// \brief The value of the Dmstatus register
    uint32_t mDmstatusReg;
  };

  /// \brief The class modeling the \c hartinfo register.
  class Hartinfo
  {
  public:
    // Constructors & destructor
    Hartinfo () = delete;
    Hartinfo (std::unique_ptr<IDtm> &dtm_);
    ~Hartinfo () = default;

    // Delete copy operator
    Hartinfo &operator= (const Hartinfo &) = delete;

    // API
    void read ();
    void prettyPrint (const bool flag);
    uint8_t nscratch () const;
    bool dataaccess () const;
    uint8_t datasize () const;
    uint16_t dataaddr () const;

    // Output operator is a friend
    friend std::ostream &operator<< (std::ostream &s,
                                     const std::unique_ptr<Hartinfo> &p);

  private:
    /// \brief Masks for flag bits in \c hartinfo
    enum Masks
    {
      NSCRATCH_MASK = 0x00f00000,
      DATAACCESS_MASK = 0x00010000,
      DATASIZE_MASK = 0x0000f000,
      DATAADDR_MASK = 0x00000fff,
    };

    /// \brief Offsets for flag bits in \c hartinfo
    ///
    /// How much to shift right to get the mask into the LS bits of the word.
    enum Offsets
    {
      NSCRATCH_OFFSET = 20,
      DATAACCESS_OFFSET = 16,
      DATASIZE_OFFSET = 12,
      DATAADDR_OFFSET = 0,
    };

    /// \brief Sizes of flag bits in \c hartinfo
    ///
    /// How many bits are set in the flag.
    enum Sizes
    {
      NSCRATCH_SIZE = 4,
      DATAACCESS_SIZE = 1,
      DATASIZE_SIZE = 4,
      DATAADDR_SIZE = 12,
    };

    /// \brief The address of the \c hartinfo register in the DMI
    static const uint64_t DMI_ADDR = 0x12;

    /// \brief Whether pretty printing is enabled for the \c hartinfo
    /// register.
    bool mPrettyPrint;

    /// \brief A reference to the DTM we are using
    std::unique_ptr<IDtm> &mDtm;

    /// \brief The value of the Hartinfo register
    uint32_t mHartinfoReg;
  };

  /// \brief The class modeling the \c haltsum registers.
  class Haltsum
  {
  public:
    /// \brief The number of \c haltsum registers
    static const std::size_t NUM_REGS = 4;

    // Constructors & destructor
    Haltsum () = delete;
    Haltsum (std::unique_ptr<IDtm> &dtm_);
    ~Haltsum () = default;

    // Delete copy operator
    Haltsum &operator= (const Haltsum &) = delete;

    // API
    void read (const std::size_t n);
    uint32_t haltsum (const std::size_t n) const;

    // Output operator is a friend
    friend std::ostream &operator<< (std::ostream &s,
                                     const std::unique_ptr<Haltsum> &p);

  private:
    /// \brief The address of the \c haltsum registers in the DMI
    static constexpr uint64_t DMI_ADDR[NUM_REGS] = { 0x40, 0x13, 0x34, 0x35 };

    /// \brief A reference to the DTM we are using
    std::unique_ptr<IDtm> &mDtm;

    /// \brief The value of the Haltsum registers
    uint32_t mHaltsumReg[NUM_REGS];
  };

  /// \brief The class modeling the \c hawindowsel register.
  class Hawindowsel
  {
  public:
    // Constructors & destructor
    Hawindowsel () = delete;
    Hawindowsel (std::unique_ptr<IDtm> &dtm_);
    ~Hawindowsel () = default;

    // Delete copy operator
    Hawindowsel &operator= (const Hawindowsel &) = delete;

    // API
    void read ();
    void reset ();
    void write ();
    uint16_t hawindowsel () const;
    void hawindowsel (const uint16_t hawindowselVal);

    // Output operator is a friend
    friend std::ostream &operator<< (std::ostream &s,
                                     const std::unique_ptr<Hawindowsel> &p);

  private:
    /// \brief Masks for flag bits in \c hawindowsel
    enum Masks
    {
      HAWINDOWSEL_MASK = 0x00007fff,
    };

    /// \brief Offsets for flag bits in \c hawindowsel
    ///
    /// How much to shift right to get the mask into the LS bits of the word.
    enum Offsets
    {
      HAWINDOWSEL_OFFSET = 0,
    };

    /// \brief Sizes of flag bits in \c hawindowsel
    ///
    /// How many bits are set in the flag.
    enum Sizes
    {
      HAWINDOWSEL_SIZE = 15,
    };

    /// \brief The address of the \c hawindowsel register in the DMI
    static const uint64_t DMI_ADDR = 0x14;

    /// \brief The reset value of the \c hawindowsel register in the DMI.
    static const uint32_t RESET_VALUE = 0x0;

    /// \brief A reference to the DTM we are using
    std::unique_ptr<IDtm> &mDtm;

    /// \brief The value of the Hawindowsel register
    uint32_t mHawindowselReg;
  };

  /// \brief The class modeling the \c hawindow register.
  class Hawindow
  {
  public:
    // Constructors & destructor
    Hawindow () = delete;
    Hawindow (std::unique_ptr<IDtm> &dtm_);
    ~Hawindow () = default;

    // Delete copy operator
    Hawindow &operator= (const Hawindow &) = delete;

    // API
    void read ();
    void reset ();
    void write ();
    uint32_t hawindow () const;
    void hawindow (const uint32_t hawindowVal);

    // Output operator is a friend
    friend std::ostream &operator<< (std::ostream &s,
                                     const std::unique_ptr<Hawindow> &p);

  private:
    /// \brief The address of the \c hawindow register in the DMI
    static const uint64_t DMI_ADDR = 0x15;

    /// \brief The reset value of the \c hawindow register in the DMI.
    static const uint32_t RESET_VALUE = 0x0;

    /// \brief A reference to the DTM we are using
    std::unique_ptr<IDtm> &mDtm;

    /// \brief The value of the Hawindow register
    uint32_t mHawindowReg;
  };

  /// \brief The class modeling the \c abstractcs register.
  class Abstractcs
  {
  public:
    // Constructors & destructor
    Abstractcs () = delete;
    Abstractcs (std::unique_ptr<IDtm> &dtm_);
    ~Abstractcs () = default;

    // Delete copy operator
    Abstractcs &operator= (const Abstractcs &) = delete;

    // API
    void read ();
    void reset ();
    void write ();
    void prettyPrint (const bool flag);
    uint8_t progbufsize () const;
    bool busy () const;
    uint8_t cmderr () const;
    void cmderrClear ();
    uint8_t datacount () const;

    // Output operator is a friend
    friend std::ostream &operator<< (std::ostream &s,
                                     const std::unique_ptr<Abstractcs> &p);

  private:
    /// \brief Masks for flag bits in \c abstractcs
    enum Masks
    {
      PROGBUFSIZE_MASK = 0x1f000000,
      BUSY_MASK = 0x00001000,
      CMDERR_MASK = 0x00000700,
      DATACOUNT_MASK = 0x0000000f,
    };

    /// \brief Offsets for flag bits in \c abstractcs
    ///
    /// How much to shift right to get the mask into the LS bits of the word.
    enum Offsets
    {
      PROGBUFSIZE_OFFSET = 24,
      BUSY_OFFSET = 12,
      CMDERR_OFFSET = 8,
      DATACOUNT_OFFSET = 0,
    };

    /// \brief Sizes of flag bits in \c abstractcs
    ///
    /// How many bits are set in the flag.
    enum Sizes
    {
      PROGBUFSIZE_SIZE = 5,
      BUSY_SIZE = 1,
      CMDERR_SIZE = 3,
      DATACOUNT_SIZE = 4,
    };

    /// \brief The address of the \c abstractcs register in the DMI.
    static const uint64_t DMI_ADDR = 0x16;

    /// \brief The reset value of the \c abstractcs register in the DMI.
    ///
    /// Set all the \c cmderr bits to 1 t clear.
    static const uint32_t RESET_VALUE = CMDERR_MASK;

    /// \brief Whether pretty printing is enabled for the \c abstractcs
    /// register.
    bool mPrettyPrint;

    /// \brief A reference to the DTM we are using.
    std::unique_ptr<IDtm> &mDtm;

    /// \brief the value of the Abstractcs register.
    uint32_t mAbstractcsReg;
  };

  /// \brief The class modeling the \c command register.
  ///
  /// \note This is a write only register, thus no read method is needed.
  class Command
  {
  public:
    // Constructors & destructor
    Command () = delete;
    Command (std::unique_ptr<IDtm> &dtm_);
    ~Command () = default;

    // Delete copy operator
    Command &operator= (const Command &) = delete;

    // API
    void reset ();
    void write ();
    void prettyPrint (const bool flag);
    void cmdtype (const uint8_t cmdtypeVal);
    void control (const uint32_t controlVal);

    // Output operator is a friend
    friend std::ostream &operator<< (std::ostream &s,
                                     const std::unique_ptr<Command> &p);

  private:
    /// \brief Masks for flag bits in \c command
    enum Masks
    {
      CMDTYPE_MASK = 0xff000000,
      CONTROL_MASK = 0x00ffffff,
    };

    /// \brief Offsets for flag bits in \c command
    ///
    /// How much to shift right to get the mask into the LS bits of the word.
    enum Offsets
    {
      CMDTYPE_OFFSET = 24,
      CONTROL_OFFSET = 0,
    };

    /// \brief Sizes of flag bits in \c command
    ///
    /// How many bits are set in the flag.
    enum Sizes
    {
      CMDTYPE_SIZE = 8,
      CONTROL_SIZE = 24,
    };

    /// \brief The address of the \c command register in the DMI.
    static const uint64_t DMI_ADDR = 0x17;

    /// \brief The reset value of the \c command register in the DMI.
    ///
    /// \todo  Is this the correct value for the reset value of the \c command
    ///        register in the DMI.
    static const uint32_t RESET_VALUE = 0;

    /// \brief Whether pretty printing is enabled for the \c command
    /// register.
    bool mPrettyPrint;

    /// \brief A reference to the DTM we are using.
    std::unique_ptr<IDtm> &mDtm;

    /// \brief the value of the Command register.
    uint32_t mCommandReg;
  };

  /// \brief The class modeling the \c abstractauto register.
  ///
  /// \note This is a write only register, thus no read method is needed.
  class Abstractauto
  {
  public:
    // Constructors & destructor
    Abstractauto () = delete;
    Abstractauto (std::unique_ptr<IDtm> &dtm_);
    ~Abstractauto () = default;

    // Delete copy operator
    Abstractauto &operator= (const Abstractauto &) = delete;

    // API
    void read ();
    void reset ();
    void write ();
    void prettyPrint (const bool flag);
    uint16_t autoexecprogbuf () const;
    void autoexecprogbuf (const uint16_t autoexecprogbufVal);
    uint16_t autoexecdata () const;
    void autoexecdata (const uint16_t autoexecdataVal);

    // Output operator is a friend
    friend std::ostream &operator<< (std::ostream &s,
                                     const std::unique_ptr<Abstractauto> &p);

  private:
    /// \brief Masks for flag bits in \c abstractauto
    enum Masks
    {
      AUTOEXECPROGBUF_MASK = 0xffff0000,
      AUTOEXECDATA_MASK = 0x00000fff,
    };

    /// \brief Offsets for flag bits in \c abstractauto
    ///
    /// How much to shift right to get the mask into the LS bits of the word.
    enum Offsets
    {
      AUTOEXECPROGBUF_OFFSET = 16,
      AUTOEXECDATA_OFFSET = 0,
    };

    /// \brief Sizes of flag bits in \c abstractauto
    ///
    /// How many bits are set in the flag.
    enum Sizes
    {
      AUTOEXECPROGBUF_SIZE = 16,
      AUTOEXECDATA_SIZE = 12,
    };

    /// \brief The address of the \c abstractauto register in the DMI.
    static const uint64_t DMI_ADDR = 0x18;

    /// \brief The reset value of the \c abstractauto register in the DMI.
    static const uint32_t RESET_VALUE = 0;

    /// \brief Whether pretty printing is enabled for the \c abstractauto
    /// register.
    bool mPrettyPrint;

    /// \brief A reference to the DTM we are using.
    std::unique_ptr<IDtm> &mDtm;

    /// \brief the value of the Abstractauto register.
    uint32_t mAbstractautoReg;
  };

  /// \brief The class modeling the \c confstrptr registers.
  class Confstrptr
  {
  public:
    /// \brief The number of \c confstrptr registers
    static const std::size_t NUM_REGS = 4;

    // Constructors & destructor
    Confstrptr () = delete;
    Confstrptr (std::unique_ptr<IDtm> &dtm_);
    ~Confstrptr () = default;

    // Delete copy operator
    Confstrptr &operator= (const Confstrptr &) = delete;

    // API
    void read (const std::size_t n);
    uint32_t confstrptr (const std::size_t n) const;

    // Output operator is a friend
    friend std::ostream &operator<< (std::ostream &s,
                                     const std::unique_ptr<Confstrptr> &p);

  private:
    /// \brief The address of the \c confstrptr registers in the DMI
    static constexpr uint64_t DMI_ADDR[NUM_REGS] = { 0x19, 0x1a, 0x1b, 0x1c };

    /// \brief A reference to the DTM we are using
    std::unique_ptr<IDtm> &mDtm;

    /// \brief The value of the Confstrptr registers
    uint32_t mConfstrptrReg[NUM_REGS];
  };

  /// \brief The class modeling the \c nextdm register.
  class Nextdm
  {
  public:
    // Constructors & destructor
    Nextdm () = delete;
    Nextdm (std::unique_ptr<IDtm> &dtm_);
    ~Nextdm () = default;

    // Delete copy operator
    Nextdm &operator= (const Nextdm &) = delete;

    // API
    void read ();
    uint32_t nextdm () const;
    void nextdm (const uint32_t nextdmVal);

    // Output operator is a friend
    friend std::ostream &operator<< (std::ostream &s,
                                     const std::unique_ptr<Nextdm> &p);

  private:
    /// \brief The address of the \c nextdm register in the DMI
    static const uint64_t DMI_ADDR = 0x1d;

    /// \brief The reset value of the \c nextdm register in the DMI.
    static const uint32_t RESET_VALUE = 0x0;

    /// \brief A reference to the DTM we are using
    std::unique_ptr<IDtm> &mDtm;

    /// \brief The value of the Nextdm register
    uint32_t mNextdmReg;
  };

  /// \brief The class modeling the \c progbuf registers.
  class Progbuf
  {
  public:
    /// \brief The number of \c progbuf registers
    static const std::size_t NUM_REGS = 16;

    // Constructors & destructor
    Progbuf () = delete;
    Progbuf (std::unique_ptr<IDtm> &dtm_);
    ~Progbuf () = default;

    // Delete copy operator
    Progbuf &operator= (const Progbuf &) = delete;

    // API
    void read (const std::size_t n);
    void reset (const std::size_t n);
    void write (const std::size_t n);
    uint32_t progbuf (const std::size_t n) const;
    void progbuf (const std::size_t n, const uint32_t progbufVal);

    // Output operator is a friend
    friend std::ostream &operator<< (std::ostream &s,
                                     const std::unique_ptr<Progbuf> &p);

  private:
    /// \brief The address of the \c progbuf registers in the DMI
    static constexpr uint64_t DMI_ADDR[NUM_REGS]
        = { 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27,
            0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f };

    /// \brief The reset value of the \c progbuf registers in the DMI.
    static const uint32_t RESET_VALUE = 0x0;

    /// \brief A reference to the DTM we are using
    std::unique_ptr<IDtm> &mDtm;

    /// \brief The value of the Progbuf registers
    uint32_t mProgbufReg[NUM_REGS];
  };

  /// \brief The class modeling the \c authdata register.
  ///
  /// \warning  Since \c dmcontrol does not support authentication, it would be
  ///           wise to assume that this is really a dummy register.
  class Authdata
  {
  public:
    // Constructors & destructor
    Authdata () = delete;
    Authdata (std::unique_ptr<IDtm> &dtm_);
    ~Authdata () = default;

    // Delete copy operator
    Authdata &operator= (const Authdata &) = delete;

    // API
    void read ();
    void reset ();
    void write ();
    uint32_t authdata () const;
    void authdata (const uint32_t authdataVal);

    // Output operator is a friend
    friend std::ostream &operator<< (std::ostream &s,
                                     const std::unique_ptr<Authdata> &p);

  private:
    /// \brief The address of the \c authdata register in the DMI
    static const uint64_t DMI_ADDR = 0x30;

    /// \brief The reset value of the \c authdata register in the DMI.
    static const uint32_t RESET_VALUE = 0x0;

    /// \brief A reference to the DTM we are using
    std::unique_ptr<IDtm> &mDtm;

    /// \brief The value of the Authdata register
    uint32_t mAuthdataReg;
  };

  /// \brief The class modeling the \c sbaddress registers.
  class Sbaddress
  {
  public:
    /// \brief The number of \c sbaddress registers
    static const std::size_t NUM_REGS = 4;

    // Constructors & destructor
    Sbaddress () = delete;
    Sbaddress (std::unique_ptr<IDtm> &dtm_);
    ~Sbaddress () = default;

    // Delete copy operator
    Sbaddress &operator= (const Sbaddress &) = delete;

    // API
    void read (const std::size_t n);
    void reset (const std::size_t n);
    void write (const std::size_t n);
    uint32_t sbaddress (const std::size_t n) const;
    void sbaddress (const std::size_t n, const uint32_t sbaddressVal);

    // Output operator is a friend
    friend std::ostream &operator<< (std::ostream &s,
                                     const std::unique_ptr<Sbaddress> &p);

  private:
    /// \brief The address of the \c sbaddress registers in the DMI
    static constexpr uint64_t DMI_ADDR[NUM_REGS] = { 0x39, 0x3a, 0x3b, 0x37 };

    /// \brief The reset value of the \c sbaddress registers in the DMI.
    static const uint32_t RESET_VALUE = 0x0;

    /// \brief A reference to the DTM we are using
    std::unique_ptr<IDtm> &mDtm;

    /// \brief The value of the Sbaddress registers
    uint32_t mSbaddressReg[NUM_REGS];
  };

  /// \brief The class modeling the \c sbcs register.
  class Sbcs
  {
  public:
    // Constructors & destructor
    Sbcs () = delete;
    Sbcs (std::unique_ptr<IDtm> &dtm_);
    ~Sbcs () = default;

    // Delete copy operator
    Sbcs &operator= (const Sbcs &) = delete;

    // API
    void read ();
    void reset ();
    void write ();
    void prettyPrint (const bool flag);
    uint8_t sbversion () const;
    bool sbbusyerror () const;
    void sbbusyerrorClear ();
    bool sbbusy () const;
    bool sbreadonaddr () const;
    void sbreadonaddr (const bool flag);
    uint8_t sbaccess () const;
    void sbaccess (const uint8_t val);
    bool sbautoincrement () const;
    void sbautoincrement (const bool flag);
    bool sbreadondata () const;
    void sbreadondata (const bool flag);
    uint8_t sberror () const;
    void sberrorClear ();
    uint8_t sbasize () const;
    bool sbaccess128 () const;
    bool sbaccess64 () const;
    bool sbaccess32 () const;
    bool sbaccess16 () const;
    bool sbaccess8 () const;

    // Output operator is a friend
    friend std::ostream &operator<< (std::ostream &s,
                                     const std::unique_ptr<Sbcs> &p);

  private:
    /// \brief Masks for flag bits in \c sbcs
    enum Masks
    {
      SBVERSION_MASK = 0xe0000000,
      SBBUSYERROR_MASK = 0x00400000,
      SBBUSY_MASK = 0x00200000,
      SBREADONADDR_MASK = 0x00100000,
      SBACCESS_MASK = 0x000e0000,
      SBAUTOINCREMENT_MASK = 0x00010000,
      SBREADONDATA_MASK = 0x00008000,
      SBERROR_MASK = 0x00007000,
      SBASIZE_MASK = 0x00000fe0,
      SBACCESS128_MASK = 0x00000010,
      SBACCESS64_MASK = 0x00000008,
      SBACCESS32_MASK = 0x00000004,
      SBACCESS16_MASK = 0x00000002,
      SBACCESS8_MASK = 0x00000001,
    };

    /// \brief Offsets for flag bits in \c sbcs
    ///
    /// How much to shift right to get the mask into the LS bits of the word.
    enum Offsets
    {
      SBVERSION_OFFSET = 29,
      SBBUSYERROR_OFFSET = 22,
      SBBUSY_OFFSET = 21,
      SBREADONADDR_OFFSET = 20,
      SBACCESS_OFFSET = 17,
      SBAUTOINCREMENT_OFFSET = 16,
      SBREADONDATA_OFFSET = 15,
      SBERROR_OFFSET = 12,
      SBASIZE_OFFSET = 5,
      SBACCESS128_OFFSET = 4,
      SBACCESS64_OFFSET = 3,
      SBACCESS32_OFFSET = 2,
      SBACCESS16_OFFSET = 1,
      SBACCESS8_OFFSET = 0,
    };

    /// \brief Sizes of flag bits in \c sbcs
    ///
    /// How many bits are set in the flag.
    enum Sizes
    {
      SBVERSION_SIZE = 3,
      SBBUSYERROR_SIZE = 1,
      SBBUSY_SIZE = 1,
      SBREADONADDR_SIZE = 1,
      SBACCESS_SIZE = 3,
      SBAUTOINCREMENT_SIZE = 1,
      SBREADONDATA_SIZE = 1,
      SBERROR_SIZE = 3,
      SBASIZE_SIZE = 7,
      SBACCESS128_SIZE = 1,
      SBACCESS64_SIZE = 1,
      SBACCESS32_SIZE = 1,
      SBACCESS16_SIZE = 1,
      SBACCESS8_SIZE = 1,
    };

    /// \brief The address of the \c sbcs register in the DMI.
    static const uint64_t DMI_ADDR = 0x38;

    /// \brief The reset value of the \c sbcs register in the DMI.
    ///
    /// Default version is 2 and access size is 32-bit
    static const uint32_t RESET_VALUE = 0x20040000;

    /// \brief Whether pretty printing is enabled for the \c sbcs
    /// register.
    bool mPrettyPrint;

    /// \brief A reference to the DTM we are using.
    std::unique_ptr<IDtm> &mDtm;

    /// \brief the value of the Sbcs register.
    uint32_t mSbcsReg;
  };

  /// \brief The class modeling the \c sbdata registers.
  class Sbdata
  {
  public:
    /// \brief The number of \c sbdata registers
    static const std::size_t NUM_REGS = 4;

    // Constructors & destructor
    Sbdata () = delete;
    Sbdata (std::unique_ptr<IDtm> &dtm_);
    ~Sbdata () = default;

    // Delete copy operator
    Sbdata &operator= (const Sbdata &) = delete;

    // API
    void read (const std::size_t n);
    void reset (const std::size_t n);
    void write (const std::size_t n);
    uint32_t sbdata (const std::size_t n) const;
    void sbdata (const std::size_t n, const uint32_t sbdataVal);

    // Output operator is a friend
    friend std::ostream &operator<< (std::ostream &s,
                                     const std::unique_ptr<Sbdata> &p);

  private:
    /// \brief The address of the \c sbdata registers in the DMI
    static constexpr uint64_t DMI_ADDR[NUM_REGS] = { 0x3c, 0x3d, 0x3e, 0x3f };

    /// \brief The reset value of the \c sbdata registers in the DMI.
    static const uint32_t RESET_VALUE = 0x0;

    /// \brief A reference to the DTM we are using
    std::unique_ptr<IDtm> &mDtm;

    /// \brief The value of the Sbdata registers
    uint32_t mSbdataReg[NUM_REGS];
  };

  // Constructor and destructor
  Dmi (std::unique_ptr<IDtm> dtm);
  Dmi () = delete;
  ~Dmi () = default;

  // API for the underlying DTM
  void dtmReset ();

  // Accessors for registers
  std::unique_ptr<Data> &data ();
  std::unique_ptr<Dmcontrol> &dmcontrol ();
  std::unique_ptr<Dmstatus> &dmstatus ();
  std::unique_ptr<Hartinfo> &hartinfo ();
  std::unique_ptr<Haltsum> &haltsum ();
  std::unique_ptr<Hawindowsel> &hawindowsel ();
  std::unique_ptr<Hawindow> &hawindow ();
  std::unique_ptr<Abstractcs> &abstractcs ();
  std::unique_ptr<Command> &command ();
  std::unique_ptr<Abstractauto> &abstractauto ();
  std::unique_ptr<Confstrptr> &confstrptr ();
  std::unique_ptr<Nextdm> &nextdm ();
  std::unique_ptr<Progbuf> &progbuf ();
  std::unique_ptr<Authdata> &authdata ();
  std::unique_ptr<Sbaddress> &sbaddress ();
  std::unique_ptr<Sbcs> &sbcs ();
  std::unique_ptr<Sbdata> &sbdata ();

private:
  /// \brief the Debug Transport Module we use.
  std::unique_ptr<IDtm> mDtm;

  /// \brief The \c data register set.
  std::unique_ptr<Data> mData;

  /// \brief The \c dmcontrol register.
  std::unique_ptr<Dmcontrol> mDmcontrol;

  /// \brief The \c dmstatus register.
  std::unique_ptr<Dmstatus> mDmstatus;

  /// \brief The \c hartinfo register.
  std::unique_ptr<Hartinfo> mHartinfo;

  /// \brief The \c haltsum register set.
  std::unique_ptr<Haltsum> mHaltsum;

  /// \brief The \c hawindowsel register.
  std::unique_ptr<Hawindowsel> mHawindowsel;

  /// \brief The \c hawindow register.
  std::unique_ptr<Hawindow> mHawindow;

  /// \brief The \c abstractcs register.
  std::unique_ptr<Abstractcs> mAbstractcs;

  /// \brief The \c command register.
  std::unique_ptr<Command> mCommand;

  /// \brief The \c abstractauto register.
  std::unique_ptr<Abstractauto> mAbstractauto;

  /// \brief The \c confstrptr register set.
  std::unique_ptr<Confstrptr> mConfstrptr;

  /// \brief The \c nextdm register.
  std::unique_ptr<Nextdm> mNextdm;

  /// \brief The \c progbuf register set.
  std::unique_ptr<Progbuf> mProgbuf;

  /// \brief The \c authdata register.
  std::unique_ptr<Authdata> mAuthdata;

  /// \brief The \c sbaddress register set.
  std::unique_ptr<Sbaddress> mSbaddress;

  /// \brief The \c sbcs register.
  std::unique_ptr<Sbcs> mSbcs;

  /// \brief The \c sbdata register set.
  std::unique_ptr<Sbdata> mSbdata;
};

// Stream output operators for DMI register classes
std::ostream &operator<< (std::ostream &s, const std::unique_ptr<Dmi::Data> &p);
std::ostream &operator<< (std::ostream &s,
                          const std::unique_ptr<Dmi::Dmcontrol> &p);
std::ostream &operator<< (std::ostream &s,
                          const std::unique_ptr<Dmi::Dmstatus> &p);
std::ostream &operator<< (std::ostream &s,
                          const std::unique_ptr<Dmi::Hartinfo> &p);
std::ostream &operator<< (std::ostream &s,
                          const std::unique_ptr<Dmi::Haltsum> &p);
std::ostream &operator<< (std::ostream &s,
                          const std::unique_ptr<Dmi::Hawindowsel> &p);
std::ostream &operator<< (std::ostream &s,
                          const std::unique_ptr<Dmi::Hawindow> &p);
std::ostream &operator<< (std::ostream &s,
                          const std::unique_ptr<Dmi::Abstractcs> &p);
std::ostream &operator<< (std::ostream &s,
                          const std::unique_ptr<Dmi::Command> &p);
std::ostream &operator<< (std::ostream &s,
                          const std::unique_ptr<Dmi::Abstractauto> &p);
std::ostream &operator<< (std::ostream &s,
                          const std::unique_ptr<Dmi::Confstrptr> &p);
std::ostream &operator<< (std::ostream &s,
                          const std::unique_ptr<Dmi::Nextdm> &p);
std::ostream &operator<< (std::ostream &s,
                          const std::unique_ptr<Dmi::Progbuf> &p);
std::ostream &operator<< (std::ostream &s,
                          const std::unique_ptr<Dmi::Authdata> &p);
std::ostream &operator<< (std::ostream &s,
                          const std::unique_ptr<Dmi::Sbaddress> &p);
std::ostream &operator<< (std::ostream &s, const std::unique_ptr<Dmi::Sbcs> &p);
std::ostream &operator<< (std::ostream &s,
                          const std::unique_ptr<Dmi::Sbdata> &p);

#endif // DMI_H
