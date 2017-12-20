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
  * 22/06/2016 Version 1.0: ... (Hoang-Long Cao)
  * 25/01/2017 Version 2.0: ... (Hoang-Long Cao)
  */ 

#include "systemGUIController.h"
#include "systemGUIYarpInterface.h"

using namespace std;
using namespace boost;

namespace common_functions
{
	//methods used to split a string
	vector<string> split(const string &s, char delim, vector<string> &elems) {
		stringstream ss(s);
		string item;
		while (getline(ss, item, delim)) {
			elems.push_back(item);
		}
		return elems;
	}

	vector<string> split(const string &s, char delim) {
		vector<string> elems;
		split(s, delim, elems);
		return elems;
	}

	//method used to remove a string from another string
	string removeString(string base, string toRemove) {
		string::size_type index = base.find(toRemove);
		if (index != string::npos) {
			base.erase(index, toRemove.length());
		}

		return base;
	}
}

using namespace common_functions;

SystemGUIController::SystemGUIController(SystemGUIYarpInterface *yarpInterface, string logo ){
	_yarpInterface = yarpInterface;
	/* Initialize GUI elements' variables */
	_debug = true;

	_logoUrl = logo;	
	_iconUrl =  boost:: filesystem::path(_logoUrl.c_str()).parent_path().string() +ICONS_FOLDER;
	_logoImage = new Fl_PNG_Image(_logoUrl.c_str());
	_dream_logoUrl = boost::filesystem::path(_logoUrl.c_str()).parent_path().string() + "/dream-eu-logo.png";
	_dream_logoImage = new Fl_PNG_Image(_dream_logoUrl.c_str());
	_logUrl = boost:: filesystem::path(_logoUrl.c_str()).parent_path().string() +LOGS_FOLDER;
	boost:: filesystem::create_directory(_logUrl.c_str());
	_xml_folder = boost::filesystem::path(_logoUrl.c_str()).parent_path().string() +XML_FOLDER;
	_scriptManager_folder = boost::filesystem::path(_logoUrl.c_str()).parent_path().parent_path().parent_path().string() +SCRIPTMANAGER_FOLDER;
	
	_sUserDataFolder = boost::filesystem::path(_logoUrl.c_str()).parent_path().parent_path().parent_path().string() + USER_DATA_FOLDER;
	_sUserDataListFile = _sUserDataFolder + USER_LIST_FILENAME;


	_userName_choice_value = 0;
	_sessionPartner_choice_value = 0;
	_sessionPartner = "Robot";

	_scriptType_choice_value = 0;
	_scriptType = "Intervention";
	
	_script_choice_value = 0;
	_currentUserInteractionData.m_sPerformance= "";
	_currentPerformanceString = calculateCurrentPerformance(_currentUserInteractionData.m_sPerformance);
	sprintf(_currentPerformanceDisplay, _currentPerformanceString.c_str());

	sprintf(_connectionMessage,"");
	_isRobotConnected = false;			
	_isSandtrayConnected =  false;

	// Load user name list and script list
	loadUserNames();	
	loadScripts();	
	
	_currentStep = -1;
	_currentStep_old = -1;

	_isGoToNextStep = false;
	_isScriptRunning = false;
	_isSimulationRunning = false;
	_isStopTimerAction = false;
	_timer_value = TIMER_VALUE;
	_timer_value_temp = TIMER_VALUE;
	_current_timer_value = 0;// test
	_progress_Timer_value = _current_timer_value/_timer_value;
	_progress_bar_color = FL_GRAY0;
	//_actionTimeDisplay = actionTimeDisplay(_current_timer_value);
	_actionTimeDisplay = "";
	_suggestionLabel = "";
	_suggestionMessage = "";

	_proposedActionID = _ACTION_NO_ACTION_;

	_waitingForGaze = false;
	_waitingForPerf = false;
	_backFromWaitingForGazePerf = false;
	_backFromStepMod = false;
	_feedbackStep = false;
	_feedbackCorrected = false;	

	loadFixedButtons();
	loadAlwaysOnButtons();
	loadFeedbackButtons();
	loadBetweenScriptsButtons();
	_checkButton_enableRAS_text.push_back("Reaction Subsystem");
	_checkButton_enableRAS_text.push_back("Attention Subsystem");
	
	//loadChildHistory();
	_output_userData_text.clear();
	_output_userData_text.push_back("Name");
	_output_userData_text.push_back("Sex");
	_output_userData_text.push_back("Psychiatric diagnosis");
	_output_userData_text.push_back("ADOS total");
	_output_userData_text.push_back("ADOS communication");
	_output_userData_text.push_back("ADOS social interaction");
	_output_userData_text.push_back("ADOS play");
	_output_userData_text.push_back("ADOS stereotyped behaviors");
	_output_userData_text.push_back("ADOS diagnosis");
	_output_userData_text.push_back("ADOS diagnosis");
	_output_userData_text.push_back("Stereotyped behavior 1 name");
	_output_userData_text.push_back("Stereotyped behavior 2 name");
	_output_userData_text.push_back("IQ (Raven)");

	_script = "";
	_list_scriptStepsBuffer = new Fl_Text_Buffer();
	_list_developerInfoBuffer = new Fl_Text_Buffer();

	/* Initialize GUI */
	GUIinit();
	/* Enable/Disable widgets*/
	_tab_loadScript->deactivate();
	_button_loadScript->deactivate();
	//_box_onScript->deactivate();
	//_box_offScript->deactivate();
	/* GUI - loop */
	_tGUIloop = new boost::thread(&SystemGUIController::GUIloop,this);
	logWrite("System starts");
}

void SystemGUIController::GUIloop(){
	// Create main window
	string GUIlabel = "DREAM System Control Panel v." + string(SYSTEMGUI_VERSION);
	DVwindow = new Fl_Double_Window(GUI_X,GUI_Y,GUI_WIDTH,GUI_HEIGHT,GUIlabel.c_str());
	Fl::visual(FL_DOUBLE); // enable double buffering of the window 
	DVwindow->end(); 
	
	// Add the GUI
	DVwindow->add(_start_systemArchitecture_GUI);
	//DVwindow->resizable(_start_systemArchitecture_GUI);
	DVwindow->redraw();
	DVwindow->iconize();
	DVwindow->icon(_logoImage);
	DVwindow->show();

	// Default value for debugging
	if (_debug == true) {
		_choice_userName->value(0);
		_userName=_userName_list[0];
		_textInput_sessionName->value("default");
		sprintf(_textInput_sessionName_value,"default");
	}

	// Loop to check GUI events
	while (true) { 
		DVwindow->redraw();
		Fl::check();         // refresh the GUI
		/* Check simulation events */
		checkProposedAction();
		goToNextStep();
		
		/* Check GUI events */
		checkRefreshEvent();
		checkConnections();
		checkLoadScriptChanged();
		button_loadUser_Clicked();
		button_loadScript_Clicked();
		button_updateSliderTimer_Clicked();		
		buttons_onScript_Clicked();
		buttons_offScript_Clicked();
		buttons_betweenScripts_Clicked();
		checkButton_enableRAS_Changed();
		button_exportLogfile_Clicked();

		//Sleep(50);
		timer_miliseconds(50);
		//cout <<"The loop is working..."<<endl;
		//cout <<"Script loop: "<< i <<endl;
	}	
}
void SystemGUIController::loadUserNames() {
	_userName_list.clear();
	
	// JK help to replace this with new userModel format:
	// import: #include "userListLibrary.h" (shared header from /include - may need to rebuild first to get include path)
	// create variables in header: _sUserDataFolder, _sUserDataListFile, _userList
	// create defines in header: #define USER_DATA_FOLDER "/userdata/" & #define USER_LIST_FILENAME "userlist.dream"
	// first line below should end up in /components/userModel/config/userdata/ (copied from userModel, so will need rewriting)
	// _sUserDataFolder = boost::filesystem::path(sUserDataFolder.c_str()).parent_path().string() + USER_DATA_FOLDER;
	// _sUserDataListFile = _sUserDataFolder + USER_LIST_FILENAME;
	// _userList.load(_sUserDataListFile);		// create user list
	// can read name and ID for each user _userList.m_users[i].m_name / _userList.m_users[i].m_id
	// I need the ID passed from GUI to SMS when a user is selected (see button handler ln877 / sendToUserID() method in GUIYarpInterface.cpp)
	// HLC: Done
	_userList.load(_sUserDataListFile);		// create user list
	for (unsigned int i = 0; i<_userList.m_users.size(); i++) {
		_userName_list.push_back(_userList.m_users[i].m_name);		
	}
}

void SystemGUIController::loadUserData(int iUserID) {
	stringstream ss;
	ss << iUserID;
	string sUserID = ss.str();
	string sFilePath = _sUserDataFolder + "user_" + sUserID + ".user";
	_userData.load(sFilePath);

	_output_userData_value.resize(_output_userData_text.size());

	_output_userData_value[0] = _userData.sUserName;
	_output_userData_value[1] = _userData.sGender;
	_output_userData_value[2] = _userData.sPsych;
	_output_userData_value[3] = _userData.sADOSTotal;
	_output_userData_value[4] = _userData.sADOSComm;
	_output_userData_value[5] = _userData.sADOSSocial;
	_output_userData_value[6] = _userData.sADOSPlay;
	_output_userData_value[7] = _userData.sADOSStereo;
	_output_userData_value[8] = _userData.sADOSDiag;
	_output_userData_value[9] = _userData.sADOSSeverity;
	_output_userData_value[10] = _userData.sStereo1;
	_output_userData_value[11] = _userData.sStereo2;
	_output_userData_value[12]= _userData.sIQ;	

	_output_userHistory.clear();
	for (unsigned int i=0; i<_userData.m_user_interaction.size();i++) {
		stringstream ss;
		ss	<< _userData.m_user_interaction[i].m_iInteractionId << "\t" 
			<< _userData.m_user_interaction[i].m_iScriptId << "\t"
			<< _userData.m_user_interaction[i].m_iSessionId << "\t"
			<< _userData.m_user_interaction[i].m_sSessionName << "\t"
			<< _userData.m_user_interaction[i].m_sSessionPartner << "\t"
			<< _userData.m_user_interaction[i].m_sPerformance << "\t"
			<< _userData.m_user_interaction[i].m_sEngagement << "\t"
			<< _userData.m_user_interaction[i].m_sTurnTake << "\t"
			<< _userData.m_user_interaction[i].m_sWP5Perf << "\t"
			<< _userData.m_user_interaction[i].m_sScriptTime;
		string ss_string = ss.str();
		_output_userHistory.push_back(ss_string);
	}
}

void SystemGUIController::loadScripts(){
	_script_list.clear();
	_script_list_library.load(_scriptManager_folder+"scripts.dream");
	for (unsigned int i = 0; i<_script_list_library.m_scripts.size(); i++) {
		_script_list.push_back(_script_list_library.m_scripts[i].m_name);		
	}
}

void SystemGUIController::loadScenarioButtons(void){
	_buttons_offScript_scenario.clear();
	_buttons_offScript_scenario_text.clear();
	_buttons_offScript_scenario_icons.clear();
	_buttons_offScript_scenario_color.clear();
	_buttons_offScript_scenario_action.clear();
	_buttons_offScript_scenario_value.clear();
	_buttons_offScript_scenario_value_old.clear();

	_button_offScript_scenario_library.load(_xml_folder+"buttons.dream","Scenario");	
	for (unsigned int i = 0; i<_button_offScript_scenario_library.m_buttons_library.size(); i++) {
		if (_button_offScript_scenario_library.m_buttons_library[i].m_id == _script) {
			for (unsigned int j=0; j< _button_offScript_scenario_library.m_buttons_library[i].m_buttons.size();j++){
				_buttons_offScript_scenario_text.push_back(_button_offScript_scenario_library.m_buttons_library[i].m_buttons[j].m_label);
				_buttons_offScript_scenario_icons.push_back(_iconUrl + _button_offScript_scenario_library.m_buttons_library[i].m_buttons[j].m_icon);
				_buttons_offScript_scenario_color.push_back(_button_offScript_scenario_library.m_buttons_library[i].m_buttons[j].m_color);
				_buttons_offScript_scenario_action.push_back(_button_offScript_scenario_library.m_buttons_library[i].m_buttons[j].m_action);
			}			
		}
	}
}

void SystemGUIController::loadFeedbackButtons(void){
	_buttons_offScript_feedback.clear();
	_buttons_offScript_feedback_text.clear();
	_buttons_offScript_feedback_icons.clear();
	_buttons_offScript_feedback_color.clear();
	_buttons_offScript_feedback_action.clear();
	_buttons_offScript_feedback_value.clear();
	_buttons_offScript_feedback_value_old.clear();

	_button_offScript_feedback_library.load(_xml_folder+"buttons.dream","Feedback");	
	for (unsigned int i = 0; i<_button_offScript_feedback_library.m_buttons_library.size(); i++) {
		if (_button_offScript_feedback_library.m_buttons_library[i].m_id == _script) {
			for (unsigned int j=0; j< _button_offScript_feedback_library.m_buttons_library[i].m_buttons.size();j++){
				_buttons_offScript_feedback_text.push_back(_button_offScript_feedback_library.m_buttons_library[i].m_buttons[j].m_label);
				_buttons_offScript_feedback_icons.push_back(_iconUrl + _button_offScript_feedback_library.m_buttons_library[i].m_buttons[j].m_icon);
				_buttons_offScript_feedback_color.push_back(_button_offScript_feedback_library.m_buttons_library[i].m_buttons[j].m_color);
				_buttons_offScript_feedback_action.push_back(_button_offScript_feedback_library.m_buttons_library[i].m_buttons[j].m_action);
			}			
		}
	}
}

void SystemGUIController::loadScriptSteps(string script_value) {
	// Load steps from library
	_list_scriptStepsBuffer = new Fl_Text_Buffer();
	_currentScriptStepIDs.clear();
	_currentScriptSteps.clear();
	_currentScriptSteps_raw.clear();
	for (unsigned int i=0; i<_script_list_library.m_scripts.size();i ++) {
		if (_script_list_library.m_scripts[i].m_name == script_value) {
			for (unsigned int j=0; j<_script_list_library.m_scripts[i].m_steps.size(); j++) {
				_currentScriptSteps_raw.push_back(_script_list_library.m_scripts[i].m_steps[j]); // read raw
				// Translate to readable text
				// slit string
				vector<string> sSplitStep = split(_currentScriptSteps_raw[j], ',');			// split into vector of strings (should be 2+ elements for a step)				
				int scriptMessage = atoi(sSplitStep[1].c_str());
				_currentScriptStepIDs.push_back(atoi(sSplitStep[0].c_str()));
				string scriptText =  sSplitStep[0]; // ID
				// convert to text
				switch (scriptMessage){
				case _SCRIPT_ID_: break; 										// expects 1 argument int script Id
				case _SCRIPT_CHECK_GAZE_:										// expects 0 argument
					scriptText += ". Check mutual gaze";	break;	
				case _SCRIPT_SAY_:												// expects 1 argument string
					scriptText += ". Say: \"" + sSplitStep[2] + "\"";	break;
				case _SCRIPT_SAY_IF_GOOD_PERF_:												// expects 1 argument string
					scriptText += ". Say if good feedback: \"" + sSplitStep[2] + "\"";	break;
				case _SCRIPT_MOVE_:												// expects 1 argument string
					scriptText += ". Move: " + sSplitStep[2];	break;	
				case _SCRIPT_WAIT_:       										// expects 1 argument int milliseconds
					scriptText += ". Wait: " + sSplitStep[2] + " millisecond(s)";	break;	
				case _SCRIPT_WAIT_FOR_PERF_:									// expects 3 arguments 3 int id, parameter and time
					scriptText += ". Wait for performance - "+ waitingArgument(sSplitStep);	break;
				case _SCRIPT_GRAB_OBJECT_:										// expects 0 argument
					scriptText += ". Grab object";	break;	
				case _SCRIPT_DROP_OBJECT_:										// expects 0 argument
					scriptText += ". Drop object";	break;	
				case _SCRIPT_FEEDBACK_IMITATION_OBJECT_:						// expects 0 argument
					scriptText += ". Feedback imitation object";
					if (_scriptType == "Diagnosis") scriptText += " DISABLED";  break;
				case _SCRIPT_LAST_FEEDBACK_IMITATION_OBJECT_:					// expects 0 argument
					scriptText += ". Last feedback imitation object";
					if (_scriptType == "Diagnosis") scriptText += " DISABLED";  break;
				case _SCRIPT_FEEDBACK_IMITATION_MOVE_:							// expects 0 argument
					scriptText += ". Feedback imitation move";
					if (_scriptType == "Diagnosis") scriptText += " DISABLED";  break;
				case _SCRIPT_LAST_FEEDBACK_IMITATION_MOVE_:						// expects 0 argument
					scriptText += ". Last feedback imitation move";
					if (_scriptType == "Diagnosis") scriptText += " DISABLED";  break;
				case _SCRIPT_FEEDBACK_TT_WAITING_:								// expects 0 argument
					scriptText += ". Feedback turn taking waiting";
					if (_scriptType == "Diagnosis") scriptText += " DISABLED";  break;
				case _SCRIPT_LAST_FEEDBACK_TT_WAITING_:								// expects 0 argument
					scriptText += ". Last Feedback turn taking waiting";
					if (_scriptType == "Diagnosis") scriptText += " DISABLED";  break;
				case _SCRIPT_FEEDBACK_TT_SORTING_:								// expects 0 argument
					scriptText += ". Feedback turn taking sorting";
					if (_scriptType == "Diagnosis") scriptText += " DISABLED";  break;
				case _SCRIPT_LAST_FEEDBACK_TT_SORTING_:								// expects 0 argument
					scriptText += ". Last Feedback turn taking sorting";
					if (_scriptType == "Diagnosis") scriptText += " DISABLED";  break;
				case _SCRIPT_FEEDBACK_TT_CHOOSING_:								// expects 0 argument
					scriptText += ". Feedback turn taking choosing";
					if (_scriptType == "Diagnosis") scriptText += " DISABLED";  break;
				case _SCRIPT_LAST_FEEDBACK_TT_CHOOSING_:								// expects 0 argument
					scriptText += ". Last Feedback turn taking choosing";
					if (_scriptType == "Diagnosis") scriptText += " DISABLED";  break;
				case _SCRIPT_FEEDBACK_TT_PATTERN_:								// expects 0 argument
					scriptText += ". Feedback turn taking pattern";
					if (_scriptType == "Diagnosis") scriptText += " DISABLED";  break;
				case _SCRIPT_LAST_FEEDBACK_TT_PATTERN_:								// expects 0 argument
					scriptText += ". Last Feedback turn taking pattern";
					if (_scriptType == "Diagnosis") scriptText += " DISABLED";  break;
				case _SCRIPT_FEEDBACK_JA_:										// expects 0 argument
					scriptText += ". Feedback joint attention";
					if (_scriptType == "Diagnosis") scriptText += " DISABLED";  break;
				case _SCRIPT_LAST_FEEDBACK_JA_:										// expects 0 argument
					scriptText += ". Last Feedback joint attention";
					if (_scriptType == "Diagnosis") scriptText += " DISABLED";  break;
				case _SCRIPT_SANDTRAY_DISPLAY_WHITE_:							// expects 0 argument
					scriptText += ". Sandtray display white";	break;	
				case _SCRIPT_SANDTRAY_LOAD_LIBRARY_:							// expects 1 argument int
					scriptText += ". Sandtray load library: "+ sSplitStep[2];	break;	
				case _SCRIPT_SANDTRAY_SET_IMAGE_ONE_:							// expects 0 argument
					scriptText += ". Sandtray set image one";	break;	
				case _SCRIPT_SANDTRAY_SET_IMAGE_ALL_:							// expects 0 argument
					scriptText += ". Sandtray set image all";	break;	
				case _SCRIPT_SANDTRAY_ENABLE_FEEDBACK_:							// expects 0 argument
					if (_scriptType == "Diagnosis")
						scriptText += ". Sandtray disable feedback";	
					else
						scriptText += ". Sandtray enable feedback";	break;
				case _SCRIPT_SANDTRAY_DISABLE_FEEDBACK_:						// expects 0 argument
					scriptText += ". Sandtray disable feedback";	break;	
				case _SCRIPT_SANDTRAY_ENABLE_TOUCH_:							// expects 0 argument
					scriptText += ". Sandtray enable touch";	break;	
				case _SCRIPT_SANDTRAY_DISABLE_TOUCH_:							// expects 0 argument
					scriptText += ". Sandtray disable touch";	break;	
				case _SCRIPT_ROBOT_GOOD_MOVE_:									// expects 0 argument
					scriptText += ". Robot good move";	break;	
				case _SCRIPT_POINT_IMAGE_:										// expects 1 argument int id
					scriptText += ". Point image: " + sSplitStep[2];	break;	
				case _SCRIPT_LOOK_POINT_SAY_:									// expects 1 argument int id (0 left/1 right)
					if (sSplitStep[2] == "0") scriptText += ". Point say: Left";
					if (sSplitStep[2] == "1") scriptText += ". Point say: Right";
					break;
				case _SCRIPT_LOOK_POINT_:										// expects 1 argument int id (0 left/1 right)
					if (sSplitStep[2] == "0") scriptText += ". Look point: Left";
					if (sSplitStep[2] == "1") scriptText += ". Look point: Right";
					break;	
				case _SCRIPT_LOOK_:												// expects 1 argument int id (0 left/1 right)
					if (sSplitStep[2] == "0") scriptText += ". Look: Left";
					if (sSplitStep[2] == "1") scriptText += ". Look: Right";
					if (sSplitStep[2] == "2") scriptText += ". Look: Straight";
					break;	
				case _SCRIPT_SANDTRAY_NEW_GAME_:								// expects 0 argument
					scriptText += ". Sandtray new game";	break;	
				case _SCRIPT_ENABLE_ROBOT_:
					scriptText += ". Enable robot";	break;
				case _SCRIPT_DISABLE_ROBOT_:
					scriptText += ". Disable robot";	break;
				case _SCRIPT_STAND_:
					scriptText += ". Stand";	break;
				case _SCRIPT_FINISHED_:											// expects nothing, we are done
					//scriptText += ". Script finished";	break;	
					scriptText = "";	break;	
				case _SCRIPT_TYPE_:	break;										// expects 1 argument int script type id						
				case _SCRIPT_CORRUPTED_ : break;								// expects nothing, we are ****ed						
				}
				//string temp_step  = _script_list_library.m_scripts[i].m_steps[j] + "\r\n"; // add new line
				if (scriptText!=""){
					_currentScriptSteps.push_back(scriptText);
					string temp_step  = scriptText + "\r\n"; // add new line
					_list_scriptStepsBuffer->append(temp_step.c_str());
					//cout << scriptText << endl;
				}
			}
		}
	}
	cout << "Script loaded!" << endl;
}

