/* -*- C++ -*- */
/* Timer.h,v 1.9 1999/09/23 16:29:21 levine Exp */

// ============================================================================
//
// = LIBRARY
//    TAO/tests
//
// = FILENAME
//    Timer.h
//
// = AUTHOR
//    Andy Gokhale, Sumedh Mungee, Sergio Flores-Gaitan and Nagarajan
//    Surendran.
//
// ============================================================================

#ifndef _MT_CUBIT_TIMER_H
#define _MT_CUBIT_TIMER_H

#include "ace/OS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/High_Res_Timer.h"

class MT_Cubit_Timer
{
  // = TITLE
  //     A class that encapsulates the pccTimer for chorus and uses
  //     ACE Timer for other platforms.
public:
  MT_Cubit_Timer (u_int granularity);

  void start (void);
  void stop (void);

  ACE_timer_t get_elapsed (void);

private:
  ACE_High_Res_Timer timer_;
  // timer.

  ACE_Time_Value delta_;
  // Elapsed time in microseconds.

  u_int granularity_;
  // This is the granularity of the timing of the CORBA requests. A
  // value of 5 represents that we will take time every 5 requests,
  // instead of the default of every request (1).

#if defined (CHORUS_MVME)
  // Variables for the pccTimer.
  int pstartTime_;
  int pstopTime_;
#endif /* CHORUS_MVME */
};

#endif /* _MT_CUBIT_TIMER_H */
