// -*- C++ -*-
//
// DynamicC.i,v 1.7 2001/07/06 04:39:03 bala Exp

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

// *************************************************************
// Inline operations for class Dynamic::Parameter_var
// *************************************************************

ACE_INLINE
Dynamic::Parameter_var::Parameter_var (void) // default constructor
  : ptr_ (0)
{}

ACE_INLINE
Dynamic::Parameter_var::Parameter_var (Parameter *p)
  : ptr_ (p)
{}

ACE_INLINE
Dynamic::Parameter_var::Parameter_var (const ::Dynamic::Parameter_var &p) // copy constructor
{
  if (p.ptr_)
    ACE_NEW (this->ptr_, ::Dynamic::Parameter (*p.ptr_));
  else
    this->ptr_ = 0;
}

ACE_INLINE
Dynamic::Parameter_var::~Parameter_var (void) // destructor
{
  delete this->ptr_;
}

ACE_INLINE Dynamic::Parameter_var &
Dynamic::Parameter_var::operator= (Parameter *p)
{
  delete this->ptr_;
  this->ptr_ = p;
  return *this;
}

ACE_INLINE ::Dynamic::Parameter_var &
Dynamic::Parameter_var::operator= (const ::Dynamic::Parameter_var &p)
{
  if (this != &p)
    {
      if (p.ptr_ == 0)
        {
          delete this->ptr_;
          this->ptr_ = 0;
        }
      else
        {
          Parameter *deep_copy =
            new Parameter (*p.ptr_);

          if (deep_copy != 0)
            {
              Parameter *tmp = deep_copy;
              deep_copy = this->ptr_;
              this->ptr_ = tmp;
              delete deep_copy;
            }
        }
    }

  return *this;
}

ACE_INLINE const ::Dynamic::Parameter *
Dynamic::Parameter_var::operator-> (void) const
{
  return this->ptr_;
}

ACE_INLINE ::Dynamic::Parameter *
Dynamic::Parameter_var::operator-> (void)
{
  return this->ptr_;
}

ACE_INLINE
Dynamic::Parameter_var::operator const ::Dynamic::Parameter &() const // cast
{
  return *this->ptr_;
}

ACE_INLINE
Dynamic::Parameter_var::operator ::Dynamic::Parameter &() // cast
{
  return *this->ptr_;
}

ACE_INLINE
Dynamic::Parameter_var::operator ::Dynamic::Parameter &() const // cast
{
  return *this->ptr_;
}

// variable-size types only
ACE_INLINE
Dynamic::Parameter_var::operator ::Dynamic::Parameter *&() // cast
{
  return this->ptr_;
}

ACE_INLINE const ::Dynamic::Parameter &
Dynamic::Parameter_var::in (void) const
{
  return *this->ptr_;
}

ACE_INLINE ::Dynamic::Parameter &
Dynamic::Parameter_var::inout (void)
{
  return *this->ptr_;
}

// mapping for variable size
ACE_INLINE ::Dynamic::Parameter *&
Dynamic::Parameter_var::out (void)
{
  delete this->ptr_;
  this->ptr_ = 0;
  return this->ptr_;
}

ACE_INLINE ::Dynamic::Parameter *
Dynamic::Parameter_var::_retn (void)
{
  ::Dynamic::Parameter *tmp = this->ptr_;
  this->ptr_ = 0;
  return tmp;
}

ACE_INLINE ::Dynamic::Parameter *
Dynamic::Parameter_var::ptr (void) const
{
  return this->ptr_;
}

// *************************************************************
// Inline operations for class Dynamic::Parameter_out
// *************************************************************

ACE_INLINE
Dynamic::Parameter_out::Parameter_out (::Dynamic::Parameter *&p)
  : ptr_ (p)
{
  this->ptr_ = 0;
}

ACE_INLINE
Dynamic::Parameter_out::Parameter_out (Parameter_var &p) // constructor from _var
  : ptr_ (p.out ())
{
  delete this->ptr_;
  this->ptr_ = 0;
}

ACE_INLINE
Dynamic::Parameter_out::Parameter_out (const ::Dynamic::Parameter_out &p) // copy constructor
  : ptr_ (ACE_const_cast (Parameter_out&, p).ptr_)
{}

