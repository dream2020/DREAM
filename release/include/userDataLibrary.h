 /** @file userDataLibrary.h 
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

#ifndef USERDATA_H
#define USERDATA_H

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

struct user_interaction_data_t {
	int m_iInteractionId;
	int m_iScriptId;
	string m_sPerformance;
	int m_iSessionId;
	string m_sSessionPartner;
	string m_sSessionName;
	string m_sEngagement;
	string m_sTurnTake;
	string m_sWP5Perf;
	string m_sScriptTime;
	string m_sScriptEnd;
	string m_sWP5Turn;
};

struct UserDataLibrary {
	// user data (1 entry)
	int iUserId;
	string sUserName;
	string sGender;
	string sPsych;
	string sADOSTotal;
	string sADOSComm;
	string sADOSSocial;
	string sADOSPlay;
	string sADOSStereo;
	string sADOSDiag;
	string sADOSSeverity;
	string sStereo1;
	string sStereo2;
	string sIQ;
	
	// interaction data (0+ entries)
	vector<user_interaction_data_t> m_user_interaction;
    
	void load(const string &filename) {
		ptree tree;						// Create empty property tree object
		read_xml(filename, tree);		// Parse the XML into the property tree.

		if (tree.empty()) {
			cout << "user data file empty!" << endl;
			return;
		}

		m_user_interaction.clear();
		BOOST_FOREACH( ptree::value_type const& v, tree.get_child("UserData") ) {
			if (v.first == "User") {
				sUserName = v.second.get<string>("Name");
				iUserId = v.second.get<int>("ID");
				sGender = v.second.get<string>("Sex");
				sPsych = v.second.get<string>("Psychiatric_Diagnosis");
				sADOSTotal = v.second.get<string>("ADOS_Total");
				sADOSComm = v.second.get<string>("ADOS_Communication");
				sADOSSocial = v.second.get<string>("ADOS_Social_Interaction");
				sADOSPlay = v.second.get<string>("ADOS_Play");
				sADOSStereo = v.second.get<string>("ADOS_Stereotyped_Behaviors");
				sADOSDiag = v.second.get<string>("ADOS_Diagnosis");
				sADOSSeverity = v.second.get<string>("ADOS_Severity");
				sStereo1 = v.second.get<string>("Stereotyped_Behavior_1_Name");
				sStereo2 = v.second.get<string>("Stereotyped_Behavior_2_Name");
				sIQ = v.second.get<string>("IQ");
			}
			else if (v.first == "Interaction") {
				user_interaction_data_t interaction;

				if( v.second.count("ID") != 0 ) interaction.m_iInteractionId = v.second.get<int>("ID");
				if( v.second.count("SessionID") != 0 ) interaction.m_iSessionId = v.second.get<int>("SessionID");
				if( v.second.count("SessionName") != 0 ) interaction.m_sSessionName = v.second.get<string>("SessionName");
				if( v.second.count("SessionPartner") != 0 ) interaction.m_sSessionPartner = v.second.get<string>("SessionPartner");
				if( v.second.count("ScriptID") != 0 ) interaction.m_iScriptId = v.second.get<int>("ScriptID");	
				if( v.second.count("Performance") != 0 ) interaction.m_sPerformance = v.second.get<string>("Performance");	// these 4 are actually string lists of ints
				if( v.second.count("Engagement") != 0 ) interaction.m_sEngagement = v.second.get<string>("Engagement");	    //
				if( v.second.count("TurnTake") != 0 ) interaction.m_sTurnTake = v.second.get<string>("TurnTake");			//
				if( v.second.count("WP5Perf") != 0 ) interaction.m_sWP5Perf = v.second.get<string>("WP5Perf");				//
				if( v.second.count("ScriptTime") != 0 ) interaction.m_sScriptTime = v.second.get<string>("ScriptTime");
				if( v.second.count("ScriptEnd") != 0 ) interaction.m_sScriptEnd = v.second.get<string>("ScriptEnd");
				if( v.second.count("WP5Turn") != 0 ) interaction.m_sWP5Turn = v.second.get<string>("WP5Turn");
				
				m_user_interaction.push_back(interaction);
			}
		}
	}
};

#endif