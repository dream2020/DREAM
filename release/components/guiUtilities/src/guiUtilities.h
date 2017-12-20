/************************************************************************************************

guiUtilities
  
GUI utilities for DREAM

This package was implemented by David Vernon

It is not compliant with the DREAM software engineering standards and is a support tool

*************************************************************************************************/

#define MENUBAR_HEIGHT      30
#define HEADING_HEIGHT      60
#define BORDER              3

#define DV_DISPLAY_MAX_NUMBER_OF_STRINGS 10    // maximum number of strings that can be drawn on a display
#define DV_DISPLAY_FONT_SIZE 9                 // size of font when drawing text
#define DV_DISPLAY_MAX_STRING_LENGTH 100       // maximum length of strings that can be drawn on a display
#define DV_DISPLAY_MAX_NUMBER_OF_CROSSHAIRS 10 // maximum number of crosshairs that can be drawn on a display
#define DV_DISPLAY_CROSSHAIR_LENGTH 5          // half length of crosshair to be drawn on a display
#define DV_DISPLAY_MAX_NUMBER_OF_SAMPLES 1     // maximum number of pixels values that can be sampled from a display

#define DRAGONFLY_RESOLUTION_X  1024
#define DRAGONFLY_RESOLUTION_Y  768

#define DRAGONFLY_DISPLAY_WIDTH   (DRAGONFLY_RESOLUTION_X/2)
#define DRAGONFLY_DISPLAY_HEIGHT  (DRAGONFLY_RESOLUTION_Y/2)

#define FIREFLY_RESOLUTION_X  640
#define FIREFLY_RESOLUTION_Y  480

#define FIREFLY_DISPLAY_WIDTH   (FIREFLY_RESOLUTION_X)
#define FIREFLY_DISPLAY_HEIGHT  (FIREFLY_RESOLUTION_Y)

#define DISPLAY_WIDTH   640    // general purpose resolution to be used for the image display
#define DISPLAY_HEIGHT  480

#define SOUND_DISPLAY_WIDTH   (750)
#define SOUND_DISPLAY_HEIGHT  (100)


#define MAX_NUMBER_OF_CAMERAS   8   // maximum number of cameras on the IEEE 1394 bus

#define SINGLE_MODE          0   // process input just once
#define CONTINUOUS_MODE       1   // process input continuously
 
#define CAMERA_SOURCE         0   // acquire images from camera
#define FILE_SOURCE          1    // or from file

#define INTERACTIVE_MODE      0   // detect colour/disparity for colour/stereo segmentation interactively (mouse click)
#define MOTION_MODE          1    // detect colour/disparity for colour/stereo segmentation from object motion

#define ADVANCED_SERVO_CONTROLLER 1 // use the advanced servo-controller

#define DVINT                 1   // used to designate an integer-valued image (implemented as unsigned char)
#define DVFLOAT               2   // used to designate a real-valued image (implemented as float)

#define COLOUR_IMAGE          3   // used to designate a colour image by specifying image depth: 3 bytes per pixel
#define GREYSCALE_IMAGE       1   // used to designate a greyscale image by specifying image depth: 1 byte per pixel

#define HUE_SAMPLES           360 // number of hue bins in colour histogram
#define SATURATION_SAMPLES    100 // number of saturation bins in colour histogram

#define CARTESIAN2LOGPOLAR    0   // log-polar transform: cartesian to log-polar mode
#define LOGPOLAR2CARTESIAN    1   // log-polar transform: log-polar to cartesian mode

#define VIEW_LOGPOLAR         0   // log-polar transform: view log-polar image
#define VIEW_CARTESIAN        1   // log-polar transform: view cartesian reconstruction
#define LOG_POLAR_WIDTH       252 // number of angles
#define LOG_POLAR_HEIGHT      152 // number of rings
 
#define MAX_WINNING_UNITS     10  // number of winning units in WTA competitions
#define MAX_PYRAMID_LEVELS    8   // number of levels in an image pyramid

#define PIX(f,width,i,j)   (*((f) + ( (j) * (width) )  + (i) ))
#define _min(X, Y)  ((X) < (Y) ? (X) : (Y))


// FLTK includes
// ------------------------------------------------------------------------