void SystemGUIController::update_list_scriptSteps(int step){
	if ((step >=0) &&(step<= (int)_currentScriptSteps.size())){
		_list_scriptStepsBuffer = new Fl_Text_Buffer();
		for (unsigned int i=step; i<_currentScriptSteps.size(); i++) {
			_list_scriptSteps->buffer(_list_scriptStepsBuffer);
			string temp_step  = _currentScriptSteps[i] + "\r\n";
			_list_scriptStepsBuffer->append(temp_step.c_str());
		}		
	}
	cout << "Update list script steps:" << step << endl;
}

void SystemGUIController::loadFixedButtons(void){
	_buttons_offScript_fixed.clear();
	_buttons_offScript_fixed_text.clear();
	_buttons_offScript_fixed_icons.clear();
	_buttons_offScript_fixed_color.clear();
	_buttons_offScript_fixed_action.clear();
	_buttons_offScript_fixed_value.clear();
	_buttons_offScript_fixed_value_old.clear();

	_button_offScript_fixed_library.load(_xml_folder+"buttons.dream","Fixed");	
	for (unsigned int i = 0; i<_button_offScript_fixed_library.m_buttons_library.size(); i++) {
		if (_button_offScript_fixed_library.m_buttons_library[i].m_id == _script) {
			for (unsigned int j=0; j< _button_offScript_fixed_library.m_buttons_library[i].m_buttons.size();j++){
				_buttons_offScript_fixed_text.push_back(_button_offScript_fixed_library.m_buttons_library[i].m_buttons[j].m_label);
				_buttons_offScript_fixed_icons.push_back(_iconUrl + _button_offScript_fixed_library.m_buttons_library[i].m_buttons[j].m_icon);
				_buttons_offScript_fixed_color.push_back(_button_offScript_fixed_library.m_buttons_library[i].m_buttons[j].m_color);				
				_buttons_offScript_fixed_action.push_back(_button_offScript_fixed_library.m_buttons_library[i].m_buttons[j].m_action);				
			}			
		}
	}
}

void SystemGUIController::loadAlwaysOnButtons(void){
	_buttons_offScript_alwaysOn.clear();
	_buttons_offScript_alwaysOn_text.clear();
	_buttons_offScript_alwaysOn_icons.clear();
	_buttons_offScript_alwaysOn_color.clear();
	_buttons_offScript_alwaysOn_action.clear();
	_buttons_offScript_alwaysOn_value.clear();
	_buttons_offScript_alwaysOn_value_old.clear();

	_button_offScript_alwaysOn_library.load(_xml_folder+"buttons.dream","AlwaysOn");	
	for (unsigned int i = 0; i<_button_offScript_alwaysOn_library.m_buttons_library.size(); i++) {
		if (_button_offScript_alwaysOn_library.m_buttons_library[i].m_id == _script) {
			for (unsigned int j=0; j< _button_offScript_alwaysOn_library.m_buttons_library[i].m_buttons.size();j++){
				_buttons_offScript_alwaysOn_text.push_back(_button_offScript_alwaysOn_library.m_buttons_library[i].m_buttons[j].m_label);
				_buttons_offScript_alwaysOn_icons.push_back(_iconUrl + _button_offScript_alwaysOn_library.m_buttons_library[i].m_buttons[j].m_icon);
				_buttons_offScript_alwaysOn_color.push_back(_button_offScript_alwaysOn_library.m_buttons_library[i].m_buttons[j].m_color);				
				_buttons_offScript_alwaysOn_action.push_back(_button_offScript_alwaysOn_library.m_buttons_library[i].m_buttons[j].m_action);				
			}			
		}
	}
}

void SystemGUIController::loadBetweenScriptsButtons(void){
	_buttons_betweenScripts.clear();
	_buttons_betweenScripts_text.clear();
	_buttons_betweenScripts_icons.clear();
	_buttons_betweenScripts_color.clear();
	_buttons_betweenScripts_value.clear();
	_buttons_betweenScripts_value_old.clear();
	_button_betweenScripts_library.load(_xml_folder+"buttons.dream","BetweenScripts");	
	for (unsigned int i = 0; i<_button_betweenScripts_library.m_buttons_library.size(); i++) {
		if (_button_betweenScripts_library.m_buttons_library[i].m_id == _script) {
			for (unsigned int j=0; j< _button_betweenScripts_library.m_buttons_library[i].m_buttons.size();j++){
				_buttons_betweenScripts_text.push_back(_button_betweenScripts_library.m_buttons_library[i].m_buttons[j].m_label);
				_buttons_betweenScripts_icons.push_back(_iconUrl + _button_betweenScripts_library.m_buttons_library[i].m_buttons[j].m_icon);
				_buttons_betweenScripts_color.push_back(_button_betweenScripts_library.m_buttons_library[i].m_buttons[j].m_color);				
				_buttons_betweenScripts_action.push_back(_button_betweenScripts_library.m_buttons_library[i].m_buttons[j].m_action);
			}			
		}
	}

	_buttons_enableRobot.clear();
	_buttons_enableRobot_text.clear();
	_buttons_enableRobot_icons.clear();
	_buttons_enableRobot_color.clear();
	_buttons_enableRobot_value.clear();
	_buttons_enableRobot_value_old.clear();
	_button_enableRobot_library.load(_xml_folder+"buttons.dream","EnableRobot");	
	for (unsigned int i = 0; i<_button_enableRobot_library.m_buttons_library.size(); i++) {
		if (_button_enableRobot_library.m_buttons_library[i].m_id == _script) {
			for (unsigned int j=0; j< _button_enableRobot_library.m_buttons_library[i].m_buttons.size();j++){
				_buttons_enableRobot_text.push_back(_button_enableRobot_library.m_buttons_library[i].m_buttons[j].m_label);
				_buttons_enableRobot_icons.push_back(_iconUrl + _button_enableRobot_library.m_buttons_library[i].m_buttons[j].m_icon);
				_buttons_enableRobot_color.push_back(_button_enableRobot_library.m_buttons_library[i].m_buttons[j].m_color);				
				_buttons_enableRobot_action.push_back(_button_enableRobot_library.m_buttons_library[i].m_buttons[j].m_action);
			}			
		}
	}
}

void SystemGUIController::showOffscriptButtons(bool value){
	// On/off all buttons except the AlwaysOn ones
	if (value) {
		for (unsigned int i=0; i< _buttons_offScript_fixed.size(); i++)
			_buttons_offScript_fixed[i]->activate();
		for (unsigned int i=0; i< _buttons_offScript_feedback.size(); i++)
			_buttons_offScript_feedback[i]->activate();
		for (unsigned int i=0; i< _buttons_offScript_scenario.size(); i++)
			_buttons_offScript_scenario[i]->activate();
	}
	else {
		for (unsigned int i=0; i< _buttons_offScript_fixed.size(); i++)
			_buttons_offScript_fixed[i]->deactivate();
		for (unsigned int i=0; i< _buttons_offScript_feedback.size(); i++)
			_buttons_offScript_feedback[i]->deactivate();
		for (unsigned int i=0; i< _buttons_offScript_scenario.size(); i++)
			_buttons_offScript_scenario[i]->deactivate();
	}

}

