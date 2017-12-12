/*****************************************************************************

  Licensed to Accellera Systems Initiative Inc. (Accellera) under one or
  more contributor license agreements.  See the NOTICE file distributed
  with this work for additional information regarding copyright ownership.
  Accellera licenses this file to you under the Apache License, Version 2.0
  (the "License"); you may not use this file except in compliance with the
  License.  You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
  implied.  See the License for the specific language governing
  permissions and limitations under the License.

 ****************************************************************************/

/**
 *  @file     parameter_configurator.h
 *  @brief    This header declares and defines configurator
 *  @author   Girish Verma, CircuitSutra Technologies     <girish@circuitsutra.com>
 *            P V S Phaneendra, CircuitSutra Technologies <pvs@circuitsutra.com>
 *  @date     14th December, 2011
 *            16th May, 2011 (Monday)
 */

#ifndef EXAMPLES_EX19_API_AND_PARAMETER_IMPLEMENTATIONS_FROM_DIFFERENT_PROVIDERS_EX19_PARAMETER_CONFIGURATOR_H_
#define EXAMPLES_EX19_API_AND_PARAMETER_IMPLEMENTATIONS_FROM_DIFFERENT_PROVIDERS_EX19_PARAMETER_CONFIGURATOR_H_

#include <cci_configuration>
#include "xreport.hpp"

/**
 *  @class  ex19_parameter_configurator
 *  @brief  The configurator class illustrates the different ways in which the value
 *          of a cci-parameter of the owner class can be accessed using both a
 *          cci_base_parameter object and a cci_param object (by making the
 *          configurator a friend class to the owner)
 */
SC_MODULE(ex19_parameter_configurator) {
 public:
  /**
   *  @fn     ex19_parameter_configurator()
   *  @brief  The class cosntructor.
   *  @return void
   */
  SC_CTOR(ex19_parameter_configurator)
      : m_broker(cci::cci_get_broker()),
        cfgr_param_handle(cci::cci_originator(*this)),
        cfgr_user_param_handle(cci::cci_originator(*this)),
        cfgr_shared_param(0)
  {

    // Checks the parameter exists using name-based look-up
    cfgr_param_handle =
        m_broker.get_param_handle("param_owner.mutable_int_param");
    if (cfgr_param_handle.is_valid()) {
      XREPORT("[CFGR C_TOR] : Parameter param_owner.mutable_int_param exists");
    } else {
      XREPORT("[CFGR C_TOR] : Parameter param_owner.mutable_int_param"
              " doesn't exists.");
    }

    cfgr_user_param_handle =
        m_broker.get_param_handle("param_owner.mutable_udtype_param");
    if (cfgr_user_param_handle.is_valid()) {
      XREPORT("[CFGR C_TOR] : Parameter param_owner.mutable_udtype_param"
              " exists");
      XREPORT("[CFGR C_TOR] : Got param_owner.mutable_udtype_param ");
    } else {
      XREPORT("[CFGR C_TOR] : Parameter param_owner.mutable_udtype_param"
              " doesn't exists.");
      XREPORT("Try Again with mutable_udtype_param ");

      cci::cci_param_handle temp_handle =
          m_broker.get_param_handle("mutable_udtype_param");
      if (temp_handle.is_valid()) {
        XREPORT("Got Now with mutable_udtype_param");
      } else {
        XREPORT("It will always Fail :(");
      }
    }

    // Set parameter value using cci_base_parameter object
    XREPORT("[CFGR C_TOR] : Set parameter value to 10 using"
            " cci_base_parameter");
    cfgr_param_handle.set_cci_value(cci::cci_value(10));

    /// Registering SC_THREAD with the SystemC kernel
    SC_THREAD(run_mutable_cfgr);
  }

  /**
   *  @fn     void run_mutable_cfgr(void)
   *  @brief  This process illustrates use of the cci_param (shared parameter) that
   *          stores the reference of the owner cci-parameter to make changes
   *          to the parameter's value
   *  @return void
   */
  void run_mutable_cfgr(void) {
    while (1) {
      wait(10.0, sc_core::SC_NS);

      XREPORT("@ " << sc_core::sc_time_stamp());
      XREPORT("[CFGR] : Parameter Value   : " << cfgr_shared_param->get_value());
      if (cfgr_user_param_handle.is_valid()) {
        XREPORT("[CFGR] : Get :Parameter Value[User data type]   : "
                << cfgr_user_param_handle.get_cci_value().to_json());

        cfgr_user_param_handle.set_cci_value(cci::cci_value::from_json("EXP"));

        XREPORT("[CFGR] : Get :Parameter Value[User data type]   : "
                << cfgr_user_param_handle.get_cci_value().to_json());
      }

      wait(5.0, sc_core::SC_NS);

      XREPORT("@ " << sc_core::sc_time_stamp());
      XREPORT("[CFGR] : Change value to 20");
      *cfgr_shared_param = 20;

      wait(50.0, sc_core::SC_NS);
    }
  }

  /**
   *  @fn     void set_cfgr_parameter(ex19_parameter_owner *owner)
   *  @brief  This parameter gets the reference of the 'int_param', a private-type
   *          cci-parameter member of the owner class
   *  @param  owner The parameter owner.
   *  @return void
   */
  void set_cfgr_parameter(ex19_parameter_owner *owner) {
    // Getting reference of the owner parameter
    cfgr_shared_param = &owner->int_param;
  }

 private:
  cci::cci_broker_handle m_broker;  ///< Declaring a CCI configuration broker handle

  // Declaring a CCI base parameter pointer
  cci::cci_param_handle cfgr_param_handle; ///< Handle to a cci parameter
  cci::cci_param_handle cfgr_user_param_handle;  ///< Handle to a cci parameter that is accessible by the user

  cci::cci_param<int>* cfgr_shared_param;  ///< Declaring a CCI parameter pointer (which will hold the reference of the owner CCI parameter 'int_param'
};
// ex19_parameter_configurator

#endif  // EXAMPLES_EX19_API_AND_PARAMETER_IMPLEMENTATIONS_FROM_DIFFERENT_PROVIDERS_EX19_PARAMETER_CONFIGURATOR_H_
