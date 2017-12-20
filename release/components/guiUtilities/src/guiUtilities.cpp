/************************************************************************************************

  guiUtilities
  
  GUI utilities for the DVR package of computer vision and sound analysis programs
  for articulated stereo and binaural heads.

  This project builds the guiUtilities.lib library and places it in the DVR/lib directory.

  David Vernon (www.vernon.eu)

*************************************************************************************************/


// System includes
// ------------------------------------------------------------------------

#include <assert.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/timeb.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>




// GUI includes
// ------------------------------------------------------------------------

#include "guiUtilities.h"



/************************************************************************************************

guiUtilities classes method definitions

      DVdisplay

*************************************************************************************************/
 
 
/*---------------------------------------------------------------------------------------------*

class DVdisplay

*----------------------------------------------------------------------------------------------*/


     DVdisplay::DVdisplay (int x, int y, int w, int h, char *label, DVimage *image) : Fl_Widget(x,y,w,h,label) {
 
        int i,j,k, p, q;
        float sf, fvalue, min, max;
        int min_height,max_height, min_width, max_width;
        unsigned char value;
          struct _timeb tb;  


        // copy the arguments to the data members

        width = w;
        height = h;
        origin_x = x;
        origin_y = y;


        // set defaults for drawing crosshairs and strings

       display_strings = FALSE;
       display_crosshairs = FALSE;
       display_handle_graphics = FALSE;
       number_of_strings = 0;
       number_of_crosshairs = 0;
       number_of_pixel_samples = 0;
         plot.row_number = 0;
         _ftime(&tb); 
         time_stamp = tb.time;

        // create an image of the same size as the display, assuming RGB 

        display_image = new DVimage(w, h, 3); 

        // create a buffer for a single row of data to be used with the plot capability
 
        plot.data = (float *) calloc(w, sizeof(float)); 

        // if necessary, copy the image passed
        
        if (image != NULL) {

           // do a direct copy if we can (same size and type unsigned char, otherwise re-sample and contrast stretch if necessary

           if (w == image->width && h == image->height && image->image_type == DVINT) {

              //printf("DVdisplay::constructor direct copy - %d %d -> %d %d \n",image->height, image->width,  height, width);
               
              display_image->set_image_mode(image->get_image_mode());
              display_image->write(image->idata);
           }
           else {

              // scale and pad if necessary

             display_image->set_image_mode(image->get_image_mode()); 
            
             if (((float)w/(float)h) >= ((float)image->width/(float)image->height)) {

                // pad left and right

                min_height=0; 
                max_height = h;
               sf = (float)(image->height) / (float)(height);
               min_width=(int)((width - (float) image->width / sf )/2.0);
               max_width=(int)((width + (float) image->width / sf )/2.0);
          }
             else {

                // pad top and bottom

               min_width=0; 
               max_width = width;
            sf = (float)(image->width) / (float)(width);
              min_height=(int)((height - (float) image->height / sf )/2.0);
               max_height=(int)((height + (float) image->height / sf )/2.0);
          }

 


             if (image->image_type == DVFLOAT) {

                // find range of values so that we can contrast stretch

                min = 1E10;
               max = -1E10;

               j=image->width*image->height*image->colour_mode;

               for (i=0; i<j; i++){
                  if (*(image->fdata+i) > max) max = *(image->fdata+i);
                  if (*(image->fdata+i) < min) min = *(image->fdata+i);
               }
             }

             // printf("DVdisplay::constructor resampling - image (%d %d) (%d %d) %d -> display (%d %d) (%d %d) %d; %f\n",0,image->width,0,image->height,image->colour_mode,min_width, max_width, min_height, max_height,display_image->colour_mode, sf);

             for (i=0; i<height; i++) {
               for (j=0; j<width; j++) {
                  for (k=1; k<image->colour_mode; k++) {
                     display_image->put_pixel(j,i,(unsigned char)0,k);
                  }
               }
            }


                      
            if (image->image_type == DVINT) {
            for (i=min_height; i<max_height; i++) { 
              for (j=min_width; j<max_width; j++) { 
                     for (k=0; k<image->colour_mode; k++) {
                  value = 255;
                        p = (int)(((float)(j-min_width)*sf));
                  q = (int)(((float)(i-min_height)*sf));
   
                  if (p>=0 && p<image->width && q>=0 && q<image->height) image->get_pixel( p, q, &value, k);
                        display_image->put_pixel(j,i, value,k);
                     }
                  }
               }
            }
            else {
               // displaying a float image so normalize
               for (i=min_height; i<max_height; i++) {
                  for (j=min_width; j<max_width; j++) {
                     for (k=0; k<image->colour_mode; k++) {
                  p = (int)(((float)(j-min_width)*sf));
                  q = (int)(((float)(i-min_height)*sf));

                        if (p>=0 && p<image->width && q>=0 && q<image->height) image->get_pixel( p, q, &fvalue, k);
                        value = (unsigned char) (255 * ((fvalue - min)/(max-min)));
                        display_image->put_pixel(j,i, value,k);
                     }
                  }
               }
            }

            }
        }
     }
    
     DVdisplay::~DVdisplay() {
       delete display_image;
       free (plot.data);   
     }

     void DVdisplay::draw()  {
                  
        //printf("DVdisplay::draw  default draw is being called %d %d %d\n",width, height, display_image->colour_mode);
       
        int x1, y1, x2, y2;
        int i,j;
        unsigned char value = 0;
        int amplitude;

         
        if (display_plot == 2) {
          display_image->initialize(); // reset the image so that it displays blanks
        }
        
        fl_draw_image(display_image->idata, origin_x, origin_y, width, height, display_image->colour_mode,0); 

       
       // draw the character strings

       //printf("number of crosshairs %d\n",number_of_crosshairs);

       if (display_strings == TRUE) {
          for (i=0; i<number_of_strings; i++) {
                         
             fl_color((unsigned char) string[i].red, (unsigned char) string[i].green, (unsigned char) string[i].blue);
             fl_font(FL_HELVETICA, DV_DISPLAY_FONT_SIZE);
             fl_draw(string[i].text,origin_x+string[i].x,origin_y+string[i].y); 
          }
       }


       // draw the crosshairs

       if (display_crosshairs == TRUE) {

          for (i=0; i<number_of_crosshairs; i++) {
             
             //printf("drawing crosshairs %d %d\n",crosshair[i].x,crosshair[i].y); 
 
             fl_color((unsigned char) crosshair[i].red, (unsigned char)crosshair[i].green, (unsigned char) crosshair[i].blue);

             fl_line(origin_x+crosshair[i].x-DV_DISPLAY_CROSSHAIR_LENGTH,
                   origin_y+crosshair[i].y,
                   origin_x+crosshair[i].x+DV_DISPLAY_CROSSHAIR_LENGTH,
                   origin_y+crosshair[i].y); 
             fl_line(origin_x+crosshair[i].x,
                   origin_y+crosshair[i].y-DV_DISPLAY_CROSSHAIR_LENGTH,
                   origin_x+crosshair[i].x,
                   origin_y+crosshair[i].y+DV_DISPLAY_CROSSHAIR_LENGTH); 
          }
       }


       // draw a plot of one row of the image
       //
       // two versions:  1: Place the horizontal axis in the middle
       //                2: Place the horizontal axis on the bottom

       if (display_plot == 1) {
             
          //printf("drawing plot of row %d\n",plot.row_number); 
           
          // draw the axis first
         
          x1 = origin_x;
          y1 = origin_y + height / 2;
          x2 = origin_x + width;
          y2 = origin_y + height / 2;
          fl_color((unsigned char) 255, (unsigned char)255, (unsigned char) 255);
          fl_line(x1, y1, x2, y2); 

          fl_color((unsigned char) plot.red, (unsigned char)plot.green, (unsigned char) plot.blue);

          // now plot the values along the specified row
  
          for (j=0; j<width; j++) { 
             
             //display_image->get_pixel(j,plot.row_number+height/2-1, &value, 0);
             //printf("value at %d %d, %d\n", j, plot.row_number, value);

             amplitude = (int) ((float) plot.data[j] * (float) (height/2) / (float) plot.maximum);
             if (amplitude > height/2)
                amplitude = height/2;
             else if (amplitude < -height/2)
                amplitude = -height/2;

             //printf("amplitude %d\n", amplitude);

             x1 = origin_x + j;
             y1 = origin_y + (height / 2);
             x2 = origin_x + j;
             y2 = origin_y + (height / 2) -  amplitude;   
             fl_line(x1, y1, x2, y2); 
          }
       }
       else if (display_plot == 2) {

          //printf("drawing plot of row %d\n",plot.row_number); 

          // draw the axis first
         
          x1 = origin_x;
          y1 = origin_y + height;
          x2 = origin_x + width;
          y2 = origin_y + height;
          fl_color((unsigned char) 255, (unsigned char)255, (unsigned char) 255);
          fl_line(x1, y1, x2, y2); 
           
          fl_color((unsigned char) plot.red, (unsigned char)plot.green, (unsigned char) plot.blue);
           
          // now plot the values along the specified row
  
          for (j=0; j<width; j++) { 

             //display_image->get_pixel(j,plot.row_number+height/2-1, &value, 0);
             //printf("value at %d %d, %d\n", j, plot.row_number, value);

             amplitude = (int) ((float) plot.data[j] * (float) (height) / (float) plot.maximum);
             if (amplitude > height)
                amplitude = height;
             else if (amplitude < -height)
                amplitude = -height;

             //printf("amplitude %d\n", amplitude);

             x1 = origin_x + j;
             y1 = origin_y + (height);
             x2 = origin_x + j;
             y2 = origin_y + (height) -  amplitude;   
             fl_line(x1, y1, x2, y2); 
          }
        }
     }
 
     void DVdisplay::draw(DVimage *image)  {

        int i,j,k, p, q;
        float sf, fvalue, min, max;
        int min_height,max_height, min_width, max_width;
        unsigned char value = 0;
          float intensity;

        //printf("DVdisplay::draw(image)\n");

        if (image != NULL) {

              display_image->set_image_mode(image->get_image_mode());

            if (width == image->width && height == image->height && image->image_type == DVINT) {

                // do a direct copy

                display_image->write(image->idata);

             }
           else {

                // scale and pad and stretch if necessary

                if (((float)width/(float)height) >= ((float)image->width/(float)image->height)) {

                // pad left and right

                min_height=0; 
                max_height = height;
                sf = (float)(image->height) / (float)(height);
                min_width=(int)((width - (float) image->width / sf )/2.0);
                max_width=(int)((width + (float) image->width / sf )/2.0);
                }
             else {

                // pad top and bottom

                  min_width=0; 
                  max_width = width;
                   sf = (float)(image->width) / (float)(width);
                  min_height=(int)((height - (float) image->height / sf )/2.0);
                  max_height=(int)((height + (float) image->height / sf )/2.0);
                }

                //printf("DVdisplay::draw resampling - %d %d %d -> %d %d %d; \n               %f %d %d %d %d\n",
              //image->width,image->height,image->colour_mode,width,height,display_image->colour_mode, sf, min_width, max_width, min_height, max_height);

            // blank the display

            for (i=0; i<height; i++) {
               for (j=0; j<width; j++) {
                  for (k=0; k<image->colour_mode; k++) {
                     display_image->put_pixel(j,i,(unsigned char)0,k);
                   }
               }
            }

                if (image->image_type == DVFLOAT) {
 
                   // find range of values so that we can contrast stretch

                min = 1E10;
               max = -1E10;

               j=image->width*image->height*image->colour_mode;
 
               for (i=0; i<j; i++){
                  if (*((image->fdata)+i) > max) max = *((image->fdata)+i);
                  if (*((image->fdata)+i) < min) min = *((image->fdata)+i);
               }
            }
 
                // sample the image and normalize if it is a float type
            
            if (image->image_type == DVINT) {
                   for (i=min_height; i<max_height; i++) { 
                      for (j=min_width; j<max_width; j++) { 
                     for (k=0; k<image->colour_mode; k++) {
                            value = 255;
                        p = (int)(((float)(j-min_width)*sf));
                            q = (int)(((float)(i-min_height)*sf));
   
                            if (p>=0 && p<image->width && q>=0 && q<image->height) 
                               image->get_pixel( p, q, &value, k);
                     display_image->put_pixel(j,i, value,k);
                    }
                 }
               }
            }
            else {
               // displaying a float image so normalize
               for (i=min_height; i<max_height; i++) {
                  for (j=min_width; j<max_width; j++) {
                     for (k=0; k<image->colour_mode; k++) {
                                           
                              p = (int)(((float)(j-min_width)*sf));
                              q = (int)(((float)(i-min_height)*sf));

                           if (p>=0 && p<image->width && q>=0 && q<image->height) image->get_pixel( p, q, &fvalue, k);
                         value = (unsigned char) (255 * ((fvalue - min)/(max-min)));
                       display_image->put_pixel(j,i, value,k);
                     }
                  }
               }
            }


                // extract sampled and unscaled data for the plot buffer

                for (j=min_width; j<max_width; j++) { 
               p = (int)(((float)(j-min_width)*sf));
                    intensity=0;
                for (k=0; k<image->colour_mode; k++) {
                       value = 0;
                       fvalue = 0;
                       if (p>=0 && p<image->width) {
                          if (image->image_type == DVINT) {
                             image->get_pixel( p, plot.row_number, &value, k);
                         intensity += value;
                          }
                          else {
                             image->get_pixel( p, plot.row_number, &fvalue, k);
                             intensity += fvalue;
                          }
                       }
               } 
                    plot.data[j] = intensity / image->colour_mode;
            }
           }
       }
     }

     void DVdisplay::draw(DVhs_histogram *hs)  {

        int i,j, p, q;
        int h, s;
        float sf, min, max;
        int min_height,max_height, min_width, max_width;
        int value = 0;

        //printf("DVdisplay::draw(hs)\n");

        display_image->set_image_mode(GREYSCALE_IMAGE);
        hs->get_dimensions(&h, &s);


        // scale and pad and stretch if necessary

        if (((float)width/(float)height) >= ((float)h/(float)s)) {

           // pad left and right

           min_height=0; 
           max_height = height;
           sf = (float)(s) / (float)(height);
           min_width=(int)((width - (float) h / sf )/2.0);
           max_width=(int)((width + (float) h / sf )/2.0);
        }
        else {

           // pad top and bottom

           min_width=0; 
           max_width = width;
           sf = (float)(h) / (float)(width);
           min_height=(int)((height - (float) s / sf )/2.0);
           max_height=(int)((height + (float) s / sf )/2.0);
        }

        //printf("DVdisplay::draw resampling - %d %d -> %d %d; \n               %f %d %d %d %d\n",
        //h,s,width,height, sf, min_width, max_width, min_height, max_height);

        // blank the display

        for (i=0; i<height; i++) {
           for (j=0; j<width; j++) {
              display_image->put_pixel(j,i,(unsigned char)255);  // use white background because most histogram bins will be zero
           }
        }
 
         
        // find range of values so that we can contrast stretch

         min = 1E10;
        max = -1E10;

        j=h*s;
 
        for (i=0; i<j; i++){
           if (*((hs->data)+i) > max) max = (float) *((hs->data)+i);
           if (*((hs->data)+i) < min) min = (float) *((hs->data)+i);
        }
   
        // displaying an int datastructure so normalize

        for (i=min_height; i<max_height; i++) {
           for (j=min_width; j<max_width; j++) {
                                           
              p = (int)(((float)(j-min_width)*sf));
              q = (int)(((float)(i-min_height)*sf));

                if (p>=0 && p<h && q>=0 && q<s) 
                 hs->get_bin( p, q, &value);
              value = (unsigned char) (255 * (((float)value - min)/(max-min)));
              //display_image->put_pixel(j,i, (unsigned char) value);
              //re-map so that low saturation is at the bottom of the display
              display_image->put_pixel(j,max_height - i + min_height - 1, (unsigned char) value); 
           }
        }    
     }




     void DVdisplay::set_origin(int x, int y) {
       origin_x = x;
       origin_y = y;
     }
 

     void DVdisplay::clear()  {

        //printf("DVimage::clear()\n");

        display_image->initialize();

     }

     void DVdisplay::draw_strings(int value)  {

        //printf("DVdisplay::draw_strings()\n");

        display_strings = value;

     }

     void DVdisplay::draw_crosshairs(int value)  {

        //printf("DVimage::draw_crosshairs()\n");

        display_crosshairs = value;

     }


     void DVdisplay::draw_plot(int value)  {

        //printf("DVimage::draw_plot()\n");

        display_plot = value;

     }
     
     void DVdisplay::set_plot_colour(int r, int g, int b) {

        //printf("DVimage::set_plot_colour()\n");

        plot.red = r;
        plot.green = g;
        plot.blue = b;
     }

     void DVdisplay::set_plot_maximum(int value) {

        //printf("DVimage::set_plot_maximum()\n");

        plot.maximum = value;
     }

     void DVdisplay::set_plot_row(int value){

        //printf("DVimage::set_plot_row()\n");

        plot.row_number = value;
     }


     void DVdisplay::add_string(int i, int j, char text[], int r, int g, int b,  int w, int h)  {

        //printf("DVimage::add_string()\n");

        int min_width, min_height;
        float sf;

      
      sf = 1;
      min_width = 0;
      min_height = 0;


        // if the image size is not the same as the display size,
        // we need to scale and perhaps also add an offset to the string coordinates

        if ((width != w) || (height != h)) {
            if (((float)width/(float)height) >= ((float)w/(float)h)) {
               min_height=0; 
               min_width=(int)((width - height*( (float)w/(float)h))/2);
               sf = (float)(height) / (float)(h);
            }
            else {
               min_width=0; 
               min_height=(int)((height - width*( (float)h/(float)w))/2);
               sf = (float)(width) / (float)(w);
            }
        }

        if (number_of_strings < DV_DISPLAY_MAX_NUMBER_OF_STRINGS - 1) {
           number_of_strings++;
           string[number_of_strings-1].x = (int)((i*sf)+min_width );
           string[number_of_strings-1].y = (int)((j*sf)+min_height);
           strncpy(string[number_of_strings-1].text, text, DV_DISPLAY_MAX_STRING_LENGTH);
           string[number_of_strings-1].red   = r;
           string[number_of_strings-1].green = g;
           string[number_of_strings-1].blue  = b;
        }

     }

     void DVdisplay::reset_strings()  {

        //printf("DVdisplay::reset_strings()\n");

         number_of_strings=0;
     }


     void DVdisplay::add_crosshair(int i, int j, int r, int g, int b, int w, int h)  {

        //printf("DVdisplay::add_crosshairs(%d %d %d %d %d % d %d)\n", i, j, r, g, b, w, h);

        int min_width, min_height;
        float sf;

      sf = 1;
      min_width = 0;
      min_height = 0;

        // if the image size is not the same as the display size,
        // we need to scale and perhaps also add an offset to the cross-hair coordinates

        if ((width != w) || (height != h)) {
            if (((float)width/(float)height) >= ((float)w/(float)h)) {
               min_height=0; 
               min_width=(int)((width - height*( (float)w/(float)h))/2);
               sf = (float)(height) / (float)(h);
            }
            else {
               min_width=0; 
               min_height=(int)((height - width*( (float)h/(float)w))/2);
               sf = (float)(width) / (float)(w);
            }
        }

        if (number_of_crosshairs < DV_DISPLAY_MAX_NUMBER_OF_CROSSHAIRS - 1) {
           number_of_crosshairs++;
           crosshair[number_of_crosshairs-1].x = (int)((i*sf)+min_width);  
           crosshair[number_of_crosshairs-1].y = (int)((j*sf)+min_height);  
           crosshair[number_of_crosshairs-1].red   = r;
           crosshair[number_of_crosshairs-1].green = g;
           crosshair[number_of_crosshairs-1].blue  = b;
         //printf("crosshair coords %d %d\n", crosshair[number_of_crosshairs-1].x, crosshair[number_of_crosshairs-1].y);
        }

     }

     void DVdisplay::reset_crosshairs()  {

        //printf("DVdisplay::reset_crosshairs()\n");

         number_of_crosshairs=0;
     }

 
   int DVdisplay::handle(int event) {

      extern Fl_Double_Window *DVwindow;

      //handles events on the image display; only caters so far for mouse clicks to sample pixel values
      
      int i=0, j=0;
      unsigned char r=0, g=0, b=0;
      char string1[1000];
      int temp_display_crosshairs;
      int temp_display_strings;
      struct _timeb tb;  

      
      
      //printf("DVdisplay::handle()\n");
 

      switch (event){

         case FL_PUSH:

               
            // time-stamp this event, i.e. when someone clicked on this image

            _ftime(&tb); 
            time_stamp = tb.time;
          
           if (number_of_pixel_samples == 0) number_of_pixel_samples = 1; 

           i = Fl::event_x() - origin_x;
         j = Fl::event_y() - origin_y;
           display_image->get_pixel( i, j, &r, 0);
           display_image->get_pixel( i, j, &g, 1);
           display_image->get_pixel( i, j, &b, 2);
          sample_x = i;  
         sample_y = j;  
         sample_red   = r;
         sample_green = g;
         sample_blue  = b;
            
           reset_crosshairs();
          reset_strings();
           add_crosshair(i,j, 255, 0, 0, width, height);

           //printf("sample %d %d %d %d\n", i, j, width, height);

           sprintf(string1,"%3d %3d %3d",sample_red, sample_green, sample_blue);
           add_string((int) i - (DV_DISPLAY_FONT_SIZE/2 * strlen(string1)/2),              // centre text
                          (int) j + DV_DISPLAY_FONT_SIZE + 2*DV_DISPLAY_CROSSHAIR_LENGTH,   // under crosshair
                              string1, 0, 0, 255,
                              width, height);
        
           //printf("DVdisplay::handle(): (%d, %d) = (%d, %d, %d)\n", sample_x, sample_y, sample_red, sample_green, sample_blue);


           // save current graphic display status

           temp_display_crosshairs = display_crosshairs;
           temp_display_strings    = display_strings;

           // set display status for the handle

           display_crosshairs = display_handle_graphics;
           //display_strings    = display_handle_graphics;
           display_strings    = FALSE;

           DVwindow->redraw();
           Fl::check();
           
           // restore current graphic display status

           display_crosshairs = temp_display_crosshairs;
           display_strings    = temp_display_strings;

            return 1;

       default:
         return Fl_Widget::handle(event);

      }
   }

    void DVdisplay::handle_graphics_status(int flag)  {

        //printf("DVdisplay::handle_graphics_status(%d)\n", flag);

         display_handle_graphics = flag;

    }

   
    void DVdisplay::reset_samples()  {

        //printf("DVdisplay::reset_sample()\n");

         number_of_pixel_samples=0;
     }



    int DVdisplay::number_of_samples()  {

        //printf("DVdisplay::number_of_samples()\n");

         return(number_of_pixel_samples);
     }

    void DVdisplay::get_sample(int *i, int *j, unsigned char *r, unsigned char *g, unsigned char *b)  {
       if (number_of_pixel_samples == 1) {
         *i = sample_x;  
         *j = sample_y;  
         *r = sample_red;
         *g = sample_green;
         *b = sample_blue;
       }
       else {
         *i = 0;  
         *j = 0;  
         *r = 0;
         *g = 0;
         *b = 0;
       }
    }

   void DVdisplay::get_time_stamp( __time64_t *t)  {
      *t = time_stamp;  
   }

   