void SystemGUIController::GUIinit(){
	/* Variables for GUI elements' positions */
	int border, row_height;
	int radio_button_width, radio_button_height;
	int check_button_width, check_button_height;

	int box_loadUser_x, box_loadUser_y,box_loadUser_w,box_loadUser_h;
	int choice_userName_x,choice_userName_y, choice_userName_w, choice_userName_h;
	int textInput_sessionName_x,textInput_sessionName_y,textInput_sessionName_w,textInput_sessionName_h;
	int choice_sessionPartner_x,choice_sessionPartner_y,choice_sessionPartner_w,choice_sessionPartner_h;
	int choice_scriptType_x,choice_scriptType_y,choice_scriptType_w,choice_scriptType_h;
	int button_loadUser_x, button_loadUser_y, button_loadUser_w, button_loadUser_h;
	string icon; // is used to store a temporary icon
	Fl_PNG_Image *icon_image;
	int box_currentPerformance_x, box_currentPerformance_y, box_currentPerformance_w, box_currentPerformance_h;
	int box_dreamLogo_x, box_dreamLogo_y, box_dreamLogo_w, box_dreamLogo_h;
	
	int tabs_supervisedControl_x, tabs_supervisedControl_y,tabs_supervisedControl_w,tabs_supervisedControl_h;
	int tab_supervisedControl_x, tab_supervisedControl_y,tab_supervisedControl_w,tab_supervisedControl_h;

	int choice_script_x,choice_script_y, choice_script_w, choice_script_h;
	int button_loadScript_x, button_loadScript_y, button_loadScript_w, button_loadScript_h;
	int box_connectionMessage_x, box_connectionMessage_y, box_connectionMessage_w, box_connectionMessage_h;
	int list_scriptSteps_x, list_scriptSteps_y, list_scriptSteps_w, list_scriptSteps_h;
	int slider_Timer_x, slider_Timer_y, slider_Timer_w, slider_Timer_h;
	int button_updateSliderTimer_x, button_updateSliderTimer_y, button_updateSliderTimer_w, button_updateSliderTimer_h;
	int box_onScript_x, box_onScript_y, box_onScript_w, box_onScript_h;
	int box_offScript_x, box_offScript_y, box_offScript_w, box_offScript_h;
	int progress_Timer_x, progress_Timer_y, progress_Timer_w, progress_Timer_h;
	int box_actionTime_x, box_actionTime_y, box_actionTime_w, box_actionTime_h;
	int button_doItNow_x, button_doItNow_y, button_doItNow_w, button_doItNow_h;
	int button_iWillChoose_x, button_iWillChoose_y, button_iWillChoose_w, button_iWillChoose_h;
	
	int buttons_offScript_w, buttons_offScript_h;
	int buttons_offScript_spacing, buttons_offScript_col;
	int buttons_offScript_fixed_row, buttons_offScript_alwaysOn_row, buttons_offScript_feedback_row, buttons_offScript_scenario_row;

	int buttons_betweenScripts_w, buttons_betweenScripts_h;
	int buttons_betweenScripts_spacing, buttons_betweenScripts_col;
	int buttons_betweenScripts_row, buttons_enableRobot_row;

	int textInput_say_x, textInput_say_y, textInput_say_width, textInput_say_height;
	int button_say_x, button_say_y, button_say_w, button_say_h;

	int checkButton_enableRAS_x,checkButton_enableRAS_y,checkButton_enableRAS_w,checkButton_enableRAS_h;

	int box_childInfo_x, box_childInfo_y, box_childInfo_w, box_childInfo_h;
	int output_userData_x,output_userData_y,output_userData_w,output_userData_h;
	int browser_childPerformance_x,browser_childPerformance_y,browser_childPerformance_w,browser_childPerformance_h;

	int list_developerInfo_x, list_developerInfo_y, list_developerInfo_w, list_developerInfo_h;
	int button_exportLogfile_x, button_exportLogfile_y, button_exportLogfile_w, button_exportLogfile_h;

	/* Initialize variables for GUI widgets' positions */
	loadScenarioButtons();

	// Widgets
	border = 5;
	row_height = 25;
	radio_button_width = row_height;
	radio_button_height = radio_button_width;
	check_button_width  = row_height;
	check_button_height = check_button_width;
	// LOAD USER
	box_loadUser_x = border;
	box_loadUser_y = border;
	box_loadUser_w = GUI_WIDTH - 2*border;
	box_loadUser_h = 2*row_height + 2*border;

	choice_userName_x = box_loadUser_x + 40;
	choice_userName_w = 170;
	choice_userName_h = row_height;
	choice_userName_y = box_loadUser_y + box_loadUser_h - border - choice_userName_h;

	textInput_sessionName_x = choice_userName_x + choice_userName_w + 90;
	textInput_sessionName_y = choice_userName_y;
	textInput_sessionName_w = 200;
	textInput_sessionName_h = row_height;

	choice_sessionPartner_x = textInput_sessionName_x + textInput_sessionName_w + 100;
	choice_sessionPartner_y = choice_userName_y;
	choice_sessionPartner_w = 100;
	choice_sessionPartner_h = row_height;

	choice_scriptType_x = choice_sessionPartner_x + choice_sessionPartner_w + 70;
	choice_scriptType_y = choice_userName_y;
	choice_scriptType_w = 110;
	choice_scriptType_h = row_height;

	button_loadUser_x = choice_scriptType_x + choice_scriptType_w + border;
	button_loadUser_y = choice_userName_y;
	button_loadUser_w = 170;
	button_loadUser_h = row_height;

	box_currentPerformance_x = button_loadUser_x;// + button_loadUser_w + border;
	box_currentPerformance_w = 200;
	box_currentPerformance_h = row_height;
	box_currentPerformance_y = button_loadUser_y + 2*row_height + border;

	box_dreamLogo_w = _dream_logoImage->w();
	box_dreamLogo_h = _dream_logoImage->h();
	box_dreamLogo_x = box_loadUser_x + box_loadUser_w - border - box_dreamLogo_w;
	box_dreamLogo_y = box_loadUser_y + button_loadUser_h/2 + border;;

	// TABS SUPERVISED CONTROL
	tabs_supervisedControl_x = box_loadUser_x;
	tabs_supervisedControl_y = box_loadUser_y + box_loadUser_h + border;
	tabs_supervisedControl_w = box_loadUser_w;
	tabs_supervisedControl_h = GUI_HEIGHT - box_loadUser_h- 3*border;
	
	tab_supervisedControl_x = tabs_supervisedControl_x + border;
	tab_supervisedControl_y = tabs_supervisedControl_y + row_height;
	tab_supervisedControl_w = tabs_supervisedControl_w - 2*border;
	tab_supervisedControl_h = tabs_supervisedControl_h - row_height -2*border;

	// Load Script
	choice_script_x = tab_supervisedControl_x + 80;
	choice_script_y = tab_supervisedControl_y + row_height;
	choice_script_w = 300;
	choice_script_h = row_height;	

	button_loadScript_x = choice_script_x + choice_script_w + border;
	button_loadScript_y = choice_script_y;
	button_loadScript_w = 100;
	button_loadScript_h = row_height;

	box_connectionMessage_x = button_loadScript_x + button_loadScript_w + border;
	box_connectionMessage_w = 200;
	box_connectionMessage_h = row_height;
	box_connectionMessage_y = (int) (choice_script_y + 0.875*row_height);

	list_scriptSteps_x = tab_supervisedControl_x + border;
	list_scriptSteps_y = choice_script_y + choice_script_h + border;
	list_scriptSteps_w = choice_script_x + choice_script_w - list_scriptSteps_x;
	//list_scriptSteps_h
	
	slider_Timer_x = list_scriptSteps_x;
	//slider_Timer_y
	slider_Timer_w = list_scriptSteps_w;
	slider_Timer_h = (int) (1.5*row_height);

	button_updateSliderTimer_x = list_scriptSteps_x;
	//button_updateSliderTimer_y
	button_updateSliderTimer_w = list_scriptSteps_w;
	button_updateSliderTimer_h = row_height;

	list_scriptSteps_h = tab_supervisedControl_y + tab_supervisedControl_h - list_scriptSteps_y - slider_Timer_h - button_updateSliderTimer_h - 2*border;
	slider_Timer_y = list_scriptSteps_y + list_scriptSteps_h + border;
	button_updateSliderTimer_y = slider_Timer_y + slider_Timer_h + border;

	box_onScript_x = list_scriptSteps_x + list_scriptSteps_w + border;
	box_onScript_y = list_scriptSteps_y;
	box_onScript_w = tab_supervisedControl_x + tab_supervisedControl_w - list_scriptSteps_x - list_scriptSteps_w - 2*border;

	progress_Timer_x = box_onScript_x + border;
	progress_Timer_y = box_onScript_y + 2*row_height;
	progress_Timer_w = 500;
	progress_Timer_h = (int) (1.5*row_height);

	box_actionTime_x = progress_Timer_x;
	box_actionTime_y = progress_Timer_y + progress_Timer_h + border;
	box_actionTime_w = progress_Timer_w;
	box_actionTime_h = row_height;
		
	box_onScript_h = box_actionTime_y + box_actionTime_h + border - box_onScript_y; // change later
	
	button_doItNow_w = 150;
	button_doItNow_h = (int) ((box_onScript_h - 3*border)/2);
	button_doItNow_x = box_onScript_x + box_onScript_w - border - button_doItNow_w;
	button_doItNow_y = box_onScript_y + border;

	button_iWillChoose_x= button_doItNow_x;
	button_iWillChoose_y = button_doItNow_y + button_doItNow_h + border;
	button_iWillChoose_w = button_doItNow_w;
	button_iWillChoose_h = button_doItNow_h;
	
	buttons_offScript_w = 140;
	buttons_offScript_h = (int)(2.7*row_height);
	
	box_offScript_x = box_onScript_x;
	box_offScript_y = box_onScript_y + box_onScript_h + border;
	box_offScript_w = box_onScript_w;
	box_offScript_h = tab_supervisedControl_y + tab_supervisedControl_h - box_offScript_y;

	buttons_offScript_col = (int) box_offScript_w/buttons_offScript_w;
	//buttons_offScript_spacing = (box_offScript_w-buttons_offScript_col*buttons_offScript_w)/(buttons_offScript_col+1);
	
	double temp_row = (double)_buttons_offScript_fixed_text.size()/buttons_offScript_col;
	buttons_offScript_fixed_row = (int) ceil(temp_row);
	temp_row = (double)_buttons_offScript_alwaysOn_text.size()/buttons_offScript_col;
	buttons_offScript_alwaysOn_row = (int) ceil(temp_row);
	temp_row = (double)_buttons_offScript_feedback_text.size()/buttons_offScript_col;
	buttons_offScript_feedback_row = (int) ceil(temp_row);
	temp_row = (double)_buttons_offScript_scenario_text.size()/buttons_offScript_col;
	buttons_offScript_scenario_row = (int) ceil(temp_row);
	
	// Adjust the spacing when scollbar appears
	int box_offScript_w_adjust = 0;
	if (buttons_offScript_fixed_row + buttons_offScript_feedback_row + buttons_offScript_scenario_row > 5 )
		box_offScript_w_adjust = 15;
	buttons_offScript_spacing = (box_offScript_w - box_offScript_w_adjust -buttons_offScript_col*buttons_offScript_w)/(buttons_offScript_col+1);

	buttons_betweenScripts_w = 200;
	buttons_betweenScripts_h = 3*row_height;

	buttons_betweenScripts_col = (int) tab_supervisedControl_w/buttons_betweenScripts_w;
	buttons_betweenScripts_spacing = (tab_supervisedControl_w-buttons_betweenScripts_col*buttons_betweenScripts_w)/(buttons_betweenScripts_col+1);
	//cout << tab_supervisedControl_w << buttons_betweenScripts_spacing <<endl;
	//buttons_betweenScripts_row = (int) ( _buttons_betweenScripts_text.size()/buttons_betweenScripts_col +1);
	temp_row = (double)_buttons_betweenScripts_text.size()/buttons_betweenScripts_col;
	buttons_betweenScripts_row = (int) ceil(temp_row);
	temp_row = (double)_buttons_enableRobot_text.size()/buttons_betweenScripts_col;
	buttons_enableRobot_row = (int) ceil(temp_row);

	textInput_say_x = tab_supervisedControl_x + buttons_betweenScripts_spacing;
	//textInput_say_y = tab_supervisedControl_y + (buttons_betweenScripts_row + buttons_enableRobot_row)*(buttons_betweenScripts_h + buttons_betweenScripts_spacing) + buttons_betweenScripts_spacing;
	textInput_say_width = 2*buttons_betweenScripts_w + buttons_betweenScripts_spacing;
	textInput_say_height = row_height;

	button_say_x = textInput_say_x + textInput_say_width + buttons_betweenScripts_spacing;
	button_say_y = tab_supervisedControl_y + (buttons_betweenScripts_row + buttons_enableRobot_row)*(buttons_betweenScripts_h + buttons_betweenScripts_spacing) + buttons_betweenScripts_spacing;
	button_say_w = buttons_betweenScripts_w;
	button_say_h = buttons_betweenScripts_h;
	textInput_say_y = button_say_y + button_say_h - textInput_say_height;

	checkButton_enableRAS_x = textInput_say_x;
	checkButton_enableRAS_y  = textInput_say_y + textInput_say_height + buttons_betweenScripts_spacing;
	checkButton_enableRAS_w  = textInput_say_width;
	checkButton_enableRAS_h = row_height;


	box_childInfo_x = tab_supervisedControl_x + border;
	box_childInfo_y = (int) (tab_supervisedControl_y + 1.5*border);
	box_childInfo_w = 2*(tab_supervisedControl_w - 3*border)/5;
	box_childInfo_h = tab_supervisedControl_h - 2*border;

	//browser_childPerformance_x = tab_supervisedControl_x + border;
	//browser_childPerformance_y = (int) (tab_supervisedControl_y + 1.5*border);
	browser_childPerformance_x = box_childInfo_x + box_childInfo_w + border;
	browser_childPerformance_y = box_childInfo_y;
	browser_childPerformance_w = 3*(tab_supervisedControl_w - 3*border)/5;
	browser_childPerformance_h = box_childInfo_h;

	output_userData_w = 270;
	output_userData_h = row_height;
	output_userData_y = box_childInfo_y + border;
	output_userData_x = box_childInfo_x + box_childInfo_w - output_userData_w - border;
	
	list_developerInfo_x = tab_supervisedControl_x + border;
	list_developerInfo_y = (int) (tab_supervisedControl_y + 1.5*border);
	list_developerInfo_w = (int) (5*(tab_supervisedControl_w - 3*border)/6);
	list_developerInfo_h = tab_supervisedControl_h - 2*border; 

	button_exportLogfile_x = list_developerInfo_x + list_developerInfo_w + border;
	button_exportLogfile_y = list_developerInfo_y;
	button_exportLogfile_w = buttons_betweenScripts_w;
	button_exportLogfile_h = buttons_betweenScripts_h;
	
	/* Start GUI */
	_start_systemArchitecture_GUI = new Fl_Group(0,MENUBAR_HEIGHT,GUI_WIDTH,GUI_HEIGHT-MENUBAR_HEIGHT,"");
	_start_systemArchitecture_GUI->begin();

	// Load User
	_box_loadUser = new Fl_Box(box_loadUser_x, box_loadUser_y, box_loadUser_w, box_loadUser_h,"" );
	_box_loadUser->align(FL_ALIGN_TOP | FL_ALIGN_LEFT | FL_ALIGN_INSIDE);
	_box_loadUser->labelsize(14);
	_box_loadUser->labelfont(FL_HELVETICA_BOLD);
	_box_loadUser->box(FL_NO_BOX);
	//_box_loadUser->box(FL_DOWN_BOX);

	_choice_userName = new Fl_Choice(choice_userName_x,choice_userName_y,choice_userName_w,choice_userName_h, "Name"); 
	// Add names from userName list
	//_choice_userName->add("Please select a user", 0, NULL);
	for (unsigned int i = 0 ; i < _userName_list.size(); i++)
		_choice_userName->add(_userName_list[i].c_str(), 0, userName_change_cb, &_userName);
	_choice_userName->value(0); 
	_choice_userName->labelsize(12); 
	_choice_userName->textsize(12);
	_choice_userName->labelfont(FL_HELVETICA_BOLD);	
	_choice_userName->down_box(FL_BORDER_BOX);
	_choice_userName->value(_userName_choice_value);

	_textInput_sessionName = new Fl_Input(textInput_sessionName_x, textInput_sessionName_y, textInput_sessionName_w, textInput_sessionName_h,"Session Name");
	_textInput_sessionName->textsize(12);
	_textInput_sessionName->callback(get_input_cb,&_textInput_sessionName_value);  
	_textInput_sessionName->labelsize(12);
	_textInput_sessionName->labelfont(FL_HELVETICA_BOLD); 
	_textInput_sessionName->align(FL_ALIGN_LEFT);
	//_textInput_sessionName->value("default");
	get_input_cb(_textInput_sessionName,_textInput_sessionName_value);
	_textInput_sessionName->when(FL_WHEN_RELEASE);  // callback is called: when the enter key is hit

	_choice_sessionPartner = new Fl_Choice(choice_sessionPartner_x,choice_sessionPartner_y,choice_sessionPartner_w,choice_sessionPartner_h, "Session Partner"); 
	// Add names from sessionPartner list
	_choice_sessionPartner->add("Robot", 0, sessionPartner_change_cb, &_sessionPartner);
	_choice_sessionPartner->add("Therapist", 0, sessionPartner_change_cb, &_sessionPartner);
	_choice_sessionPartner->value(0); 
	_choice_sessionPartner->labelsize(12); 
	_choice_sessionPartner->textsize(12);
	_choice_sessionPartner->labelfont(FL_HELVETICA_BOLD);	
	_choice_sessionPartner->down_box(FL_BORDER_BOX);
	_choice_sessionPartner->value(_sessionPartner_choice_value);

	_choice_scriptType = new Fl_Choice(choice_scriptType_x,choice_scriptType_y,choice_scriptType_w,choice_scriptType_h, "Script Type"); 
	// Add names from scriptType list
	_choice_scriptType->add("Intervention", 0, scriptType_change_cb, &_scriptType);
	_choice_scriptType->add("Diagnosis", 0, scriptType_change_cb, &_scriptType);
	_choice_scriptType->value(0); 
	_choice_scriptType->labelsize(12); 
	_choice_scriptType->textsize(12);
	_choice_scriptType->labelfont(FL_HELVETICA_BOLD);	
	_choice_scriptType->down_box(FL_BORDER_BOX);
	_choice_scriptType->value(_scriptType_choice_value);
	
	//_button_loadUser = new Fl_Button(button_loadUser_x, button_loadUser_y, button_loadUser_w, button_loadUser_h,"Load User");
	_button_loadUser = new Fl_Button(button_loadUser_x, button_loadUser_y, button_loadUser_w, button_loadUser_h,"Create Session");
	_button_loadUser ->type(FL_NORMAL_BUTTON);
	_button_loadUser ->callback(button_cb, &_button_loadUser_value);
	_button_loadUser ->labelsize(12);
	_button_loadUser->labelfont(FL_HELVETICA_BOLD);  
	_button_loadUser->when(FL_WHEN_ENTER_KEY | FL_WHEN_CHANGED | FL_WHEN_RELEASE); 
	_button_loadUser->align(FL_ALIGN_CENTER);

	_box_currentPerformance = new Fl_Box(box_currentPerformance_x, box_currentPerformance_y, box_currentPerformance_w, box_currentPerformance_h,_currentPerformanceDisplay);
	_box_currentPerformance->labelsize(14);
	_box_currentPerformance->labelfont(FL_HELVETICA_BOLD);
	_box_currentPerformance->box(FL_NO_BOX);
	_box_currentPerformance->align(FL_ALIGN_TOP | FL_ALIGN_LEFT);
	
	_box_dreamLogo = new Fl_Button(box_dreamLogo_x, box_dreamLogo_y, box_dreamLogo_w, box_dreamLogo_h,"" );
	_box_dreamLogo->box(FL_NO_BOX);
	_box_dreamLogo->image(_dream_logoImage);
	_box_dreamLogo->callback(button_cb, &_box_dreamLogo_value);
	//_box_dreamLogo ->type(FL_NORMAL_BUTTON);
	_box_dreamLogo->when(FL_WHEN_ENTER_KEY | FL_WHEN_CHANGED | FL_WHEN_RELEASE); 

	_tabs_supervisedControl = new Fl_Tabs(tabs_supervisedControl_x, tabs_supervisedControl_y, tabs_supervisedControl_w, tabs_supervisedControl_h);
	{
		_tab_loadScript = new Fl_Group(tab_supervisedControl_x, tab_supervisedControl_y, tab_supervisedControl_w,tab_supervisedControl_h,"Load Script");
		_tab_loadScript->labelsize(13);
		_tab_loadScript->labelfont(FL_HELVETICA_BOLD);{
			_choice_script = new Fl_Choice(choice_script_x,choice_script_y,choice_script_w,choice_script_h, "Load script"); 
			// Add names from script list
			_choice_script->add("Please select a script", 0, NULL);
			for (unsigned int i = 0 ; i < _script_list.size(); i++)
				_choice_script->add(_script_list[i].c_str(), 0, script_change_cb, &_script);
			_choice_script->value(0); 
			_choice_script->labelsize(12); 
			_choice_script->textsize(12);
			_choice_script->labelfont(FL_HELVETICA_BOLD);	
			_choice_script->down_box(FL_BORDER_BOX);
			_choice_script->value(_script_choice_value);
			
			_button_loadScript = new Fl_Button(button_loadScript_x, button_loadScript_y, button_loadScript_w, button_loadScript_h,"Start script");
			_button_loadScript ->type(FL_NORMAL_BUTTON);
			_button_loadScript ->callback(button_cb, &_button_loadScript_value);
			_button_loadScript ->labelsize(12);
			_button_loadScript->labelfont(FL_HELVETICA_BOLD);  
			_button_loadScript->when(FL_WHEN_ENTER_KEY | FL_WHEN_CHANGED | FL_WHEN_RELEASE); 
			_button_loadScript->align(FL_ALIGN_IMAGE_NEXT_TO_TEXT);
			icon = _iconUrl + "play_icon.png";
			//icon_image = (Fl_PNG_Image*) resizeImage(icon,button_loadScript_h,button_loadScript_h);			
			resizeImage(icon,button_loadScript_h,button_loadScript_h,icon_image);			
			_button_loadScript->image(icon_image);

			_box_connectionMessage = new Fl_Box(box_connectionMessage_x, box_connectionMessage_y, box_connectionMessage_w, box_connectionMessage_h,_connectionMessage);
			_box_connectionMessage->labelsize(14);
			_box_connectionMessage->labelfont(FL_HELVETICA_BOLD);
			_box_connectionMessage->box(FL_NO_BOX);
			_box_connectionMessage->align(FL_ALIGN_TOP | FL_ALIGN_LEFT);

			//_list_scriptStepsBuffer = new Fl_Text_Buffer();
			_list_scriptSteps = new Fl_Text_Display(list_scriptSteps_x, list_scriptSteps_y, list_scriptSteps_w, list_scriptSteps_h);
			_list_scriptSteps->textsize(12);
			_list_scriptSteps->align(FL_ALIGN_TOP | FL_ALIGN_LEFT);
			_list_scriptSteps->buffer(_list_scriptStepsBuffer);

			_slider_Timer = new Fl_Value_Slider(slider_Timer_x, slider_Timer_y, slider_Timer_w, slider_Timer_h,"");
			_slider_Timer->type(FL_HOR_NICE_SLIDER);
			_slider_Timer->textsize(14);
			_slider_Timer->callback(valuator_cb, &_timer_value_temp);
			_slider_Timer->labelsize(12); 
			_slider_Timer->labelfont(FL_HELVETICA_BOLD);  
			_slider_Timer->when(FL_WHEN_ENTER_KEY | FL_WHEN_CHANGED | FL_WHEN_RELEASE); 
			_slider_Timer->align(FL_ALIGN_LEFT);
			_slider_Timer->minimum(3);
			_slider_Timer->maximum(10);
			_slider_Timer->step(1);
			_slider_Timer->value(_timer_value_temp); 
			_slider_Timer->box(FL_PLASTIC_DOWN_BOX);

			_button_updateSliderTimer = new Fl_Button(button_updateSliderTimer_x, button_updateSliderTimer_y, button_updateSliderTimer_w, button_updateSliderTimer_h,"Change countdown time");
			_button_updateSliderTimer ->type(FL_NORMAL_BUTTON);
			_button_updateSliderTimer ->callback(button_cb, &_button_updateSliderTimer_value);
			_button_updateSliderTimer ->labelsize(12);
			_button_updateSliderTimer->labelfont(FL_HELVETICA_BOLD);  
			_button_updateSliderTimer->when(FL_WHEN_ENTER_KEY | FL_WHEN_CHANGED | FL_WHEN_RELEASE); 
			_button_updateSliderTimer->align(FL_ALIGN_CENTER);

			//_box_onScript = new Fl_Box(box_onScript_x, box_onScript_y, box_onScript_w, box_onScript_h,"I'm going to:" );
			//_box_onScript = new Fl_Group(box_onScript_x, box_onScript_y, box_onScript_w, box_onScript_h,_suggestionLabel.c_str());
			_box_onScript = new Fl_Group(box_onScript_x, box_onScript_y, box_onScript_w, box_onScript_h,"");
			_box_onScript->align(FL_ALIGN_TOP | FL_ALIGN_LEFT | FL_ALIGN_INSIDE);
			_box_onScript->labelsize(16);
			_box_onScript->labelfont(FL_HELVETICA_BOLD);
			_box_onScript->box(FL_DOWN_BOX);{

				_progress_Timer = new Fl_Progress(progress_Timer_x, progress_Timer_y, progress_Timer_w, progress_Timer_h,_progress_Timer_label);
				//_progress_Timer = new Fl_Progress(progress_Timer_x, progress_Timer_y, progress_Timer_w, progress_Timer_h,_suggestionMessage.c_str());
				_progress_Timer->labelfont(FL_HELVETICA_BOLD);  
				_progress_Timer->align(FL_ALIGN_LEFT | FL_ALIGN_TOP);
				_progress_Timer->minimum(0);
				_progress_Timer->maximum(1);
				_progress_Timer->value((float)_progress_Timer_value); 
				_progress_Timer->color2(_progress_bar_color);
				_progress_Timer->box(FL_PLASTIC_DOWN_BOX);

				_box_actionTime = new Fl_Box(box_actionTime_x, box_actionTime_y, box_actionTime_w, box_actionTime_h,"");
				//_box_actionTime = new Fl_Box(box_actionTime_x, box_actionTime_y, box_actionTime_w, box_actionTime_h,_actionTimeDisplay.c_str());
				_box_actionTime->labelsize(14);
				_box_actionTime->labelfont(FL_HELVETICA_BOLD);
				_box_actionTime->box(FL_NO_BOX);

				_button_doItNow = new Fl_Button(button_doItNow_x, button_doItNow_y, button_doItNow_w, button_doItNow_h,"  Do it now");
				_button_doItNow ->type(FL_NORMAL_BUTTON);
				_button_doItNow ->callback(button_cb, &_button_doItNow_value);
				_button_doItNow ->labelsize(16);
				_button_doItNow->labelfont(FL_HELVETICA_BOLD);  
				_button_doItNow->when(FL_WHEN_ENTER_KEY | FL_WHEN_CHANGED | FL_WHEN_RELEASE); 
				_button_doItNow->align(FL_ALIGN_LEFT | FL_ALIGN_INSIDE |FL_ALIGN_IMAGE_NEXT_TO_TEXT);
				_button_doItNow->color(FL_GREEN);
				icon = _iconUrl + "playpause_icon.png";
				//icon_image = (Fl_PNG_Image*) resizeImage(icon,button_doItNow_h,button_doItNow_h);
				icon_image = new Fl_PNG_Image(icon.c_str());
				_button_doItNow->image(icon_image);

				_button_iWillChoose = new Fl_Button(button_iWillChoose_x, button_iWillChoose_y, button_iWillChoose_w, button_iWillChoose_h,"I will choose");
				_button_iWillChoose ->type(FL_NORMAL_BUTTON);
				_button_iWillChoose ->callback(button_cb, &_button_iWillChoose_value);
				_button_iWillChoose ->labelsize(16);
				_button_iWillChoose->labelfont(FL_HELVETICA_BOLD);  
				_button_iWillChoose->when(FL_WHEN_ENTER_KEY | FL_WHEN_CHANGED | FL_WHEN_RELEASE); 
				_button_iWillChoose->align(FL_ALIGN_LEFT | FL_ALIGN_INSIDE | FL_ALIGN_IMAGE_NEXT_TO_TEXT);
				_button_iWillChoose->color(FL_RED);
				icon = _iconUrl + "pause_icon.png";
				//icon_image = (Fl_PNG_Image*) resizeImage(icon,button_iWillChoose_h,button_iWillChoose_h);
				icon_image = new Fl_PNG_Image(icon.c_str());
				_button_iWillChoose->image(icon_image);
			}
			_box_onScript->end();


			_box_offScript = new Fl_Scroll(box_offScript_x, box_offScript_y, box_offScript_w, box_offScript_h,"" );
			_box_offScript->align(FL_ALIGN_TOP | FL_ALIGN_LEFT | FL_ALIGN_INSIDE);
			_box_offScript->labelsize(14);
			_box_offScript->labelfont(FL_HELVETICA_BOLD);
			_box_offScript->box(FL_DOWN_BOX);{

				_buttons_offScript_fixed.resize(_buttons_offScript_fixed_text.size());
				_buttons_offScript_fixed_value.resize(_buttons_offScript_fixed.size());
				_buttons_offScript_fixed_value_old.resize(_buttons_offScript_fixed.size());
				for (int j=0; j<buttons_offScript_fixed_row;j++){
					for (int i=0; i<buttons_offScript_col;i++){
						unsigned int button_index = j*buttons_offScript_col + i;
						if (button_index <_buttons_offScript_fixed.size()) {
							_buttons_offScript_fixed[button_index] = new Fl_Button(box_offScript_x + (i+1)*buttons_offScript_spacing + i*buttons_offScript_w, box_offScript_y + (j+1)*buttons_offScript_spacing + j*buttons_offScript_h, buttons_offScript_w, buttons_offScript_h, _buttons_offScript_fixed_text[button_index].c_str());
							_buttons_offScript_fixed[button_index] ->type(FL_NORMAL_BUTTON);
							_buttons_offScript_fixed[button_index] ->callback(button_cb, &_buttons_offScript_fixed_value[button_index]);
							_buttons_offScript_fixed[button_index] ->labelsize(14);
							_buttons_offScript_fixed[button_index]->labelfont(FL_HELVETICA_BOLD);  
							_buttons_offScript_fixed[button_index]->when(FL_WHEN_ENTER_KEY | FL_WHEN_CHANGED | FL_WHEN_RELEASE); 
							_buttons_offScript_fixed[button_index]->align(FL_ALIGN_CENTER | FL_ALIGN_INSIDE |FL_ALIGN_IMAGE_OVER_TEXT);
							icon_image = new Fl_PNG_Image(_buttons_offScript_fixed_icons[button_index].c_str());
							_buttons_offScript_fixed[button_index]->image(icon_image);
							_buttons_offScript_fixed[button_index]->color(_buttons_offScript_fixed_color[button_index]);
						}
					}
				}

				_buttons_offScript_alwaysOn.resize(_buttons_offScript_alwaysOn_text.size());
				_buttons_offScript_alwaysOn_value.resize(_buttons_offScript_alwaysOn.size());
				_buttons_offScript_alwaysOn_value_old.resize(_buttons_offScript_alwaysOn.size());
				for (int j=0; j<buttons_offScript_alwaysOn_row;j++){
					for (int i=0; i<buttons_offScript_col;i++){
						unsigned int button_index = j*buttons_offScript_col + i;
						if (button_index <_buttons_offScript_alwaysOn.size()) {
							_buttons_offScript_alwaysOn[button_index] = new Fl_Button(box_offScript_x + (_buttons_offScript_fixed.size()+i+1)*buttons_offScript_spacing + (_buttons_offScript_fixed.size()+i)*buttons_offScript_w,
																					box_offScript_y + (j+1)*buttons_offScript_spacing + j*buttons_offScript_h, buttons_offScript_w, buttons_offScript_h, _buttons_offScript_alwaysOn_text[button_index].c_str());
							_buttons_offScript_alwaysOn[button_index] ->type(FL_NORMAL_BUTTON);
							_buttons_offScript_alwaysOn[button_index] ->callback(button_cb, &_buttons_offScript_alwaysOn_value[button_index]);
							_buttons_offScript_alwaysOn[button_index] ->labelsize(14);
							_buttons_offScript_alwaysOn[button_index]->labelfont(FL_HELVETICA_BOLD);  
							_buttons_offScript_alwaysOn[button_index]->when(FL_WHEN_ENTER_KEY | FL_WHEN_CHANGED | FL_WHEN_RELEASE); 
							_buttons_offScript_alwaysOn[button_index]->align(FL_ALIGN_CENTER | FL_ALIGN_INSIDE |FL_ALIGN_IMAGE_OVER_TEXT);
							icon_image = new Fl_PNG_Image(_buttons_offScript_alwaysOn_icons[button_index].c_str());
							_buttons_offScript_alwaysOn[button_index]->image(icon_image);
							_buttons_offScript_alwaysOn[button_index]->color(_buttons_offScript_alwaysOn_color[button_index]);
						}
					}
				}

				_buttons_offScript_feedback.resize(_buttons_offScript_feedback_text.size());
				_buttons_offScript_feedback_value.resize(_buttons_offScript_feedback.size());
				_buttons_offScript_feedback_value_old.resize(_buttons_offScript_feedback.size());
				for (int j=0; j<buttons_offScript_feedback_row;j++){
					for (int i=0; i<buttons_offScript_col;i++){
						unsigned int button_index = j*buttons_offScript_col + i;
						if (button_index <_buttons_offScript_feedback.size()) {
							_buttons_offScript_feedback[button_index] = new Fl_Button(box_offScript_x + (i+1)*buttons_offScript_spacing + i*buttons_offScript_w,
																					box_offScript_y + (buttons_offScript_fixed_row+buttons_offScript_alwaysOn_row)*(buttons_offScript_h + buttons_offScript_spacing) + (j+1)*buttons_offScript_spacing + j*buttons_offScript_h, buttons_offScript_w, buttons_offScript_h, _buttons_offScript_feedback_text[button_index].c_str());
							_buttons_offScript_feedback[button_index] ->type(FL_NORMAL_BUTTON);
							_buttons_offScript_feedback[button_index] ->callback(button_cb, &_buttons_offScript_feedback_value[button_index]);
							_buttons_offScript_feedback[button_index] ->labelsize(14);
							_buttons_offScript_feedback[button_index]->labelfont(FL_HELVETICA_BOLD);  
							_buttons_offScript_feedback[button_index]->when(FL_WHEN_ENTER_KEY | FL_WHEN_CHANGED | FL_WHEN_RELEASE); 
							_buttons_offScript_feedback[button_index]->align(FL_ALIGN_CENTER | FL_ALIGN_INSIDE |FL_ALIGN_IMAGE_OVER_TEXT);
							icon_image = new Fl_PNG_Image(_buttons_offScript_feedback_icons[button_index].c_str());
							_buttons_offScript_feedback[button_index]->image(icon_image);
							_buttons_offScript_feedback[button_index]->color(_buttons_offScript_feedback_color[button_index]);
						}
					}
				}

				_buttons_offScript_scenario.resize(_buttons_offScript_scenario_text.size());
				_buttons_offScript_scenario_value.resize(_buttons_offScript_scenario.size());		
				_buttons_offScript_scenario_value_old.resize(_buttons_offScript_scenario.size());
				for (int j=0; j<buttons_offScript_scenario_row;j++){
					for (int i=0; i<buttons_offScript_col;i++){
						unsigned int button_index = j*buttons_offScript_col + i;
						if (button_index <_buttons_offScript_scenario.size()) {
							_buttons_offScript_scenario[button_index] = new Fl_Button(box_offScript_x + (i+1)*buttons_offScript_spacing + i*buttons_offScript_w,
																					box_offScript_y + (buttons_offScript_fixed_row + buttons_offScript_feedback_row)*(buttons_offScript_h + buttons_offScript_spacing) + (j+1)*buttons_offScript_spacing + j*buttons_offScript_h,
																					buttons_offScript_w, buttons_offScript_h, _buttons_offScript_scenario_text[button_index].c_str());
							_buttons_offScript_scenario[button_index] ->type(FL_NORMAL_BUTTON);
							_buttons_offScript_scenario[button_index] ->callback(button_cb, &_buttons_offScript_scenario_value[button_index]);
							_buttons_offScript_scenario[button_index] ->labelsize(14);
							_buttons_offScript_scenario[button_index]->labelfont(FL_HELVETICA_BOLD);  
							_buttons_offScript_scenario[button_index]->when(FL_WHEN_ENTER_KEY | FL_WHEN_CHANGED | FL_WHEN_RELEASE); 
							_buttons_offScript_scenario[button_index]->align(FL_ALIGN_CENTER | FL_ALIGN_INSIDE |FL_ALIGN_TEXT_OVER_IMAGE);
							icon_image = new Fl_PNG_Image(_buttons_offScript_scenario_icons[button_index].c_str());
							_buttons_offScript_scenario[button_index]->image(icon_image);
							_buttons_offScript_scenario[button_index]->color(_buttons_offScript_scenario_color[button_index]);
						}
					}
				}
			}
			_box_offScript->end();
		}
		_tab_loadScript->end();
	}
	{
		_tab_betweenScripts = new Fl_Group(tab_supervisedControl_x, tab_supervisedControl_y, tab_supervisedControl_w,tab_supervisedControl_h,"Between Scripts");
		_tab_betweenScripts->labelsize(13);
		_tab_betweenScripts->labelfont(FL_HELVETICA_BOLD);{
			_buttons_betweenScripts.resize(_buttons_betweenScripts_text.size());
			_buttons_betweenScripts_value.resize(_buttons_betweenScripts.size());
			_buttons_betweenScripts_value_old.resize(_buttons_betweenScripts.size());
			for (int j=0; j<buttons_betweenScripts_row;j++){
				for (int i=0; i<buttons_betweenScripts_col;i++){
					unsigned int button_index = j*buttons_betweenScripts_col + i;
					if (button_index <_buttons_betweenScripts.size()) {
						//cout << "button_index: " << button_index <<endl;
						_buttons_betweenScripts[button_index] = new Fl_Button(tab_supervisedControl_x + (i+1)*buttons_betweenScripts_spacing + i*buttons_betweenScripts_w,
																			tab_supervisedControl_y + (j+1)*buttons_betweenScripts_spacing + j*buttons_betweenScripts_h,
																			buttons_betweenScripts_w, buttons_betweenScripts_h, _buttons_betweenScripts_text[button_index].c_str());
						//cout << box_offScript_x + (i+1)*buttons_betweenScripts_spacing + i*buttons_betweenScripts_w  << " " << box_offScript_y + (j+1)*buttons_betweenScripts_spacing + j*buttons_betweenScripts_h << " " << buttons_betweenScripts_w << " " << buttons_betweenScripts_h <<endl;
						_buttons_betweenScripts[button_index] ->type(FL_NORMAL_BUTTON);
						_buttons_betweenScripts[button_index] ->callback(button_cb, &_buttons_betweenScripts_value[button_index]);
						_buttons_betweenScripts[button_index] ->labelsize(16);
						_buttons_betweenScripts[button_index]->labelfont(FL_HELVETICA_BOLD);  
						_buttons_betweenScripts[button_index]->when(FL_WHEN_ENTER_KEY | FL_WHEN_CHANGED | FL_WHEN_RELEASE); 
						_buttons_betweenScripts[button_index]->align(FL_ALIGN_CENTER | FL_ALIGN_INSIDE |FL_ALIGN_IMAGE_OVER_TEXT);
						icon_image = new Fl_PNG_Image(_buttons_betweenScripts_icons[button_index].c_str());
						_buttons_betweenScripts[button_index]->image(icon_image);
						_buttons_betweenScripts[button_index]->color(_buttons_betweenScripts_color[button_index]);						
					}
				}
			}

			_buttons_enableRobot.resize(_buttons_enableRobot_text.size());
			_buttons_enableRobot_value.resize(_buttons_enableRobot.size());
			_buttons_enableRobot_value_old.resize(_buttons_enableRobot.size());			
			for (int j=0; j<buttons_enableRobot_row;j++){
				for (int i=0; i<buttons_betweenScripts_col;i++){
					unsigned int button_index = j*buttons_betweenScripts_col + i;
					if (button_index <_buttons_enableRobot.size()) {
						//cout << "button_index: " << button_index <<endl;
						_buttons_enableRobot[button_index] = new Fl_Button(tab_supervisedControl_x + (i+1)*buttons_betweenScripts_spacing + i*buttons_betweenScripts_w,
																			tab_supervisedControl_y + buttons_betweenScripts_row*(buttons_betweenScripts_h + buttons_betweenScripts_spacing) + (j+1)*buttons_betweenScripts_spacing + j*buttons_betweenScripts_h,
																			buttons_betweenScripts_w, buttons_betweenScripts_h, _buttons_enableRobot_text[button_index].c_str());
						//cout << box_offScript_x + (i+1)*buttons_enableRobot_spacing + i*buttons_enableRobot_w  << " " << box_offScript_y + (j+1)*buttons_enableRobot_spacing + j*buttons_enableRobot_h << " " << buttons_enableRobot_w << " " << buttons_enableRobot_h <<endl;
						_buttons_enableRobot[button_index] ->type(FL_NORMAL_BUTTON);
						_buttons_enableRobot[button_index] ->callback(button_cb, &_buttons_enableRobot_value[button_index]);
						_buttons_enableRobot[button_index] ->labelsize(16);
						_buttons_enableRobot[button_index]->labelfont(FL_HELVETICA_BOLD);  
						_buttons_enableRobot[button_index]->when(FL_WHEN_ENTER_KEY | FL_WHEN_CHANGED | FL_WHEN_RELEASE); 
						_buttons_enableRobot[button_index]->align(FL_ALIGN_CENTER | FL_ALIGN_INSIDE |FL_ALIGN_IMAGE_OVER_TEXT);
						icon_image = new Fl_PNG_Image(_buttons_enableRobot_icons[button_index].c_str());
						_buttons_enableRobot[button_index]->image(icon_image);
						_buttons_enableRobot[button_index]->color(_buttons_enableRobot_color[button_index]);
					}
				}
			}

			_textInput_say = new Fl_Input(textInput_say_x, textInput_say_y, textInput_say_width, textInput_say_height,"");
			_textInput_say->textsize(12);
			_textInput_say->callback(get_input_cb,&_textInput_say_value);  
			_textInput_say->labelsize(12);
			_textInput_say->labelfont(FL_HELVETICA_BOLD); 
			_textInput_say->align(FL_ALIGN_LEFT);
			//_textInput_say->value(_textInput_say_value);
			get_input_cb(_textInput_say,_textInput_say_value);
			_textInput_say->when(FL_WHEN_RELEASE);  // callback is called: when the enter key is hit

			_button_say = new Fl_Button(button_say_x, button_say_y, button_say_w, button_say_h,"Say");
			_button_say ->type(FL_NORMAL_BUTTON);
			_button_say ->callback(button_cb, &_button_say_value);
			_button_say ->labelsize(16);
			_button_say->labelfont(FL_HELVETICA_BOLD);  
			_button_say->when(FL_WHEN_ENTER_KEY | FL_WHEN_CHANGED | FL_WHEN_RELEASE); 
			_button_say->align(FL_ALIGN_CENTER | FL_ALIGN_INSIDE | FL_ALIGN_IMAGE_OVER_TEXT);
			icon = _iconUrl + "say_icon.png";
			//icon_image = (Fl_PNG_Image*) resizeImage(icon,button_say_h,button_say_h);
			icon_image = new Fl_PNG_Image(icon.c_str());
			_button_say->image(icon_image);			

			_checkButton_enableRAS.resize(_checkButton_enableRAS_text.size());
			_checkButton_enableRAS_value.resize(_checkButton_enableRAS_text.size());
			_checkButton_enableRAS_value_old.resize(_checkButton_enableRAS_text.size());
			for (unsigned int i=0; i< _checkButton_enableRAS.size(); i++) {
				_checkButton_enableRAS[i] = new Fl_Check_Button(checkButton_enableRAS_x,checkButton_enableRAS_y + i*(checkButton_enableRAS_h + border),checkButton_enableRAS_w,checkButton_enableRAS_h,_checkButton_enableRAS_text[i].c_str());
				_checkButton_enableRAS[i]->callback(button_cb, &_checkButton_enableRAS_value[i]); 
				_checkButton_enableRAS[i]->labelsize(12);
				_checkButton_enableRAS[i]->labelfont(FL_HELVETICA_BOLD);
				_checkButton_enableRAS[i]->when(FL_WHEN_ENTER_KEY | FL_WHEN_CHANGED); 
				//_checkButton_enableRAS[i]->align(FL_ALIGN_LEFT | FL_ALIGN_CENTER | FL_ALIGN_TEXT_NEXT_TO_IMAGE); 
				_checkButton_enableRAS[i]->value(0);
				// Write first value
				_checkButton_enableRAS_value[i] = _checkButton_enableRAS[i]->value();
				_checkButton_enableRAS_value_old[i] = _checkButton_enableRAS[i]->value();
			}
		}
		_tab_betweenScripts->end();
	}
	{
		_tab_childHistory = new Fl_Group(tab_supervisedControl_x, tab_supervisedControl_y, tab_supervisedControl_w,tab_supervisedControl_h,"Child History");
		_tab_childHistory->labelsize(13);
		_tab_childHistory->labelfont(FL_HELVETICA_BOLD);{
			
			_box_childInfo = new Fl_Scroll(box_childInfo_x, box_childInfo_y, box_childInfo_w, box_childInfo_h,"" );
			_box_childInfo->align(FL_ALIGN_TOP | FL_ALIGN_LEFT | FL_ALIGN_INSIDE);
			_box_childInfo->labelsize(14);
			_box_childInfo->labelfont(FL_HELVETICA_BOLD);
			_box_childInfo->box(FL_DOWN_BOX);{

				_output_userData.resize(_output_userData_text.size());
				_output_userData_value.resize(_output_userData_text.size());
				for (unsigned int i=0; i< _output_userData.size(); i++) {
					_output_userData[i] = new Fl_Output(output_userData_x,output_userData_y + i*(output_userData_h+ 2*border),output_userData_w,output_userData_h,_output_userData_text[i].c_str());
					_output_userData[i]->labelsize(14);
					_output_userData[i]->labelfont(FL_HELVETICA_BOLD);
					_output_userData[i]->box(FL_DOWN_BOX);
					_output_userData[i]->value(_output_userData_value[i].c_str());
				}			
			}
			_box_childInfo->end();

			_browser_childPerformance = new Fl_Browser(browser_childPerformance_x,browser_childPerformance_y,browser_childPerformance_w,browser_childPerformance_h);
			_browser_childPerformance->column_char('\t');                                                       // tabs as column delimiters
			_browser_childPerformance->type(FL_MULTI_BROWSER);
			static int widths[] = {20,70,80,200,80,100,100,100};               // widths for each column
			_browser_childPerformance->column_widths(widths);	
			_browser_childPerformance->add("@bID\t@bScript ID\t@bSession ID\t@bSession Name\t@bPartner\t@bPerformance\t@bEngagement\t@bTurn Taking"); // lines of tab delimited data
			
			// Add user history data
			/*
			for (unsigned int i=0; i<_childPerformance_Session.size();i ++) {
				stringstream ss;
				ss << i+1 << "\t" << _childPerformance_Session[i] << "\t" << _childPerformance_value[i] << "/4";
				string temp_line = ss.str();
				_browser_childPerformance->add(temp_line.c_str());
			}*/
			for (unsigned int i=0; i<_output_userHistory.size();i++){
				_browser_childPerformance->add(_output_userHistory[i].c_str());
			}

		}
		_tab_childHistory->end();
	}
	{
		_tab_developerInfo = new Fl_Group(tab_supervisedControl_x, tab_supervisedControl_y, tab_supervisedControl_w,tab_supervisedControl_h,"Developer Info");
		_tab_developerInfo->labelsize(13);
		_tab_developerInfo->labelfont(FL_HELVETICA_BOLD);{		
			//_list_developerInfoBuffer = new Fl_Text_Buffer();
			_list_developerInfo = new Fl_Text_Display(list_developerInfo_x, list_developerInfo_y, list_developerInfo_w, list_developerInfo_h);
			_list_developerInfo->textsize(12);
			_list_developerInfo->color(FL_BLACK);
			_list_developerInfo->textcolor(FL_WHITE);
			_list_developerInfo->align(FL_ALIGN_TOP | FL_ALIGN_LEFT);
			_list_developerInfo->buffer(_list_developerInfoBuffer);
			
			_button_exportLogfile = new Fl_Button(button_exportLogfile_x, button_exportLogfile_y, button_exportLogfile_w, button_exportLogfile_h,"Export to file");
			_button_exportLogfile ->type(FL_NORMAL_BUTTON);
			_button_exportLogfile ->callback(button_cb, &_button_exportLogfile_value);
			_button_exportLogfile ->labelsize(16);
			_button_exportLogfile->labelfont(FL_HELVETICA_BOLD);  
			_button_exportLogfile->when(FL_WHEN_ENTER_KEY | FL_WHEN_CHANGED | FL_WHEN_RELEASE); 
			_button_exportLogfile->align(FL_ALIGN_CENTER | FL_ALIGN_INSIDE | FL_ALIGN_IMAGE_OVER_TEXT);
			icon = _iconUrl + "script_icon.png";
			//icon_image = (Fl_PNG_Image*) resizeImage(icon,button_exportLogfile_h,button_exportLogfile_h);
			icon_image = new Fl_PNG_Image(icon.c_str());
			_button_exportLogfile->image(icon_image);
		}
		_tab_developerInfo->end();
	}
	_start_systemArchitecture_GUI->end();
	// end of the group 
}