#include <FL/Fl.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Light_Button.H>
#include <FL/Fl_Round_Button.H>
#include <FL/Fl_Radio_Round_Button.H>
#include <FL/Fl_Radio_Light_Button.H>
#include <FL/Fl_Check_Button.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Widget.H>
#include <FL/Fl_Image.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Output.H>
#include <FL/Fl_Menu_Bar.H>
#include <FL/Fl_Menu_Item.H>
#include <FL/Fl_Value_Slider.H>
#include <FL/Fl_Value_Input.H>
#include <FL/Fl_Valuator.H>
#include <FL/fl_draw.H>
#include <FL/x.H>
#include <FL/Fl_Toggle_Button.H>
#include <FL/Fl_File_Chooser.H>
#include <FL/filename.H>
#include <FL/Fl_JPEG_Image.H>
#include <FL/Fl_BMP_Image.H>
#include <FL/Fl_PNG_Image.H>
#include <FL/Fl_GIF_Image.H>
#include <FL/Fl_Multiline_Output.H>
#include <FL/Enumerations.H>


/************************************************************************************************

class definitions

    DVimage
    DVdisplay
    DVhs_histogram

************************************************************************************************/



/*---------------------------------------------------------------------------------------------*

class DVimage

Description: General purpose image, greyscale or colour (RGB)


Methods:     DVimage
             ~DVimage
             get_size
             get_image_type
             set_image_type
             read
             write
             get_pixel
             put_pixel
             read_annotation
             write_annotation
             contrast_stretch

  

DVimage(int w, int h, int mode, unsigned char *image=NULL, char *description=NULL, int type=DVINT) 

   default constructor

   w, h       - width and size of image (required arguments)
   mode       - defaults to GREYSCALE_IMAGE
   image      - image data (defaults to NULL otherwise must by of type unsigned char)
                if no data is provided, the constructor allocates space and initializes the image to 255
	    	    otherwise it does a deep copy of the argument;

		        We do this to avoid the overhead in copying image data which might be generated by 
		        functions which don't use DVimage(s).

   description - image annotation (defaults to NULL)
				 if no description is provided, the constructor allocates space for a string with 
				 one null character '\0' otherwise it does a DEEP copy of the argument

   type        - type of image to be represented; must be either unsigned char (DVINT) or float (DVFLOAT)

~DVimage()    destructor: deletes image data and annotation string
 


void get_size(int *w, int *h)

  finds the size of the image
 
  w, h         - return the width and height of the image
         


int get_image_type();

   returns the type of the image, either DVINT or DVFLOAT


int get_image_mode();

   returns the mode of the image, either GREYSCALE_IMAGE or COLOUR_IMAGE



void set_image_mode(int colour_mode)

    set the colour mode
  


void read(unsigned char *image)

   copy image data from object to argument; arguments as follows
  
   image - if not NULL, returns with a copy of the image data



void write(unsigned char *image)

   copy image data from argument to object; arguments as follows
  
   image - pointer to data to be copied to object

  
void read(float *image)

   copy image data from object to argument; convert from unsigned char to float
  
   image - if not NULL, returns with a copy of the image data



void write(float *image)

   copy image data from argument to object; convert from float to unsigned char
  
   image - pointer to data to be copied to object

                 
unsigned char get_pixel(int x, int y, int byte_number=0)

   read and return a single pixel value from the image
   note that the byte number needs to be specified if the mode is COLOUR_IMAGE 
   (it defaults to byte 0, i.e. the first byte)

   x, y  - width, height coordinates of the pixel
   byte_number - the byte number in a triplet for COLOUR_IMAGES (0, 1, or 2)
   method returns the pixel value



void put_pixel(int x, int y, unsigned char value, int byte_number=0) 

   x, y  - width, height coordinates of the pixel
   byte_number - the byte number in a triplet for COLOUR_IMAGES (0, 1, or 2)


   write a single pixel value to the image
   note that the byte number needs to be specified if the mode is COLOUR_IMAGE
   it defaults to byte 0, i.e. the first byte)



char *read_annotation()

   return pointer to the image annotation string
 
 

void write_annotation(char *description)	  

   write a string to the image annotation
 
   description - string to be written


void initialize()

   initialized entire image to zero


void contrast_stretch();

   constrast stretch an image in place; minimum and maximum values will be 0 and 255 after invoking this method


Public Members:  none

*----------------------------------------------------------------------------------------------*/

