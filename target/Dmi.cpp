// Definition of the generic Debug Module Interface
//
// This file is part of the Embecosm Debug Server target for CORE-V MCU
//
// Copyright (C) 2021 Embecosm Limited
// SPDX-License-Identifier: Apache-2.0

#include <iostream>
#include <iomanip>

#include "Dmi.h"

using std::cout;
using std::cerr;
using std::dec;
using std::endl;
using std::hex;
using std::setfill;
using std::setw;
using std::unique_ptr;

/// \brief Constructor for the DMI
///
/// The DTM has to be constructed locally, but is then passed to us, and we
/// take ownership.  It is a unique_ptr, so the call to the constructor must
/// explicitly move the pointer.
///
/// We create local instances of all the interesting registers
///
/// \param[in] dtm_  The Debug Transport Module we will use.
Dmi::Dmi (unique_ptr<IDtm> dtm_) :
  mDtm (std::move(dtm_))
{
  mDmcontrol.reset (new Dmcontrol (mDtm));
  mDmstatus.reset (new Dmstatus (mDtm));
}

/// \brief Reset the underlying DTM.
void
Dmi::dtmReset ()
{
  mDtm->reset ();
}

/// \brief Get the \p dmcontrol register.
///
/// \return an instance of class Dmcontrol:: representing the status register.
std::unique_ptr<Dmi::Dmcontrol> &
Dmi::dmcontrol ()
{
  return mDmcontrol;
}

/// \brief Get the \p dmstatus register.
///
/// \return an instance of class Dmstatus:: representing the status register.
std::unique_ptr<Dmi::Dmstatus> &
Dmi::dmstatus ()
{
  return mDmstatus;
}

/*******************************************************************************
 *                                                                             *
 * The Dmi::Dmcontrol class                                                     *
 *                                                                             *
 ******************************************************************************/

/// \brief constructor for the Dmi::Dmcontrol class.
///
/// \param[in] dtm_  The DTM we shall use.  This is a unique_ptr owned by the
///                  Dmi, so passed and stored by reference.
Dmi::Dmcontrol::Dmcontrol (unique_ptr<IDtm> & dtm_) :
  mDtm (dtm_), mDmcontrolReg (0)
{
}

/// \brief Read the value of the \c dmcontrol register.
///
/// The register is refreshed via the DTM.
void
Dmi::Dmcontrol::read ()
{
  mDmcontrolReg = mDtm->dmiRead (DMI_ADDR);
  // cout << "dmcontrol read:  0x" << setw(8) << setfill ('0') << hex
  //      << mDmcontrolReg << dec << setfill (' ') << setw (0) << endl;
}

/// \brief Set the \c dmcontrol register to its reset value.
void
Dmi::Dmcontrol::reset ()
{
  mDmcontrolReg = RESET_VALUE;
}

/// \brief Write the value of the \c dmcontrol register.
///
/// The register is refreshed via the DTM, and we save the value read back.
void
Dmi::Dmcontrol::write ()
{
  mDmcontrolReg = mDtm->dmiWrite (DMI_ADDR, mDmcontrolReg);
  // cout << "dmcontrol write: 0x" << setw(8) << setfill ('0') << hex
  //      << mDmcontrolReg << dec << setfill (' ') << setw (0) << endl;
}

/// \brief Set the \c haltreq bit in \c dmcontrol.
///
/// \note Value set will apply to the \em new value of \c hartsel.
///
/// \param[in] flag  \c true sets the halt request bit for all currently
///                  selected harts, so that running hards will halt whenever
///                  their halt request bit is set.\c false clears the halt
///                  request bit, 
void
Dmi::Dmcontrol::haltreq (const bool flag)
{
  if (flag)
    mDmcontrolReg |= HALTREQ_MASK;
  else
    mDmcontrolReg &= ~HALTREQ_MASK;
}