ACE_INLINE Dynamic::Parameter_out &
Dynamic::Parameter_out::operator= (const ::Dynamic::Parameter_out &p)
{
  this->ptr_ = ACE_const_cast (Parameter_out&, p).ptr_;
  return *this;
}

ACE_INLINE Dynamic::Parameter_out &
Dynamic::Parameter_out::operator= (Parameter *p)
{
  this->ptr_ = p;
  return *this;
}

ACE_INLINE
Dynamic::Parameter_out::operator ::Dynamic::Parameter *&() // cast
{
  return this->ptr_;
}

ACE_INLINE ::Dynamic::Parameter *&
Dynamic::Parameter_out::ptr (void) // ptr
{
  return this->ptr_;
}

ACE_INLINE ::Dynamic::Parameter *
Dynamic::Parameter_out::operator-> (void)
{
  return this->ptr_;
}


#if !defined (TAO_USE_SEQUENCE_TEMPLATES)

#if !defined (__TAO_UNBOUNDED_SEQUENCE_DYNAMIC_PARAMETERLIST_CI_)
#define __TAO_UNBOUNDED_SEQUENCE_DYNAMIC_PARAMETERLIST_CI_

  // = Static operations.
  ACE_INLINE Dynamic::Parameter *
  Dynamic::_TAO_Unbounded_Sequence_Dynamic_ParameterList::allocbuf (CORBA::ULong size)
  // Allocate storage for the sequence.
  {
    Dynamic::Parameter *retval = 0;
    ACE_NEW_RETURN (retval, Dynamic::Parameter[size], 0);
    return retval;
  }

  ACE_INLINE void Dynamic::_TAO_Unbounded_Sequence_Dynamic_ParameterList::freebuf (Dynamic::Parameter *buffer)
  // Free the sequence.
  {
    delete [] buffer;
  }

  ACE_INLINE
  Dynamic::_TAO_Unbounded_Sequence_Dynamic_ParameterList::_TAO_Unbounded_Sequence_Dynamic_ParameterList (void) // Default constructor.
  {
  }

  ACE_INLINE
  Dynamic::_TAO_Unbounded_Sequence_Dynamic_ParameterList::_TAO_Unbounded_Sequence_Dynamic_ParameterList (CORBA::ULong maximum) // Constructor using a maximum length value.
    : TAO_Unbounded_Base_Sequence (maximum, _TAO_Unbounded_Sequence_Dynamic_ParameterList::allocbuf (maximum))
  {
  }

  ACE_INLINE
  Dynamic::_TAO_Unbounded_Sequence_Dynamic_ParameterList::_TAO_Unbounded_Sequence_Dynamic_ParameterList (CORBA::ULong maximum,
    CORBA::ULong length,
    Dynamic::Parameter *data,
    CORBA::Boolean release)
  : TAO_Unbounded_Base_Sequence (maximum, length, data, release)
  {
  }

  ACE_INLINE
  Dynamic::_TAO_Unbounded_Sequence_Dynamic_ParameterList::_TAO_Unbounded_Sequence_Dynamic_ParameterList (const _TAO_Unbounded_Sequence_Dynamic_ParameterList &rhs)
  // Copy constructor.
    : TAO_Unbounded_Base_Sequence (rhs)
  {
    if (rhs.buffer_ != 0)
    {
      Dynamic::Parameter *tmp1 = _TAO_Unbounded_Sequence_Dynamic_ParameterList::allocbuf (this->maximum_);
      Dynamic::Parameter * const tmp2 = ACE_reinterpret_cast (Dynamic::Parameter * ACE_CAST_CONST, rhs.buffer_);

      for (CORBA::ULong i = 0; i < this->length_; ++i)
        tmp1[i] = tmp2[i];

      this->buffer_ = tmp1;
    }
    else
    {
      this->buffer_ = 0;
    }
  }

  ACE_INLINE Dynamic::_TAO_Unbounded_Sequence_Dynamic_ParameterList &
  Dynamic::_TAO_Unbounded_Sequence_Dynamic_ParameterList::operator= (const _TAO_Unbounded_Sequence_Dynamic_ParameterList &rhs)
  // Assignment operator.
  {
    if (this == &rhs)
      return *this;

    if (this->release_)
    {
      if (this->maximum_ < rhs.maximum_)
      {
        // free the old buffer
        Dynamic::Parameter *tmp = ACE_reinterpret_cast (Dynamic::Parameter *, this->buffer_);
        _TAO_Unbounded_Sequence_Dynamic_ParameterList::freebuf (tmp);
        this->buffer_ = _TAO_Unbounded_Sequence_Dynamic_ParameterList::allocbuf (rhs.maximum_);
      }
    }
    else
      this->buffer_ = _TAO_Unbounded_Sequence_Dynamic_ParameterList::allocbuf (rhs.maximum_);

    TAO_Unbounded_Base_Sequence::operator= (rhs);

    Dynamic::Parameter *tmp1 = ACE_reinterpret_cast (Dynamic::Parameter *, this->buffer_);
    Dynamic::Parameter * const tmp2 = ACE_reinterpret_cast (Dynamic::Parameter * ACE_CAST_CONST, rhs.buffer_);

    for (CORBA::ULong i = 0; i < this->length_; ++i)
      tmp1[i] = tmp2[i];

    return *this;
  }

  // = Accessors.
  ACE_INLINE Dynamic::Parameter &
  Dynamic::_TAO_Unbounded_Sequence_Dynamic_ParameterList::operator[] (CORBA::ULong i)
  // operator []
  {
    ACE_ASSERT (i < this->maximum_);
    Dynamic::Parameter* tmp = ACE_reinterpret_cast(Dynamic::Parameter*,this->buffer_);
    return tmp[i];
  }

  ACE_INLINE const Dynamic::Parameter &
  Dynamic::_TAO_Unbounded_Sequence_Dynamic_ParameterList::operator[] (CORBA::ULong i) const
  // operator []
  {
    ACE_ASSERT (i < this->maximum_);
    Dynamic::Parameter * const tmp = ACE_reinterpret_cast (Dynamic::Parameter* ACE_CAST_CONST, this->buffer_);
    return tmp[i];
  }

  // Implement the TAO_Base_Sequence methods (see Sequence.h)

  ACE_INLINE Dynamic::Parameter *
  Dynamic::_TAO_Unbounded_Sequence_Dynamic_ParameterList::get_buffer (CORBA::Boolean orphan)
  {
    Dynamic::Parameter *result = 0;
    if (orphan == 0)
    {
      // We retain ownership.
      if (this->buffer_ == 0)
      {
        result = _TAO_Unbounded_Sequence_Dynamic_ParameterList::allocbuf (this->length_);
        this->buffer_ = result;
        this->release_ = 1;
      }
      else
      {
        result = ACE_reinterpret_cast (Dynamic::Parameter*, this->buffer_);
      }
    }
    else // if (orphan == 1)
    {
      if (this->release_ != 0)
      {
        // We set the state back to default and relinquish
        // ownership.
        result = ACE_reinterpret_cast(Dynamic::Parameter*,this->buffer_);
        this->maximum_ = 0;
        this->length_ = 0;
        this->buffer_ = 0;
        this->release_ = 0;
      }
    }
    return result;
  }

  ACE_INLINE const Dynamic::Parameter *
  Dynamic::_TAO_Unbounded_Sequence_Dynamic_ParameterList::get_buffer (void) const
  {
    return ACE_reinterpret_cast(const Dynamic::Parameter * ACE_CAST_CONST, this->buffer_);
  }

  ACE_INLINE void
  Dynamic::_TAO_Unbounded_Sequence_Dynamic_ParameterList::replace (CORBA::ULong max,
  CORBA::ULong length,
  Dynamic::Parameter *data,
  CORBA::Boolean release)
  {
    this->maximum_ = max;
    this->length_ = length;
    if (this->buffer_ && this->release_ == 1)
    {
      Dynamic::Parameter *tmp = ACE_reinterpret_cast(Dynamic::Parameter*,this->buffer_);
      _TAO_Unbounded_Sequence_Dynamic_ParameterList::freebuf (tmp);
    }
    this->buffer_ = data;
    this->release_ = release;
  }

