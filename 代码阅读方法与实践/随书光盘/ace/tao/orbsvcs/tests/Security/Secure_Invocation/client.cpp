// -*- C++ -*-

#include "ace/Get_Opt.h"

#include "FooC.h"
#include "orbsvcs/SecurityC.h"

ACE_RCSID (Secure_Invocation,
           client,
           "client.cpp,v 1.4 2001/07/09 22:41:10 parsons Exp")

const char *ior = "file://test.ior";

void
insecure_invocation_test (CORBA::ORB_ptr orb,
                          CORBA::Object_ptr obj,
                          CORBA::Environment &ACE_TRY_ENV)
{
  // Disable protection for this insecure invocation test.

  Security::QOP qop = Security::SecQOPNoProtection;

  CORBA::Any no_protection;
  no_protection <<= qop;

  // Create the Security::QOPPolicy.
  CORBA::Policy_var policy =
    orb->create_policy (Security::SecQOPPolicy,
                        no_protection,
                        ACE_TRY_ENV);
  ACE_CHECK;

  CORBA::PolicyList policy_list (1);
  policy_list.length (1);
  policy_list[0] = CORBA::Policy::_duplicate (policy.in ());

  // Create an object reference that uses plain IIOP (i.e. no
  // protection).
  CORBA::Object_var object =
    obj->_set_policy_overrides (policy_list,
                                CORBA::SET_OVERRIDE,
                                ACE_TRY_ENV);
  ACE_CHECK;

  Foo::Bar_var server =
    Foo::Bar::_narrow (object.in (), ACE_TRY_ENV);
  ACE_CHECK;

  if (CORBA::is_nil (server.in ()))
    {
      ACE_ERROR ((LM_ERROR,
                  "(%P|%t) ERROR: Object reference <%s> is "
                  "nil.\n",
                  ior));

      ACE_THROW (CORBA::INTERNAL ());
    }

  ACE_TRY
    {
      // This invocation should result in a CORBA::NO_PERMISSION
      // exception.
      server->baz (ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCH (CORBA::NO_PERMISSION, exc)
    {
      ACE_DEBUG ((LM_INFO,
                  "(%P|%t) Received CORBA::NO_PERMISSION from "
                  "server, as expected.\n"));

      return;
    }
  ACE_ENDTRY;
  ACE_CHECK;

  ACE_ERROR ((LM_ERROR,
              "(%P|%t) ERROR: CORBA::NO_PERMISSION was not thrown.\n"
              "(%P|%t) ERROR: It should have been thrown.\n"));

  ACE_THROW (CORBA::INTERNAL ());
}

void
secure_invocation_test (CORBA::Object_ptr object,
                        CORBA::Environment &ACE_TRY_ENV)
{
  Foo::Bar_var server =
    Foo::Bar::_narrow (object, ACE_TRY_ENV);
  ACE_CHECK;

  if (CORBA::is_nil (server.in ()))
    {
      ACE_ERROR ((LM_ERROR,
                  "(%P|%t) ERROR: Object reference <%s> is "
                  "nil.\n",
                  ior));

      ACE_THROW (CORBA::INTERNAL ());
    }

  // This invocation should return successfully.
  server->baz (ACE_TRY_ENV);
  ACE_CHECK;

  server->shutdown (ACE_TRY_ENV);
  ACE_CHECK;
}

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "k:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        ior = get_opts.optarg;
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Usage:  %s "
                           "-k <ior> "
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates sucessful parsing of the command line
  return 0;
}

int
main (int argc, char *argv[])
{
  ACE_TRY_NEW_ENV
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, "", ACE_TRY_ENV);
      ACE_TRY_CHECK;

      if (parse_args (argc, argv) != 0)
        return 1;

      CORBA::Object_var object =
        orb->string_to_object (ior, ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // This test sets creates a Security::QOPPolicy with the
      // Quality-of-Protection set to "no protection."  It then
      // invokes a method on the server (insecurely), which should
      // then result in a CORBA::NO_PERMISSION exception.
      //
      // The server is not shutdown by this test.
      insecure_invocation_test (orb.in (), object.in (), ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // This test uses the default secure SSLIOP settings to securely
      // invoke a method on the server.  No exception should occur.
      //
      // The server *is* shutdown by this test.
      secure_invocation_test (object.in (), ACE_TRY_ENV);
      ACE_TRY_CHECK;

      orb->destroy (ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Caught exception:");
      return 1;
    }
  ACE_ENDTRY;

  ACE_DEBUG ((LM_DEBUG,
              "\n"
              "Secure_Invocation test passed.\n"));

  return 0;
}