void SystemGUIController::GUIrefresh(void) {
	// Back up
	int old_userName_choice = _choice_userName->value();
	char old_textInput_sessionName[1024];
	sprintf(old_textInput_sessionName, _textInput_sessionName->value());
	int old_choice_sessionPartner = _choice_sessionPartner->value();
	int old_choice_scriptType =  _choice_scriptType->value();

	GUIinit();
	DVwindow->clear();
	DVwindow->add(_start_systemArchitecture_GUI);
	// Restore old values
	_choice_userName->value(old_userName_choice);
	_textInput_sessionName->value(old_textInput_sessionName);
	sprintf(_textInput_sessionName_value,old_textInput_sessionName);
	_choice_sessionPartner->value(old_choice_sessionPartner);
	_choice_scriptType->value(old_choice_scriptType);
	_button_loadUser_value_old = 0;
	_button_loadUser_value = 0;
}

// Refresh GUI if GUI crashes
void SystemGUIController::checkRefreshEvent(){
	if ((_box_dreamLogo_value == TRUE) && (_box_dreamLogo_value_old!=_box_dreamLogo_value)) {
		DVwindow->redraw();
		Fl::check();
		cout << "Attempt to defreeze GUI"<< endl;
	}
	// Update old value
	_box_dreamLogo_value_old=_box_dreamLogo_value;
}

