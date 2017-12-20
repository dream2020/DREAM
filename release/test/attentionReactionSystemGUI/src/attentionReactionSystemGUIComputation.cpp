/* 
 * Copyright (C) 2015 DREAM Consortium
 * FP7 Project 611391 co-funded by the European Commission
 *
 * Author:  Pablo Gomez, Vrije Universiteit Brussel
 * Email:   pablo.gomez.esteban@vub.ac.be
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
 * 13/08/15  First version validated (Pablo Gomez)
 */ 
 
#include "attentionReactionSystemGUI.h"

AttentionReactionSystemGUIThread::AttentionReactionSystemGUIThread(                                        
                                                         BufferedPort<Bottle>			    *recognizeSpeechOut,   
                                                         BufferedPort<VectorOf<int>>        *checkMutualGazeOut, 
                                                         BufferedPort<Bottle>               *affectiveStateOut,
														 BufferedPort<Bottle>               *eyeBlinkingIn,
														 BufferedPort<VectorOf<int>>        *fallingInterruptionIn,
                                                         BufferedPort<VectorOf<int>>	    *fallingReactionSpeechIn,
                                                         BufferedPort<Bottle>			    *moveSequenceIn,
                                                         BufferedPort<Bottle>			    *sayIn,
                                                         BufferedPort<VectorOf<double>>     *getFacesOut,
                                                         BufferedPort<VectorOf<double>>     *getSoundDirectionOut,
                                                         BufferedPort<VectorOf<int>>		*identifyFaceExpressionOut, 
                                                         BufferedPort<Bottle>               *robotSensorsOut,                                                         
                                                         BufferedPort<VectorOf<double>>     *attentionBiasOut, 
                                                         BufferedPort<VectorOf<double>>     *therapistGazeCommandOut,
                                                         BufferedPort<VectorOf<int>>        *attentionSwitchOffOut,
                                                         BufferedPort<VectorOf<int>>        *reactiveSwitchOffOut,
														 string logoFilename)
{

   recognizeSpeechPortOut                   = recognizeSpeechOut;
   checkMutualGazePortOut                   = checkMutualGazeOut;
   identifyFaceExpressionPortOut            = identifyFaceExpressionOut;   
   affectiveStatePortOut                    = affectiveStateOut;
   robotSensorsPortOut                        = robotSensorsOut;
   eyeBlinkingPortIn                     = eyeBlinkingIn;  
   reactiveSwitchOffPortOut                 = reactiveSwitchOffOut;  
   fallingInterruptionPortIn                    = fallingInterruptionIn;
   fallingReactionSpeechPortIn           = fallingReactionSpeechIn;
   moveSequencePortIn                       = moveSequenceIn;
   sayPortIn                                = sayIn;
   getFacesPortOut                          = getFacesOut;
   getSoundDirectionPortOut                 = getSoundDirectionOut;
   attentionBiasPortOut                     = attentionBiasOut;
   therapistGazeCommandPortOut              = therapistGazeCommandOut;
   attentionSwitchOffPortOut                = attentionSwitchOffOut;
   logoFilenameValue                        = logoFilename;
}

bool AttentionReactionSystemGUIThread::threadInit() 
{
   /* initialize variables and create data-structures if needed */

   debug = false;

   return true;
}

