/* 
 * Copyright (C) 2014 DREAM Consortium
 * FP7 Project 611391 co-funded by the European Commission
 *
 * Author:  David Vernon, University of Skövde 
 * Email:   david.vernon@his.se 
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
 * 08/12/2014  First version validated (David Vernon)
 *
 */ 
 
#include "systemArchitectureGUI.h"

SystemArchitectureGUIThread::SystemArchitectureGUIThread(
                                                         bool                            *verboseValue,          // simulation control parameters
                                                         bool                            *cycleValue,
                                                         double                          *cyclePeriodValue,
                                                         double                          *eventRateValue,            
                                                         Port                            *sensoryInterpretationOut,                            
                                                         BufferedPort<VectorOf<double>>  *getChildBehaviourIn,   
                                                         BufferedPort<VectorOf<double>>  *getChildMotivationIn, 
                                                         BufferedPort<VectorOf<double>>  *getChildPerformanceIn, 
                                                         Port                            *childBehaviourClassificationOut, 
                                                         BufferedPort<VectorOf<int>>     *getInterventionStateIn,
                                                         BufferedPort<VectorOf<int>>     *gripOut,
                                                         BufferedPort<VectorOf<double>>  *moveHandOut,
                                                         BufferedPort<VectorOf<double>>  *moveHeadOut,
                                                         BufferedPort<VectorOf<int>>     *moveSequenceOut,
                                                         BufferedPort<VectorOf<double>>  *moveTorsoOut,
                                                         BufferedPort<Bottle>            *sayOut,
                                                         BufferedPort<VectorOf<int>>     *enableRobotOut,
                                                         Port                            *cognitiveControlOut,
                                                         string logoFilename)
{
   verbose                             = verboseValue;    
   cycle                               = cycleValue;
   cyclePeriod                         = cyclePeriodValue;
   eventRate                           = eventRateValue;

   sensoryInterpretationPortOut        = sensoryInterpretationOut;
      
   getChildBehaviourPortIn             = getChildBehaviourIn;
   getChildMotivationPortIn            = getChildMotivationIn;
   getChildPerformancePortIn           = getChildPerformanceIn;
   childBehaviourClassificationPortOut = childBehaviourClassificationOut;

   getInterventionStatePortIn          = getInterventionStateIn;  
   gripPortOut                         = gripOut;
   moveHandPortOut                     = moveHandOut;
   moveHeadPortOut                     = moveHeadOut;
   moveSequencePortOut                 = moveSequenceOut;
   moveTorsoPortOut                    = moveTorsoOut;
   sayPortOut                          = sayOut;
   enableRobotPortOut                  = enableRobotOut;
   cognitiveControlPortOut             = cognitiveControlOut;
   logoFilenameValue                   = logoFilename;
}

bool SystemArchitectureGUIThread::threadInit() 
{
   /* initialize variables and create data-structures if needed */

   debug = false;

   return true;
}

