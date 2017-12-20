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
 * 28/11/14  First version completed (David Vernon)
 *
 */ 
 

#include "childBehaviourClassification.h"

ChildBehaviourClassificationThread::ChildBehaviourClassificationThread(
                                                         bool                            *verboseValue,          // simulation control parameters
                                                         bool                            *cycleValue,
                                                         double                          *cyclePeriodValue,
                                                         double                          *eventRateValue,                                                      
                                                         BufferedPort<VectorOf<int>>     *checkMutualGazeIn,    // functional parameters 
                                                         BufferedPort<VectorOf<double>>  *getArmAngleIn,
                                                         BufferedPort<VectorOf<double>>  *getBodyIn,
                                                         BufferedPort<VectorOf<double>>  *getBodyPoseIn, 
                                                         BufferedPort<VectorOf<double>>  *getEyeGazeIn, 
                                                         BufferedPort<VectorOf<double>>  *getEyesIn,
                                                         BufferedPort<VectorOf<double>>  *getFacesIn, 
                                                         BufferedPort<VectorOf<double>>  *getGripLocationIn, 
                                                         BufferedPort<VectorOf<double>>  *getHandsIn,
                                                         BufferedPort<VectorOf<double>>  *getHeadIn,
                                                         BufferedPort<VectorOf<double>>  *getHeadGazeIn, 
                                                         BufferedPort<VectorOf<double>>  *getObjectsIn, 
                                                         BufferedPort<VectorOf<double>>  *getObjectTableDistanceIn, 
                                                         BufferedPort<VectorOf<double>>  *getSoundDirectionIn, 
                                                         BufferedPort<VectorOf<int>>     *identifyFaceIn,   
                                                         BufferedPort<VectorOf<int>>     *identifyFaceExpressionIn,  
                                                         BufferedPort<VectorOf<int>>     *identifyObjectIn,   
                                                         BufferedPort<VectorOf<int>>     *identifyTrajectoryIn,  
                                                         BufferedPort<VectorOf<int>>     *identifyVoiceIn,
                                                         BufferedPort<Bottle>            *recognizeSpeechIn, 
                                                         BufferedPort<VectorOf<double>>  *trackFaceIn, 
                                                         BufferedPort<VectorOf<double>>  *trackHandIn, 
                                                         BufferedPort<VectorOf<double>>  *trackObjectIn,  
                                                         BufferedPort<VectorOf<double>>  *getChildBehaviourOut,
                                                         BufferedPort<VectorOf<double>>  *getChildMotivationOut,
                                                         BufferedPort<VectorOf<double>>  *getChildPerformanceOut
                                                        )
{
   verbose                      = verboseValue;    
   cycle                        = cycleValue;
   cyclePeriod                  = cyclePeriodValue;
   eventRate                    = eventRateValue;
   checkMutualGazePortIn        = checkMutualGazeIn;
   getArmAnglePortIn            = getArmAngleIn;
   getBodyPortIn                = getBodyIn;
   getBodyPosePortIn            = getBodyPoseIn;
   getEyeGazePortIn             = getEyeGazeIn;
   getEyesPortIn                = getEyesIn;  
   getFacesPortIn               = getFacesIn;
   getGripLocationPortIn        = getGripLocationIn; 
   getHandsPortIn               = getHandsIn;
   getHeadPortIn                = getHeadIn;
   getHeadGazePortIn            = getHeadGazeIn; 
   getObjectsPortIn             = getObjectsIn; 
   getObjectTableDistancePortIn = getObjectTableDistanceIn;  
   getSoundDirectionPortIn      = getSoundDirectionIn; 
   identifyFacePortIn           = identifyFaceIn;
   identifyFaceExpressionPortIn = identifyFaceExpressionIn;     
   identifyObjectPortIn         = identifyObjectIn;
   identifyTrajectoryPortIn     = identifyTrajectoryIn;  
   identifyVoicePortIn          = identifyVoiceIn;
   recognizeSpeechPortIn        = recognizeSpeechIn; 
   trackFacePortIn              = trackFaceIn; 
   trackHandPortIn              = trackHandIn; 
   trackObjectPortIn            = trackObjectIn;  
   getChildBehaviourPortOut     = getChildBehaviourOut;  
   getChildMotivationPortOut    = getChildMotivationOut;  
   getChildPerformancePortOut   = getChildPerformanceOut;  
}