void AttentionReactionSystemGUIThread::run(){

    /* 
    * Here we set up the GUI for the system architecture
    * 
    */ 

    int p;
    double x,y,z;
   
    Fl_Group *start_systemArchitecture_GUI;
   
    Fl_Box *heading;
    Fl_PNG_Image *logo=new Fl_PNG_Image(logoFilenameValue.c_str());
 
    Bottle command;
    VectorOf<double>  *tempDoubleVector;
    VectorOf<int>     *tempIntVector;
    Bottle            *tempBottle;  
    Bottle            *tempBottleAux;  
    char modifiedString[STRINGLENGTH]; 
    string auxString;

	/* variable that determine the presentation of the GUI */ 

    int attention_panel_x;
    int attention_panel_y;
    int attention_panel_width;
    int attention_panel_height;

    int reactive_panel_x;
    int reactive_panel_y;
    int reactive_panel_width;
    int reactive_panel_height;
   
    int heading_x, heading_y, heading_width, heading_height;
	
	int attention_offset_x, attention_offset_y;
	int reactive_offset_x, reactive_offset_y;

	int button_size_x, button_size_y;
    int button_spacing_x, button_spacing_y;
    int check_button_size_x;
	int check_button_size_y;
	int input_size_x, input_size_y;
	int title_height;
    int window_width, window_height, border_width, border_height;
	
    int reactive_output_width;
    int reactive_output_height;
    int reactive_output_x;
    int reactive_output_y;

    int attention_output_width;
    int attention_output_height;
    int attention_output_x;
    int attention_output_y;

    int text_input_width;
    int text_input_height;

    /********/
    
    int switchOff_x;
    int switchOff_y;
    int switchOff_value;
    int switchOff_value_old;

    int switchOff_reactive_x;
    int switchOff_reactive_y;
    int switchOff_reactive_value;
    int switchOff_reactive_value_old;

    int sound_x;
    int sound_y;
    int sound_value;
    int sound_value_old;

    int attention_bias_x;
    int attention_bias_y;
    int attention_bias_value;
    int attention_bias_value_old;
    
    int therapist_command_x;
    int therapist_command_y;
    int therapist_command_value;
    int therapist_command_value_old;
    
    int getFaces_x;
    int getFaces_y;
    int getFaces_value;
    int getFaces_value_old;

    int faces = 0;
    /********/

    int mutual_gaze_x;
    int mutual_gaze_y;
    int mutual_gaze_value;
    int mutual_gaze_value_old;

    int recognize_speech_x;
    int recognize_speech_y;
    char recognize_speech_value[STRINGLENGTH]; 

    int identify_face_expression_box_width;
    int identify_face_expression_box_height;

    int identify_face_expression_value;
    int identify_face_expression_value_old;
    int identify_face_expression_box_x;
    int identify_face_expression_box_y;
    int identify_face_expression_happy_x;
    int identify_face_expression_happy_y;
    int identify_face_expression_sad_x;
    int identify_face_expression_sad_y;
    int identify_face_expression_angry_x;
    int identify_face_expression_angry_y;
    int identify_face_expression_fearful_x;
    int identify_face_expression_fearful_y;
    int identify_face_expression_none_x;
    int identify_face_expression_none_y;

    int robot_speech_x;
    int robot_speech_y;
    char robot_speech_value[STRINGLENGTH]; 

    int deliberative_face_expression_value;
    int deliberative_face_expression_value_old;
    int deliberative_face_expression_box_x;
    int deliberative_face_expression_box_y;
    int deliberative_face_expression_happy_x;
    int deliberative_face_expression_happy_y;
    int deliberative_face_expression_sad_x;
    int deliberative_face_expression_sad_y;
    int deliberative_face_expression_angry_x;
    int deliberative_face_expression_angry_y;
    int deliberative_face_expression_fearful_x;
    int deliberative_face_expression_fearful_y;
    int deliberative_face_expression_none_x;
    int deliberative_face_expression_none_y;
    
    int get_balance_x;
    int get_balance_y;
    int get_balance_value;
    int get_balance_value_old;
    
    int get_head_touch_x;
    int get_head_touch_y;
    int get_head_touch_value;
    int get_head_touch_value_old;
    
    int get_left_hand_touch_x;
    int get_left_hand_touch_y;
    int get_left_hand_touch_value;
    int get_left_hand_touch_value_old;
    
    int get_right_hand_touch_x;
    int get_right_hand_touch_y;
    int get_right_hand_touch_value;
    int get_right_hand_touch_value_old;
    
    int get_left_foot_touch_x;
    int get_left_foot_touch_y;
    int get_left_foot_touch_value;
    int get_left_foot_touch_value_old;

    int get_right_foot_touch_x;
    int get_right_foot_touch_y;
    int get_right_foot_touch_value;
    int get_right_foot_touch_value_old;
 
    int radio_button_size_x;
    int radio_button_size_y;
    int radio_button_spacing_y;
   
    /* intialize the presentation settings */

    border_width  = GetSystemMetrics(SM_CXSIZEFRAME);  
    border_height = GetSystemMetrics(SM_CYSIZEFRAME);
    
    title_height = 12;

	attention_offset_x = 20;
	attention_offset_y = 0;

    reactive_panel_width = 500;
	reactive_panel_height = 400+ 20*4 + BORDER*2;

	button_size_x = 140;
	button_size_y = 40;
    button_spacing_x = 10;
	button_spacing_y = 5;

    check_button_size_x = 20;
	check_button_size_y = 20;

    radio_button_size_x = 60;
	radio_button_size_y = 22;
	radio_button_spacing_y = 2;

	input_size_x = button_size_x/2;
	input_size_y = button_size_y;

    //window_width  = GetSystemMetrics(SM_CXFULLSCREEN);                      // this is the area available for the window on the screen
    window_width  = (reactive_panel_width  + 30) * 2 + 30; // we initialize window_height once we have worked out the heading values

	heading_x = (window_width - reactive_panel_width*2 - 30)/2; // where to begin the column of displays
	heading_y = 0; // MENUBAR_HEIGHT;
	heading_width = reactive_panel_width*2 + 30;                // centre over the two columns: child behaviour column and cognitive control column
	heading_height = HEADING_HEIGHT*2;                                // logo and caption
	
    reactive_offset_x = 20;
    reactive_offset_y = 0;
	
    attention_panel_width  = reactive_panel_width;
    attention_panel_height = reactive_panel_height;  
    attention_panel_x = heading_x;
    attention_panel_y = heading_y + heading_height;

    reactive_panel_x = heading_x + attention_panel_width;
    reactive_panel_y = heading_y + heading_height;

    //window_height = GetSystemMetrics(SM_CYFULLSCREEN);    
    window_height = heading_height + reactive_panel_height +title_height ; 
       
    reactive_output_width  = reactive_panel_width - BORDER*2;
    reactive_output_height = 50;
    reactive_output_x = reactive_panel_x + BORDER*1;
    reactive_output_y = reactive_panel_y; // + BORDER;

    attention_output_width = attention_panel_width - BORDER*2;
    attention_output_height = 150;
    attention_output_x = attention_panel_x + BORDER*1;
    attention_output_y = attention_panel_y; // + BORDER;

    text_input_height = button_size_y;
   
    /*********/
    get_balance_x      = attention_output_x + BORDER  ;
    get_balance_y      = attention_panel_y+text_input_height;

    get_head_touch_x      = attention_output_x + attention_output_width - BORDER - check_button_size_x;
    get_head_touch_y      = get_balance_y ;

    get_left_hand_touch_x      = attention_output_x + BORDER  ;
    get_left_hand_touch_y      = get_head_touch_y +text_input_height;

    get_right_hand_touch_x      =attention_output_x + attention_output_width - BORDER - check_button_size_x;
    get_right_hand_touch_y      = get_left_hand_touch_y;

    get_left_foot_touch_x      = attention_output_x + BORDER  ;
    get_left_foot_touch_y      = get_left_hand_touch_y +text_input_height;

    get_right_foot_touch_x      = attention_output_x + attention_output_width - BORDER - check_button_size_x;
    get_right_foot_touch_y      = get_left_foot_touch_y;
    
    mutual_gaze_x      = attention_output_x + attention_output_width - BORDER - check_button_size_x;
    mutual_gaze_y      = get_left_foot_touch_y + text_input_height;

    sound_x      = attention_output_x + BORDER ;
    sound_y      = attention_output_y + attention_output_height + button_spacing_y +text_input_height + 2*button_spacing_y;

    getFaces_x      = attention_output_x + BORDER ;
    getFaces_y      =  sound_y+text_input_height + 2*button_spacing_y;

    attention_bias_x      = attention_output_x + attention_output_width - BORDER - button_size_x;
    attention_bias_y      = sound_y;
    
    therapist_command_x      = attention_output_x + attention_output_width - BORDER - button_size_x;
    therapist_command_y      = attention_bias_y+text_input_height+ 2*button_spacing_y;

    switchOff_x      = attention_output_x + attention_output_width - BORDER - check_button_size_x;
    switchOff_y      = therapist_command_y +text_input_height+ 2*button_spacing_y +text_input_height;
        
    switchOff_reactive_x      = attention_output_x + BORDER;
    switchOff_reactive_y      = switchOff_y ;

    /**********/

    text_input_width  = 3*reactive_panel_width/4 - BORDER;
    text_input_height = button_size_y;
   
    recognize_speech_x = reactive_output_x + reactive_output_width - BORDER - text_input_width;
    recognize_speech_y = reactive_output_y +text_input_height + 4*button_spacing_y;

    identify_face_expression_box_width = button_size_x;
    identify_face_expression_box_height = radio_button_size_y;

    identify_face_expression_box_x = reactive_output_x + BORDER;
    identify_face_expression_box_y = recognize_speech_y + button_size_y + 2*button_spacing_y;

    identify_face_expression_happy_x = identify_face_expression_box_x ;
    identify_face_expression_happy_y = identify_face_expression_box_y-BORDER + identify_face_expression_box_height;

	identify_face_expression_sad_x = identify_face_expression_happy_x + radio_button_size_x;
    identify_face_expression_sad_y = identify_face_expression_happy_y;

    identify_face_expression_angry_x = identify_face_expression_happy_x + 2*radio_button_size_x;
    identify_face_expression_angry_y = identify_face_expression_happy_y;

    identify_face_expression_fearful_x = identify_face_expression_happy_x  + 3*radio_button_size_x;
    identify_face_expression_fearful_y = identify_face_expression_happy_y;

    identify_face_expression_none_x = identify_face_expression_happy_x  + 4*radio_button_size_x;
    identify_face_expression_none_y = identify_face_expression_fearful_y;

    robot_speech_x = reactive_output_x + reactive_output_width - BORDER - text_input_width;
    robot_speech_y = identify_face_expression_fearful_y+text_input_height;
    
    deliberative_face_expression_box_x = reactive_output_x + BORDER;
    deliberative_face_expression_box_y = robot_speech_y + button_size_y + 2*button_spacing_y;

    deliberative_face_expression_happy_x = deliberative_face_expression_box_x ;
    deliberative_face_expression_happy_y = deliberative_face_expression_box_y-BORDER + identify_face_expression_box_height;

    deliberative_face_expression_sad_x = deliberative_face_expression_happy_x + radio_button_size_x;
    deliberative_face_expression_sad_y = deliberative_face_expression_happy_y;

    deliberative_face_expression_angry_x = deliberative_face_expression_happy_x + 2*radio_button_size_x;
    deliberative_face_expression_angry_y = deliberative_face_expression_happy_y;

    deliberative_face_expression_fearful_x = deliberative_face_expression_happy_x  + 3*radio_button_size_x;
    deliberative_face_expression_fearful_y = deliberative_face_expression_happy_y;

    deliberative_face_expression_none_x = deliberative_face_expression_happy_x  + 4.2*radio_button_size_x;
    deliberative_face_expression_none_y = deliberative_face_expression_fearful_y;

    /* define a group with all the GUI widgets */

    start_systemArchitecture_GUI = new Fl_Group(0,MENUBAR_HEIGHT,window_width,window_height-MENUBAR_HEIGHT,"");

    start_systemArchitecture_GUI->begin();  

      /**************************************/
      /*                                    */
      /*           heading part             */
      /*                                    */
      /**************************************/

      /*  ... note: we don't declare heading because it is declared in the guiUtilities library   */ 

      heading = new Fl_Box(heading_x, heading_y, heading_width,heading_height,"Combined User-Developer Interface" );

      heading->align(FL_ALIGN_INSIDE | FL_ALIGN_CENTER |  FL_ALIGN_IMAGE_OVER_TEXT);
      heading->labelsize(20);
      heading->labelcolor(FL_BLACK);  
      heading->labelfont(FL_HELVETICA_BOLD);
      heading->box(FL_NO_BOX);
      heading->image(logo);

      /**************************************/
      /*                                    */
      /*      cognitive control part        */
      /*                                    */
      /**************************************/

      Fl_Box *attention_panel;
      attention_panel = new Fl_Box(attention_panel_x, attention_panel_y-BORDER, attention_panel_width, attention_panel_height,"" );
      attention_panel->align(FL_ALIGN_TOP | FL_ALIGN_CENTER | FL_ALIGN_INSIDE);
      attention_panel->labelsize(12);
      attention_panel->labelfont(FL_HELVETICA_BOLD);
      attention_panel->box(FL_DOWN_BOX);

	  Fl_Box *reactive_panel;
      reactive_panel = new Fl_Box(reactive_panel_x, reactive_panel_y-BORDER, reactive_panel_width, reactive_panel_height,"" );
      reactive_panel->align(FL_ALIGN_TOP | FL_ALIGN_CENTER | FL_ALIGN_INSIDE);
      reactive_panel->labelsize(12);
      reactive_panel->labelfont(FL_HELVETICA_BOLD);
      reactive_panel->box(FL_DOWN_BOX);
      
      Fl_Multiline_Output *reactive_output;
      reactive_output = new Fl_Multiline_Output(reactive_output_x, reactive_output_y, reactive_output_width, reactive_output_height,"" );
      reactive_output->align(FL_ALIGN_RIGHT | FL_ALIGN_CENTER );
      reactive_output->labelsize(12);
      reactive_output->textsize(12);
      reactive_output->color(FL_GREEN);
      reactive_output->value("\n Please wait while ports are being connected ... ");
      reactive_output->box(FL_PLASTIC_DOWN_BOX);   // rounded edges for something more visually appealing

      /* Text output ... note: we don't declare text_output because it is declared in the guiUtilities library   */ 
      /*Fl_Multiline_Output *attention_output;
      attention_output = new Fl_Multiline_Output(attention_output_x, attention_output_y, attention_output_width, attention_output_height,"" );
      attention_output->align(FL_ALIGN_RIGHT | FL_ALIGN_CENTER );
      attention_output->labelsize(12);
      attention_output->textsize(12);
      attention_output->color(FL_GREEN);
      attention_output->value("\n (x, y, z)     EA x HF = A \n (x, y, z)     EA x HF = B \n (x, y, z)     EA x HF = C \n        [...] ");
      //text_output->box(FL_THIN_DOWN_FRAME);  // matches the image display frames
      attention_output->box(FL_PLASTIC_DOWN_BOX);   // rounded edges for something more visually appealing*/
 
      Fl_Button *getFaces;
      getFaces = new Fl_Button(getFaces_x, getFaces_y, button_size_x, button_size_y,"get faces");
      getFaces->type(FL_NORMAL_BUTTON);
      getFaces->callback(button_cb, &getFaces_value); 
      getFaces->when(FL_WHEN_ENTER_KEY | FL_WHEN_CHANGED | FL_WHEN_RELEASE);
      
      Fl_Button *sound;
      sound = new Fl_Button(sound_x, sound_y, button_size_x, button_size_y,"sound direction");
      sound->type(FL_NORMAL_BUTTON);
      sound->callback(button_cb, &sound_value); 
      sound->when(FL_WHEN_ENTER_KEY | FL_WHEN_CHANGED | FL_WHEN_RELEASE); 
       
      Fl_Button *attention_bias;
      attention_bias = new Fl_Button(attention_bias_x, attention_bias_y, button_size_x, button_size_y,"attention bias");  
      attention_bias->type(FL_NORMAL_BUTTON);
      attention_bias->callback(button_cb, &attention_bias_value); 
      attention_bias->when(FL_WHEN_ENTER_KEY | FL_WHEN_CHANGED | FL_WHEN_RELEASE); 
      
      Fl_Button *therapist_command;
      therapist_command = new Fl_Button(therapist_command_x, therapist_command_y, button_size_x, button_size_y,"therapist command");       
      therapist_command->type(FL_NORMAL_BUTTON); 
      therapist_command->callback(button_cb, &therapist_command_value); 
      therapist_command->when(FL_WHEN_ENTER_KEY | FL_WHEN_CHANGED | FL_WHEN_RELEASE); 
           
      Fl_Check_Button *switchOff;
      switchOff = new Fl_Check_Button(switchOff_x, switchOff_y, check_button_size_x, check_button_size_y,"switchOff attention");
      switchOff->callback(button_cb, &switchOff_value); 
      switchOff->labelsize(12);  
      switchOff->labelfont(FL_HELVETICA_BOLD);
      switchOff->when(FL_WHEN_ENTER_KEY | FL_WHEN_CHANGED); 
      switchOff->align(FL_ALIGN_LEFT | FL_ALIGN_CENTER); 
      switchOff->value(0);

      /* enable / disable robot check box */

      Fl_Check_Button *mutual_gaze;
      mutual_gaze = new Fl_Check_Button(mutual_gaze_x, mutual_gaze_y, check_button_size_x, check_button_size_y,"mutual_gaze control");
      mutual_gaze->callback(button_cb, &mutual_gaze_value); 
      mutual_gaze->labelsize(12);  
      mutual_gaze->labelfont(FL_HELVETICA_BOLD);
      mutual_gaze->when(FL_WHEN_ENTER_KEY | FL_WHEN_CHANGED); 
      mutual_gaze->align(FL_ALIGN_LEFT | FL_ALIGN_CENTER); 
      mutual_gaze->value(0);

      /* say */

      Fl_Input *recognize_speech;
	  recognize_speech = new Fl_Input(recognize_speech_x, recognize_speech_y, text_input_width, input_size_y,"Child say:");
	  recognize_speech->textsize(12);
	  recognize_speech->callback(get_input_cb,recognize_speech_value);  
	  recognize_speech->labelsize(12);
      recognize_speech->labelfont(FL_HELVETICA_BOLD); 
	  recognize_speech->align(FL_ALIGN_LEFT);
	  recognize_speech->value("");
      get_input_cb(recognize_speech,recognize_speech_value);
      recognize_speech->when(FL_WHEN_ENTER_KEY | FL_WHEN_NOT_CHANGED);  // callback is called: when the enter key is hit 
 
      // Processing Mode Box
    
      Fl_Box *identify_face_expression_box;
      identify_face_expression_box = new Fl_Box(identify_face_expression_box_x, identify_face_expression_box_y, identify_face_expression_box_width, identify_face_expression_box_height,"Identify Facial Expression" );
      identify_face_expression_box->align(FL_ALIGN_TOP | FL_ALIGN_LEFT | FL_ALIGN_INSIDE);
      identify_face_expression_box->labelsize(12);
      identify_face_expression_box->labelfont(FL_HELVETICA_BOLD);
      
      // happy tone of voice  

      Fl_Round_Button *sensor_happy_mode = new Fl_Round_Button (identify_face_expression_happy_x, identify_face_expression_happy_y, radio_button_size_x, radio_button_size_y, "Happy");
      sensor_happy_mode->callback(happy_cb, &identify_face_expression_value);
      sensor_happy_mode->labelsize(12);
      sensor_happy_mode->type(FL_RADIO_BUTTON);
      sensor_happy_mode->value(1);
        
      // sad tone of voice  

      Fl_Round_Button *sensor_sad_mode = new Fl_Round_Button (identify_face_expression_sad_x, identify_face_expression_sad_y, radio_button_size_x, radio_button_size_y, "Sad");
      sensor_sad_mode->callback(sad_cb, &identify_face_expression_value);
      sensor_sad_mode->labelsize(12);
      sensor_sad_mode->type(FL_RADIO_BUTTON);
      sensor_sad_mode->value(0);
        
      // angry tone of voice  

      Fl_Round_Button *sensor_angry_mode = new Fl_Round_Button (identify_face_expression_angry_x, identify_face_expression_angry_y, radio_button_size_x, radio_button_size_y, "Angry");
      sensor_angry_mode->callback(angry_cb, &identify_face_expression_value);
      sensor_angry_mode->labelsize(12);
      sensor_angry_mode->type(FL_RADIO_BUTTON);
      sensor_angry_mode->value(0);
       
      // fearful tone of voice  

      Fl_Round_Button *sensor_fearful_mode = new Fl_Round_Button (identify_face_expression_fearful_x, identify_face_expression_fearful_y, radio_button_size_x, radio_button_size_y, "Fearful");
      sensor_fearful_mode->callback(fearful_cb, &identify_face_expression_value);
      sensor_fearful_mode->labelsize(12);
      sensor_fearful_mode->type(FL_RADIO_BUTTON);
      sensor_fearful_mode->value(0);

	  Fl_Round_Button *sensor_none_mode = new Fl_Round_Button (identify_face_expression_none_x, identify_face_expression_none_y, radio_button_size_x, radio_button_size_y, "None");
      sensor_none_mode->callback(none_cb, &identify_face_expression_value);
      sensor_none_mode->labelsize(12);
      sensor_none_mode->type(FL_RADIO_BUTTON);
      sensor_none_mode->value(0);

	  Fl_Input *robot_speech;
	  robot_speech = new Fl_Input(robot_speech_x, robot_speech_y, text_input_width, input_size_y,"Robot say:");
	  robot_speech->textsize(12);
	  robot_speech->callback(get_input_cb,robot_speech_value);  
	  robot_speech->labelsize(12);
      robot_speech->labelfont(FL_HELVETICA_BOLD); 
	  robot_speech->align(FL_ALIGN_LEFT);
	  robot_speech->value("");
      get_input_cb(robot_speech,robot_speech_value);
      robot_speech->when(FL_WHEN_ENTER_KEY | FL_WHEN_NOT_CHANGED);  // callback is called: when the enter key is hit 

	  Fl_Box *deliberative_face_expression_box;
      deliberative_face_expression_box = new Fl_Box(deliberative_face_expression_box_x, deliberative_face_expression_box_y, identify_face_expression_box_width, identify_face_expression_box_height,"Affective State" );
      deliberative_face_expression_box->align(FL_ALIGN_TOP | FL_ALIGN_LEFT | FL_ALIGN_INSIDE);
      deliberative_face_expression_box->labelsize(12);
      deliberative_face_expression_box->labelfont(FL_HELVETICA_BOLD);
      
      // happy tone of voice  

      Fl_Round_Button *deliberative_happy_mode = new Fl_Round_Button (deliberative_face_expression_happy_x, deliberative_face_expression_happy_y, radio_button_size_x, radio_button_size_y, "Happy");
      deliberative_happy_mode->callback(happy_cb, &deliberative_face_expression_value);
      deliberative_happy_mode->labelsize(12);
      deliberative_happy_mode->type(FL_RADIO_BUTTON);
      deliberative_happy_mode->value(1);
        
      // sad tone of voice  

      Fl_Round_Button *deliberative_sad_mode = new Fl_Round_Button (deliberative_face_expression_sad_x, deliberative_face_expression_sad_y, radio_button_size_x, radio_button_size_y, "Sad");
      deliberative_sad_mode->callback(sad_cb, &deliberative_face_expression_value);
      deliberative_sad_mode->labelsize(12);
      deliberative_sad_mode->type(FL_RADIO_BUTTON);
      deliberative_sad_mode->value(0);
        
      // angry tone of voice  

      Fl_Round_Button *deliberative_angry_mode = new Fl_Round_Button (deliberative_face_expression_angry_x, deliberative_face_expression_angry_y, radio_button_size_x, radio_button_size_y, "Angry");
      deliberative_angry_mode->callback(angry_cb, &deliberative_face_expression_value);
      deliberative_angry_mode->labelsize(12);
      deliberative_angry_mode->type(FL_RADIO_BUTTON);
      deliberative_angry_mode->value(0);
       
      // fearful tone of voice  

      Fl_Round_Button *deliberative_fearful_mode = new Fl_Round_Button (deliberative_face_expression_fearful_x, deliberative_face_expression_fearful_y, radio_button_size_x, radio_button_size_y, "Fearful");
      deliberative_fearful_mode->callback(fearful_cb, &deliberative_face_expression_value);
      deliberative_fearful_mode->labelsize(12);
      deliberative_fearful_mode->type(FL_RADIO_BUTTON);
      deliberative_fearful_mode->value(0);

	  Fl_Round_Button *deliberative_none_mode = new Fl_Round_Button (deliberative_face_expression_none_x, deliberative_face_expression_none_y, radio_button_size_x, radio_button_size_y, "None");
      deliberative_none_mode->callback(none_cb, &deliberative_face_expression_value);
      deliberative_none_mode->labelsize(12);
      deliberative_none_mode->type(FL_RADIO_BUTTON);
      deliberative_none_mode->value(0);

	  Fl_Check_Button *get_balance;
      get_balance = new Fl_Check_Button(get_balance_x, get_balance_y, check_button_size_x, check_button_size_y,"outOfbalance");
      get_balance->callback(button_cb, &get_balance_value); 
      get_balance->labelsize(12);  
      get_balance->labelfont(FL_HELVETICA_BOLD);
      get_balance->when(FL_WHEN_ENTER_KEY | FL_WHEN_CHANGED); 
      get_balance->align(FL_ALIGN_RIGHT | FL_ALIGN_CENTER); 
      get_balance->value(0);

	  Fl_Check_Button *get_head_touch;
      get_head_touch = new Fl_Check_Button(get_head_touch_x, get_head_touch_y, check_button_size_x, check_button_size_y,"get_head_touch");
      get_head_touch->callback(button_cb, &get_head_touch_value); 
      get_head_touch->labelsize(12);  
      get_head_touch->labelfont(FL_HELVETICA_BOLD);
      get_head_touch->when(FL_WHEN_ENTER_KEY | FL_WHEN_CHANGED); 
      get_head_touch->align(FL_ALIGN_LEFT | FL_ALIGN_CENTER); 
      get_head_touch->value(0);

	  Fl_Check_Button *get_left_hand_touch;
      get_left_hand_touch = new Fl_Check_Button(get_left_hand_touch_x, get_left_hand_touch_y, check_button_size_x, check_button_size_y,"left_hand_touch");
      get_left_hand_touch->callback(button_cb, &get_left_hand_touch_value); 
      get_left_hand_touch->labelsize(12);  
      get_left_hand_touch->labelfont(FL_HELVETICA_BOLD);
      get_left_hand_touch->when(FL_WHEN_ENTER_KEY | FL_WHEN_CHANGED); 
      get_left_hand_touch->align(FL_ALIGN_RIGHT | FL_ALIGN_CENTER); 
      get_left_hand_touch->value(0);

	  Fl_Check_Button *get_right_hand_touch;
      get_right_hand_touch = new Fl_Check_Button(get_right_hand_touch_x, get_right_hand_touch_y, check_button_size_x, check_button_size_y,"right_hand_touch");
      get_right_hand_touch->callback(button_cb, &get_right_hand_touch_value); 
      get_right_hand_touch->labelsize(12);  
      get_right_hand_touch->labelfont(FL_HELVETICA_BOLD);
      get_right_hand_touch->when(FL_WHEN_ENTER_KEY | FL_WHEN_CHANGED); 
      get_right_hand_touch->align(FL_ALIGN_LEFT | FL_ALIGN_CENTER); 
      get_right_hand_touch->value(0);

	  Fl_Check_Button *get_left_foot_touch;
      get_left_foot_touch = new Fl_Check_Button(get_left_foot_touch_x, get_left_foot_touch_y, check_button_size_x, check_button_size_y,"left_foot_touch");
      get_left_foot_touch->callback(button_cb, &get_left_foot_touch_value); 
      get_left_foot_touch->labelsize(12);  
      get_left_foot_touch->labelfont(FL_HELVETICA_BOLD);
      get_left_foot_touch->when(FL_WHEN_ENTER_KEY | FL_WHEN_CHANGED); 
      get_left_foot_touch->align(FL_ALIGN_RIGHT | FL_ALIGN_CENTER); 
      get_left_foot_touch->value(0);

	  Fl_Check_Button *get_right_foot_touch;
      get_right_foot_touch = new Fl_Check_Button(get_right_foot_touch_x, get_right_foot_touch_y, check_button_size_x, check_button_size_y,"right_foot_touch");
      get_right_foot_touch->callback(button_cb, &get_right_foot_touch_value); 
      get_right_foot_touch->labelsize(12);  
      get_right_foot_touch->labelfont(FL_HELVETICA_BOLD);
      get_right_foot_touch->when(FL_WHEN_ENTER_KEY | FL_WHEN_CHANGED); 
      get_right_foot_touch->align(FL_ALIGN_LEFT | FL_ALIGN_CENTER); 
      get_right_foot_touch->value(0);

      Fl_Check_Button *switchOff_reactive;
      switchOff_reactive = new Fl_Check_Button(switchOff_reactive_x, switchOff_reactive_y, check_button_size_x, check_button_size_y,"switchOff reactive");
      switchOff_reactive->callback(button_cb, &switchOff_reactive_value); 
      switchOff_reactive->labelsize(12);  
      switchOff_reactive->labelfont(FL_HELVETICA_BOLD);
      switchOff_reactive->when(FL_WHEN_ENTER_KEY | FL_WHEN_CHANGED); 
      switchOff_reactive->align(FL_ALIGN_RIGHT | FL_ALIGN_CENTER); 
      switchOff_reactive->value(0);

      /**************************************/
      /*                                    */
      /*     simulation parameters part     */
      /*                                    */
      /**************************************/

      /* box to frame controls */
   
    start_systemArchitecture_GUI->end();  // end of the group 

    sound_value                               = -1;
    sound_value_old                           = -1; // this forces the old value to be updated and the current value to be issued

    getFaces_value_old                        = -1;

    switchOff_value                           = 0;
    switchOff_value_old                       = -1; 

    therapist_command_value                   = -1;
    therapist_command_value_old               = -1; 

    attention_bias_value                      = -1;
    attention_bias_value_old                  = -1;

    mutual_gaze_value                         = 0;
    mutual_gaze_value_old                     = -1; // this forces the old value to be updated and the current value to be issued

    switchOff_reactive_value                  = 0;
    switchOff_reactive_value_old              = -1; 

    identify_face_expression_value            = 1;
    identify_face_expression_value_old        = -1;

   	deliberative_face_expression_value        = 1;
	deliberative_face_expression_value_old    = -1;

	get_balance_value                         = 0;
	get_balance_value_old                     = -1;
	get_head_touch_value                      = 0;
	get_head_touch_value_old                  =-1;
	get_left_hand_touch_value                 = 0;
	get_left_hand_touch_value_old             = -1;
	get_right_hand_touch_value                = 0;
	get_right_hand_touch_value_old            = -1;
	get_left_foot_touch_value                 = 0;
	get_left_foot_touch_value_old             = -1;
	get_right_foot_touch_value                = 0;
	get_right_foot_touch_value_old            = -1;
	
    // create main window
  
    DVwindow = new Fl_Double_Window(0,50,window_width-border_width,window_height-border_height,"The DREAM Project");

    Fl::visual(FL_DOUBLE); // enable double buffering of the window 

    DVwindow->end(); 
    //DVwindow->show(); 

    // add the GUI

    DVwindow->add(start_systemArchitecture_GUI);
    DVwindow->redraw();
    //DVwindow->show();
    DVwindow->iconize();
    DVwindow->show();

  
    /*************************************************************************************/
    /*   This is where the definition of the GUI ends and the normal run() loop begins   */
    /*************************************************************************************/

    while (isStopping() != true) { // the thread continues to run until isStopping() returns true

        DVwindow->redraw();
        Fl::check();         // refresh the GUI   .... we do it this way rather than calling Fl::run() as we nomally would 
                           // because we do not want to cede control to the event loop
 
                  
        /**************************************/
        /*                                    */
        /*       cognitive control part       */
        /*                                    */
        /**************************************/
        if (sound_value_old != sound_value) {
            sound_value_old = sound_value;
            if (sound_value==1){  
                VectorOf<double> &soundVector = getSoundDirectionPortOut->prepare();
                soundVector.resize(2);
                soundVector(0) = -119 + (double)(rand() %230); 
                soundVector(1) = -38 + (double)(rand() %50); //vertical
                                
                getSoundDirectionPortOut->write(); 
            }
        }
        

        /*attention bias*/
        if (attention_bias_value_old != attention_bias_value) {
            attention_bias_value_old = attention_bias_value;
            if (attention_bias_value==1){         
                    VectorOf<double> &emulationVector = attentionBiasPortOut->prepare();
                    emulationVector.resize(3);
            
                    emulationVector(0) = (double)(rand() %20)+0.5;
                    emulationVector(1) = (double)(rand() %10)+0.5;
                    emulationVector(2) = (double)(rand() %3)+0.25;
           
                    attentionBiasPortOut->write(); 
            }            
        }
        /*Therapist command*/
        if (therapist_command_value_old != therapist_command_value) {
            therapist_command_value_old = therapist_command_value;
            if (therapist_command_value==1){         
                VectorOf<double> &therapistVector = therapistGazeCommandPortOut->prepare();
                therapistVector.resize(3);
            
                therapistVector(0) = (double)(rand() %20)+0.5;
                therapistVector(1) = (double)(rand() %10)+0.5;
                therapistVector(2) = (double)(rand() %3)+0.25;            
                      
                therapistGazeCommandPortOut->write(); 
            }
        }
        
        if (getFaces_value_old != getFaces_value) {
            getFaces_value_old = getFaces_value;
            if (getFaces_value==1){
                
                VectorOf<double> &getFacesVector = getFacesPortOut->prepare();

                faces = (int)(rand()%3)+1;
            
                getFacesVector.resize(3*faces);
            
                for(int i = 0; i <faces;i++){
                    getFacesVector(0+(i*3)) = (double)(rand() %20)+0.5;
                    getFacesVector(1+(i*3)) = (double)(rand() %10)+0.5;
                    getFacesVector(2+(i*3)) = (double)(rand() %3)+0.5; 
                }
                getFacesPortOut->write();
            }
        }
	    
        /* switchOff_value */
        if (switchOff_value_old != switchOff_value) {
            switchOff_value_old = switchOff_value;
         
            VectorOf<int> &dataVector = attentionSwitchOffPortOut->prepare();
            dataVector.resize(1);

            dataVector(0) = switchOff_value;
            
            attentionSwitchOffPortOut->write(); 

            if(switchOff_value == 1){
                
                therapist_command->deactivate();
                getFaces->deactivate();
                sound->deactivate();
                attention_bias->deactivate();
            }
            if(switchOff_value == 0){
                
                therapist_command->activate();
                getFaces->activate();
                sound->activate();
                attention_bias->activate();
            }
        }
               
        /* mutual_gaze_value primitive */
        if (mutual_gaze_value_old != mutual_gaze_value) {
            mutual_gaze_value_old = mutual_gaze_value;
         
            VectorOf<int> &dataVector = checkMutualGazePortOut->prepare();
            dataVector.resize(1);

            dataVector(0) = mutual_gaze_value;
            
            checkMutualGazePortOut->write(); 
        }

	    /* get_balance_value primitive */
        if (get_balance_value_old != get_balance_value) {
            get_balance_value_old = get_balance_value;
         
            Bottle &dataVector = robotSensorsPortOut->prepare();
            dataVector.clear();
            dataVector.addString("Falling");
            if(get_balance_value){
			    dataVector.addInt(1);
            }
		    else{
			    dataVector.addInt(0);
            }
                        
            robotSensorsPortOut->write(); 
        }
      
	    /* get_head_touch_value primitive */
        if (get_head_touch_value_old != get_head_touch_value) {
            get_head_touch_value_old = get_head_touch_value;
         
            Bottle &dataVector = robotSensorsPortOut->prepare();
            dataVector.clear();
            dataVector.addString("FrontTactil");
            if(get_head_touch_value){
			    dataVector.addInt(1);
            }
		    else{
			    dataVector.addInt(0);
            }
                        
            robotSensorsPortOut->write(); 
         
        }

	    /* get_left_hand_touch_value primitive */
        if (get_left_hand_touch_value_old != get_left_hand_touch_value) {
            get_left_hand_touch_value_old = get_left_hand_touch_value;
         
            Bottle &dataVector = robotSensorsPortOut->prepare();
            dataVector.clear();
            dataVector.addString("LeftHand");
            if(get_left_hand_touch_value){
			    dataVector.addInt(1);
            }
		    else{
			    dataVector.addInt(0);
            }
                        
            robotSensorsPortOut->write(); 
         
         
        }

	    /* get_right_hand_touch_value primitive */
        if (get_right_hand_touch_value_old != get_right_hand_touch_value) {
            get_right_hand_touch_value_old = get_right_hand_touch_value;
         
           Bottle &dataVector = robotSensorsPortOut->prepare();
            dataVector.clear();
            dataVector.addString("RightHand");
            if(get_right_hand_touch_value){
			    dataVector.addInt(1);
            }
		    else{
			    dataVector.addInt(0);
            }
                        
            robotSensorsPortOut->write();      
        }
	    /* get_left_foot_touch_value primitive */
        if (get_left_foot_touch_value_old != get_left_foot_touch_value) {
            get_left_foot_touch_value_old = get_left_foot_touch_value;
         
            Bottle &dataVector = robotSensorsPortOut->prepare();
            dataVector.clear();
            dataVector.addString("LeftBumper");
            if(get_left_foot_touch_value){
			    dataVector.addInt(1);
            }
		    else{
			    dataVector.addInt(0);
            }
                        
            robotSensorsPortOut->write(); 
         
        }

	    /* get_right_foot_touch_value primitive */
        if (get_right_foot_touch_value_old != get_right_foot_touch_value) {
            get_right_foot_touch_value_old = get_right_foot_touch_value;
         
            Bottle &dataVector = robotSensorsPortOut->prepare();
            dataVector.clear();
            dataVector.addString("RightBumper");
            if(get_right_foot_touch_value){
			    dataVector.addInt(1);
            }
		    else{
			    dataVector.addInt(0);
            }
                        
            robotSensorsPortOut->write();   
         
        }

        /* switchOff_blinking_value */
        if (switchOff_reactive_value_old != switchOff_reactive_value) {
            switchOff_reactive_value_old = switchOff_reactive_value;
         
            VectorOf<int> &dataVector = reactiveSwitchOffPortOut->prepare();
            dataVector.resize(1);

            dataVector(0) = switchOff_reactive_value;

            reactiveSwitchOffPortOut->write(); 
            if(switchOff_reactive_value == 1){
                
                mutual_gaze->deactivate();
			    recognize_speech->deactivate();
			    robot_speech->deactivate();
			    sensor_happy_mode->deactivate();
			    sensor_sad_mode->deactivate();
			    sensor_angry_mode->deactivate();
			    sensor_fearful_mode->deactivate();
			    sensor_none_mode->deactivate();
			    deliberative_happy_mode->deactivate();
			    deliberative_sad_mode->deactivate();
			    deliberative_angry_mode->deactivate();
			    deliberative_fearful_mode->deactivate();
			    deliberative_none_mode->deactivate();
			    get_head_touch->deactivate();
			    get_left_hand_touch->deactivate();
			    get_right_hand_touch->deactivate();
			    get_left_foot_touch->deactivate();
			    get_right_foot_touch->deactivate();
                
			    sprintf(modifiedString,"\n Reactive subsystem switched off. ");
                reactive_output->value(modifiedString);
                reactive_output->deactivate();
                
            }
            if(switchOff_reactive_value == 0){
                
                mutual_gaze->activate();
			    recognize_speech->activate();
			    robot_speech->activate();
			    sensor_happy_mode->activate();
			    sensor_sad_mode->activate();
			    sensor_angry_mode->activate();
			    sensor_fearful_mode->activate();
			    sensor_none_mode->activate();
			    deliberative_happy_mode->activate();
			    deliberative_sad_mode->activate();
			    deliberative_angry_mode->activate();
			    deliberative_fearful_mode->activate();
			    deliberative_none_mode->activate();
			    get_head_touch->activate();
			    get_left_hand_touch->activate();
			    get_right_hand_touch->activate();
			    get_left_foot_touch->activate();
			    get_right_foot_touch->activate();
                
			    sprintf(modifiedString," ");
                reactive_output->value(modifiedString);
                reactive_output->activate();
                
            } 
        }
        
	    /* recognize_speech_value(text, tone) primitive */
        if (strlen(recognize_speech_value) != 0) {

            Bottle &message = recognizeSpeechPortOut->prepare();

            message.clear();
            message.addString(recognize_speech_value);
         
            recognizeSpeechPortOut->write(); 
            recognize_speech->value("");
            recognize_speech_value[0] = '\0'; // clear the message
      
        }

	    /* identify_face_expression_value primitive */
        if (identify_face_expression_value_old != identify_face_expression_value) {
            identify_face_expression_value_old = identify_face_expression_value;
         
            VectorOf<int> &dataVector = identifyFaceExpressionPortOut->prepare(); 
            dataVector.resize(1);

            dataVector(0) = identify_face_expression_value;

            identifyFaceExpressionPortOut->write(); 
         
        }

	    /* deliberative_face_expression_value primitive */
        if (deliberative_face_expression_value_old != deliberative_face_expression_value) {
            deliberative_face_expression_value_old = deliberative_face_expression_value;
         
            Bottle &data = affectiveStatePortOut->prepare(); 
            data.clear();
            data.addInt(deliberative_face_expression_value);
            
            affectiveStatePortOut->write(); 
        }

        tempBottle = eyeBlinkingPortIn->read(false);                   // non-blocking version
        if (tempBottle != NULL) {
            strcpy(modifiedString, (*tempBottle).get(0).toString().c_str());
		    strcat(modifiedString, (*tempBottle).get(1).toString().c_str());
		    strcat(modifiedString, (*tempBottle).get(2).toString().c_str());
            reactive_output->value(modifiedString);
		    DVwindow->redraw();
		    Fl::check();
        }
        tempIntVector = fallingReactionSpeechPortIn->read(false);                   // non-blocking version
        if (tempIntVector != NULL) {
            
            tempBottle = sayPortIn->read(false);                   // non-blocking version
            if (tempBottle != NULL) {
                strcpy(modifiedString, (*tempBottle).get(0).toString().c_str());
                //message(modifiedString);
                reactive_output->value(modifiedString);
		        //falling_speech->value(modifiedString);
                //social_speech->value("");
		        DVwindow->redraw();
		        Fl::check();
            }
        }
        else{ //the social speech will be printed only when there is no falling speech
            
            tempBottle = sayPortIn->read(false);                   // non-blocking version
            if (tempBottle != NULL) {
                strcpy(modifiedString, (*tempBottle).get(0).toString().c_str());
                //message(modifiedString);
                reactive_output->value(modifiedString);
		        //social_speech->value(modifiedString);
                //falling_speech->value("");
		        DVwindow->redraw();
		        Fl::check();
            }
            
        }
        
	    tempIntVector = fallingInterruptionPortIn->read(false);                   // non-blocking version
        if (tempIntVector != NULL) {
            p = (*tempIntVector)[0];
            
		    /*If true it blocks all control panel inputs*/
		    if(p ==1){
                
			    mutual_gaze->deactivate();
			    sensor_happy_mode->deactivate();
			    sensor_sad_mode->deactivate();
			    sensor_angry_mode->deactivate();
			    sensor_fearful_mode->deactivate();
			    sensor_none_mode->deactivate();
			    deliberative_happy_mode->deactivate();
			    deliberative_sad_mode->deactivate();
			    deliberative_angry_mode->deactivate();
			    deliberative_fearful_mode->deactivate();
			    deliberative_none_mode->deactivate();
			    get_head_touch->deactivate();
			    get_left_hand_touch->deactivate();
			    get_right_hand_touch->deactivate();
			    get_left_foot_touch->deactivate();
			    get_right_foot_touch->deactivate();
                
			    sprintf(modifiedString,"\n System interrupted by fall. ");
                reactive_output->value(modifiedString);
                reactive_output->deactivate();
                //message(modifiedString);

		     }
		     else if (p==0){
			     reactive_output->activate();
			     mutual_gaze->activate();
			     sensor_happy_mode->activate();
			     sensor_sad_mode->activate();
			     sensor_angry_mode->activate();
			     sensor_fearful_mode->activate();
			     sensor_none_mode->activate();
			     deliberative_happy_mode->activate();
			     deliberative_sad_mode->activate();
			     deliberative_angry_mode->activate();
			     deliberative_fearful_mode->activate();
			     deliberative_none_mode->activate();
			     get_head_touch->activate();
			     get_left_hand_touch->activate();
			     get_right_hand_touch->activate();
			     get_left_foot_touch->activate();
			     get_right_foot_touch->activate();
			     sprintf(modifiedString,"\n System restored after fall. ");
                 reactive_output->value(modifiedString);
                 //message(modifiedString);
             }
		 }         
      }
   }


