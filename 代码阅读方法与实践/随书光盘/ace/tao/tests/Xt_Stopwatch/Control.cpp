// Control.cpp,v 1.4 2000/11/11 17:39:55 bala Exp
#include "Control.h"

#if defined (ACE_HAS_XT)

Control::Control (Widget &parent)
{
  // Instantiate the  sub-components of the Control
  this->frame_ = XmCreateFrame (parent,
                                "frame",
                                0,
                                0 );

  this->rowcolumn_ = XmCreateRowColumn (this->frame_,
                                        "rwc",
                                        0,
                                        0 );

  this->startwidget_ = XmCreatePushButton (this->rowcolumn_,
                                           "Start",
                                           0,
                                           0);

  this->stopwidget_ = XmCreatePushButton (this->rowcolumn_,
                                          "Stop",
                                          0,
                                          0);
}

Control::~Control (void)
{
}

void
Control::manage (void)
{
  XtManageChild (frame_);
  XtManageChild (rowcolumn_);
  XtManageChild (startwidget_);
  XtManageChild (stopwidget_);
}

Widget &
Control::startwidget (void)
{
  return this->startwidget_;
}

Widget &
Control::stopwidget (void)
{
  return this->stopwidget_;
}

#endif /*ACE_HAS_XT*/