/*---------------------------------------------------------------------------------------------*

class DVimage methods

*----------------------------------------------------------------------------------------------*/

	// constructor

	DVimage::DVimage(int w, int h, int mode, unsigned char *image, char *description, int type) {

		  width = w;
        height = h;
        colour_mode = mode; // 1 => GREYSCALE_IMAGE  3 => COLOUR_IMAGE 
        image_type = type;  // DVFLOAT or DVINT 

		  if (w>0 && h>0)
			  if (type == DVINT)
			    idata = new unsigned char[w*h*mode];
			  else
			    fdata = new float[w*h*mode];
			  
		  if (image == NULL) {
			  
			  // no image passed as argument; initialize the image
			  
			  //printf("DVimage constructor: creating image data %d %d %d\n",h,w,mode);

			  if (type == DVINT) {
				  for (int i=0; i<w*h*mode; i++)
					 *(idata+i) = 0;
			  }
			  else {
				  for (int i=0; i<w*h*mode; i++)
					 *(fdata+i) = 0;
           }
		  }
        else {
		  
		     // the image data has been passed as an argument so copy the data

			  if (type == DVINT) {		// image argument and type of DVimage are the same
				  for (int i=0; i<w*h*mode; i++)
					  *(idata+i) = *(image+i);       
			  }
			  else {                     // image argument is unsigned char but type is DVFLOAT so need to convert
				  for (int i=0; i<w*h*mode; i++)
					  *(fdata+i) = (float)(*(image+i));
			  }
		  }


		  if (description == NULL) {

			  // no image description so just create a null string

			  annotation = new char[1];
           annotation = '\0';
		  }
		  else {

			  // do a deep copy on the description

			  annotation = new char[strlen(description)+1];
			  strcpy(annotation, description);
        }
	}
	 


	DVimage::~DVimage() {
		delete [] annotation;
		if (image_type == DVINT) {
			if (width>0 && height>0) delete [] idata;
		}
		else {
			if (width>0 && height>0) delete [] fdata;
		}
	}


	void DVimage::get_size(int *w, int *h) {
		*w = width;
		*h = height;
	}


	int DVimage::get_image_mode() {
		return(colour_mode);
	}


	void DVimage::set_image_mode(int mode) {
		colour_mode = mode;   // needs to be completed
		                 // if we change the colour mode, need to either computer
		                 // greyscale or create other colour bands
	}


   int DVimage::get_image_type() {
		return(image_type);
	}


	// read data from entire image

	void DVimage::read(unsigned char *image) {

		if (image != NULL) {
			if (image_type == DVINT) {
				int j;
				j=width*height*colour_mode;
				for (int i=0; i<j; i++)
					*(image+i) = *(idata+i);
			}
			else { // image_type == DVFLOAT  // may lose data due to casting
				int j;
				j=width*height*colour_mode;
				for (int i=0; i<j; i++)
					*(image+i) = (unsigned char)*(fdata+i);
			}
		}
	}

	void DVimage::read(float *image) {

		if (image != NULL) {
			if (image_type == DVINT) {
				int j;
				j=width*height*colour_mode;
				for (int i=0; i<j; i++)
					*(image+i) = (float) *(idata+i);
			}
			else { // image_type == DVFLOAT
				int j;
				j=width*height*colour_mode;
				for (int i=0; i<j; i++)
					*(image+i) = *(fdata+i);
			}
		}
	}

	  


	  // write data to entire image

	  void DVimage::write(unsigned char *image) {
		  //printf("DVimage::write - %d %d %d\n",width, height, colour_mode);

		  if (image != NULL) {
			  if (image_type == DVINT) {  // unsigned char to unsigned char ... straight copy
			      int j;
				   j=width*height*colour_mode;
				   for (int i=0; i<j; i++)
				      *(idata+i) = *(image+i);
			  }
			  else { // image_type == DVFLOAT; unsigned char to float ... simple cast

			      int j;
				   j=width*height*colour_mode;
				   for (int i=0; i<j; i++)
				      *(fdata+i) = (float)(*(image+i));
			  }
		  }
	  }


	  void DVimage::write(float *image) {
		  //printf("DVimage::write - %d %d %d\n",width, height, colour_mode);

		  if (image != NULL) {
			  if (image_type == DVINT) {  // float to unsigned char ...  dangerous as the cast wmay cause loss of data

				  int j;
				   j=width*height*colour_mode;
				   for (int i=0; i<j; i++)
				      *(idata+i) = (unsigned char) *(image+i);
			  }
			  else { // image_type == DVFLOAT; float to float ... straight copy

			      int j;
				   j=width*height*colour_mode;
				   for (int i=0; i<j; i++)
				      *(fdata+i) = (*(image+i));
			  }
		  }
	  }
	  	  

	  // read and return a single pixel value from the image
	  // note that the byte number needs to be specified if the mode is COLOUR_IMAGE
	  // (it defaults to byte 0, i.e. the first byte)

	  void DVimage::get_pixel(int x, int y, unsigned char *value, int byte_number) {
		  if (image_type == DVINT) { 
			*value = (*(idata+(y*width*colour_mode + x*colour_mode + byte_number)));
		  }
		  else {
			*value = (unsigned char) (*(fdata+(y*width*colour_mode + x*colour_mode + byte_number)));
		  }
	  }

	  void DVimage::get_pixel(int x, int y, float *value, int byte_number) {
		  if (image_type == DVINT) { 
			*value = (float)(*(idata+(y*width*colour_mode + x*colour_mode + byte_number)));
		  }
		  else {
			*value = (*(fdata+(y*width*colour_mode + x*colour_mode + byte_number)));
		  }
	  }

	  
	  // write a single pixel value to the image
	  // note that the byte number needs to be specified if the mode is COLOUR_IMAGE
	  // (it defaults to byte 0, i.e. the first byte)

	  void DVimage::put_pixel(int x, int y, unsigned char value, int byte_number) {

		  //printf("DVimage::put_pixel(%d, %d, %d, %d) colour mode %d\n", x, y, value, byte_number, colour_mode);
		  if (image_type == DVINT) { 
			*(idata+(y*width*colour_mode + x*colour_mode + byte_number))=value;
		  }
		  else {
			*(fdata+(y*width*colour_mode + x*colour_mode + byte_number))=(float)value;
		  }
	  }

	  void DVimage::put_pixel(int x, int y, float value, int byte_number) {

		  //printf("DVimage::put_pixel(%d, %d, %f, %d) colour mode %d\n", x, y, value, byte_number, colour_mode);
		  if (image_type == DVINT) { 
			*(idata+(y*width*colour_mode + x*colour_mode + byte_number))=(unsigned char)value;
		  }
		  else {
			*(fdata+(y*width*colour_mode + x*colour_mode + byte_number))=value;
		  }
	  }

	  // return pointer to the image annotation string
 
	  char *DVimage::read_annotation() {
        return(annotation);
	  }

	  
	  // write a string to the image annotation

	  void DVimage::write_annotation(char *description) {
        if (annotation != NULL)
			  delete [] annotation;
		   annotation = new char[strlen(description)+1];
		   strcpy(annotation, description);
	  }


	  // initialize entire image to zero

	  void DVimage::initialize() {
		  //printf("DVimage::initialize\n");

			  if (image_type == DVINT) {  // unsigned char to unsigned char ... straight copy
			      int j;
				   j=width*height*colour_mode;
				   for (int i=0; i<j; i++)
				      *(idata+i) = 0;
			  }
			  else { // image_type == DVFLOAT; unsigned char to float ... simple cast

			      int j;
				   j=width*height*colour_mode;
				   for (int i=0; i<j; i++)
				      *(fdata+i) = (float)0.0;
			  }
	  }


	  	  
	  // constrast stretch an image in place; minimum and maximum values will be 0 and 255 after invoking this method


	  void DVimage::contrast_stretch() {
		  //printf("DVimage::contrast_stretch\n");

		  float min, max;
		  int i, j;

 
	     // find range of values so that we can contrast stretch

 	     min = 1E10;
	     max = -1E10;

		  j=width*height*colour_mode;
 
		  if (image_type == DVINT) {  //  
	       for (i=0; i<j; i++){
		      if ((float) *(idata+i) > max) max = (float) *(idata+i);
	         if ((float) *(idata+i) < min) min = (float) *(idata+i);
			 }
		  }
		  else { // image_type == DVFLOAT 
	       for (i=0; i<j; i++){
		      if (*(fdata+i) > max) max = *(fdata+i);
	         if (*(fdata+i) < min) min = *(fdata+i);
			 }
		  }

		  		  
		  if (image_type == DVINT) {  //  
	       for (i=0; i<j; i++){
		      *(idata+i)  = (unsigned char) (255 * (( (float)(*(idata+i)) - min)/(max-min)));
			 }
		  }
		  else { // image_type == DVFLOAT 
	       for (i=0; i<j; i++){
		      *(fdata+i)  = (255 * (( (*(fdata+i)) - min)/(max-min)));
			 }
		  } 
	  }


