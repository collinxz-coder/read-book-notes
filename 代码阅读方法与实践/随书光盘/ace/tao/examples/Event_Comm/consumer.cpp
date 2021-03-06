// consumer.cpp,v 1.9 1998/11/08 03:07:06 pradeep Exp

#include "Consumer_Handler.h"
#include "Consumer_Input_Handler.h"

ACE_RCSID(Consumer, consumer, "consumer.cpp,v 1.9 1998/11/08 03:07:06 pradeep Exp")

class Consumer : public ACE_Event_Handler, public ShutdownCallback
{
  // = TITLE
  //    Consumer driver for the Publish/Subscribe example.
  //
  // = DESCRIPTION
  //    The Consumer holds the <Consumer_Input_Handler> and
  //    <Cosumer_Handler> objects.
public:
  // = Initialization and termination methods.
  Consumer (void);
  // Constructor.

  ~Consumer (void);
  // Destructor.

  int initialize (int argc, char *argv[]);
  // Initialization method.

  int run (void);
  // Execute the consumer;

  virtual void close (void);
  // Shutdown the consumer.

private:
  virtual int handle_signal (int signum, siginfo_t *, ucontext_t *);
  // Signal handler method.

  Consumer_Input_Handler ih_;
  // Handler for keyboard input.

  Consumer_Handler ch_;
  // Handler for CORBA Consumer.
};

Consumer::Consumer (void)
{
  // No-Op.
}

Consumer::~Consumer (void)
{
  // No-Op.
}

int
Consumer::handle_signal (int signum,
                         siginfo_t *,
                         ucontext_t *)
{
  ACE_DEBUG ((LM_DEBUG,
              "%S\n",
              signum));

  // Indicate that the consumer initiated the shutdown.
  this->ih_.consumer_initiated_shutdown (1);

  this->close ();

  return 0;
}

void
Consumer::close (void)
{
  // clean up the input handler.
  ih_.close ();
 // Shut down the ORB
  ch_.close ();
}

int
Consumer::run (void)
{
  // Run the <Consumer_Handler>'s ORB.
  return ch_.run ();
}

int
Consumer::initialize (int argc, char *argv[])
{
  // Initialize the <Consumer_Handler>.
  if (this->ch_.init (argc, argv, this) == -1)
     ACE_ERROR_RETURN ((LM_ERROR,
			"%p\n",
			"Consumer_Handler failed to initialize\n"),
                       -1);
   // Initialize the <Consumer_Input_Handler>.
  else if (this->ih_.initialize (&this->ch_) == -1)
     ACE_ERROR_RETURN ((LM_ERROR,
			"%p\n",
			"Consumer_Input_Handler failed to initialize\n"),
                       -1);
  else if (this->ch_.reactor ()->register_handler (SIGINT,
                                              this) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
		       "%p\n",
		       "register_handler"),
                      -1);
  else
    return 0;
}

int
main (int argc, char *argv[])
{
  // Initialize the supplier and consumer object references.
  Consumer consumer;

  if (consumer.initialize (argc, argv) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
		       "%p\n",
		       "Consumer init failed\n"),
		      1);

  // Loop forever handling events.
  if (consumer.run () == -1)
  ACE_ERROR_RETURN ((LM_ERROR,
		       "%p\n",
		       "Consumer run failed\n"),
		      1);

  return 0;
}