class DVimage {

   public:
	  
      DVimage(int w, int h, int mode=GREYSCALE_IMAGE, unsigned char *image=NULL, char *description=NULL, int type = DVINT);
	   ~DVimage();
	   void get_size(int *w, int *h);
      int get_image_type();
	   int get_image_mode();
      void set_image_mode(int mode);
	   void read(unsigned char *image);
	   void read(float *image);
	   void write(unsigned char *image);
	   void write(float *image);
	   void get_pixel(int x, int y, unsigned char *value, int byte_number=0);
	   void get_pixel(int x, int y, float *value, int byte_number=0);
	   void put_pixel(int x, int y, unsigned char value, int byte_number=0);
	   void put_pixel(int x, int y, float value, int byte_number=0);
	   char *read_annotation();
	   void write_annotation(char *description);
	   void initialize();
	   void contrast_stretch();


      //private:
      unsigned char *idata; // default holder for DVINT image type
	   float *fdata;         // holder for DVFLOAT image type
      int width, height;
      int colour_mode;      // COLOUR or GREYSCALE
	   char *annotation;
	   int image_type;       // DVINT or DVFLOAT
};


/*---------------------------------------------------------------------------------------------*

class DVhs_histogram

*----------------------------------------------------------------------------------------------*/


class DVhs_histogram {

   public:
	  
      DVhs_histogram(int h=HUE_SAMPLES, int s=SATURATION_SAMPLES);
	   ~DVhs_histogram();
	   void get_dimensions(int *h, int *s);
 	   void get_bin(int h, int s, int *value);
	   void put_bin(int h, int s, int value);
	   void increment_bin(int h, int s);
	   void initialize();
       void hsMode(float *hue, float *saturation);

   //private:
      int *data;  
      int hue_dimension, saturation_dimension;
};
 

/*---------------------------------------------------------------------------------------------*

class DVdisplay

Description: General purpose image display
             Sub-class of Fl_widget


Methods:     DVdisplay
             ~DVdisplay
             draw
             set_origin
             clear



DVdisplay(int x, int y, int w, int h, char *label=0, DVimage *image=NULL)

   default constructor

   x, y       - origin of display (width and height coordinates of top right corner)
   w, y       - width and height of the display
   label      - optional label to be printed under the display
   image      - DVimage to be displayed (defaults to NULL)


~DVdisplay()

   destructor

  
void draw()

   display the image data currently in the display image
 


void draw(DVimage *image)   

   copy image to the display image and display it

   image - the DVimage object to be displayed




void set_origin(int x, int y) 

   sets the origin of the display (i.e. the position on the screen where it will be displayed)

   x, y  - the new origin


void clear()

   clear image data currently in the display image and call draw()
 


Public Members:  none

*----------------------------------------------------------------------------------------------*/

class DVdisplay : public Fl_Widget{

   // Create an image display on screen and show an image

   public:
	  
      DVdisplay (int x, int y, int w, int h, char *label=0, DVimage *image=NULL);
	  ~DVdisplay();
	  void draw();
	  void draw(DVimage *image);
	  void draw(DVhs_histogram *hs);
	  void set_origin(int x, int y);
	  void clear();	  
	  void draw_strings(int value);
 	  void draw_crosshairs(int value);   
 	  void add_string(int i, int j, char text[], int red, int green, int blue, int image_width, int image_height);  
 	  void reset_strings();   
 	  void add_crosshair(int i, int j, int red, int green, int blue, int image_width, int image_height);  
 	  void reset_crosshairs();
	  int  handle(int event); // handles events on the display; only cater so far in mouse clicks to sample pixel values
     void handle_graphics_status(int flag); // turn on and off graphics for event handling
     void reset_samples();
     void get_sample(int *x, int *y, unsigned char *red, unsigned char *green, unsigned char *blue); 
     int  number_of_samples();
 	  void draw_plot(int value);   
     void set_plot_colour(int red, int green, int blue);
     void set_plot_maximum(int value);
     void set_plot_row(int value);
     void get_time_stamp( __time64_t *value);
   private:
      DVimage *display_image;
      int origin_x, origin_y, width, height;

	  int display_strings;         // display string if TRUE
	  int display_crosshairs;      // display crosshairs if TRUE
	  int display_plot;            // display plot of a row of the image
 	  int display_handle_graphics; // display crosshairs and strings when handling events if TRUE
 
