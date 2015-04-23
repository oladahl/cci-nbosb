// LICENSETEXT
//
//   Copyright (C) 2011 : GreenSocs Ltd
// 	 http://www.greensocs.com/ , email: info@greensocs.com
//
//   Developed by:
//    Christian Schroeder <schroeder@eis.cs.tu-bs.de>,
//
//
// The contents of this file are subject to the licensing terms specified
// in the file LICENSE. Please consult this file for restrictions and
// limitations that may apply.
// 
// ENDLICENSETEXT

#define SC_INCLUDE_DYNAMIC_PROCESSES
#include "gs_cci_symbols_c.h"
#include "test_datatype_without_parameter_implementation.h"

//
// This makes sure the symbols for supported parameter types are available in the library!
//

namespace cci { namespace cnf {

#define CCI_IMPL_CREATE_SYMBOLS_FOR_FUNCTIONS_T_TM(PTYPE, MUTABILITY_TYPE) \
  template  cci_param_impl_if*  create_cci_param (cci_param<PTYPE, MUTABILITY_TYPE> *owner_par, const std::string &nam, const PTYPE& val, const bool is_top_level_name, cci_cnf_broker_if* broker_accessor); \
  template  cci_param_impl_if*  create_cci_param (cci_param<PTYPE, MUTABILITY_TYPE> *owner_par, const std::string &nam, const cci_value& val,  const bool is_top_level_name, cci_cnf_broker_if* broker_accessor);

#define CCI_IMPL_CREATE_SYMBOLS_FOR_FUNCTIONS(PTYPE) \
  CCI_IMPL_CREATE_SYMBOLS_FOR_FUNCTIONS_T_TM(PTYPE, mutable_param) \
  CCI_IMPL_CREATE_SYMBOLS_FOR_FUNCTIONS_T_TM(PTYPE, immutable_param) \
  CCI_IMPL_CREATE_SYMBOLS_FOR_FUNCTIONS_T_TM(PTYPE, elaboration_time_param) \
  CCI_IMPL_CREATE_SYMBOLS_FOR_FUNCTIONS_T_TM(PTYPE, other_param)

  CCI_IMPL_CREATE_SYMBOLS_FOR_FUNCTIONS(test_datatype_without_parameter_implementation)

} } // end namespace


std::istream& operator >>(std::istream &is,test_datatype_without_parameter_implementation &obj)
{
  is>>obj.strVal;
  return is;
}
std::ostream& operator <<(std::ostream &os,const test_datatype_without_parameter_implementation &obj)
{
  os<<obj.strVal;
  return os;
}
bool operator==(test_datatype_without_parameter_implementation const& lhs, test_datatype_without_parameter_implementation const& rhs) {
  return lhs.strVal.compare(rhs.strVal);
}

#define ENABLE_CCI_VALUE_SUPPORT_FOR_TEST_DATATYPE
// The following code enables the new data type parameter to work with cci_values !!!!
#ifdef ENABLE_CCI_VALUE_SUPPORT_FOR_TEST_DATATYPE


namespace cci { namespace cnf {
template<>
struct cci_value_traits< test_datatype_without_parameter_implementation >
{
  typedef test_datatype_without_parameter_implementation type;
  static bool pack( cci_value::reference dst, type const & src )
  {
    dst.set_string( src.strVal );
    return true;
  }
  static bool unpack( type & dst, cci_value::const_reference src )
  {
    if( !src.is_string() )
      return false;

    dst.strVal = src.get_string();
    return true;
  }

};
} /* namespace cnf */ } /* namespace cci */

#endif
// end if ENABLE_CCI_VALUE_SUPPORT_FOR_TEST_DATATYPE
