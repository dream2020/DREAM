 /** @file userListLibrary.h 
  *
  * Version 1.0
  * 
  *
  *
  * \defgroup DREAM_cognitiveComponent include header
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
  * <James Kennedy>, <Plymouth University>  
  * 
  * Copyright (C) 2014 DREAM Consortium
  * 
  */

/* 
 * Copyright (C) 2014 DREAM Consortium
 * FP7 Project 611391 co-funded by the European Commission
 *
 * Author:  James Kennedy, Plymouth University 
 * Email:   james.kennedy@plymouth.ac.uk
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
  * 06/07/2016 Version 1.0: creation (James Kennedy)
  */

#ifndef USERLIST_H
#define USERLIST_H

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

struct user_list_t {
	string m_name;
	int m_id;
};

struct UserListLibrary {
	vector<user_list_t> m_users;
	
    void load(const string &filename) {
		ptree tree;						// Create empty property tree object
		read_xml(filename, tree);		// Parse the XML into the property tree.

		if (tree.empty()) {
			cout << "user list file empty!" << endl;
			return;
		}

		m_users.clear();
		BOOST_FOREACH( ptree::value_type const& v, tree.get_child("UserList") ) {
			if (v.first == "User") {
				user_list_t user;				
				user.m_name = v.second.get<string>("Name");
				cout << user.m_name << endl;
				user.m_id = v.second.get<int>("ID");
				m_users.push_back(user);
			}
		}
	}
};
#endif