#endif /* end #if !defined */


#endif /* !TAO_USE_SEQUENCE_TEMPLATES */

#if !defined (_DYNAMIC_PARAMETERLIST_CI_)
#define _DYNAMIC_PARAMETERLIST_CI_

// *************************************************************
// Inline operations for class Dynamic::ParameterList_var
// *************************************************************

ACE_INLINE
Dynamic::ParameterList_var::ParameterList_var (void) // default constructor
  : ptr_ (0)
{}

ACE_INLINE
Dynamic::ParameterList_var::ParameterList_var (ParameterList *p)
  : ptr_ (p)
{}

ACE_INLINE
Dynamic::ParameterList_var::ParameterList_var (const ::Dynamic::ParameterList_var &p) // copy constructor
{
  if (p.ptr_)
    ACE_NEW (this->ptr_, ::Dynamic::ParameterList (*p.ptr_));
  else
    this->ptr_ = 0;
}

ACE_INLINE
Dynamic::ParameterList_var::~ParameterList_var (void) // destructor
{
  delete this->ptr_;
}

ACE_INLINE Dynamic::ParameterList_var &
Dynamic::ParameterList_var::operator= (ParameterList *p)
{
  delete this->ptr_;
  this->ptr_ = p;
  return *this;
}

ACE_INLINE ::Dynamic::ParameterList_var &
Dynamic::ParameterList_var::operator= (const ::Dynamic::ParameterList_var &p)
{
  if (this != &p)
    {
      if (p.ptr_ == 0)
        {
          delete this->ptr_;
          this->ptr_ = 0;
        }
      else
        {
          ParameterList *deep_copy =
            new ParameterList (*p.ptr_);

          if (deep_copy != 0)
            {
              ParameterList *tmp = deep_copy;
              deep_copy = this->ptr_;
              this->ptr_ = tmp;
              delete deep_copy;
            }
        }
    }

  return *this;
}