	  int number_of_strings;	  
	  struct {
		  char text[DV_DISPLAY_MAX_STRING_LENGTH]; 
		  int x;
	     int y;
		  int red,green,blue;
	  } string[DV_DISPLAY_MAX_NUMBER_OF_STRINGS];

	  int number_of_crosshairs;	  
	  struct {
		  int x;
	     int y;
		  int red, green, blue;
	  } crosshair[DV_DISPLAY_MAX_NUMBER_OF_CROSSHAIRS];

     struct {
        float *data;
        int row_number;
        unsigned char red, green, blue;
        int maximum;
     } plot;

     int number_of_pixel_samples;	  
	  int sample_x;
	  int sample_y;
     unsigned char sample_red, sample_green, sample_blue;

     __time64_t time_stamp;
};

 

/************************************************************************************************

GUI prototype definitions
 
*************************************************************************************************/

void browse1_cb(Fl_Widget *b,void *);
void browse2_cb(Fl_Widget *b,void *);
void get_filename1_cb(Fl_Widget *temp,void *);
void get_filename2_cb(Fl_Widget *temp,void *);
void load_multiple_images_cb(Fl_Widget *temp,void *);
void save_multiple_images_cb(Fl_Widget *temp,void *);
//void camera_one_cb(Fl_Widget *temp,void *);
//void camera_two_cb(Fl_Widget *temp,void *);
//void processing_single_cb(Fl_Widget *temp,void *);
//void processing_continuous_cb(Fl_Widget *temp,void *);
//void image_source_file_cb(Fl_Widget *temp,void *);
//void image_source_camera_cb(Fl_Widget *temp,void *);
void browse_cb(Fl_Widget *b,void *);
void get_filename1_cb(Fl_Widget *b,void *);
void read_raster_image(char *filename, DVimage **image);
void valuator_cb(Fl_Widget *temp,void *);
void button_cb(Fl_Widget *temp,void *);
void debug_cb(Fl_Widget *temp,void *);
//void servo_enable_cb(Fl_Widget *temp,void *);
//void servo_mover_cb(Fl_Widget *temp,void *);
//void resetServo_cb(Fl_Widget *temp,void *);
void message(char *string); // the easiest way to do formatted output when creating the message string is to use 
                            // sprintf(string,"This is x %d %f\n",i,x); // for example
                            // note too that message() is dedicated to the globally-defined extern Fl_Multiline_Output *text_output 

//void view_logpolar_cb(Fl_Widget *temp,void *);
//void view_cartesian_cb(Fl_Widget *temp,void *);

//void resolution1_cb(Fl_Widget *b, void *destination);
//void resolution2_cb(Fl_Widget *b, void *destination);
 
 
 

/************************************************************************************************

Global variables: defined global to allow them to be accessed by different callback routines
 
*************************************************************************************************/


extern Fl_Double_Window *DVwindow;                // Allow different callbacks to refresh the window 

extern DVdisplay    *display1; 
extern DVdisplay    *display2; 
extern DVdisplay    *display3; 
extern DVdisplay    *display4; 
extern DVdisplay    *display5; 
extern DVdisplay    *display6; 
extern DVdisplay    *display7; 
extern DVdisplay    *display8; 
extern DVdisplay    *display9;
extern DVdisplay    *display10;

extern DVimage      *image1;
extern DVimage      *image2;
extern DVimage      *image3;
extern DVimage      *image4;
extern DVimage      *image5;
extern DVimage      *image6;
extern DVimage      *image7;
extern DVimage      *image8;
extern DVimage      *image9;
extern DVimage      *image10;
extern DVimage      *image11; 

// general purpose globals typically shared by general purpose call-back routines
// which is why we need to make them global

extern Fl_Input *input1;                 // input box 1; for inputting read filenames
extern Fl_Input *input2;	              // input box 2; for inputting write filenames
extern Fl_Multiline_Output *text_output; // text output box
extern Fl_Button *button;                // button; typically a start toggle button so that exit can call it to stop
extern const char *filename1;            // get_filename1_cb puts the read filename here
extern const char *filename2;            // get_filename2_cb puts the write filename here
extern int GUI_debug;                    // used to control debug status from GUI


 