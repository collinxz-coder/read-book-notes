// -*- C++ -*-
//
// DomainS.i,v 1.3 2001/05/31 04:50:09 othman Exp

// ****  Code generated by the The ACE ORB (TAO) IDL Compiler ****
// TAO and the TAO IDL Compiler have been developed by:
//       Center for Distributed Object Computing
//       Washington University
//       St. Louis, MO
//       USA
//       http://www.cs.wustl.edu/~schmidt/doc-center.html
// and
//       Distributed Object Computing Laboratory
//       University of California at Irvine
//       Irvine, CA
//       USA
//       http://doc.ece.uci.edu/
//
// Information about TAO is available at:
//     http://www.cs.wustl.edu/~schmidt/TAO.html

ACE_INLINE void POA_CORBA_ConstructionPolicy::_get_policy_type_skel (
    TAO_ServerRequest &req,
    void *obj,
    void *context,
    CORBA::Environment &ACE_TRY_ENV
  )
{
  POA_CORBA_Policy_ptr impl = (POA_CORBA_ConstructionPolicy_ptr) obj;
  POA_CORBA_Policy::_get_policy_type_skel (
      req,
      (POA_CORBA_Policy_ptr) impl,
      context,
      ACE_TRY_ENV
    );
}
ACE_INLINE void POA_CORBA_ConstructionPolicy::copy_skel (
    TAO_ServerRequest &req,
    void *obj,
    void *context,
    CORBA::Environment &ACE_TRY_ENV
  )
{
  POA_CORBA_Policy_ptr impl = (POA_CORBA_ConstructionPolicy_ptr) obj;
  POA_CORBA_Policy::copy_skel (
      req,
      (POA_CORBA_Policy_ptr) impl,
      context,
       ACE_TRY_ENV
    );
}
ACE_INLINE void POA_CORBA_ConstructionPolicy::destroy_skel (
    TAO_ServerRequest &req,
    void *obj,
    void *context,
    CORBA::Environment &ACE_TRY_ENV
  )
{
  POA_CORBA_Policy_ptr impl = (POA_CORBA_ConstructionPolicy_ptr) obj;
  POA_CORBA_Policy::destroy_skel (
      req,
      (POA_CORBA_Policy_ptr) impl,
      context,
       ACE_TRY_ENV
    );
}
