// -*- C++ -*-
//
// BiDirPolicyC.i,v 1.3 2001/04/24 13:44:43 parsons Exp

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


#if !defined (_BIDIRPOLICY_BIDIRECTIONALPOLICY___CI_)
#define _BIDIRPOLICY_BIDIRECTIONALPOLICY___CI_

ACE_INLINE BiDirPolicy::BidirectionalPolicy_ptr
tao_BiDirPolicy_BidirectionalPolicy_duplicate (
    BiDirPolicy::BidirectionalPolicy_ptr p
  )
{
  return BiDirPolicy::BidirectionalPolicy::_duplicate (p);
}

ACE_INLINE void
tao_BiDirPolicy_BidirectionalPolicy_release (
    BiDirPolicy::BidirectionalPolicy_ptr p
  )
{
  CORBA::release (p);
}

ACE_INLINE BiDirPolicy::BidirectionalPolicy_ptr
tao_BiDirPolicy_BidirectionalPolicy_nil (
    void
  )
{
  return BiDirPolicy::BidirectionalPolicy::_nil ();
}

ACE_INLINE BiDirPolicy::BidirectionalPolicy_ptr
tao_BiDirPolicy_BidirectionalPolicy_narrow (
    CORBA::Object *p,
    CORBA::Environment &ACE_TRY_ENV
  )
{
  return BiDirPolicy::BidirectionalPolicy::_narrow (p, ACE_TRY_ENV);
}

ACE_INLINE CORBA::Object *
tao_BiDirPolicy_BidirectionalPolicy_upcast (
    void *src
  )
{
  BiDirPolicy::BidirectionalPolicy **tmp =
    ACE_static_cast (BiDirPolicy::BidirectionalPolicy **, src);
  return *tmp;
}


#endif /* end #if !defined */

