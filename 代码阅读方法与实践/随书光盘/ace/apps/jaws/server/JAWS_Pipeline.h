/* -*- c++ -*- */
// JAWS_Pipeline.h,v 1.4 1998/10/20 02:34:34 levine Exp

#ifndef JAWS_PIPELINE_H
#define JAWS_PIPELINE_H

#include "ace/Synch.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Service_Config.h"
#include "ace/Stream.h"
#include "ace/Module.h"
#include "ace/Task.h"

typedef ACE_Stream<ACE_NULL_SYNCH> JAWS_Pipeline_Stream;
typedef ACE_Module<ACE_NULL_SYNCH> JAWS_Pipeline_Module;
typedef ACE_Task<ACE_NULL_SYNCH> JAWS_Pipeline_Task;

class JAWS_Pipeline : public JAWS_Pipeline_Task
  // = TITLE
  //   Methods that are common to pipeline components
{
public:
  JAWS_Pipeline (void);
  // ACE_Task hooks

  virtual int open (void * = 0);
  virtual int close (u_long = 0);
};

#include "JAWS_Pipeline_Handler.h"

#endif /* !defined (JAWS_PIPELINE_H) */
