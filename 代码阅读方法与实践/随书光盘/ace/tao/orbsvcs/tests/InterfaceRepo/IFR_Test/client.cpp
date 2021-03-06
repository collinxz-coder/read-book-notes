// -*- C++ -*-
// client.cpp,v 1.1 2000/07/06 17:47:25 parsons Exp

#include "Admin_Client.h"

ACE_RCSID(IFR_Test, client, "client.cpp,v 1.1 2000/07/06 17:47:25 parsons Exp")

int main (int argc, char *argv[])
{
  Admin_Client admin_client;

  ACE_DEBUG ((LM_DEBUG,
              "\n\t IFR_Test \n\n"));

  int retval = admin_client.init (argc, 
                                  argv);

  if (retval == -1)
    return 1;

  retval = admin_client.run ();

  return retval;
}
