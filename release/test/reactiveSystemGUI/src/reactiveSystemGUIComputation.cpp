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
 
#include "reactiveSystemGUI.h"

ReactiveSystemGUIThread::ReactiveSystemGUIThread(            
                                                         Port                            *sensoryInterpretationOut,                            
                                                         BufferedPort<Bottle>			 *recognizeSpeechOut,   
                                                         BufferedPort<VectorOf<int>>     *checkMutualGazeOut, 
                                                         BufferedPort<VectorOf<int>>     *identifyFaceExpressionOut, 
                                                         BufferedPort<VectorOf<int>>     *getDeliberativeFacialExpressionOut,
                                                         BufferedPort<Bottle>		     *getSpeechOut,
                                                         BufferedPort<VectorOf<int>>     *getGazeVariationOut,
                                                         BufferedPort<VectorOf<int>>     *getBalanceOut,
                                                         BufferedPort<Bottle>            *getHeadTactileOut,
                                                         BufferedPort<Bottle>            *getHandTactileOut,
                                                         BufferedPort<Bottle>            *getBumperTactileOut,
														 BufferedPort<Bottle>            *getEyeBlinkingIn,
														 BufferedPort<VectorOf<int>>     *eyeBlinkingSwitchOffOut,
														 BufferedPort<VectorOf<int>>     *getInterruptionIn,
														 BufferedPort<VectorOf<int>>     *fallingReactionSwitchOffOut,
														 BufferedPort<VectorOf<int>>     *socialReactionSwitchOffOut,	
                                                         BufferedPort<VectorOf<int>>	 *getSocialReactionSpeechIn,
                                                         BufferedPort<VectorOf<int>>	 *getFallingReactionSpeechIn,
                                                         BufferedPort<Bottle>			 *moveSequenceIn,
                                                         BufferedPort<Bottle>			 *sayIn,
														 string logoFilename)
{

   sensoryInterpretationPortOut             = sensoryInterpretationOut;      
   recognizeSpeechPortOut                   = recognizeSpeechOut;
   checkMutualGazePortOut                   = checkMutualGazeOut;
   identifyFaceExpressionPortOut            = identifyFaceExpressionOut;   
   getDeliberativeFacialExpressionPortOut   = getDeliberativeFacialExpressionOut;
   getSpeechPortOut                         = getSpeechOut;
   getGazeVariationPortOut                  = getGazeVariationOut;
   getBalancePortOut                        = getBalanceOut;
   getHeadTactilePortOut                    = getHeadTactileOut;
   getHandTactilePortOut                    = getHandTactileOut;
   getBumperTactilePortOut                  = getBumperTactileOut;
   getEyeBlinkingPortIn                     = getEyeBlinkingIn;  
   eyeBlinkingSwitchOffPortOut              = eyeBlinkingSwitchOffOut;  
   getInterruptionPortIn                    = getInterruptionIn;
   fallingReactionSwitchOffPortOut          = fallingReactionSwitchOffOut;
   socialReactionSwitchOffPortOut           = socialReactionSwitchOffOut;
   getSocialReactionSpeechPortIn            = getSocialReactionSpeechIn; 
   getFallingReactionSpeechPortIn           = getFallingReactionSpeechIn;
   moveSequencePortIn                       = moveSequenceIn;
   sayPortIn                                = sayIn;
   logoFilenameValue                        = logoFilename;
}

bool ReactiveSystemGUIThread::threadInit() 
{
   /* initialize variables and create data-structures if needed */

   debug = false;

   return true;
}