/*---------------------------------------------------------------------------------------------*

class DVhs_histogram methods

*----------------------------------------------------------------------------------------------*/
 
// constructor

DVhs_histogram::DVhs_histogram(int h, int s) {

   hue_dimension = h;
   saturation_dimension = s;
 
   if (h>0 && s>0) {
      data = new int[h*s];
 			  
      for (int i=0; i<h*s; i++)
		   *(data+i) = 0;
 
 	}
}
	 

// destructor

DVhs_histogram::~DVhs_histogram() {

   if (hue_dimension>0 && saturation_dimension>0) delete [] data;
}


void DVhs_histogram::get_dimensions(int *h, int *s) {
   *h = hue_dimension;
   *s = saturation_dimension;
}
 	  	  


void DVhs_histogram::get_bin(int h, int s, int *value) {

   *value = (*(data+(s*hue_dimension + h)));
 
}

 
void DVhs_histogram::put_bin(int h, int s, int value) {

   *(data+(s*hue_dimension + h))=value;
}

 
void DVhs_histogram::increment_bin(int h, int s) {

   *(data+(s*hue_dimension + h)) += 1;
}

 
 // initialize histogram to zero

void DVhs_histogram::initialize() {
 
   int j;
   j=hue_dimension*saturation_dimension;
   for (int i=0; i<j; i++)
      *(data+i) = 0;
}


 // compute the hue and saturation values corresponding to the mode of the histogram
 // these values are in the range [0,360] and [0,1] respectively
 // NB they are NOT the bin numbers of the histogram mode