ACE_INLINE const ::Dynamic::ParameterList *
Dynamic::ParameterList_var::operator-> (void) const
{
  return this->ptr_;
}

ACE_INLINE ::Dynamic::ParameterList *
Dynamic::ParameterList_var::operator-> (void)
{
  return this->ptr_;
}

ACE_INLINE
Dynamic::ParameterList_var::operator const ::Dynamic::ParameterList &() const // cast
{
  return *this->ptr_;
}

ACE_INLINE
Dynamic::ParameterList_var::operator ::Dynamic::ParameterList &() // cast
{
  return *this->ptr_;
}

ACE_INLINE
Dynamic::ParameterList_var::operator ::Dynamic::ParameterList &() const // cast
{
  return *this->ptr_;
}

// variable-size types only
ACE_INLINE
Dynamic::ParameterList_var::operator ::Dynamic::ParameterList *&() // cast
{
  return this->ptr_;
}

ACE_INLINE Dynamic::Parameter &
Dynamic::ParameterList_var::operator[] (CORBA::ULong index)
{
  return this->ptr_->operator[] (index);
}

ACE_INLINE const Dynamic::Parameter &
Dynamic::ParameterList_var::operator[] (CORBA::ULong index) const
{
  return ACE_const_cast (const Dynamic::Parameter &, this->ptr_->operator[] (index));
}

ACE_INLINE const ::Dynamic::ParameterList &
Dynamic::ParameterList_var::in (void) const
{
  return *this->ptr_;
}

ACE_INLINE ::Dynamic::ParameterList &
Dynamic::ParameterList_var::inout (void)
{
  return *this->ptr_;
}

// mapping for variable size
ACE_INLINE ::Dynamic::ParameterList *&
Dynamic::ParameterList_var::out (void)
{
  delete this->ptr_;
  this->ptr_ = 0;
  return this->ptr_;
}

ACE_INLINE ::Dynamic::ParameterList *
Dynamic::ParameterList_var::_retn (void)
{
  ::Dynamic::ParameterList *tmp = this->ptr_;
  this->ptr_ = 0;
  return tmp;
}

ACE_INLINE ::Dynamic::ParameterList *
Dynamic::ParameterList_var::ptr (void) const
{
  return this->ptr_;
}

