/*
  Palabos case timer

  Error handling - a single exception will do

  Palabos case timer is free software released under the GPLv3

  author: Philippe Seil (philippe.seil@jku.at)
*/

#ifndef PLB_CASE_TIMER_EXCEPTION_H
#define PLB_CASE_TIMER_EXCEPTION_H

#include <exception>
#include <string>

namespace plb {
  class PlbCaseTimerException : std::exception
  {
  public:
    PlbCaseTimerException(char const *errmsg_)
      : errmsg(errmsg_) {}
    virtual ~PlbCaseTimerException() throw() {}
    virtual const char* what() const throw()
    {
      return errmsg.c_str();
    }
  private:
    std::string errmsg;
  };
}
#endif // PLB_CASE_TIMER_EXCEPTION_H
