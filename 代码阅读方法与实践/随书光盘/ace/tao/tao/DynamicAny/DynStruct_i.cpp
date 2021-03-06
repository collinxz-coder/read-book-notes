/* -*- C++ -*- */
// DynStruct_i.cpp,v 1.14 2001/06/25 20:14:54 parsons Exp

#include "DynStruct_i.h"
#include "DynAnyFactory.h"
#include "tao/Marshal.h"

ACE_RCSID(DynamicAny, DynStruct_i, "DynStruct_i.cpp,v 1.14 2001/06/25 20:14:54 parsons Exp")

// Constructors and destructor.

TAO_DynStruct_i::TAO_DynStruct_i (void)
{
}

TAO_DynStruct_i::~TAO_DynStruct_i (void)
{
}

void
TAO_DynStruct_i::check_typecode (CORBA::TypeCode_ptr tc,
                                 CORBA::Environment &ACE_TRY_ENV)
{
  CORBA::TCKind kind = TAO_DynAnyFactory::unalias (tc, 
                                                   ACE_TRY_ENV);
  ACE_CHECK;

  if (kind == CORBA::tk_struct || kind == CORBA::tk_except)
    {
      return;
    }

  ACE_THROW (DynamicAny::DynAnyFactory::InconsistentTypeCode ());
}

void
TAO_DynStruct_i::init_common (void)
{
  this->ref_to_component_ = 0;
  this->container_is_destroying_ = 0;
  this->has_components_ = 1;
  this->destroyed_ = 0;
  this->component_count_ = this->da_members_.size ();
  this->current_position_ = this->component_count_ ? 0 : -1;
}

void
TAO_DynStruct_i::init (const CORBA_Any& any,
                       CORBA::Environment &ACE_TRY_ENV)
{
  CORBA::TypeCode_var tc = any.type ();
  this->check_typecode (tc.in (), 
                        ACE_TRY_ENV);
  ACE_CHECK;

  this->type_ = tc;

  // Member_count works with aliased type codes.
  CORBA::ULong numfields = this->type_->member_count (ACE_TRY_ENV);
  ACE_CHECK;

  // Resize the array.
  this->da_members_.size (numfields);

  this->init_common ();

  // Get the CDR stream of the argument.
  ACE_Message_Block *mb = any._tao_get_cdr ();

  TAO_InputCDR cdr (mb,
                    any._tao_byte_order ());

  // If we have an exception type, unmarshal the repository ID.
  CORBA::TCKind kind = TAO_DynAnyFactory::unalias (this->type_.in (),
                                                   ACE_TRY_ENV);
  ACE_CHECK;

  if (kind == CORBA::tk_except)
    {
      CORBA::String_var str;
      cdr >> str.out ();
    }

  // member_type() does not work with aliased type codes.
  CORBA::TypeCode_var unaliased_tc =
    TAO_DynAnyFactory::strip_alias (this->type_.in (),
                                    ACE_TRY_ENV);
  ACE_CHECK;

  for (CORBA::ULong i = 0; i < numfields; i++)
    {
      CORBA::TypeCode_var field_tc =
        unaliased_tc->member_type (i,
                                   ACE_TRY_ENV);
      ACE_CHECK;

      // This Any constructor is a TAO extension.
      CORBA_Any field_any (field_tc.in (),
                           0,
                           cdr.byte_order (),
                           cdr.start ());

      // This recursive step will call the correct constructor
      // based on the type of field_any.
      this->da_members_[i] =
        TAO_DynAnyFactory::make_dyn_any (field_any,
                                         ACE_TRY_ENV);
      ACE_CHECK;

      // Move to the next field in the CDR stream.
      (void) TAO_Marshal_Object::perform_skip (field_tc.in (),
                                               &cdr,
                                               ACE_TRY_ENV);
      ACE_CHECK;
    }
}

void
TAO_DynStruct_i::init (CORBA_TypeCode_ptr tc,
                       CORBA::Environment &ACE_TRY_ENV)
{
  this->check_typecode (tc, 
                        ACE_TRY_ENV);
  ACE_CHECK;

  this->type_ = CORBA::TypeCode::_duplicate (tc);

  CORBA::ULong numfields = tc->member_count (ACE_TRY_ENV);
  ACE_CHECK;

  // Resize the array.
  this->da_members_.size (numfields);

  this->init_common ();

  CORBA::TypeCode_var mtype;

  for (CORBA::ULong i = 0; i < numfields; i++)
    {
      mtype = tc->member_type (i,
                               ACE_TRY_ENV);
      ACE_CHECK;

      // Recursively initialize each member.
      this->da_members_[i] = TAO_DynAnyFactory::make_dyn_any (mtype.in (),
                                                              ACE_TRY_ENV);
      ACE_CHECK;
    }
}