void SystemArchitectureGUIThread::run(){

   /* 
    * Here we set up the GUI for the system architecture
    * 
    * It provides a visualization of the outputs from the childBehaviourClassification subsystem and cognitiveControl subsystem
    * It also provides a means of providing input to the cognitiveControl subsystem
    */ 

   int p, q, r;
   int i, j;
   double x, y;
   int max_amplitude;       // maximum amplitude of the plot display
   int index1;
   int index2;
   int index3;
   int index4;
   int index5;

   extern DVdisplay *display1;
   extern DVdisplay *display2;
   extern DVdisplay *display3;
   extern DVdisplay *display4;
   extern DVdisplay *display5;
   extern DVimage   *image1;
   extern DVimage   *image2;
   extern DVimage   *image3;
   extern DVimage   *image4;
   extern DVimage   *image5;

   float *buffer1 = NULL;  
   float *buffer2 = NULL;  
   float *buffer3 = NULL;  
   float *buffer4 = NULL;   
   float *buffer5 = NULL;

   Fl_Group *start_systemArchitecture_GUI;
	Fl_Box *box1;
   Fl_Box *box2;
   Fl_Box *box3;
   Fl_Box *box4;   
   Fl_Box *box5;
   Fl_Box *box6;
   Fl_Box *box7;
   Fl_Box *box8;
   Fl_Box *box9;
   Fl_Box *heading;
   Fl_PNG_Image *logo=new Fl_PNG_Image(logoFilenameValue.c_str());
 
   Bottle command;
   VectorOf<double>  *tempDoubleVector;
   VectorOf<int>     *tempIntVector;
   //Bottle            *tempBottle;  
   char modifiedString[STRINGLENGTH]; 

	/* variable that determine the presentation of the GUI */ 

	int control_panel_x;
   int control_panel_y;
   int control_panel_width;
   int control_panel_height;
   int simulation_panel_x, simulation_panel_y;
   int simulation_panel_width, simulation_panel_height;
   int simulation_panel_offset;
	int heading_x, heading_y, heading_width, heading_height;
	int display_width, display_height;
	int display_x1, display_x2, display_x3, display_x4, display_x5;
   int display_y1, display_y2, display_y3, display_y4, display_y5;
	int display_spacing;
	int display_offset_x, display_offset_y;
	int control_offset_x, control_offset_y;
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
   int text_output_x;
   int text_output_y;
   int enable_robot_x;
   int enable_robot_y;
   int enable_robot_value;
   int enable_robot_value_old;
   int grip_x;
   int grip_y;
   int grip_value;
   int grip_value_old;
   int move_left_hand_x;
   int move_left_hand_y;
   double move_left_hand_x_value;
   double move_left_hand_y_value;
   double move_left_hand_z_value;
   double move_left_hand_r_value;
   double move_left_hand_x_value_old;
   double move_left_hand_y_value_old;
   double move_left_hand_z_value_old;
   double move_left_hand_r_value_old;
   int move_right_hand_x;
   int move_right_hand_y;
   double move_right_hand_x_value;
   double move_right_hand_y_value;
   double move_right_hand_z_value;
   double move_right_hand_r_value;
   double move_right_hand_x_value_old;
   double move_right_hand_y_value_old;
   double move_right_hand_z_value_old;
   double move_right_hand_r_value_old;
   int move_head_x;
   int move_head_y;
   double move_head_x_value;
   double move_head_y_value;
   double move_head_z_value;
   double move_head_x_value_old;
   double move_head_y_value_old;
   double move_head_z_value_old;
   int move_torso_x;
   int move_torso_y;
   double move_torso_x_value;
   double move_torso_y_value;
   double move_torso_z_value;
   double move_torso_x_value_old;
   double move_torso_y_value_old;
   double move_torso_z_value_old;
   int move_sequence_x;
   int move_sequence_y;
   int move_sequence_value;
   int move_sequence_value_old;
   char move_sequence_string[STRINGLENGTH]; 
   int say_x;
   int say_y;
   char say_value[STRINGLENGTH]; 
   int tone_of_voice_value;
   int tone_of_voice_value_old;
   int tone_of_voice_box_x;
   int tone_of_voice_box_y;
   int tone_of_voice_box_width;
   int tone_of_voice_box_height;
   int happy_voice_x;
   int happy_voice_y;
   int sad_voice_x;
   int sad_voice_y;
   int angry_voice_x;
   int angry_voice_y;
   int fearful_voice_x;
   int fearful_voice_y;
   int radio_button_size_x;
   int radio_button_size_y;
   int radio_button_spacing_y;
   int hslider_size_x;
   int hslider_size_y;
   int vslider_size_x;
   int vslider_size_y;
   int sensory_interpretation_x1;
   int sensory_interpretation_x2;
   int sensory_interpretation_x3;
   int sensory_interpretation_x4;
   int sensory_interpretation_y1;
	int sensory_interpretation_verbose_value;
   int sensory_interpretation_verbose_value_old;
   int sensory_interpretation_cycle_value;
   int sensory_interpretation_cycle_value_old;
   double sensory_interpretation_event_rate;
   double sensory_interpretation_event_rate_old;
   double sensory_interpretation_cycle_period;
   double sensory_interpretation_cycle_period_old;
   int child_behaviour_classification_x1;
   int child_behaviour_classification_x2;
   int child_behaviour_classification_x3;
   int child_behaviour_classification_x4;
   int child_behaviour_classification_y1;
	int child_behaviour_classification_verbose_value;
   int child_behaviour_classification_verbose_value_old;
   int child_behaviour_classification_cycle_value;
   int child_behaviour_classification_cycle_value_old;
   double child_behaviour_classification_event_rate;
   double child_behaviour_classification_event_rate_old;
   double child_behaviour_classification_cycle_period;
   double child_behaviour_classification_cycle_period_old;  
   int cognitive_control_x1;
   int cognitive_control_x2;
   int cognitive_control_x3;
   int cognitive_control_x4;
   int cognitive_control_y1;
	int cognitive_control_verbose_value;
	int cognitive_control_verbose_value_old;
	int cognitive_control_cycle_value;
	int cognitive_control_cycle_value_old;
   double cognitive_control_event_rate;  
   double cognitive_control_event_rate_old;
   double cognitive_control_cycle_period;
   double cognitive_control_cycle_period_old;
   int system_architecture_GUI_x1;
   int system_architecture_GUI_x2;
   int system_architecture_GUI_x3;
   int system_architecture_GUI_x4;
   int system_architecture_GUI_y1;
   int system_architecture_GUI_verbose_value;
   int system_architecture_GUI_verbose_value_old;
   int system_architecture_GUI_cycle_value;
   int system_architecture_GUI_cycle_value_old;
   double system_architecture_GUI_event_rate;
   double system_architecture_GUI_event_rate_old;
   double system_architecture_GUI_cycle_period;
   double system_architecture_GUI_cycle_period_old;


   /* intialize the presentation settings */

   border_width  = GetSystemMetrics(SM_CXSIZEFRAME);  
   border_height = GetSystemMetrics(SM_CYSIZEFRAME);

	display_width  = BEHAVIOUR_DISPLAY_WIDTH;   
	display_height = BEHAVIOUR_DISPLAY_HEIGHT;
 	display_spacing = 30;
    
   title_height = 12;

	control_offset_x = 20;
	control_offset_y = 0;

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

   hslider_size_x      = 100;
   hslider_size_y      = button_size_y;
   vslider_size_x      = button_size_y;
   vslider_size_y      = 75;


   //window_width  = GetSystemMetrics(SM_CXFULLSCREEN);                      // this is the area available for the window on the screen
   window_width  = (display_width  + display_spacing) * 2 + display_spacing; // we initialize window_height once we have worked out the heading values

	heading_x = (window_width - display_width*2 - display_spacing)/2; // where to begin the column of displays
	heading_y = 0; // MENUBAR_HEIGHT;
	heading_width = display_width*2 + display_spacing;                // centre over the two columns: child behaviour column and cognitive control column
	heading_height = HEADING_HEIGHT*2;                                // logo and caption
	
   display_offset_x = heading_x ;
	display_offset_y = heading_y + heading_height;
	display_x1 = display_offset_x;
   display_x2 = display_offset_x;
   display_x3 = display_offset_x;
   display_x4 = display_offset_x;
   display_x5 = display_offset_x;
   
	display_y1 = display_offset_y;
   display_y2 = display_y1 + display_height + display_spacing; 
   display_y3 = display_y2 + display_height + display_spacing;
   display_y4 = display_y3 + display_height + display_spacing;   
   display_y5 = display_y4 + display_height + display_spacing; 

   control_panel_width  = display_width * 1;
   control_panel_height = display_height*5 + display_spacing*4 + BORDER*2;  
   control_panel_x = display_x1 + display_width + display_spacing;
   control_panel_y = display_y1;
  
   simulation_panel_width  = display_width * 2 + display_spacing;
   simulation_panel_height = title_height + 4*(5 + button_size_y) + title_height;  
   simulation_panel_x      = display_x1;
   simulation_panel_y      = display_y5 + display_height + display_spacing;
   simulation_panel_offset = 250;  

   //window_height = GetSystemMetrics(SM_CYFULLSCREEN);    
   window_height = heading_height + display_height * 5 + display_spacing * 6 + simulation_panel_height ; 

   sensory_interpretation_x1 = simulation_panel_x + simulation_panel_offset;
   sensory_interpretation_x2 = sensory_interpretation_x1 + button_size_x + button_spacing_x;
   sensory_interpretation_x3 = sensory_interpretation_x2 + 1*button_size_x;
   sensory_interpretation_x4 = sensory_interpretation_x3 + hslider_size_x + 2*button_spacing_x;
   sensory_interpretation_y1 = simulation_panel_y + 	2*button_spacing_y + title_height;

   child_behaviour_classification_x1 = simulation_panel_x + simulation_panel_offset;
   child_behaviour_classification_x2 = child_behaviour_classification_x1 + button_size_x + button_spacing_x;
   child_behaviour_classification_x3 = child_behaviour_classification_x2 + 1*button_size_x;
   child_behaviour_classification_x4 = child_behaviour_classification_x3 + hslider_size_x + 2*button_spacing_x;
   child_behaviour_classification_y1 = sensory_interpretation_y1 + button_spacing_y + button_size_y;

   cognitive_control_x1 = simulation_panel_x + simulation_panel_offset;
   cognitive_control_x2 = cognitive_control_x1 + button_size_x + button_spacing_x;
   cognitive_control_x3 = cognitive_control_x2 + 1*button_size_x;
   cognitive_control_x4 = cognitive_control_x3 + hslider_size_x + 2*button_spacing_x;
   cognitive_control_y1 = child_behaviour_classification_y1 + button_spacing_y + button_size_y;

   system_architecture_GUI_x1 = simulation_panel_x + simulation_panel_offset;
   system_architecture_GUI_x2 = system_architecture_GUI_x1 + button_size_x + button_spacing_x;
   system_architecture_GUI_x3 = system_architecture_GUI_x2 + 1*button_size_x;
   system_architecture_GUI_x4 = system_architecture_GUI_x3 + hslider_size_x + 2*button_spacing_x;
   system_architecture_GUI_y1 = cognitive_control_y1 + button_spacing_y + button_size_y;

   text_output_width  = control_panel_width - BORDER*2;
   text_output_height = display_height;
   text_output_x = control_panel_x + BORDER*1;
   text_output_y = control_panel_y; // + BORDER;
   
   text_input_width  = 3*control_panel_width/4 - BORDER;
   text_input_height = button_size_y;
   
   enable_robot_x      = text_output_x + text_output_width - BORDER - check_button_size_x;
   enable_robot_y      = text_output_y + text_output_height + button_spacing_y;

   grip_x              = text_output_x + BORDER;
   grip_y              = enable_robot_y;

   move_left_hand_x    = text_output_x + BORDER;
   move_left_hand_y    = grip_y + 2*button_size_y;

   move_right_hand_x   = text_output_x + text_output_width - BORDER - 4* vslider_size_x - 3*button_spacing_x;
   move_right_hand_y   = move_left_hand_y;

   move_head_x         = move_left_hand_x + (vslider_size_x + button_spacing_x)/2;
   move_head_y         = move_left_hand_y  + vslider_size_y + button_size_y + 3*button_spacing_y;

   move_torso_x        = text_output_x + text_output_width - BORDER - 4* vslider_size_x - 3*button_spacing_x + (vslider_size_x + button_spacing_x)/2;
   move_torso_y        = move_left_hand_y  + vslider_size_y + button_size_y + 3*button_spacing_y;

   move_sequence_x     = text_output_x + text_output_width - BORDER - input_size_x;
   move_sequence_y     = move_head_y + vslider_size_y + button_size_y + 1*button_spacing_y;

   say_x               = text_output_x + text_output_width - BORDER - text_input_width;
   say_y               = move_sequence_y + text_input_height + 1*button_spacing_y;
   
   tone_of_voice_box_width = button_size_x;
	tone_of_voice_box_height = radio_button_size_y;
   tone_of_voice_box_x = text_output_x + BORDER;
   tone_of_voice_box_y = say_y + button_size_y + button_spacing_y;

   happy_voice_x = tone_of_voice_box_x + tone_of_voice_box_width;
   happy_voice_y = tone_of_voice_box_y-BORDER;

	sad_voice_x = tone_of_voice_box_x + tone_of_voice_box_width + radio_button_size_x;
   sad_voice_y = happy_voice_y;

   angry_voice_x = tone_of_voice_box_x + tone_of_voice_box_width + 2*radio_button_size_x;
   angry_voice_y = happy_voice_y;

   fearful_voice_x = tone_of_voice_box_x + tone_of_voice_box_width + 3*radio_button_size_x;
   fearful_voice_y = happy_voice_y;


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
      /* child behaviour classfication part */
      /*                                    */
      /**************************************/

      /* boxes to frame images ... note: we don't declare box1 or box2 because they are declared in the guiUtilities library   */ 

      box1 = new Fl_Box(display_x1-BORDER,display_y1-BORDER, display_width+2*BORDER,display_height+2*BORDER,"Child Behaviour Class PDF" );
      box1->box(FL_DOWN_BOX);
      box1->align(FL_ALIGN_BOTTOM | FL_ALIGN_CENTER);
      box1->labelsize(12);
 
      box2 = new Fl_Box(display_x2-BORDER,display_y2-BORDER, display_width+2*BORDER,display_height+2*BORDER,"Level of Engagement" );
      box2->box(FL_DOWN_BOX);
      box2->align(FL_ALIGN_BOTTOM | FL_ALIGN_CENTER);
      box2->labelsize(12);

      box3 = new Fl_Box(display_x3-BORDER,display_y3-BORDER, display_width+2*BORDER,display_height+2*BORDER,"Confidence in Engagement Value" );
      box3->box(FL_DOWN_BOX);
      box3->align(FL_ALIGN_BOTTOM | FL_ALIGN_CENTER);
      box3->labelsize(12);

      box4 = new Fl_Box(display_x4-BORDER,display_y4-BORDER, display_width+2*BORDER,display_height+2*BORDER,"Level of Performance" );
      box4->box(FL_DOWN_BOX);
      box4->align(FL_ALIGN_BOTTOM | FL_ALIGN_CENTER);
      box4->labelsize(12);

      box5 = new Fl_Box(display_x5-BORDER,display_y5-BORDER, display_width+2*BORDER,display_height+2*BORDER,"Confidence in Performance Value" );
      box5->box(FL_DOWN_BOX);
      box5->align(FL_ALIGN_BOTTOM | FL_ALIGN_CENTER);
      box5->labelsize(12);
 
      /* image display ... note: we don't declare display1 or display2 because they declared in the guiUtilities library   */ 
      
      display1 = new DVdisplay(display_x1, display_y1, display_width,display_height);
      display2 = new DVdisplay(display_x2, display_y2, display_width,display_height);
      display3 = new DVdisplay(display_x3, display_y3, display_width,display_height);
      display4 = new DVdisplay(display_x4, display_y4, display_width,display_height);
      display5 = new DVdisplay(display_x5, display_y5, display_width,display_height);

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

      Fl_Check_Button *enable_robot;
      enable_robot = new Fl_Check_Button(enable_robot_x, enable_robot_y, check_button_size_x, check_button_size_y,"Enable robot control");
      enable_robot->callback(button_cb, &enable_robot_value); 
      enable_robot->labelsize(12);  
      enable_robot->labelfont(FL_HELVETICA_BOLD);
      enable_robot->when(FL_WHEN_ENTER_KEY | FL_WHEN_CHANGED); 
      enable_robot->align(FL_ALIGN_LEFT | FL_ALIGN_CENTER); 
      enable_robot->value(0);

      /* grip check box */

      Fl_Check_Button *grip;
      grip = new Fl_Check_Button(grip_x, grip_y, check_button_size_x, check_button_size_y,"Activate gripper");
      grip->callback(button_cb, &grip_value); 
      grip->labelsize(12);         
      grip->labelfont(FL_HELVETICA_BOLD);
      grip->when(FL_WHEN_ENTER_KEY | FL_WHEN_CHANGED); 
      grip->align(FL_ALIGN_RIGHT | FL_ALIGN_CENTER); 
      grip->value(0);

      /* move left hand sliders */

      Fl_Box *hand_slider_label;
      hand_slider_label = new Fl_Box(move_left_hand_x, move_left_hand_y+vslider_size_y + button_size_y, 0, 0,"        Left Hand                                         Right Hand" );
      hand_slider_label->align(FL_ALIGN_BOTTOM | FL_ALIGN_LEFT | FL_ALIGN_INSIDE);
      hand_slider_label->labelsize(12);
      hand_slider_label->labelfont(FL_HELVETICA_BOLD);
      hand_slider_label->box(FL_DOWN_BOX);
 
      Fl_Value_Slider *move_left_hand_x_slider;
      move_left_hand_x_slider = new Fl_Value_Slider(move_left_hand_x, move_left_hand_y, vslider_size_x, vslider_size_y,"X");
      move_left_hand_x_slider->type(FL_VERT_NICE_SLIDER);
      move_left_hand_x_slider->textsize(10);
      move_left_hand_x_slider->callback(valuator_cb, &move_left_hand_x_value);
      move_left_hand_x_slider->labelsize(10); 
      //move_left_hand_x_slider->labelfont(FL_HELVETICA_BOLD);  
      move_left_hand_x_slider->when(FL_WHEN_ENTER_KEY | FL_WHEN_CHANGED | FL_WHEN_RELEASE); 
      move_left_hand_x_slider->align(FL_ALIGN_TOP);
      move_left_hand_x_slider->minimum(100);
      move_left_hand_x_slider->maximum(-100);
      move_left_hand_x_slider->step(1);
      move_left_hand_x_slider->value(0); 
      move_left_hand_x_slider->box(FL_PLASTIC_DOWN_BOX);

      Fl_Value_Slider *move_left_hand_y_slider;
      move_left_hand_y_slider = new Fl_Value_Slider(move_left_hand_x+(vslider_size_x+button_spacing_x), move_left_hand_y, vslider_size_x, vslider_size_y,"Y");
      move_left_hand_y_slider->type(FL_VERT_NICE_SLIDER);
      move_left_hand_y_slider->textsize(10);
      move_left_hand_y_slider->callback(valuator_cb, &move_left_hand_y_value);
      move_left_hand_y_slider->labelsize(10); 
      //move_left_hand_y_slider->labelfont(FL_HELVETICA_BOLD);  
      move_left_hand_y_slider->when(FL_WHEN_ENTER_KEY | FL_WHEN_CHANGED | FL_WHEN_RELEASE); 
      move_left_hand_y_slider->align(FL_ALIGN_TOP);
      move_left_hand_y_slider->minimum(100);
      move_left_hand_y_slider->maximum(-100);
      move_left_hand_y_slider->step(1);
      move_left_hand_y_slider->value(0); 
      move_left_hand_y_slider->box(FL_PLASTIC_DOWN_BOX);

      Fl_Value_Slider *move_left_hand_z_slider;
      move_left_hand_z_slider = new Fl_Value_Slider(move_left_hand_x+2*(vslider_size_x+button_spacing_x), move_left_hand_y, vslider_size_x, vslider_size_y,"Z");
      move_left_hand_z_slider->type(FL_VERT_NICE_SLIDER);
      move_left_hand_z_slider->textsize(10);
      move_left_hand_z_slider->callback(valuator_cb, &move_left_hand_z_value);
      move_left_hand_z_slider->labelsize(10); 
      //move_left_hand_z_slider->labelfont(FL_HELVETICA_BOLD);  
      move_left_hand_z_slider->when(FL_WHEN_ENTER_KEY | FL_WHEN_CHANGED | FL_WHEN_RELEASE); 
      move_left_hand_z_slider->align(FL_ALIGN_TOP);
      move_left_hand_z_slider->minimum(100);
      move_left_hand_z_slider->maximum(-100);
      move_left_hand_z_slider->step(1);
      move_left_hand_z_slider->value(0); 
      move_left_hand_z_slider->box(FL_PLASTIC_DOWN_BOX);

      Fl_Value_Slider *move_left_hand_r_slider;
      move_left_hand_r_slider = new Fl_Value_Slider(move_left_hand_x+3*(vslider_size_x+button_spacing_x), move_left_hand_y, vslider_size_x, vslider_size_y,"R");
      move_left_hand_r_slider->type(FL_VERT_NICE_SLIDER);
      move_left_hand_r_slider->textsize(10);
      move_left_hand_r_slider->callback(valuator_cb, &move_left_hand_r_value);
      move_left_hand_r_slider->labelsize(10); 
      //move_left_hand_r_slider->labelfont(FL_HELVETICA_BOLD);  
      move_left_hand_r_slider->when(FL_WHEN_ENTER_KEY | FL_WHEN_CHANGED | FL_WHEN_RELEASE); 
      move_left_hand_r_slider->align(FL_ALIGN_TOP);
      move_left_hand_r_slider->minimum(100);
      move_left_hand_r_slider->maximum(-100);
      move_left_hand_r_slider->step(1);
      move_left_hand_r_slider->value(0); 
      move_left_hand_r_slider->box(FL_PLASTIC_DOWN_BOX);

      /* move right hand sliders */
 
      Fl_Value_Slider *move_right_hand_x_slider;
      move_right_hand_x_slider = new Fl_Value_Slider(move_right_hand_x, move_right_hand_y, vslider_size_x, vslider_size_y,"X");
      move_right_hand_x_slider->type(FL_VERT_NICE_SLIDER);
      move_right_hand_x_slider->textsize(10);
      move_right_hand_x_slider->callback(valuator_cb, &move_right_hand_x_value);
      move_right_hand_x_slider->labelsize(10); 
      //move_right_hand_x_slider->labelfont(FL_HELVETICA_BOLD);  
      move_right_hand_x_slider->when(FL_WHEN_ENTER_KEY | FL_WHEN_CHANGED | FL_WHEN_RELEASE); 
      move_right_hand_x_slider->align(FL_ALIGN_TOP);
      move_right_hand_x_slider->minimum(100);
      move_right_hand_x_slider->maximum(-100);
      move_right_hand_x_slider->step(1);
      move_right_hand_x_slider->value(0); 
      move_right_hand_x_slider->box(FL_PLASTIC_DOWN_BOX);

      Fl_Value_Slider *move_right_hand_y_slider;
      move_right_hand_y_slider = new Fl_Value_Slider(move_right_hand_x+(vslider_size_x+button_spacing_x), move_right_hand_y, vslider_size_x, vslider_size_y,"Y");
      move_right_hand_y_slider->type(FL_VERT_NICE_SLIDER);
      move_right_hand_y_slider->textsize(10);
      move_right_hand_y_slider->callback(valuator_cb, &move_right_hand_y_value);
      move_right_hand_y_slider->labelsize(10); 
      //move_right_hand_y_slider->labelfont(FL_HELVETICA_BOLD);  
      move_right_hand_y_slider->when(FL_WHEN_ENTER_KEY | FL_WHEN_CHANGED | FL_WHEN_RELEASE); 
      move_right_hand_y_slider->align(FL_ALIGN_TOP);
      move_right_hand_y_slider->minimum(100);
      move_right_hand_y_slider->maximum(-100);
      move_right_hand_y_slider->step(1);
      move_right_hand_y_slider->value(0); 
      move_right_hand_y_slider->box(FL_PLASTIC_DOWN_BOX);

      Fl_Value_Slider *move_right_hand_z_slider;
      move_right_hand_z_slider = new Fl_Value_Slider(move_right_hand_x+2*(vslider_size_x+button_spacing_x), move_right_hand_y, vslider_size_x, vslider_size_y,"Z");
      move_right_hand_z_slider->type(FL_VERT_NICE_SLIDER);
      move_right_hand_z_slider->textsize(10);
      move_right_hand_z_slider->callback(valuator_cb, &move_right_hand_z_value);
      move_right_hand_z_slider->labelsize(10); 
      //move_right_hand_z_slider->labelfont(FL_HELVETICA_BOLD);  
      move_right_hand_z_slider->when(FL_WHEN_ENTER_KEY | FL_WHEN_CHANGED | FL_WHEN_RELEASE); 
      move_right_hand_z_slider->align(FL_ALIGN_TOP);
      move_right_hand_z_slider->minimum(100);
      move_right_hand_z_slider->maximum(-100);
      move_right_hand_z_slider->step(1);
      move_right_hand_z_slider->value(0); 
      move_right_hand_z_slider->box(FL_PLASTIC_DOWN_BOX);

      Fl_Value_Slider *move_right_hand_r_slider;
      move_right_hand_r_slider = new Fl_Value_Slider(move_right_hand_x+3*(vslider_size_x+button_spacing_x), move_right_hand_y, vslider_size_x, vslider_size_y,"R");
      move_right_hand_r_slider->type(FL_VERT_NICE_SLIDER);
      move_right_hand_r_slider->textsize(10);
      move_right_hand_r_slider->callback(valuator_cb, &move_right_hand_r_value);
      move_right_hand_r_slider->labelsize(10); 
      //move_right_hand_r_slider->labelfont(FL_HELVETICA_BOLD);  
      move_right_hand_r_slider->when(FL_WHEN_ENTER_KEY | FL_WHEN_CHANGED | FL_WHEN_RELEASE); 
      move_right_hand_r_slider->align(FL_ALIGN_TOP);
      move_right_hand_r_slider->minimum(100);
      move_right_hand_r_slider->maximum(-100);
      move_right_hand_r_slider->step(1);
      move_right_hand_r_slider->value(0); 
      move_right_hand_r_slider->box(FL_PLASTIC_DOWN_BOX);


      /* move head sliders */
      
      Fl_Box *head_slider_label;
      head_slider_label = new Fl_Box(move_head_x, move_head_y+vslider_size_y + button_size_y, 0, 0,"        Head                                                 Torso" );
      head_slider_label->align(FL_ALIGN_BOTTOM | FL_ALIGN_LEFT | FL_ALIGN_INSIDE);
      head_slider_label->labelsize(12);
      head_slider_label->labelfont(FL_HELVETICA_BOLD);
      //head_slider_label->box(FL_PLASTIC_DOWN_BOX);
      head_slider_label->box(FL_DOWN_BOX);

      Fl_Value_Slider *move_head_x_slider;
      move_head_x_slider = new Fl_Value_Slider(move_head_x, move_head_y, vslider_size_x, vslider_size_y,"X");
      move_head_x_slider->type(FL_VERT_NICE_SLIDER);
      move_head_x_slider->textsize(10);
      move_head_x_slider->callback(valuator_cb, &move_head_x_value);
      move_head_x_slider->labelsize(10); 
      //move_head_x_slider->labelfont(FL_HELVETICA_BOLD);  
      move_head_x_slider->when(FL_WHEN_ENTER_KEY | FL_WHEN_CHANGED | FL_WHEN_RELEASE); 
      move_head_x_slider->align(FL_ALIGN_TOP);
      move_head_x_slider->minimum(100);
      move_head_x_slider->maximum(-100);
      move_head_x_slider->step(1);
      move_head_x_slider->value(0); 
      move_head_x_slider->box(FL_PLASTIC_DOWN_BOX);

      Fl_Value_Slider *move_head_y_slider;
      move_head_y_slider = new Fl_Value_Slider(move_head_x+(vslider_size_x+button_spacing_x), move_head_y, vslider_size_x, vslider_size_y,"Y");
      move_head_y_slider->type(FL_VERT_NICE_SLIDER);
      move_head_y_slider->textsize(10);
      move_head_y_slider->callback(valuator_cb, &move_head_y_value);
      move_head_y_slider->labelsize(10); 
      //move_head_y_slider->labelfont(FL_HELVETICA_BOLD);  
      move_head_y_slider->when(FL_WHEN_ENTER_KEY | FL_WHEN_CHANGED | FL_WHEN_RELEASE); 
      move_head_y_slider->align(FL_ALIGN_TOP);
      move_head_y_slider->minimum(100);
      move_head_y_slider->maximum(-100);
      move_head_y_slider->step(1);
      move_head_y_slider->value(0); 
      move_head_y_slider->box(FL_PLASTIC_DOWN_BOX);

      Fl_Value_Slider *move_head_z_slider;
      move_head_z_slider = new Fl_Value_Slider(move_head_x+2*(vslider_size_x+button_spacing_x), move_head_y, vslider_size_x, vslider_size_y,"Z");
      move_head_z_slider->type(FL_VERT_NICE_SLIDER);
      move_head_z_slider->textsize(10);
      move_head_z_slider->callback(valuator_cb, &move_head_z_value);
      move_head_z_slider->labelsize(10); 
      //move_head_z_slider->labelfont(FL_HELVETICA_BOLD);  
      move_head_z_slider->when(FL_WHEN_ENTER_KEY | FL_WHEN_CHANGED | FL_WHEN_RELEASE); 
      move_head_z_slider->align(FL_ALIGN_TOP);
      move_head_z_slider->minimum(100);
      move_head_z_slider->maximum(-100);
      move_head_z_slider->step(1);
      move_head_z_slider->value(0); 
      move_head_z_slider->box(FL_PLASTIC_DOWN_BOX);

          
      /* move torso sliders */
 
      Fl_Value_Slider *move_torso_x_slider;
      move_torso_x_slider = new Fl_Value_Slider(move_torso_x, move_torso_y, vslider_size_x, vslider_size_y,"X");
      move_torso_x_slider->type(FL_VERT_NICE_SLIDER);
      move_torso_x_slider->textsize(10);
      move_torso_x_slider->callback(valuator_cb, &move_torso_x_value);
      move_torso_x_slider->labelsize(10); 
      //move_torso_x_slider->labelfont(FL_HELVETICA_BOLD);  
      move_torso_x_slider->when(FL_WHEN_ENTER_KEY | FL_WHEN_CHANGED | FL_WHEN_RELEASE); 
      move_torso_x_slider->align(FL_ALIGN_TOP);
      move_torso_x_slider->minimum(100);
      move_torso_x_slider->maximum(-100);
      move_torso_x_slider->step(1);
      move_torso_x_slider->value(0); 
      move_torso_x_slider->box(FL_PLASTIC_DOWN_BOX);

      Fl_Value_Slider *move_torso_y_slider;
      move_torso_y_slider = new Fl_Value_Slider(move_torso_x+(vslider_size_x+button_spacing_x), move_torso_y, vslider_size_x, vslider_size_y,"Y");
      move_torso_y_slider->type(FL_VERT_NICE_SLIDER);
      move_torso_y_slider->textsize(10);
      move_torso_y_slider->callback(valuator_cb, &move_torso_y_value);
      move_torso_y_slider->labelsize(10); 
      //move_torso_y_slider->labelfont(FL_HELVETICA_BOLD);  
      move_torso_y_slider->when(FL_WHEN_ENTER_KEY | FL_WHEN_CHANGED | FL_WHEN_RELEASE); 
      move_torso_y_slider->align(FL_ALIGN_TOP);
      move_torso_y_slider->minimum(100);
      move_torso_y_slider->maximum(-100);
      move_torso_y_slider->step(1);
      move_torso_y_slider->value(0); 
      move_torso_y_slider->box(FL_PLASTIC_DOWN_BOX);

      Fl_Value_Slider *move_torso_z_slider;
      move_torso_z_slider = new Fl_Value_Slider(move_torso_x+2*(vslider_size_x+button_spacing_x), move_torso_y, vslider_size_x, vslider_size_y,"Z");
      move_torso_z_slider->type(FL_VERT_NICE_SLIDER);
      move_torso_z_slider->textsize(10);
      move_torso_z_slider->callback(valuator_cb, &move_torso_z_value);
      move_torso_z_slider->labelsize(10); 
      //move_torso_z_slider->labelfont(FL_HELVETICA_BOLD);  
      move_torso_z_slider->when(FL_WHEN_ENTER_KEY | FL_WHEN_CHANGED | FL_WHEN_RELEASE); 
      move_torso_z_slider->align(FL_ALIGN_TOP);
      move_torso_z_slider->minimum(100);
      move_torso_z_slider->maximum(-100);
      move_torso_z_slider->step(1);
      move_torso_z_slider->value(0); 
      move_torso_z_slider->box(FL_PLASTIC_DOWN_BOX);

      /* move sequence */

      Fl_Input *move_sequence;
      move_sequence = new Fl_Input(move_sequence_x, move_sequence_y, input_size_x, input_size_y,"Execute movement sequence number:");
      move_sequence->textsize(12);
      move_sequence->labelfont(FL_HELVETICA_BOLD);  
      move_sequence->callback(get_input_cb,move_sequence_string);  
      move_sequence->labelsize(12);
      move_sequence->align(FL_ALIGN_LEFT);
      move_sequence->value("");
      get_input_cb(move_sequence,move_sequence_string);
      move_sequence->when(FL_WHEN_ENTER_KEY | FL_WHEN_NOT_CHANGED);  // callback is called: when the enter key is hit
 
      /* say */

      Fl_Input *say;
	   say = new Fl_Input(say_x, say_y, text_input_width, input_size_y,"Robot say:");
	   say->textsize(12);
	   say->callback(get_input_cb,say_value);  
	   say->labelsize(12);
      say->labelfont(FL_HELVETICA_BOLD); 
	   say->align(FL_ALIGN_LEFT);
	   say->value("");
      get_input_cb(say,say_value);
      say->when(FL_WHEN_ENTER_KEY | FL_WHEN_NOT_CHANGED);  // callback is called: when the enter key is hit 
 
      // Processing Mode Box
    
      Fl_Box *tone_of_voice_box;
      tone_of_voice_box = new Fl_Box(tone_of_voice_box_x, tone_of_voice_box_y, tone_of_voice_box_width, tone_of_voice_box_height,"Voice" );
      //tone_of_voice_box->box(FL_THIN_DOWN_FRAME);
      tone_of_voice_box->align(FL_ALIGN_TOP | FL_ALIGN_CENTER | FL_ALIGN_INSIDE);
      tone_of_voice_box->labelsize(12);
      tone_of_voice_box->labelfont(FL_HELVETICA_BOLD);
      //tone_of_voice_box->box(FL_PLASTIC_DOWN_BOX);
      //tone_of_voice_box->labelcolor(FL_BLUE);

      // happy tone of voice  

      Fl_Round_Button *happy_mode = new Fl_Round_Button (happy_voice_x, happy_voice_y, radio_button_size_x, radio_button_size_y, "Happy");
      happy_mode->callback(happy_voice_cb, &tone_of_voice_value);
      happy_mode->labelsize(12);
      happy_mode->type(FL_RADIO_BUTTON);
      happy_mode->value(1);
        
      // sad tone of voice  

      Fl_Round_Button *sad_mode = new Fl_Round_Button (sad_voice_x, sad_voice_y, radio_button_size_x, radio_button_size_y, "Sad");
      sad_mode->callback(sad_voice_cb, &tone_of_voice_value);
      sad_mode->labelsize(12);
      sad_mode->type(FL_RADIO_BUTTON);
      sad_mode->value(0);
        
      // angry tone of voice  

      Fl_Round_Button *angry_mode = new Fl_Round_Button (angry_voice_x, angry_voice_y, radio_button_size_x, radio_button_size_y, "Angry");
      angry_mode->callback(angry_voice_cb, &tone_of_voice_value);
      angry_mode->labelsize(12);
      angry_mode->type(FL_RADIO_BUTTON);
      angry_mode->value(0);
       
      // fearful tone of voice  

      Fl_Round_Button *fearful_mode = new Fl_Round_Button (fearful_voice_x, fearful_voice_y, radio_button_size_x, radio_button_size_y, "Fearful");
      fearful_mode->callback(fearful_voice_cb, &tone_of_voice_value);
      fearful_mode->labelsize(12);
      fearful_mode->type(FL_RADIO_BUTTON);
      fearful_mode->value(0);
 

      /**************************************/
      /*                                    */
      /*     simulation parameters part     */
      /*                                    */
      /**************************************/

      /* box to frame controls */
   
      Fl_Box *simulation_panel;
      simulation_panel = new Fl_Box(simulation_panel_x-BORDER, simulation_panel_y, simulation_panel_width+1*BORDER, simulation_panel_height,
                         "Simulation Parameters                                  Verbose         Cycle             Cycle Period                Event Rate" );
      simulation_panel->align(FL_ALIGN_TOP | FL_ALIGN_LEFT | FL_ALIGN_INSIDE);
      simulation_panel->labelsize(12);
      simulation_panel->labelfont(FL_HELVETICA_BOLD);
      //simulation_panel->box(FL_PLASTIC_DOWN_BOX);
      simulation_panel->box(FL_DOWN_BOX);
      
      /* labels for the simulation parameters */

      box6 = new Fl_Box(simulation_panel_x,sensory_interpretation_y1, 0,check_button_size_y,"sensoryInterpretation Component" );
      box6->align(FL_ALIGN_RIGHT | FL_ALIGN_CENTER );
      box6->labelsize(12);
       
      box7 = new Fl_Box(simulation_panel_x,child_behaviour_classification_y1, 0,check_button_size_y,"childBehaviourClassification Component" );
      box7->align(FL_ALIGN_RIGHT | FL_ALIGN_CENTER );
      box7->labelsize(12);

      box8 = new Fl_Box(simulation_panel_x,cognitive_control_y1, 0,check_button_size_y,"cognitiveControl Component" );
      box8->align(FL_ALIGN_RIGHT | FL_ALIGN_CENTER );
      box8->labelsize(12);

      box9 = new Fl_Box(simulation_panel_x,system_architecture_GUI_y1, 0,check_button_size_y,"systemArchitectureGUI Component" );
      box9->align(FL_ALIGN_RIGHT | FL_ALIGN_CENTER );
      box9->labelsize(12);

      /* verbose checkboxes */

      Fl_Check_Button *sensory_interpretation_verbose;
      sensory_interpretation_verbose = new Fl_Check_Button(sensory_interpretation_x1, sensory_interpretation_y1, check_button_size_x, check_button_size_y,"");
      //sensory_interpretation_verbose->callback(sensory_interpretation_verbose_button_cb, &sensory_intepretation_verbose_value); 
      sensory_interpretation_verbose->callback(button_cb, &sensory_interpretation_verbose_value); 
      sensory_interpretation_verbose->labelsize(12);                                                 
      //sensory_interpretation_verbose->when(FL_WHEN_ENTER_KEY | FL_WHEN_CHANGED | FL_WHEN_RELEASE); 
      sensory_interpretation_verbose->when(FL_WHEN_ENTER_KEY | FL_WHEN_CHANGED); 
      sensory_interpretation_verbose->align(FL_ALIGN_LEFT | FL_ALIGN_CENTER); 
      if (*verbose)
         sensory_interpretation_verbose->value(1);
      else
         sensory_interpretation_verbose->value(0);


      Fl_Check_Button *child_behaviour_classification_verbose;
      child_behaviour_classification_verbose = new Fl_Check_Button(child_behaviour_classification_x1, child_behaviour_classification_y1, check_button_size_x, check_button_size_y,"");
      child_behaviour_classification_verbose->callback(button_cb, &child_behaviour_classification_verbose_value); 
      child_behaviour_classification_verbose->labelsize(12);                                                 
      //child_behaviour_classification_verbose->when(FL_WHEN_ENTER_KEY | FL_WHEN_CHANGED | FL_WHEN_RELEASE); 
      child_behaviour_classification_verbose->when(FL_WHEN_ENTER_KEY | FL_WHEN_CHANGED); 
      child_behaviour_classification_verbose->align(FL_ALIGN_LEFT | FL_ALIGN_CENTER); 
      if (*verbose)
         child_behaviour_classification_verbose->value(1);
      else
         child_behaviour_classification_verbose->value(0);

      Fl_Check_Button *cognitive_control_verbose;
      cognitive_control_verbose = new Fl_Check_Button(cognitive_control_x1, cognitive_control_y1, check_button_size_x, check_button_size_y,"");
      cognitive_control_verbose->callback(button_cb, &cognitive_control_verbose_value); 
      cognitive_control_verbose->labelsize(12);                                                 
      //cognitive_control_verbose->when(FL_WHEN_ENTER_KEY | FL_WHEN_CHANGED | FL_WHEN_RELEASE); 
      cognitive_control_verbose->when(FL_WHEN_ENTER_KEY | FL_WHEN_CHANGED); 
      cognitive_control_verbose->align(FL_ALIGN_LEFT | FL_ALIGN_CENTER); 
      if (*verbose)
         cognitive_control_verbose->value(1);
      else
         cognitive_control_verbose->value(0);

      Fl_Check_Button *system_architecture_GUI_cycle;
      system_architecture_GUI_cycle = new Fl_Check_Button(system_architecture_GUI_x2, system_architecture_GUI_y1, check_button_size_x, check_button_size_y,"");
      system_architecture_GUI_cycle->callback(button_cb, &system_architecture_GUI_cycle_value); 
      system_architecture_GUI_cycle->labelsize(12);                                                 
      system_architecture_GUI_cycle->when(FL_WHEN_ENTER_KEY | FL_WHEN_CHANGED | FL_WHEN_RELEASE); 
      //system_architecture_GUI_cycle->when(FL_WHEN_ENTER_KEY | FL_WHEN_CHANGED); 
      system_architecture_GUI_cycle->align(FL_ALIGN_LEFT | FL_ALIGN_CENTER); 
      if (*cycle)
         system_architecture_GUI_cycle->value(1);
      else
         system_architecture_GUI_cycle->value(0);

      /* cycle checkboxes */

      Fl_Check_Button *sensory_interpretation_cycle;
      sensory_interpretation_cycle = new Fl_Check_Button(sensory_interpretation_x2, sensory_interpretation_y1, check_button_size_x, check_button_size_y,"");
      sensory_interpretation_cycle->callback(button_cb, &sensory_interpretation_cycle_value); 
      sensory_interpretation_cycle->labelsize(12);                                                 
      sensory_interpretation_cycle->when(FL_WHEN_ENTER_KEY | FL_WHEN_CHANGED | FL_WHEN_RELEASE); 
      //sensory_intrepretation_cycle->when(FL_WHEN_ENTER_KEY | FL_WHEN_CHANGED); 
      sensory_interpretation_cycle->align(FL_ALIGN_LEFT | FL_ALIGN_CENTER); 
      if (*cycle)
         sensory_interpretation_cycle->value(1);
      else
         sensory_interpretation_cycle->value(0);

      Fl_Check_Button *child_behaviour_classification_cycle;
      child_behaviour_classification_cycle = new Fl_Check_Button(child_behaviour_classification_x2, child_behaviour_classification_y1, check_button_size_x, check_button_size_y,"");
      child_behaviour_classification_cycle->callback(button_cb, &child_behaviour_classification_cycle_value); 
      child_behaviour_classification_cycle->labelsize(12);                                                 
      child_behaviour_classification_cycle->when(FL_WHEN_ENTER_KEY | FL_WHEN_CHANGED | FL_WHEN_RELEASE); 
      //child_behaviour_classification_cycle->when(FL_WHEN_ENTER_KEY | FL_WHEN_CHANGED); 
      child_behaviour_classification_cycle->align(FL_ALIGN_LEFT | FL_ALIGN_CENTER); 
      if (*cycle)
         child_behaviour_classification_cycle->value(1);
      else
         child_behaviour_classification_cycle->value(0);
      
      Fl_Check_Button *cognitive_control_cycle;
      cognitive_control_cycle = new Fl_Check_Button(cognitive_control_x2, cognitive_control_y1, check_button_size_x, check_button_size_y,"");
      cognitive_control_cycle->callback(button_cb, &cognitive_control_cycle_value); 
      cognitive_control_cycle->labelsize(12);                                                 
      cognitive_control_cycle->when(FL_WHEN_ENTER_KEY | FL_WHEN_CHANGED | FL_WHEN_RELEASE); 
      //cognitive_control_cycle->when(FL_WHEN_ENTER_KEY | FL_WHEN_CHANGED); 
      cognitive_control_cycle->align(FL_ALIGN_LEFT | FL_ALIGN_CENTER); 
      if (*cycle)
         cognitive_control_cycle->value(1);
      else
         cognitive_control_cycle->value(0);

      Fl_Check_Button *system_architecture_GUI_verbose;
      system_architecture_GUI_verbose = new Fl_Check_Button(system_architecture_GUI_x1, system_architecture_GUI_y1, check_button_size_x, check_button_size_y,"");
      system_architecture_GUI_verbose->callback(button_cb, &system_architecture_GUI_verbose_value); 
      system_architecture_GUI_verbose->labelsize(12);                                                 
      //system_architecture_GUI_verbose->when(FL_WHEN_ENTER_KEY | FL_WHEN_CHANGED | FL_WHEN_RELEASE); 
      system_architecture_GUI_verbose->when(FL_WHEN_ENTER_KEY | FL_WHEN_CHANGED); 
      system_architecture_GUI_verbose->align(FL_ALIGN_LEFT | FL_ALIGN_CENTER); 
      if (*verbose)
         system_architecture_GUI_verbose->value(1);
      else
         system_architecture_GUI_verbose->value(0);

      /* cycle period sliders */
 
      Fl_Value_Slider *sensory_interpretation_cycle_period_slider;
      sensory_interpretation_cycle_period_slider = new Fl_Value_Slider(sensory_interpretation_x3, sensory_interpretation_y1, hslider_size_x, hslider_size_y,"");
      sensory_interpretation_cycle_period_slider->type(FL_HOR_NICE_SLIDER);
      sensory_interpretation_cycle_period_slider->textsize(11);
      sensory_interpretation_cycle_period_slider->callback(valuator_cb, &sensory_interpretation_cycle_period);
      sensory_interpretation_cycle_period_slider->labelsize(12); 
      sensory_interpretation_cycle_period_slider->labelfont(FL_HELVETICA_BOLD);  
      sensory_interpretation_cycle_period_slider->when(FL_WHEN_ENTER_KEY | FL_WHEN_CHANGED | FL_WHEN_RELEASE); 
      sensory_interpretation_cycle_period_slider->align(FL_ALIGN_TOP);
      sensory_interpretation_cycle_period_slider->minimum(0.5);
      sensory_interpretation_cycle_period_slider->maximum(10);
      sensory_interpretation_cycle_period_slider->step(0.5);
      sensory_interpretation_cycle_period_slider->value((double)*cyclePeriod); 
      sensory_interpretation_cycle_period_slider->box(FL_PLASTIC_DOWN_BOX);
 
      Fl_Value_Slider *child_behaviour_classification_cycle_period_slider;
      child_behaviour_classification_cycle_period_slider = new Fl_Value_Slider(child_behaviour_classification_x3, child_behaviour_classification_y1, hslider_size_x, hslider_size_y,"");
      child_behaviour_classification_cycle_period_slider->type(FL_HOR_NICE_SLIDER);
      child_behaviour_classification_cycle_period_slider->textsize(11);
      child_behaviour_classification_cycle_period_slider->callback(valuator_cb, &child_behaviour_classification_cycle_period); 
      child_behaviour_classification_cycle_period_slider->labelsize(12);   
      child_behaviour_classification_cycle_period_slider->labelfont(FL_HELVETICA_BOLD); 
      child_behaviour_classification_cycle_period_slider->when(FL_WHEN_ENTER_KEY | FL_WHEN_CHANGED | FL_WHEN_RELEASE); 
      child_behaviour_classification_cycle_period_slider->align(FL_ALIGN_TOP);
      child_behaviour_classification_cycle_period_slider->minimum(0.5);
      child_behaviour_classification_cycle_period_slider->maximum(10);
      child_behaviour_classification_cycle_period_slider->step(0.5);
      child_behaviour_classification_cycle_period_slider->value((double)*cyclePeriod); 
      child_behaviour_classification_cycle_period_slider->box(FL_PLASTIC_DOWN_BOX);

      Fl_Value_Slider *cognitive_control_cycle_period_slider;
      cognitive_control_cycle_period_slider = new Fl_Value_Slider(cognitive_control_x3, cognitive_control_y1, hslider_size_x, hslider_size_y,"");
      cognitive_control_cycle_period_slider->type(FL_HOR_NICE_SLIDER);
      cognitive_control_cycle_period_slider->textsize(11);
      cognitive_control_cycle_period_slider->callback(valuator_cb, &cognitive_control_cycle_period); 
      cognitive_control_cycle_period_slider->labelsize(12);      
      cognitive_control_cycle_period_slider->labelfont(FL_HELVETICA_BOLD);     
      cognitive_control_cycle_period_slider->when(FL_WHEN_ENTER_KEY | FL_WHEN_CHANGED | FL_WHEN_RELEASE); 
      cognitive_control_cycle_period_slider->align(FL_ALIGN_TOP);
      cognitive_control_cycle_period_slider->minimum(0.5);
      cognitive_control_cycle_period_slider->maximum(10);
      cognitive_control_cycle_period_slider->step(0.5);
      cognitive_control_cycle_period_slider->value((double)*cyclePeriod); 
      cognitive_control_cycle_period_slider->box(FL_PLASTIC_DOWN_BOX);

      Fl_Value_Slider *system_architecture_GUI_cycle_period_slider;
      system_architecture_GUI_cycle_period_slider = new Fl_Value_Slider(system_architecture_GUI_x3, system_architecture_GUI_y1, hslider_size_x, hslider_size_y,"");
      system_architecture_GUI_cycle_period_slider->type(FL_HOR_NICE_SLIDER);
      system_architecture_GUI_cycle_period_slider->textsize(11);
      system_architecture_GUI_cycle_period_slider->callback(valuator_cb, &system_architecture_GUI_cycle_period); 
      system_architecture_GUI_cycle_period_slider->labelsize(12);      
      system_architecture_GUI_cycle_period_slider->labelfont(FL_HELVETICA_BOLD);     
      system_architecture_GUI_cycle_period_slider->when(FL_WHEN_ENTER_KEY | FL_WHEN_CHANGED | FL_WHEN_RELEASE); 
      system_architecture_GUI_cycle_period_slider->align(FL_ALIGN_TOP);
      system_architecture_GUI_cycle_period_slider->minimum(0.5);
      system_architecture_GUI_cycle_period_slider->maximum(10);
      system_architecture_GUI_cycle_period_slider->step(0.5);
      system_architecture_GUI_cycle_period_slider->value((double)*cyclePeriod); 
      system_architecture_GUI_cycle_period_slider->box(FL_PLASTIC_DOWN_BOX);

      /* event rate sliders */
 
      Fl_Value_Slider *sensory_interpretation_event_rate_slider;
      sensory_interpretation_event_rate_slider = new Fl_Value_Slider(sensory_interpretation_x4, sensory_interpretation_y1, hslider_size_x, hslider_size_y,"");
      sensory_interpretation_event_rate_slider->type(FL_HOR_NICE_SLIDER);
      sensory_interpretation_event_rate_slider->textsize(11);
      sensory_interpretation_event_rate_slider->callback(valuator_cb, &sensory_interpretation_event_rate);
      sensory_interpretation_event_rate_slider->labelsize(12); 
      sensory_interpretation_event_rate_slider->labelfont(FL_HELVETICA_BOLD);  
      sensory_interpretation_event_rate_slider->when(FL_WHEN_ENTER_KEY | FL_WHEN_CHANGED | FL_WHEN_RELEASE); 
      sensory_interpretation_event_rate_slider->align(FL_ALIGN_TOP);
      sensory_interpretation_event_rate_slider->minimum(1);
      sensory_interpretation_event_rate_slider->maximum(100);
      sensory_interpretation_event_rate_slider->step(1);
      sensory_interpretation_event_rate_slider->value((double)*eventRate); 
      sensory_interpretation_event_rate_slider->box(FL_PLASTIC_DOWN_BOX);
 
      Fl_Value_Slider *child_behaviour_classification_event_rate_slider;
      child_behaviour_classification_event_rate_slider = new Fl_Value_Slider(child_behaviour_classification_x4, child_behaviour_classification_y1, hslider_size_x, hslider_size_y,"");
      child_behaviour_classification_event_rate_slider->type(FL_HOR_NICE_SLIDER);
      child_behaviour_classification_event_rate_slider->textsize(11);
      child_behaviour_classification_event_rate_slider->callback(valuator_cb, &child_behaviour_classification_event_rate); 
      child_behaviour_classification_event_rate_slider->labelsize(12);   
      child_behaviour_classification_event_rate_slider->labelfont(FL_HELVETICA_BOLD); 
      child_behaviour_classification_event_rate_slider->when(FL_WHEN_ENTER_KEY | FL_WHEN_CHANGED | FL_WHEN_RELEASE); 
      child_behaviour_classification_event_rate_slider->align(FL_ALIGN_TOP);
      child_behaviour_classification_event_rate_slider->minimum(1);
      child_behaviour_classification_event_rate_slider->maximum(100);
      child_behaviour_classification_event_rate_slider->step(1);
      child_behaviour_classification_event_rate_slider->value((double)*eventRate); 
      child_behaviour_classification_event_rate_slider->box(FL_PLASTIC_DOWN_BOX);

      Fl_Value_Slider *cognitive_control_event_rate_slider;
      cognitive_control_event_rate_slider = new Fl_Value_Slider(cognitive_control_x4, cognitive_control_y1, hslider_size_x, hslider_size_y,"");
      cognitive_control_event_rate_slider->type(FL_HOR_NICE_SLIDER);
      cognitive_control_event_rate_slider->textsize(11);
      cognitive_control_event_rate_slider->callback(valuator_cb, &cognitive_control_event_rate); 
      cognitive_control_event_rate_slider->labelsize(12);      
      cognitive_control_event_rate_slider->labelfont(FL_HELVETICA_BOLD);     
      cognitive_control_event_rate_slider->when(FL_WHEN_ENTER_KEY | FL_WHEN_CHANGED | FL_WHEN_RELEASE); 
      cognitive_control_event_rate_slider->align(FL_ALIGN_TOP);
      cognitive_control_event_rate_slider->minimum(1);
      cognitive_control_event_rate_slider->maximum(100);
      cognitive_control_event_rate_slider->step(1);
      cognitive_control_event_rate_slider->value((double)*eventRate); 
      cognitive_control_event_rate_slider->box(FL_PLASTIC_DOWN_BOX);

      Fl_Value_Slider *system_architecture_GUI_event_rate_slider;
      system_architecture_GUI_event_rate_slider = new Fl_Value_Slider(system_architecture_GUI_x4, system_architecture_GUI_y1, hslider_size_x, hslider_size_y,"");
      system_architecture_GUI_event_rate_slider->type(FL_HOR_NICE_SLIDER);
      system_architecture_GUI_event_rate_slider->textsize(11);
      system_architecture_GUI_event_rate_slider->callback(valuator_cb, &system_architecture_GUI_event_rate); 
      system_architecture_GUI_event_rate_slider->labelsize(12);      
      system_architecture_GUI_event_rate_slider->labelfont(FL_HELVETICA_BOLD);     
      system_architecture_GUI_event_rate_slider->when(FL_WHEN_ENTER_KEY | FL_WHEN_CHANGED | FL_WHEN_RELEASE); 
      system_architecture_GUI_event_rate_slider->align(FL_ALIGN_TOP);
      system_architecture_GUI_event_rate_slider->minimum(1);
      system_architecture_GUI_event_rate_slider->maximum(100);
      system_architecture_GUI_event_rate_slider->step(1);
      system_architecture_GUI_event_rate_slider->value((double)*eventRate); 
      system_architecture_GUI_event_rate_slider->box(FL_PLASTIC_DOWN_BOX);

   start_systemArchitecture_GUI->end();  // end of the group 

   enable_robot_value                                = 0;
   enable_robot_value_old                            = -1; // this forces the old value to be updated and the current value to be issued

   grip_value                                        = 0;
   grip_value_old                                    = -1;  

   move_left_hand_x_value                            = 0;
   move_left_hand_y_value                            = 0;
   move_left_hand_z_value                            = 0;
   move_left_hand_r_value                            = 0;
   move_left_hand_x_value_old                        = -1;
   move_left_hand_y_value_old                        = -1;
   move_left_hand_z_value_old                        = -1;
   move_left_hand_r_value_old                        = -1;

   move_right_hand_x_value                           = 0;
   move_right_hand_y_value                           = 0;
   move_right_hand_z_value                           = 0;
   move_right_hand_r_value                           = 0;
   move_right_hand_x_value_old                       = -1;
   move_right_hand_y_value_old                       = -1;
   move_right_hand_z_value_old                       = -1;
   move_right_hand_r_value_old                       = -1;

   move_head_x_value                                 = 0;
   move_head_y_value                                 = 0;
   move_head_z_value                                 = 0;
   move_head_x_value_old                             = -1;
   move_head_y_value_old                             = -1;
   move_head_z_value_old                             = -1;

   move_torso_x_value                                 = 0;
   move_torso_y_value                                 = 0;
   move_torso_z_value                                 = 0;
   move_torso_x_value_old                             = -1;
   move_torso_y_value_old                             = -1;
   move_torso_z_value_old                             = -1;

   move_sequence_value                                = 0;
   move_sequence_value_old                            = -1;

   tone_of_voice_value                                = 1;
   tone_of_voice_value_old                            = -1;

   sensory_interpretation_verbose_value              = *verbose;
   sensory_interpretation_verbose_value_old          = -1; 
   sensory_interpretation_cycle_value                = *cycle;
   sensory_interpretation_cycle_value_old            = -1;
   sensory_interpretation_event_rate                 = *eventRate;
   sensory_interpretation_cycle_period               = -1.0;
   sensory_interpretation_event_rate_old             = *cyclePeriod;
   sensory_interpretation_cycle_period_old           = -1.0;

   child_behaviour_classification_verbose_value     = *verbose;
   child_behaviour_classification_verbose_value_old = -1; 
   child_behaviour_classification_cycle_value       = *cycle;
   child_behaviour_classification_cycle_value_old   = -1;
   child_behaviour_classification_event_rate        = *eventRate;
   child_behaviour_classification_event_rate_old    = -1.0;
   child_behaviour_classification_cycle_period      = *cyclePeriod;
   child_behaviour_classification_cycle_period_old  = -1.0;

   cognitive_control_verbose_value                  = *verbose;
   cognitive_control_verbose_value_old              = -1;
   cognitive_control_cycle_value                    = *cycle;
   cognitive_control_cycle_value_old                = -1;
   cognitive_control_event_rate                     = *eventRate;
   cognitive_control_event_rate_old                 = -1.0;
   cognitive_control_cycle_period                   = *cyclePeriod;
   cognitive_control_cycle_period_old               = -1.0;

   system_architecture_GUI_verbose_value            = *verbose;
   system_architecture_GUI_verbose_value_old        = -1;
   system_architecture_GUI_cycle_value              = *cycle;
   system_architecture_GUI_cycle_value_old          = -1;
   system_architecture_GUI_event_rate               = *eventRate;
   system_architecture_GUI_event_rate_old           = -1.0;
   system_architecture_GUI_cycle_period             = *cyclePeriod;
   system_architecture_GUI_cycle_period_old         = -1.0;

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

   /* create buffers for childBehaviourClassification data */
   /* ---------------------------------------------------- */

   image1 = new DVimage(display_width, 1, GREYSCALE_IMAGE, NULL, NULL, DVFLOAT);
   if (image1 == NULL) {
      printf("systemArchitectureGUI: unable to allocate image1\n");
      return;
   }
      
   image2 = new DVimage(display_width, 1, GREYSCALE_IMAGE, NULL, NULL, DVFLOAT);
   if (image2 == NULL) {
      printf("systemArchitectureGUI: unable to allocate image1\n");
      return;
   }

   image3 = new DVimage(display_width, 1, GREYSCALE_IMAGE, NULL, NULL, DVFLOAT);
   if (image3 == NULL) {
      printf("systemArchitectureGUI: unable to allocate image1\n");
      return;
   }

   image4 = new DVimage(display_width, 1, GREYSCALE_IMAGE, NULL, NULL, DVFLOAT);
   if (image4 == NULL) {
      printf("systemArchitectureGUI: unable to allocate image1\n");
      return;
   }

   image5 = new DVimage(display_width, 1, GREYSCALE_IMAGE, NULL, NULL, DVFLOAT);
   if (image5 == NULL) {
      printf("systemArchitectureGUI: unable to allocate image1\n");
      return;
   }

   buffer1 = (float *) calloc(display_width, sizeof(float));
   if (buffer1 == NULL) {
      printf("systemArchitectureGUI: unable to allocate buffer\n");
      return;
   }

   buffer2 = (float *) calloc(display_width, sizeof(float));
   if (buffer2 == NULL) {
      printf("systemArchitectureGUI: unable to allocate buffer\n");
      return;
   }

   buffer3 = (float *) calloc(display_width, sizeof(float));
   if (buffer3 == NULL) {
      printf("systemArchitectureGUI: unable to allocate buffer\n");
      return;
   }
  
   buffer4 = (float *) calloc(display_width, sizeof(float));
   if (buffer4 == NULL) {
      printf("systemArchitectureGUI: unable to allocate buffer\n");
      return;
   }

   buffer5 = (float *) calloc(display_width, sizeof(float));
   if (buffer5 == NULL) {
      printf("systemArchitectureGUI: unable to allocate buffer\n");
      return;
   }

   // set up the display parameters to plot a row of the (one-row) image

   max_amplitude = 1;

   display1->clear(); 
   display1->set_plot_row(0);                  // plot row 0
   display1->set_plot_colour(206, 057, 101);   // DREAM colour 1
   display1->set_plot_maximum(max_amplitude);  // and scale so that this is the maximum value plotted    
   display1->draw_plot(2);                     // set the flag so that the row is plotted upon redraw

   display2->clear(); 
   display2->set_plot_row(0);                  // plot row 0
   display2->set_plot_colour(228, 135, 58);    // DREAM colour 2
   display2->set_plot_maximum(max_amplitude);  // and scale so that this is the maximum value plotted    
   display2->draw_plot(2);                  // set the flag so that the row is plotted upon redraw
 
   display3->clear(); 
   display3->set_plot_row(0);                  // plot row 0
   display3->set_plot_colour(240, 195, 44);    // DREAM colour 3
   display3->set_plot_maximum(max_amplitude);  // and scale so that this is the maximum value plotted    
   display3->draw_plot(2);                  // set the flag so that the row is plotted upon redraw

   display4->clear(); 
   display4->set_plot_row(0);                  // plot row 0
   display4->set_plot_colour(144, 186, 64);    // DREAM colour 4
   display4->set_plot_maximum(max_amplitude);  // and scale so that this is the maximum value plotted    
   display4->draw_plot(2);                  // set the flag so that the row is plotted upon redraw
 
   display5->clear(); 
   display5->set_plot_row(0);                  // plot row 0
   display5->set_plot_colour(119, 189, 217);   // DREAM colour 5
   display5->set_plot_maximum(max_amplitude);  // and scale so that this is the maximum value plotted    
   display5->draw_plot(2);                  // set the flag so that the row is plotted upon redraw
   
   index1 = 0; // this is the column in which the currently read motivation and performance values and confidences are stored
   index2 = 0;
   index3 = 0;
   index4 = 0;
   index5 = 0;

 
  
   /*************************************************************************************/
   /*   This is where the definition of the GUI ends and the normal run() loop begins   */
   /*************************************************************************************/

   while (isStopping() != true) { // the thread continues to run until isStopping() returns true

      DVwindow->redraw();
      Fl::check();         // refresh the GUI   .... we do it this way rather than calling Fl::run() as we nomally would 
                           // because we do not want to cede control to the event loop
 
      /**************************************/
      /*                                    */
      /*     simulation parameters part     */
      /*                                    */
      /**************************************/

      /* sensoryInterpretation component verbose command */
   
      if (sensory_interpretation_verbose_value_old != sensory_interpretation_verbose_value) {
         sensory_interpretation_verbose_value_old = sensory_interpretation_verbose_value;
         command.clear();
         if (sensory_interpretation_verbose_value == 0) {
            command.addString("verbose");
            command.addString("off");
            if (debug) printf("SystemArchitectureGUIThread: sensoryIntepretation verbose value is off\n");
         }
         else {
            command.addString("verbose");
            command.addString("on");
            if (debug) printf("SystemArchitectureGUIThread: sensoryIntepretation verbose value is on\n");
         }
         sensoryInterpretationPortOut->write(command);
      }

      /* childBehaviourClassification component verbose command */
 
      if (child_behaviour_classification_verbose_value_old != child_behaviour_classification_verbose_value) {
         child_behaviour_classification_verbose_value_old = child_behaviour_classification_verbose_value;
         command.clear();
         if (child_behaviour_classification_verbose_value == 0) {
            command.addString("verbose");
            command.addString("off");
            if (debug) printf("SystemArchitectureGUIThread: childBehaviourClassification verbose value is off\n");
         }
         else {
            command.addString("verbose");
            command.addString("on");
            if (debug) printf("SystemArchitectureGUIThread: childBehaviourClassification verbose value is on\n");
         }
         childBehaviourClassificationPortOut->write(command);
      }
        
      /* cognitiveControl component verbose command */
      
      if (cognitive_control_verbose_value_old != cognitive_control_verbose_value) {
         cognitive_control_verbose_value_old = cognitive_control_verbose_value;
         command.clear();
         if (cognitive_control_verbose_value == 0) {
            command.addString("verbose");
            command.addString("off");
            if (debug) printf("SystemArchitectureGUIThread: cognitiveControl verbose value is off\n");
         }
         else {
            command.addString("verbose");
            command.addString("on");
            if (debug) printf("SystemArchitectureGUIThread: cognitiveControl verbose value is on\n");
         }
         cognitiveControlPortOut->write(command);
      }

      /* systemArchitectureGUI component verbose command */
      
      if (system_architecture_GUI_verbose_value_old != system_architecture_GUI_verbose_value) {
         system_architecture_GUI_verbose_value_old = system_architecture_GUI_verbose_value;

         if (system_architecture_GUI_verbose_value == 0) {
            *verbose = false;
            if (debug) printf("SystemArchitectureGUIThread: systemArchitectureGUI verbose value is off\n");
         }
         else {
            *verbose = true;
            if (debug) printf("SystemArchitectureGUIThread: systemArchitectureGUI verbose value is on\n");
         }
      }
      
      
      /* sensoryInterpretation component cycle command */
   
      if (sensory_interpretation_cycle_value_old != sensory_interpretation_cycle_value) {
         sensory_interpretation_cycle_value_old = sensory_interpretation_cycle_value;
         command.clear();
         if (sensory_interpretation_cycle_value == 0) {
            command.addString("cycle");
            command.addString("off");
            if (debug) printf("SystemArchitectureGUIThread: sensoryIntepretation cycle value is off\n");
         }
         else {
            command.addString("cycle");
            command.addString("on");
            if (debug) printf("SystemArchitectureGUIThread: sensoryIntepretation cycle value is on\n");
         }
         sensoryInterpretationPortOut->write(command);
      }
      
      /* childBehaviourClassification component cycle command */
 
      if (child_behaviour_classification_cycle_value_old != child_behaviour_classification_cycle_value) {
         child_behaviour_classification_cycle_value_old = child_behaviour_classification_cycle_value;
         command.clear();
         if (child_behaviour_classification_cycle_value == 0) {
            command.addString("cycle");
            command.addString("off");
            if (debug) printf("SystemArchitectureGUIThread: childBehaviourClassification cycle value is off\n");
         }
         else {
            command.addString("cycle");
            command.addString("on");
            if (debug) printf("SystemArchitectureGUIThread: childBehaviourClassification cycle value is on\n");
         }
         childBehaviourClassificationPortOut->write(command);
      }

      /* cognitiveControl component cycle command */
      
      if (cognitive_control_cycle_value_old != cognitive_control_cycle_value) {
         cognitive_control_cycle_value_old = cognitive_control_cycle_value;
         command.clear();
         if (cognitive_control_cycle_value == 0) {
            command.addString("cycle");
            command.addString("off");
            if (debug) printf("SystemArchitectureGUIThread: cognitiveControl cycle value is off\n");
         }
         else {
            command.addString("cycle");
            command.addString("on");
            if (debug) printf("SystemArchitectureGUIThread: cognitiveControl cycle value is on\n");
         }
         cognitiveControlPortOut->write(command);
      }

      /* systemArchitectureGUI component cycle command */
      
      if (system_architecture_GUI_cycle_value_old != system_architecture_GUI_cycle_value) {
         system_architecture_GUI_cycle_value_old = system_architecture_GUI_cycle_value;
         if (system_architecture_GUI_cycle_value == 0) {
            *cycle = false;
            if (debug) printf("SystemArchitectureGUIThread: systemArchitectureGUI cycle value is off\n");
         }
         else {
            *cycle = true;
            if (debug) printf("SystemArchitectureGUIThread: systemArchitectureGUI cycle value is on\n");
         }
      }
      
      
      /* sensoryInterpretation component cycle period command */
   
      if (sensory_interpretation_cycle_period_old != sensory_interpretation_cycle_period) {
         sensory_interpretation_cycle_period_old = sensory_interpretation_cycle_period;
         command.clear();
         command.addString("cycle_period");
         command.addDouble(sensory_interpretation_cycle_period);
         sensoryInterpretationPortOut->write(command);
         if (debug) printf("SystemArchitectureGUIThread: sensoryIntepretation cycle period is %2.1f\n",sensory_interpretation_cycle_period);
      }

      /* childBehaviourClassification component cycle period command */
   
      if (child_behaviour_classification_cycle_period_old != child_behaviour_classification_cycle_period) {
         child_behaviour_classification_cycle_period_old = child_behaviour_classification_cycle_period;
         command.clear();
         command.addString("cycle_period");
         command.addDouble(child_behaviour_classification_cycle_period);
         childBehaviourClassificationPortOut->write(command);
         if (debug) printf("SystemArchitectureGUIThread: child behaviour classification cycle period is %2.1f\n",child_behaviour_classification_cycle_period);
      }

      
      /* cognitiveControl component cycle period command */
   
      if (cognitive_control_cycle_period_old != cognitive_control_cycle_period) {
         cognitive_control_cycle_period_old = cognitive_control_cycle_period;
         command.clear();
         command.addString("cycle_period");
         command.addDouble(cognitive_control_cycle_period);
         cognitiveControlPortOut->write(command);
         if (debug) printf("SystemArchitectureGUIThread: cognitiveControl cycle period is %2.1f\n",cognitive_control_cycle_period);
      }

      /* systemArchitectureGUI component cycle period command */
   
      if (system_architecture_GUI_cycle_period_old != system_architecture_GUI_cycle_period) {
         system_architecture_GUI_cycle_period_old = system_architecture_GUI_cycle_period;
         *cyclePeriod = system_architecture_GUI_cycle_period;
         if (debug) printf("SystemArchitectureGUIThread: systemArchitectureGUI cycle period is %2.1f\n",system_architecture_GUI_cycle_period);
      }

      
   
      /* sensoryInterpretation component event rate command */
   
      if (sensory_interpretation_event_rate_old != sensory_interpretation_event_rate) {
         sensory_interpretation_event_rate_old = sensory_interpretation_event_rate;
         command.clear();
         command.addString("event_rate");
         command.addDouble(sensory_interpretation_event_rate);
         sensoryInterpretationPortOut->write(command);
         if (debug) printf("SystemArchitectureGUIThread: sensoryIntepretation event rate is %2.1f\n",sensory_interpretation_event_rate);
      }
      
      /* childBehaviourClassification component event rate command */
   
      if (child_behaviour_classification_event_rate_old != child_behaviour_classification_event_rate) {
         child_behaviour_classification_event_rate_old = child_behaviour_classification_event_rate;
         command.clear();
         command.addString("event_rate");
         command.addDouble(child_behaviour_classification_event_rate);
         childBehaviourClassificationPortOut->write(command);
         if (debug) printf("SystemArchitectureGUIThread: child behaviour classification event rate is %2.1f\n",child_behaviour_classification_event_rate);
      }

      
      /* cognitiveControl component event rate command */
   
      if (cognitive_control_event_rate_old != cognitive_control_event_rate) {
         cognitive_control_event_rate_old = cognitive_control_event_rate;
         command.clear();
         command.addString("event_rate");
         command.addDouble(cognitive_control_event_rate);
         cognitiveControlPortOut->write(command);
         if (debug) printf("SystemArchitectureGUIThread: cognitiveControl event rate is %2.1f\n",cognitive_control_event_rate);
      }
      
      /* systemArchitectureGUI component event rate command */
   
      if (system_architecture_GUI_event_rate_old != system_architecture_GUI_event_rate) {
         system_architecture_GUI_event_rate_old = system_architecture_GUI_event_rate;
         *eventRate = system_architecture_GUI_event_rate;
         if (debug) printf("SystemArchitectureGUIThread: systemArchitectureGUI event rate is %2.1f\n",system_architecture_GUI_event_rate);
      }


      /**************************************/
      /*                                    */
      /* child behaviour classfication part */
      /*                                    */
      /**************************************/

      /* Primitive 1: getChildBehaviour(<state, probability>)  */

      tempDoubleVector = getChildBehaviourPortIn->read(false);                   // non-blocking version
      /* do {                              
            tempDoubleVector = getChildBehaviourPortIn->read(false);             // blocking version 
      } while ((tempDoubleVector == NULL) && (isStopping() != true)); */

      if (tempDoubleVector != NULL) {

         p = (int)     (*tempDoubleVector)[0];             // number of tuples in vector
            
         for (j=0; j<display_width; j++) {
            buffer1[j] = 0.0;
         }

         if (*verbose) printf("systemArchitectureGUI:        getChildBehaviour      < ");

         for (j=0; j<p; j++) {
            x = (*tempDoubleVector)[j*2+1];  // state element of tuple
            y = (*tempDoubleVector)[j*2+2];  // PDF vlaue of tuple
            if (*verbose) printf("%2.1f %2.1f ", x, y);

            /* fill in the bin for this state */
            
            q = (int)(x)   * (display_width / p);
            r = (int)(x+1) * (display_width / p);

            for (i = q; i < r; i++) {
               buffer1[i] = (float)y; 
            }
         }
         if (*verbose) printf("\n");

         image1->write(buffer1);
 	      display1->draw(image1);
      }
      
      
      /* Primitive 2: getChildMotivation(degree_of_engagement, confidence)  */

      tempDoubleVector = getChildMotivationPortIn->read(false);                   // non-blocking version
      /* do {                              
            tempDoubleVector = getChildMotivationPortIn->read(false);             // blocking version 
      } while ((tempDoubleVector == NULL) && (isStopping() != true)); */
      if (tempDoubleVector != NULL) {
         x = (double) (*tempDoubleVector)[0];
         y = (double) (*tempDoubleVector)[1];
         if (*verbose) printf("systemArchitectureGUI:        getChildMotivation     < %2.1f %2.1f\n",x,y);

         if (index2 == display_width-1) { // last column
            for (j=0; j<display_width-1; j++) {
               buffer2[j] = buffer2[j+1]; // shift all values left to make room
            }
            buffer2[index2] = (float)x;  
         }
         else {
            buffer2[index2] = (float)x;  
            index2++;
         }
                  
         image2->write(buffer2);
 	      display2->draw(image2);

         if (index3 == display_width-1) { // last column
            for (j=0; j<display_width-1; j++) {
               buffer3[j] = buffer3[j+1]; // shift all values left to make room
            }
            buffer3[index3] = (float)y;  
         }
         else {
            buffer3[index3] = (float)y;  
            index3++;
         }
                  
         image3->write(buffer3);
 	      display3->draw(image3);
      }
   
      /* Primitive 3: getChildPerformance(degree_of_performance, confidence)  */

      tempDoubleVector = getChildPerformancePortIn->read(false);                   // non-blocking version
      /* do {                              
            tempDoubleVector = getChildPerformancenPortIn->read(false);             // blocking version 
      } while ((tempDoubleVector == NULL) && (isStopping() != true)); */
      if (tempDoubleVector != NULL) {
         x = (double) (*tempDoubleVector)[0];
         y = (double) (*tempDoubleVector)[1];
         if (*verbose) printf("systemArchitectureGUI:        getChildPerformance    < %2.1f %2.1f\n",x,y);

         if (index4 == display_width-1) { // last column
            for (j=0; j<display_width-1; j++) {
               buffer4[j] = buffer4[j+1]; // shift all values left to make room
            }
            buffer4[index4] = (float)x;  
         }
         else {
            buffer4[index4] = (float)x;  
            index4++;
         }
                  
         image4->write(buffer4);
 	      display4->draw(image4);

         if (index5 == display_width-1) { // last column
            for (j=0; j<display_width-1; j++) {
               buffer5[j] = buffer5[j+1]; // shift all values left to make room
            }
            buffer5[index5] = (float)y;  
         }
         else {
            buffer5[index5] = (float)y;  
            index5++;
         }
                  
         image5->write(buffer5);
 	      display5->draw(image5);
      }

      /**************************************/
      /*                                    */
      /*       cognitive control part       */
      /*                                    */
      /**************************************/

      tempIntVector = getInterventionStatePortIn->read(false);                   // non-blocking version
      /* do {                              
            tempIntVector = getInterventionStatePortIn->read(false);             // blocking version 
      } while ((tempIntVector == NULL) && (isStopping() != true)); */
      if (tempIntVector != NULL) {
         p = (*tempIntVector)[0];
         q = (*tempIntVector)[1];
         r = (*tempIntVector)[2];
         if (*verbose) printf("systemArchitectureGUI:        getInterventionState   < %d %d %d\n", p, q, r);
         if (r == 0) {
            sprintf(modifiedString,"\n Intervention: %2d    State: %2d     Mode: cognition off",p, q);
            message(modifiedString);
         }
         else {
            sprintf(modifiedString,"\n Intervention: %2d    State: %2d     Mode: cognition on",p, q);
            message(modifiedString);
         }
      }


      /* enableRobot(state) primitive */
    
      if (enable_robot_value_old != enable_robot_value) {
         enable_robot_value_old = enable_robot_value;
         
         VectorOf<int> &dataVector = enableRobotPortOut->prepare();
         dataVector.resize(1);

         dataVector(0) = enable_robot_value;

         enableRobotPortOut->write(); 
         if (*verbose) printf("systemArchitectureGUI:        enableRobot            > %d\n",enable_robot_value);
      }

      

      /* grip(state) primitive */
    
      if (grip_value_old != grip_value) {
         grip_value_old = grip_value;
         
         VectorOf<int> &dataVector = gripPortOut->prepare();
         dataVector.resize(1);

         dataVector(0) = grip_value;

         gripPortOut->write(); 
         if (*verbose) printf("systemArchitectureGUI:        grip                   > %d\n",grip_value);
      }

      
      /* moveHand(handDescriptor, x, y, z, roll) primitive */
    
      if (
          (move_left_hand_x_value_old != move_left_hand_x_value) ||
          (move_left_hand_y_value_old != move_left_hand_y_value) ||
          (move_left_hand_z_value_old != move_left_hand_z_value) ||
          (move_left_hand_r_value_old != move_left_hand_r_value)    ) {

         move_left_hand_x_value_old = move_left_hand_x_value;
         move_left_hand_y_value_old = move_left_hand_y_value;
         move_left_hand_z_value_old = move_left_hand_z_value;
         move_left_hand_r_value_old = move_left_hand_r_value;

         VectorOf<double> &dataVector = moveHandPortOut->prepare();
         dataVector.resize(5);

         dataVector(0) = (double) LEFT_HAND;
         dataVector(1) = (double) move_left_hand_x_value;
         dataVector(2) = (double) move_left_hand_y_value;
         dataVector(3) = (double) move_left_hand_z_value;
         dataVector(4) = (double) move_left_hand_r_value;

         moveHandPortOut->write(); 
         if (*verbose) printf("systemArchitectureGUI:        moveHand               > %d %2.1f %2.1f %2.1f %2.1f\n", 
                              LEFT_HAND, move_left_hand_x_value, move_left_hand_y_value, move_left_hand_z_value, move_left_hand_r_value);
      }
         
      if (
          (move_right_hand_x_value_old != move_right_hand_x_value) ||
          (move_right_hand_y_value_old != move_right_hand_y_value) ||
          (move_right_hand_z_value_old != move_right_hand_z_value) ||
          (move_right_hand_r_value_old != move_right_hand_r_value)    ) {

         move_right_hand_x_value_old = move_right_hand_x_value;
         move_right_hand_y_value_old = move_right_hand_y_value;
         move_right_hand_z_value_old = move_right_hand_z_value;
         move_right_hand_r_value_old = move_right_hand_r_value;

         VectorOf<double> &dataVector = moveHandPortOut->prepare();
         dataVector.resize(5);

         dataVector(0) = (double) RIGHT_HAND;
         dataVector(1) = (double) move_right_hand_x_value;
         dataVector(2) = (double) move_right_hand_y_value;
         dataVector(3) = (double) move_right_hand_z_value;
         dataVector(4) = (double) move_right_hand_r_value;

         moveHandPortOut->write(); 
         if (*verbose) printf("systemArchitectureGUI:        moveHand               > %d %2.1f %2.1f %2.1f %2.1f\n", 
                              RIGHT_HAND, move_right_hand_x_value, move_right_hand_y_value, move_right_hand_z_value, move_right_hand_r_value);
      }

     
      /* moveHead(x, y, z) primitive */
      
      if (
          (move_head_x_value_old != move_head_x_value) ||
          (move_head_y_value_old != move_head_y_value) ||
          (move_head_z_value_old != move_head_z_value)    ) {

         move_head_x_value_old = move_head_x_value;
         move_head_y_value_old = move_head_y_value;
         move_head_z_value_old = move_head_z_value;

         VectorOf<double> &dataVector = moveHeadPortOut->prepare();
         dataVector.resize(3);

         dataVector(0) = (double) move_head_x_value;
         dataVector(1) = (double) move_head_y_value;
         dataVector(2) = (double) move_head_z_value;

         moveHeadPortOut->write(); 
         if (*verbose) printf("systemArchitectureGUI:        moveHand               > %2.1f %2.1f %2.1f\n", 
                              move_head_x_value, move_head_y_value);
      }


      /* moveTorso(x, y, z) primitive */
    
      if (
          (move_torso_x_value_old != move_torso_x_value) ||
          (move_torso_y_value_old != move_torso_y_value) ||
          (move_torso_z_value_old != move_torso_z_value)    ) {

         move_torso_x_value_old = move_torso_x_value;
         move_torso_y_value_old = move_torso_y_value;
         move_torso_z_value_old = move_torso_z_value;

         VectorOf<double> &dataVector = moveTorsoPortOut->prepare();
         dataVector.resize(3);

         dataVector(0) = (double) move_torso_x_value;
         dataVector(1) = (double) move_torso_y_value;
         dataVector(2) = (double) move_torso_z_value;

         moveTorsoPortOut->write(); 
         if (*verbose) printf("systemArchitectureGUI:        moveTorso              > %2.1f %2.1f %2.1f\n", 
                              move_torso_x_value, move_torso_y_value);
      }


      /* moveSequence(sequenceDescriptor) primitive */
    
      if (strlen(move_sequence_string) != 0) { 

         sscanf(move_sequence_string, "%d", &move_sequence_value);
         
         VectorOf<int> &dataVector = moveSequencePortOut->prepare();
         dataVector.resize(1);

         dataVector(0) = move_sequence_value;

         moveSequencePortOut->write(); 
         move_sequence_string[0] = '\0'; // clear the message
         if (*verbose) printf("systemArchitectureGUI:        moveSequence           > %d\n",move_sequence_value);
      }

      
      /* say(text, tone) primitive */
    
      if (strlen(say_value) != 0) {

         Bottle &message = sayPortOut->prepare();

         message.clear();
         message.addString(say_value);
         message.addInt(tone_of_voice_value);  
         
         if (*verbose) printf("sensoryInterpretation:        say                    > %s %d\n",message.get(0).asString(),tone_of_voice_value);
         
         sayPortOut->write(); 
         say_value[0] = '\0'; // clear the message
      
      }
   }
}

