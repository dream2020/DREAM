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
  * 13/06/2016 Version 1.0: ... (James Kennedy)
  */ 

#include "userModelController.h"
#include "userModelYarpInterface.h"

using namespace std;
using namespace boost;
using boost::property_tree::ptree;

UserModelController::UserModelController(UserModelYarpInterface *yarpInterface, string sUserDataFolder){
	_yarpInterface = yarpInterface;
	_iCurrentUser = -1;

	_sUserDataFolder = boost::filesystem::path(sUserDataFolder.c_str()).parent_path().string() + USER_DATA_FOLDER;
	_sUserDataListFile = _sUserDataFolder + USER_LIST_FILENAME;
	_userList.load(_sUserDataListFile);		// create user list
}

void UserModelController::newUserID(int iUserID) {
	if (iUserID >= 0) {
		bool bUserExists = false;

		// check if we actually have this user in our list
		for (unsigned int i = 0; i < _userList.m_users.size(); i++) {
			if (_userList.m_users[i].m_id == iUserID) {
				bUserExists = true;
				_iCurrentUser = iUserID;
			}
		}

		if (!bUserExists) {
			// incorrect user ID
			cout << "<<<userModel>>> error: received bad user ID (does not exist)" << endl;
		}
		else {
			loadAndSendUserHistoryByID(iUserID);
		}
	}
	else {
		// error, so flag
		cout << "<<<userModel>>> error: received bad user ID (less than 0)" << endl;
	}
}

void UserModelController::loadAndSendUserHistoryByID(int iUserID) {
	stringstream ss;
	ss << iUserID;
	string sUserID = ss.str();
	string sFilePath = _sUserDataFolder + "user_" + sUserID + ".user";
	_userData.load(sFilePath);
	_yarpInterface->sendToUserData(_userData);
}

bool UserModelController::saveNewInteractionToFile(user_interaction_data_t newInteraction) {
	try {
        bool bExists = false;

		string sFilePath;
		stringstream ss;
		ss << _iCurrentUser;
		string sUserID = ss.str();
		sFilePath = _sUserDataFolder + "user_" + sUserID + ".user";

        int iID = newInteraction.m_iInteractionId;
        int iSessionID = newInteraction.m_iSessionId;
        int iScriptID = newInteraction.m_iScriptId;
        string sPerformance = newInteraction.m_sPerformance;
        string sEngagement = newInteraction.m_sEngagement;
        string sTurnTake = newInteraction.m_sTurnTake;
        string sWP5Performance = newInteraction.m_sWP5Perf;
		string sScriptTime = newInteraction.m_sScriptTime;
		string sScriptEnd = newInteraction.m_sScriptEnd;
		string sWP5Turn = newInteraction.m_sWP5Turn;

        int iThisID = -1;
        int iThisSessionID = -1;
        int iThisScriptID = -1;
		string sThisScriptTime = "";

		// read in the current XML file to append to
		ptree treeRead;
		read_xml(sFilePath, treeRead, boost::property_tree::xml_parser::trim_whitespace);

        // Read
        BOOST_FOREACH(ptree::value_type &v, treeRead.get_child("UserData")) {
           if (v.first == "Interaction") {
                user_interaction_data_t interaction;

                if (v.second.count("ID") != 0) iThisID = v.second.get<int>("ID");
                if (v.second.count("SessionID") != 0) iThisSessionID = v.second.get<int>("SessionID");
                if (v.second.count("ScriptID") != 0) iThisScriptID = v.second.get<int>("ScriptID");
				if (v.second.count("ScriptTime") != 0) sThisScriptTime = v.second.get<string>("ScriptTime");
                
                // if we already have some data, update it
                if (iThisID == iID && iSessionID == iThisSessionID && iThisScriptID == iScriptID && sThisScriptTime == sScriptTime) {
                    v.second.put("Performance", sPerformance);
                    v.second.put("Engagement", sEngagement);
                    v.second.put("TurnTake", sTurnTake);
                    v.second.put("WP5Perf", sWP5Performance);
					v.second.put("ScriptEnd", sScriptEnd);
					v.second.put("WP5Turn", sWP5Turn);

                    bExists = true;
                }

                if (bExists) {
                    break;
                }
            }
        }

        // we don't already have this, so write fresh tree
        if (!bExists) {
            // create new XML node with new interaction data
            ptree newIntTree;
            newIntTree.put("ID", iID);
            newIntTree.put("SessionID", iSessionID);
            newIntTree.put("SessionName", newInteraction.m_sSessionName);
            newIntTree.put("SessionPartner", newInteraction.m_sSessionPartner);
            newIntTree.put("ScriptID", iScriptID);
            newIntTree.put("Performance", sPerformance);
            newIntTree.put("Engagement", sEngagement);
            newIntTree.put("TurnTake", sTurnTake);
            newIntTree.put("WP5Perf", sWP5Performance);
			newIntTree.put("ScriptTime", sScriptTime);
			newIntTree.put("ScriptEnd", sScriptEnd);
			newIntTree.put("WP5Turn", sWP5Turn);

            // add node to existing tree
            treeRead.add_child("UserData.Interaction", newIntTree);
        }

		// write back full tree (first line sorts indents - otherwise output is messy)
		boost::property_tree::xml_writer_settings<char> settings(' ', 4);
		write_xml(sFilePath, treeRead, std::locale(), settings);

		return true;
	}
	catch (...) {
		return false;
	}
}