void ReactiveSystemGUIThread::run(){

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

    int control_panel_x;
    int control_panel_y;
    int control_panel_width;
    int control_panel_height;

    int output_panel_x;
    int output_panel_y;
    int output_panel_width;
    int output_panel_height;
   
    int heading_x, heading_y, heading_width, heading_height;
	
	int control_offset_x, control_offset_y;
	int output_offset_x, output_offset_y;

	int button_size_x, button_size_y;
    int button_spacing_x, button_spacing_y;
    int check_button_size_x;
	int check_button_size_y;
	int input_size_x, input_size_y;
	int title_height;
    int window_width, window_height, border_width, border_height;
	
    int text_output_width;
    int text_output_height;
    int text_input_width;
    int text_input_height;

    int output_text_width;
    int output_text_height;

    int text_output_x;
    int text_output_y;

    int mutual_gaze_x;
    int mutual_gaze_y;
    int mutual_gaze_value;
    int mutual_gaze_value_old;

    int switchOff_blinking_x;
    int switchOff_blinking_y;
    int switchOff_blinking_value;
    int switchOff_blinking_value_old;

    int switchOff_falling_x;
    int switchOff_falling_y;
    int switchOff_falling_value;
    int switchOff_falling_value_old;

    int switchOff_socialReaction_x;
    int switchOff_socialReaction_y;
    int switchOff_socialReaction_value;
    int switchOff_socialReaction_value_old;

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

    int gaze_variation_x;
    int gaze_variation_y;
    int gaze_variation_value;
    int gaze_variation_value_old;

    int blinking_rate_output_x;
    int blinking_rate_output_y;

    int falling_speech_output_x;
    int falling_speech_output_y;

    int social_speech_output_x;
    int social_speech_output_y;

    int radio_button_size_x;
    int radio_button_size_y;
    int radio_button_spacing_y;
   

    /* intialize the presentation settings */

    border_width  = GetSystemMetrics(SM_CXSIZEFRAME);  
    border_height = GetSystemMetrics(SM_CYSIZEFRAME);
    
    title_height = 12;

	control_offset_x = 20;
	control_offset_y = 0;

	output_panel_width = 300;
	output_panel_height = 250+ 30*4 + BORDER*2;

	button_size_x = 60;
	button_size_y = 20;
    button_spacing_x = 10;
	button_spacing_y = 5;

    check_button_size_x = 20;
	check_button_size_y = 20;

    radio_button_size_x = button_size_x-BORDER;
	radio_button_size_y = 22;
	radio_button_spacing_y = 0;

	input_size_x = button_size_x/2;
	input_size_y = button_size_y;

    //window_width  = GetSystemMetrics(SM_CXFULLSCREEN);                      // this is the area available for the window on the screen
    window_width  = (output_panel_width  + 30) * 2 + 30; // we initialize window_height once we have worked out the heading values

	heading_x = (window_width - output_panel_width*2 - 30)/2; // where to begin the column of displays
	heading_y = 0; // MENUBAR_HEIGHT;
	heading_width = output_panel_width*2 + 30;                // centre over the two columns: child behaviour column and cognitive control column
	heading_height = HEADING_HEIGHT*2;                                // logo and caption
	
    output_offset_x = 20;
    output_offset_y = 0;
	
    control_panel_width  = output_panel_width;
    control_panel_height = output_panel_height;  
    control_panel_x = heading_x + output_panel_width;
    control_panel_y = heading_y + heading_height;

    output_panel_x = heading_x;
    output_panel_y = heading_y + heading_height;
  
    //window_height = GetSystemMetrics(SM_CYFULLSCREEN);    
    window_height = heading_height + output_panel_height + (title_height + 4*(5+button_size_y)+title_height) ; 
       
    text_output_width  = control_panel_width - BORDER*2;
    text_output_height = 50;
    text_output_x = control_panel_x + BORDER*1;
    text_output_y = control_panel_y; // + BORDER;

    output_text_width  = 2*control_panel_width/4 - BORDER;
    output_text_height = button_size_y;

    blinking_rate_output_x = output_panel_x + output_text_width + BORDER*1; 
    blinking_rate_output_y = output_panel_y + output_text_height + 4*button_spacing_y;
    
    falling_speech_output_x = output_panel_x + output_text_width + BORDER*1;
    falling_speech_output_y = blinking_rate_output_y + 2.3*output_text_height + 4*button_spacing_y;

    social_speech_output_x = output_panel_x + output_text_width + BORDER*1;
    social_speech_output_y = falling_speech_output_y + output_text_height + 4*button_spacing_y;

    text_input_width  = 3*control_panel_width/4 - BORDER;
    text_input_height = button_size_y;
   
    mutual_gaze_x      = text_output_x + text_output_width - BORDER - check_button_size_x;
    mutual_gaze_y      = text_output_y + text_output_height + button_spacing_y;

    gaze_variation_x      = text_output_x + BORDER;
    gaze_variation_y      = mutual_gaze_y;

    recognize_speech_x = text_output_x + text_output_width - BORDER - text_input_width;
    recognize_speech_y = mutual_gaze_y+text_input_height + 1*button_spacing_y;

    identify_face_expression_box_width = button_size_x;
    identify_face_expression_box_height = radio_button_size_y;


    identify_face_expression_box_x = text_output_x + BORDER;
    identify_face_expression_box_y = recognize_speech_y + button_size_y + 2*button_spacing_y;

    identify_face_expression_happy_x = identify_face_expression_box_x ;
    identify_face_expression_happy_y = identify_face_expression_box_y-BORDER + identify_face_expression_box_height;

	identify_face_expression_sad_x = identify_face_expression_happy_x + radio_button_size_x;
    identify_face_expression_sad_y = identify_face_expression_happy_y;

    identify_face_expression_angry_x = identify_face_expression_happy_x + 2*radio_button_size_x;
    identify_face_expression_angry_y = identify_face_expression_happy_y;

    identify_face_expression_fearful_x = identify_face_expression_happy_x  + 3*radio_button_size_x;
    identify_face_expression_fearful_y = identify_face_expression_happy_y;

    identify_face_expression_none_x = identify_face_expression_happy_x  + 4.2*radio_button_size_x;
    identify_face_expression_none_y = identify_face_expression_fearful_y;

    robot_speech_x = text_output_x + text_output_width - BORDER - text_input_width;
    robot_speech_y = identify_face_expression_fearful_y+text_input_height;
    
    deliberative_face_expression_box_x = text_output_x + BORDER;
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

    get_balance_x      = text_output_x + BORDER ;
    get_balance_y      = deliberative_face_expression_fearful_y +text_input_height + 2*button_spacing_y;

    get_head_touch_x      = text_output_x + text_output_width - BORDER - check_button_size_x;
    get_head_touch_y      = get_balance_y;

    get_left_hand_touch_x      = text_output_x + BORDER ;
    get_left_hand_touch_y      = get_head_touch_y +text_input_height;

    get_right_hand_touch_x      = text_output_x + text_output_width - BORDER - check_button_size_x;
    get_right_hand_touch_y      = get_left_hand_touch_y;

    get_left_foot_touch_x      = text_output_x + BORDER ;
    get_left_foot_touch_y      = get_left_hand_touch_y +text_input_height;

    get_right_foot_touch_x      = text_output_x + text_output_width - BORDER - check_button_size_x;
    get_right_foot_touch_y      = get_left_foot_touch_y;

    switchOff_blinking_x      = text_output_x + BORDER;
    switchOff_blinking_y      = get_right_foot_touch_y +text_input_height;

    switchOff_falling_x      = text_output_x + text_output_width - BORDER - check_button_size_x;
    switchOff_falling_y      = switchOff_blinking_y;

    switchOff_socialReaction_x      = text_output_x + BORDER;
    switchOff_socialReaction_y      = switchOff_falling_y +text_input_height;


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

      Fl_Box *control_panel;
      control_panel = new Fl_Box(control_panel_x, control_panel_y-BORDER, control_panel_width, control_panel_height,"" );
      control_panel->align(FL_ALIGN_TOP | FL_ALIGN_CENTER | FL_ALIGN_INSIDE);
      control_panel->labelsize(12);
      control_panel->labelfont(FL_HELVETICA_BOLD);
      //control_panel->box(FL_PLASTIC_DOWN_BOX);
      control_panel->box(FL_DOWN_BOX);

	  Fl_Box *output_panel;
      output_panel = new Fl_Box(output_panel_x, output_panel_y-BORDER, output_panel_width, output_panel_height,"" );
      output_panel->align(FL_ALIGN_TOP | FL_ALIGN_CENTER | FL_ALIGN_INSIDE);
      output_panel->labelsize(12);
      output_panel->labelfont(FL_HELVETICA_BOLD);
      output_panel->box(FL_DOWN_BOX);
      //output_panel->box(FL_PLASTIC_DOWN_BOX);

      /* Text output ... note: we don't declare text_output because it is declared in the guiUtilities library   */ 

      text_output = new Fl_Multiline_Output(text_output_x, text_output_y, text_output_width, text_output_height,"" );
      text_output->align(FL_ALIGN_RIGHT | FL_ALIGN_CENTER );
      text_output->labelsize(12);
      text_output->textsize(12);
      text_output->color(FL_GREEN);
      text_output->value("\n Please wait while ports are being connected ... ");
      //text_output->box(FL_THIN_DOWN_FRAME);  // matches the image display frames
      text_output->box(FL_PLASTIC_DOWN_BOX);   // rounded edges for something more visually appealing
 
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
      //tone_of_voice_box->box(FL_THIN_DOWN_FRAME);
      identify_face_expression_box->align(FL_ALIGN_TOP | FL_ALIGN_LEFT | FL_ALIGN_INSIDE);
      identify_face_expression_box->labelsize(12);
      identify_face_expression_box->labelfont(FL_HELVETICA_BOLD);
      //tone_of_voice_box->box(FL_PLASTIC_DOWN_BOX);
      //tone_of_voice_box->labelcolor(FL_BLUE);

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
      deliberative_face_expression_box = new Fl_Box(deliberative_face_expression_box_x, deliberative_face_expression_box_y, identify_face_expression_box_width, identify_face_expression_box_height,"Deliberative Facial Expression" );
      //tone_of_voice_box->box(FL_THIN_DOWN_FRAME);
      deliberative_face_expression_box->align(FL_ALIGN_TOP | FL_ALIGN_LEFT | FL_ALIGN_INSIDE);
      deliberative_face_expression_box->labelsize(12);
      deliberative_face_expression_box->labelfont(FL_HELVETICA_BOLD);
      //tone_of_voice_box->box(FL_PLASTIC_DOWN_BOX);
      //tone_of_voice_box->labelcolor(FL_BLUE);

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
      get_balance = new Fl_Check_Button(get_balance_x, get_balance_y, check_button_size_x, check_button_size_y,"get_balance");
      get_balance->callback(button_cb, &get_balance_value); 
      get_balance->labelsize(12);  
      get_balance->labelfont(FL_HELVETICA_BOLD);
      get_balance->when(FL_WHEN_ENTER_KEY | FL_WHEN_CHANGED); 
      get_balance->align(FL_ALIGN_RIGHT | FL_ALIGN_CENTER); 
      get_balance->value(1);

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

	  Fl_Check_Button *gaze_variation;
      gaze_variation = new Fl_Check_Button(gaze_variation_x, gaze_variation_y, check_button_size_x, check_button_size_y,"gaze_variation");
      gaze_variation->callback(button_cb, &gaze_variation_value); 
      gaze_variation->labelsize(12);  
      gaze_variation->labelfont(FL_HELVETICA_BOLD);
      gaze_variation->when(FL_WHEN_ENTER_KEY | FL_WHEN_CHANGED); 
      gaze_variation->align(FL_ALIGN_RIGHT | FL_ALIGN_CENTER); 
      gaze_variation->value(0);


      Fl_Check_Button *switchOff_blinking;
      switchOff_blinking = new Fl_Check_Button(switchOff_blinking_x, switchOff_blinking_y, check_button_size_x, check_button_size_y,"switchOff blinking");
      switchOff_blinking->callback(button_cb, &switchOff_blinking_value); 
      switchOff_blinking->labelsize(12);  
      switchOff_blinking->labelfont(FL_HELVETICA_BOLD);
      switchOff_blinking->when(FL_WHEN_ENTER_KEY | FL_WHEN_CHANGED); 
      switchOff_blinking->align(FL_ALIGN_RIGHT | FL_ALIGN_CENTER); 
      switchOff_blinking->value(0);

      Fl_Check_Button *switchOff_falling;
      switchOff_falling = new Fl_Check_Button(switchOff_falling_x, switchOff_falling_y, check_button_size_x, check_button_size_y,"switchOff falling");
      switchOff_falling->callback(button_cb, &switchOff_falling_value); 
      switchOff_falling->labelsize(12);  
      switchOff_falling->labelfont(FL_HELVETICA_BOLD);
      switchOff_falling->when(FL_WHEN_ENTER_KEY | FL_WHEN_CHANGED); 
      switchOff_falling->align(FL_ALIGN_LEFT | FL_ALIGN_CENTER); 
      switchOff_falling->value(0);

      Fl_Check_Button *switchOff_socialReaction;
      switchOff_socialReaction = new Fl_Check_Button(switchOff_socialReaction_x, switchOff_socialReaction_y, check_button_size_x, check_button_size_y,"switchOff socialReaction");
      switchOff_socialReaction->callback(button_cb, &switchOff_socialReaction_value); 
      switchOff_socialReaction->labelsize(12);  
      switchOff_socialReaction->labelfont(FL_HELVETICA_BOLD);
      switchOff_socialReaction->when(FL_WHEN_ENTER_KEY | FL_WHEN_CHANGED); 
      switchOff_socialReaction->align(FL_ALIGN_RIGHT | FL_ALIGN_CENTER); 
      switchOff_socialReaction->value(0);

	  Fl_Multiline_Output *blinking_rate;
	  blinking_rate = new Fl_Multiline_Output(blinking_rate_output_x, blinking_rate_output_x, output_text_width, output_text_height,"Blinking Rate:" );
      blinking_rate->align(FL_ALIGN_LEFT | FL_ALIGN_CENTER );
      blinking_rate->labelsize(12);
	  blinking_rate->labelfont(FL_HELVETICA_BOLD);
      blinking_rate->textsize(12);
      //blinking_rate->color(FL_GREEN);
      
      //text_output->box(FL_THIN_DOWN_FRAME);  // matches the image display frames
      blinking_rate->box(FL_PLASTIC_DOWN_BOX);   // rounded edges for something more visually appealing

	  Fl_Multiline_Output *falling_speech;
	  falling_speech = new Fl_Multiline_Output(falling_speech_output_x, falling_speech_output_y, output_text_width, output_text_height,"Falling Speech:" );
      falling_speech->align(FL_ALIGN_LEFT | FL_ALIGN_CENTER );
      falling_speech->labelsize(12);
	  falling_speech->labelfont(FL_HELVETICA_BOLD);
      falling_speech->textsize(12);
      //falling_speech->color(FL_GREEN);
      
      //text_output->box(FL_THIN_DOWN_FRAME);  // matches the image display frames
      falling_speech->box(FL_PLASTIC_DOWN_BOX);   // rounded edges for something more visually appealing
 
	  Fl_Multiline_Output *social_speech;
	  social_speech = new Fl_Multiline_Output(social_speech_output_x, social_speech_output_y, output_text_width, output_text_height,"Social Speech:" );
      social_speech->align(FL_ALIGN_LEFT | FL_ALIGN_CENTER );
      social_speech->labelsize(12);
	  social_speech->labelfont(FL_HELVETICA_BOLD);
      social_speech->textsize(12);
      //social_speech->color(FL_GREEN);
      
      //text_output->box(FL_THIN_DOWN_FRAME);  // matches the image display frames
      social_speech->box(FL_PLASTIC_DOWN_BOX);   // rounded edges for something more visually appealing
 

      /**************************************/
      /*                                    */
      /*     simulation parameters part     */
      /*                                    */
      /**************************************/

      /* box to frame controls */
   
   
    start_systemArchitecture_GUI->end();  // end of the group 

    mutual_gaze_value                               = 0;
    mutual_gaze_value_old                           = -1; // this forces the old value to be updated and the current value to be issued

    switchOff_blinking_value                        = 0;
    switchOff_blinking_value_old                    = -1; 

    switchOff_falling_value                         = 0;
    switchOff_falling_value_old                     = -1; 

    switchOff_socialReaction_value                  = 0;
    switchOff_socialReaction_value_old              = -1; 

    identify_face_expression_value                  = 1;
    identify_face_expression_value_old              = -1;

   	deliberative_face_expression_value              = 1;
	deliberative_face_expression_value_old          = -1;

	get_balance_value                               = 1;
	get_balance_value_old                           = -1;
	get_head_touch_value                            = 0;
	get_head_touch_value_old                        =-1;
	get_left_hand_touch_value                       = 0;
	get_left_hand_touch_value_old                   = -1;
	get_right_hand_touch_value                      = 0;
	get_right_hand_touch_value_old                  = -1;
	get_left_foot_touch_value                       = 0;
	get_left_foot_touch_value_old                   = -1;
	get_right_foot_touch_value                      = 0;
	get_right_foot_touch_value_old                  = -1;
	
	gaze_variation_value                            = 0;
	gaze_variation_value_old                        = -1;

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
         
            VectorOf<int> &dataVector = getBalancePortOut->prepare();
            dataVector.resize(1);

            dataVector(0) = get_balance_value;

            getBalancePortOut->write(); 
         
        }
      
	    /* get_head_touch_value primitive */
        if (get_head_touch_value_old != get_head_touch_value) {
            get_head_touch_value_old = get_head_touch_value;
         
            Bottle &dataVector = getHeadTactilePortOut->prepare();
            dataVector.clear();
		    if(get_head_touch_value){
			    dataVector.addString("Front1");
            }
		    else{
			    dataVector.addString("Front0");
            }

            getHeadTactilePortOut->write(); 
         
        }

	    /* get_left_hand_touch_value primitive */
        if (get_left_hand_touch_value_old != get_left_hand_touch_value) {
            get_left_hand_touch_value_old = get_left_hand_touch_value;
         
            Bottle &dataVector = getHandTactilePortOut->prepare();
            dataVector.clear();
		    if(get_left_hand_touch_value){
			    dataVector.addString("Left1");
            }
		    else{
			    dataVector.addString("Left0");
            }

            getHandTactilePortOut->write(); 
         
        }

	    /* get_right_hand_touch_value primitive */
        if (get_right_hand_touch_value_old != get_right_hand_touch_value) {
            get_right_hand_touch_value_old = get_right_hand_touch_value;
         
            Bottle &dataVector = getHandTactilePortOut->prepare();
            dataVector.clear();
		    if(get_right_hand_touch_value){
			    dataVector.addString("Right1");
            }
		    else{
			    dataVector.addString("Right0");
            }

            getHandTactilePortOut->write(); 
          
         
        }
	    /* get_left_foot_touch_value primitive */
        if (get_left_foot_touch_value_old != get_left_foot_touch_value) {
            get_left_foot_touch_value_old = get_left_foot_touch_value;
         
            Bottle &dataVector = getBumperTactilePortOut->prepare();
            dataVector.clear();
		    if(get_left_foot_touch_value){
			    dataVector.addString("Left1");
            }
		    else{
			    dataVector.addString("Left0");
            }

            getBumperTactilePortOut->write(); 
         
        }

	    /* get_right_foot_touch_value primitive */
        if (get_right_foot_touch_value_old != get_right_foot_touch_value) {
            get_right_foot_touch_value_old = get_right_foot_touch_value;
         
            Bottle &dataVector = getBumperTactilePortOut->prepare();
            dataVector.clear();
		    if(get_right_foot_touch_value){
			    dataVector.addString("Right1");
            }
		    else{
			    dataVector.addString("Right0");
            }

            getBumperTactilePortOut->write();  
         
        }

        /* switchOff_blinking_value */
        if (switchOff_blinking_value_old != switchOff_blinking_value) {
            switchOff_blinking_value_old = switchOff_blinking_value;
         
            VectorOf<int> &dataVector = eyeBlinkingSwitchOffPortOut->prepare();
            dataVector.resize(1);

            dataVector(0) = switchOff_blinking_value;

            eyeBlinkingSwitchOffPortOut->write(); 
         
        }

        /* switchOff_falling_value */
        if (switchOff_falling_value_old != switchOff_falling_value) {
            switchOff_falling_value_old = switchOff_falling_value;
         
            VectorOf<int> &dataVector = fallingReactionSwitchOffPortOut->prepare();
            dataVector.resize(1);

            dataVector(0) = switchOff_falling_value;

            fallingReactionSwitchOffPortOut->write(); 
         
        }

        /* switchOff_socialReaction_value */
        if (switchOff_socialReaction_value_old != switchOff_socialReaction_value) {
            switchOff_socialReaction_value_old = switchOff_socialReaction_value;
         
            VectorOf<int> &dataVector = socialReactionSwitchOffPortOut->prepare();
            dataVector.resize(1);

            dataVector(0) = switchOff_socialReaction_value;

            socialReactionSwitchOffPortOut->write(); 
         
        }

	    /* gaze_variation_value primitive */
        if (gaze_variation_value_old != gaze_variation_value) {
            gaze_variation_value_old = gaze_variation_value;
         
            VectorOf<int> &dataVector = getGazeVariationPortOut->prepare();
            dataVector.resize(1);

            dataVector(0) = gaze_variation_value;

            getGazeVariationPortOut->write(); 
         
        }


        /* robot_speech_value(text, tone) primitive */
        if (strlen(robot_speech_value) != 0) {

            Bottle &message = getSpeechPortOut->prepare();

            message.clear();
            message.addString(robot_speech_value);
         
         
            getSpeechPortOut->write(); 
            robot_speech_value[0] = '\0'; // clear the message
      
        }

	    /* recognize_speech_value(text, tone) primitive */
        if (strlen(recognize_speech_value) != 0) {

            Bottle &message = recognizeSpeechPortOut->prepare();

            message.clear();
            message.addString(recognize_speech_value);
         
            recognizeSpeechPortOut->write(); 
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
         
            VectorOf<int> &dataVector = getDeliberativeFacialExpressionPortOut->prepare(); 
            dataVector.resize(1);

            dataVector(0) = deliberative_face_expression_value;

            getDeliberativeFacialExpressionPortOut->write(); 
         
        }

	    
	    /*Output fields*/

	    tempBottle = getEyeBlinkingPortIn->read(false);                   // non-blocking version
        if (tempBottle != NULL) {
            strcpy(modifiedString, (*tempBottle).get(0).toString().c_str());
		    strcat(modifiedString, (*tempBottle).get(1).toString().c_str());
		    strcat(modifiedString, (*tempBottle).get(2).toString().c_str());
		    blinking_rate->value(modifiedString);
		    DVwindow->redraw();
		    Fl::check();
        }

        tempBottleAux = moveSequencePortIn->read(false);                   // non-blocking version
        if (tempBottleAux != NULL) {
            strcpy(modifiedString, (*tempBottleAux).get(0).toString().c_str());
		    message(modifiedString);
		    DVwindow->redraw();
		    Fl::check();
        }

        tempIntVector = getFallingReactionSpeechPortIn->read(false);                   // non-blocking version
        if (tempIntVector != NULL) {
            
            tempBottle = sayPortIn->read(false);                   // non-blocking version
            if (tempBottle != NULL) {
                strcpy(modifiedString, (*tempBottle).get(0).toString().c_str());
		        falling_speech->value(modifiedString);
                social_speech->value("");
		        DVwindow->redraw();
		        Fl::check();
            }
        }
        else{ //the social speech will be printed only when there is no falling speech
            tempIntVector = getSocialReactionSpeechPortIn->read(false);                   // non-blocking version
            if (tempIntVector != NULL) {
            
                tempBottle = sayPortIn->read(false);                   // non-blocking version
                if (tempBottle != NULL) {
                    strcpy(modifiedString, (*tempBottle).get(0).toString().c_str());
		            social_speech->value(modifiedString);
                    falling_speech->value("");
		            DVwindow->redraw();
		            Fl::check();
                }
            }
        }
        
	    tempIntVector = getInterruptionPortIn->read(false);                   // non-blocking version
        if (tempIntVector != NULL) {
            p = (*tempIntVector)[0];

		    /*If true it blocks all control panel inputs*/
		    if(p ==1){
			    gaze_variation->deactivate();
			 
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
			    sprintf(modifiedString,"\n System interrupted by fall. ");
                message(modifiedString);

		 }
		 else if (p==0){
			 gaze_variation->activate();
			 
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
			 sprintf(modifiedString,"\n System restored after fall. ");
             message(modifiedString);

		 }          
      }
   }
}

void ReactiveSystemGUIThread::threadRelease() 
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