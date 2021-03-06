//=============================================================================
/**
 *  @file    RT_Collocation_Resolver.h
 *
 *  RT_Collocation_Resolver.h,v 1.4 2001/09/18 00:05:54 irfan Exp
 *
 *  @author  Irfan Pyarali
 */
// ===================================================================

#ifndef TAO_RT_COLLOCATION_RESOLVER_H
#define TAO_RT_COLLOCATION_RESOLVER_H

#include "ace/pre.h"
#include "tao/orbconf.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/RTPortableServer/rtportableserver_export.h"
#include "tao/Collocation_Resolver.h"
#include "ace/Service_Config.h"

/**
 * @class TAO_RT_Collocation_Resolver
 *
 * @brief This class helps decide on collocation among the maze of
 * thread pools and lanes.
 *
 * \nosubgrouping
 *
 **/
class TAO_RTPortableServer_Export TAO_RT_Collocation_Resolver :
  public TAO_Collocation_Resolver
{
public:

  /// Is <object> collocated?
  virtual CORBA::Boolean is_collocated (CORBA::Object_ptr object,
                                        CORBA::Environment &ACE_TRY_ENV) const;
};

ACE_STATIC_SVC_DECLARE_EXPORT (TAO_RTPortableServer, TAO_RT_Collocation_Resolver)
ACE_FACTORY_DECLARE (TAO_RTPortableServer, TAO_RT_Collocation_Resolver)

#if defined (__ACE_INLINE__)
# include "tao/RTPortableServer/RT_Collocation_Resolver.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"

#endif /* TAO_RT_COLLOCATION_RESOLVER_H */
