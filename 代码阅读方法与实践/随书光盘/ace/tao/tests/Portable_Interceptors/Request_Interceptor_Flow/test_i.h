// -*- C++ -*-

//=============================================================================
/**
 * @file test_i.h
 *
 * test_i.h,v 1.1 2001/04/09 23:27:06 othman Exp
 *
 * Implementation header for the "test" IDL interface for the
 * request interceptor flow test.
 *
 * @author Ossama Othman <ossama@uci.edu>
 */
//=============================================================================

#ifndef TEST_I_H
#define TEST_I_H

#include "testS.h"

/**
 * @class test_i
 *
 * @brief Simple test class.
 *
 * This class implements the "test" interface used in this test.
 */
class test_i : public virtual POA_Test
{
public:

  /// Constructor.
  test_i (CORBA::ORB_ptr orb);

  /// The client-side test operation.
  virtual void client_test (Test::TestScenario scenario,
                            CORBA::Environment &ACE_TRY_ENV =
                              TAO_default_environment ())
    ACE_THROW_SPEC ((Test::X,
                     Test::UnknownScenario,
                     CORBA::SystemException));

  /// The server-side test operation.
  virtual void server_test (Test::TestScenario scenario,
                            CORBA::Environment &ACE_TRY_ENV =
                              TAO_default_environment ())
    ACE_THROW_SPEC ((Test::X,
                     Test::UnknownScenario,
                     CORBA::SystemException));

  /// Shutdown the ORB.
  virtual void shutdown (CORBA::Environment &ACE_TRY_ENV =
                           TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

private:

  /// Pseudo-reference to the ORB.
  CORBA::ORB_var orb_;

};

#endif  /* TEST_I_H */