void DVhs_histogram::hsMode(float *hue, float *saturation) {

   int i, j;
   int max_value;
   int max_i;
   int max_h;
   int max_s;

   max_value = 0;
   max_i = 0;

   j=hue_dimension*saturation_dimension;
   for (i=0; i<j; i++) {
      if (*(data+i) >= max_value) {
         max_value = *(data+i);
         max_i = i;
      }
   }

   max_s = max_i / hue_dimension;
   max_h = max_i - (max_s * hue_dimension);

   if (saturation_dimension <= 1) {
      *saturation = 0.5;
   }
   else {
      *saturation = (float) max_s / (float) (saturation_dimension-1);
   }

   if (hue_dimension <= 1) {
      *hue = 180;
   }
   else {
      *hue = (float) (360 * max_h) / (float) (hue_dimension-1);
   }
}

  
/************************************************************************************************

GUI Callbacks

*************************************************************************************************/

// -----------------------------------------------------------------------------------------------
// browse1_cb
//
// browse for a file and put name in global variable filename1
//
// -----------------------------------------------------------------------------------------------

void browse1_cb(Fl_Widget *b,void *) {

   extern const char *fl_filename_name(const char *);

   Fl_Button *button;

   button = (Fl_Button *) b;
   char filename[200]="";
   extern const char *filename1;
   extern Fl_Input *input1;

   // bring up a file chooser window
   // and put the chosen filename in the input box
 
   filename1 = fl_file_chooser("Browse files","*",filename);
   //printf("browse1_cb: filename chosen: %s\n",filename1);
   button->value(0);
   if (filename1 != NULL)
      input1->value(fl_filename_name(filename1)); // skip the path using fl_filename_name()
}

// -----------------------------------------------------------------------------------------------
// browse2_cb
//
// browse for a file and put name in global variable filename2
//
// -----------------------------------------------------------------------------------------------

void browse2_cb(Fl_Widget *b,void *) {

   extern const char *fl_filename_name(const char *);

   Fl_Button *button;

   button = (Fl_Button *) b;
   char filename[200]="";
   extern const char *filename2;
   extern Fl_Input *input2;

   // bring up a file chooser window
   // and put the chosen filename in the input box
 
   filename2 = fl_file_chooser("Browse files","*",filename);
   //printf("browse2_cb: filename chosen: %s\n",filename2);
   button->value(0);
   if (filename2 != NULL)
      input2->value(fl_filename_name(filename2)); // skip the path using filename_name()
}