void SystemGUIController::button_loadUser_Clicked() {
	// Check if the button is clicked
	//cout << _button_loadUser_value << _button_loadUser_value_old << _button_loadUser_value << endl;
	if ((_button_loadUser_value == TRUE) && (_button_loadUser_value_old!=_button_loadUser_value)) {
		//cout <<"Text:" << _textInput_sessionName_value << endl;
		if ((_userName != "")&&(string(_textInput_sessionName_value) != "")&&(_sessionPartner != "")&&(_scriptType != "")){
	/*
	if ((_button_loadUser_value == TRUE) && (_button_loadUser_value_old!=_button_loadUser_value)) {
		if (((_userName != "")&&(string(_textInput_sessionName_value) != "")&&(_sessionPartner != ""))&&
			((_userName_old != _userName)||
			(string(_textInput_sessionName_value_old) != string(_textInput_sessionName_value))||
			(_sessionPartner_old != _sessionPartner))){*/
			logWrite("Load user profile: " + _userName + " " + string(_textInput_sessionName_value) + " " + _sessionPartner + " " + _scriptType);
			// Do more things here
			
			_userName_choice_value = _choice_userName->value();
			// JK TODO: need to add session name textbox (string free entry), and a interaction partner dropdown (robot or therapist)
			// rename the label on this button to be 'Create Session'
			// send the values to SMS when this is clicked on the therapistCommand port ("Create Session", session_name (string), interaction_partner (string))
			// HLC: Done
			//_userID = _userList.m_users[_userName_choice_value-1].m_id; // -1 because of "Please select a user"
			
			// Check userID based on userName
			for (unsigned int i = 0; i<_userList.m_users.size(); i++) {
				if (_userName == _userList.m_users[i].m_name)
					_userID = _userList.m_users[i].m_id;
			}

			_yarpInterface->sendToUserID(_userID);
			_yarpInterface->sendToTherapistCommand(_ACTION_CREATE_, string(_textInput_sessionName_value), _sessionPartner);
			_yarpInterface->sendToSelectedBySupervisor(_ORIGIN_THERAPIST_,_ACTION_SCRIPT_TYPE_, _sessionPartner, _scriptType);

			// Refresh GUI
			loadUserData(_userID); // load userData and history
			GUIrefresh(); // check later to add back
			//_box_onScript->deactivate();// test IWC
			_box_onScript->activate();// test IWC
			_button_doItNow->deactivate();// test IWC
			_button_iWillChoose->deactivate();// test IWC
			
			//_box_offScript->deactivate();
			showOffscriptButtons(0);
			_tab_loadScript->activate();
			
			_tabs_supervisedControl->value(_tab_loadScript); // focus
			// Update old values
			_userName_old = _userName;
			sprintf(_textInput_sessionName_value_old,_textInput_sessionName_value);
			_sessionPartner_old = _sessionPartner;			
			_scriptType_old = _scriptType;
		}		
	}
	// Update old value
	_button_loadUser_value_old = _button_loadUser_value;
}

void SystemGUIController::button_loadScript_Clicked() {
	// Check if the button is clicked
	//if ((_button_loadScript_value == TRUE) && (_button_loadScript_value_old!=_button_loadScript_value)) {
	//if ((_script!="")&&(_script_old!=_script)) {
	//if (_button_loadScript_value == TRUE){
	if ((_button_loadScript_value == TRUE) && (_button_loadScript_value_old!=_button_loadScript_value)) {
		if (!strcmp(_button_loadScript->label(),"Start script")) {
			if (_script!="") {
				logWrite("Load script: " + _script);
				// Do more things here
				_script_choice_value = _choice_script->value();
				// Set flag
				_isScriptRunning = true;
				// Refresh GUI
				//Update scenario steps
				loadScriptSteps(_script);
				update_list_scriptSteps(_currentStep); // added because we dont refresh GUI
			
				GUIrefresh(); // check later to add back
				_tab_betweenScripts->deactivate();
				_choice_userName->deactivate();
				_button_loadUser->deactivate();
				_choice_script->deactivate();
				
				_button_loadScript->label("Stop script");
				// fix bug
				_button_loadScript->value(0);
				_button_loadScript_value = 0;
				//_button_loadScript->deactivate(); // don't deactivate loadScript button but change name
				//_box_onScript->deactivate();
				_box_onScript->activate(); //testing, switch back if neccessary
				//_box_offScript->deactivate();
				showOffscriptButtons(0);


				_button_iWillChoose->deactivate(); // test IWC
				_button_doItNow->deactivate(); 
			
				_box_onScript->label("Starting script...");
				_box_actionTime->label("");// fix bug

				// Reset current performance
				_currentUserInteractionData.m_sPerformance= "";
				_currentPerformanceString = calculateCurrentPerformance(_currentUserInteractionData.m_sPerformance);
				sprintf(_currentPerformanceDisplay, _currentPerformanceString.c_str());
						
				// Send to therapistCommand
				_scriptID = _script_list_library.m_scripts[_script_choice_value-1].m_id;		// -1  Please select a script
				_scriptTypeID = _script_list_library.m_scripts[_script_choice_value-1].m_type_id; // -1  Please select a script
				_yarpInterface->sendToTherapistCommand(_ACTION_SCRIPTLOAD_,_scriptID);

				// Send to interventionCommand
				sendFirstScriptStep();

                // Turn OFF Reactive and Attention subsystems
                _yarpInterface->sendToTherapistCommand(_checkButton_enableRAS_text[0],0);
                _yarpInterface->sendToTherapistCommand(_checkButton_enableRAS_text[1],0);
				_checkButton_enableRAS_value[0] = 0;
				_checkButton_enableRAS_value[1] = 0;
				_checkButton_enableRAS[0]->value(0);
                _checkButton_enableRAS[1]->value(0);			

				// Start simulating the script
				//_tStartSimulation = new boost::thread(&SystemGUIController::startSimulation,this); // check later			
				// test: called by the loop anyway

				// THERAPIST partner - Overwrite the above parameters (temporary solution)
				if (_sessionPartner == "Therapist") {
					_button_iWillChoose->deactivate();
					_button_doItNow->activate();
					showOffscriptButtons(1);
					_isGoToNextStep = true;
					_currentStep = -1;
					//_box_onScript->label("Next script step:");
					_box_onScript->label("");
					cout << "Therapist partner mode" << endl;
				}
				// Update old value
				_script_old=_script;
			}
		}
		else{
			// Send stop script message
			_yarpInterface->sendToTherapistCommand(_ACTION_STOPSCRIPT_);
			vector<string> stopscriptAction;
			stringstream ss;
			ss << _ORIGIN_THERAPIST_;
			stopscriptAction.push_back(ss.str());
			ss.clear();
			ss.str("");
			ss << _ACTION_GO_OFF_SCRIPT_;
			stopscriptAction.push_back(ss.str());
			_yarpInterface->sendToSelectedBySupervisor(stopscriptAction);

			// Update GUI
			cout << "Script stopped" <<endl;
			
			// Clear flags
			_isSimulationRunning = false;
			_isScriptRunning = false;
			_isStopTimerAction = true;// stop timer
			_isGoToNextStep = false;

			//Clear variables
			_script_choice_value = 0;			

			_proposedAction.clear();
			sprintf(_proposedActionText,"");
			_proposedActionID = _ACTION_NO_ACTION_;
			// Reset current action variables
			_currentAction.clear();
			sprintf(_currentPerformActionText,"");
			_currentActionID = _ACTION_NO_ACTION_; 	

			_currentStep = -1;
			_currentStep_old = -1;
			//_script="";

			// Update GUI
			_button_loadScript->label("Start script");
			_list_scriptStepsBuffer->text("");
			_progress_Timer->label("Script stopped - select another or between script actions");
			_box_actionTime->label("");
			_box_onScript->label("");
			_choice_userName->activate();
			_button_loadUser->activate();
			_choice_script->activate();
			//_button_loadScript->activate();
			_tab_betweenScripts->activate();
			_button_doItNow->deactivate();
			// fix bug
			_button_iWillChoose->value(0);
			_button_iWillChoose_value = 0;
			_button_iWillChoose->deactivate(); // test IWC
			_button_loadScript->value(0);
			_button_loadScript_value = 0;

			// Turn off offscript buttons
			showOffscriptButtons(0);

            // Turn ON Reactive and Attention subsystems
            //_yarpInterface->sendToTherapistCommand(_checkButton_enableRAS_text[0],1);
            //_yarpInterface->sendToTherapistCommand(_checkButton_enableRAS_text[1],1);
			_checkButton_enableRAS_value[0] = 1;
			_checkButton_enableRAS_value[1] = 1;
			_checkButton_enableRAS[0]->value(1);
            _checkButton_enableRAS[1]->value(1);
		}
	}
	// Update old value
	_button_loadScript_value_old = _button_loadScript_value;
	//cout << _button_loadScript_value << " " << _button_loadScript_value_old << endl;
}

void SystemGUIController::button_updateSliderTimer_Clicked() {
	// Check if the button is clicked
	if ((_button_updateSliderTimer_value == TRUE) && (_button_updateSliderTimer_value_old!=_button_updateSliderTimer_value)) {
		if (_timer_value != _timer_value_temp) {
			_timer_value = _timer_value_temp;
			stringstream ss;
			ss << _timer_value;
			logWrite("Timer changed: " + ss.str());	
			// Send to therapistCommand port
			_yarpInterface->sendToTherapistCommand(_ACTION_COUNTDOWN_, _timer_value);

		}
	}
	// Update old value
	_button_updateSliderTimer_value_old = _button_updateSliderTimer_value;
}

void SystemGUIController::timerActionStart(void){
	_isStopTimerAction = false;
	_tTimerAction = new boost::thread(&SystemGUIController::timerAction,this);
	//cout << "Timer started"<<endl;
}

void SystemGUIController::timerActionStop(void){
	_isStopTimerAction = true;
	//cout << "Attempt to stop timer"<<endl;
	_current_timer_value = 0;
	_progress_Timer_value = 0;
	_progress_Timer->value((float)_progress_Timer_value);
}
void SystemGUIController::timerAction(void){
	_current_timer_value = 0;
	//cout << "Timer is running" << endl;
	while(_current_timer_value<_timer_value) { 
		_actionTimeDisplay= actionTimeDisplay(_current_timer_value);		
		_box_actionTime->label(_actionTimeDisplay.c_str());
		
		double one_second = 0;
		while (one_second <20) { // 1 second
			//Sleep(50);
			timer_miliseconds(50);
			one_second++;
			_progress_Timer_value = (_current_timer_value + one_second*50/1000)/_timer_value;
			//cout << "Current progress timer value: "<<_progress_Timer_value << endl;
			if (_progress_Timer_value>1) _progress_Timer_value =1;
			_progress_Timer->value((float)_progress_Timer_value);
			if (_isStopTimerAction == true){
				_progress_Timer_value = 0;
				//_progress_Timer->value(0.0f);	
				_progress_Timer->value((float)_progress_Timer_value); //testing
				return; // return when _isStopTimerAction is set
			}
		}		
		_current_timer_value++;
	}
	//logWrite("Perform the current action (Timeout): " + _currentScriptSteps[_currentStep]);
	//cout << "Perform the current action (Timeout): " << _proposedActionText << endl;
	//logWrite("Perform the current action (Timeout): " + string(_proposedActionText));
	cout << "Perform the current action (Timeout): " << _currentPerformActionText << endl;
	logWrite("Perform the current action (Timeout): " + string(_currentPerformActionText));
	
	// Send action to selectedBySupervisor
	//_yarpInterface->sendToSelectedBySupervisor(_proposedAction);
	_yarpInterface->sendToSelectedBySupervisor(_currentAction);

	//_current_timer_value = 0;
	_tPerformAction = new boost::thread(&SystemGUIController::performCurrentAction,this);
	//performCurrentAction();
}

void SystemGUIController::performCurrentAction(void){
	//cout <<"Perform action (start): "<< _currentScriptSteps[_currentStep] << endl;
	//cout <<"Perform action (start): "<< _proposedActionText << endl;
	cout <<"Perform action (start): "<< _currentPerformActionText << endl;
	
	_box_actionTime->label("");
	_box_onScript->label("");
	//_progress_Timer->label("Robot performing selected action");

	_progress_Timer_value = 0;
	//_progress_Timer->value(1.0f);	// full and red
	_progress_Timer->value((float)_progress_Timer_value); // test
	_progress_Timer->label("");
	//_progress_Timer->color(FL_RED);
	//_progress_Timer->color2(FL_RED);
		
	_button_doItNow->value(0);
	_button_doItNow_value = 0;
	_button_doItNow->deactivate();
	_button_iWillChoose->deactivate();
	
	
	// HLC: Do not need to wait anymore
	//DVwindow->cursor(FL_CURSOR_WAIT);
	//Fl::check();//avoid crashing
	// Send action to the robot
	//Sleep(500); // from James - change later to condition - e.g. while loop // Done
	// Wait until the selected action finishes
	//while (_proposedActionID!=_ACTION_FINISHED_) Sleep(50); // maybe use another variable e.g. a flag
	//DVwindow->cursor(FL_CURSOR_DEFAULT);
	
	//_progress_Timer_value = 0;
	//_progress_Timer->value(0.0f); // back to 0 and green
	//_progress_Timer->value((float)_progress_Timer_value);
	//_progress_Timer->color(FL_WHITE);
	//_progress_Timer->color2(FL_GREEN);
	//_progress_Timer->label("");
	//_box_onScript->redraw();

	//_isGoToNextStep = true;
	//_box_onScript->label("Loading next step...");
	_box_onScript->label("Action finished!");
	_button_iWillChoose->activate(); // test IWC

	//cout <<"Perform action (finish): "<< _currentScriptSteps[_currentStep] << endl;
	//cout <<"Perform action (finish): "<< _proposedActionText << endl;
	cout <<"Perform action (finish): "<< _currentPerformActionText << endl;	

	// Reset current action variables
	_currentAction.clear();
	sprintf(_currentPerformActionText,"");
	_currentActionID = _ACTION_NO_ACTION_; 	

	_isSimulationRunning = false;
}