// *************************************************************
// Inline operations for class Dynamic::ParameterList_out
// *************************************************************

ACE_INLINE
Dynamic::ParameterList_out::ParameterList_out (ParameterList *&p)
  : ptr_ (p)
{
  this->ptr_ = 0;
}

ACE_INLINE
Dynamic::ParameterList_out::ParameterList_out (ParameterList_var &p) // constructor from _var
  : ptr_ (p.out ())
{
  delete this->ptr_;
  this->ptr_ = 0;
}

ACE_INLINE
Dynamic::ParameterList_out::ParameterList_out (const ::Dynamic::ParameterList_out &p) // copy constructor
  : ptr_ (ACE_const_cast (ParameterList_out&, p).ptr_)
{}

ACE_INLINE ::Dynamic::ParameterList_out &
Dynamic::ParameterList_out::operator= (const ::Dynamic::ParameterList_out &p)
{
  this->ptr_ = ACE_const_cast (ParameterList_out&, p).ptr_;
  return *this;
}

ACE_INLINE ::Dynamic::ParameterList_out &
Dynamic::ParameterList_out::operator= (ParameterList *p)
{
  this->ptr_ = p;
  return *this;
}

ACE_INLINE
Dynamic::ParameterList_out::operator ::Dynamic::ParameterList *&() // cast
{
  return this->ptr_;
}

ACE_INLINE ::Dynamic::ParameterList *&
Dynamic::ParameterList_out::ptr (void) // ptr
{
  return this->ptr_;
}

ACE_INLINE ::Dynamic::ParameterList *
Dynamic::ParameterList_out::operator-> (void)
{
  return this->ptr_;
}

ACE_INLINE Dynamic::Parameter &
Dynamic::ParameterList_out::operator[] (CORBA::ULong index)
{
  return this->ptr_->operator[] (index);
}


#endif /* end #if !defined */


#if !defined (TAO_USE_SEQUENCE_TEMPLATES)

