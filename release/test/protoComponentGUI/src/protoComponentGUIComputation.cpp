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
 * 30/10/14  First version validated (David Vernon)
 * 16/11/14  Fixed problem when the default display size is too big for the available window size (David Vernon)
 *
 */ 
 
#include "protoComponentGUI.h"

ProtoComponentGUIThread::ProtoComponentGUIThread(BufferedPort<ImageOf<PixelRgb> > *colourImageIn, 
                                                 BufferedPort<ImageOf<PixelRgb> > *binaryImageIn,
                                                 BufferedPort<Bottle>             *statisticsIn, 
                                                 BufferedPort<VectorOf<int> >     *thresholdOut,
                                                 int *threshold,
                                                 string logoFilename)
{
   colourImagePortIn    = colourImageIn;
   binaryImagePortIn    = binaryImageIn;    
   statisticsPortIn     = statisticsIn;
   thresholdPortOut     = thresholdOut;
   thresholdValue       = threshold;
   thresholdValueDouble = (double)*thresholdValue;
   logoFilenameValue    = logoFilename;
}

bool ProtoComponentGUIThread::threadInit() 
{
   /* initialize variables and create data-structures if needed */

   debug = false;
   rgbDVImage = NULL;
   binaryDVImage = NULL;
   rgbYARPImage = NULL;
   binaryYARPImage = NULL;

   return true;
}