void SystemGUIController::startSimulation(void){	
	_isSimulationRunning = true;
	cout << "Waiting for proposed action" << endl;
	//_button_iWillChoose->deactivate(); // test IWC
	_button_doItNow->deactivate();
	
	//Waiting for proposed action
	//Sleep(2000); //wait 2 second before going to the next step
	
	_box_onScript->label("Waiting for a proposed action");
	while (!strcmp(_proposedActionText,"")){
		if ((!_isSimulationRunning)||(!_isScriptRunning)) return; // when script finished or button stop script is clicked
		Sleep(50);	
	}

	/* Old code
	_box_onScript->label("Waiting for a proposed action");
	while (!strcmp(_proposedActionText,"")){
		if (_proposedActionID == _ACTION_GO_NEXT_STEP_){
			cout << "Go to next step" << endl;
			_box_onScript->label("Go to next step...");
			_isSimulationRunning = false;
			// Reset proposedAction
			//_proposedAction.clear();
			//sprintf(_proposedActionText,"");
			//_proposedActionID = _ACTION_NO_ACTION_;
			return;
		}
		Sleep(50);	
	}
	*/
	
	// Assign action text, prevent it is changed when new value comes at the port
	_currentAction = _proposedAction;
	_currentActionID = _proposedActionID;
	sprintf(_currentPerformActionText,_proposedActionText);

	// Reset proposed action variables //
	_proposedAction.clear();
	sprintf(_proposedActionText,""); 
	_proposedActionID = _ACTION_NO_ACTION_; 
	
	_box_onScript->activate();
	_box_onScript->label("I'm going to:");
	//_box_offScript->deactivate();
	showOffscriptButtons(0);
	_button_doItNow->activate();
	_button_iWillChoose->activate();
	
	// Check _ACTION_FEEDBACK_ (change progress bar color) and _ACTION_MOVE_ parameter to skip timmer
	bool zeroDelayMoveAction = false;
	bool noFeedbackAction= false;
	
	switch (_currentActionID) {
        // these are all the options for a 1 performance
        case _ACTION_FEEDBACK_IMITATION_MOVE_GOOD_:
        case _ACTION_FEEDBACK_IMITATION_OBJECT_GOOD_:
        case _ACTION_FEEDBACK_TT_WAITING_GOOD_:
        case _ACTION_FEEDBACK_TT_SORTING_GOOD_:
        case _ACTION_FEEDBACK_TT_CHOOSING_GOOD_:
        case _ACTION_FEEDBACK_TT_PATTERN_GOOD_:
        case _ACTION_FEEDBACK_JA_GOOD_:
            _progress_bar_color = FL_GREEN;
			if (_currentAction[2]=="0"){
				noFeedbackAction = true;
				stringstream ss;
				ss << _currentPerformActionText<< " - Disabled";
				sprintf(_currentPerformActionText, ss.str().c_str());
			}
            break;

        // these are all the options for a 0 performance
        case _ACTION_FEEDBACK_IMITATION_MOVE_BAD_:
        case _ACTION_FEEDBACK_IMITATION_OBJECT_BAD_:
        case _ACTION_FEEDBACK_TT_WAITING_BAD_:
		case _ACTION_FEEDBACK_LAST_TT_WAITING_BAD_:
        case _ACTION_FEEDBACK_TT_SORTING_BAD_:
        case _ACTION_FEEDBACK_TT_CHOOSING_BAD_:
        case _ACTION_FEEDBACK_TT_PATTERN_BAD_:
        case _ACTION_FEEDBACK_JA_BAD_:
		case _ACTION_FEEDBACK_BAD_NO_ACTION_:
            _progress_bar_color = FL_RED;
			if (_currentAction[2]=="0"){
				//noFeedbackAction = true;
				stringstream ss;
				ss << _currentPerformActionText<< "-Disabled";
				sprintf(_currentPerformActionText, ss.str().c_str());
			}
			break;
		case _ACTION_MOVE_:{
			_progress_bar_color = FL_GRAY0;
			int moveID = atoi(_currentAction[2].c_str());
			zeroDelayMoveAction = isZeroDelayMoveAction(moveID);
			break;
		}
		default: 
			_progress_bar_color = FL_GRAY0;
            break;
    }

	//_progress_Timer->label(_currentScriptSteps[_currentStep].c_str());
	//_progress_Timer->label(_proposedActionText);
	
	_progress_Timer->label(_currentPerformActionText);
	_progress_Timer->color2(_progress_bar_color);
	
	// If non-zero action, skip timer
	
	if ((isZeroDelayAction(_currentActionID))||zeroDelayMoveAction){
		// Send to SMS		
		cout << "Perform the current action (0): " << _currentPerformActionText << endl;
		logWrite("Perform the current action (0): " + string(_currentPerformActionText));
		_yarpInterface->sendToSelectedBySupervisor(_currentAction);
		_tPerformAction = new boost::thread(&SystemGUIController::performCurrentAction,this);
	}
	/* Keep these actions as normal ones with countdown
	else if (noFeedbackAction){
		cout << "Waiting for therapist's decision:" << _currentPerformActionText << endl;
	}*/
	else timerActionStart();
}

void SystemGUIController::checkProposedAction(void){
	// Start simulation if it is not running
	if (_isScriptRunning) {
		if (_sessionPartner == "Robot"){
			if (!_isSimulationRunning) _tStartSimulation = new boost::thread(&SystemGUIController::startSimulation,this);
		}		
	}
	// Check robot and sandtray connection

	/* Old code
	//if (_box_onScript->active()){
	if (_isScriptRunning){
		switch (_proposedActionID){
		case _ACTION_GO_NEXT_STEP_:
			_box_onScript->label("Loading next step...");
			// Reset
			_proposedAction.clear();
			sprintf(_proposedActionText,"");
			_proposedActionID = _ACTION_NO_ACTION_;
			_isGoToNextStep = true;
			break;
		case _ACTION_NO_ACTION_:
			if (!_isSimulationRunning) _tStartSimulation = new boost::thread(&SystemGUIController::startSimulation,this);
			break;
		default:
			break;
		}
	}
	*/
}

void SystemGUIController::goToNextStep(void){
	// Robot partner
	if ((_isScriptRunning)&&(_isGoToNextStep)&&(_sessionPartner=="Robot")){
		_isGoToNextStep = false;
		if (_currentStep < (int) (_currentScriptSteps.size()-1) ) {
			_currentStep++;
			goToNextScriptStep();
			// Update list script
			update_list_scriptSteps(_currentStep);
			cout <<"Current step (GUI): " <<_currentStep <<endl;
			//_box_onScript->label("Loading next step...");
			if (!_isSimulationRunning) _tStartSimulation = new boost::thread(&SystemGUIController::startSimulation,this);
			// StartSimulation is called by the main loop anyway
	
			if (_currentScriptSteps[_currentStep].find ("Check mutual gaze") != string::npos)	
				_box_onScript->label("Waiting for mutual gaze");
			else if (_currentScriptSteps[_currentStep].find ("Wait for performance") != string::npos)
				_box_onScript->label("Waiting for performance");
			else
				_box_onScript->label("Waiting for a proposed action");
		}		
		else { // end script
			cout << "Script finished" <<endl;
			// Clear flags
			_isSimulationRunning = false;
			_isScriptRunning = false;
			_isGoToNextStep = false;
			_isSimulationRunning = false;

			// Clear variables
			_script_choice_value = 0;
			_currentStep = -1;

			// Update GUI
			_list_scriptStepsBuffer->text("");
			_progress_Timer->label("Script finished - select another or between script actions");
			_box_actionTime->label("");
			_box_onScript->label("");
			_choice_userName->activate();
			_button_loadUser->activate();
			_choice_script->activate();
			_button_loadScript->activate();
			_button_loadScript->label("Start script");
			_tab_betweenScripts->activate();
			_button_doItNow->deactivate();
			// correct bug
			_button_iWillChoose->value(0);
			_button_iWillChoose_value = 0;
			//_button_iWillChoose->deactivate(); // test IWC
			_button_iWillChoose->activate(); // test IWC
			// Check later to make sure everything is (de)activated correcly
			
			// Testing only
			/*
			i++;
			logWrite("Load script: " + _script);
			// Do more things here
			_script_choice_value = _choice_script->value();
			// Refresh GUI
			//Update scenario steps
			loadScriptSteps(_script);
			GUIrefresh();
			_tab_betweenScripts->deactivate();
			_choice_userName->deactivate();
			_button_loadUser->deactivate();
			_choice_script->deactivate();
			_button_loadScript->deactivate();
			_box_onScript->deactivate();
			_box_offScript->deactivate();
			
			_progress_Timer->label("Starting script...");
			
			// Start simulating the script
			cout <<"Current step: " <<_currentStep+1 <<"/" << _currentScriptSteps.size() <<endl;
			_tStartSimulation = new boost::thread(&SystemGUIController::startSimulation,this);
			*/
		}		
	}

	// Therapist partner
	if ((_isScriptRunning)&&(_isGoToNextStep)&&(_sessionPartner=="Therapist")){
		_isGoToNextStep = false;
		//goToNextScriptStep(); // Not sure if correct
		// Increase step and update script list
		if (!_backFromWaitingForGazePerf){
			goToNextScriptStep(); // Not sure if correct
			_currentStep++;			
		}
				
		update_list_scriptSteps(_currentStep+1); // show the next step, not current one
		cout << "Current step (GUI-Therapist mode): "<< _currentStep << endl;
		// THERAPIST partner - Check GO NEXT STEP
		if (_currentStep < (int) (_currentScriptSteps.size()-1)) {
			// Current step is gaze or performance (but not init waiting) - wait - do not enable Do It Now button
			if (!_backFromStepMod&&!_backFromWaitingForGazePerf&&(_currentStep >=0)&&(_currentScriptSteps[_currentStep].find ("Check mutual gaze") != string::npos)){
				_box_onScript->label("Currently:");
				_progress_Timer->label("Waiting for gaze");				
				cout << "Waiting for gaze" << endl;
				_waitingForGaze = true;				
			}

			else if (!_backFromStepMod&&!_backFromWaitingForGazePerf&&(_currentStep >=0)&&
					(_currentScriptSteps[_currentStep].find ("Wait for performance") != string::npos)&&
					(_currentScriptSteps[_currentStep].find ("init") == string::npos)){
				_box_onScript->label("Currently:");
				_progress_Timer->label("Waiting for performance");				
				cout << "Waiting for performance"<< endl;
				_waitingForPerf = true;				
			}
			else {
				//_box_onScript->label("Next script step:");
				_box_onScript->label("");
				// Show next step
				string temp_text = _currentScriptSteps[_currentStep+1];
				// trim length
				unsigned int trim_len = 80;
				if (temp_text.length() > trim_len)	temp_text = temp_text.substr(0,trim_len) + "..."; 
				sprintf(_progress_Timer_label,temp_text.c_str());
				_progress_Timer->label(_progress_Timer_label);
				cout << "Waiting for next step: "<< _progress_Timer_label << endl;
				// Enable button
				_button_doItNow->value(0);
				_button_doItNow_value = 0;
				_button_doItNow->activate();

				// Update old values
				_currentStep_old = _currentStep;
			}			
		}

		// Reset 
		_backFromWaitingForGazePerf = false;
		_backFromStepMod = false;
	}
}

void SystemGUIController::buttons_onScript_Clicked(){
	// Do it now
	if ((_button_doItNow_value == TRUE) && (_button_doItNow_value_old!=_button_doItNow_value)) {
		//logWrite("Perform the current action (Theparist): " + _currentScriptSteps[_currentStep]);
		//logWrite("Perform the current action (Theparist): " + string(_proposedActionText));
		if (_sessionPartner == "Robot") {
			cout << "Perform the current action (Therapist): " << _currentPerformActionText << endl;
			logWrite("Perform the current action (Theparist): " + string(_currentPerformActionText));		
			//_current_timer_value = _timer_value;
			//timerActionStop();
			_isStopTimerAction = true;// stop timer
			// Send action to selectedBySupervisor
			//_yarpInterface->sendToSelectedBySupervisor(_proposedAction);
			_yarpInterface->sendToSelectedBySupervisor(_currentAction);
			_tPerformAction = new boost::thread(&SystemGUIController::performCurrentAction,this);				
		}

		else if (_sessionPartner == "Therapist") {
			if (_feedbackStep&&!_feedbackCorrected){
				_yarpInterface->sendToSelectedBySupervisor(_proposedAction);
				_feedbackCorrected = false;
				_feedbackStep = false;
				Sleep (10);
			}

			vector<string> doItNow;
			stringstream ss1, ss2;
			ss1 << _ORIGIN_THERAPIST_;
			doItNow.push_back(ss1.str());
			ss2 << _ACTION_GO_NEXT_STEP_;
			doItNow.push_back(ss2.str());		
			_yarpInterface->sendToSelectedBySupervisor(doItNow);

			_button_doItNow->value(0);
			_button_doItNow_value = 0;
			_button_doItNow->deactivate();

			// go to next step and check waiting steps there
			_isGoToNextStep = true;

			//_button_doItNow->deactivate();
			// fix bug
			//_button_doItNow->value(0);
			//_button_doItNow = 0;	
		}
	}
	// Update old value
	_button_doItNow_value_old = _button_doItNow_value;

	// I will choose
	if ((_button_iWillChoose_value == TRUE) && (_button_iWillChoose_value_old!=_button_iWillChoose_value)) {
		if (!strcmp(_button_iWillChoose->label(),"I will choose")) {		
			logWrite("Go offscript");
			//_box_offScript->activate();
			showOffscriptButtons(1);
			_tab_betweenScripts->activate();
			_button_doItNow->deactivate();
			_button_iWillChoose->label("Back to script");

			_box_actionTime->label("");
			timerActionStop();
			_progress_Timer->label("Please select off script action or return to next script step");

			//Send offScript command to SMS
			vector<string> iWillChooseData;
			stringstream ss1, ss2;
			ss1 << _ORIGIN_THERAPIST_;
			iWillChooseData.push_back(ss1.str());
			ss2 << _ACTION_GO_OFF_SCRIPT_;
			iWillChooseData.push_back(ss2.str());
			// Turn ON RAS subsystem
			//_yarpInterface->sendToTherapistCommand(_checkButton_enableRAS_text[0],1);
			//_yarpInterface->sendToTherapistCommand(_checkButton_enableRAS_text[1],1);
			 /*?????*/
			//_checkButton_enableRAS_value[0] = 1;
			//_checkButton_enableRAS_value[1] = 1;
           // _checkButton_enableRAS[0]->value(1);
            //_checkButton_enableRAS[1]->value(1);
			//Fl::check();

			_yarpInterface->sendToSelectedBySupervisor(iWillChooseData);
			
           

		}

		else if (!strcmp(_button_iWillChoose->label(),"Back to script")) {		
			logWrite("Back to script");
			//_box_offScript->deactivate();
			showOffscriptButtons(0);
			_tab_betweenScripts->deactivate();
			//_button_doItNow->deactivate();
			// fix bug
			_button_iWillChoose->value(0);
			_button_iWillChoose_value = 0;			
			//_button_iWillChoose->deactivate();// test IWC
			

			_button_iWillChoose->label("I will choose");
			_progress_Timer->label("");
			//_isGoToNextStep = true;
			_isSimulationRunning = false;
			//Reset proposed action
			_proposedAction.clear();
			sprintf(_proposedActionText,"");
			_proposedActionID = _ACTION_NO_ACTION_; 

			// Send onScript command to SMS
			vector<string> backToScript;
			stringstream ss1, ss2;
			ss1 << _ORIGIN_THERAPIST_;
			backToScript.push_back(ss1.str());
			ss2 << _ACTION_BACK_ON_SCRIPT_;
			backToScript.push_back(ss2.str());
			_yarpInterface->sendToSelectedBySupervisor(backToScript);
			// Send go next step
			Sleep(10); // 10ms
			vector<string> goNextStep;
			stringstream ss3, ss4;
			ss3 << _ORIGIN_THERAPIST_;
			goNextStep.push_back(ss3.str());
			ss4 << _ACTION_GO_NEXT_STEP_;
			goNextStep.push_back(ss4.str());
			_yarpInterface->sendToSelectedBySupervisor(goNextStep);

			//goToNextStep(); // this is called by the GUI loop anyway
			// Turn OFF RAS subsystem
			//_checkButton_enableRAS_value[0] = 0;
			//_yarpInterface->sendToTherapistCommand(_checkButton_enableRAS_text[0],0);
            //_yarpInterface->sendToTherapistCommand(_checkButton_enableRAS_text[1],0);
			/*?????*/
            //_checkButton_enableRAS[0]->value(0);
            //_checkButton_enableRAS[1]->value(0);
			//Fl::check();			

			// Send to delib - interventionCommand
			goToNextScriptStep();            
		}

	}
	// Update old value
	_button_iWillChoose_value_old = _button_iWillChoose_value;	
}

