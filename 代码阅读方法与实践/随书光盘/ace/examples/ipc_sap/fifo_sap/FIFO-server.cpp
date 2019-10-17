// FIFO-server.cpp,v 4.4 2000/05/26 18:08:44 othman Exp

#include "ace/FIFO_Recv.h"
#include "ace/Log_Msg.h"

ACE_RCSID(FIFO_SAP, FIFO_server, "FIFO-server.cpp,v 4.4 2000/05/26 18:08:44 othman Exp")

int 
main (int, char *[])
{
  ACE_OS::unlink (ACE_DEFAULT_RENDEZVOUS);
  ACE_FIFO_Recv server (ACE_DEFAULT_RENDEZVOUS);
  char buf[BUFSIZ];
  int n;

  while ((n = server.recv (buf, sizeof buf)) > 0)
    {
      ACE_OS::printf ("%4d: ", n);
      ACE_OS::fflush (stdout);
      ACE_OS::write (ACE_STDOUT, buf, n);
    }

  if (n == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "recv"), 1);

  return 0;
}
