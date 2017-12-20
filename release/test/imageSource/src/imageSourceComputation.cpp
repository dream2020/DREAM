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
 * 21/09/09  First version validated   DV
 * 04/01/10  Added encoder functionality DV
 * 05/04/11  Added parameters to specify window shift in x and y directions DV
 * 06/10/14  Moved imageSourceThread->stop() from ImageSource::close() to ImageSource::interruptModule()
 *           to solve a problem with the module hanging when requested to stop DV
 */ 

 
#include "imageSource.h"

ImageSourceThread::ImageSourceThread(BufferedPort<ImageOf<PixelRgb> > *imageOut, 
                                     BufferedPort<VectorOf<double> > *gazeOut,   
                                     BufferedPort<VectorOf<double> > *encoderOut,
                                     string *imageFilename, int period, int *width, int *height, int *noiseLevel, 
                                     int *window, int *random,
                                     int *xShift, int *yShift,
                                     double *horizontalViewAngle, double *verticalViewAngle) : RateThread(period)
{
   debug                    = false;

   imagePortOut             = imageOut;
   gazePortOut              = gazeOut;
   encoderPortOut           = encoderOut;
   imageFilenameValue       = imageFilename;
   widthValue               = width;
   heightValue              = height;
   noiseValue               = noiseLevel;
   windowValue              = window;
   randomValue              = random;
   xShiftValue              = xShift;
   yShiftValue              = yShift;
   horizontalViewAngleValue = horizontalViewAngle;
   verticalViewAngleValue   = verticalViewAngle;
     
   
   if (debug) {
      cout << "ImageSourceThread: image file name  " << *imageFilenameValue << endl;
      cout << "ImageSourceThread: width            " << *widthValue << endl;
      cout << "ImageSourceThread: height           " << *heightValue << endl;
      cout << "ImageSourceThread: noise level      " << *noiseValue << endl;
      cout << "ImageSourceThread: window flag      " << *windowValue << endl;
      cout << "ImageSourceThread: random flag      " << *randomValue << endl;
      cout << "ImageSourceThread: x shift          " << *xShiftValue << endl;
      cout << "ImageSourceThread: y shift          " << *yShiftValue << endl;
      cout << "ImageSourceThread: horizontal FoV   " << *horizontalViewAngleValue << endl;
      cout << "ImageSourceThread: vertical FoV     " << *verticalViewAngleValue << endl;
   }
}

bool ImageSourceThread::threadInit() 
{

   /* generate a seed for the random variables */

   srand((int)(1000*yarp::os::Time::now()));

   /* set the window offsets to zero */

   xOffset = 0;
   yOffset = 0;

   /* ensure the xShift and yShift values are within reasonable limits */

   if (*xShiftValue > *widthValue)
      *xShiftValue = *widthValue;

   if (*yShiftValue > *heightValue)
      *yShiftValue = *heightValue;

   if (debug) {
      cout << "ImageSourceThread::threadInit: xOffset        " << xOffset << endl;
      cout << "ImageSourceThread::threadInit: yOffset        " << yOffset << endl;
      cout << "ImageSourceThread::threadInit: xShift         " << *xShiftValue << endl;
      cout << "ImageSourceThread::threadInit: yShift         " << *yShiftValue << endl;
   }


   /* open the image file and create an image */
   
   if (debug) {
      cout << "ImageSourceThread::threadInit: image file name  " << imageFilenameValue->c_str() << endl;
   }

   if (yarp::sig::file::read(inputImage, imageFilenameValue->c_str())) {
      cout << "ImageSourceThread::threadInit: input image read completed" << endl;
      return true;
   }
   else {
      cout << "ImageSourceThread::threadInit: unable to read image file" << endl;
      return false;
   }
}