// -----------------------------------------------------------------------------------------------
// get_filename1_cb
//
// read the string in the input widget and return in global variable filename1
// -----------------------------------------------------------------------------------------------

void get_filename1_cb(Fl_Widget *temp,void *) {

   extern const char *filename1;

   Fl_Input *f;
   f = (Fl_Input *)temp;
   filename1=f->value();
   //if (filename1 != NULL)   printf("get_filename1_cb: %s\n",filename1);

}


// -----------------------------------------------------------------------------------------------
// get_filename2_cb
//
// read the string in the input widget and return in global variable filename2
// -----------------------------------------------------------------------------------------------

void get_filename2_cb(Fl_Widget *temp,void *) {

   extern const char *filename2;

   Fl_Input *f;
   f = (Fl_Input *)temp;
   filename2=f->value();
   //if (filename2 != NULL) printf("get_filename2_cb: %s\n",filename2);

}



// -----------------------------------------------------------------------------------------------
// save_cb
//
// save a BMP image
// -----------------------------------------------------------------------------------------------

void save_cb(Fl_Widget *b,void *image_ptr) { // NB second parameter is a pointer to a pointer to the image to be saved

   extern const char *fl_filename_name(const char *);
   extern void write_BMP_image(char *filename, DVimage *image);

   extern DVimage *image1;

   Fl_Button *button;

   button = (Fl_Button *) b;
   char filename[200]="";
   extern const char *filename2;
   
   if (filename2 != NULL)
      strcpy(filename,filename2);

   if ( * ((DVimage **) image_ptr) == NULL) printf("*image_ptr == NULL\n");
 
   //printf("save_cb: filename = %s\n",filename);

   if (strcmp(filename,""))
      write_BMP_image(filename,*((DVimage **) image_ptr));   // nb pass a pointer to a pointer to the image
 
}



 


// -----------------------------------------------------------------------------------------------
// processing_single_cb
//
// handle the single processing mode radio button
// -----------------------------------------------------------------------------------------------

void processing_single_cb(Fl_Widget *b,void *destination) {

   Fl_Round_Button *button;
   button = (Fl_Round_Button *) b;


   *(int *)destination  = SINGLE_MODE;
   
   // printf("processing_single_cb: value is now %d\n",*(int *)destination );

   button->setonly();
}



// -----------------------------------------------------------------------------------------------
// processing_continuous_cb
//
// handle the continuous processing mode radio button
// -----------------------------------------------------------------------------------------------

void processing_continuous_cb(Fl_Widget *b,void *destination) {

   Fl_Round_Button *button;
   button = (Fl_Round_Button *) b;

 
   *(int *)destination  = CONTINUOUS_MODE;
   
   //printf("processing_single_cb: value is now %d\n",*(int *)destination );

   button->setonly();
}



 

// -----------------------------------------------------------------------------------------------
// image_source_camera_cb
//
// handle the camera_source radio buttons
// -----------------------------------------------------------------------------------------------

void image_source_camera_cb(Fl_Widget *b,void *destination) {

   Fl_Round_Button *button;
   button = (Fl_Round_Button *) b;

   // set a global variable to correct state 

   *((int *)destination) = CAMERA_SOURCE;

   // printf("image_source_camera_cb: image_source value is now %d\n",*((int *)destination));

   button->setonly();
}


 

// -----------------------------------------------------------------------------------------------
// image_source_file_cb
//
// handle the file_source radio buttons
// -----------------------------------------------------------------------------------------------

void image_source_file_cb(Fl_Widget *b,void *destination) {

   Fl_Round_Button *button;
   button = (Fl_Round_Button *) b;

   // set a global variable to correct state 

   *((int *)destination) = FILE_SOURCE;

   // printf("image_source_file_cb: image_source value is now %d\n",*((int *)destination));

   button->setonly();
}


 
// -----------------------------------------------------------------------------------------------
// view_logpolar_cb
//
// handle the view_mode radio buttons
// -----------------------------------------------------------------------------------------------

void view_logpolar_cb(Fl_Widget *b,void *destination) {

   Fl_Round_Button *button;
   button = (Fl_Round_Button *) b;

   // set a global variable to correct state 

   *((int *)destination) = VIEW_LOGPOLAR;

   // printf("view_logpolar_cb: view_mode value is now %d\n",*((int *)destination));

   button->setonly();
}


 

// -----------------------------------------------------------------------------------------------
// view_cartesian_cb
//
// handle the view_mode radio buttons
// -----------------------------------------------------------------------------------------------

void view_cartesian_cb(Fl_Widget *b,void *destination) {

   Fl_Round_Button *button;
   button = (Fl_Round_Button *) b;

   // set a global variable to correct state 

   *((int *)destination) = VIEW_CARTESIAN;

   // printf("view_cartesian_cb: view_mode value is now %d\n",*((int *)destination));

   button->setonly();
}


 

 
// -----------------------------------------------------------------------------------------------
// debug_cb
//
// handle the debug radio buttons and set the debug variable to appropriately
// -----------------------------------------------------------------------------------------------

void debug_cb(Fl_Widget *b,void *value) {

   extern int GUI_debug;
   Fl_Button *button;
   button = (Fl_Button *) b;

   // set a global variable to correct value 

   if (value==NULL)
      GUI_debug = button->value();
   else
      GUI_debug = *((int *)value);
   
   printf("debug: %d\n", GUI_debug);
 
}
 


// -----------------------------------------------------------------------------------------------
// input_images_two_cb
//
// input_images_two radio button  and set the argument  accordingly
//------------------------------------------------------------------------------------------------

void input_images_two_cb(Fl_Widget *b,void *destination) {

   Fl_Round_Button *button;
   button = (Fl_Round_Button *) b;

   *((int *)destination) = 2;
   
   //printf("number of input images is %d\n", *((int *)destination));

   button->setonly();
}



// -----------------------------------------------------------------------------------------------
// input_images_four_cb
//
// input_images_four radio button  and set the argument  accordingly
//------------------------------------------------------------------------------------------------

void input_images_four_cb(Fl_Widget *b,void *destination) {

   Fl_Round_Button *button;
   button = (Fl_Round_Button *) b;

   *((int *)destination) = 4;
   
   //printf("number of input images is %d\n", *((int *)destination));

   button->setonly();
}


// -----------------------------------------------------------------------------------------------
// camera_one_cb
//
// handle the camera_one radio button and set the argument  appropriately
// -----------------------------------------------------------------------------------------------

void camera_one_cb(Fl_Widget *b, void *destination) {

   Fl_Round_Button *button;
   button = (Fl_Round_Button *) b;

   *((int *) destination) = 1;
   
   //printf("number of cameras is %d\n",*((int *) destination) );

   button->setonly();
 
}


// -----------------------------------------------------------------------------------------------
// camera_two_cb
//
// handle the camera_two radio button and set the argument  appropriately
// -----------------------------------------------------------------------------------------------

void camera_two_cb(Fl_Widget *b, void *destination) {

   Fl_Round_Button *button;
   button = (Fl_Round_Button *) b;

   *((int *) destination) = 2;
   
   // printf("number of cameras is %d\n",*((int *) destination) );

   button->setonly();
 
}
 
 


// -----------------------------------------------------------------------------------------------
// resolution1_cb
//
// handle the resolution1 radio button and set the argument  appropriately
// -----------------------------------------------------------------------------------------------

void resolution1_cb(Fl_Widget *b, void *destination) {

   Fl_Round_Button *button;
   button = (Fl_Round_Button *) b;

   *((int *) destination) = 320;
   
   //printf("camera resolution is %d\n",*((int *) destination) );

   button->setonly();
 
}


// -----------------------------------------------------------------------------------------------
// resolution2_cb
//
// handle the resolution1 radio button and set the argument  appropriately
// -----------------------------------------------------------------------------------------------

void resolution2_cb(Fl_Widget *b, void *destination) {

   Fl_Round_Button *button;
   button = (Fl_Round_Button *) b;

   *((int *) destination) = 640;
   
   // printf("camera resolution is %d\n",*((int *) destination) );

   button->setonly();
 
}
 
  

// -----------------------------------------------------------------------------------------------
// valuator_cb
//
// general purpose valuator callback; assigns the value of the valuator widget to the argument passed
// -----------------------------------------------------------------------------------------------

void valuator_cb (Fl_Widget *b,void *destination) {

   Fl_Valuator *valuator;
   valuator = (Fl_Valuator *) b;

   // set a global variable to correct value 

   *(double *)destination = valuator->value();

   //printf("valuator_cb: assigned value is now %f\n",*(double *)destination);
 
}




// -----------------------------------------------------------------------------------------------
// button_cb
//
// general purpose button callback; assigns the value of the button widget to the argument passed
// -----------------------------------------------------------------------------------------------

void button_cb (Fl_Widget *b,void *destination) {

 
   Fl_Button *button;
   button = (Fl_Button *) b;

   // set a global variable to correct value 

   *(int *)destination = button->value();

   //printf("button_cb: assigned value is now %d\n",*(int *)destination);
  
}
 
  


     
    