// ****************************************************************

TAO_DynStruct_i *
TAO_DynStruct_i::_narrow (CORBA::Object_ptr obj,
                          CORBA::Environment &)
{
  if (CORBA::is_nil (obj))
    {
      return 0;
    }

  return ACE_reinterpret_cast (
             TAO_DynStruct_i*,
             obj->_tao_QueryInterface (
                      ACE_reinterpret_cast (
                          ptr_arith_t,
                          &TAO_DynStruct_i::_narrow
                        )
                    )
           );
}

void*
TAO_DynStruct_i::_tao_QueryInterface (ptr_arith_t type)
{
  ptr_arith_t mytype =
    ACE_reinterpret_cast (ptr_arith_t,
                          &TAO_DynStruct_i::_narrow);
  if (type == mytype)
    {
      this->_add_ref ();
      return this;
    }

  return 
    this->ACE_NESTED_CLASS (DynamicAny, DynStruct::_tao_QueryInterface) (type);
}

// ****************************************************************

DynamicAny::FieldName
TAO_DynStruct_i::current_member_name (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((
      CORBA::SystemException,
      DynamicAny::DynAny::TypeMismatch,
      DynamicAny::DynAny::InvalidValue
    ))
{
  if (this->destroyed_)
    {
      ACE_THROW_RETURN (CORBA::OBJECT_NOT_EXIST (),
                        0);
    }

  // Is this an empty exception?
  if (this->component_count_ == 0)
    {
      ACE_THROW_RETURN (DynamicAny::DynAny::TypeMismatch (),
                        0);
    }

  if (this->current_position_ == -1)
    {
      ACE_THROW_RETURN (DynamicAny::DynAny::InvalidValue (),
                        0);
    }

  // Member_name works with aliased type codes.
  const char *name =
    this->type_->member_name (this->current_position_, 
                              ACE_TRY_ENV);
  ACE_CHECK_RETURN (0);

  return CORBA::string_dup (name);
}

// Returns the unaliased TCKind.
CORBA::TCKind
TAO_DynStruct_i::current_member_kind (CORBA::Environment& ACE_TRY_ENV)
  ACE_THROW_SPEC ((
      CORBA::SystemException,
      DynamicAny::DynAny::TypeMismatch,
      DynamicAny::DynAny::InvalidValue
    ))
{
  if (this->destroyed_)
    {
      ACE_THROW_RETURN (CORBA::OBJECT_NOT_EXIST (),
                        CORBA::tk_null);
    }

  // Is this an empty exception?
  if (this->component_count_ == 0)
    {
      ACE_THROW_RETURN (DynamicAny::DynAny::TypeMismatch (),
                        CORBA::tk_null);
    }

  if (this->current_position_ == -1)
    {
      ACE_THROW_RETURN (DynamicAny::DynAny::InvalidValue (),
                        CORBA::tk_null);
    }

  CORBA::TypeCode_var tc = 
    this->type_.in ()->member_type (this->current_position_,
                                    ACE_TRY_ENV);
  ACE_CHECK_RETURN (CORBA::tk_null);

  CORBA::TCKind retval = TAO_DynAnyFactory::unalias (tc.in (),
                                                     ACE_TRY_ENV);
  ACE_CHECK_RETURN (CORBA::tk_null);

  return retval;
}

DynamicAny::NameValuePairSeq *
TAO_DynStruct_i::get_members (CORBA::Environment& ACE_TRY_ENV)
  ACE_THROW_SPEC ((
      CORBA::SystemException
    ))
{
  if (this->destroyed_)
    {
      ACE_THROW_RETURN (CORBA::OBJECT_NOT_EXIST (),
                        0);
    }

  DynamicAny::NameValuePairSeq* members;
  ACE_NEW_THROW_EX (members,
                    DynamicAny::NameValuePairSeq (this->component_count_),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (0);

  // We must do this explicitly.
  members->length (this->component_count_);

  DynamicAny::NameValuePairSeq_var safe_retval = members;
  CORBA::Any_var temp;

  // Assign name and value to each pearl on the string.
  for (CORBA::ULong i = 0; i < this->component_count_; ++i)
    {
      safe_retval[i].id =
        CORBA::string_dup (this->type_.in ()->member_name (i));

      temp = this->da_members_[i]->to_any (ACE_TRY_ENV);
      ACE_CHECK_RETURN (0);

      safe_retval[i].value = temp.in ();
    }

  return safe_retval._retn ();
}

void
TAO_DynStruct_i::set_members (const DynamicAny::NameValuePairSeq & values,
                              CORBA::Environment& ACE_TRY_ENV)
  ACE_THROW_SPEC ((
      CORBA::SystemException,
      DynamicAny::DynAny::TypeMismatch,
      DynamicAny::DynAny::InvalidValue
    ))
{
  if (this->destroyed_)
    {
      ACE_THROW (CORBA::OBJECT_NOT_EXIST ());
    }

  CORBA::ULong length = values.length ();

  // Check for length match.
  if (length != this->component_count_)
    {
      ACE_THROW (DynamicAny::DynAny::InvalidValue ());
    }

  CORBA::TypeCode_var value_tc;
  CORBA::TypeCode_var my_tc;
  CORBA::Boolean equivalent;

  // member_type() does not work with aliased type codes.
  CORBA::TypeCode_var unaliased_tc =
    TAO_DynAnyFactory::strip_alias (this->type_.in (),
                                    ACE_TRY_ENV);
  ACE_CHECK;

  for (CORBA::ULong i = 0; i < length; ++i)
    {
      // Check for type and name match.
      my_tc = unaliased_tc->member_type (i, 
                                         ACE_TRY_ENV);
      ACE_CHECK;

      value_tc = values[i].value.type ();

      equivalent = my_tc->equivalent (value_tc.in (),
                                      ACE_TRY_ENV);
      ACE_CHECK;

      if (!equivalent)
        {
          ACE_THROW (DynamicAny::DynAny::TypeMismatch ());
        }

      this->da_members_[i]->destroy (ACE_TRY_ENV);
      ACE_CHECK;

      this->da_members_[i] =
        TAO_DynAnyFactory::make_dyn_any (values[i].value,
                                         ACE_TRY_ENV);
      ACE_CHECK;
    }

  this->current_position_ = length ? 0 : -1;
}

DynamicAny::NameDynAnyPairSeq *
TAO_DynStruct_i::get_members_as_dyn_any (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((
      CORBA::SystemException
    ))
{
  if (this->destroyed_)
    {
      ACE_THROW_RETURN (CORBA::OBJECT_NOT_EXIST (),
                        0);
    }

  DynamicAny::NameDynAnyPairSeq * members;
  ACE_NEW_THROW_EX (members,
                    DynamicAny::NameDynAnyPairSeq (this->component_count_),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (0);

  // We must do this explicitly.
  members->length (this->component_count_);

  DynamicAny::NameDynAnyPairSeq_var safe_retval = members;

  // Assign name and value to each pearl on the string.
  for (CORBA::ULong i = 0; i < this->component_count_; ++i)
    {
      safe_retval[i].id =
        CORBA::string_dup (this->type_.in ()->member_name (i));

      // A deep copy is made only by copy() (CORBA 2.4.2 section 9.2.3.6).
      // Set the flag so the caller can't destroy.
      this->set_flag (this->da_members_[i].in (),
                      0,
                      ACE_TRY_ENV);
      ACE_CHECK_RETURN (0);

      safe_retval[i].value = 
        DynamicAny::DynAny::_duplicate (this->da_members_[i].in ());
      ACE_CHECK_RETURN (0);
    }

  return safe_retval._retn ();
}

void
TAO_DynStruct_i::set_members_as_dyn_any (
    const DynamicAny::NameDynAnyPairSeq & values,
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
      CORBA::SystemException,
      DynamicAny::DynAny::TypeMismatch,
      DynamicAny::DynAny::InvalidValue
    ))
{
  if (this->destroyed_)
    {
      ACE_THROW (CORBA::OBJECT_NOT_EXIST ());
    }

  CORBA::ULong length = values.length ();

  // Check for length match.
  if (length != this->component_count_)
    {
      ACE_THROW (DynamicAny::DynAny::InvalidValue ());
    }

  CORBA::TypeCode_var value_tc;
  CORBA::TypeCode_var my_tc;
  CORBA::Boolean equivalent;

  // member_type() does not work with aliased type codes.
  CORBA::TypeCode_var unaliased_tc =
    TAO_DynAnyFactory::strip_alias (this->type_.in (),
                                    ACE_TRY_ENV);
  ACE_CHECK;

  for (CORBA::ULong i = 0; i < length; ++i)
    {
      // Check for type and name match.
      my_tc = unaliased_tc->member_type (i, 
                                         ACE_TRY_ENV);
      ACE_CHECK;

      value_tc = values[i].value->type (ACE_TRY_ENV);
      ACE_CHECK;

      equivalent = my_tc->equivalent (value_tc.in (),
                                      ACE_TRY_ENV);
      ACE_CHECK;

      if (!equivalent)
        {
          ACE_THROW (DynamicAny::DynAny::TypeMismatch ());
        }

      this->da_members_[i]->destroy (ACE_TRY_ENV);
      ACE_CHECK;

      this->da_members_[i] = values[i].value->copy (ACE_TRY_ENV);
      ACE_CHECK;
    }

  this->current_position_ = length ? 0 : -1;
}

// ****************************************************************

void
TAO_DynStruct_i::from_any (const CORBA_Any & any,
                           CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((
      CORBA::SystemException,
      DynamicAny::DynAny::TypeMismatch,
      DynamicAny::DynAny::InvalidValue
    ))
{
  if (this->destroyed_)
    {
      ACE_THROW (CORBA::OBJECT_NOT_EXIST ());
    }

  CORBA::TypeCode_var tc = any.type ();
  CORBA::Boolean equivalent = this->type_->equivalent (tc.in (),
                                                       ACE_TRY_ENV);
  ACE_CHECK;

  if (equivalent)
    {
      // Get the CDR stream of the argument.
      ACE_Message_Block* mb = any._tao_get_cdr ();
      TAO_InputCDR cdr (mb,
                        any._tao_byte_order ());

      // If we have an exception type, unmarshal the repository ID.
      CORBA::TCKind kind = TAO_DynAnyFactory::unalias (this->type_.in (),
                                                       ACE_TRY_ENV);
      ACE_CHECK;

      if (kind == CORBA::tk_except)
        {
          CORBA::String_var str;
          cdr >> str.out ();
        }

      CORBA::TypeCode_var field_tc;

      for (CORBA::ULong i = 0; i < this->component_count_; ++i)
        {
          field_tc = this->type_.in ()->member_type (i,
                                                     ACE_TRY_ENV);
          ACE_CHECK;

          // This Any constructor is a TAO extension.
          CORBA_Any field_any (field_tc.in (),
                               0,
                               cdr.byte_order (),
                               cdr.start ());

          this->da_members_[i]->destroy (ACE_TRY_ENV);
          ACE_CHECK;

          this->da_members_[i] =
            TAO_DynAnyFactory::make_dyn_any (field_any,
                                                       ACE_TRY_ENV);
          ACE_CHECK;

          // Move to the next field in the CDR stream.
          (void) TAO_Marshal_Object::perform_skip (field_tc.in (),
                                                   &cdr,
                                                   ACE_TRY_ENV);
          ACE_CHECK;
        }

      this->current_position_ = this->component_count_ ? 0 : -1;
    }
  else
    {
      ACE_THROW (DynamicAny::DynAny::InvalidValue ());
    }
}

CORBA::Any_ptr
TAO_DynStruct_i::to_any (CORBA::Environment& ACE_TRY_ENV)
  ACE_THROW_SPEC ((
      CORBA::SystemException
    ))
{
  if (this->destroyed_)
    {
      ACE_THROW_RETURN (CORBA::OBJECT_NOT_EXIST (),
                        0);
    }

  TAO_OutputCDR out_cdr;

  // If we have an exception type, marshal the repository ID.
  CORBA::TCKind kind = TAO_DynAnyFactory::unalias (this->type_.in (),
                                                   ACE_TRY_ENV);
  ACE_CHECK_RETURN (0);

  if (kind == CORBA::tk_except)
    {
      out_cdr << this->type_->id ();
    }

  for (CORBA::ULong i = 0; i < this->component_count_; ++i)
    {
      CORBA_TypeCode_var field_tc = this->da_members_[i]->type (ACE_TRY_ENV);
      ACE_CHECK_RETURN (0);

      // Recursive step.
      CORBA_Any_var field_any = this->da_members_[i]->to_any (ACE_TRY_ENV);
      ACE_CHECK_RETURN (0);

      ACE_Message_Block *field_mb = field_any->_tao_get_cdr ();

      TAO_InputCDR field_cdr (field_mb,
                              field_any->_tao_byte_order ());

      (void) TAO_Marshal_Object::perform_append (field_tc.in (),
                                                 &field_cdr,
                                                 &out_cdr,
                                                 ACE_TRY_ENV);
      ACE_CHECK_RETURN (0);
    }

  TAO_InputCDR in_cdr (out_cdr);

  CORBA_Any_ptr retval = 0;
  ACE_NEW_THROW_EX (retval,
                    CORBA_Any (this->type_.in (),
                               0,
                               in_cdr.byte_order (),
                               in_cdr.start ()),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (0);

  return retval;
}

CORBA::Boolean
TAO_DynStruct_i::equal (DynamicAny::DynAny_ptr rhs,
                        CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((
      CORBA::SystemException
    ))
{
  if (this->destroyed_)
    {
      ACE_THROW_RETURN (CORBA::OBJECT_NOT_EXIST (),
                        0);
    }

  CORBA::TypeCode_var tc = rhs->type (ACE_TRY_ENV);
  ACE_CHECK_RETURN (0);

  CORBA::Boolean equivalent = tc->equivalent (this->type_.in (),
                                              ACE_TRY_ENV);
  ACE_CHECK_RETURN (0);

  if (!equivalent)
    {
      return 0;
    }

  DynamicAny::DynAny_var tmp;
  CORBA::Boolean member_equal;

  for (CORBA::ULong i = 0; i < this->component_count_; ++i)
    {
      rhs->seek (ACE_static_cast (CORBA::Long, 
                                  i),
                 ACE_TRY_ENV);
      ACE_CHECK_RETURN (0);

      tmp = rhs->current_component (ACE_TRY_ENV);
      ACE_CHECK_RETURN (0);

      // Recursive step.
      member_equal = tmp->equal (this->da_members_[i].in (),
                                 ACE_TRY_ENV);
      ACE_CHECK_RETURN (0);

      if (!member_equal)
        {
          return 0;
        }
    }

  return 1;
}

void
TAO_DynStruct_i::destroy (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((
      CORBA::SystemException
    ))
{
  if (this->destroyed_)
    {
      ACE_THROW (CORBA::OBJECT_NOT_EXIST ());
    }

  if (!this->ref_to_component_ || this->container_is_destroying_)
    {
      // Do a deep destroy.
      for (CORBA::ULong i = 0; i < this->component_count_; ++i)
        {
          this->set_flag (da_members_[i].in (),
                          1,
                          ACE_TRY_ENV);
          ACE_CHECK;

          this->da_members_[i]->destroy (ACE_TRY_ENV);
          ACE_CHECK;
        }

      this->destroyed_ = 1;
    }
}

DynamicAny::DynAny_ptr
TAO_DynStruct_i::current_component (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((
      CORBA::SystemException,
      DynamicAny::DynAny::TypeMismatch
    ))
{
  if (this->destroyed_)
    {
      ACE_THROW_RETURN (CORBA::OBJECT_NOT_EXIST (),
                        DynamicAny::DynAny::_nil ());
    }

  if (this->component_count_ == 0)
    {
      // Empty exception - no component.
      ACE_THROW_RETURN (DynamicAny::DynAny::TypeMismatch (),
                        DynamicAny::DynAny::_nil ());
    }

  if (this->current_position_ == -1)
    {
      return DynamicAny::DynAny::_nil ();
    }

  CORBA::ULong index = ACE_static_cast (CORBA::ULong, 
                                        this->current_position_);

  this->set_flag (this->da_members_[index].in (),
                  0,
                  ACE_TRY_ENV);
  ACE_CHECK_RETURN (DynamicAny::DynAny::_nil ());

  return DynamicAny::DynAny::_duplicate (
            this->da_members_[index].in ()
          );
}