#if !defined (__TAO_UNBOUNDED_OBJECT_SEQUENCE_DYNAMIC_EXCEPTIONLIST_CI_)
#define __TAO_UNBOUNDED_OBJECT_SEQUENCE_DYNAMIC_EXCEPTIONLIST_CI_

  ACE_INLINE CORBA::TypeCode **
  Dynamic::_TAO_Unbounded_Object_Sequence_Dynamic_ExceptionList::allocbuf (CORBA::ULong nelems)
  {
    CORBA::TypeCode **buf = 0;

    ACE_NEW_RETURN (buf, CORBA::TypeCode*[nelems], 0);

    for (CORBA::ULong i = 0; i < nelems; i++)
      {
        buf[i] = CORBA::TypeCode::_nil ();
      }

    return buf;
  }

  ACE_INLINE void
  Dynamic::_TAO_Unbounded_Object_Sequence_Dynamic_ExceptionList::freebuf (CORBA::TypeCode **buffer)
  {
    if (buffer == 0)
      return;
    delete[] buffer;
  }

  ACE_INLINE
  Dynamic::_TAO_Unbounded_Object_Sequence_Dynamic_ExceptionList::_TAO_Unbounded_Object_Sequence_Dynamic_ExceptionList (void)
  {
  }

  ACE_INLINE
  Dynamic::_TAO_Unbounded_Object_Sequence_Dynamic_ExceptionList::_TAO_Unbounded_Object_Sequence_Dynamic_ExceptionList (CORBA::ULong maximum)
    : TAO_Unbounded_Base_Sequence (maximum, _TAO_Unbounded_Object_Sequence_Dynamic_ExceptionList::allocbuf (maximum))
  {
  }

  ACE_INLINE
  Dynamic::_TAO_Unbounded_Object_Sequence_Dynamic_ExceptionList::_TAO_Unbounded_Object_Sequence_Dynamic_ExceptionList (CORBA::ULong maximum,
    CORBA::ULong length,
    CORBA::TypeCode* *value,
    CORBA::Boolean release)
  : TAO_Unbounded_Base_Sequence (maximum, length, value, release)
  {
  }

  ACE_INLINE
  Dynamic::_TAO_Unbounded_Object_Sequence_Dynamic_ExceptionList::_TAO_Unbounded_Object_Sequence_Dynamic_ExceptionList(const _TAO_Unbounded_Object_Sequence_Dynamic_ExceptionList &rhs)
    : TAO_Unbounded_Base_Sequence (rhs)
  {
    if (rhs.buffer_ != 0)
    {
      CORBA::TypeCode **tmp1 = _TAO_Unbounded_Object_Sequence_Dynamic_ExceptionList::allocbuf (this->maximum_);
      CORBA::TypeCode ** const tmp2 = ACE_reinterpret_cast (CORBA::TypeCode ** ACE_CAST_CONST, rhs.buffer_);

      for (CORBA::ULong i = 0; i < rhs.length_; ++i)
        {
          tmp1[i] = CORBA::TypeCode::_duplicate (tmp2[i]);
        }

      this->buffer_ = tmp1;
    }
    else
    {
      this->buffer_ = 0;
    }
  }

  ACE_INLINE Dynamic::_TAO_Unbounded_Object_Sequence_Dynamic_ExceptionList &
  Dynamic::_TAO_Unbounded_Object_Sequence_Dynamic_ExceptionList::operator= (const _TAO_Unbounded_Object_Sequence_Dynamic_ExceptionList &rhs)
  {
    if (this == &rhs)
      return *this;

    if (this->release_)
    {
      CORBA::TypeCode **tmp = ACE_reinterpret_cast (CORBA::TypeCode **, this->buffer_);

      for (CORBA::ULong i = 0; i < this->length_; ++i)
      {
        CORBA::release (tmp[i]);
        tmp[i] = CORBA::TypeCode::_nil ();
      }
      if (this->maximum_ < rhs.maximum_)
      {
        _TAO_Unbounded_Object_Sequence_Dynamic_ExceptionList::freebuf (tmp);
        this->buffer_ = _TAO_Unbounded_Object_Sequence_Dynamic_ExceptionList::allocbuf (rhs.maximum_);
      }
    }
    else
      this->buffer_ = _TAO_Unbounded_Object_Sequence_Dynamic_ExceptionList::allocbuf (rhs.maximum_);

    TAO_Unbounded_Base_Sequence::operator= (rhs);

    CORBA::TypeCode **tmp1 = ACE_reinterpret_cast (CORBA::TypeCode **, this->buffer_);
    CORBA::TypeCode ** const tmp2 = ACE_reinterpret_cast (CORBA::TypeCode ** ACE_CAST_CONST, rhs.buffer_);

    for (CORBA::ULong i = 0; i < rhs.length_; ++i)
      {
        tmp1[i] = CORBA::TypeCode::_duplicate (tmp2[i]);
      }

    return *this;
  }

  ACE_INLINE TAO_Pseudo_Object_Manager<Dynamic::TypeCode,Dynamic::TypeCode_var>
  Dynamic::_TAO_Unbounded_Object_Sequence_Dynamic_ExceptionList::operator[] (CORBA::ULong index) const
  // read-write accessor
  {
    ACE_ASSERT (index < this->maximum_);
    CORBA::TypeCode ** const tmp = ACE_reinterpret_cast (CORBA::TypeCode ** ACE_CAST_CONST, this->buffer_);
    return TAO_Pseudo_Object_Manager<Dynamic::TypeCode,Dynamic::TypeCode_var> (tmp + index, this->release_);
  }

  ACE_INLINE CORBA::TypeCode* *
  Dynamic::_TAO_Unbounded_Object_Sequence_Dynamic_ExceptionList::get_buffer (CORBA::Boolean orphan)
  {
    CORBA::TypeCode **result = 0;
    if (orphan == 0)
    {
      // We retain ownership.
      if (this->buffer_ == 0)
      {
        result = _TAO_Unbounded_Object_Sequence_Dynamic_ExceptionList::allocbuf (this->maximum_);
        this->buffer_ = result;
        this->release_ = 1;
      }
      else
      {
        result = ACE_reinterpret_cast (CORBA::TypeCode**, this->buffer_);
      }
    }
    else // if (orphan == 1)
    {
      if (this->release_ != 0)
      {
        // We set the state back to default and relinquish
        // ownership.
        result = ACE_reinterpret_cast(CORBA::TypeCode**,this->buffer_);
        this->maximum_ = 0;
        this->length_ = 0;
        this->buffer_ = 0;
        this->release_ = 0;
      }
    }
    return result;
  }

  ACE_INLINE const CORBA::TypeCode* *
  Dynamic::_TAO_Unbounded_Object_Sequence_Dynamic_ExceptionList::get_buffer (void) const
  {
    return ACE_reinterpret_cast(const CORBA::TypeCode ** ACE_CAST_CONST, this->buffer_);
  }