// -----------------------------------------------------------------------------------------------
//
// load_multiple_images_cb
//
// Load images from file and display
//
// -----------------------------------------------------------------------------------------------


void load_multiple_images_cb(Fl_Widget *temp,void *value) {

   extern void read_BMP_image(char *filename, DVimage **image);
 
   extern Fl_Double_Window *DVwindow;

   extern DVdisplay *display1;
   extern DVdisplay *display2;
   extern DVdisplay *display3;
   extern DVdisplay *display4;
   extern DVdisplay *display5;
   extern DVdisplay *display6;
   extern DVdisplay *display7;
   extern DVdisplay *display8;
   extern DVdisplay *display9;

   extern DVimage *image1;
   extern DVimage *image2;
   extern DVimage *image3;
   extern DVimage *image4;
   extern DVimage *image5;
   extern DVimage *image6;
   extern DVimage *image7;
   extern DVimage *image8; 
   extern const char *filename1;

   int i;
   char filename[200]="";
   char ext[10]="";


   fl_filename_absolute(filename, filename1);

   if (filename1 != NULL && *filename1 != '\0') {

     // FRAGILE: only works if number of images <= 8
     
      for (i=1; i<= *((int *)value); i++) {

          
         // don't put a serial number on the first image
         
         if (i==1) {
            strcpy(ext,".bmp");
         }
         else {
            sprintf(ext,"%d",i);
            strcat(ext, ".bmp");

         }
         strcpy(filename,filename1);             
         fl_filename_setext(filename,ext);
         //printf("load_multiple_images_cb: filename %s\n",filename);

         switch (i) {
            case 1: read_BMP_image((char *)filename,&image1);
                   display1->draw(image1);
                  break;
         case 2: read_BMP_image((char *)filename,&image2);
                    display2->draw(image2);
                  break;
            case 3: read_BMP_image((char *)filename,&image3);
                  display3->draw(image3);
                    break;
         case 4: read_BMP_image((char *)filename,&image4);
                    display4->draw(image4);
                  break;
            case 5: read_BMP_image((char *)filename,&image5);
                    display5->draw(image5);
                  break;
         case 6: read_BMP_image((char *)filename,&image6);
                    display6->draw(image6);
                  break;
            case 7: read_BMP_image((char *)filename,&image7);
                    display7->draw(image7);
                  break;
         case 8: read_BMP_image((char *)filename,&image8);
                    display8->draw(image8);
                  break;
         }
     }
   }
   DVwindow->redraw();
   Fl::check();
}



// -----------------------------------------------------------------------------------------------
//
// save_multiple_images_cb
//
// Save n images currently held in global images 1-n.
//
// -----------------------------------------------------------------------------------------------


void save_multiple_images_cb(Fl_Widget *temp,void *value) {

   extern DVimage *image1;
   extern DVimage *image2;
   extern DVimage *image3;
   extern DVimage *image4;
   extern DVimage *image5;
   extern DVimage *image6;
   extern DVimage *image7;
   extern DVimage *image8; 


    extern const char *filename2;
   extern const char *fl_filename_name(const char *);
   extern void write_BMP_image(char *filename, DVimage *image);
   extern int fl_filename_absolute(char *out, const char *in);
   extern char *fl_filename_setext(char *f, const char *ext);
 

   char filename[200]="";
   char ext[10]="";

   int i;

   //printf("save_multiple_images_cb: filename %s number %d\n",filename2, *((int *)value) );
 
   fl_filename_absolute(filename, filename2);
    
   if (filename2 != NULL && *filename2 != '\0') {
         
      for (i=1; i<= *((int *)value); i++){

         // don't put a serial number on the first image except if we are saving a stereo pair
         
         if (i==1) {
            if (*((int *)value) == 2) {
               strcpy(ext,"L.bmp");
            } 
            else {
               strcpy(ext,".bmp");
            }
         }
         else {
            if (*((int *)value) == 2) {
               strcpy(ext,"R.bmp");
            } 
            else {
               sprintf(ext,"%d",i);
               strcat(ext, ".bmp");
            }
         }
         strcpy(filename,filename2);             
         fl_filename_setext(filename,ext);
         //printf("%d save_multiple_images_cb: filename %s\n",i, filename);
         
         switch (i) {
            case 1: write_BMP_image(filename,image1);
                  break;
         case 2: write_BMP_image(filename,image2);
                  break;
            case 3: write_BMP_image(filename,image3);
                  break;
         case 4: write_BMP_image(filename,image4);
                  break;
            case 5: write_BMP_image(filename,image5);
                  break;
         case 6: write_BMP_image(filename,image6);
                  break;
            case 7: write_BMP_image(filename,image7);
                  break;
         case 8: write_BMP_image(filename,image8);
                  break;
         }
      }
   }
   else
      printf("save_multiple_images_cb: you must specify a filename\n");
}

 

 



// -----------------------------------------------------------------------------------------------
//
// message
//
// general purpose routine to output a string to the text_output
//
// -----------------------------------------------------------------------------------------------

void message(char *string) {

   extern Fl_Double_Window *DVwindow;
   extern Fl_Multiline_Output *text_output;
   
   text_output->value(string);
   DVwindow->redraw();
   Fl::check();

  // the easiest way to do formatted output when creating the message string is to use 
  // sprintf(string,"This is x %d %f\n",i,x); // for example
 

  return;
}





/************************************************************************************************

General Purpose Image Routines
 

       read_raster_image
       write_raster_image
       read_BMP_image
       write_BMP_image

*************************************************************************************************/



void read_raster_image(char *filename, DVimage **image) {

   unsigned int magic=0xFFFF, width, height, depth, length, type, maptype, maplength;

   unsigned char *byte, temp, byte1, byte2, byte3;

   unsigned int i, j, k, bytes_per_pixel, finished;
   int number_of_bytes;

   FILE *fp_input;
 
   if ((fp_input=fopen(filename, "rb")) == 0) {
     printf("unable to open input file %s \n", filename);
     return;
   }
 
   fread(&magic,4,1,fp_input);
   fread(&width,4,1,fp_input);
   fread(&height,4,1,fp_input);
   fread(&depth,4,1,fp_input);
   fread(&length,4,1,fp_input);
   fread(&type,4,1,fp_input);
   fread(&maptype,4,1,fp_input);
   fread(&maplength,4,1,fp_input);


   //printf("%x %d %d %d %d %d %d %d\n",magic, width, height, depth, length, type, maptype, maplength);

   if (magic != 0x59a66a95) {

      // dealing with a little endian machine so swap the order of all bytes in the header values

      byte = (unsigned char *)(&magic);
      temp = byte[0]; byte[0] = byte[3]; byte[3] = temp;
      temp = byte[1]; byte[1] = byte[2]; byte[2] = temp;

      byte = (unsigned char *)(&width);
      temp = byte[0]; byte[0] = byte[3]; byte[3] = temp;
      temp = byte[1]; byte[1] = byte[2]; byte[2] = temp;

      byte = (unsigned char *)(&height);
      temp = byte[0]; byte[0] = byte[3]; byte[3] = temp;
      temp = byte[1]; byte[1] = byte[2]; byte[2] = temp;

      byte = (unsigned char *)(&depth);
      temp = byte[0]; byte[0] = byte[3]; byte[3] = temp;
      temp = byte[1]; byte[1] = byte[2]; byte[2] = temp;

      byte = (unsigned char *)(&length);
      temp = byte[0]; byte[0] = byte[3]; byte[3] = temp;
      temp = byte[1]; byte[1] = byte[2]; byte[2] = temp;

      byte = (unsigned char *)(&type);
      temp = byte[0]; byte[0] = byte[3]; byte[3] = temp;
      temp = byte[1]; byte[1] = byte[2]; byte[2] = temp;

      byte = (unsigned char *)(&maptype);
      temp = byte[0]; byte[0] = byte[3]; byte[3] = temp;
      temp = byte[1]; byte[1] = byte[2]; byte[2] = temp;

      byte = (unsigned char *)(&maplength);
      temp = byte[0]; byte[0] = byte[3]; byte[3] = temp;
      temp = byte[1]; byte[1] = byte[2]; byte[2] = temp;

    }

   //printf("read_raster_image: %x %d %d %d %d %d %d %d\n",magic, width, height, depth, length, type, maptype, maplength);


   if (*image != NULL) {
         delete *image;
         //printf("read_raster_image: reading to an existing image - deleting the image\n");
         *image = NULL;
   }


   if (depth==8) {
      *image = new DVimage(width,height,1);
      bytes_per_pixel = 1;
   }
   else {
      *image = new DVimage(width,height,3);
      bytes_per_pixel = 3;
   }

   //printf("read_raster_image: image size is %d %d %d\n", (*image)->width, (*image)->height, (*image)->colour_mode);


   if (type !=2 ) {

      // type is not 2 so we assume it is bit-mapped, one byte per colour per pixel

      // make sure there is enough data

      if (length != width*height*bytes_per_pixel) {
         printf("read_raster_image: unable to interpret this type of encoding\n");
      }
      else {

         for (i=0; i< length/bytes_per_pixel; i++) {
            for (j=0; j<bytes_per_pixel; j++) {
               if ((number_of_bytes=fread(&byte1,1,1,fp_input)) < 1) {
                  printf("read_raster_image: insufficient data in file\n");
               }

               //(*image)->put_pixel(i%width, i/width, byte1, j);
               
               if (type == 3 && bytes_per_pixel > 1) {

                  // BGR coding rather than RGB

                  switch (j) {
                    case 0: (*image)->put_pixel(i%width, i/width, byte1, 2);
                       break;
                    case 1: (*image)->put_pixel(i%width, i/width, byte1, 1);
                       break;
                    case 2: (*image)->put_pixel(i%width, i/width, byte1, 0);
                       break;
                  }
               }
               else {
                  // RGB coding or grey-scale

                  if (i%width == 0 && byte1==0 && bytes_per_pixel == 1) {
                     //printf("%d %d\n", i,byte1);
                     i--; // skip end of line byte  ... inserted as a bug fix on 2/5/02
                  }
                  else   
                     (*image)->put_pixel(i%width, i/width, byte1, j);

               }
            } 

         }

      }

   }
   else {

      // type 2 - assume run-length encoded grey-scale image

      i = 0; // track bytes in file
      j = 0; // track pixels in image
      finished = FALSE;

      while (!finished) {

         if ((number_of_bytes=fread(&byte1,1,1,fp_input)) < 1) {
            printf("read_raster_image: insufficient data in file\n");
            finished=TRUE;
         }
         i++; if (i == length) finished = TRUE;

         if (byte1 != 0x80) {

            // record is one byte long, and contains a pixel value

            (*image)->put_pixel(j%width, j/width, byte1, 0);
            j++;

         } 
         else {
         
            if ((number_of_bytes=fread(&byte2,1,1,fp_input)) < 1) {
               printf("read_raster_image: insufficient data in file\n");
               finished=TRUE;
            }
            i++; if (i == length) finished = TRUE;


            if (byte2 == 0) {

               // record is two bytes long, and represents a single pixel value of 0x80

               (*image)->put_pixel(j%width, j/width, byte1, 0);
               j++;
            }
            else {

               // record is three bytes long; second byte is a count and third byte is a value
               if ((number_of_bytes=fread(&byte3,1,1,fp_input)) < 1) {
                  printf("read_raster_image: insufficient data in file\n");
                  finished=TRUE;
               }
               i++; if (i == length) finished = TRUE;

               for (k=0;k<=byte2;k++) {

                  //printf("pixel %d %d : %d %d %d\n",j%width, j/width, byte1,byte2,byte3);

                  (*image)->put_pixel(j%width, j/width, byte3, 0);
                  j++;
               }
            }
         }
      
      }
   }


   //printf("i=%d, j=%d (x=%d; y=%d) i*j=%d \n",i,j,j%width, j/width, i*j);
   
   //printf("%x %d %d %d %d %d %d %d\n",magic, width, height, depth, length, type, maptype, maplength);

   fclose(fp_input);
}


