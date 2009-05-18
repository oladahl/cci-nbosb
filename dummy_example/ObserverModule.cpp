//   OSCI CCI WG
//
// LICENSETEXT
//
//   Copyright (C) 2009 : GreenSocs Ltd
// 	 http://www.greensocs.com/ , email: info@greensocs.com
//
//   Developed by :
//   
//   Christian Schroeder <schroeder@eis.cs.tu-bs.de>,
//     Technical University of Braunschweig, Dept. E.I.S.
//     http://www.eis.cs.tu-bs.de
//
//
// 
// ENDLICENSETEXT


#include "ObserverModule.h"
#include <systemc.h>


ObserverModule::ObserverModule(sc_core::sc_module_name name)
: sc_core::sc_module(name)
{ 
  mApi = cci::get_cnf_api_instance(this);
  SC_THREAD(main_action);
}

ObserverModule::~ObserverModule() {
  // unregister all callbacks
  std::vector< boost::shared_ptr<cci::callb_adapt_b> >::iterator iter;
  for (iter = mCallbacks.begin(); iter != mCallbacks.end(); iter++) {
    (*iter)->unregister_at_parameter();
  }
}

void ObserverModule::main_action() {
  DEMO_DUMP(name(), "register new parameter callback");
  mApi->register_callback(cci::create_param, "*", this, &ObserverModule::config_new_param_callback);
  
  DEMO_DUMP(name(), "register callback for int_param");
  // Register Callback for parameter int_param in module other_ip (IP1)
  cci::cci_param_base* p = mApi->get_param("Owner.int_param");
  if (p != NULL) {
    boost::shared_ptr<cci::callb_adapt_b> cbAdapt_int_param;
    cbAdapt_int_param = p->register_callback(cci::post_write, this, &ObserverModule::config_callback);
    mCallbacks.push_back(cbAdapt_int_param);
  }
  

  cout << "----------------------------" << endl;
}


// ///////////////////// ///////////////// ///////////////////////////////////// //
// ///////////////////// reacting methods: ///////////////////////////////////// //
// ///////////////////// ///////////////// ///////////////////////////////////// //


void ObserverModule::config_new_param_callback(cci::cci_param_base& par) {
  DEMO_DUMP(name(), "New parameter callback method called:");
  cout << "  New parameter '" << par.get_name() << "'"<< endl;
}

// Callback function with default signature.
void ObserverModule::config_callback(cci::cci_param_base& par) {
  DEMO_DUMP(name(), "Callback method called:");
  cout << "  Parameter '" << par.get_name() << "'"<< endl;
  std::string str; par.get_string(str);
  cout << "    changed to value '" << str << "'." << endl;
  cout << endl;
}
