// CV32E40 ITarget interface implementation
//
// This file is part of the Embecosm Debug Server targets.
//
// Copyright (C) 2020 Embecosm Limited
// SPDX-License-Identifier: MIT
// ----------------------------------------------------------------------------

#include "Cv32e40.h"
#include "embdebug/Compat.h"
#include "embdebug/ITarget.h"

using namespace EmbDebug;

// Instantiate the model. TODO the argument will change to pass in the
// residual argv.
Cv32e40::Cv32e40(const TraceFlags *traceFlags EMBDEBUG_ATTR_UNUSED)
  : ITarget(traceFlags) { return; }

// Clean up the model
Cv32e40::~Cv32e40() { return; }

// Force termination of the model execution
ITarget::ResumeRes Cv32e40::terminate() { return ITarget::ResumeRes::NONE; }

// Reset the model state
ITarget::ResumeRes Cv32e40::reset(ITarget::ResetType) {
  return ITarget::ResumeRes::SUCCESS;
}

// Count cycles
uint64_t Cv32e40::getCycleCount() const { return 0; }

// Count instructions
uint64_t Cv32e40::getInstrCount() const { return 0; }

// How many registers do we have?
int Cv32e40::getRegisterCount() const { return 32; }

// How big is each register in bytes
int Cv32e40::getRegisterSize() const { return 4; }

// Read a register into the VALUE reference argument, returning the number of
// bytes read
std::size_t Cv32e40::readRegister(const int reg EMBDEBUG_ATTR_UNUSED,
				   uint_reg_t &value) {
  value = 0;
  return getRegisterSize();
}

// This is the mechanism for handling host file I/O. It provides a mechanism
// for supplying arguments and returning results. In this template we report
// that the syscall number is in register 1, the arguments in registers 2-4
// and the result in register 5. Consult your ABI to determine the correct
// values for a real architecture. Return if this was successful or not.
bool Cv32e40::getSyscallArgLocs(SyscallArgLoc &syscallIDLoc,
				 std::vector<SyscallArgLoc> &syscallArgLocs,
				 SyscallArgLoc &syscallReturnLoc) const {
  syscallIDLoc = SyscallArgLoc::RegisterLoc({SyscallArgLocType::REGISTER, 1});

  syscallArgLocs.clear();
  syscallArgLocs.push_back(SyscallArgLoc::RegisterLoc({SyscallArgLocType::REGISTER, 2}));
  syscallArgLocs.push_back(SyscallArgLoc::RegisterLoc({SyscallArgLocType::REGISTER, 3}));
  syscallArgLocs.push_back(SyscallArgLoc::RegisterLoc({SyscallArgLocType::REGISTER, 4}));

  // Return value in a0
  syscallReturnLoc = SyscallArgLoc::RegisterLoc({SyscallArgLocType::REGISTER, 5});
  return true;
}

// Write a register supplied in the VALUE argument and returning the number of
// bytes written
std::size_t Cv32e40::writeRegister(const int reg EMBDEBUG_ATTR_UNUSED,
				    const uint_reg_t value EMBDEBUG_ATTR_UNUSED) {
  return getRegisterSize();
}

// Read a block of memory into the supplied buffer, returning the number of
// bytes read
std::size_t Cv32e40::read(const uint_addr_t addr EMBDEBUG_ATTR_UNUSED,
			   uint8_t *buffer EMBDEBUG_ATTR_UNUSED,
			   const std::size_t size) {
  return size;
}

// Write a block of memory from the supplied buffer, returning the number of
// bytes read
std::size_t Cv32e40::write(const uint_addr_t addr EMBDEBUG_ATTR_UNUSED,
			    const uint8_t *buffer EMBDEBUG_ATTR_UNUSED,
			    const std::size_t size) {
  return size;
}

// Insert a matchpoint (breakpoint or watchpoint), returning whether or not
// this succeeded
bool Cv32e40::insertMatchpoint(const uint_addr_t EMBDEBUG_ATTR_UNUSED addr,
				const MatchType EMBDEBUG_ATTR_UNUSED matchType) {
  return false;
}

// Delete a matchpoint (breakpoint or watchpoint), returning whether or not
// this succeeded
bool Cv32e40::removeMatchpoint(const uint_addr_t EMBDEBUG_ATTR_UNUSED addr,
                             const MatchType EMBDEBUG_ATTR_UNUSED matchType) {
  return false;
}

// Passthru' a command to the target, returning whether or not this succeeded.
bool Cv32e40::command(const std::string EMBDEBUG_ATTR_UNUSED cmd,
		       std::ostream EMBDEBUG_ATTR_UNUSED &stream) {
  return false;
}

// Return the time taken by the CPU so far in seconds
double Cv32e40::timeStamp() { return 0.0; }

// Return the number of CPUs
unsigned int Cv32e40::getCpuCount(void) { return 1; }

// Return the curent CPU (must be consistent with the number of CPUs, use -1
// to indicate an invalid response
unsigned int Cv32e40::getCurrentCpu(void) { return 0; }

// Specify the current CPU
void Cv32e40::setCurrentCpu(unsigned int num) {
  assert(num == 0);
  return;
}


// Prepare each core to be resumed. The supplied vector, ACTIONS, says what
// each core should do when resume is next called. Return whether this
// succeeded.
bool Cv32e40::prepare(const std::vector<ITarget::ResumeType> &actions EMBDEBUG_ATTR_UNUSED) {
  return true;
}

// Resume each core, according to what was specified in the earlier call to
// prepare. Return whether this succeeded.
bool Cv32e40::resume(void) {
  return true;
}

// Clock the model waiting for any core to stop
ITarget::WaitRes Cv32e40::wait(std::vector<ResumeRes> &results EMBDEBUG_ATTR_UNUSED) {
  return ITarget::WaitRes::TIMEOUT;
}

// Force all cores to halt, returning whether this was successful
bool Cv32e40::halt(void) { return true; }

// Entry point for the shared library
extern "C" {
// Create and return a new model
EMBDEBUG_VISIBLE_API ITarget *create_target(TraceFlags *traceFlags) {
  return new Cv32e40(traceFlags);
}
// Used to ensure API compatibility
EMBDEBUG_VISIBLE_API uint64_t ITargetVersion() {
  return ITarget::CURRENT_API_VERSION;
}
}
