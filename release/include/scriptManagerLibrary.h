 /** @file scriptManagerLibrary.h 
  *
  * Version 1.0
  * 
  *
  *
  * \defgroup DREAM_cognitiveComponent systemGUI
  * 
  * Main class with code, use the yarpInterface class to communicate with the other components.
  * 
  * \section lib_sec Libraries
  *
  * None
  *
  * \section in_files_sec Input Data Files
  *
  * None
  *
  * \section out_data_sec Output Data Files
  *
  * None
  *
  * \author 
  * 
  * <Hoang-Long Cao>, <Vrije Universiteit Brussel>  
  * 
  * Copyright (C) 2014 DREAM Consortium
  * 
  */

/* 
 * Copyright (C) 2014 DREAM Consortium
 * FP7 Project 611391 co-funded by the European Commission
 *
 * Author:  Hoang-Long Cao, Vrije Universiteit Brussel 
 * Email:   hoang.long.cao@vub.ac.be
 * Website: www.dream2020.eu 
 * 
 * This file is part of DREAM.
 * 
 * DREAM is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * DREAM is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with DREAM.  If not, see <http://www.gnu.org/licenses/>.
 */
 
 /* 
  * Audit Trail
  * -----------
  * 06/07/2016 Version 1.0: ... (Hoang-Long Cao)
  */

#ifndef SCRIPTMANAGER_H
#define SCRIPTMANAGER_H

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/foreach.hpp>
#include <string>
#include <set>
#include <exception>
#include <iostream>
#include <vector>

using namespace std;
using boost::property_tree::ptree;

struct script_t {
	string m_name;
	int m_id;
	int m_type_id;
	vector<string> m_steps;
};

struct ScriptLibrary{
	vector<script_t> m_scripts;
	
    void load(const string &filename){
		// Create empty property tree object
		ptree tree;
		// Parse the XML into the property tree.
		read_xml(filename, tree);	

		if (tree.empty()) {
			cout << "script file empty!" << endl;
			return;
		}

		m_scripts.clear();
		BOOST_FOREACH( ptree::value_type const& v, tree.get_child("Scripts") ) {
			if (v.first == "Script") {
				script_t script;				
				script.m_name = v.second.get<string>("Name");
				cout << script.m_name << endl;
				script.m_id = v.second.get<int>("ID");
				script.m_type_id = v.second.get<int>("TypeID");
				ptree stree = (ptree) v.second.get_child("Steps");
				
				BOOST_FOREACH(ptree::value_type &vs,stree) {
					script.m_steps.push_back(vs.second.data());					
				}
				
				m_scripts.push_back(script);
			}
		}		
	}
};
#endif