//
// Peer.cpp,v 1.2 2001/04/24 08:02:58 coryan Exp
//
#include "Peer.h"
#include "Session.h"

ACE_RCSID(Big_Oneways, Peer, "Peer.cpp,v 1.2 2001/04/24 08:02:58 coryan Exp")

Peer::Peer (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

Peer::~Peer (void)
{
}

Test::Session_ptr
Peer::create_session (Test::Session_Control_ptr control,
                      CORBA::ULong payload_size,
                      CORBA::ULong thread_count,
                      CORBA::ULong message_count,
                      CORBA::ULong peer_count,
                      CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  Session *session_impl;
  ACE_NEW_THROW_EX (session_impl,
                    Session (control,
                             payload_size,
                             thread_count,
                             message_count,
                             peer_count),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (Test::Session::_nil ());
  PortableServer::ServantBase_var transfer_ownership (session_impl);

  return session_impl->_this (ACE_TRY_ENV);
}

void
Peer::shutdown (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) Peer::shutdown, waiting for threads\n"));

  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) Peer::shutdown, shutting down ORB\n"));
  this->orb_->shutdown (0, ACE_TRY_ENV);
}