void AttentionReactionSystemGUIThread::threadRelease() 
{
   /* for example, delete dynamically created data-structures */
}


void pause(double period) {

   double start_time;
   double current_time;

   start_time      = yarp::os::Time::now(); // start time

   do {
      current_time     = yarp::os::Time::now();
   } while ((current_time - start_time) < period);
}

// -----------------------------------------------------------------------------------------------
// get_input_cb
//
// handle the text input widgets
// -----------------------------------------------------------------------------------------------

void get_input_cb(Fl_Widget *temp, void *input) {

   Fl_Input *f;

   f = (Fl_Input *)temp;

   strcpy((char *)input, f->value());

   //if (input != NULL)   printf("get_input_cb: %s\n",input);
}


// -----------------------------------------------------------------------------------------------
// happy_voice_cb
//
// handle the happy tone of voice radio button
// -----------------------------------------------------------------------------------------------

void happy_cb(Fl_Widget *b,void *destination) {

   Fl_Round_Button *button;
   button = (Fl_Round_Button *) b;


   *(int *)destination  = HAPPY;
   
    //printf("happy_voice_cb: value is now %d\n",*(int *)destination );

   button->setonly();
}

// -----------------------------------------------------------------------------------------------
// sad_voice_cb
//
// handle the sad tone of voice radio button
// -----------------------------------------------------------------------------------------------