void ProtoComponentGUIThread::run(){

   /* 
    * Here we set up the GUI for the component
    * The particular GUI complements the functionality of the protoComponent component:
    * taking its image output and displaying it, 
    * taking its statistics text and writing it in an output box,
    * using a GUI widget slider to set the threshold and send it to protoComponent,
    * It also reads & displays the original colour image in a separate display. 
    */ 

   Fl_Group *start_protoComponent_GUI;
	Fl_Box *box1, *box2, *heading;
   Fl_PNG_Image *logo=new Fl_PNG_Image(logoFilenameValue.c_str());
 
   int i, j;
   char modifiedString[STRINGLENGTH]; 

	/* variable that determine the presentation of the GUI */ 

	int control_panel_x, control_panel_y;
   int control_panel_width, control_panel_height;
	int heading_x, heading_y, heading_width, heading_height;
	int display_width, display_height;
	int display_x1, display_x2, display_y;
	int display_spacing;
	int display_offset_x, display_offset_y;
	int control_offset_x, control_offset_y;
	int button_size_x, button_size_y, button_spacing_y;
	int input_size_x, input_size_y;
	int title_height;
   int threshold_x, threshold_y;
   int threshold_size_x, threshold_size_y;
	int section_spacing_y;
   int window_width, window_height, border_width, border_height;
   int text_output_width,text_output_height;
   int text_output_x,   text_output_y;
	
   /* intialize the presentation settings */

   window_width  = GetSystemMetrics(SM_CXFULLSCREEN);      // this is the area available for the window on the screen
   window_height = GetSystemMetrics(SM_CYFULLSCREEN);

   border_width  = GetSystemMetrics(SM_CXSIZEFRAME);  
   border_height = GetSystemMetrics(SM_CYSIZEFRAME);

	display_width  = DISPLAY_WIDTH;   
	display_height = DISPLAY_HEIGHT;
 	display_spacing = 25;

   /* Adjust display size to make sure it will fit                                                  */
   /* this is to avoid problems on laptop screens which are wide enough to accommodate the display  */
   /* but not high enough to accommodate the dispaly and the other widgets underneath it            */

   if (display_height > window_height/2) {
      display_width  = display_width / 2;
      display_height = display_height / 2;
   }
     
	heading_x = (window_width - display_width*2 - display_spacing)/2; 
	heading_y = MENUBAR_HEIGHT;
	heading_width = display_width*2 + display_spacing;       // centre over displays
	heading_height = HEADING_HEIGHT*2;                       // logo and caption

	display_offset_x = heading_x ;
	display_offset_y = heading_y + heading_height;
	display_x1 = display_offset_x;
	display_x2 = display_x1 + display_spacing + display_width  ;
	display_y  = display_offset_y;
  
	title_height = 12;

	button_size_x = 130;
	button_size_y = 20;
	button_spacing_y = 5;

	input_size_x = button_size_x;
	input_size_y = button_size_y;

	control_offset_x = 20;
	control_offset_y = 0;

   control_panel_width  = display_width * 1;
   control_panel_height = display_height / 2; // alternative to match the panel height to the number of widgets: 
                                              // control_panel_height = title_height + 1*(5 + button_size_y) + title_height; 
   control_panel_x = display_x2;
   control_panel_y = display_y + display_height + display_spacing;

	section_spacing_y = 8;

   threshold_size_x = control_panel_width - 2 * control_offset_x;
   threshold_size_y = input_size_y;
   threshold_x = control_panel_x + control_offset_x;
   threshold_y = control_panel_y + control_offset_x + 0 * (button_size_y + button_spacing_y); // 0 because threshold is the first widget in the control panel
                                                                                              // increase by 1 for every extra widget added
   text_output_width  = display_width;
   text_output_height = display_height / 2;
   text_output_x = display_offset_x;
   text_output_y = display_offset_y + display_height + display_spacing;
   
   
   /* define a group with all the GUI widgets */

   start_protoComponent_GUI = new Fl_Group(0,MENUBAR_HEIGHT,window_width,window_height-MENUBAR_HEIGHT,"");

   start_protoComponent_GUI->begin();  

      /*  ... note: we don't declare heading because it is declared in the guiUtilities library   */ 

      heading = new Fl_Box(heading_x, heading_y,
                           heading_width,heading_height,"The protoComponentGUI Example" );

      heading->align(FL_ALIGN_INSIDE | FL_ALIGN_CENTER |  FL_ALIGN_IMAGE_OVER_TEXT);
      heading->labelsize(20);
      heading->labelcolor(FL_BLACK);  
      heading->labelfont(FL_HELVETICA_BOLD);
      heading->box(FL_NO_BOX);
      heading->image(logo);
  

      /* boxes to frame images ... note: we don't declare box1 or box2 because they are declared in the guiUtilities library   */ 

      box1 = new Fl_Box(display_x1-BORDER,display_y-BORDER,
                        display_width+2*BORDER,display_height+2*BORDER,"" );
      box1->box(FL_DOWN_BOX);
      box1->align(FL_ALIGN_BOTTOM | FL_ALIGN_CENTER);
      box1->labelsize(12);
 
      box2 = new Fl_Box(display_x2-BORDER,display_y-BORDER,
                        display_width+2*BORDER,display_height+2*BORDER,"" );
      box2->box(FL_DOWN_BOX);
      box2->align(FL_ALIGN_BOTTOM | FL_ALIGN_CENTER);
      box2->labelsize(12);
 

      /* image display ... note: we don't declare display1 or display2 because they declared in the guiUtilities library   */ 
      
      display1 = new DVdisplay(display_x1, display_y, 
	                            display_width,display_height);

      display2 = new DVdisplay(display_x2, display_y, 
   	                         display_width,display_height);

      /* box to frame controls */

      Fl_Box *control_panel;
      control_panel = new Fl_Box(control_panel_x, control_panel_y, control_panel_width, control_panel_height,"" );
      control_panel->align(FL_ALIGN_TOP | FL_ALIGN_CENTER | FL_ALIGN_INSIDE);
      control_panel->labelsize(12);
      control_panel->labelfont(FL_HELVETICA_BOLD);
      control_panel->box(FL_PLASTIC_DOWN_BOX);
 

      /* threshold slider */

      Fl_Value_Slider *threshold;
      threshold = new Fl_Value_Slider(threshold_x, threshold_y, threshold_size_x, threshold_size_y,"Binary Threshold Value");
      threshold->type(FL_HOR_NICE_SLIDER);
      threshold->textsize(11);
      threshold->callback(valuator_cb, &thresholdValueDouble); // NB valuator_cb() is a general-purpose callback defined in guiUtilities.h
      threshold->labelsize(12);                                // other general-purpose callbacks are defined there too, e.g. radio buttons
      threshold->labelfont(FL_HELVETICA_BOLD);                 // some widgets may require you to write your own callback
      threshold->when(FL_WHEN_ENTER_KEY | FL_WHEN_CHANGED | FL_WHEN_RELEASE); 
      threshold->align(FL_ALIGN_TOP);
      threshold->minimum(0);
      threshold->maximum(255);
      threshold->step(1);
      threshold->value((double)*thresholdValue);  // initialize the threshold to the value passed as a parameter
      threshold->box(FL_PLASTIC_DOWN_BOX);
 

      /* Text output ... note: we don't declare text_output because it is declared in the guiUtilities library   */ 

      text_output = new Fl_Multiline_Output(text_output_x, text_output_y, text_output_width, text_output_height,"" );
      text_output->align(FL_ALIGN_TOP | FL_ALIGN_CENTER );
      text_output->labelsize(12);
      text_output->textsize(12);
      //text_output->color(FL_GREEN);
      text_output->value("");
      //text_output->box(FL_THIN_DOWN_FRAME);  // matches the image display frames
      text_output->box(FL_PLASTIC_DOWN_BOX);   // rounded edges for something more visually appealing
 
   start_protoComponent_GUI->end();  // end of the group 

   // create main window
  
   DVwindow = new Fl_Double_Window(window_width-border_width,window_height-border_height,"The DREAM Project");
   DVwindow->position(0,0); 
	
   Fl::visual(FL_DOUBLE|FL_INDEX); // enable double buffering of the window 

   DVwindow->end(); 
   DVwindow->show();

   // add the GUI

   DVwindow->add(start_protoComponent_GUI);
   DVwindow->redraw();
   Fl::check();

   /* write some text in the window ...                                                               */
   /* note that function message() is defined in guiUtilities as a general-purpose routine to write   */
   /* to the predefined display widget text_output ... notice how we didn't declare text_output above */
   /* we didn't have to because it is declared in guiUtilities                                        */

   message("Waiting to start .... image statistics will be displayed here");


   /*************************************************************************************/
   /*   This is where the definition of the GUI ends and the normal run() loop begins   */
   /*************************************************************************************/

   while (isStopping() != true) { // the thread continues to run until isStopping() returns true
 
      DVwindow->redraw();

      Fl::check();         // refresh the GUI   .... we do it this way rather than calling Fl::run() as we nomally would 
                           // because we do not want to cede control to the event loop 

      if (debug) cout << "protoComponentGUIThread: threshold value is " << *thresholdValue << endl;
      
      /* get the threshold value set by the threshold widget callback                                   */
      /* we have to do this because we passed a temporary variable thresholdValueDouble to the callback */
      /* to ensure the type of the argument agrees with the type of the callback parameter (double)     */

      *thresholdValue = (int) thresholdValueDouble;

      if (debug) cout << "protoComponentGUIThread: threshold value is " << *thresholdValue << endl;

      /*** read colour image ... do not block if image is not received ***/

      rgbYARPImage = colourImagePortIn->read(false);

      if (rgbYARPImage != NULL) {

         rgb_width  = rgbYARPImage->width();  
         rgb_height = rgbYARPImage->height();
         rgb_depth  = 3;
    
         if (debug) printf("protoComponentGUI: rgb width = %d, rgb height = %d, rgb depth = %d\n",rgb_width, rgb_height, rgb_depth);
 
         if (rgbDVImage == NULL) {
             rgbDVImage = new DVimage(rgb_width, rgb_height, rgb_depth);
         }


         /*  now copy the image to local format */
 
         for (x = 0; x < rgb_width; x++) {
            for (y = 0; y < rgb_height; y++) {
               rgbPixel = rgbYARPImage->safePixel(x,y);  
               rgbDVImage->put_pixel(x, y, rgbPixel.r, 0);
               rgbDVImage->put_pixel(x, y, rgbPixel.g, 1);
               rgbDVImage->put_pixel(x, y, rgbPixel.b, 2);
           }
         } 
 
         display1->draw(rgbDVImage); 
      }
      else {
         if (debug) printf("protoComponentGUI: colour image not received\n");
      }


      /*** read binary image ... do not block if image is not received ***/

      binaryYARPImage = binaryImagePortIn->read(false);

      if (binaryYARPImage != NULL) {

         binary_width  = binaryYARPImage->width();  
         binary_height = binaryYARPImage->height();
         binary_depth  = 3;
    
         if (debug) printf("protoComponentGUI: rgb width = %d, rgb height = %d, rgb depth = %d\n",binary_width, binary_height, binary_depth);
 
         if (binaryDVImage == NULL) {
             binaryDVImage = new DVimage(binary_width, binary_height, binary_depth);
         }


         /*  now copy the image to local format */
 
         for (x = 0; x < binary_width; x++) {
            for (y = 0; y < binary_height; y++) {
               rgbPixel = binaryYARPImage->safePixel(x,y);  
               binaryDVImage->put_pixel(x, y, rgbPixel.r, 0);
               binaryDVImage->put_pixel(x, y, rgbPixel.g, 1);
               binaryDVImage->put_pixel(x, y, rgbPixel.b, 2);
           }
         } 
 
         display2->draw(binaryDVImage); 
      }
      else {
         if (debug) printf("protoComponentGUI: binary image not received\n");
      }
      
      /* read the image statistics ... do not block if bottle is not received*/

      statisticsMessage = statisticsPortIn->read(false);
     
      if (statisticsMessage != NULL) {

         /* for some strange reason, YARP puts double quotes around every individual string in a bottle so we strip them here */

         j=0;
         for (i=0; i<min(STRINGLENGTH-1,(int)strlen(statisticsMessage->toString().c_str()));i++) {
            if (statisticsMessage->toString().c_str()[i]!='\"') {
               modifiedString[j] = statisticsMessage->toString().c_str()[i];
               j++; 
            }
         }
         modifiedString[j]='\0';


         /* now copy the message to the display */

         message(modifiedString);
         if (debug) cout << "protoComponentGUIThread: statistics message is " << modifiedString << endl;    
      }
      else {
         if (debug) printf("protoComponentGUI: statistics not received\n");
      }


      /*** write the threshold ***/

      VectorOf<int> &thresholdVector = thresholdPortOut->prepare(); 
      thresholdVector.resize(1);
      thresholdVector(0) = *thresholdValue;
 
      thresholdPortOut->write(); 

      if (debug) cout << "protoComponentGUIThread: threshold value is " << *thresholdValue << endl;
 
   }
}

void ProtoComponentGUIThread::threadRelease() 
{
   /* for example, delete dynamically created data-structures */
}
 