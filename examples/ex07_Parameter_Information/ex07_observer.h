/*******************************************************************************
 *   The following code is derived, directly or indirectly, from the SystemC
 *   source code Copyright (c) 1996-2010 by all Contributors.
 *   All Rights reserved.
 *
 *   The contents of this file are subject to the restrictions and limitations
 *   set forth in the SystemC Open Source License Version 2.2.0 (the "License");
 *   One may not use this file except in compliance with such restrictions and
 *   limitations.  One may obtain instructions on how to receive a copy of the
 *   License at http://www.systemc.org/.  Software distributed by Contributors
 *   under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF
 *   ANY KIND, either express or implied. See the License for the specific
 *   language governing rights and limitations under the License.
 *******************************************************************************/

/*!
 * \file    observer.h
 * \brief   This file defines an observer class
 * \author  P V S Phaneendra, CircuitSutra Technologies   <pvs@circuitsutra.com>
 * \date    12th September, 2011 (Monday)
 */
#ifndef EXAMPLES_EX07_PARAMETER_INFORMATION_EX07_OBSERVER_H_
#define EXAMPLES_EX07_PARAMETER_INFORMATION_EX07_OBSERVER_H_

#include <cci>
#include <cassert>
#include <vector>
#include "xreport.hpp"

/// SC_MODULE registers callbacks on CCI parameters
SC_MODULE(ex07_observer) {
 public:
  SC_CTOR(ex07_observer) {
    /// Instantiate a cci_originator in order to get hold
    /// of the configuration broker interface
    cci::cnf::cci_originator observerOriginator("observerOriginator");

    /// Get the broker responsible for this module using
    /// 'get_current_broker' API
    observerBrokerIF = &cci::cnf::cci_broker_manager::get_current_broker(
        observerOriginator);

    // Assert if broker handle returned is NULL
    assert(observerBrokerIF != NULL
           && "Observer Broker Handle Returned is NULL");

    /// Check for the broker type (default or private) using
    /// 'is_private_broker()' API
    if (observerBrokerIF->is_private_broker()) {
      /// Access broker's name using 'name()'
      XREPORT("[OBSERVER C_TOR] : Broker Type : " << observerBrokerIF->name());
    } else {
      XREPORT("[OBSERVER C_TOR] : Broker Type : " << observerBrokerIF->name()
              << "- is not a private broker.");
    }

    /// Gets the reference to the 'int' type cci-parameter of OWNER module
    obsv_int_base_ptr = observerBrokerIF->get_param(
        "param_owner.mutable_int_param");

    assert(obsv_int_base_ptr != NULL
           && "Returned Handle of 'integer type' cci-parameter is NULL");

    /// Observer registering PRE_READ, PRE_WRITE & POST_WRITE callbacks
    /// on the integer parameter to monitor all actions on it
    int_pre_read_cb = obsv_int_base_ptr->register_callback(
        cci::cnf::pre_read, this,
        cci::bind(&ex07_observer::read_callback, this, _1, _2));
    int_pre_write_cb = obsv_int_base_ptr->register_callback(
        cci::cnf::pre_write, this,
        cci::bind(&ex07_observer::write_callbacks, this, _1, _2));
    int_post_write_cb = obsv_int_base_ptr->register_callback(
        cci::cnf::post_write, this,
        cci::bind(&ex07_observer::write_callbacks, this, _1, _2));
  }

  /*!
   *  \fn      cci::cnf::callback_return_type read_callback(cci::cnf::cci_base_param &, const cci::cnf::callback_type &)
   *  \brief   'PRE_READ' Callbacks Implementations
   *  \param   cci::cnf::cci_base_param&   Reference of cci_base_param on which the (pre-read) callback is registered
   *  \param   cci::cnf::callback_type&  Callback type
   *  \return  cci::cnf::callback_return_type  Callback return type
   */
  cci::cnf::callback_return_type read_callback(
      cci::cnf::cci_base_param & _selected_base_param,
      const cci::cnf::callback_type & cb_reason) {
    switch (cb_reason) {
      case cci::cnf::pre_read: {
        const cci::cnf::cci_originator* myOriginator =
            cci::cnf::cci_originator::get_global_originator();

        XREPORT("[OBSERVER pre_read_cb] :  Parameter Name : "
                << _selected_base_param.get_name() << "\tOriginator info : "
                << myOriginator->name());

        break;
      }

      default:
        XREPORT("[OBSERVER pre_read_cb] - Unknown Callback Type");
    }

    return cci::cnf::return_nothing;
  }

  /*!
   *  \fn      cci::cnf::callback_return_type write_callback(cci::cnf::cci_base_param &, const cci::cnf::callback_type &)
   *  \brief   'PRE_WRITE' & 'POST_WRITE' Callbacks Implementations
   *  \param   cci::cnf::cci_base_param&   Reference of cci_base_param on which the (pre/post-write) callback is registered
   *  \param   cci::cnf::callback_type&  Callback type (pre-read or post-read)
   *  \return  cci::cnf::callback_return_type  Callback return type
   */
  cci::cnf::callback_return_type write_callbacks(
      cci::cnf::cci_base_param & _selected_base_param,
      const cci::cnf::callback_type & cb_reason) {
    const cci::cnf::cci_originator* myOriginator =
        cci::cnf::cci_originator::get_global_originator();

    switch (cb_reason) {
      case cci::cnf::pre_write: {
        XREPORT("[OBSERVER pre_write_cb] :  Parameter Name : "
                << _selected_base_param.get_name() << "\tOriginator info : "
                << myOriginator->name());
        break;
      }

      case cci::cnf::post_write: {
        XREPORT("[OBSERVER post_write_cb] :  Parameter Name : "
                << _selected_base_param.get_name() << "\tOriginator info : "
                << myOriginator->name());
        break;
      }

      default:
        XREPORT("[OBSERVER write_cb] - Unknown Callback Type");
    }

    return cci::cnf::return_nothing;
  }

 private:
  // CCI configuration broker instance
  //!< Configuration broker instance
  cci::cnf::cci_cnf_broker_if* observerBrokerIF;

  // Declare cci_base_param for int type cci-parameter
  //!< cci_base_param for integer type cci-param
  cci::cnf::cci_base_param* obsv_int_base_ptr;

  // Callback Adaptor Objects for 'int' type parameter
  //!< 'pre_read' callback adaptor obj for int type cci-param
  cci::shared_ptr<cci::cnf::callb_adapt> int_pre_read_cb;
  //!< 'pre_write' callback adaptor obj for int type cci-param
  cci::shared_ptr<cci::cnf::callb_adapt> int_pre_write_cb;
  //!< 'post_write' callback adaptor obj for int type cci-param
  cci::shared_ptr<cci::cnf::callb_adapt> int_post_write_cb;
};
// ex07_observer

#endif  // EXAMPLES_EX07_PARAMETER_INFORMATION_EX07_OBSERVER_H_