#endif /* end #if !defined */


#endif /* !TAO_USE_SEQUENCE_TEMPLATES */

#if !defined (_DYNAMIC_EXCEPTIONLIST_CI_)
#define _DYNAMIC_EXCEPTIONLIST_CI_

// *************************************************************
// Inline operations for class Dynamic::ExceptionList_var
// *************************************************************

ACE_INLINE
Dynamic::ExceptionList_var::ExceptionList_var (void) // default constructor
  : ptr_ (0)
{}

ACE_INLINE
Dynamic::ExceptionList_var::ExceptionList_var (ExceptionList *p)
  : ptr_ (p)
{}

ACE_INLINE
Dynamic::ExceptionList_var::ExceptionList_var (const ::Dynamic::ExceptionList_var &p) // copy constructor
{
  if (p.ptr_)
    ACE_NEW (this->ptr_, ::Dynamic::ExceptionList (*p.ptr_));
  else
    this->ptr_ = 0;
}

ACE_INLINE
Dynamic::ExceptionList_var::~ExceptionList_var (void) // destructor
{
  delete this->ptr_;
}

ACE_INLINE Dynamic::ExceptionList_var &
Dynamic::ExceptionList_var::operator= (ExceptionList *p)
{
  delete this->ptr_;
  this->ptr_ = p;
  return *this;
}

ACE_INLINE ::Dynamic::ExceptionList_var &
Dynamic::ExceptionList_var::operator= (const ::Dynamic::ExceptionList_var &p)
{
  if (this != &p)
    {
      if (p.ptr_ == 0)
        {
          delete this->ptr_;
          this->ptr_ = 0;
        }
      else
        {
          ExceptionList *deep_copy =
            new ExceptionList (*p.ptr_);

          if (deep_copy != 0)
            {
              ExceptionList *tmp = deep_copy;
              deep_copy = this->ptr_;
              this->ptr_ = tmp;
              delete deep_copy;
            }
        }
    }

  return *this;
}

ACE_INLINE const ::Dynamic::ExceptionList *
Dynamic::ExceptionList_var::operator-> (void) const
{
  return this->ptr_;
}

ACE_INLINE ::Dynamic::ExceptionList *
Dynamic::ExceptionList_var::operator-> (void)
{
  return this->ptr_;
}

ACE_INLINE
Dynamic::ExceptionList_var::operator const ::Dynamic::ExceptionList &() const // cast
{
  return *this->ptr_;
}

ACE_INLINE
Dynamic::ExceptionList_var::operator ::Dynamic::ExceptionList &() // cast
{
  return *this->ptr_;
}

ACE_INLINE
Dynamic::ExceptionList_var::operator ::Dynamic::ExceptionList &() const // cast
{
  return *this->ptr_;
}

// variable-size types only
ACE_INLINE
Dynamic::ExceptionList_var::operator ::Dynamic::ExceptionList *&() // cast
{
  return this->ptr_;
}

ACE_INLINE TAO_Pseudo_Object_Manager<Dynamic::TypeCode,Dynamic::TypeCode_var>
Dynamic::ExceptionList_var::operator[] (CORBA::ULong index)
{
  return this->ptr_->operator[] (index);
}

ACE_INLINE const ::Dynamic::ExceptionList &
Dynamic::ExceptionList_var::in (void) const
{
  return *this->ptr_;
}

ACE_INLINE ::Dynamic::ExceptionList &
Dynamic::ExceptionList_var::inout (void)
{
  return *this->ptr_;
}