void ImageSourceThread::run(){

   if (debug) {
      cout << "ImageSourceThread::run: width          " << *widthValue << endl;
      cout << "ImageSourceThread::run: height         " << *heightValue << endl;
      cout << "ImageSourceThread::run: noise          " << *noiseValue << endl;
      cout << "ImageSourceThread::run: window flag    " << *windowValue << endl;
      cout << "ImageSourceThread::run: random flag    " << *randomValue << endl;
      cout << "ImageSourceThread::run: horizontal FoV " << *horizontalViewAngleValue << endl;
      cout << "ImageSourceThread::run: vertical FoV   " << *verticalViewAngleValue << endl;
      cout << "ImageSourceThread::run: xShift         " << *xShiftValue << endl;
      cout << "ImageSourceThread::run: yShift         " << *yShiftValue << endl;
   }


   /* 
    * copy the image data from file, either scale or extract sub-image, and add noise
    */ 

   double noise;
   double azimuth;
   double elevation;
 
   /* generate offsets for sub-image extraction */

   if (*windowValue == 1) {

      windowFlag = true;

      if ( (*widthValue < inputImage.width())  && (*heightValue < inputImage.height()) ) {

         if (*randomValue == 1) {

            // random position of window

            xOffset = (int)((float)(inputImage.width() - *widthValue)*((float)rand() / (float)(RAND_MAX)));
            yOffset = (int)((float)(inputImage.height() - *heightValue)*((float)rand() / (float)(RAND_MAX)));

         }
         else {

            // regular scan pattern: row major order, with x and y increment equal to the xShift and yShift value
            // so that the window scans the complete image (except for borders at the right-hand side and bottom)

            xOffset = xOffset + *xShiftValue;
            if (xOffset > (inputImage.width() - *widthValue)) {
               xOffset = 0;
               yOffset = yOffset + *yShiftValue;
               if (yOffset > (inputImage.height() - *heightValue)) {
                  yOffset = 0;
               }
            }
         }
      }
      else {
         xOffset = 0;
         yOffset = 0;
      }
   }
   else {
      windowFlag = false;
   }

   if (debug) {
      cout << "ImageSourceThread::run: xOffset     " << xOffset << endl;
      cout << "ImageSourceThread::run: yOffset     " << yOffset << endl;
   }
      
   ImageOf<PixelRgb> &outputImage = imagePortOut->prepare();
   outputImage.resize(*widthValue, *heightValue);
      
   for (x=0; x<outputImage.width(); x++) {
      for (y=0; y<outputImage.height(); y++) {
 
         noise = ((float)rand() / (float)(RAND_MAX));          // 0-1
         noise = 2 * (noise - 0.5);                            // -1 - +1
         noise = noise * (*noiseValue);                        // -noiseValue - +noiseValue

         // decide whether to copy directly or extract a sub-image

         if (windowFlag == false) {

            // scale the image
            
            rgbPixel = inputImage((int)(x * ((float)inputImage.width()/(float)outputImage.width())), 
                                  (int)(y * ((float)inputImage.height()/(float)outputImage.height())));
         }
         else {
              
            // extract a sub-image from the original image
            
            rgbPixel = inputImage((int)(x + xOffset), 
                                  (int)(y + yOffset));
         }

         if (((double) rgbPixel.r + noise) < 0)   
            rgbPixel.r = 0; 
         else if (((double) rgbPixel.r + noise) > 255) 
            rgbPixel.r = 255; 
         else 
            rgbPixel.r = (unsigned char) (rgbPixel.r + noise);

         if (((double) rgbPixel.g + noise) < 0)   
            rgbPixel.g = 0; 
         else if (((double) rgbPixel.g + noise) > 255) 
            rgbPixel.g = 255; 
         else 
            rgbPixel.g = (unsigned char) (rgbPixel.g + noise);

         if (((double) rgbPixel.b + noise) < 0)   
            rgbPixel.b = 0; 
         else if (((double) rgbPixel.b + noise) > 255) 
            rgbPixel.b = 255; 
         else 
            rgbPixel.b = (unsigned char) (rgbPixel.b + noise);
           
         outputImage(x,y) = rgbPixel;
      }
   }
   imagePortOut->write();

   /*
    * Now write out the simulated gaze angles
    */

   if (windowFlag == true) {

      azimuth   = ((xOffset * 2.0) - ((double)inputImage.width() - (double)outputImage.width())) * 
                  (*horizontalViewAngleValue /  (2.0 * (double)inputImage.width()));
   
      elevation = ((yOffset * 2.0) - ((double)inputImage.height() - (double)outputImage.height())) * 
                  (*verticalViewAngleValue /  (2.0 * (double)inputImage.height()));

      
      if (debug) {
         cout << "ImageSourceThread::run: azimuth     " << azimuth << endl;
         cout << "ImageSourceThread::run: elevation   " << elevation << endl;
      }

      VectorOf<double> &vctPos = gazePortOut->prepare();
      vctPos.resize(5);
      vctPos(0) = azimuth;
      vctPos(1) = elevation;
      vctPos(2) = (double)(int)'a'; // absolute (neck reference) coordinates are sent
      vctPos(3) = (double)(int)'s'; // receiver module should do saccades
      vctPos(4) = 0; // saccade index

      // write output vector
       
      gazePortOut->write();
   }
           
   /*
    * Now write out the simulated encoder values
    */

   VectorOf<double> &vctEnc = encoderPortOut->prepare();
   vctEnc.resize(6);
   vctEnc(0) = 0;
   vctEnc(1) = 1;
   vctEnc(2) = 2; 
   vctEnc(3) = 3;  
   vctEnc(4) = 4; 
   vctEnc(5) = 5; 

   // write output vector
       
   encoderPortOut->write();
}

void ImageSourceThread::threadRelease() 
{
   /* delete dynamically created data-structures */
}