void write_raster_image(char *filename, DVimage *image) {

   int magic, width, height, depth, length, type, maptype, maplength;

   int i, j, k, bytes_per_pixel;
   unsigned char byte;
        
   float fvalue, min, max;

   FILE *fp_output;
 
   if ((fp_output=fopen(filename, "wb")) == 0) {
     printf("unable to open output file %s \n", filename);
     return;
   }
 

   if (image == NULL) return;

   magic = 0x59a66a95;
   image->get_size(&width, &height);
   bytes_per_pixel = image->get_image_mode();
   depth = bytes_per_pixel * 8;
   length= width*height*bytes_per_pixel;
   type=1; // no compression; store colour image in RGB order
   maptype = 0;
   maplength = 0;

   fwrite(&magic,4,1,fp_output);
   fwrite(&width,4,1,fp_output);
   fwrite(&height,4,1,fp_output);
   fwrite(&depth,4,1,fp_output);
   fwrite(&length,4,1,fp_output);
   fwrite(&type,4,1,fp_output);
   fwrite(&maptype,4,1,fp_output);
   fwrite(&maplength,4,1,fp_output);

   //printf("write_raster_image %x %d %d %d %d %d %d %d\n",magic, width, height, depth, length, type, maptype, maplength);

   // stretch if necessary

   //printf("image type: %d\n",image->image_type);

   if (image->image_type == DVFLOAT) {

      // find range of values so that we can contrast stretch

       min = 1E10;
      max = -1E10;

      j=image->width*image->height*image->colour_mode;
 
      for (i=0; i<j; i++){
        if (*((image->fdata)+i) > max) max = *((image->fdata)+i);
        if (*((image->fdata)+i) < min) min = *((image->fdata)+i);
      }
   }


   for (i=0; i< height; i++) {
      for (j=0; j<width; j++) { // row major order ... scan fastest along width 
         for (k=0; k<bytes_per_pixel; k++) {   

            if (image->image_type == DVINT) {
               image->get_pixel(j,i,&byte,k);
            }
            else {
               image->get_pixel(j,i,&fvalue,k);
               byte = (unsigned char) (255 * ((fvalue - min)/(max-min)));
            }

            fwrite(&byte,1,1,fp_output);
         }
      } 
   }

   fclose(fp_output);
}



void read_BMP_image(char *filename, DVimage **image) {

   Fl_RGB_Image *input_RGB_image=NULL;
   input_RGB_image = new Fl_BMP_Image(filename);

   unsigned int width, height, depth;

 
   width  = input_RGB_image->w();
   height = input_RGB_image->h();
   depth  = input_RGB_image->d();


   //printf("read_BMP_image: %d %d %d\n",width, height, depth);

   if (*image != NULL) {
         delete *image;
         //printf("read_raster_image: reading to an existing image - deleting the image\n");
         *image = NULL;
   }


   if (depth==1) {
      *image = new DVimage(width,height,1);
   }
   else {
      *image = new DVimage(width,height,3);
   }
 
   (*image)->write((unsigned char *) *input_RGB_image->data());

}
 
 
void write_BMP_image(char *filename, DVimage *image) {

   // take any image and convert its data to unsigned char RGB 
   // this means we may have to convert from float to int (and therefore contrast stretch)
   // we may also have to convert from greyscale (mode 1) to colour (mode 3)

   extern void saveBMP(char * fileName, unsigned char *buf, int width, int height);

   int width, height,  type;
   long int length;
   long int i, j, mode;
   unsigned char byte;
        
   float fvalue, min, max;
   unsigned char *ibuffer;  
 
   if (image == NULL) return;


   image->get_size(&width, &height);
   mode = image->get_image_mode();
   type = image->get_image_type();
   //printf("write_BMP_image: filename =%s, width=%d, height=%d, depth=%d, type=%d\n",filename,width,height,mode,type);


   // now allocate temporary buffer space

    length= (long) width * (long) height * (long)3;
   
   //printf("write_BMP_image: length = %d\n",length);

   ibuffer = new unsigned char[length];
 
   // convert to integer RGB data

   j= (long) width * (long) height * (long) mode; // size of input data

   //printf("write_BMP_image: j = %d\n",j);

   if (type == DVFLOAT) {

      // find range of values so that we can contrast stretch

       min = 1E10;
      max = -1E10;

 
      for (i=0; i<j; i++){
        if (*((image->fdata)+i) > max) max = *((image->fdata)+i);
        if (*((image->fdata)+i) < min) min = *((image->fdata)+i);
      }   
   }

   //!!!!!!!!!
      
   // now copy data, and convert float to int and greyscale to colour as necessary

    for (i=0; i<j; i++){

      if (type == DVINT) {
        byte = *((image->idata)+i);
      }
      else {
        fvalue = *((image->fdata)+i);
        byte = (unsigned char) (255 * ((fvalue - min)/(max-min)));
      }


      // if the mode of the input image is GREYSCALE_IMAGE then we need to pad the G and B value

      if (mode == GREYSCALE_IMAGE) {
        *(ibuffer+i*3  )=byte;
        *(ibuffer+i*3+1)=byte;
        *(ibuffer+i*3+2)=byte;
      }
      else { // mode == COLOUR_IMAGE
        *(ibuffer+i)=byte;
      }
   }

      
   saveBMP(filename,ibuffer,width,height);
 
   delete [] ibuffer;
 
}


// -----------------------------------------------------------------------------------------------
//
// Save a BMP file
//
//   write a 24-bit BMP file
//
// This code has been adapted from the MfcApp jpeg demonstration program provided 
// on http://www.smalleranimals.com/jpegfile.htm
//
// Main changes are that this save routing now does the RGB - BGR reordering and 
// reverses the order of writing rows so that the data passed to the function can now
// simply be the raw RGB interleaved (R-G-B) unpadded data array associated with 
// normal RGB images
//
// D.V. 8/12/03
//
// -----------------------------------------------------------------------------------------------

