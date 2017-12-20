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
 * 23/11/14  First version completed (David Vernon)
 *
 */ 
 

#include "sensoryInterpretation.h"

SensoryInterpretationThread::SensoryInterpretationThread(
                                                         bool                            *verboseValue,          // simulation control parameters
                                                         bool                            *cycleValue,
                                                         double                          *cyclePeriodValue,
                                                         double                          *eventRateValue,                                                      
                                                         BufferedPort<VectorOf<int>>     *checkMutualGazeOut,    // functional parameters 
                                                         BufferedPort<VectorOf<double>>  *getArmAngleOut,
                                                         BufferedPort<VectorOf<double>>  *getBodyOut,
                                                         BufferedPort<VectorOf<double>>  *getBodyPoseOut, 
                                                         BufferedPort<VectorOf<int>>     *getEyeGazeIn,
                                                         BufferedPort<VectorOf<double>>  *getEyeGazeOut, 
                                                         BufferedPort<VectorOf<double>>  *getEyesOut,
                                                         BufferedPort<VectorOf<double>>  *getFacesOut, 
                                                         BufferedPort<VectorOf<double>>  *getGripLocationIn, 
                                                         BufferedPort<VectorOf<double>>  *getGripLocationOut, 
                                                         BufferedPort<VectorOf<double>>  *getHandsOut,
                                                         BufferedPort<VectorOf<double>>  *getHeadOut,
                                                         BufferedPort<VectorOf<double>>  *getHeadGazeIn, 
                                                         BufferedPort<VectorOf<double>>  *getHeadGazeOut, 
                                                         BufferedPort<VectorOf<double>>  *getObjectsIn,
                                                         BufferedPort<VectorOf<double>>  *getObjectsOut, 
                                                         BufferedPort<VectorOf<double>>  *getObjectTableDistanceIn, 
                                                         BufferedPort<VectorOf<double>>  *getObjectTableDistanceOut, 
                                                         BufferedPort<VectorOf<double>>  *getSoundDirectionIn,
                                                         BufferedPort<VectorOf<double>>  *getSoundDirectionOut,  
                                                         BufferedPort<VectorOf<double>>  *identifyFaceIn,
                                                         BufferedPort<VectorOf<int>>     *identifyFaceOut,  
                                                         BufferedPort<VectorOf<double>>  *identifyFaceExpressionIn,  
                                                         BufferedPort<VectorOf<int>>     *identifyFaceExpressionOut,  
                                                         BufferedPort<VectorOf<double>>  *identifyObjectIn,
                                                         BufferedPort<VectorOf<int>>     *identifyObjectOut, 
                                                         BufferedPort<VectorOf<double>>  *identifyTrajectoryIn,  
                                                         BufferedPort<VectorOf<int>>     *identifyTrajectoryOut,  
                                                         BufferedPort<VectorOf<int>>     *identifyVoiceOut,
                                                         BufferedPort<Bottle>            *recognizeSpeechOut, 
                                                         BufferedPort<VectorOf<double>>  *trackFaceIn,
                                                         BufferedPort<VectorOf<double>>  *trackFaceOut,  
                                                         BufferedPort<VectorOf<double>>  *trackHandIn,
                                                         BufferedPort<VectorOf<double>>  *trackHandOut, 
                                                         BufferedPort<VectorOf<double>>  *trackObjectIn,  
                                                         BufferedPort<VectorOf<double>>  *trackObjectOut
                                                         )
{
   verbose                       = verboseValue;    
   cycle                         = cycleValue;
   cyclePeriod                   = cyclePeriodValue;
   eventRate                     = eventRateValue;
   checkMutualGazePortOut        = checkMutualGazeOut;
   getArmAnglePortOut            = getArmAngleOut;
   getBodyPortOut                = getBodyOut;
   getBodyPosePortOut            = getBodyPoseOut;
   getEyeGazePortIn              = getEyeGazeIn;
   getEyeGazePortOut             = getEyeGazeOut;
   getEyesPortOut                = getEyesOut;  
   getFacesPortOut               = getFacesOut;
   getGripLocationPortIn         = getGripLocationIn; 
   getGripLocationPortOut        = getGripLocationOut;
   getHandsPortOut               = getHandsOut;
   getHeadPortOut                = getHeadOut;
   getHeadGazePortIn             = getHeadGazeIn; 
   getHeadGazePortOut            = getHeadGazeOut;  
   getObjectsPortIn              = getObjectsIn; 
   getObjectsPortOut             = getObjectsOut;
   getObjectTableDistancePortIn  = getObjectTableDistanceIn;  
   getObjectTableDistancePortOut = getObjectTableDistanceOut;
   getSoundDirectionPortIn       = getSoundDirectionIn; 
   getSoundDirectionPortOut      = getSoundDirectionOut; 
   identifyFacePortIn            = identifyFaceIn;
   identifyFacePortOut           = identifyFaceOut;  
   identifyFaceExpressionPortIn  = identifyFaceExpressionIn;    
   identifyFaceExpressionPortOut = identifyFaceExpressionOut;  
   identifyObjectPortIn          = identifyObjectIn;
   identifyObjectPortOut         = identifyObjectOut; 
   identifyTrajectoryPortIn      = identifyTrajectoryIn;  
   identifyTrajectoryPortOut     = identifyTrajectoryOut;  
   identifyVoicePortOut          = identifyVoiceOut;
   recognizeSpeechPortOut        = recognizeSpeechOut; 
   trackFacePortIn               = trackFaceIn;
   trackFacePortOut              = trackFaceOut;  
   trackHandPortIn               = trackHandIn;
   trackHandPortOut              = trackHandOut;  
   trackObjectPortIn             = trackObjectIn;  
   trackObjectPortOut            = trackObjectOut;  
}

bool SensoryInterpretationThread::threadInit() 
{
   /* initialize variables and create data-structures if needed */

   debug             = true;     // print debug statements

   return true;
}