bool ChildBehaviourClassificationThread::threadInit() 
{
   /* initialize variables and create data-structures if needed */

   debug             = true;     // print debug statements

   return true;
}

void ChildBehaviourClassificationThread::run(){

   int               i, j;
   int               p;
   double            a, b, c;
   double            x, y, z;

   VectorOf<double>  *tempDoubleVector;
   VectorOf<int>     *tempIntVector;
   Bottle            *tempBottle;  
   char modifiedString[STRINGLENGTH]; 

   target_primitive = 0;
   current_primitive = 0;  

   start_time      = yarp::os::Time::now(); // start time
   cumulative_time = 0.0;
   elapsed_time    = 0.0;

   while (isStopping() != true) { // the thread continues to run until isStopping() returns true
 
      //if (*verbose) printf("childBehaviourClassification\n");
  
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

         //printf("childBehaviourClassification: cycle period %f\n", *cyclePeriod);
         //printf("childBehaviourClassification: event rate   %f\n", *eventRate);

      }

      /* -----------------------------------------------------------------------------*/
      /* Read all the information being sent from the sensoryInterpretation component */
      /* -----------------------------------------------------------------------------*/


      /* Primitive 1: checkMutualGaze() */

      tempIntVector = checkMutualGazePortIn->read(false);                   // non-blocking version
      /* do {                              
            tempIntVector = checkMutualGazePortIn->read(false);             // blocking version 
      } while ((tempdIntVector == NULL) && (isStopping() != true)); */
      if (tempIntVector != NULL) {
         p = (int) (*tempIntVector)[0];
         if (*verbose) printf("childBehaviourClassification: checkMutualGaze        < %d\n", p);
      }


      /* Primitive 2: getArmAngle(left_azimuth, left_elevation, right_azimuth, right_elevation) */

      tempDoubleVector = getArmAnglePortIn->read(false);                   // non-blocking version
      /* do {                              
            tempDoubleVector = getArmAnglePortIn->read(false);             // blocking version 
      } while ((tempDoubleVector == NULL) && (isStopping() != true)); */
      if (tempDoubleVector != NULL) {
         a = (double)  (*tempDoubleVector)[0];
         b = (double)  (*tempDoubleVector)[1];
         x = (double)  (*tempDoubleVector)[2];
         y = (double)  (*tempDoubleVector)[3];
         if (*verbose) printf("childBehaviourClassification: getArmAngle            < %2.1f %2.1f %2.1f %2.1f\n", a, b, x, y);
      }
      

      /* Primitive 3: getBody(x, y, z) */

      tempDoubleVector = getBodyPortIn->read(false);                   // non-blocking version
      /* do {                              
            tempDoubleVector = getBodyPortIn->read(false);             // blocking version 
      } while ((tempDoubleVector == NULL) && (isStopping() != true)); */
      if (tempDoubleVector != NULL) {
         x = (double)  (*tempDoubleVector)[0];
         y = (double)  (*tempDoubleVector)[1];
         z = (double)  (*tempDoubleVector)[2];
         if (*verbose) printf("childBehaviourClassification: getBody                < %2.1f %2.1f %2.1f\n", x, y, z);
      }
 

      /* Primitive 4: getBodyPose(<joint_i>) */

      tempDoubleVector = getBodyPosePortIn->read(false);                   // non-blocking version
      /* do {                              
            tempDoubleVector = getBodyPosePortIn->read(false);             // blocking version 
      } while ((tempDoubleVector == NULL) && (isStopping() != true)); */
      if (tempDoubleVector != NULL) {
         p = (int)     (*tempDoubleVector)[0];
         if (*verbose) printf("childBehaviourClassification: getBodyPose            < ");

         for (j=0; j<p; j++) {
            x = (*tempDoubleVector)[j+1];
            if (*verbose) printf("%2.1f ", x);
         }
         if (*verbose) printf("\n");
      }
   

      /* Primitive 5: getEyeGaze(eye, x, y, z) */

      tempDoubleVector = getEyeGazePortIn->read(false);                   // non-blocking version
      /* do {                              
            tempDoubleVector = getEyeGazePortIn->read(false);             // blocking version 
      } while ((tempDoubleVector == NULL) && (isStopping() != true)); */
      if (tempDoubleVector != NULL) {
         x = (double)  (*tempDoubleVector)[0];
         y = (double)  (*tempDoubleVector)[1];
         z = (double)  (*tempDoubleVector)[2];
         if (*verbose) printf("childBehaviourClassification: getEyeGaze             < %2.1f %2.1f %2.1f\n",x,y,z);
      }


      /* Primitive 6: getEyes(eyeL_x, eyeL_y, eyeL_z, eyeR_x, eyeR_y, eyeR_z) */

      tempDoubleVector = getEyesPortIn->read(false);                   // non-blocking version
      /* do {                              
            tempDoubleVector = getEyesPortIn->read(false);             // blocking version 
      } while ((tempDoubleVector == NULL) && (isStopping() != true)); */
      if (tempDoubleVector != NULL) {
         a = (double)  (*tempDoubleVector)[0];
         b = (double)  (*tempDoubleVector)[1];
         c = (double)  (*tempDoubleVector)[2];
         x = (double)  (*tempDoubleVector)[3];
         y = (double)  (*tempDoubleVector)[4];
         z = (double)  (*tempDoubleVector)[5];
         if (*verbose) printf("childBehaviourClassification: getEyes                < %2.1f %2.1f %2.1f %2.1f %2.1f %2.1f\n", a, b, c, x, y, z);
      }


      /* Primitive 7: getFaces(<x, y, z>) */

      tempDoubleVector = getFacesPortIn->read(false);                   // non-blocking version
      /* do {                              
            tempDoubleVector = getFacesPortIn->read(false);             // blocking version 
      } while ((tempDoubleVector == NULL) && (isStopping() != true)); */
      if (tempDoubleVector != NULL) {
         p = (int)     (*tempDoubleVector)[0];
         if (*verbose) printf("childBehaviourClassification: getFaces               < ");

         for (j=0; j<p; j++) {
            x = (*tempDoubleVector)[j*3+1];
            y = (*tempDoubleVector)[j*3+2];
            z = (*tempDoubleVector)[j*3+3];
            if (*verbose) printf("%2.1f %2.1f %2.1f ", x, y, z);
         }
         if (*verbose) printf("\n");
      }


      /* Primitive 8: getGripLocation(object_x, object_y, object_z, grip_x, grip_y, grip_z)  */

      tempDoubleVector = getGripLocationPortIn->read(false);                   // non-blocking version
      /* do {                              
            tempDoubleVector = getGripLocationPortIn->read(false);             // blocking version 
      } while ((tempDoubleVector == NULL) && (isStopping() != true)); */
      if (tempDoubleVector != NULL) {
         x = (double)  (*tempDoubleVector)[0];
         y = (double)  (*tempDoubleVector)[1];
         z = (double)  (*tempDoubleVector)[2];
         if (*verbose) printf("childBehaviourClassification: getGripLocation        < %2.1f %2.1f %2.1f\n",x,y,z);
      }
 

      /* Primitive 9: getHands(<x, y, z>) */

      tempDoubleVector = getHandsPortIn->read(false);                   // non-blocking version
      /* do {                              
            tempDoubleVector = getHandsPortIn->read(false);             // blocking version 
      } while ((tempDoubleVector == NULL) && (isStopping() != true)); */
      if (tempDoubleVector != NULL) {
         p = (int)     (*tempDoubleVector)[0];
         if (*verbose) printf("childBehaviourClassification: getHands               < ");

         for (j=0; j<p; j++) {
            x = (*tempDoubleVector)[j*3+1];
            y = (*tempDoubleVector)[j*3+2];
            z = (*tempDoubleVector)[j*3+3];
            if (*verbose) printf("%2.1f %2.1f %2.1f ", x, y, z);
         }
         if (*verbose) printf("\n");
      }

         
       /* Primitive 10: getHead(x, y, z) */

      tempDoubleVector = getHeadPortIn->read(false);                   // non-blocking version
      /* do {                              
            tempDoubleVector = getHeadPortIn->read(false);             // blocking version 
      } while ((tempDoubleVector == NULL) && (isStopping() != true)); */
      if (tempDoubleVector != NULL) {
         x = (double)  (*tempDoubleVector)[0];
         y = (double)  (*tempDoubleVector)[1];
         z = (double)  (*tempDoubleVector)[2];
         if (*verbose) printf("childBehaviourClassification: getHead                < %2.1f %2.1f %2.1f\n",x,y,z);
      }

 
      /* Primitives 11 and 12: getHeadGaze(x, y, z) and getHeadGaze(p_x1, p_y1, p_z1, p_x2, p_y2, p_z2, p_x3, p_y3, p_z3, x, y, z) */

      tempDoubleVector = getHeadGazePortIn->read(false);                   // non-blocking version
      /* do {                              
            tempDoubleVector = getHeadGazePortIn->read(false);             // blocking version 
      } while ((tempDoubleVector == NULL) && (isStopping() != true)); */
      if (tempDoubleVector != NULL) {
         x = (double)  (*tempDoubleVector)[0];
         y = (double)  (*tempDoubleVector)[1];
         z = (double)  (*tempDoubleVector)[2];
         if (*verbose) printf("childBehaviourClassification: getHeadGaze            < %2.1f %2.1f %2.1f\n",x,y,z);
      }
   

      /* Primitives 13 and 14: getObjects(<x, y, z>) and getObjects(centre_x, centre_y, centre_z, radius, <x, y, z>) */

      tempDoubleVector = getObjectsPortIn->read(false);                   // non-blocking version
      /* do {                              
            tempDoubleVector = getObjectsPortIn->read(false);             // blocking version 
      } while ((tempDoubleVector == NULL) && (isStopping() != true)); */
      if (tempDoubleVector != NULL) {
         p = (int)     (*tempDoubleVector)[0];
         if (*verbose) printf("childBehaviourClassification: getObjects             < ");

         for (j=0; j<p; j++) {
            x = (*tempDoubleVector)[j*3+1];
            y = (*tempDoubleVector)[j*3+2];
            z = (*tempDoubleVector)[j*3+3];
            if (*verbose) printf("%2.1f %2.1f %2.1f ", x, y, z);
         }
         if (*verbose) printf("\n");
      }
       
  
      /* Primitive 15: getObjectTableDistance(object_x, object_y, object_z, vertical_distance) */

      tempDoubleVector = getObjectTableDistancePortIn->read(false);                   // non-blocking version
      /* do {                              
            tempDoubleVector = getObjectTableDistancePortIn->read(false);             // blocking version 
      } while ((tempDoubleVector == NULL) && (isStopping() != true)); */
      if (tempDoubleVector != NULL) {
         x = (double)  (*tempDoubleVector)[0];
         if (*verbose) printf("childBehaviourClassification: getObjectTableDistance < %2.1f\n",x);
      }


      /* Primitive 16: getSoundDirection(threshold, azimuth, elevation) */

      tempDoubleVector = getSoundDirectionPortIn->read(false);                   // non-blocking version
      /* do {                              
            tempDoubleVector = getSoundDirectionPortIn->read(false);             // blocking version 
      } while ((tempDoubleVector == NULL) && (isStopping() != true)); */
      if (tempDoubleVector != NULL) {
         x = (double)  (*tempDoubleVector)[0];
         y = (double)  (*tempDoubleVector)[1];
         if (*verbose) printf("childBehaviourClassification: getSoundDirection      < %2.1f %2.1f\n",x,y);
      }
 

      /* Primitive 17: identifyFace(x, y, z, face_id) */

      tempIntVector = identifyFacePortIn->read(false);                   // non-blocking version
      /* do {                              
            tempIntVector = identifyFacePortIn->read(false);             // blocking version 
      } while ((tempdIntVector == NULL) && (isStopping() != true)); */
      if (tempIntVector != NULL) {
         p = (int) (*tempIntVector)[0];
         if (*verbose) printf("childBehaviourClassification: identifyFace           < %d\n", p);
      }
 
      
      /* Primitive 18: identifyFaceExpression(x, y, z, expression_id)  */

      tempIntVector = identifyFaceExpressionPortIn->read(false);                   // non-blocking version
      /* do {                              
            tempIntVector = identifyFaceExpressionPortIn->read(false);             // blocking version 
      } while ((tempdIntVector == NULL) && (isStopping() != true)); */
      if (tempIntVector != NULL) {
         p = (int) (*tempIntVector)[0];
         if (*verbose) printf("childBehaviourClassification: identifyFaceExpression < %d\n", p);
      }
 
      
      /* Primitive 19: identifyObject(x, y, z, object_id)  */

      tempIntVector = identifyObjectPortIn->read(false);                   // non-blocking version
      /* do {                              
            tempIntVector = identifyObjectPortIn->read(false);             // blocking version 
      } while ((tempdIntVector == NULL) && (isStopping() != true)); */
      if (tempIntVector != NULL) {
         p = (int) (*tempIntVector)[0];
         if (*verbose) printf("childBehaviourClassification: identifyObject         < %d\n", p);
      }
 
         
      /* Primitive 20: identifyTrajectory(<x, y, z, t>, trajectory_descriptor)   */

      tempIntVector = identifyTrajectoryPortIn->read(false);                   // non-blocking version
      /* do {                              
            tempIntVector = identifyTrajectoryPortIn->read(false);             // blocking version 
      } while ((tempdIntVector == NULL) && (isStopping() != true)); */
      if (tempIntVector != NULL) {
         p = (int) (*tempIntVector)[0];
         if (*verbose) printf("childBehaviourClassification: identifyTrajectory     < %d\n", p);
      }

   
      /* Primitive 21: identifyVoice(voice_descriptor) */

      tempIntVector = identifyVoicePortIn->read(false);                   // non-blocking version
      /* do {                              
            tempIntVector = identifyVoicePortIn->read(false);             // blocking version 
      } while ((tempdIntVector == NULL) && (isStopping() != true)); */
      if (tempIntVector != NULL) {
         p = (int) (*tempIntVector)[0];
         if (*verbose) printf("childBehaviourClassification: identifyVoice          < %d\n", p);
      }


      /* Primitive 22: recognizeSpeech(text) */

      tempBottle = recognizeSpeechPortIn->read(false);                   // non-blocking version
      /* do {                              
            tempBottle = recognizeSpeechPortIn->read(false);             // blocking version 
      } while ((tempBottle == NULL) && (isStopping() != true)); */
      if (tempBottle != NULL) {
         
         /* for some strange reason, YARP puts double quotes around every individual string in a bottle so we strip them here */
      
         j=0;
         for (i=0; i<min(STRINGLENGTH-1,(int)strlen(tempBottle->toString().c_str()));i++) {
            if (tempBottle->toString().c_str()[i]!='\"') {
               modifiedString[j] = tempBottle->toString().c_str()[i];
               j++; 
            }
         }
         modifiedString[j]='\0';
         if (*verbose) printf("childBehaviourClassification: recognizeSpeech        < %s\n", modifiedString);
      }


      /* Primitive 23:  trackFace(seed_x, seed_y, seed_z, time_interval, projected_x, projected_y, projected_z) */

      tempDoubleVector = trackFacePortIn->read(false);                   // non-blocking version
      /* do {                              
            tempDoubleVector = trackFacePortIn->read(false);             // blocking version 
      } while ((tempDoubleVector == NULL) && (isStopping() != true)); */
      if (tempDoubleVector != NULL) {
         x = (double)  (*tempDoubleVector)[0];
         y = (double)  (*tempDoubleVector)[1];
         z = (double)  (*tempDoubleVector)[2];
         if (*verbose) printf("childBehaviourClassification: trackFace              < %2.1f %2.1f %2.1f\n",x,y,z);
      }

 
      /* Primitive 24: trackHand(seed_x, seed_y, seed_z, time_interval, projected_x, projected_y, projected_z)  */

      tempDoubleVector = trackHandPortIn->read(false);                   // non-blocking version
      /* do {                              
            tempDoubleVector = trackHandPortIn->read(false);             // blocking version 
      } while ((tempDoubleVector == NULL) && (isStopping() != true)); */
      if (tempDoubleVector != NULL) {
         x = (double)  (*tempDoubleVector)[0];
         y = (double)  (*tempDoubleVector)[1];
         z = (double)  (*tempDoubleVector)[2];
         if (*verbose) printf("childBehaviourClassification: trackHand              < %2.1f %2.1f %2.1f\n",x,y,z);
      }


      /* Primitive 25: trackObject(seed_x, seed_y, seed_z, time_interval, projected_x, projected_y, projected_z)  */

      tempDoubleVector = trackObjectPortIn->read(false);                   // non-blocking version
      /* do {                              
            tempDoubleVector = trackObjectPortIn->read(false);             // blocking version 
      } while ((tempDoubleVector == NULL) && (isStopping() != true)); */
      if (tempDoubleVector != NULL) {
         x = (double)  (*tempDoubleVector)[0];
         y = (double)  (*tempDoubleVector)[1];
         z = (double)  (*tempDoubleVector)[2];
         if (*verbose) printf("childBehaviourClassification: trackObject            < %2.1f %2.1f %2.1f\n",x,y,z);
      }
 


      /* -----------------------------------------------------------------------------*/
      /* Simulate the three childBehaviourClassification component primitives         */
      /* -----------------------------------------------------------------------------*/
 
      

      /* Primitive 1 */
      
      /* getChildBehaviour(<state, probability>)                                                                */
      /* ------------------------------------------------------------------------------------------------------ */
      /* The getChildBehaviour() primitive classifies the child’s behaviour on the basis of current percepts    */
      /* It produces a list of number pairs where the first element of each pair represents a child state       */
      /* and the second element the likelihood that the child is in that state. Thus, the primitive effectively */
      /* produces a discrete probability distribution across the space of child states.                         */
      /*                                                                                                        */
      /* Output list of (state, probability) tuples to port /getChildBehaviour:o                                */
      /* The first element of the list is the number of tuples in the list                                      */
 

      /* find out how many events have occurred and perform that many write operations on the port */

      lambda = event_rate[current_primitive++] * elapsed_time; // number of events = number of events per second x time in seconds
      //if (*verbose) printf("childBehaviourClassification: getChildBehaviour rate lambda %4.3f \n", lambda); 
      count = samplePoisson(lambda);     

      for (i=0; i<count; i++) {

         VectorOf<double> &dataVector = getChildBehaviourPortOut->prepare();
         dataVector.resize(2*NUMBER_OF_CHILD_BEHAVIOURAL_STATES+1);
      
         dataVector(0) = NUMBER_OF_CHILD_BEHAVIOURAL_STATES;

         if (*verbose) printf("childBehaviourClassification: getChildBehaviour      > \n");
         for (j=0; j<NUMBER_OF_CHILD_BEHAVIOURAL_STATES; j++) {
    
            x = ((double) rand() / (double) RAND_MAX);  // randomly select values;

            dataVector(j*2 + 1) = (double)j;
            dataVector(j*2 + 2) = x;

            if (*verbose) printf(" %d %2.1f", j, x);
         }
         if (*verbose) printf("\n");

         getChildBehaviourPortOut->write(); 
      }
  

      /* Primitive 2 */
      
      /* getChildMotivation(degree_of_engagement, confidence)                                                   */
      /* ------------------------------------------------------------------------------------------------------ */
      /* The getChildMotivation() primitive classifies the child’s motivation and engagement on the basis of    */
      /* the temporal sequence of child behaviour states, quantifying the extent the children are motivated     */
      /* to participate in the tasks with the robot and detect in particular when their attention is lost.      */
      /* It produces two numbers, the first representing an estimate of the degree of engagement and the        */
      /* second representing an indication of confidence in that estimate.                                      */
      /*                                                                                                        */
      /* Output motivation and confidence to port /getChildMotivation:o                                         */ 

      /* find out how many events have occurred and perform that many write operations on the port */

      
      lambda = event_rate[current_primitive++] * elapsed_time; // number of events = number of events per second x time in seconds
      //if (*verbose) printf("childBehaviourClassification: getChildMotivation rate lambda %4.3f \n", lambda); 
      count = samplePoisson(lambda);     

      for (i=0; i<count; i++) {

         VectorOf<double> &dataVector = getChildMotivationPortOut->prepare();
         dataVector.resize(2);
 
         x = ((double) rand() / (double) RAND_MAX);  // randomly select values;
         y = ((double) rand() / (double) RAND_MAX);  // randomly select values;

         dataVector(0) = x;
         dataVector(1) = y;

         getChildMotivationPortOut->write(); 
         if (*verbose) printf("childBehaviourClassification: getChildMotivation     > %2.1f %2.1f\n", x, y);
      }

      
 
      /* Primitive 3 */
      
      /* getChildPerformance(degree_of_performance, confidence)                                                 */
      /* ------------------------------------------------------------------------------------------------------ */
      /* The getChildPerformance() primitive determines the degree of performance of the child on the           */
      /* basis of a temporal sequence of child behaviour states, quantifying the performance of the children in */
      /* the therapeutic sessions. It produces two numbers, the first representing an estimate of the degree of */
      /* performance and the second representing an indication of confidence in that estimate.                  */
      /*                                                                                                        */
      /* Output performance and confidence to port /getChildPerformance:o                                       */
 

      /* find out how many events have occurred and perform that many write operations on the port */
      lambda = event_rate[current_primitive++] * elapsed_time; // number of events = number of events per second x time in seconds
      //if (*verbose) printf("childBehaviourClassification: getChildBehaviour rate lambda %4.3f \n", lambda); 
      count = samplePoisson(lambda);     

      for (i=0; i<count; i++) {

         VectorOf<double> &dataVector = getChildPerformancePortOut->prepare();
         dataVector.resize(2);
 
         x = ((double) rand() / (double) RAND_MAX);  // randomly select values;
         y = ((double) rand() / (double) RAND_MAX);  // randomly select values;

         dataVector(0) = x;
         dataVector(1) = y;

         getChildPerformancePortOut->write(); 
         if (*verbose) printf("childBehaviourClassification: getChildPerformance    > %2.1f %2.1f\n", x, y);
      }

      
      /* last perception primitive processed */

      current_primitive = 0; //   reset the primitive counter for the next iteration
   }
}


void ChildBehaviourClassificationThread::threadRelease() 
{
   /* for example, delete dynamically created data-structures */
}


/* Method to sample a Poisson distribution */

int ChildBehaviourClassificationThread::samplePoisson(double lambda) {

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

