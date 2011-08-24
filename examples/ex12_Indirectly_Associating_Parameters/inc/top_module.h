/*******************************************************************************
 * The following code is derived, directly or indirectly, from the SystemC
 * source code Copyright (c) 1996-2010 by all Contributors.
 * All Rights reserved.
 * 
 * The contents of this file are subject to the restrictions and limitations
 * set forth in the SystemC Open Source License Version 2.2.0 (the "License");
 * One may not use this file except in compliance with such restrictions and
 * limitations.  One may obtain instructions on how to receive a copy of the
 * License at http://www.systemc.org/.  Software distributed by Contributors
 * under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF
 * ANY KIND, either express or implied. See the License for the specific
 * language governing rights and limitations under the License.
 * *******************************************************************************/

/**
 * @file     top_module.h
 * @brief    This header declares and defines the top module which instantiates the two owner modules 
 * @author   P V S Phaneendra, CircuitSutra Technologies Pvt. Ltd.
 * @date     9th June, 2011 (Thursday)
 */
#ifndef TOP_MODULE_H
#define TOP_MODULE_H

/// Include the "cci.h" header file in all cci-based applications
#include <cci.h>
#include <assert.h>
#include <vector>
#include <sstream>

#include "parameter_owner.h"
#include "param_value_sync.h"

/**
 * @brief    The configurator class registers 'post_write' callbacks for the
 *           owner's parameters in order to update an owner cci_parameter directly
 *           when another cci_parameter value is modified 
 * @author   P V S Phaneendra, CircuitSutra Technologies Pvt. Ltd.
 * @date     9th June, 2011 (Thursday)
 */ 
class top_module : public sc_module
{
	public:
		
		/// Pointers to the owner module
		parameter_owner*   param_owner1;
		parameter_owner*   param_owner2;

		/// Create an instance of the 'param_value_sync' class
		param_value_sync   *param_sync;

		/// Default constructor
		SC_CTOR(top_module)
		{
			/// Get handle of the broker responsible for the class/module
			myTopModBrokerIF	=	&cci::cnf::cci_broker_manager::get_current_broker(cci::cnf::cci_originator(*this));
		
			std::string str1,str2;		/*!Strings to store the names of the owner's parameters*/
			str1="clk_freq_Hz";
			str2="clock_speed_KHz";	
			param_owner1	= new parameter_owner("param_owner1", str1, 1);
			param_owner2	= new parameter_owner("param_owner2", str2, 2);

			/// Report if handle returned is NULL
			assert(myTopModBrokerIF != NULL && "Configuration Broker handle is NULL");
			
			/// Check for existence of the owner cci_parameter using name-based look up access
			/// and then assign their reference to respective cci_base_param
			std::string param1_str = "top_mod.param_owner1.clk_freq_Hz";
			std::string param2_str = "top_mod.param_owner2.clock_speed_KHz";

			if(myTopModBrokerIF->exists_param(param1_str))
			{
				cci::cnf::cci_base_param *temp = myTopModBrokerIF->get_param(param1_str);
				selectedBaseParamList.push_back(temp);
			
				std::cout << "\n\t[TOP_MODULE C_TOR] : Parameter Name : " << temp->get_name()	\
					<< "\tParameter Value : " << temp->json_serialize() << std::endl;
			}
			else
				std::cout << "\t[TOP_MODULE C_TOR] : Parameter Name : " << param1_str << "\tnot found." << std::endl;

			/// Check for existence of the owner cci_parameter using name-based look up access
			/// and then assign their reference to respective cci_base_param
			if(myTopModBrokerIF->exists_param(param2_str))
			{
				cci::cnf::cci_base_param *temp = myTopModBrokerIF->get_param(param2_str);
				selectedBaseParamList.push_back(temp);
			
				std::cout << "\n\t[TOP_MODULE C_TOR] : Parameter Name : " << temp->get_name()	\
					<< "\tParameter Value : " << temp->json_serialize() << std::endl;
			}
			else
				std::cout << "\t[TOP_MODULE C_TOR] : Parameter Name : " << param2_str << "\tnot found." << std::endl;


			/// Pass the selected base parameters list to the 'param_value_sync' class
			/// synchronization related activity
			param_sync	=	new param_value_sync(selectedBaseParamList);
			

	}// End of Constructor


	private	:
	
	/// Declaring a CCI configuration broker interface instance
	cci::cnf::cci_cnf_broker_if* myTopModBrokerIF;
	
	/// std::vector storing the searched owner parameters references to CCI base parameter pointers
	std::vector<cci::cnf::cci_base_param*> selectedBaseParamList;

};// End of Class/SC_MODULE

#endif	// End of TOP_MODULE_H

