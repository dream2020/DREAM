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
 * 20/08/14  First version validated (David Vernon)
 * 06/10/14  Additional ports for threshold input and statistic output (David Vernon)
 *
 */ 
 

#include "protoComponent.h"

ProtoComponentThread::ProtoComponentThread(BufferedPort<ImageOf<PixelRgb> > *imageIn, 
                                           BufferedPort<VectorOf<int> >     *thresholdIn, 
                                           BufferedPort<ImageOf<PixelRgb> > *imageOut,
                                           BufferedPort<Bottle>             *statisticsOut, 
                                           int *threshold)
{
   imagePortIn       = imageIn;
   thresholdPortIn   = thresholdIn;
   imagePortOut      = imageOut; 
   statisticsPortOut = statisticsOut;
   thresholdValue    = threshold;
}

bool ProtoComponentThread::threadInit() 
{
   /* initialize variables and create data-structures if needed */

   debug = false;

   return true;
}

void ProtoComponentThread::run(){

   /* 
    * do some work ....
    * for example, convert the input image to a binary image using the threshold provided 
    */ 
   
   unsigned char value;
   double start;

   start = yarp::os::Time::now(); // start time

   while (isStopping() != true) { // the thread continues to run until isStopping() returns true
 
      if (debug)
         cout << "protoComponentThread: threshold value is " << *thresholdValue << endl;
      
      /* read image ... block until image is received */

      do {
         image = imagePortIn->read(true);
      } while ((image == NULL) && (isStopping() != true));  // exit loop if shutting down;
      
      if (isStopping()) break; // abort this loop to avoid make sure we don't continue and possibly use NULL images 


      /* read threshold ... block if threshold is not received */
      /*
      do {
         thresholdVector = thresholdPortIn->read(false);
      } while ((thresholdVector == NULL) && (isStopping() != true));  // exit loop if shutting down;
      */
  
      /* read threshold ... do not block if threshold is not received */

      thresholdVector = thresholdPortIn->read(false);
    
      if (thresholdVector != NULL) {
            *thresholdValue = (int) (*thresholdVector)[0];
      }


      if (debug)
         cout << "protoComponentThread: threshold value is " << *thresholdValue << endl;
      

      /* write out the binary image */

      ImageOf<PixelRgb> &binary_image = imagePortOut->prepare();
      binary_image.resize(image->width(),image->height());

      numberOfForegroundPixels = 0;

      for (x=0; x<image->width(); x++) {
         for (y=0; y<image->height(); y++) {

             rgbPixel = image->safePixel(x,y);

             if (((rgbPixel.r + rgbPixel.g + rgbPixel.b)/3) > *thresholdValue) {
                value = (unsigned char) 255;
                numberOfForegroundPixels++;
             }
             else {
                value = (unsigned char) 0;
             }

             rgbPixel.r = value;
             rgbPixel.g = value;
             rgbPixel.b = value;

             binary_image(x,y) = rgbPixel;
          }
       }
       
       imagePortOut->write();

       /* write out the image statistics */

       Bottle &statisticsMessage = statisticsPortOut->prepare();

       statisticsMessage.clear();

       statisticsMessage.addInt((int)(yarp::os::Time::now()-start));
       statisticsMessage.addString("seconds elapsed");
       statisticsMessage.addString(" - foreground pixel count is");
       statisticsMessage.addInt(numberOfForegroundPixels);
       statisticsPortOut->write();
   }
}

void ProtoComponentThread::threadRelease() 
{
   /* for example, delete dynamically created data-structures */
}
