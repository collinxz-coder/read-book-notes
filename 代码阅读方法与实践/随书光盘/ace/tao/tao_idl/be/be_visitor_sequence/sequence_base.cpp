//
// sequence_base.cpp,v 1.6 2001/05/30 18:49:49 parsons Exp
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    sequence_base.cpp
//
// = DESCRIPTION
//    Visitor generating code for the base type of the Sequence
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#include	"idl.h"
#include	"idl_extern.h"
#include	"be.h"

#include "be_visitor_sequence.h"

ACE_RCSID(be_visitor_sequence, sequence_base, "sequence_base.cpp,v 1.6 2001/05/30 18:49:49 parsons Exp")


// ****************************************************************
// We have to generate the parameters for the template that implements
// each sequence type.
// ****************************************************************

be_visitor_sequence_base::
be_visitor_sequence_base (be_visitor_context *ctx)
  : be_visitor_decl (ctx)
{
}

be_visitor_sequence_base::~be_visitor_sequence_base (void)
{
}


int
be_visitor_sequence_base::visit_predefined_type (be_predefined_type *node)
{
  TAO_OutStream *os = this->ctx_->stream ();
  *os << node->name ();
  return 0;
}

// helper
int
be_visitor_sequence_base::visit_node (be_type *node)
{
  TAO_OutStream *os = this->ctx_->stream ();
  be_type *bt;

  if (this->ctx_->alias ())
    {
      bt = this->ctx_->alias ();
    }
  else
    {
      bt = node;
    }

  if (this->ctx_->state () == TAO_CodeGen::TAO_SEQUENCE_BASE_CH)
    {
      if (this->ctx_->sub_state () 
            == TAO_CodeGen::TAO_ARRAY_SEQ_CH_TEMPLATE_VAR)
        {
          *os << bt->nested_type_name (this->ctx_->scope (), "_var");
        }
      else
        {
          *os << bt->nested_type_name (this->ctx_->scope ());
        }
    }
  else
    {
      *os << bt->name ();
    }

  return 0;
}

int
be_visitor_sequence_base::visit_sequence (be_sequence *node)
{
  return this->visit_node (node);
}

int
be_visitor_sequence_base::visit_interface (be_interface *node)
{
  
  return this->visit_node (node);

}

int
be_visitor_sequence_base::visit_interface_fwd (be_interface_fwd *node)
{
  return this->visit_node (node);
}

int
be_visitor_sequence_base::visit_string (be_string *)
{
  // NO-OP, we have ad-hoc classes from strings.
  return 0;
}

int
be_visitor_sequence_base::visit_structure (be_structure *node)
{
  return this->visit_node (node);
}

int
be_visitor_sequence_base::visit_union (be_union *node)
{
  return this->visit_node (node);
}

int
be_visitor_sequence_base::visit_enum (be_enum *node)
{
  return this->visit_node (node);
}

int
be_visitor_sequence_base::visit_exception (be_exception *node)
{
  return this->visit_node (node);
}

int
be_visitor_sequence_base::visit_array (be_array *node)
{
  return this->visit_node (node);
}

int
be_visitor_sequence_base::visit_typedef (be_typedef *node)
{
  // Set the alias node.
  this->ctx_->alias (node);

  if (node->primitive_base_type ()->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_sequence_base::"
                         "visit_typedef - "
                         "accept on primitive type failed\n"),
                        -1);
    }

  this->ctx_->alias (0);
  return 0;
}


be_visitor_sequence_base_template_args::
  be_visitor_sequence_base_template_args (be_visitor_context *ctx,
                                          be_sequence *seq)
  : be_visitor_sequence_base (ctx),
    beseq_ (seq)
{
  // no-op
}


be_visitor_sequence_base_template_args::
  ~be_visitor_sequence_base_template_args (void)
{
  //no-op
}

int
be_visitor_sequence_base_template_args::visit_interface (be_interface *node)
{
  TAO_OutStream *os = this->ctx_->stream ();
  be_type *bt;

  if (this->ctx_->alias ())
    {
      bt = this->ctx_->alias ();
    }
  else
    {
      bt = node;
    }

  if (this->ctx_->state () == TAO_CodeGen::TAO_SEQUENCE_BASE_CH)
    {
      *os << bt->nested_type_name (this->ctx_->scope ()) << ",";
      *os << bt->nested_type_name (this->ctx_->scope (), "_var");
    }
  else
    {
      *os << bt->name () << ",";
      *os << bt->name () << "_var";
    }

  return 0;      
}

int
be_visitor_sequence_base_template_args::visit_interface_fwd (
    be_interface_fwd *node
  )
{
  TAO_OutStream *os = this->ctx_->stream ();
  be_type *bt;

  if (this->ctx_->alias ())
    {
      bt = this->ctx_->alias ();
    }
  else
    {
      bt = node;
    }

  if (this->ctx_->state () == TAO_CodeGen::TAO_SEQUENCE_BASE_CH)
    {
      *os << bt->nested_type_name (this->ctx_->scope ()) << ",";
      *os << bt->nested_type_name (this->ctx_->scope (), "_var");
    }
  else
    {
      *os << bt->name () << ",";
      *os << bt->name () << "_var";
    }

  return 0;      
}

int
be_visitor_sequence_base_template_args::visit_predefined_type (
    be_predefined_type *node
  )
{
  TAO_OutStream *os = this->ctx_->stream ();
  *os << node->name ();

  if (beseq_->managed_type () == be_sequence::MNG_PSEUDO 
      || beseq_->managed_type () == be_sequence::MNG_OBJREF)
    {
      *os << "," << node->name () << "_var";
    }
      
  return 0;
}