/// \brief Set the \c resumereq bit in \c dmcontrol to 1.
void
Dmi::Dmcontrol::resumereq ()
{
  mDmcontrolReg |= RESUMEREQ_MASK;
}

/// \brief Get the \c hartreset bit in \c dmcontrol (NOT IMPLEMENTED).
///
/// \warning Not implemented for the CORE-V MCU debug unit. Always returns
///          \c false.
///
/// \returns Always returns \c false.
bool
Dmi::Dmcontrol::hartreset () const
{
  return false;
}

/// \brief Set the \c hartreset bit in \c dmcontrol (NOT IMPLEMENTED).
///
/// \warning Not implemented for the CORE-V MCU debug unit. Ignored with a
///          warning.
///
/// \param[in] flag  Value ignored.
void
Dmi::Dmcontrol::hartreset (const bool flag __attribute ((unused)) )
{
  cerr << "Warning: Setting dmcontrol:hartreset not supported: ignored."
       << endl;
}

/// \brief Set the \c ackhavereset bit in \c dmcontrol to 1.
void
Dmi::Dmcontrol::ackhavereset ()
{
  mDmcontrolReg |= ACKHAVERESET_MASK;
}

/// \brief Get the \c hasel bit in \c dmcontrol (NOT IMPLEMENTED).
///
/// \warning Not implemented for the CORE-V MCU debug unit. Always returns
///          \c false (only ever one selected hart).
///
/// \return Always returns \c false.
bool
Dmi::Dmcontrol::hasel () const
{
  return false;
}

/// \brief Set the \c hasel bit in \c dmcontrol (NOT IMPLEMENTED).
///
/// \warning Not implemented for the CORE-V MCU debug unit. Ignored with a
///          warning.
///
/// \param[in] flag  Value ignored.
void
Dmi::Dmcontrol::hasel (const bool flag __attribute ((unused)) )
{
  cerr << "Warning: Setting dmcontrol:hasel not supported: ignored."
       << endl;
}

/// \brief Get the \c hartsello and \c hartselhi bits of \c dmcontrol
///
/// Computes \c hartsel as \c hartselhi << 10 | \c hartsello.
///
/// \return The value of \c hartsel
uint32_t
Dmi::Dmcontrol::hartsel () const
{
  uint32_t hartsello = (mDmcontrolReg & (HARTSELLO_MASK)) >> HARTSELLO_OFFSET;
  uint32_t hartselhi = (mDmcontrolReg & (HARTSELHI_MASK)) >> HARTSELHI_OFFSET;
  return (hartselhi << HARTSELLO_SIZE) | hartsello;
}

/// \brief Set the \c hartsello and \c hartselhi bits of \c dmcontrol
///
/// Sets \c hartslehi to \p (hartselVal >> 10) & 0x3ff
/// Sets \c hartsello to \p hartselVal & 0x3ff
///
/// \param[in] hartselVal  The value of \c hartsel to set.
void
Dmi::Dmcontrol::hartsel (const uint32_t hartselVal)
{
  if (hartselVal >= (1 << (HARTSELLO_SIZE + HARTSELHI_SIZE)))
    cerr << "Warning: requested value of hartsel, " << hartselVal
	 << ", exceeds the maximum permitted value: higher bits ignored."
	 << endl;

  uint32_t hartsello = (hartselVal << HARTSELLO_OFFSET) & HARTSELLO_MASK;
  uint32_t hartselhi = ((hartselVal >> HARTSELLO_SIZE) << HARTSELHI_OFFSET) & HARTSELHI_MASK;
  mDmcontrolReg |= hartselhi | hartsello;
}

