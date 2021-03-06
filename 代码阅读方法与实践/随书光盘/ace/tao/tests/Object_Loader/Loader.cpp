// Loader.cpp,v 1.5 2001/05/23 12:20:44 bala Exp

#include "Loader.h"
#include "Test_i.h"

#if !defined(__ACE_INLINE__)
#include "Loader.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(Object_Loader, Loader, "Loader.cpp,v 1.5 2001/05/23 12:20:44 bala Exp")

Loader::Loader (void)
{
}

CORBA::Object_ptr
Loader::create_object (CORBA::ORB_ptr orb,
                       int,
                       char * [],
                       CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_TRY
    {
      CORBA::Object_var obj =
        orb->resolve_initial_references ("RootPOA", ACE_TRY_ENV);
      ACE_TRY_CHECK;

      PortableServer::POA_var poa =
        PortableServer::POA::_narrow (obj.in (), ACE_TRY_ENV);
      ACE_TRY_CHECK;

      PortableServer::POAManager_var mgr =
        poa->the_POAManager (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      mgr->activate (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      Test_i *test;
      ACE_NEW_RETURN (test, Test_i (poa.in ()),
                      CORBA::Object::_nil ());

      PortableServer::ServantBase_var tmp = test;
      obj = test->_this (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      return obj._retn ();
    }
  ACE_CATCHANY
    {
    }
  ACE_ENDTRY;
  return CORBA::Object::_nil ();
}

// ****************************************************************

#if 0
ACE_STATIC_SVC_DEFINE (Loader,
                       ACE_TEXT ("TAO_Object_Loader"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (Loader),
                       ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
                       0)
#endif /* 0 */

ACE_FACTORY_DEFINE (OLT, Loader)