void SensoryInterpretationThread::run(){

   int               i, j;
   int               p;
   double            a, b, c;
   double            x, y, z;
   double            x1, y1, z1, t1;
   double            x2, y2, z2;
   double            x3, y3, z3;
   double            rad;
   VectorOf<double>  *tempDoubleVector;
   VectorOf<int>     *tempIntVector;

   target_primitive = 0;
   current_primitive = 0;  

   start_time      = yarp::os::Time::now(); // start time
   cumulative_time = 0.0;
   elapsed_time    = 0.0;

   while (isStopping() != true) { // the thread continues to run until isStopping() returns true
 
      //if (*verbose) printf("sensoryInterpretationThread\n");
  
      /* process simulation parameters based on how much time has passed */
      /* --------------------------------------------------------------- */

      current_time     = yarp::os::Time::now();
      elapsed_time     = current_time - start_time;
      start_time       = current_time;                // reset start time so that we measure elapsed time since the last calculation of elapsed time
      cumulative_time += elapsed_time;

      if (cumulative_time > *cyclePeriod) {
         cumulative_time = 0.0;                         // reset
         if (*cycle) { 

            /* in selective mode, only the target primitive has a non-zero event rate */
            /* to make it easier to check that everything is working as expected      */

            for (i=0; i<NUMBER_OF_PRIMITIVES; i++) {
               event_rate[i] = 0;     
            }                        

            event_rate[target_primitive] = *eventRate;
            target_primitive = (target_primitive + 1) % NUMBER_OF_PRIMITIVES;
         }
         else {
            for (i=0; i<NUMBER_OF_PRIMITIVES; i++) {
               event_rate[i] = *eventRate;     // events per second
            }
         }

         //printf("sensoryInterpretation:        cycle period %f\n", *cyclePeriod);
         //printf("sensoryInterpretation:        event rate   %f\n", *eventRate);

      }

      /* Primitive 1 */
      
      /* checkMutualGaze()                                                                               */
      /* ----------------------------------------------------------------------------------------------- */
      /* The checkMutualGaze() primitive returns a value of true or false depending on whether the       */
      /* child and the robot are looking at each other. Note, when detecting mutual gaze, the robot only */
      /* has to determine whether or not the child looks at the robot's head, but not necessarily at the */
      /* robot's eyes. The gaze has to be held for a minimum period, e.g. 3 seconds.                     */
      /*                                                                                                 */
      /* Output TRUE (1) or FALSE (0) to port /checkMutualGaze:o                                         */
 

      /* find out how many events have occurred and perform that many write operations on the port */

      lambda = event_rate[current_primitive++] * elapsed_time; // number of events = number of events per second x time in seconds
      //if (*verbose) printf("sensoryInterpretation:        checkMutualGaze rate lambda %4.3f \n", lambda); 
      count = samplePoisson(lambda);     

      for (i=0; i<count; i++) {

         VectorOf<int> &dataVector = checkMutualGazePortOut->prepare();
         dataVector.resize(1);

         p = rand() % 2;  // randomly select between FALSE (0) and TRUE (1)
         dataVector(0) = p;

         checkMutualGazePortOut->write(); 
         if (*verbose) printf("sensoryInterpretation:        checkMutualGaze        > %d\n", p);
      }

      /* Primitive 2 */

      /* getArmAngle(left_azimuth, left_elevation, right_azimuth, right_elevation)                             */
      /* ----------------------------------------------------------------------------------------------------- */
      /* The getArmAngle() primitive returns the azimuth and elevation angles of the child’s upper left        */
      /* and right arms, defined with respect to the robot’s torso, i.e. in a child-centred frame of reference */
      /*                                                                                                       */
      /* Output arm angles to port /getArmAngle:o                                                              */


      /* find out how many events have occurred and perform that many write operations on the port */

      lambda = event_rate[current_primitive++] * elapsed_time; 
      count = samplePoisson(lambda);
      //if (*verbose) printf("sensoryInterpretation:        getArmAngle lambda %4.3f \n",  lambda);
       
      for (i=0; i<count; i++) {

         VectorOf<double> &dataVector = getArmAnglePortOut->prepare();
         dataVector.resize(4);

         a = ((double) rand() / (double) RAND_MAX);  // randomly select values;
         b = ((double) rand() / (double) RAND_MAX);  // randomly select values;
         x = ((double) rand() / (double) RAND_MAX);  // randomly select values;
         y = ((double) rand() / (double) RAND_MAX);  // randomly select values;

         dataVector(0) = a;
         dataVector(1) = b;
         dataVector(2) = x;
         dataVector(3) = y;

         getArmAnglePortOut->write(); 
         if (*verbose) printf("sensoryInterpretation:        getArmAngle            > %2.1f %2.1f %2.1f %2.1f\n", a, b, x, y);
      }

      /* Primitive 3 */
   
      /* getBody(x, y, z)                                                               */
      /* ------------------------------------------------------------------------------ */
      /* The getBody() primitive returns the location of the centre of the child’s body */
      /*                                                                                */
      /* Output the location of the centre of the child’s body to port /getBody:o       */

      lambda = event_rate[current_primitive++] * elapsed_time; 
      count = samplePoisson(lambda);
      //if (*verbose) printf("sensoryInterpretation:        getArmAngle lambda %4.3f \n",  lambda);
       
      for (i=0; i<count; i++) {

         VectorOf<double> &dataVector = getBodyPortOut->prepare();
         dataVector.resize(3);

         x = ((double) rand() / (double) RAND_MAX);  // randomly select values;
         y = ((double) rand() / (double) RAND_MAX);  // randomly select values;
         z = ((double) rand() / (double) RAND_MAX);  // randomly select values;

         dataVector(0) = x;
         dataVector(1) = y;
         dataVector(2) = z;

         getBodyPortOut->write(); 
         if (*verbose) printf("sensoryInterpretation:        getBody                > %2.1f %2.1f %2.1f\n", x, y, z);
      }


      /* Primitive 4 */
        
      /* getBodyPose(<joint_i>)                                                                                                 */
      /* ---------------------------------------------------------------------------------------------------------------------- */
      /* The getBodyPose() primitive returns a list (denoted by < >) of joint angles describing the child’s current body pose   */
      /* The joint angles correspond to a canonical representation of a body skeleton, yet to be defined                        */
      /* The first item in the list is the number of joint angles that follow.                                                  */
      /*                                                                                                                        */
      /* Output a vector of joint angles specifying the child’s current body post to port /getBodyPose:o                        */


      lambda = event_rate[current_primitive++] * elapsed_time; 
      count = samplePoisson(lambda);
      //if (*verbose) printf("sensoryInterpretation:        getArmAngle lambda %4.3f \n",  lambda);
       
      for (i=0; i<count; i++) {

         VectorOf<double> &dataVector = getBodyPosePortOut->prepare();
         dataVector.resize(NUMBER_OF_JOINTS+1);

         dataVector(0) = NUMBER_OF_JOINTS;

         for (j=0; j<NUMBER_OF_JOINTS; j++) {
            body_angles[j] = ((double) rand() / (double) RAND_MAX);  // randomly select values;
            dataVector(j+1) = body_angles[j];
         }

         getBodyPosePortOut->write(); 

         if (*verbose) {
            printf("sensoryInterpretation:        getBodyPose            > ");
            for (j=0; j<NUMBER_OF_JOINTS; j++) {
               printf("%2.1f ", body_angles[j]);
            }
            printf("\n");
         }
      }

      /* Primitive 5 */

      /* getEyeGaze(eye, x, y, z)              */
      /* ------------------------------------- */
      /* The getEyeGaze() primitive returns the location in the world at which the child’s eye gaze is     */
      /* directed. The gaze direction is given by the line connecting the child’s eye and that location.   */
      /* Any point on that line could be the object of the child’s attention. The eye to be used is passed */ 
      /* in the eye parameter (0 for left a 1 for right).                                                  */ 
      /*                                                                                                   */
      /* Input eye from port /getEyeGaze:i                                                                 */
      /* Output x, y, z, to port /getEyeGaze:o                                                             */

      tempIntVector = getEyeGazePortIn->read(false);                   // non-blocking version
      /* do {                              
            tempIntVector = getEyeGazePortIn->read(false);             // blocking version 
      } while ((tempdIntVector == NULL) && (isStopping() != true)); */
      if (tempIntVector != NULL) {
         p = (int) (*tempIntVector)[0];
         if (*verbose) printf("sensoryInterpretation:        getEyeGaze          < %d\n", p);
      }
  
      lambda = event_rate[current_primitive++] * elapsed_time; 
      count = samplePoisson(lambda);  
      //if (*verbose) printf("sensoryInterpretation:        getEyeGaze lambda %4.3f \n", lambda);

      for (i=0; i<count; i++) {

         VectorOf<double> &dataVector = getEyeGazePortOut->prepare();
         dataVector.resize(3);

         x = ((double) rand() / (double) RAND_MAX);  // randomly select values;
         y = ((double) rand() / (double) RAND_MAX);  // randomly select values;
         z = ((double) rand() / (double) RAND_MAX);  // randomly select values;

         dataVector(0) = x;
         dataVector(1) = y;
         dataVector(2) = z;

         getEyeGazePortOut->write(); 
         if (*verbose) printf("sensoryInterpretation:        getEyeGaze             > %2.1f %2.1f %2.1f\n",x,y,z);
      }

      
      /* Primitive 6 */

      /* getEyes(eyeL_x, eyeL_y, eyeL_z, eyeR_x, eyeR_y, eyeR_z)                    */
      /* -------------------------------------------------------------------------- */
      /* The getEyes() primitive the locations of the child’s left and right eyes,  */
      /* specified in Cartesian coordinates in a world frame of reference.          */
      /*                                                                            */
      /* Output the locations of the child’s left and right eyes to port /getEyes:o */

      lambda = event_rate[current_primitive++] * elapsed_time; 
      count = samplePoisson(lambda);
      //if (*verbose) printf("sensoryInterpretation:        getEyes lambda %4.3f \n",  lambda);
       
      for (i=0; i<count; i++) {
        
         VectorOf<double> &dataVector = getEyesPortOut->prepare();
         dataVector.resize(6);

         x = ((double) rand() / (double) RAND_MAX);  // randomly select values;
         y = ((double) rand() / (double) RAND_MAX);  // randomly select values;
         z = ((double) rand() / (double) RAND_MAX);  // randomly select values;
         a = ((double) rand() / (double) RAND_MAX);  // randomly select values;
         b = ((double) rand() / (double) RAND_MAX);  // randomly select values;
         c = ((double) rand() / (double) RAND_MAX);  // randomly select values;

         dataVector(0) = x;
         dataVector(1) = y;
         dataVector(2) = z;
         dataVector(3) = a;
         dataVector(4) = b;
         dataVector(5) = c;

         getEyesPortOut->write(); 
         if (*verbose) printf("sensoryInterpretation:        getEyes                > %2.1f %2.1f %2.1f %2.1f %2.1f %2.1f\n", x, y, z, a, b, c);
      }

       
      /* Primitive 7 */

      /* getFaces(<x, y, z>)                                                                             */
      /* ----------------------------------------------------------------------------------------------- */
      /* The getFaces() primitive returns a list (denoted by < >) of locations where faces are visible   */
      /* in the therapy environment (viewed from any of the therapy cameras). Each location is specified */
      /* by a triple of Cartesian coordinates x, y, and z.                                               */
      /* The first item in the list is the number of triples that follow.                                */
      /* The maximum number of faces is given by NUMBER_OF_FACES                                         */
      /*                                                                                                 */
      /* Output locations of faces that are visible in the therapy environment to port /getFaces:o       */


      lambda = event_rate[current_primitive++] * elapsed_time; 
      count = samplePoisson(lambda);
      //if (*verbose) printf("sensoryInterpretation:        getFaces lambda %4.3f \n",  lambda);
       
      for (i=0; i<count; i++) {

         VectorOf<double> &dataVector = getFacesPortOut->prepare();
         dataVector.resize(3*NUMBER_OF_FACES+1);

         dataVector(0) = NUMBER_OF_FACES;

         if (*verbose) printf("sensoryInterpretation:        getFaces               > "); 
 
         for (j=0; j<NUMBER_OF_FACES; j++) {
    
            x = ((double) rand() / (double) RAND_MAX);  // randomly select values;
            y = ((double) rand() / (double) RAND_MAX);  // randomly select values;
            z = ((double) rand() / (double) RAND_MAX);  // randomly select values;

            if (*verbose) printf("%2.1f %2.1f %2.1f ", x, y, z);

            dataVector(j*3 + 1) = x;
            dataVector(j*3 + 2) = y;
            dataVector(j*3 + 3) = z;
         }
         getFacesPortOut->write(); 
         if (*verbose) printf("\n"); 
      }


       /* Primitive 8 */

      /* getGripLocation(object_x, object_y, object_z, grip_x, grip_y, grip_z)                        */
      /* -------------------------------------------------------------------------------------------- */
      /* The getGripLocation() primitive returns the location of a grip point of a play object in the */
      /* therapy environment. The location of the object is passed to the primitive.                  */
      /*                                                                                              */
      /* Input location of an object from port /getGripLocation:i                                     */
      /* Output grip point to port /getGripLocation:o                                                 */

      tempDoubleVector = getGripLocationPortIn->read(false);                   // non-blocking version
      /* do {                              
            tempDoubleVector = getGripLocationPortIn->read(false);             // blocking version 
      } while ((tempdDoubleVector == NULL) && (isStopping() != true)); */
      if (tempDoubleVector != NULL) {
         x = (double) (*tempDoubleVector)[0];
         y = (double) (*tempDoubleVector)[1];
         z = (double) (*tempDoubleVector)[2];
         if (*verbose) printf("sensoryInterpretation:        getGripLocation     < %2.1f %2.1f %2.1f\n", x, y, z);
      }
  
      lambda = event_rate[current_primitive++] * elapsed_time; 
      count = samplePoisson(lambda);  
      //if (*verbose) printf("sensoryInterpretation:        getGripLocation lambda %4.3f \n", lambda);

      for (i=0; i<count; i++) {
         
         VectorOf<double> &dataVector = getGripLocationPortOut->prepare();
         dataVector.resize(3);

         x = ((double) rand() / (double) RAND_MAX);  // randomly select values;
         y = ((double) rand() / (double) RAND_MAX);  // randomly select values;
         z = ((double) rand() / (double) RAND_MAX);  // randomly select values;

         dataVector(0) = x;
         dataVector(1) = y;
         dataVector(2) = z;

         getGripLocationPortOut->write(); 
         if (*verbose) printf("sensoryInterpretation:        getGripLocation        > %2.1f %2.1f %2.1f\n",x,y,z);
      }


   
      /* Primitive 9 */

      /* getHands(<x, y, z>)                                                                             */
      /* ----------------------------------------------------------------------------------------------- */
      /* The getHands() primitive returns a list (denoted by < >) of locations where hands are visible   */
      /* in the therapy environment (viewed from any of the therapy cameras). Each location is specified */
      /* by a triple of Cartesian coordinates x, y, and z.                                               */
      /* The first item in the list is the number of triples that follow.                                */
      /* The maximum number of hands is given by NUMBER_OF_HANDS                                         */
      /*                                                                                                 */
      /* Output locations of hands that are visible in the therapy environment to port /getHands:o       */

 
      lambda = event_rate[current_primitive++] * elapsed_time; 
      count = samplePoisson(lambda);
      //if (*verbose) printf("sensoryInterpretation:        getHands lambda %4.3f \n",  lambda);
       
      for (i=0; i<count; i++) {

         VectorOf<double> &dataVector = getHandsPortOut->prepare();
         dataVector.resize(3*NUMBER_OF_HANDS+1);
 
         dataVector(0) = NUMBER_OF_HANDS;

         if (*verbose) printf("sensoryInterpretation:        getHands               > ");

         for (j=0; j<NUMBER_OF_HANDS; j++) {
    
            x = ((double) rand() / (double) RAND_MAX);  // randomly select values;
            y = ((double) rand() / (double) RAND_MAX);  // randomly select values;
            z = ((double) rand() / (double) RAND_MAX);  // randomly select values;

            if (*verbose) printf("%2.1f %2.1f %2.1f ", x, y, z);

            dataVector(j*3 + 1) = x;
            dataVector(j*3 + 2) = y;
            dataVector(j*3 + 3) = z;
         }

         if (*verbose) printf("\n");

         getHandsPortOut->write(); 
      }

 
   
      /* Primitive 10 */
   
      /* getHead(x, y, z)                                                                          */
      /* ----------------------------------------------------------------------------------------- */
      /* The getHead() primitive returns the locations of the child’s head, specified in Cartesian */
      /* coordinates in a world frame of reference.                                                */
      /*                                                                                           */
      /* Output location of the child’s head to port /getHead:o                                    */

      lambda = event_rate[current_primitive++] * elapsed_time; 
      count = samplePoisson(lambda);
      //if (*verbose) printf("sensoryInterpretation:        getHead lambda %4.3f \n",  lambda);
       
      for (i=0; i<count; i++) {

         VectorOf<double> &dataVector = getHeadPortOut->prepare();
         dataVector.resize(3);

         x = ((double) rand() / (double) RAND_MAX);  // randomly select values;
         y = ((double) rand() / (double) RAND_MAX);  // randomly select values;
         z = ((double) rand() / (double) RAND_MAX);  // randomly select values;

         dataVector(0) = x;
         dataVector(1) = y;
         dataVector(2) = z;

         getHeadPortOut->write(); 
         if (*verbose) printf("sensoryInterpretation:        getHead                > %2.1f %2.1f %2.1f\n", x, y, z);
      }

 
      /* Primitive 11 */
   
      /* getHeadGaze(x, y, z)                                                                                         */
      /* ------------------------------------------------------------------------------------------------------------ */
      /* The getHeadGaze() primitive returns the location in the world at which the child’s head is directed.         */
      /* The head direction is given by the line connecting the mid-point between the child’s eyes and that location. */
      /* Any point on that line could be the object of the child’s attention.                                         */
      /*                                                                                                              */
      /* Output location of the point in the world at which the child’s head is directed to port /getHeadGaze:o       */
 

      lambda = event_rate[current_primitive++] * elapsed_time; 
      count = samplePoisson(lambda);
      //if (*verbose) printf("sensoryInterpretation:        getHeadGaze lambda %4.3f \n",  lambda);
       
      for (i=0; i<count; i++) {

         VectorOf<double> &dataVector = getHeadGazePortOut->prepare();
         dataVector.resize(3);

         x = ((double) rand() / (double) RAND_MAX);  // randomly select values;
         y = ((double) rand() / (double) RAND_MAX);  // randomly select values;
         z = ((double) rand() / (double) RAND_MAX);  // randomly select values;

         dataVector(0) = x;
         dataVector(1) = y;
         dataVector(2) = z;

         getHeadGazePortOut->write(); 
         if (*verbose) printf("sensoryInterpretation:        getHeadGaze1           > %2.1f %2.1f %2.1f\n", x, y, z);
      }

      
      /* Primitive 12 */

      /* getHeadGaze(p_x1, p_y1, p_z1, p_x2, p_y2, p_z2, p_x3, p_y3, p_z3, x, y, z)                                                                    */
      /* -------------------------------------------------------------------------------------------------------------------- */
      /* This alternative version of the getHeadGaze() primitive returns the location on a flat surface                       */
      /* (i.e. a plane defined by a vector of Cartesian coordinates ) at which the child’s head is directed.                  */
      /* Typically, this plane corresponds to the table on which play objects are placed.                                     */
      /*                                                                                                                      */
      /* Input a triple of 3-D coordinates specifying the plane of a flat surface from port /getHeadGaze:i                    */
      /* Output location of the location a point on that surface at which the child’s head is directed to port /getHeadGaze:o */

      tempDoubleVector = getHeadGazePortIn->read(false);                   // non-blocking version
      /* do {                              
            tempDoubleVector = getHeadGazePortIn->read(false);             // blocking version 
      } while ((tempdDoubleVector == NULL) && (isStopping() != true)); */
      if (tempDoubleVector != NULL) {
         x1 = (double) (*tempDoubleVector)[0];
         y1 = (double) (*tempDoubleVector)[1];
         z1 = (double) (*tempDoubleVector)[2];
         x2 = (double) (*tempDoubleVector)[3];
         y2 = (double) (*tempDoubleVector)[4];
         z2 = (double) (*tempDoubleVector)[5];
         x3 = (double) (*tempDoubleVector)[6];
         y3 = (double) (*tempDoubleVector)[7];
         z3 = (double) (*tempDoubleVector)[8];
         if (*verbose) printf("sensoryInterpretation:        getHeadGaze2       < %2.1f %2.1f %2.1f %2.1f %2.1f %2.1f \n", x1, y1, z1,  x2, y2, z2);
      }
  
      lambda = event_rate[current_primitive++] * elapsed_time; 
      count = samplePoisson(lambda);  
      //if (*verbose) printf("sensoryInterpretation:        getHeadGaze lambda %4.3f \n", lambda);

      for (i=0; i<count; i++) {
         
         VectorOf<double> &dataVector = getHeadGazePortOut->prepare();
         dataVector.resize(3);

         x = ((double) rand() / (double) RAND_MAX);  // randomly select values;
         y = ((double) rand() / (double) RAND_MAX);  // randomly select values;
         z = ((double) rand() / (double) RAND_MAX);  // randomly select values;

         dataVector(0) = x;
         dataVector(1) = y;
         dataVector(2) = z;

         getHeadGazePortOut->write(); 
         if (*verbose) printf("sensoryInterpretation:        getHeadGaze2           > %2.1f %2.1f %2.1f\n",x,y,z);
      }

 
   
      /* Primitive 13 */

      /* getObjects(<x, y, z>)                                                                                        */
      /* ------------------------------------------------------------------------------------------------------------ */
      /* The getObjects() primitive returns a list (denoted by < >) of locations where play objects are visible       */
      /* in the therapy environment (viewed from any of the therapy cameras). Each location is specified by a triple  */
      /* of Cartesian coordinates x, y, and z. The first item in the list is the number of triples that follow.       */
      /* The maximum number of objects is given by NUMBER_OF_OBJECTS                                                  */
      /*                                                                                                              */
      /* Output locations of objects that are visible in the therapy environment to port /getObjects:o                */


      lambda = event_rate[current_primitive++] * elapsed_time; 
      count = samplePoisson(lambda);
      //if (*verbose) printf("sensoryInterpretation:        getObjects1 lambda %4.3f \n",  lambda);
       
      for (i=0; i<count; i++) {

         VectorOf<double> &dataVector = getObjectsPortOut->prepare();
         dataVector.resize(3*NUMBER_OF_OBJECTS+1);
          
         dataVector(0) = NUMBER_OF_OBJECTS;

         if (*verbose) printf("sensoryInterpretation:        getObjects1            > ");

         for (j=0; j<NUMBER_OF_OBJECTS; j++) {
    
            x = ((double) rand() / (double) RAND_MAX);  // randomly select values;
            y = ((double) rand() / (double) RAND_MAX);  // randomly select values;
            z = ((double) rand() / (double) RAND_MAX);  // randomly select values;

            if (*verbose) printf("%2.1f %2.1f %2.1f ", x, y, z);

            dataVector(j*3 + 1) = x;
            dataVector(j*3 + 2) = y;
            dataVector(j*3 + 3) = z;
         }
 
         getObjectsPortOut->write(); 
         if (*verbose) printf("\n");
      }

      
      /* Primitive 14 */

      /* getObjects(centre_x, centre_y, centre_z, radius, <x, y, z>)                                                  */
      /* ------------------------------------------------------------------------------------------------------------ */
      /* This alternative form of getObjects() primitive returns a list (denoted by < >) of locations                 */
      /* where play objects are visible in the therapy environment (viewed from any of the therapy                    */
      /* cameras). Each location is specified by a triple of Cartesian coordinates x, y, and z. In this case,         */
      /* the search for the objects is restricted to a circular region given by the centre coordinates                */
      /* and radius parameters. The first item in the list is the number of triples that follow.                                                                                      */
      /*                                                                                                              */
      /* Input the coordinates specifying the centre of a circle and its radius from port /getObjects:i               */
      /* Output location of objects that are visible in this circle in the therapy environment to port /getObjects:o  */

      tempDoubleVector = getObjectsPortIn->read(false);                   // non-blocking version
      /* do {                              
            tempDoubleVector = getObjectsPortIn->read(false);             // blocking version 
      } while ((tempdDoubleVector == NULL) && (isStopping() != true)); */
      if (tempDoubleVector != NULL) {
         x   = (double) (*tempDoubleVector)[0];
         y   = (double) (*tempDoubleVector)[1];
         z   = (double) (*tempDoubleVector)[2];
         rad = (double) (*tempDoubleVector)[3];
         if (*verbose) printf("sensoryInterpretation:        getObjects2        < %2.1f %2.1f %2.1f %2.1f %2.1f %2.1f \n", x, y, z, rad);
      }
  
      lambda = event_rate[current_primitive++] * elapsed_time; 
      count = samplePoisson(lambda);  
      //if (*verbose) printf("sensoryInterpretation:        getObjects2 lambda %4.3f \n", lambda);

      for (i=0; i<count; i++) {

         VectorOf<double> &dataVector = getObjectsPortOut->prepare();
         dataVector.resize(3*NUMBER_OF_OBJECTS+1);
      
         dataVector(0) = NUMBER_OF_OBJECTS;

         if (*verbose) printf("sensoryInterpretation:        getObjects2            > ");

         for (j=0; j<NUMBER_OF_OBJECTS; j++) {
    
            x = ((double) rand() / (double) RAND_MAX);  // randomly select values;
            y = ((double) rand() / (double) RAND_MAX);  // randomly select values;
            z = ((double) rand() / (double) RAND_MAX);  // randomly select values;

            if (*verbose) printf("%2.1f %2.1f %2.1f ", x, y, z);

            dataVector(j*3 + 1) = x; 
            dataVector(j*3 + 2) = y;
            dataVector(j*3 + 3) = z;
         }

         getObjectsPortOut->write(); 
         if (*verbose) printf("\n");
      }

   
      /* Primitive 15 */

      /* getObjectTableDistance(object_x, object_y, object_z, vertical_distance)                        */
      /* ---------------------------------------------------------------------------------------------- */
      /* The getObjectTableDistance() primitive returns the vertical distance of an object given by the */
      /* Cartesian coordinates to the table on which play objects are places.                           */
      /*                                                                                                */
      /* Input location of an object from port /getObjectTableDistance:i                                */
      /* Output vertical distance between object and table to port /getObjectTableDistance:o            */

      tempDoubleVector = getObjectTableDistancePortIn->read(false);                   // non-blocking version
      /* do {                              
            tempDoubleVector = getObjectTableDistancePortIn->read(false);             // blocking version 
      } while ((tempdDoubleVector == NULL) && (isStopping() != true)); */
      if (tempDoubleVector != NULL) {
         x = (double) (*tempDoubleVector)[0];
         y = (double) (*tempDoubleVector)[1];
         z = (double) (*tempDoubleVector)[2];
         if (*verbose) printf("sensoryInterpretation:        getObjectTableDistance < %2.1f %2.1f %2.1f\n", x, y, z);
      }
  
      lambda = event_rate[current_primitive++] * elapsed_time; 
      count = samplePoisson(lambda);  
      //if (*verbose) printf("sensoryInterpretation:        getObjectTableDistance lambda %4.3f \n", lambda);

      for (i=0; i<count; i++) {
         
         VectorOf<double> &dataVector = getObjectTableDistancePortOut->prepare();
         dataVector.resize(1);     

         x = ((double) rand() / (double) RAND_MAX);  // randomly select values;

         dataVector(0) = x;

         getObjectTableDistancePortOut->write(); 
         if (*verbose) printf("sensoryInterpretation:        getObjectTableDistance > %2.1f\n",x);
      }

 
      /* Primitive 16 */
   
      /* getSoundDirection(threshold, azimuth, elevation)                                                           */
      /* ---------------------------------------------------------------------------------------------------------- */
      /* The getSoundDirection() primitive returns the horizontal (azimuth) and vertical (elevation) angles         */
      /* defining the direction to the loudest sound in the environment (e.g. when someone is speaking).            */
      /* The angles are defined with respect to the robot’s local frame of reference centred in its torso.          */
      /*                                                                                                            */
      /* Input sound threshold from from port /getSoundDirection:i                                                  */
      /* the azimuth) and elevation angles defining the direction to the loudest sound to port /getSoundDirection:o */

      tempDoubleVector = getSoundDirectionPortIn->read(false);                   // non-blocking version
      /* do {                              
            tempDoubleVector = getSoundDirectionPortIn->read(false);             // blocking version 
      } while ((tempdDoubleVector == NULL) && (isStopping() != true)); */
      if (tempDoubleVector != NULL) {
         x = (double) (*tempDoubleVector)[0];
         if (*verbose) printf("sensoryInterpretation:        getSoundDirection   < %2.1f\n", x);
      }
  
      lambda = event_rate[current_primitive++] * elapsed_time; 
      count = samplePoisson(lambda);  
      //if (*verbose) printf("sensoryInterpretation:        getSoundDirection lambda %4.3f \n", lambda);

      for (i=0; i<count; i++) {

         VectorOf<double> &dataVector = getSoundDirectionPortOut->prepare();
         dataVector.resize(2);

         a = ((double) rand() / (double) RAND_MAX);  // randomly select values;
         b = ((double) rand() / (double) RAND_MAX);  // randomly select values;

         dataVector(0) = a;
         dataVector(1) = b;

         getSoundDirectionPortOut->write(); 
         if (*verbose) printf("sensoryInterpretation:        getSoundDirection      > %2.1f %2.1f\n",a, b);
      }

       
      /* Primitive 17 */

      /* identifyFace(x, y, z, face_id)                                                                       */
      /* ---------------------------------------------------------------------------------------------------- */
      /* The identifyFace() primitive classifies the face at the specified location. The possible classes     */
      /* are determined during the set up phase. Typically, they will include the therapist and the           */
      /* children involved in the therapy sessions. The class identifier is returned in the face_id parameter */
      /* The maximum number of faces is given by NUMBER_OF_FACES                                              */
      /*                                                                                                      */
      /* Input location of a face from port /identifyFace:i                                                   */
      /* Output face identification number to port /identifyFace:o                                            */

      tempDoubleVector = identifyFacePortIn->read(false);                   // non-blocking version
      /* do {                              
            tempDoubleVector = identifyFacePortIn->read(false);             // blocking version 
      } while ((tempdDoubleVector == NULL) && (isStopping() != true)); */
      if (tempDoubleVector != NULL) {
         x = (double) (*tempDoubleVector)[0];
         y = (double) (*tempDoubleVector)[1];
         z = (double) (*tempDoubleVector)[2];
         if (*verbose) printf("sensoryInterpretation:        identifyFace           < %2.1f %2.1f %2.1f\n", x, y, z);
      }
  
      lambda = event_rate[current_primitive++] * elapsed_time; 
      count = samplePoisson(lambda);  
      //if (*verbose) printf("sensoryInterpretation:        identifyFace lambda %4.3f \n", lambda);

      for (i=0; i<count; i++) {
         
         VectorOf<int> &dataVector = identifyFacePortOut->prepare();
         dataVector.resize(1);     
        
         p = (int) (rand() % NUMBER_OF_FACES);  // randomly select identification number

         dataVector(0) = p;

         identifyFacePortOut->write();                   
         if (*verbose) printf("sensoryInterpretation:        identifyFace           > %d\n",p);
      }

  
      /* Primitive 18 */

      /* identifyFaceExpression(x, y, z, expression_id)                                                       */
      /* ---------------------------------------------------------------------------------------------------- */
      /* The identifyFaceExpression() primitive classifies the expression of the face at the specified        */
      /* location. The possible classes are determined during the set up phase. Typically, they will be       */
      /* happy, sad, angry, or fearful (i.e. the four classes of emotion used in the interventions defined in */
      /* Deliverable D1.1). The emotion identifier is returned in the expression_id parameter.                */
      /* The maximum number of expressions is given by NUMBER_OF_EXPRESSIONS                                  */
      /*                                                                                                      */
      /* Input location of a face from port /identifyFaceExpression:i                                         */
      /* Output expression identification number to port /identifyFaceExpression:o                            */

      tempDoubleVector = identifyFaceExpressionPortIn->read(false);                   // non-blocking version
      /* do {                              
            tempDoubleVector = identifyFaceExpressionPortIn->read(false);             // blocking version 
      } while ((tempdDoubleVector == NULL) && (isStopping() != true)); */
      if (tempDoubleVector != NULL) {
         x = (double) (*tempDoubleVector)[0];
         y = (double) (*tempDoubleVector)[1];
         z = (double) (*tempDoubleVector)[2];
         if (*verbose) printf("sensoryInterpretation:        identifyFaceExpression < %2.1f %2.1f %2.1f\n", x, y, z);
      }
  
      lambda = event_rate[current_primitive++] * elapsed_time; 
      count = samplePoisson(lambda);  
      //if (*verbose) printf("sensoryInterpretation:        identifyFaceExpression lambda %4.3f \n", lambda);

      for (i=0; i<count; i++) {
         
         VectorOf<int> &dataVector = identifyFaceExpressionPortOut->prepare();
         dataVector.resize(1);     
        
         p = (int) (rand() % NUMBER_OF_EXPRESSIONS);  // randomly select identification number

         dataVector(0) = p;

         identifyFaceExpressionPortOut->write();                   
         if (*verbose) printf("sensoryInterpretation:        identifyFaceExpression > %d\n",p);
      }

 
  
      /* Primitive 19 */

      /* identifyObject(x, y, z, object_id)                                                              */
      /* ----------------------------------------------------------------------------------------------- */
      /* The identifyObject() primitive classifies the object at the specified location. The possible    */
      /* classes are determined during the set up phase. Typically, they will correspond to a designated */
      /*  set of play objects, pictures to be placed on the table, or pictures on the sand-tray.         */
      /* The object identifier is returned in the object_id parameter.                                   */
      /*                                                                                                 */
      /* Input location of an from port /identifyObject:i                                                */
      /* Output object identification number to port /identifyObject:o                                   */

      tempDoubleVector = identifyObjectPortIn->read(false);                   // non-blocking version
      /* do {                              
            tempDoubleVector = identifyObjectPortIn->read(false);             // blocking version 
      } while ((tempdDoubleVector == NULL) && (isStopping() != true)); */
      if (tempDoubleVector != NULL) {
         x = (double) (*tempDoubleVector)[0];
         y = (double) (*tempDoubleVector)[1];
         z = (double) (*tempDoubleVector)[2];
         if (*verbose) printf("sensoryInterpretation:        identifyObject         < %2.1f %2.1f %2.1f\n", x, y, z);
      }
  
      lambda = event_rate[current_primitive++] * elapsed_time; 
      count = samplePoisson(lambda);  
      //if (*verbose) printf("sensoryInterpretation:        identifyObject lambda %4.3f \n", lambda);

      for (i=0; i<count; i++) {
         
         VectorOf<int> &dataVector = identifyObjectPortOut->prepare();
         dataVector.resize(1);     
        
         p = (int) (rand() % NUMBER_OF_OBJECTS);  // randomly select identification number

         dataVector(0) = p;

         identifyObjectPortOut->write();                   
         if (*verbose) printf("sensoryInterpretation:        identifyObject         > %d\n",p);
      }
    

      /* Primitive 20 */

      /* identifyTrajectory(<x, y, z, t>, trajectory_descriptor)                                                      */
      /* -------------------------------------------------------------------------------------------------------------------- */
      /* The identifyTrajectory() primitive classifies the trajectory defined by a list of 4-tuples, each 4-tuple defining    */
      /* a 3-D location and a time-stamp. The possible classes are determined during the set up phase. Typically, they will   */
      /* correspond to a designated set of hand gestures to be made by the child (e.g. a wave). The first item in the list    */
      /* is the number of 4-tuples that follow.                                                                               */
      /* The trajectory identifier is returned in the trajectory_descriptor parameter.                                        */
      /*                                                                                                                      */
      /* Input a trajectory defined by a sequence of 4-tuples (3-D location and a time-stamp) from port /identifyTrajectory:i */
      /* Output location of the location a point on that surface at which the child’s head is directed to port /getHeadGaze:o */

      tempDoubleVector = identifyTrajectoryPortIn->read(false);                   // non-blocking version
      /* do {                              
            tempDoubleVector = identifyTrajectoryPortIn->read(false);             // blocking version 
      } while ((tempdDoubleVector == NULL) && (isStopping() != true)); */
      if (tempDoubleVector != NULL) {
         x1 = (double) (*tempDoubleVector)[1];  // just print the first 4-tuple
         y1 = (double) (*tempDoubleVector)[2];
         z1 = (double) (*tempDoubleVector)[3];
         t1 = (double) (*tempDoubleVector)[4];
         if (*verbose) printf("sensoryInterpretation:        identifyTrajectory       < %2.1f %2.1f %2.1f %2.1f  \n", x1, y1, z1, t1);
      }
  
      lambda = event_rate[current_primitive++] * elapsed_time; 
      count = samplePoisson(lambda);  
      //if (*verbose) printf("sensoryInterpretation:        identifyTrajectory lambda %4.3f \n", lambda);

      for (i=0; i<count; i++) {
         
         VectorOf<int> &dataVector = identifyTrajectoryPortOut->prepare();
         dataVector.resize(1);
        
         p = (int) (rand() % NUMBER_OF_TRAJECTORIES);  // randomly select identification number

         dataVector(0) = p;

         identifyTrajectoryPortOut->write(); 
         if (*verbose) printf("sensoryInterpretation:        identifyTrajectory     > %d\n",p);
      }
       


      /* Primitive 21 */

      /* identifyVoice(voice_descriptor)                                                                       */
      /* ----------------------------------------------------------------------------------------------------- */
      /* The identifyVoice() primitive classifies the voice that is currently speaking. The possible classes   */
      /* are determined during the set up phase. Typically, they will include the therapist and the children   */
      /*  involved in the therapy sessions. The voice identifier is returned in the voice_descriptor parameter.*/
      /*                                                                                                       */
      /* Output voice identification number to port /identifyVoice:o                                           */
  
      lambda = event_rate[current_primitive++] * elapsed_time; 
      count = samplePoisson(lambda);  
      //if (*verbose) printf("sensoryInterpretation:        identifyVoice lambda %4.3f \n", lambda);

      for (i=0; i<count; i++) {
         
         VectorOf<int> &dataVector = identifyVoicePortOut->prepare();
         dataVector.resize(1);     
        
         p = rand() % NUMBER_OF_VOICES;  // randomly select identification number

         dataVector(0) = p;

         identifyVoicePortOut->write();                   
         if (*verbose) printf("sensoryInterpretation:        identifyVoice          > %d\n",p);
      }
    
 

      /* Primitive 22 */

      /* recognizeSpeech(text)                                                                                                              */
      /* ---------------------------------------------------------------------------------------------------------------------------------- */
      /* The recognizeSpeech() primitive returns a textual representation of anything currently being spoken by the therapist or the child. */
      /*                                                                                                                                    */
      /* Output a textual representation of anything currently being spoken by the therapist or the child to port /recognizeSpeech:o        */
  
      lambda = event_rate[current_primitive++] * elapsed_time; 
      count = samplePoisson(lambda);  
      //if (*verbose) printf("sensoryInterpretation:        recognizeSpeech lambda %4.3f \n", lambda);

      for (i=0; i<count; i++) {
         
         Bottle &message = recognizeSpeechPortOut->prepare();

         message.clear();
         message.addString("Example text");
         
         if (*verbose) printf("sensoryInterpretation:        recognizeSpeech        > %s\n",message.get(0).asString());
         
         recognizeSpeechPortOut->write();                  
      }
    

  
      /* Primitive 23 */

      /* trackFace(seed_x, seed_y, seed_z, time_interval, projected_x, projected_y, projected_z)                 */
      /* ------------------------------------------------------------------------------------------------------- */
      /* The trackFace() primitive tracks the face that is currently located at the position given by the        */
      /* Cartesian seed coordinates and returns the projected location in the next time interval. The            */
      /* duration of the time interval is specified in milliseconds. By reassigning the projected location       */
      /* to the seed and repeatedly invoking trackFace() the locations of the face can be tracked and  recorded. */
      /*                                                                                                         */
      /* Input seed coordinates of a face to be tracked and time interval  milliseconds from port /trackFace:i   */
      /* Output the projected location in the next time interval to port /trackFace:o                            */

      tempDoubleVector = trackFacePortIn->read(false);                   // non-blocking version
      /* do {                              
            tempDoubleVector = trackFacePortIn->read(false);             // blocking version 
      } while ((tempdDoubleVector == NULL) && (isStopping() != true)); */
      if (tempDoubleVector != NULL) {
         x1 = (double) (*tempDoubleVector)[0];
         y1 = (double) (*tempDoubleVector)[1];
         z1 = (double) (*tempDoubleVector)[2];
         t1 = (double) (*tempDoubleVector)[3];
         if (*verbose) printf("sensoryInterpretation:        trackFace              < %2.1f %2.1f %2.1f %2.1f\n", x1, y1, z1, t1);
      }
  
      lambda = event_rate[current_primitive++] * elapsed_time; 
      count = samplePoisson(lambda);  
      //if (*verbose) printf("sensoryInterpretation:        trackFace lambda %4.3f \n", lambda);

      for (i=0; i<count; i++) {
         
         VectorOf<double> &dataVector = trackFacePortOut->prepare();
         dataVector.resize(3);     

         x = ((double) rand() / (double) RAND_MAX);  // randomly select values;
         y = ((double) rand() / (double) RAND_MAX);  // randomly select values;
         z = ((double) rand() / (double) RAND_MAX);  // randomly select values;

         dataVector(0) = x;
         dataVector(1) = y;
         dataVector(2) = z;

         trackFacePortOut->write();                   
         if (*verbose) printf("sensoryInterpretation:        trackFace              > %2.1f %2.1f %2.1f\n",x, y, z);
      }
    

      
      /* Primitive 24 */

      /* trackHand(seed_x, seed_y, seed_z, time_interval, projected_x, projected_y, projected_z)               */
      /* ----------------------------------------------------------------------------------------------------- */
      /* The trackHand() primitive tracks the hand that is currently located at the position given by the      */
      /* Cartesian seed coordinates and returns the projected location in the next time interval. The          */
      /* duration of the time interval is specified in milliseconds. By reassigning the projected location     */
      /* to the seed and repeatedly invoking trackHand() the locations of the hand can be tracked and recorded */
      /*                                                                                                       */
      /* Input seed coordinates of a hand to be tracked and time interval  milliseconds from port /trackHand:i */
      /* Output the projected location in the next time interval to port /trackHand:o                          */


      tempDoubleVector = trackHandPortIn->read(false);                   // non-blocking version
      /* do {                              
            tempDoubleVector = trackHandPortIn->read(false);             // blocking version 
      } while ((tempdDoubleVector == NULL) && (isStopping() != true)); */
      if (tempDoubleVector != NULL) {
         x1 = (double) (*tempDoubleVector)[0];
         y1 = (double) (*tempDoubleVector)[1];
         z1 = (double) (*tempDoubleVector)[2];
         t1 = (double) (*tempDoubleVector)[3];
         if (*verbose) printf("sensoryInterpretation:        trackHand              < %2.1f %2.1f %2.1f %2.1f\n", x1, y1, z1, t1);
      }
  
      lambda = event_rate[current_primitive++] * elapsed_time; 
      count = samplePoisson(lambda);  
      //if (*verbose) printf("sensoryInterpretation:        trackHand lambda %4.3f \n", lambda);

      for (i=0; i<count; i++) {
         
         VectorOf<double> &dataVector = trackHandPortOut->prepare();
         dataVector.resize(3);     

         x = ((double) rand() / (double) RAND_MAX);  // randomly select values;
         y = ((double) rand() / (double) RAND_MAX);  // randomly select values;
         z = ((double) rand() / (double) RAND_MAX);  // randomly select values;

         dataVector(0) = x;
         dataVector(1) = y;
         dataVector(2) = z;

         trackHandPortOut->write();                   
         if (*verbose) printf("sensoryInterpretation:        trackHand              > %2.1f %2.1f %2.1f\n",x, y, z);
      }
    

      
      /* Primitive 25 */

      /* trackObject(seed_x, seed_y, seed_z, time_interval, projected_x, projected_y, projected_z)               */
      /* ------------------------------------------------------------------------------------------------------- */
      /* The trackObject() primitive tracks the object given by the object descriptor and currently              */
      /* located at the position given by the Cartesian seed coordinates and returns the projected location      */
      /* in the next time interval. The duration of the time interval is specified in milliseconds. By           */
      /* reassigning the projected location to the seed and repeatedly invoking trackObject() the                */
      /* locations of the object can be tracked and recorded.                                                    */
      /*                                                                                                         */
      /* Input seed coordinates of object to be tracked and time interval  milliseconds from port /trackObject:i */
      /* Output the projected location in the next time interval to port /trackObject:o                          */

      tempDoubleVector = trackObjectPortIn->read(false);                   // non-blocking version
      /* do {                              
            tempDoubleVector = trackObjectPortIn->read(false);             // blocking version 
      } while ((tempdDoubleVector == NULL) && (isStopping() != true)); */
      if (tempDoubleVector != NULL) {
         x1 = (double) (*tempDoubleVector)[0];
         y1 = (double) (*tempDoubleVector)[1];
         z1 = (double) (*tempDoubleVector)[2];
         t1 = (double) (*tempDoubleVector)[3];
         if (*verbose) printf("sensoryInterpretation:        trackObject            < %2.1f %2.1f %2.1f %2.1f\n", x1, y1, z1, t1);
      }
  
      lambda = event_rate[current_primitive++] * elapsed_time; 
      count = samplePoisson(lambda);  
      //if (*verbose) printf("sensoryInterpretation:        trackObject lambda %4.3f \n", lambda);

      for (i=0; i<count; i++) {
         
         VectorOf<double> &dataVector = trackObjectPortOut->prepare();
         dataVector.resize(3);     

         x = ((double) rand() / (double) RAND_MAX);  // randomly select values;
         y = ((double) rand() / (double) RAND_MAX);  // randomly select values;
         z = ((double) rand() / (double) RAND_MAX);  // randomly select values;

         dataVector(0) = x;
         dataVector(1) = y;
         dataVector(2) = z;

         trackObjectPortOut->write();                   
         if (*verbose) printf("sensoryInterpretation:        trackObject            > %2.1f %2.1f %2.1f\n",x, y, z);
      }
    

      /* last perception primitive processed */

      current_primitive = 0; //   reset the primitive counter for the next iteration
   }
}


void SensoryInterpretationThread::threadRelease() 
{
   /* for example, delete dynamically created data-structures */
}


/* Method to sample a Poisson distribution */

int SensoryInterpretationThread::samplePoisson(double lambda) {

   /* Generate a random sample from a Poisson distribution  */
   /* with a given mean, lambda                             */
   /* Use the function rand to generate a random number     */

   static bool first_call = true;

   int count;
   double product;
   double zero_probability;

   /* Seed the random-number generator with current time so */
   /* that the numbers will be different every time we run  */

   if (first_call) {
      srand( (unsigned)time( NULL ));
      first_call = false;
   }

   count = 0; 
   product = (double) rand() / (double) RAND_MAX;

   zero_probability = exp(-lambda);

   while (product > zero_probability) {
      count++;
      product = product * ((double) rand() / (double) RAND_MAX);
   }

   //printf("lambda=%f; zero_probability=%f; count=%d\n", lambda, zero_probability, count);

   return(count);
}

