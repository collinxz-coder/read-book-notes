// server.cpp,v 1.2 2001/04/24 08:02:57 coryan Exp

#include "AMI_Buffering.h"
#include "ace/Get_Opt.h"

ACE_RCSID(AMI_Buffering, server, "server.cpp,v 1.2 2001/04/24 08:02:57 coryan Exp")

const char *ior_output_file = "server.ior";
const char *ior = "file://admin.ior";

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "o:k:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':
	ior_output_file = get_opts.optarg;
	break;

      case 'k':
	ior = get_opts.optarg;
	break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
			   "-o <iorfile> "
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

      CORBA::Object_var poa_object =
        orb->resolve_initial_references("RootPOA", ACE_TRY_ENV);
      ACE_TRY_CHECK;

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (poa_object.in (), ACE_TRY_ENV);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (root_poa.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Panic: nil RootPOA\n"),
                          1);

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      if (parse_args (argc, argv) != 0)
        return 1;

      CORBA::Object_var tmp =
        orb->string_to_object(ior, ACE_TRY_ENV);
      ACE_TRY_CHECK;

      Test::AMI_Buffering_Admin_var admin =
        Test::AMI_Buffering_Admin::_narrow(tmp.in (), ACE_TRY_ENV);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (admin.in ()))
        {
          ACE_ERROR_RETURN ((LM_DEBUG,
                             "Nil AMI_Buffering_Admin reference <%s>\n",
                             ior),
                            1);
        }

      AMI_Buffering *AMI_buffering_impl;
      ACE_NEW_RETURN (AMI_buffering_impl,
                      AMI_Buffering (orb.in (),
                                        admin.in ()),
                      1);
      PortableServer::ServantBase_var owner_transfer(AMI_buffering_impl);

      Test::AMI_Buffering_var AMI_buffering =
        AMI_buffering_impl->_this (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      CORBA::String_var ior =
	orb->object_to_string (AMI_buffering.in (), ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // If the ior_output_file exists, output the ior to it
      FILE *output_file= ACE_OS::fopen (ior_output_file, "w");
      if (output_file == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Cannot open output file for writing IOR: %s",
                           ior_output_file),
                          1);
      ACE_OS::fprintf (output_file, "%s", ior.in ());
      ACE_OS::fclose (output_file);

      poa_manager->activate (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      orb->run (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG, "(%P|%t) server - event loop finished\n"));

      root_poa->destroy (1, 1, ACE_TRY_ENV);
      ACE_TRY_CHECK;

      orb->destroy (ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Exception caught:");
      return 1;
    }
  ACE_ENDTRY;

  return 0;
}
