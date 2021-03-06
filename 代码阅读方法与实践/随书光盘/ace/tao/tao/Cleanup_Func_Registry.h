// -*- C++ -*-

// ===================================================================
/**
 *  @file   Cleanup_Func_Registry.h
 *
 *  Cleanup_Func_Registry.h,v 1.2 2001/03/26 21:17:07 coryan Exp
 *
 *  @author Ossama Othman <ossama@uci.edu>
 */
// ===================================================================

#ifndef TAO_CLEANUP_FUNC_REGISTRY_H
#define TAO_CLEANUP_FUNC_REGISTRY_H
#include "ace/pre.h"

#include "corbafwd.h"
#include "ace/Array_Base.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */


/**
 * @class TAO_Cleanup_Func_Registry
 *
 * @brief
 * This is a helper class that is designed to perform cleanup on
 * thread-specific objects registered in the ORB Core TSS resources by
 * invoking the corresponding cleanup function on each object.  Hence,
 * there is a tight coupling between this class and the TAO ORB Core.
 */
class TAO_Export TAO_Cleanup_Func_Registry
{
  friend class TAO_ORBInitInfo;
  friend class TAO_ORB_Core_TSS_Resources;
  friend class TAO_ORB_Core;

public:

  /// Constructor
  TAO_Cleanup_Func_Registry (void);

  /// Destructor
  ~TAO_Cleanup_Func_Registry (void);

  /// Return the number of registered cleanup functions.
  size_t size (void) const;

protected:

  /// Register a cleanup function.  The number of slot the cleanup
  /// function is placed is in will match the one reserved for the
  /// corresponding thread specific object in the ORB Core TSS
  /// resources.  The slot_id is returned via the second reference
  /// argument.  This method returns 0 on failure, and -1 on failure.
  int register_cleanup_function (ACE_CLEANUP_FUNC func,
                                 size_t &slot_id);

  /// Invoke the corresponding cleanup function on each
  /// thread-specific object.
  void cleanup (ACE_Array_Base<void *> &ts_objects);

private:

  /// Prevent copying through the copy constructor and the assignment
  /// operator.
  ACE_UNIMPLEMENTED_FUNC (
    TAO_Cleanup_Func_Registry (const TAO_Cleanup_Func_Registry &))
  ACE_UNIMPLEMENTED_FUNC (void operator= (const TAO_Cleanup_Func_Registry &))

private:

  /// Array of registered cleanup functions.  The number of
  /// registered cleanup functions should be the same as the number
  /// of registered thread-specific objects in the ORB Core TSS
  /// resources.
  ACE_Array_Base<ACE_CLEANUP_FUNC> cleanup_funcs_;

};

#if defined (__ACE_INLINE__)
# include "Cleanup_Func_Registry.inl"
#endif  /* __ACE_INLINE__ */


#include "ace/post.h"

#endif  /* TAO_CLEANUP_FUNC_REGISTRY_H */