/// \brief Set the \c hartsello and \c hartselhi bits of \c dmcontrol to their
///        maximum possible value.
///
/// Sets \c hartslehi to 0xf3ff
/// Sets \c hartsello to 0xf3ff
void
Dmi::Dmcontrol::hartselMax ()
{
  mDmcontrolReg |= HARTSELHI_MASK | HARTSELLO_MASK;
}
/// \brief Set the \c setresethaltreq bit in \c dmcontrol to 1 (NOT
///        IMPLEMENTED).
///
/// \warning Not implemented for the CORE-V MCU debug unit. Ignored with a
///          warning.
void
Dmi::Dmcontrol::setresethaltreq ()
{
  cerr << "Warning: Setting dmcontrol:setresethaltreq not supported: ignored."
       << endl;
}

/// \brief Set the \c clrresethaltreq bit in \c dmcontrol to 1 (NOT
///        IMPLEMENTED).
///
/// \warning Not implemented for the CORE-V MCU debug unit. Ignored with a
///          warning.
void
Dmi::Dmcontrol::clrresethaltreq ()
{
  cerr << "Warning: Setting dmcontrol:clrresethaltreq not supported: ignored."
       << endl;
}

/// \brief Get the \c ndmreset bit in \c dmcontrol
///
/// return \c true if the \c ndmreset bit is set and \c false otherwise.
bool
Dmi::Dmcontrol::ndmreset () const
{
  return (mDmcontrolReg & NDMRESET_MASK) != 0;
}

/// \brief Set the \c ndmreset bit in \c dmcontrol
///
/// \param[in] flag  If \c true sets the \c ndmreset bit, otherwise clears the
///                  \c ndmreset bit.
void
Dmi::Dmcontrol::ndmreset (const bool flag)
{
  if (flag)
    mDmcontrolReg |= NDMRESET_MASK;
  else
    mDmcontrolReg &= ~NDMRESET_MASK;
}

/// \brief Get the \c dmactive bit in \c dmcontrol
///
/// return \c true if the \c dmactive bit is set and \c false otherwise.
bool
Dmi::Dmcontrol::dmactive () const
{
  return (mDmcontrolReg & DMACTIVE_MASK) != 0;
}

/// \brief Set the \c dmactive bit in \c dmcontrol
///
/// \param[in] flag  If \c true sets the \c dmactive bit, otherwise clears the
///                  \c dmactive bit.
void
Dmi::Dmcontrol::dmactive (const bool flag)
{
  if (flag)
    mDmcontrolReg |= DMACTIVE_MASK;
  else
    mDmcontrolReg &= ~DMACTIVE_MASK;
}

/*******************************************************************************
 *                                                                             *
 * The Dmi::Dmstatus class                                                     *
 *                                                                             *
 ******************************************************************************/

/// \brief constructor for the Dmi::Dmstatus class
///
/// \param[in] dtm_  The DTM we shall use.  This is a unique_ptr owned by the
///                  Dmi, so passed and stored by reference.
Dmi::Dmstatus::Dmstatus (unique_ptr<IDtm> & dtm_) :
  mDtm (dtm_), mDmstatusReg (0)
{
}

/// \brief Read the value of the \c dmstatus register.
///
/// The register is refreshed via the DTM.
void
Dmi::Dmstatus::read ()
{
  mDmstatusReg = mDtm->dmiRead (DMI_ADDR);
  // cout << "dmstatus: 0x" << setw(8) << setfill ('0') << hex << mDmstatusReg
  //      << dec << setfill (' ') << setw (0) << endl;
}

/// Get the \c impebreak field of the \c dmstatus register.
///
/// \return \c true if the \c impebreak field of \c dmstatus is set, \c false
///         otherwise.
bool
Dmi::Dmstatus::impebreak () const
{
  return (mDmstatusReg & IMPEBREAK_MASK) != 0;
}

/// Get the \c havereset fields of the \c dmstatus register.
///
/// \note we do not distinguish between "all" or "any" versions of the flag.
///
/// \return \c true if either of the \c allhavereset of \c anyhavereset
///         fields of \c dmstatus is set, \c false otherwise.
bool
Dmi::Dmstatus::havereset () const
{
  return (mDmstatusReg & (ALLHAVERESET_MASK | ANYHAVERESET_MASK)) != 0;
}