void sad_cb(Fl_Widget *b,void *destination) {

   Fl_Round_Button *button;
   button = (Fl_Round_Button *) b;


   *(int *)destination  = SAD;
   
    //printf("sad_voice_cb: value is now %d\n",*(int *)destination );

   button->setonly();
}

// -----------------------------------------------------------------------------------------------
// angry_voice_cb
//
// handle the angry tone of voice radio button
// -----------------------------------------------------------------------------------------------

void angry_cb(Fl_Widget *b,void *destination) {

   Fl_Round_Button *button;
   button = (Fl_Round_Button *) b;


   *(int *)destination  = ANGRY;
   
    //printf("angry_voice_cb: value is now %d\n",*(int *)destination );

   button->setonly();
}

// -----------------------------------------------------------------------------------------------
// fearful_voice_cb
//
// handle the fearful tone of voice radio button
// -----------------------------------------------------------------------------------------------

void fearful_cb(Fl_Widget *b,void *destination) {

   Fl_Round_Button *button;
   button = (Fl_Round_Button *) b;


   *(int *)destination  = FEARFUL;
   
    //printf("fearful_voice_cb: value is now %d\n",*(int *)destination );

   button->setonly();
}
// -----------------------------------------------------------------------------------------------
// none_cb
//
// -----------------------------------------------------------------------------------------------

void none_cb(Fl_Widget *b,void *destination) {

   Fl_Round_Button *button;
   button = (Fl_Round_Button *) b;


   *(int *)destination  = NONE;
   
   button->setonly();
}
// -----------------------------------------------------------------------------------------------
// affective_cb
//
// handle the affective state
// -----------------------------------------------------------------------------------------------

void affective_cb(Fl_Widget *b,void *destination) {

   Fl_Round_Button *button;
   button = (Fl_Round_Button *) b;


   *(int *)destination  = AFFECTIVE;
   
    //printf("fearful_voice_cb: value is now %d\n",*(int *)destination );

   button->setonly();
}

// -----------------------------------------------------------------------------------------------
// aggresive_cb
//
// handle the aggresive state
// -----------------------------------------------------------------------------------------------

void aggresive_cb(Fl_Widget *b,void *destination) {

   Fl_Round_Button *button;
   button = (Fl_Round_Button *) b;


   *(int *)destination  = AGGRESIVE;
   
    //printf("fearful_voice_cb: value is now %d\n",*(int *)destination );

   button->setonly();
}