// mapping for variable size
ACE_INLINE ::Dynamic::ExceptionList *&
Dynamic::ExceptionList_var::out (void)
{
  delete this->ptr_;
  this->ptr_ = 0;
  return this->ptr_;
}

ACE_INLINE ::Dynamic::ExceptionList *
Dynamic::ExceptionList_var::_retn (void)
{
  ::Dynamic::ExceptionList *tmp = this->ptr_;
  this->ptr_ = 0;
  return tmp;
}

ACE_INLINE ::Dynamic::ExceptionList *
Dynamic::ExceptionList_var::ptr (void) const
{
  return this->ptr_;
}

// *************************************************************
// Inline operations for class Dynamic::ExceptionList_out
// *************************************************************

ACE_INLINE
Dynamic::ExceptionList_out::ExceptionList_out (ExceptionList *&p)
  : ptr_ (p)
{
  this->ptr_ = 0;
}

ACE_INLINE
Dynamic::ExceptionList_out::ExceptionList_out (ExceptionList_var &p) // constructor from _var
  : ptr_ (p.out ())
{
  delete this->ptr_;
  this->ptr_ = 0;
}

ACE_INLINE
Dynamic::ExceptionList_out::ExceptionList_out (const ::Dynamic::ExceptionList_out &p) // copy constructor
  : ptr_ (ACE_const_cast (ExceptionList_out&, p).ptr_)
{}

ACE_INLINE ::Dynamic::ExceptionList_out &
Dynamic::ExceptionList_out::operator= (const ::Dynamic::ExceptionList_out &p)
{
  this->ptr_ = ACE_const_cast (ExceptionList_out&, p).ptr_;
  return *this;
}

ACE_INLINE ::Dynamic::ExceptionList_out &
Dynamic::ExceptionList_out::operator= (ExceptionList *p)
{
  this->ptr_ = p;
  return *this;
}

ACE_INLINE
Dynamic::ExceptionList_out::operator ::Dynamic::ExceptionList *&() // cast
{
  return this->ptr_;
}

ACE_INLINE ::Dynamic::ExceptionList *&
Dynamic::ExceptionList_out::ptr (void) // ptr
{
  return this->ptr_;
}

ACE_INLINE ::Dynamic::ExceptionList *
Dynamic::ExceptionList_out::operator-> (void)
{
  return this->ptr_;
}

ACE_INLINE TAO_Pseudo_Object_Manager<Dynamic::TypeCode,Dynamic::TypeCode_var>
Dynamic::ExceptionList_out::operator[] (CORBA::ULong index)
{
  return this->ptr_->operator[] (index);
}


#endif /* end #if !defined */

ACE_INLINE CORBA::Boolean operator<< (TAO_OutputCDR &strm, const Dynamic::Parameter &_tao_aggregate)
{
  if (
    (strm << _tao_aggregate.argument) &&
    (strm << _tao_aggregate.mode)
  )
    return 1;
  else
    return 0;

}

ACE_INLINE CORBA::Boolean operator>> (TAO_InputCDR &strm, Dynamic::Parameter &_tao_aggregate)
{
  if (
    (strm >> _tao_aggregate.argument) &&
    (strm >> _tao_aggregate.mode)
  )
    return 1;
  else
    return 0;

}


#if !defined _TAO_CDR_OP_Dynamic_ParameterList_I_
#define _TAO_CDR_OP_Dynamic_ParameterList_I_

CORBA::Boolean TAO_Export operator<< (
    TAO_OutputCDR &,
    const Dynamic::ParameterList &
  );
CORBA::Boolean TAO_Export operator>> (
    TAO_InputCDR &,
    Dynamic::ParameterList &
  );

#endif /* _TAO_CDR_OP_Dynamic_ParameterList_I_ */


#if !defined _TAO_CDR_OP_Dynamic_ExceptionList_I_
#define _TAO_CDR_OP_Dynamic_ExceptionList_I_

CORBA::Boolean TAO_Export operator<< (
    TAO_OutputCDR &,
    const Dynamic::ExceptionList &
  );
CORBA::Boolean TAO_Export operator>> (
    TAO_InputCDR &,
    Dynamic::ExceptionList &
  );

#endif /* _TAO_CDR_OP_Dynamic_ExceptionList_I_ */