/// Get the \c resumeack fields of the \c dmstatus register.
///
/// \note we do not distinguish between "all" or "any" versions of the flag.
///
/// \return \c true if either of the \c allresumeack of \c anyresumeack
///         fields of \c dmstatus is set, \c false otherwise.
bool
Dmi::Dmstatus::resumeack () const
{
  return (mDmstatusReg & (ALLRESUMEACK_MASK | ANYRESUMEACK_MASK)) != 0;
}

/// Get the \c nonexistent fields of the \c dmstatus register.
///
/// \note we do not distinguish between "all" or "any" versions of the flag.
///
/// \return \c true if either of the \c allnonexistent of \c anynonexistent
///         fields of \c dmstatus is set, \c false otherwise.
bool
Dmi::Dmstatus::nonexistent () const
{
  return (mDmstatusReg & (ALLNONEXISTENT_MASK | ANYNONEXISTENT_MASK)) != 0;
}

/// Get the \c unavail fields of the \c dmstatus register.
///
/// \note we do not distinguish between "all" or "any" versions of the flag.
///
/// \return \c true if either of the \c allunavail of \c anyunavail
///         fields of \c dmstatus is set, \c false otherwise.
bool
Dmi::Dmstatus::unavail () const
{
  return (mDmstatusReg & (ALLUNAVAIL_MASK | ANYUNAVAIL_MASK)) != 0;
}

/// Get the \c running fields of the \c dmstatus register.
///
/// \note we do not distinguish between "all" or "any" versions of the flag.
///
/// \return \c true if either of the \c allrunning of \c anyrunning
///         fields of \c dmstatus is set, \c false otherwise.
bool
Dmi::Dmstatus::running () const
{
  return (mDmstatusReg & (ALLRUNNING_MASK | ANYRUNNING_MASK)) != 0;
}

/// Get the \c halted fields of the \c dmstatus register.
///
/// \note we do not distinguish between "all" or "any" versions of the flag.
///
/// \return \c true if either of the \c allhalted of \c anyhalted
///         fields of \c dmstatus is set, \c false otherwise.
bool
Dmi::Dmstatus::halted () const
{
  return (mDmstatusReg & (ALLHALTED_MASK | ANYHALTED_MASK)) != 0;
}

/// Get the \c authenticated field of the \c dmstatus register.
///
/// \return \c true if the \c authenticated field of \c dmstatus is set,
///         \c false otherwise.
bool
Dmi::Dmstatus::authenticated () const
{
  return (mDmstatusReg & AUTHENTICATED_MASK) != 0;
}

/// Get the \c authbusy field of the \c dmstatus register.
///
/// \return \c true if the \c authbusy field of \c dmstatus is set, \c false
///         otherwise.
bool
Dmi::Dmstatus::authbusy () const
{
  return (mDmstatusReg & AUTHBUSY_MASK) != 0;
}

/// Get the \c hasresethaltreq field of the \c dmstatus register.
///
/// \return \c true if the \c hasresethaltreq field of \c dmstatus is set,
///         \c false otherwise.
bool
Dmi::Dmstatus::hasresethaltreq () const
{
  return (mDmstatusReg & HASRESETHALTREQ_MASK) != 0;
}

/// Get the \c confstrptrvalid field of the \c dmstatus register.
///
/// \return \c true if the \c confstrptrvalid field of \c dmstatus is set,
///         \c false otherwise.
bool
Dmi::Dmstatus::confstrptrvalid () const
{
  return (mDmstatusReg & CONFSTRPTRVALID_MASK) != 0;
}

/// Get the \c version field of the \c dmstatus register.
///
/// \return the value in the \c version field of \c dmstatus.
uint8_t
Dmi::Dmstatus::version () const
{
  return (mDmstatusReg & VERSION_MASK) >> VERSION_OFFSET;
}
