//
// rettype_vardecl_ss.h,v 1.5 2000/03/21 20:03:57 coryan Exp
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    rettype_vardecl_ss.h
//
// = DESCRIPTION
//    Visitor for generating code for IDL operations. This generates the return
//    type variable declaration in the server side skeleton.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#ifndef _BE_VISITOR_OPERATION_RETTYPE_VARDECL_SS_H_
#define _BE_VISITOR_OPERATION_RETTYPE_VARDECL_SS_H_

// ************************************************************
// Operation visitor for return type variable declaration in the server
// skeleton
// ************************************************************

class be_visitor_operation_rettype_vardecl_ss :public be_visitor_decl
{
  //
  // = TITLE
  //   be_visitor_operation_rettype_vardecl_ss
  //
  // = DESCRIPTION
  //   This is a visitor to generate a variable declaration ofr an operation
  //   return type in the server skeleton
  //
public:
  be_visitor_operation_rettype_vardecl_ss (be_visitor_context *ctx);
  // constructor

  ~be_visitor_operation_rettype_vardecl_ss (void);
  // destructor

  int visit_array (be_array *node);
  // visit an array node

  int visit_enum (be_enum *node);
  // visit an enum node

  int visit_interface (be_interface *node);
  // visit an interface node

  int visit_interface_fwd (be_interface_fwd *node);
  // visit an interface node

#ifdef IDL_HAS_VALUETYPE
  virtual int visit_valuetype (be_valuetype *node);
  // visit valuetype

  virtual int visit_valuetype_fwd (be_valuetype_fwd *node);
  // visit valuetype forward
#endif /* IDL_HAS_VALUETYPE */

  int visit_predefined_type (be_predefined_type *node);
  // visit a predefined type node

  int visit_sequence (be_sequence *node);
  // visit a sequence node

  int visit_string (be_string *node);
  // visit a sequence node

  int visit_structure (be_structure *node);
  // visit a structure node

  int visit_typedef (be_typedef *node);
  // visit a typedef node

  int visit_union (be_union *node);
  // visit a union node

};

#endif /* _BE_VISITOR_OPERATION_RETTYPE_VARDECL_SS_H_ */