void SystemGUIController::buttons_offScript_Clicked(){
	// Check if the button is clicked
	for (unsigned int i =0; i< _buttons_offScript_fixed_value.size(); i++) {
		if ((_buttons_offScript_fixed_value[i] == TRUE) && (_buttons_offScript_fixed_value_old[i]!=_buttons_offScript_fixed_value[i])) {
			if (_buttons_offScript_fixed_text[i] == "previous step"){
				string message = "Offscript action: " + _buttons_offScript_fixed_text[i];
				logWrite(message);
				sprintf(_progress_Timer_label, "Go to previous step");
				if (_sessionPartner == "Robot"){
					_progress_Timer->label(_progress_Timer_label);
					_currentStep--;
					if (_currentStep <0) _currentStep = 0;
					// Update list script
					update_list_scriptSteps(_currentStep);
					cout <<"Current step (GUI): " <<_currentStep <<endl;
				}
				
				else if (_sessionPartner == "Therapist"){
					_isGoToNextStep = true;
					_backFromStepMod = true;
					_currentStep = _currentStep -2; // +1 in goNextStep
					if (_currentStep <-2) _currentStep = -2;
					vector<string> message;
					stringstream ss1, ss2, ss3;
					ss1 << _ORIGIN_THERAPIST_;
					message.push_back(ss1.str());
					ss2 << _ACTION_CHANGE_STEP_;
					message.push_back(ss2.str());
					ss3 << _currentStep;
					message.push_back(ss3.str());
					_yarpInterface->sendToSelectedBySupervisor(message);
				}
				// Send to SMS
				_yarpInterface->sendToTherapistCommand(_ACTION_STEPMOD_,_ACTION_PREVIOUS_);	

			}
			else if (_buttons_offScript_fixed_text[i] == "next step"){
				string message = "Offscript action: " + _buttons_offScript_fixed_text[i];
				logWrite(message);
				sprintf(_progress_Timer_label, "Go to next step");
				if (_sessionPartner == "Robot"){
					_progress_Timer->label(_progress_Timer_label);
					_currentStep++;
					if (_currentStep > (int)_currentScriptSteps.size()) _currentStep =  _currentScriptSteps.size();
					// Update list script
					update_list_scriptSteps(_currentStep);
					cout <<"Current step (GUI): " <<_currentStep <<endl;
				}
				else if (_sessionPartner == "Therapist"){
					_isGoToNextStep = true;
					_backFromStepMod = true;
					vector<string> message;
					stringstream ss1, ss2, ss3;
					ss1 << _ORIGIN_THERAPIST_;
					message.push_back(ss1.str());
					ss2 << _ACTION_CHANGE_STEP_;
					message.push_back(ss2.str());
					ss3 << _currentStep;
					message.push_back(ss3.str());
					_yarpInterface->sendToSelectedBySupervisor(message);					
				}
				// Send to SMS
				_yarpInterface->sendToTherapistCommand(_ACTION_STEPMOD_,_ACTION_NEXT_);				
			}
			else {
				string message = "Offscript action: " + _buttons_offScript_fixed_action[i];
				logWrite(message);
				// Do things here
				vector<string> offscriptAction;
				int offscriptActionID;
				char offscriptActionText[STRINGLENGTH];
				processOffscriptAction(_buttons_offScript_fixed_action[i], offscriptAction, offscriptActionText, offscriptActionID);
				
				// Send action to the robot
				_yarpInterface->sendToSelectedBySupervisor(offscriptAction);
				// HLC: Do not need to wait anymore
				sprintf(_progress_Timer_label, offscriptActionText);
				if (_sessionPartner == "Robot") _progress_Timer->label(_progress_Timer_label);
			}
			// Release button
			_buttons_offScript_fixed[i]->value(0); // fix bug
			_buttons_offScript_fixed_value[i] = 0;
		}
		// Update old value
		_buttons_offScript_fixed_value_old[i]=_buttons_offScript_fixed_value[i];
	}
	// Check if the button is clicked
	for (unsigned int i =0; i< _buttons_offScript_alwaysOn_value.size(); i++) {
		if ((_buttons_offScript_alwaysOn_value[i] == TRUE) && (_buttons_offScript_alwaysOn_value_old[i]!=_buttons_offScript_alwaysOn_value[i])) {
			
			if ( (_buttons_offScript_alwaysOn_text[i] == "Bad perf") || (_buttons_offScript_alwaysOn_text[i] == "Good perf")){
				string message = "Offscript action: " + _buttons_offScript_alwaysOn_text[i];
				logWrite(message);
				// Do things here
				vector<string> offscriptAction;
				int offscriptActionID;
				char offscriptActionText[STRINGLENGTH];
				processOffscriptAction(_buttons_offScript_alwaysOn_action[i], offscriptAction, offscriptActionText, offscriptActionID);

				// Send action to the robot
				_yarpInterface->sendToSelectedBySupervisor(offscriptAction);
				// HLC: Do not need to wait anymore
				sprintf(_progress_Timer_label, offscriptActionText);
				if (_sessionPartner == "Robot") _progress_Timer->label(_progress_Timer_label);
				//Don't need to check by port
				if (_waitingForPerf) {
					_isGoToNextStep = true;
					_waitingForPerf = false;
					_backFromWaitingForGazePerf = true;
				}
				
			}

			else if (_buttons_offScript_alwaysOn_text[i] == "Gaze") {
				string message = "Offscript action: " + _buttons_offScript_alwaysOn_text[i];
				logWrite(message);
				// Do things here
				vector<string> offscriptAction;
				int offscriptActionID;
				char offscriptActionText[STRINGLENGTH];
				processOffscriptAction(_buttons_offScript_alwaysOn_action[i], offscriptAction, offscriptActionText, offscriptActionID);

				// If at gaze request step - cancel timer
				if (_currentActionID==_ACTION_REQUEST_GAZE_){
					_isStopTimerAction = true;// stop timer
					_box_actionTime->label("");
					_box_onScript->label("");
					_progress_Timer->label("");
					// Reset current action variables
					_currentAction.clear();
					sprintf(_currentPerformActionText,"");
					_currentActionID = _ACTION_NO_ACTION_; 	
					_isSimulationRunning = false;
				}


				// Send action to the robot
				_yarpInterface->sendToSelectedBySupervisor(offscriptAction);
				// HLC: Do not need to wait anymore
				sprintf(_progress_Timer_label, offscriptActionText);
				if (_sessionPartner == "Robot") _progress_Timer->label(_progress_Timer_label);
				// Dont need to check by port
				
				if (_waitingForGaze) {
					_isGoToNextStep = true;
					_waitingForGaze = false;
					_backFromWaitingForGazePerf = true;
				}				
			}			
			else {
				string message = "Offscript action: " + _buttons_offScript_alwaysOn_text[i];
				logWrite(message);
				// Do things here
				vector<string> offscriptAction;
				int offscriptActionID;
				char offscriptActionText[STRINGLENGTH];
				processOffscriptAction(_buttons_offScript_alwaysOn_action[i], offscriptAction, offscriptActionText, offscriptActionID);

				// Send action to the robot
				_yarpInterface->sendToSelectedBySupervisor(offscriptAction);
				// HLC: Do not need to wait anymore
				sprintf(_progress_Timer_label, offscriptActionText);
				if (_sessionPartner == "Robot") _progress_Timer->label(_progress_Timer_label);
			}
			
			// Release button
			_buttons_offScript_alwaysOn[i]->value(0); // fix bug
			_buttons_offScript_alwaysOn_value[i] = 0;
		}
		// Update old value
		_buttons_offScript_alwaysOn_value_old[i]=_buttons_offScript_alwaysOn_value[i];
	}
	// Check if the button is clicked
	for (unsigned int i =0; i< _buttons_offScript_feedback_value.size(); i++) {
		if ((_buttons_offScript_feedback_value[i] == TRUE) && (_buttons_offScript_feedback_value_old[i]!=_buttons_offScript_feedback_value[i])) {
			string message = "Offscript action: " + _buttons_offScript_feedback_text[i];
			logWrite(message);
			// Do things here
			vector<string> offscriptAction;
			int offscriptActionID;
			char offscriptActionText[STRINGLENGTH];
			processOffscriptAction(_buttons_offScript_feedback_action[i], offscriptAction, offscriptActionText, offscriptActionID);

			// Send action to the robot
			_yarpInterface->sendToSelectedBySupervisor(offscriptAction);
			// HLC: Do not need to wait anymore
			sprintf(_progress_Timer_label, offscriptActionText);
			if (_sessionPartner == "Robot") _progress_Timer->label(_progress_Timer_label);
			
			// Release button
			_buttons_offScript_feedback[i]->value(0); // fix bug
			_buttons_offScript_feedback_value[i] = 0;
		}
		// Update old value
		_buttons_offScript_feedback_value_old[i]=_buttons_offScript_feedback_value[i];
	}
	// Check if the button is clicked
	for (unsigned int i =0; i< _buttons_offScript_scenario_value.size(); i++) {
		if ((_buttons_offScript_scenario_value[i] == TRUE) && (_buttons_offScript_scenario_value_old[i]!=_buttons_offScript_scenario_value[i])) {
			string message = "Offscript action: " + _buttons_offScript_scenario_text[i];			
			logWrite(message);
			// Do things here
			vector<string> offscriptAction;
			int offscriptActionID;
			char offscriptActionText[STRINGLENGTH];
			processOffscriptAction(_buttons_offScript_scenario_action[i], offscriptAction, offscriptActionText, offscriptActionID);
			// Send action to the robot
			_yarpInterface->sendToSelectedBySupervisor(offscriptAction);

			// HLC: Do not need to wait anymore
			sprintf(_progress_Timer_label, offscriptActionText);
			if (_sessionPartner == "Robot") _progress_Timer->label(_progress_Timer_label);
			// Check if feedback is corrected in therapist mode
			
			// For Therapist and Diagnosis mode
			switch (offscriptActionID){
				case _ACTION_FEEDBACK_IMITATION_MOVE_GOOD_:
				case _ACTION_FEEDBACK_IMITATION_OBJECT_GOOD_:
				case _ACTION_FEEDBACK_TT_WAITING_GOOD_:
				case _ACTION_FEEDBACK_TT_SORTING_GOOD_:
				case _ACTION_FEEDBACK_TT_CHOOSING_GOOD_:
				case _ACTION_FEEDBACK_TT_PATTERN_GOOD_:
				case _ACTION_FEEDBACK_JA_GOOD_:					
				case _ACTION_FEEDBACK_IMITATION_MOVE_BAD_:
				case _ACTION_FEEDBACK_IMITATION_OBJECT_BAD_:
				case _ACTION_FEEDBACK_TT_WAITING_BAD_:
				case _ACTION_FEEDBACK_LAST_TT_WAITING_BAD_:
				case _ACTION_FEEDBACK_TT_SORTING_BAD_:
				case _ACTION_FEEDBACK_TT_CHOOSING_BAD_:
				case _ACTION_FEEDBACK_TT_PATTERN_BAD_:
				case _ACTION_FEEDBACK_JA_BAD_:
				case _ACTION_FEEDBACK_BAD_NO_ACTION_:{
					_feedbackCorrected = true;
					break;}
			}						
			
			// Release button
			_buttons_offScript_scenario[i]->value(0); // fix bug
			_buttons_offScript_scenario_value[i] = 0;
		}
		// Update old value
		_buttons_offScript_scenario_value_old[i]=_buttons_offScript_scenario_value[i];
	}
}
void SystemGUIController::buttons_betweenScripts_Clicked(){
	// Check if the button is clicked
	for (unsigned int i =0; i< _buttons_betweenScripts_value.size(); i++) {
		if ((_buttons_betweenScripts_value[i] == TRUE) && (_buttons_betweenScripts_value_old[i]!=_buttons_betweenScripts_value[i])) {
			logWrite("Between Scripts: Button " + _buttons_betweenScripts_text[i] + " clicked!");
			// Do things here
			vector<string> betweenScriptAction;
			int betweenScriptActionID;
			char betweenScriptActionText[STRINGLENGTH];
			processOffscriptAction(_buttons_betweenScripts_action[i], betweenScriptAction, betweenScriptActionText, betweenScriptActionID);

			// Send action to the robot
			_yarpInterface->sendToSelectedBySupervisor(betweenScriptAction);
			// Release button
			//_buttons_betweenScripts[i]->value(0); // fix bug
			//_buttons_betweenScripts_value[i] = 0;
		}
		// Update old value
		_buttons_betweenScripts_value_old[i]=_buttons_betweenScripts_value[i];
	}

	for (unsigned int i =0; i< _buttons_enableRobot.size(); i++) {
		if ((_buttons_enableRobot_value[i] == TRUE) && (_buttons_enableRobot_value_old[i]!=_buttons_enableRobot_value[i])) {
			logWrite("Between Scripts: Button " + _buttons_enableRobot_text[i] + " clicked!");
			// Do things here
			vector<string> betweenScriptAction;
			int betweenScriptActionID;
			char betweenScriptActionText[STRINGLENGTH];
			processOffscriptAction(_buttons_enableRobot_action[i], betweenScriptAction, betweenScriptActionText, betweenScriptActionID);

			// Send action to the robot
			_yarpInterface->sendToSelectedBySupervisor(betweenScriptAction);
			// Release button
			//_buttons_betweenScripts[i]->value(0); // fix bug
			//_buttons_betweenScripts_value[i] = 0;		
		}
		// Update old value
		_buttons_enableRobot_value_old[i]=_buttons_enableRobot_value[i];
	}

	// Check if the button is clicked
	if ((_button_say_value == TRUE) && (_button_say_value_old!=_button_say_value)) {
		logWrite("Between Scripts: Say " + string(_textInput_say_value));
		// Do things here
			vector<string> betweenScriptAction;
			stringstream ss;
			ss  << _ORIGIN_THERAPIST_ << "," << _ACTION_SAY_ << "," << string(_textInput_say_value);
			betweenScriptAction = split(ss.str(), ',');
			// Send action to the robot
			_yarpInterface->sendToSelectedBySupervisor(betweenScriptAction);
	}
	// Update old value
	_button_say_value_old = _button_say_value;
}

void SystemGUIController::checkButton_enableRAS_Changed(){
	for (unsigned int i=0; i<_checkButton_enableRAS.size();i++){
		if (_checkButton_enableRAS_value_old[i] != _checkButton_enableRAS_value[i]){
			if (_checkButton_enableRAS_value[i] == TRUE){
				logWrite("Enable " + _checkButton_enableRAS_text[i]);
                //Worst solution ever but just for integration purposes as I need both system to sync
                _yarpInterface->sendToTherapistCommand(_checkButton_enableRAS_text[i],1);
               // _yarpInterface->sendToTherapistCommand(_checkButton_enableRAS_text[1],1);
				//_yarpInterface->sendToTherapistCommand(_checkButton_enableRAS_text[i],1);
                /*?????*/
				_checkButton_enableRAS_value[0] = 1;
				_checkButton_enableRAS_value[1] = 1;
                _checkButton_enableRAS[0]->value(1);
                _checkButton_enableRAS[1]->value(1);
				Fl::check();
			}
			else {
				logWrite("Disable " + _checkButton_enableRAS_text[i]);
				//Worst solution ever but just for integration purposes as I need both system to sync
                _yarpInterface->sendToTherapistCommand(_checkButton_enableRAS_text[i],0);
                //_yarpInterface->sendToTherapistCommand(_checkButton_enableRAS_text[1],0);
				//_yarpInterface->sendToTherapistCommand(_checkButton_enableRAS_text[i],0);
                /*?????*/
				_checkButton_enableRAS_value[0] = 0;
				_checkButton_enableRAS_value[1] = 0;
                _checkButton_enableRAS[0]->value(0);
                _checkButton_enableRAS[1]->value(0);
				Fl::check();
			}
		}
		_checkButton_enableRAS_value_old[i] = _checkButton_enableRAS_value[i];		
	}
}

void SystemGUIController::button_exportLogfile_Clicked() {
	// Check if the button is clicked
	if ((_button_exportLogfile_value == TRUE) && (_button_exportLogfile_value_old!=_button_exportLogfile_value)) {
		// Do more things here
		boost:: filesystem::create_directory(_logUrl.c_str());
		string logFilename = _logUrl + getCurrentTime(LOG) +".txt"; 
		FILE* pFile = fopen(logFilename.c_str(), "w+");
		logWrite("Export log: " + logFilename);
		fprintf(pFile,"%s\n",_list_developerInfoBuffer->text());
		fclose(pFile);
	}
	// Update old value
	_button_exportLogfile_value_old = _button_exportLogfile_value;
}

string SystemGUIController::calculateCurrentPerformance(string performance){
	string currentPerformanceString;
	long currentPerformance = 0;
    vector<int> allPerformances;
    stringstream ss(performance);
    int i;
    // Split string by commas
	while (ss >> i) {
        allPerformances.push_back(i);
        if (ss.peek() == ',')
            ss.ignore();
    }

	for (unsigned int j=0; j<allPerformances.size();j++){
		currentPerformance += allPerformances[j];
	}

	stringstream s_currentPerformance;
	s_currentPerformance << currentPerformance;

	stringstream s_allPerformances;
	s_allPerformances << allPerformances.size();

	currentPerformanceString = "Current Performance: " + s_currentPerformance.str() + "/" + s_allPerformances.str();
	cout << currentPerformanceString << endl;
	return currentPerformanceString;
}


void SystemGUIController::resizeImage(string image_url, int w, int h, Fl_PNG_Image* &image){
	Fl_PNG_Image *source = new Fl_PNG_Image(image_url.c_str());
	if (!source) {
		//return NULL;
		return;
	}
	// Resize the image if it's too big, by replacing it with a resized copy:
	if (source->w() > w || source->h() > h) {
		Fl_Image* destination;
		if (source->w() > source->h()) {
			destination = source->copy(w, h * source->h() / source->w());
		} else {
			destination = source->copy(w * source->w() / source->h(), h);
		}
		image = (Fl_PNG_Image*) destination;
	}
}

string SystemGUIController::actionTimeDisplay(double value){
	string currentPerformance = "";
	if ((value>=0)&&(value<=_timer_value)){
		stringstream ss;
		ss << _timer_value - value;
		currentPerformance = "Action will be performed automatically in " + ss.str() + " second";
		if ((_timer_value - value) != 1) currentPerformance += "s";
	}
	return currentPerformance; 
}

void SystemGUIController::logWrite(string info) {
	string tempString = "["+ getCurrentTime() +"] " + info+ "\r\n";
	_list_developerInfoBuffer->append(tempString.c_str());
	//_list_developerInfo->insert(tempString.c_str());
	//_list_developerInfo->show_insert_position();	
}

string SystemGUIController::getCurrentTime(int type){
	time_t currentTime;
	struct tm *localTime;
	time(&currentTime);                   // Get the current time
	localTime = localtime(&currentTime);  // Convert the current time to the local time
	int day    = localTime->tm_mday;
	int month  = localTime->tm_mon + 1;
	int year   = localTime->tm_year + 1900;
	int hour   = localTime->tm_hour;
	int min    = localTime->tm_min;
	int sec    = localTime->tm_sec;
	char currentTimeChar[20];
	if (type== FULL) sprintf(currentTimeChar,"%d-%02d-%02d %02d:%02d:%02d",year,month,day,hour,min,sec);
	else if (type== LOG) sprintf(currentTimeChar,"%d-%02d-%02d %02d-%02d-%02d",year,month,day,hour,min,sec);
	else if (type== DATE) sprintf(currentTimeChar,"%d-%02d-%02d",year,month,day);
	return string(currentTimeChar);
}

void SystemGUIController::timer_miliseconds(long time) {
	boost::this_thread::sleep(boost::posix_time::milliseconds(time));
}

// All Yarp functions */
void SystemGUIController::processSmsSummary(user_interaction_data_t data){
	
	_currentUserInteractionData = data;
	// Update Current Performance
	_currentPerformanceString = calculateCurrentPerformance(_currentUserInteractionData.m_sPerformance);
	sprintf(_currentPerformanceDisplay, _currentPerformanceString.c_str());
	_box_currentPerformance->label(_currentPerformanceDisplay);	
}

void SystemGUIController::processProposedToSupervisor(string data){
	// Temp values
	vector<string> proposedAction = split(data, ',');
	int proposedActionID = atoi(proposedAction[1].c_str());

	// Check connection messages
	if (proposedActionID == _ACTION_ROBOT_CONNECTED_){
		if (proposedAction[2] == "0") _isRobotConnected = false;
		else _isRobotConnected = true;
	}
	else if (proposedActionID == _ACTION_SANDTRAY_TYPE_){
		if (proposedAction[2] == "0") _isSandtrayConnected = false;
		else _isSandtrayConnected = true;
	}

	// Other messages
	if (_isScriptRunning){
		if (_sessionPartner == "Robot") {
			_proposedAction.clear();
			// Reset _proposedActionText
			sprintf(_proposedActionText,"");

			// Temp values - already defined above
			//vector<string> proposedAction = split(data, ',');

			//int proposedActionID = atoi(proposedAction[1].c_str());

			// Check if stepID is received or not -  check later with script step
			//int temp_stepID = atoi(proposedAction[0].c_str());
			//if (temp_stepID >=0) _currentStep = temp_stepID;
		
			// Check if the command is to go to the next step
			if ((_sessionPartner=="Robot")&&(proposedActionID == _ACTION_GO_NEXT_STEP_)){
				_isGoToNextStep = true;
			}		
			// Normal action messages
			else {
				// Convert action message to readable format
				translateActionMessage(proposedActionID, proposedAction, _proposedActionText);
				// Assign new values
				_proposedAction = proposedAction;
				_proposedActionID = proposedActionID;


			}
		} // end robot
		else if (_sessionPartner == "Therapist") {
			// Reset proposedAction
			_proposedAction.clear();
			// Reset _proposedActionText
			sprintf(_proposedActionText,"");
			
			// Check proposed action ID
			switch (proposedActionID){
			case _ACTION_RECEIVED_PERF_:{
				if (proposedAction[2] == "0"){
					if (_waitingForPerf) {
						_isGoToNextStep = true;
						_waitingForPerf = false;
						_backFromWaitingForGazePerf = true;
						cout << "Robot receives bad performance" << endl;
						logWrite("Robot receives bad performance by port");
						sprintf(_progress_Timer_label, "Robot receives bad performance");
						_progress_Timer->label(_progress_Timer_label);											
					}
				}
				else if (proposedAction[2] == "1"){
					if (_waitingForPerf) {
						_isGoToNextStep = true;
						_waitingForPerf = false;
						_backFromWaitingForGazePerf = true;
						cout << "Robot receives good performance" << endl;
						logWrite("Robot receives good performance by port");
						sprintf(_progress_Timer_label, "Robot receives good performance");
						_progress_Timer->label(_progress_Timer_label);
					}
				}
				break;}
			case _ACTION_RECEIVED_GAZE_:{
				if (_waitingForGaze) {
					_isGoToNextStep = true;
					_waitingForGaze = false;
					_backFromWaitingForGazePerf = true;
					cout << "Robot receives gaze" << endl;
					logWrite("Robot receives gaze by port");
					sprintf(_progress_Timer_label, "Robot receives gaze");
					_progress_Timer->label(_progress_Timer_label);
				}
				break;}
			// these are all the options for a 1 performance
			case _ACTION_FEEDBACK_IMITATION_MOVE_GOOD_:
			case _ACTION_FEEDBACK_IMITATION_OBJECT_GOOD_:
			case _ACTION_FEEDBACK_TT_WAITING_GOOD_:
			case _ACTION_FEEDBACK_TT_SORTING_GOOD_:
			case _ACTION_FEEDBACK_TT_CHOOSING_GOOD_:
			case _ACTION_FEEDBACK_TT_PATTERN_GOOD_:
			case _ACTION_FEEDBACK_JA_GOOD_:{
				cout << "System proposes good feedback" << endl;
				_box_onScript->label("Currently:");
				sprintf(_progress_Timer_label,"System proposes good feedback");
				_progress_Timer->label(_progress_Timer_label);
				_feedbackStep = true;
				_feedbackCorrected = false;
				_proposedAction = proposedAction;// to send back if needed	
				break;}
			// these are all the options for a 0 performance
			case _ACTION_FEEDBACK_IMITATION_MOVE_BAD_:
			case _ACTION_FEEDBACK_IMITATION_OBJECT_BAD_:
			case _ACTION_FEEDBACK_TT_WAITING_BAD_:
			case _ACTION_FEEDBACK_LAST_TT_WAITING_BAD_:
			case _ACTION_FEEDBACK_TT_SORTING_BAD_:
			case _ACTION_FEEDBACK_TT_CHOOSING_BAD_:
			case _ACTION_FEEDBACK_TT_PATTERN_BAD_:
			case _ACTION_FEEDBACK_JA_BAD_:
			case _ACTION_FEEDBACK_BAD_NO_ACTION_:{
				cout << "System proposes bad feedback" << endl;
				_box_onScript->label("Currently:");
				sprintf(_progress_Timer_label,"System proposes bad feedback");
				_progress_Timer->label(_progress_Timer_label);
				_feedbackStep = true;
				_feedbackCorrected = false;
				_proposedAction = proposedAction;// to send back if needed	
				break;}
			}
		}
	}
	//cout << "Proposed action is successfully processed: "<< _proposedActionID <<endl;
	cout << "Proposed action is successfully processed: "<< data <<endl;			
}