void SystemArchitectureGUIThread::threadRelease() 
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

void happy_voice_cb(Fl_Widget *b,void *destination) {

   Fl_Round_Button *button;
   button = (Fl_Round_Button *) b;


   *(int *)destination  = HAPPY_VOICE;
   
    //printf("happy_voice_cb: value is now %d\n",*(int *)destination );

   button->setonly();
}

// -----------------------------------------------------------------------------------------------
// sad_voice_cb
//
// handle the sad tone of voice radio button
// -----------------------------------------------------------------------------------------------

void sad_voice_cb(Fl_Widget *b,void *destination) {

   Fl_Round_Button *button;
   button = (Fl_Round_Button *) b;


   *(int *)destination  = SAD_VOICE;
   
    //printf("sad_voice_cb: value is now %d\n",*(int *)destination );

   button->setonly();
}

// -----------------------------------------------------------------------------------------------
// angry_voice_cb
//
// handle the angry tone of voice radio button
// -----------------------------------------------------------------------------------------------

void angry_voice_cb(Fl_Widget *b,void *destination) {

   Fl_Round_Button *button;
   button = (Fl_Round_Button *) b;


   *(int *)destination  = ANGRY_VOICE;
   
    //printf("angry_voice_cb: value is now %d\n",*(int *)destination );

   button->setonly();
}

// -----------------------------------------------------------------------------------------------
// fearful_voice_cb
//
// handle the fearful tone of voice radio button
// -----------------------------------------------------------------------------------------------

void fearful_voice_cb(Fl_Widget *b,void *destination) {

   Fl_Round_Button *button;
   button = (Fl_Round_Button *) b;


   *(int *)destination  = FEARFUL_VOICE;
   
    //printf("fearful_voice_cb: value is now %d\n",*(int *)destination );

   button->setonly();
}