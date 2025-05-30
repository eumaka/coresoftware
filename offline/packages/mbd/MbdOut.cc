#include "MbdOut.h"
#include "MbdReturnCodes.h"

#include <iostream>
#include <iomanip>
#include <limits>

void MbdOut::identify(std::ostream& os) const
{
  os << "virtual MbdOut object" << std::endl;
  return;
}

void MbdOut::Reset()
{
  std::cout << "ERROR MbdOut: Reset() not implemented by daughter class" << std::endl;
  return;
}

int MbdOut::isValid() const
{
  virtual_warning("isValid()");
  return 0;
}

Float_t MbdOut::get_zvtx() const
{
  virtual_warning("get_zvtx()");
  return std::numeric_limits<Float_t>::quiet_NaN();
}

Float_t MbdOut::get_zvtxerr() const
{
  virtual_warning("get_zvtxerr()");
  return std::numeric_limits<Float_t>::quiet_NaN();
}

Float_t MbdOut::get_t0() const
{
  virtual_warning("get_t0()");
  return std::numeric_limits<Float_t>::quiet_NaN();
}

//__________________________________________
Float_t MbdOut::get_t0err() const
{
  virtual_warning("get_t0err()");
  return std::numeric_limits<Float_t>::quiet_NaN();
}

//__________________________________________
void MbdOut::set_t0(const Float_t /*unused*/, const Float_t /*unused*/)
{
  virtual_warning("set_t0(const Float_t t0, const Float_t t0err)");
  return;
}

//__________________________________________
void MbdOut::set_zvtx(const Float_t /*unused*/, const Float_t /*unused*/)
{
  virtual_warning("set_zvtx(const Float_t vtx, const Float_t vtxerr)");
  return;
}

//__________________________________________
void MbdOut::set_zvtxerr(const Float_t /*unused*/)
{
  virtual_warning("set_zvtxerr(const Float_t vtxerr)");
  return;
}

//________________________________________________________________
void MbdOut::set_arm(const int /*iarm*/, const Short_t /*npmt*/, const Float_t /*energy*/, const Float_t /*timing*/)
{
  virtual_warning("set_arm(const int iMBD, const Short_t npmt, const Float_t energy, const Float_t timing)");
  return;
}

void MbdOut::set_clocks(const Int_t /*evt*/, const UShort_t /*iclk*/, const UShort_t /*ifemclk*/)
{
  virtual_warning("set_clocks(const Int_t ievt, const UShort_t iclk, const UShort_t ifemclk");
  return;
}

Short_t MbdOut::get_npmt(const int /*iarm*/) const
{
  virtual_warning("get_npmt(const int iarm)");
  return MbdReturnCodes::MBD_INVALID_SHORT;
}

Float_t MbdOut::get_q(const int /*iarm*/) const
{
  virtual_warning("get_q(const int iarm)");
  return std::numeric_limits<Float_t>::quiet_NaN();
}

Float_t MbdOut::get_time(const int /*iarm*/) const
{
  virtual_warning("get_time(const int iarm)");
  return std::numeric_limits<Float_t>::quiet_NaN();
}

Int_t MbdOut::get_evt() const
{
  virtual_warning("get_evt()");
  return 0;
}

UShort_t MbdOut::get_clock() const
{
  virtual_warning("get_clock()");
  return 0;
}

UShort_t MbdOut::get_femclock() const
{
  virtual_warning("get_femclock(const int iarm)");
  return 0;
}

void MbdOut::virtual_warning(const std::string& funcsname) 
{
  std::cout << "MbdOut::" << funcsname << " is virtual, doing nothing" << std::endl;
  return;
}

void MbdOut::FillFromClass(const MbdOut& old)
{
  for (int iarm = 0; iarm < 2; iarm++)
  {
    set_arm(iarm, old.get_npmt(iarm), old.get_q(iarm), old.get_time(iarm));
  }

  set_t0zvtx(old.get_t0(), old.get_t0err(), old.get_zvtx(), old.get_zvtxerr());
}

void MbdOut::Print(Option_t * /*option*/) const
{
  std::cout << "MbdOut, evt " << get_evt() << std::endl;
  std::cout << "clk\t" << std::setw(12) << get_clock() << std::setw(12) << get_femclock() << std::endl;
  std::cout << "zvtx\t" << std::setw(12) << get_zvtx() << std::setw(12) << get_zvtxerr() << std::endl;
  std::cout << "t0\t" << std::setw(12) << get_t0() << std::setw(12) << get_t0err() << std::endl;
  std::cout << "npmt\t" << std::setw(12) << get_npmt(0) << std::setw(12) << get_npmt(1) << std::endl;
  std::cout << "time\t" << std::setw(12) << get_time(0) << std::setw(12) << get_time(1) << std::endl;
}