void SystemGUIController::processOffscriptAction(string data, vector<string> &offscriptAction, char* offscriptActionText,int &offscriptActionID){
	char tempOffscriptActionText[STRINGLENGTH];
	// Add THERAPIST ORIGIN
	stringstream ss;
	ss  << _ORIGIN_THERAPIST_ << "," << data;
	data = ss.str();

	// Temp values
	vector<string> rawOffScriptAction = split(data, ',');
	int tempOffscriptActionTextID = atoi(rawOffScriptAction[1].c_str());

	// Convert action message to readable format
	translateActionMessage(tempOffscriptActionTextID, rawOffScriptAction, tempOffscriptActionText);

	// Assign value
	offscriptAction = rawOffScriptAction;
	sprintf(offscriptActionText,tempOffscriptActionText);
	offscriptActionID = tempOffscriptActionTextID;
	cout << "Offscript action is successfully processed: " << offscriptActionID << endl;	
}

void SystemGUIController::translateActionMessage(int actionID, vector<string> actionMessage, char* actionMessageDisplay){
	char actionMessageText[STRINGLENGTH];
	sprintf(actionMessageText,"");

	// Max lenth for displayed message (SAY only)
	unsigned int trim_len = 80;

	switch (actionID){
	case _ACTION_GO_NEXT_STEP_:	break;						// expects 0 argument		
	case _ACTION_SAY_:										// expects 1 argument string	
		// Reduce the text length
		if (actionMessage[2].length() > trim_len)	actionMessage[2] = actionMessage[2].substr(0,trim_len) + "..."; 
		sprintf(actionMessageText,"Robot says: %s",actionMessage[2].c_str()); break;
	case _ACTION_MOVE_:										// expects 1 argument int
		sprintf(actionMessageText,"Robot moves: %s",actionMessage[2].c_str()); break;
	case _ACTION_GRAB_OBJECT_:								// expects 0 argument
		sprintf(actionMessageText,"Robot graps object"); break;
	case _ACTION_DROP_OBJECT_:								// expects 0 argument
		sprintf(actionMessageText,"Robot drops object"); break;
	case _ACTION_FEEDBACK_IMITATION_OBJECT_GOOD_:			// expects 0 argument
		sprintf(actionMessageText,"Robot gives good feedback: Imitation object"); break;
	case _ACTION_FEEDBACK_IMITATION_OBJECT_BAD_:			// expects 0 argument
		sprintf(actionMessageText,"Robot gives bad feedback: Imitation object"); break;
	case _ACTION_FEEDBACK_IMITATION_MOVE_GOOD_:				// expects 0 argument
		sprintf(actionMessageText,"Robot gives good feedback: Imitation"); break;
	case _ACTION_FEEDBACK_IMITATION_MOVE_BAD_:				// expects 0 argument
		sprintf(actionMessageText,"Robot gives bad feedback: Imitation"); break;
	case _ACTION_FEEDBACK_TT_WAITING_GOOD_:					// expects 0 argument
		sprintf(actionMessageText,"Robot gives good feedback: Turn taking Waiting"); break;
	case _ACTION_FEEDBACK_TT_WAITING_BAD_:					// expects 0 argument
		sprintf(actionMessageText, "Robot gives bad feedback: Turn taking Waiting"); break;
	case _ACTION_FEEDBACK_LAST_TT_WAITING_BAD_:					// expects 0 argument
		sprintf(actionMessageText,"Robot gives bad last feedback: Turn taking Waiting"); break;
	case _ACTION_FEEDBACK_TT_SORTING_GOOD_:					// expects 0 argument
		sprintf(actionMessageText,"Robot gives good feedback: Turn taking Sorting"); break;
	case _ACTION_FEEDBACK_TT_SORTING_BAD_:					// expects 0 argument
		sprintf(actionMessageText,"Robot gives bad feedback: Turn taking Sorting"); break;
	case _ACTION_FEEDBACK_TT_CHOOSING_GOOD_:				// expects 0 argument
		sprintf(actionMessageText,"Robot gives good feedback: Turn taking Choosing"); break;
	case _ACTION_FEEDBACK_TT_CHOOSING_BAD_:					// expects 0 argument
		sprintf(actionMessageText,"Robot gives bad feedback: Turn taking Choosing"); break;
	case _ACTION_FEEDBACK_TT_PATTERN_GOOD_:					// expects 0 argument
		sprintf(actionMessageText,"Robot gives good feedback: Turn taking Pattern"); break;
	case _ACTION_FEEDBACK_TT_PATTERN_BAD_:					// expects 0 argument
		sprintf(actionMessageText,"Robot gives bad feedback: Turn taking Pattern"); break;
	case _ACTION_FEEDBACK_BAD_NO_ACTION_ :
		sprintf(actionMessageText,"Robot gives bad feedback no action"); break;
	case _ACTION_FEEDBACK_JA_GOOD_:							// expects 0 argument
		sprintf(actionMessageText,"Robot gives good feedback: Joint attention"); break;
	case _ACTION_FEEDBACK_JA_BAD_:							// expects 0 argument
		sprintf(actionMessageText,"Robot gives bad feedback: Joint attention"); break;
	case _ACTION_ROBOT_GOOD_MOVE_:							// expects 0 argument
		sprintf(actionMessageText,"Robot good move"); break;
	case _ACTION_POINT_IMAGE_ :								// expects 1 argument int id
		sprintf(actionMessageText,"Robot points at image: %s",actionMessage[2].c_str()); break;
	case _ACTION_LOOK_POINT_SAY_:							// expects 1 argument int id (0 left/1 right)
		if (actionMessage[2]=="0") sprintf(actionMessageText,"Robot looks, points, and says: Left");
		if (actionMessage[2]=="1") sprintf(actionMessageText,"Robot looks, points, and says: Right"); break;
	case _ACTION_LOOK_POINT_:								// expects 1 argument int id (0 left/1 right)
		if (actionMessage[2]=="0") sprintf(actionMessageText,"Robot looks and points: Left");
		if (actionMessage[2]=="1") sprintf(actionMessageText,"Robot looks and points: Right"); break;
	case _ACTION_LOOK_:										// expects 1 argument int id (0 left/1 right)
		if (actionMessage[2]=="0") sprintf(actionMessageText,"Robot looks: Left");
		if (actionMessage[2]=="1") sprintf(actionMessageText,"Robot looks: Right");
		if (actionMessage[2]=="2") sprintf(actionMessageText,"Robot looks: Straight"); break;
	case _ACTION_MOVE_HEAD_:								// expects 4 arguments float x, y, z, speed
		// still need to define which arguments to be displayed
		sprintf(actionMessageText,"Robot moves head"); break;
	case _ACTION_MOVE_HAND_:								// expects 5 arguments bool rightArm float x, y, z, roll
		// still need to define which arguments to be displayed
		sprintf(actionMessageText,"Robot moves hand"); break;
	case _ACTION_ENABLE_: break;	
		sprintf(actionMessageText,"Action enable"); break;
	case _ACTION_DISABLE_: break;
		sprintf(actionMessageText,"Action disable"); break;
	case _ACTION_LOOK_AT_:
		// still need to define which arguments to be displayed
		sprintf(actionMessageText,"Robot looks at..."); break;
	case _ACTION_STAND_:
		sprintf(actionMessageText, "Robot stands"); break;
	case _ACTION_WAITING_FOR_PERF_: break;
		sprintf(actionMessageText,"Robot is waiting for performance"); break;
	case _ACTION_WAITING_FOR_GAZE_: break;
		sprintf(actionMessageText,"Robot is waiting for mutual gaze"); break;
	case _ACTION_POINT_AT_:								// expects 4*n (+1/2 args) (should not be used as such - subaction of robot good move)
		// still need to define which arguments to be displayed
		sprintf(actionMessageText,"Robot points at..."); break;
	case _ACTION_CLOSE_HAND_:								// expects 1 argument bool rightArm
		if (actionMessage[2]=="0") sprintf(actionMessageText,"Robot closes hand: Left");
		if (actionMessage[2]=="1") sprintf(actionMessageText,"Robot closes hand: Right"); break;
	case _ACTION_OPEN_HAND_:
		if (actionMessage[2]=="0") sprintf(actionMessageText,"Robot opens hand: Left");
		if (actionMessage[2]=="1") sprintf(actionMessageText,"Robot opens hand: Right"); break;
	case _ACTION_REQUEST_GAZE_:{
		string temp_string = "Robot says: " + _STRING_GAZE_REQUEST_;
		sprintf(actionMessageText,temp_string.c_str());
		break;}
	case _ACTION_FINISHED_:	 break;							// expects nothing, we are done
		sprintf(actionMessageText,"Action finished"); break;
	case _ACTION_NO_ACTION_: break;
	case _ACTION_GO_OFF_SCRIPT_: break;
	case _ACTION_BACK_ON_SCRIPT_: break;
	case _ACTION_RECEIVED_PERF_:
		if (actionMessage[2]=="0") sprintf(actionMessageText,"Robot receives bad performance");
		if (actionMessage[2]=="1") sprintf(actionMessageText,"Robot receives good performance"); break;
	case _ACTION_RECEIVED_GAZE_:							// expects 1 argument float
		sprintf(actionMessageText,"Robot receives gaze"); break;	
	case _ACTION_CORRUPTED_: break;

	case _ACTION_SANDTRAY_LOAD_LIBRARY_: 
		sprintf(actionMessageText,"Load sandtray library: %s",actionMessage[2].c_str()); break;	// expects 1 argument int, library to load
	case _ACTION_SANDTRAY_ENABLE_FEEDBACK_: 				// expects 1 argument bool
		if (actionMessage[2]=="0") sprintf(actionMessageText,"Disable feedback");
		if (actionMessage[2]=="1") sprintf(actionMessageText,"Enable feedback"); break;
	case _ACTION_SANDTRAY_ENABLE_TOUCH_:					// expects 1 argument bool
		if (actionMessage[2]=="0") sprintf(actionMessageText,"Disable touch");
		if (actionMessage[2]=="1") sprintf(actionMessageText,"Enable touch"); break;
	case _ACTION_SANDTRAY_ONE_BY_ONE_:					// expects 1 argument bool
		sprintf(actionMessageText,"Set images one by one"); break;
	case _ACTION_EMERGENCY_EXIT_:					// expects 0 argument
		sprintf(actionMessageText,"Emergency!!"); break;
	}
	sprintf(actionMessageDisplay,actionMessageText);
}

// Check if an action is in the zero delay actions list
bool SystemGUIController::isZeroDelayAction(int actionID){
	for (unsigned int i = 0; i< (sizeof(_ZERO_DELAY_ACTIONS_)/sizeof(*_ZERO_DELAY_ACTIONS_)); i++	) {
		if (actionID==_ZERO_DELAY_ACTIONS_[i]) return true;
	}
	return false;
}

// Check if an move action is in the zero delay move actions list
bool SystemGUIController::isZeroDelayMoveAction(int moveID){
	for (unsigned int i = 0; i< (sizeof(_ZERO_DELAY_MOVE_ACTIONS_)/sizeof(*_ZERO_DELAY_MOVE_ACTIONS_)); i++	) {
		if (moveID==_ZERO_DELAY_MOVE_ACTIONS_[i]) return true;
	}
	return false;
}

// Check if load script is changed
void SystemGUIController::checkLoadScriptChanged(){
	if (_script!=_script_old){
		_script_choice_value = _choice_script->value();
		_scriptID = _script_list_library.m_scripts[_script_choice_value-1].m_id;		// -1  Please select a script
		_scriptTypeID = _script_list_library.m_scripts[_script_choice_value-1].m_type_id; // -1  Please select a script
		//cout << _script <<" "<< _scriptID<< " " <<_scriptTypeID << endl;
		/*
		// Scripts with sandtray
		if ((_scriptTypeID==0)||(_scriptTypeID==2)||(_scriptTypeID==4)){
			_button_loadScript->deactivate(); // to be implemented later
			//_button_loadScript->activate(); // to be implemented later
			//connectionMessage("Please connect sandtray and/or robot",_WARNING_);
		}
		else {
			_button_loadScript->activate();
		}
		*/
	}
	_script_old = _script;
}

// Check connection message
void SystemGUIController::checkConnections(void){
	stringstream ss;
	if (_isRobotConnected) ss << "Robot is connected. ";
	else ss << "Robot is not connected. ";

	if (_isSandtrayConnected) ss << "Sandtray is connected.";
	else ss << "Sandtray is not connected.";

	Fl_Color messageColor;
	if (_isRobotConnected&&_isSandtrayConnected) messageColor = FL_BLUE;
	else messageColor = FL_RED;
	// Display
	connectionMessage(ss.str(), messageColor);
}

// Update connection message
void SystemGUIController::connectionMessage(string message, Fl_Color color){
	sprintf(_connectionMessage, message.c_str());
	_box_connectionMessage->label(_connectionMessage);
	_box_connectionMessage->labelcolor(color);
}

string SystemGUIController::waitingArgument(vector<string> list) {
	string toReturn;
	cout << "in arg addition" << endl;
	switch (atoi(list[2].c_str())) {
	case _PERF_NO_CHECK_:
		toReturn = "Nothing";
		break;
	case _PERF_NO_BEHAVIOUR_:
		toReturn = "Nothing";
		break;
	case _PERF_SANDTRAY_GOOD_:
		toReturn = "Good Move";
		break;
	case _PERF_SANDTRAY_TOUCH_:
		toReturn = "Touch an Image";
		break;
	case _PERF_SANDTRAY_NO_TOUCH_:
		toReturn = "Don't touch sandtray";
		break;
	case _PERF_TOUCH_ROBOT_OWNED_:
		toReturn = "Touches robot's image";
		break;
	case _PERF_LOOK_RIGHT_:
		toReturn = "Look right";
		break;
	case _PERF_LOOK_LEFT_:
		toReturn = "Look left";
		break;
	case _PERF_POINT_LEFT_:
		toReturn = "Poit Left";
		break;
	case _PERF_POINT_RIGHT_:
		toReturn = "Point Right";
		break;
	case _PERF_NO_MOVEMENT_:
		toReturn = "Don't move";
		break;
	case _PERF_SPEAK_:
		toReturn = "Speak";
		break;
	case _PERF_WAVE_:
		toReturn = "Wave";
		break;
	case _PERF_COVER_:
		toReturn = "Cover Eyes";
		break;
	case _PERF_HANDS_ON_HEAD_:
		toReturn = "Hand on head";
		break;
	case _PERF_FLY_:
		toReturn = "Fly";
		break;
	case _PERF_DRIVE_:
		toReturn = "Drive/Plane";
		break;
	case _PERF_DRINK_SMELL_:
		toReturn = "Drink/Smell";
		break;
	case _PERF_TRAJ_1_:
		toReturn = "Traj 1";
		break;
	case _PERF_TRAJ_2_:
		toReturn = "Traj 2";
		break;
	case _PERF_TRAJ_3_:
		toReturn = "Traj 3";
		break;
	case _PERF_TRAJ_4_:
		toReturn = "Traj 4";
		break;
	case _PERF_KNOCKING_:
		toReturn = "Knocking";
		break;
	default: break;
	}
	if (list.size() == 6) {
		switch (atoi(list[5].c_str())) {
		case _WAITING_PERF_STANDARD_: break;
		case _WAITING_PERF_REQUEST_: toReturn += " - init"; break;
		case _WAITING_PERF_BLOCKING_: toReturn += " - wait";  break;
		default: break;
		}
	}
	return toReturn;
}

void SystemGUIController::sendFirstScriptStep() {
	cout << "ScriptManager - sendScriptFirst" << endl;
	string sCurrentAction;
	stringstream ss;
	ss << _SCRIPT_TYPE_;											// put in script type to step_id
	ss << ",";
	ss << _SCRIPT_TYPE_;											// put in script type to delib_id
	ss << ",";
	ss << _scriptTypeID;
	sCurrentAction = ss.str();
	
	vector<string> sSplitStep = split(sCurrentAction, ',');			// split into vector of strings (should be 2+ elements for a step)
	_yarpInterface->sendToInterventionCommand(sSplitStep);			// output script action to delib	
}

void SystemGUIController::goToNextScriptStep() {
	//_currentStep++;											// increment script step
	//_currentStep++;

	string sCurrentAction;

	cout << "current step:" << _currentStep << endl;

	if (_sessionPartner == "Robot"){
		if (_currentStep >= int(_currentScriptSteps.size())) {
			cout << "<<<scriptManager>>> script complete" << endl;
			stringstream ss;
			ss << _SCRIPT_FINISHED_;
			sCurrentAction = ss.str();									// send script finished message
		}
		else {
			sCurrentAction = _currentScriptSteps_raw[_currentStep];		// read in script step
		}
	
		vector<string> sSplitStep = split(sCurrentAction, ',');			// split into vector of strings (should be 2+ elements for a step)
		_yarpInterface->sendToInterventionCommand(sSplitStep);			// output script action to delib
	}

	else if (_sessionPartner == "Therapist"){
		if ( (_currentStep+1) >= int(_currentScriptSteps.size())) { // check later if need to +1 or not
			cout << "<<<scriptManager>>> script complete" << endl;
			stringstream ss;
			ss << _SCRIPT_FINISHED_;
			sCurrentAction = ss.str();									// send script finished message
		}
		else {
			sCurrentAction = _currentScriptSteps_raw[_currentStep+1];		// read in script step
		}
	
		vector<string> sSplitStep = split(sCurrentAction, ',');			// split into vector of strings (should be 2+ elements for a step)
		_yarpInterface->sendToInterventionCommand(sSplitStep);			// output script action to delib
	}
}

// Callback when user name changed
void userName_change_cb (Fl_Widget *b, void *destination) {
	Fl_Choice *userName_selection;
	userName_selection = (Fl_Choice *) b;
	*(string *)destination = userName_selection->text();
	//cout << userName_selection->text() <<endl;
}

// Callback when session partner changed
void sessionPartner_change_cb (Fl_Widget *b, void *destination) {
	Fl_Choice *sessionPartner_selection;
	sessionPartner_selection = (Fl_Choice *) b;
	*(string *)destination = sessionPartner_selection->text();
	//cout << userName_selection->text() <<endl;
}

// Callback when script type changed
void scriptType_change_cb (Fl_Widget *b, void *destination) {
	Fl_Choice *scriptType_selection;
	scriptType_selection = (Fl_Choice *) b;
	*(string *)destination = scriptType_selection->text();
	//cout << userName_selection->text() <<endl;
}

// Callback when script changed
void script_change_cb (Fl_Widget *b, void *destination) {
	Fl_Choice *script_selection;
	script_selection = (Fl_Choice *) b;
	*(string *)destination = script_selection->text();	
}
// Callback when text input changed
void get_input_cb(Fl_Widget *b, void *input) {
	Fl_Input *f;
	f = (Fl_Input *)b;
	strcpy((char *)input, f->value());
}
