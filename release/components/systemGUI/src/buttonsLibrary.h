 /** @file buttonsLibrary.h 
  *
  * Version 1.0
  * 
  *
  * \defgroup DREAM_cognitiveComponent systemGUI
  * 
  * Read button data from a XML file.
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
  * 05/07/2016 Version 1.0: ... (Hoang-Long Cao)
  */

#ifndef BUTTONSLIBRARY_H
#define BUTTONSLIBRARY_H

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

struct button_t {
	string m_label;
	string m_icon;
	int m_color;
	string m_action;
} ;

struct buttons_t{
	string m_id;
    vector<button_t> m_buttons;
};

struct ButtonsLirary{
	vector<buttons_t> m_buttons_library;
    void load(const string &filename, string type){
		// Create empty property tree object
		ptree tree;
		// Parse the XML into the property tree.
		read_xml(filename, tree);

		m_buttons_library.clear();

		if (tree.empty()) {
			cout << "Buttons file empty!" << endl;
			return;
		}
		// Read
		BOOST_FOREACH( ptree::value_type const& v, tree.get_child("GUIButtons") ) {
			if( v.first == type ) {
				buttons_t buttons;
				buttons.m_id = v.second.get("<xmlattr>.id","");
				ptree stree = (ptree) v.second ;
				BOOST_FOREACH(ptree::value_type &vs,stree){
					if (vs.first == "Button") {
						button_t button;
						int i = 0;
						ptree sstree = (ptree) vs.second ;
						button.m_label = vs.second.get<string>("Label","");
						button.m_icon = vs.second.get<string>("Icon","");
						button.m_color = vs.second.get<unsigned>("Color",0);
						button.m_action = vs.second.get<string>("Action","");
						//cout << button.m_label << endl;
						buttons.m_buttons.push_back(button);							
					}
				}
				m_buttons_library.push_back(buttons);				
				//cout << "Buttons - "<< type <<" is loaded!" << endl;
			}			
		}
		cout << "Buttons - "<< type <<" is loaded!" << endl;

	}
};
#endif