void saveBMP(char * fileName, unsigned char *buf, int width, int height)
{

   short res1=0;
   short res2=0;
   long pixoff=54;
   long compression=0;
   long cmpsize=0;
   long colors=0;
   long impcol=0;
   char m1='B';
   char m2='M';
   long row_size;

   int i, j;
   int depth=3;
   unsigned char *p;
   unsigned char BGR[3];

   DWORD widthDW = ((((width) + 31) / 32 * 4) * 24);

   long bmfsize=sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + widthDW * height;   
   long byteswritten=0;

   BITMAPINFOHEADER header;
     header.biSize=40;                   // header size
   header.biWidth=width;
   header.biHeight=height;
   header.biPlanes=1;
   header.biBitCount=24;               // RGB encoded, 24 bit
   header.biCompression=BI_RGB;         // no compression
   header.biSizeImage=0;
   header.biXPelsPerMeter=0;
   header.biYPelsPerMeter=0;
   header.biClrUsed=0;
   header.biClrImportant=0;

   FILE *fp;   
   fp=fopen(fileName,"wb");
   if (fp==NULL) {
      return;
   }

   // should probably check for write errors here...
   
   fwrite((BYTE  *)&(m1),1,1,fp);  
   fwrite((BYTE  *)&(m2),1,1,fp);  
   fwrite((long  *)&(bmfsize),4,1,fp); 
   fwrite((int  *)&(res1),2,1,fp); 
   fwrite((int  *)&(res2),2,1,fp);  
   fwrite((long  *)&(pixoff),4,1,fp);  

   fwrite((BITMAPINFOHEADER *)&header,sizeof(BITMAPINFOHEADER),1,fp);
    
   row_size=width*3;
      
   for (i=height-1; i>=0; i--) {   // BMP pixels are stored bottom to top!!!
      p = buf + width*i*depth;    // address of first pixel on current line
      for (j=0; j<width; j++) {   // row major order ... scan fastest along width

         // BMP pixels are stored BGR order (little endian: thanks Intel!)
         
         BGR[2] = *p++;
         BGR[1] = *p++;
         BGR[0] = *p++;
         fwrite(BGR,3,1,fp);
      }
      // pad to DWORD
      for (DWORD count=row_size;count<widthDW;count++) {
         char dummy=0;
         //fwrite(&dummy,1,1,fp);      //causes problems with image sizes that are not powers of 2
                                 // and doesn't seem to make any difference for other images
      }
   }         
   fclose(fp);
}





// create a new window and display image
// -------------------------------------

void dump_float_image (float *o, int width, int height)
{


   extern Fl_Double_Window *DVwindow;


   static Fl_Group *debug_GUI=NULL;
   static Fl_Window *debug_window=NULL;
   static DVdisplay *debug_display=NULL;

   DVimage *debug_image;

   int i, j;
   unsigned char value;
   float min, max;
   int display_width, display_height;

   int debug;

   display_width = width;
   display_height = height;


   debug = FALSE;
   if (debug) printf("dump_float_image: dimensions %d %d\n",width,height);

   debug_image = new DVimage(width,height);



   // create a new image if necessary

   if (debug_window == NULL) {

      DVwindow->remove(debug_GUI);  
      debug_GUI = new Fl_Group(0,0,width,height,"");

      debug_GUI->begin();  
        debug_window = new Fl_Double_Window(display_width,display_height,"Monocle: Debug");
        debug_display = new DVdisplay(0,0,display_width,display_height); 
        debug_window->show();
      debug_GUI->end();     
   
      DVwindow->add(debug_GUI);
   }
   
   min = 1E10;
   max = -1E10;
   for (i=0; i<height; i++) {
      for (j=0; j<width; j++) {
         if (PIX(o,width,j,i) < min) min = PIX(o,width,j,i);
         if (PIX(o,width,j,i) > max) max = PIX(o,width,j,i);
      }
   }

   
   for (i=0; i<height; i++) {
      for (j=0; j<width; j++) {
         value = (unsigned char) (255.0 * ((PIX(o,width,j,i) - min)/(max-min))  );
         debug_image->put_pixel(j,i,value,0);
      }
   }

   debug_display->draw(debug_image);
   debug_window->redraw();
   debug_window->show();
   Fl::check();

   delete debug_image;
   debug_image = NULL;
        
   //debug_window->hide();
 
   return; 
}

void dump_int_image (int *o, int width, int height)
{

   extern Fl_Double_Window *DVwindow;
   static Fl_Group *debug_GUI=NULL;
   static Fl_Window *debug_window=NULL;
   static DVdisplay *debug_display=NULL;

   DVimage *debug_image;

   int i, j;
   unsigned char value;
   float min, max;
   int display_width, display_height;

   int debug;

   display_width = width;
   display_height = height;


   debug = FALSE;
   if (debug) printf("dump_int_image: dimensions %d %d\n",width,height);

   debug_image = new DVimage(width,height);



   // create a new image if necessary

   if (debug_window == NULL) {//

      DVwindow->remove(debug_GUI); // 
      debug_GUI = new Fl_Group(0,0,width,height,"");

      debug_GUI->begin();  
        debug_window = new Fl_Double_Window(display_width,display_height,"Monocle: Debug");
        debug_display = new DVdisplay(0,0,display_width,display_height); 
        debug_window->show();
      debug_GUI->end();     
   
      DVwindow->add(debug_GUI);
   }//
   
   min = 1E10;
   max = -1E10;
   for (i=0; i<height; i++) {
      for (j=0; j<width; j++) {
         if (PIX(o,width,j,i) < min) min = (float)PIX(o,width,j,i);
         if (PIX(o,width,j,i) > max) max = (float)PIX(o,width,j,i);
      }
   }

   
   for (i=0; i<height; i++) {
      for (j=0; j<width; j++) {
         value = (unsigned char) (255 * ((PIX(o,width,j,i) - min)/(max-min))  );
         debug_image->put_pixel(j,i,value,0);
      }
   }

   debug_display->draw(debug_image);
   debug_window->redraw();
   debug_window->show();
   Fl::check();

   delete debug_image;
   debug_image = NULL;
        
   //debug_window->hide();
 
   return; 
}


void dump_char_image (unsigned char *o, int width, int height)
{

   extern Fl_Double_Window *DVwindow;
   static Fl_Group *debug_GUI=NULL;
   static Fl_Window *debug_window=NULL;
   static DVdisplay *debug_display=NULL;

   DVimage *debug_image;

   int i, j;
   unsigned char value;
   float min, max;
   int display_width, display_height;

   int debug;

   display_width = width;
   display_height = height;


   debug = FALSE;
   if (debug) printf("dump_int_image: dimensions %d %d\n",width,height);

   debug_image = new DVimage(width,height);



   // create a new image if necessary

   if (debug_window == NULL) { //

      DVwindow->remove(debug_GUI);  //
      debug_GUI = new Fl_Group(0,0,width,height,"");

      debug_GUI->begin();  
        debug_window = new Fl_Double_Window(display_width,display_height,"Monocle: Debug");
        debug_display = new DVdisplay(0,0,display_width,display_height); 
        debug_window->show();
      debug_GUI->end();     
   
      DVwindow->add(debug_GUI);
   } //
   
   min = 1E10;
   max = -1E10;
   for (i=0; i<height; i++) {
      for (j=0; j<width; j++) {
         if (PIX(o,width,j,i) < min) min = (float)PIX(o,width,j,i);
         if (PIX(o,width,j,i) > max) max = (float)PIX(o,width,j,i);
      }
   }

   
   for (i=0; i<height; i++) {
      for (j=0; j<width; j++) {
         value = (unsigned char) (255 * ((PIX(o,width,j,i) - min)/(max-min))  );
         debug_image->put_pixel(j,i,value,0);
      }
   }

   debug_display->draw(debug_image);
   debug_window->redraw();
   debug_window->show();
   Fl::check();

   delete debug_image;
   debug_image = NULL;
        
   //debug_window->hide();
 
   return; 
}


/************************************************************************************************

Global Variables: defined global to allow them to be accessed by different callback routines
 
*************************************************************************************************/

Fl_Double_Window *DVwindow;                 // Allow different callbacks to refresh the window 

DVdisplay    *display1; 
DVdisplay    *display2; 
DVdisplay    *display3; 
DVdisplay    *display4; 
DVdisplay    *display5; 
DVdisplay    *display6; 
DVdisplay    *display7; 
DVdisplay    *display8; 
DVdisplay    *display9;
DVdisplay    *display10;
DVdisplay    *display11;

DVimage   *image1=NULL;
DVimage   *image2=NULL;
DVimage   *image3=NULL;
DVimage   *image4=NULL;
DVimage   *image5=NULL;
DVimage   *image6=NULL;
DVimage   *image7=NULL;
DVimage   *image8=NULL;
DVimage   *image9=NULL;
DVimage   *image10=NULL;
DVimage   *image11=NULL;

// general purpose globals typically shared by general purpose call-back routines
// which is why we need to make them global

Fl_Input *input1;                 // input box 1; for inputting read filenames
Fl_Input *input2;                   // input box 2; for inputting write filenames
Fl_Multiline_Output *text_output; // text output box
Fl_Button *button;                // button; typically a start toggle button so that exit can call it to stop
const char *filename1=NULL;       // get_filename1_cb puts the read filename here
const char *filename2=NULL;       // get_filename2_cb puts the write filename here
int GUI_debug = FALSE;            // used to control debug status from GUI


 