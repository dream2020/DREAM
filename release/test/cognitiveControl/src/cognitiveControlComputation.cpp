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
 * 04/12/2014  First version completed (David Vernon)
 *
 */ 
 

#include "cognitiveControl.h"

CognitiveControlThread::CognitiveControlThread(
                                               bool                            *verboseValue,          // simulation control parameters
                                               bool                            *cycleValue,
                                               double                          *cyclePeriodValue,
                                               double                          *eventRateValue,    
                                               BufferedPort<VectorOf<int>>     *checkMutualGazeIn,    // functional parameters 
                                               BufferedPort<VectorOf<double>>  *getArmAngleIn,
                                               BufferedPort<VectorOf<double>>  *getBodyIn,
                                               BufferedPort<VectorOf<double>>  *getBodyPoseIn, 
                                               BufferedPort<VectorOf<double>>  *getEyeGazeIn, 
                                               BufferedPort<VectorOf<int>>     *getEyeGazeOut,
                                               BufferedPort<VectorOf<double>>  *getEyesIn,
                                               BufferedPort<VectorOf<double>>  *getFacesIn, 
                                               BufferedPort<VectorOf<double>>  *getGripLocationIn,   
                                               BufferedPort<VectorOf<double>>  *getGripLocationOut, 
                                               BufferedPort<VectorOf<double>>  *getHandsIn,
                                               BufferedPort<VectorOf<double>>  *getHeadIn,
                                               BufferedPort<VectorOf<double>>  *getHeadGazeIn, 
                                               BufferedPort<VectorOf<double>>  *getHeadGazeOut, 
                                               BufferedPort<VectorOf<double>>  *getObjectsIn,
                                               BufferedPort<VectorOf<double>>  *getObjectsOut, 
                                               BufferedPort<VectorOf<double>>  *getObjectTableDistanceIn, 
                                               BufferedPort<VectorOf<double>>  *getObjectTableDistanceOut, 
                                               BufferedPort<VectorOf<double>>  *getSoundDirectionIn,
                                               BufferedPort<VectorOf<double>>  *getSoundDirectionOut,  
                                               BufferedPort<VectorOf<int>>     *identifyFaceIn,
                                               BufferedPort<VectorOf<double>>  *identifyFaceOut,  
                                               BufferedPort<VectorOf<int>>     *identifyFaceExpressionIn,  
                                               BufferedPort<VectorOf<double>>  *identifyFaceExpressionOut,  
                                               BufferedPort<VectorOf<int>>     *identifyObjectIn,
                                               BufferedPort<VectorOf<double>>  *identifyObjectOut, 
                                               BufferedPort<VectorOf<int>>     *identifyTrajectoryIn,  
                                               BufferedPort<VectorOf<double>>  *identifyTrajectoryOut,  
                                               BufferedPort<VectorOf<int>>     *identifyVoiceIn,
                                               BufferedPort<Bottle>            *recognizeSpeechIn, 
                                               BufferedPort<VectorOf<double>>  *trackFaceIn,
                                               BufferedPort<VectorOf<double>>  *trackFaceOut, 
                                               BufferedPort<VectorOf<double>>  *trackHandIn,
                                               BufferedPort<VectorOf<double>>  *trackHandOut,  
                                               BufferedPort<VectorOf<double>>  *trackObjectIn,  
                                               BufferedPort<VectorOf<double>>  *trackObjectOut,
                                               BufferedPort<VectorOf<double>>  *getChildBehaviourIn,
                                               BufferedPort<VectorOf<double>>  *getChildMotivationIn,
                                               BufferedPort<VectorOf<double>>  *getChildPerformanceIn,
                                               BufferedPort<VectorOf<int>>     *gripIn,
                                               BufferedPort<VectorOf<double>>  *moveHandIn,
                                               BufferedPort<VectorOf<double>>  *moveHeadIn,
                                               BufferedPort<VectorOf<int>>     *moveSequenceIn,
                                               BufferedPort<VectorOf<double>>  *moveTorsoIn,
                                               BufferedPort<Bottle>            *sayIn,
                                               BufferedPort<VectorOf<int>>     *enableRobotIn,
                                               BufferedPort<VectorOf<int>>     *getInterventionStateOut
                                              )
{
   verbose                       = verboseValue;    
   cycle                         = cycleValue;
   cyclePeriod                   = cyclePeriodValue;
   eventRate                     = eventRateValue;
   checkMutualGazePortIn         = checkMutualGazeIn;
   getArmAnglePortIn             = getArmAngleIn;
   getBodyPortIn                 = getBodyIn;
   getBodyPosePortIn             = getBodyPoseIn;
   getEyeGazePortOut             = getEyeGazeOut;
   getEyeGazePortIn              = getEyeGazeIn;
   getEyesPortIn                 = getEyesIn;  
   getFacesPortIn                = getFacesIn;
   getGripLocationPortOut        = getGripLocationOut; 
   getGripLocationPortIn         = getGripLocationIn;
   getHandsPortIn                = getHandsIn;
   getHeadPortIn                 = getHeadIn;
   getHeadGazePortOut            = getHeadGazeOut; 
   getHeadGazePortIn             = getHeadGazeIn;  
   getObjectsPortOut             = getObjectsOut; 
   getObjectsPortIn              = getObjectsIn;
   getObjectTableDistancePortOut = getObjectTableDistanceOut;  
   getObjectTableDistancePortIn  = getObjectTableDistanceIn;
   getSoundDirectionPortOut      = getSoundDirectionOut; 
   getSoundDirectionPortIn       = getSoundDirectionIn; 
   identifyFacePortOut           = identifyFaceOut;
   identifyFacePortIn            = identifyFaceIn;  
   identifyFaceExpressionPortOut = identifyFaceExpressionOut;    
   identifyFaceExpressionPortIn  = identifyFaceExpressionIn;  
   identifyObjectPortOut         = identifyObjectOut;
   identifyObjectPortIn          = identifyObjectIn; 
   identifyTrajectoryPortOut     = identifyTrajectoryOut;  
   identifyTrajectoryPortIn      = identifyTrajectoryIn;  
   identifyVoicePortIn           = identifyVoiceIn;
   recognizeSpeechPortIn         = recognizeSpeechIn; 
   trackFacePortOut              = trackFaceOut;
   trackFacePortIn               = trackFaceIn;  
   trackHandPortOut              = trackHandOut;
   trackHandPortIn               = trackHandIn;  
   trackObjectPortOut            = trackObjectOut;  
   trackObjectPortIn             = trackObjectIn;  

   getChildBehaviourPortIn       = getChildBehaviourIn;
   getChildMotivationPortIn      = getChildMotivationIn;
   getChildPerformancePortIn     = getChildPerformanceIn;
  
   gripPortIn                    = gripIn;
   moveHandPortIn                = moveHandIn;
   moveHeadPortIn                = moveHeadIn;
   moveSequencePortIn            = moveSequenceIn;
   moveTorsoPortIn               = moveTorsoIn;
   sayPortIn                     = sayIn;
   enableRobotPortIn             = enableRobotIn;
   getInterventionStatePortOut   = getInterventionStateOut;
}

bool CognitiveControlThread::threadInit() 
{
   /* initialize variables and create data-structures if needed */

   
   /*** set simulation mode ***/

   debug             = true;     // print debug statements

   return true;
}

void CognitiveControlThread::run(){

   int               i, j;
   int               p, q, r;
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
 
      //if (*verbose) printf("cognitiveControlThread\n");
  
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

         if (*verbose) {
            //printf("cognitiveControlThread: cycle period %f\n", *cyclePeriod);
            //printf("cognitiveControlThread: event rate   %f\n", *eventRate);
         }

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
         if (*verbose) printf("cognitiveControl:             checkMutualGaze        < %d\n", p);
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
         if (*verbose) printf("cognitiveControl:             getArmAngle            < %2.1f %2.1f %2.1f %2.1f\n", a, b, x, y);
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
         if (*verbose) printf("cognitiveControl:             getBody                < %2.1f %2.1f %2.1f\n", x, y, z);
      }
 

      /* Primitive 4: getBodyPose(<joint_i>) */

      tempDoubleVector = getBodyPosePortIn->read(false);                   // non-blocking version
      /* do {                              
            tempDoubleVector = getBodyPosePortIn->read(false);             // blocking version 
      } while ((tempDoubleVector == NULL) && (isStopping() != true)); */
      if (tempDoubleVector != NULL) {
         p = (int)     (*tempDoubleVector)[0];
         if (*verbose) printf("cognitiveControl:             getBodyPose            < ");

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
         if (*verbose) printf("cognitiveControl:             getEyeGaze             < %2.1f %2.1f %2.1f\n",x,y,z);
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
         if (*verbose) printf("cognitiveControl:             getEyes                < %2.1f %2.1f %2.1f %2.1f %2.1f %2.1f\n", a, b, c, x, y, z);
      }


      /* Primitive 7: getFaces(<x, y, z>) */

      tempDoubleVector = getFacesPortIn->read(false);                   // non-blocking version
      /* do {                              
            tempDoubleVector = getFacesPortIn->read(false);             // blocking version 
      } while ((tempDoubleVector == NULL) && (isStopping() != true)); */
      if (tempDoubleVector != NULL) {
         p = (int)     (*tempDoubleVector)[0];
         if (*verbose) printf("cognitiveControl:             getFaces               < ");

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
         if (*verbose) printf("cognitiveControl:             getGripLocation        < %2.1f %2.1f %2.1f\n",x,y,z);
      }
 

      /* Primitive 9: getHands(<x, y, z>) */

      tempDoubleVector = getHandsPortIn->read(false);                   // non-blocking version
      /* do {                              
            tempDoubleVector = getHandsPortIn->read(false);             // blocking version 
      } while ((tempDoubleVector == NULL) && (isStopping() != true)); */
      if (tempDoubleVector != NULL) {
         p = (int)     (*tempDoubleVector)[0];
         if (*verbose) printf("cognitiveControl:             getHands               < ");

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
         if (*verbose) printf("cognitiveControl:             getHead                < %2.1f %2.1f %2.1f\n",x,y,z);
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
         if (*verbose) printf("cognitiveControl:             getHeadGaze            < %2.1f %2.1f %2.1f\n",x,y,z);
      }
   

      /* Primitives 13 and 14: getObjects(<x, y, z>) and getObjects(centre_x, centre_y, centre_z, radius, <x, y, z>) */

      tempDoubleVector = getObjectsPortIn->read(false);                   // non-blocking version
      /* do {                              
            tempDoubleVector = getObjectsPortIn->read(false);             // blocking version 
      } while ((tempDoubleVector == NULL) && (isStopping() != true)); */
      if (tempDoubleVector != NULL) {
         p = (int)     (*tempDoubleVector)[0];
         if (*verbose) printf("cognitiveControl:             getObjects             < ");

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
         if (*verbose) printf("cognitiveControl:             getObjectTableDistance < %2.1f\n",x);
      }


      /* Primitive 16: getSoundDirection(threshold, azimuth, elevation) */

      tempDoubleVector = getSoundDirectionPortIn->read(false);                   // non-blocking version
      /* do {                              
            tempDoubleVector = getSoundDirectionPortIn->read(false);             // blocking version 
      } while ((tempDoubleVector == NULL) && (isStopping() != true)); */
      if (tempDoubleVector != NULL) {
         x = (double)  (*tempDoubleVector)[0];
         y = (double)  (*tempDoubleVector)[1];
         if (*verbose) printf("cognitiveControl:             getSoundDirection      < %2.1f %2.1f\n",x,y);
      }
 

      /* Primitive 17: identifyFace(x, y, z, face_id) */

      tempIntVector = identifyFacePortIn->read(false);                   // non-blocking version
      /* do {                              
            tempIntVector = identifyFacePortIn->read(false);             // blocking version 
      } while ((tempdIntVector == NULL) && (isStopping() != true)); */
      if (tempIntVector != NULL) {
         p = (int) (*tempIntVector)[0];
         if (*verbose) printf("cognitiveControl:             identifyFace           < %d\n", p);
      }
 
      
      /* Primitive 18: identifyFaceExpression(x, y, z, expression_id)  */

      tempIntVector = identifyFaceExpressionPortIn->read(false);                   // non-blocking version
      /* do {                              
            tempIntVector = identifyFaceExpressionPortIn->read(false);             // blocking version 
      } while ((tempdIntVector == NULL) && (isStopping() != true)); */
      if (tempIntVector != NULL) {
         p = (int) (*tempIntVector)[0];
         if (*verbose) printf("cognitiveControl:             identifyFaceExpression < %d\n", p);
      }
 
      
      /* Primitive 19: identifyObject(x, y, z, object_id)  */

      tempIntVector = identifyObjectPortIn->read(false);                   // non-blocking version
      /* do {                              
            tempIntVector = identifyObjectPortIn->read(false);             // blocking version 
      } while ((tempdIntVector == NULL) && (isStopping() != true)); */
      if (tempIntVector != NULL) {
         p = (int) (*tempIntVector)[0];
         if (*verbose) printf("cognitiveControl:             identifyObject         < %d\n", p);
      }
 
         
      /* Primitive 20: identifyTrajectory(<x, y, z, t>, trajectory_descriptor)   */

      tempIntVector = identifyTrajectoryPortIn->read(false);                   // non-blocking version
      /* do {                              
            tempIntVector = identifyTrajectoryPortIn->read(false);             // blocking version 
      } while ((tempdIntVector == NULL) && (isStopping() != true)); */
      if (tempIntVector != NULL) {
         p = (int) (*tempIntVector)[0];
         if (*verbose) printf("cognitiveControl:             identifyTrajectory     < %d\n", p);
      }

   
      /* Primitive 21: identifyVoice(voice_descriptor) */

      tempIntVector = identifyVoicePortIn->read(false);                   // non-blocking version
      /* do {                              
            tempIntVector = identifyVoicePortIn->read(false);             // blocking version 
      } while ((tempdIntVector == NULL) && (isStopping() != true)); */
      if (tempIntVector != NULL) {
         p = (int) (*tempIntVector)[0];
         if (*verbose) printf("cognitiveControl:             identifyVoice          < %d\n", p);
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
         if (*verbose) printf("cognitiveControl:             recognizeSpeech        < %s\n", modifiedString);
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
         if (*verbose) printf("cognitiveControl:             trackFace              < %2.1f %2.1f %2.1f\n",x,y,z);
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
         if (*verbose) printf("cognitiveControl:             trackHand              < %2.1f %2.1f %2.1f\n",x,y,z);
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
         if (*verbose) printf("cognitiveControl:             trackObject            < %2.1f %2.1f %2.1f\n",x,y,z);
      }
 
 
      
      /* ------------------------------------------------------------------------------------*/
      /* Read all the information being sent from the childBehaviourClassification component */
      /* ------------------------------------------------------------------------------------*/

      /* Primitive 1: getChildBehaviour(<state, probability>)  */

      tempDoubleVector = getChildBehaviourPortIn->read(false);                   // non-blocking version
      /* do {                              
            tempDoubleVector = getChildBehaviourPortIn->read(false);             // blocking version 
      } while ((tempDoubleVector == NULL) && (isStopping() != true)); */
      if (tempDoubleVector != NULL) {
         p = (int)     (*tempDoubleVector)[0];
         if (*verbose) printf("cognitiveControl:             getChildBehaviour      < ");

         for (j=0; j<p; j++) {
            x = (*tempDoubleVector)[j*2+1];
            y = (*tempDoubleVector)[j*2+2];
            if (*verbose) printf("%2.1f %2.1f ", x, y);
         }
         if (*verbose) printf("\n");
      }

      
      /* Primitive 2: getChildMotivation(degree_of_engagement, confidence)  */

      tempDoubleVector = getChildMotivationPortIn->read(false);                   // non-blocking version
      /* do {                              
            tempDoubleVector = getChildMotivationPortIn->read(false);             // blocking version 
      } while ((tempDoubleVector == NULL) && (isStopping() != true)); */
      if (tempDoubleVector != NULL) {
         x = (double) (*tempDoubleVector)[0];
         y = (double) (*tempDoubleVector)[1];
         if (*verbose) printf("cognitiveControl:             getChildMotivation     < %2.1f %2.1f\n",x,y);
      }

            
      /* Primitive 3: getChildPerformance(degree_of_performance, confidence)  */

      tempDoubleVector = getChildPerformancePortIn->read(false);                   // non-blocking version
      /* do {                              
            tempDoubleVector = getChildPerformancenPortIn->read(false);             // blocking version 
      } while ((tempDoubleVector == NULL) && (isStopping() != true)); */
      if (tempDoubleVector != NULL) {
         x = (double) (*tempDoubleVector)[0];
         y = (double) (*tempDoubleVector)[1];
         if (*verbose) printf("cognitiveControl:             getChildPerformance    < %2.1f %2.1f\n",x,y);
      }


      /* ----------------------------------------------------------*/
      /* Simulate the eight cognitiveControl component primitives  */
      /* ----------------------------------------------------------*/


      /* Primitive 1 */

      /* grip(state)                                                                                       */
      /* ------------------------------------------------------------------------------------------------- */
      /* The grip() primitive causes the robot to grip an object by activating an electomagnet on the hand */
      /* This assumes that a magnet is embedded in an appropriate specified location in every              */
      /* object that has to picked up. Provided the hand is placed close to this location, the object will */ 
      /* be attached to the hand when the robot electromagnet is activated                                 */ 
      /*                                                                                                   */
      /* Input flag state (0 for open; 1 for close)  from port /grip:i                                     */

      tempIntVector = gripPortIn->read(false);                   // non-blocking version
      /* do {                              
            tempIntVector = gripPortIn->read(false);             // blocking version 
      } while ((tempdIntVector == NULL) && (isStopping() != true)); */
      if (tempIntVector != NULL) {
         p = (int) (*tempIntVector)[0];
         if (*verbose) printf("cognitiveControl:             grip                   < %d\n", p);
      }
  
      
      /* Primitive 2 */

      /* moveHand(handDescriptor, x, y, z, roll)                                                           */
      /* ------------------------------------------------------------------------------------------------- */
      /* The moveHand() primitive causes the robot to move its hand to a location given by the             */
      /* coordinates x, y, and z (defined in a world frame of reference) and orient the hand at that point */
      /* to align it with the roll angle (again, defined in a world frame of reference). The hand to be    */ 
      /* moved is specified by the handDescriptor argument (either left or right).                         */ 
      /*                                                                                                   */
      /* Input handDescriptor, x, y, z, and roll from port /moveHand:i                                     */

      tempDoubleVector = moveHandPortIn->read(false);                   // non-blocking version
      /* do {                              
            tempDoubleVector = moveHandPortIn->read(false);             // blocking version 
      } while ((tempdDoubleVector == NULL) && (isStopping() != true)); */
      if (tempDoubleVector != NULL) {
         p = (int)    (*tempDoubleVector)[0];
         x = (double) (*tempDoubleVector)[1];
         y = (double) (*tempDoubleVector)[2];
         z = (double) (*tempDoubleVector)[3];
         a = (double) (*tempDoubleVector)[4];
         if (*verbose) printf("cognitiveControl:             moveHand               < %d %2.1f %2.1f %2.1f %2.1f\n", p, x, y, z, a);
      }


      /* Primitive 3 */

      /* moveHead (x, y, z)                                                                                */
      /* ------------------------------------------------------------------------------------------------- */
      /* The moveHead() primitive causes the robot to move its head so that its gaze is directed at a      */
      /* location given by the coordinates x, y, and z (defined in a world frame of reference).            */
      /*                                                                                                   */
      /* Input x, y, z from port /moveHead:i                                                               */

      tempDoubleVector = moveHeadPortIn->read(false);                   // non-blocking version
      /* do {                              
            tempDoubleVector = moveHandPortIn->read(false);             // blocking version 
      } while ((tempdDoubleVector == NULL) && (isStopping() != true)); */
      if (tempDoubleVector != NULL) {
         x = (double) (*tempDoubleVector)[0];
         y = (double) (*tempDoubleVector)[1];
         z = (double) (*tempDoubleVector)[2];
         if (*verbose) printf("cognitiveControl:             moveHead               < %2.1f %2.1f %2.1f\n", x, y, z);
      }

 
      /* Primitive 4 */

      /* moveSequence(sequenceDescriptor)                                                                  */
      /* ------------------------------------------------------------------------------------------------- */
      /* The moveSequence() primitive causes the robot to replay a sequence of moves comprising a          */
      /* some physical animation, e.g. a dance or a wave or simply a pose that conveys some emotion.       */
      /* It takes one argument, sequenceDescriptor, which indicated what animation is to be enacted.       */
      /* The values of the sequenceDescriptor include the following: angry, happy, sad, fearful,           */
      /* cover_eyes, touch_head_with both_hands, stretch_out_arms, dance_A, dance_B, wave_A, wave_B, …     */
      /* Other sequence descriptors can be added as necessary. This primitive is more convenient than      */
      /* moveHand() when you need to position the hands with respect to the robot’s body since moveHand()  */
      /* requires target location specified in world coordinates.                                          */
      /*                                                                                                   */
      /* Input sequenceDescriptor from port /moveHead:i                                                    */

      tempIntVector = moveSequencePortIn->read(false);                   // non-blocking version
      /* do {                              
            tempIntVector = moveSequencePortIn->read(false);             // blocking version 
      } while ((tempIntVector == NULL) && (isStopping() != true)); */
      if (tempIntVector != NULL) {
         p = (int) (*tempIntVector)[0];
         if (*verbose) printf("cognitiveControl:             moveSequence           < %d\n", p);
      }


      /* Primitive 5 */

      /* moveTorso(x, y, z)                                                                                */
      /* ------------------------------------------------------------------------------------------------- */
      /* The moveTorso() primitive causes the robot to adjust its body so that the normal vector from      */
      /* the centre of is torso is direct at a location given by the coordinates x, y, and z (defined in a */
      /* world frame of reference). If the robot is sitting, then this primitive has no effect since there */
      /* are no degrees of freedom in the robot’s waist.                                                   */
      /*                                                                                                   */
      /* Input x, y, z from port /moveTorso:i                                                              */

      tempDoubleVector = moveTorsoPortIn->read(false);                   // non-blocking version
      /* do {                              
            tempDoubleVector = moveTorsoPortIn->read(false);             // blocking version 
      } while ((tempdDoubleVector == NULL) && (isStopping() != true)); */
      if (tempDoubleVector != NULL) {
         x = (double) (*tempDoubleVector)[0];
         y = (double) (*tempDoubleVector)[1];
         z = (double) (*tempDoubleVector)[2];
         if (*verbose) printf("cognitiveControl:             moveTorso              < %2.1f %2.1f %2.1f\n", x, y, z);
      }
 

      /* Primitive 6 */

      /* say(text, tone)                                                                                   */
      /* ------------------------------------------------------------------------------------------------- */
      /* The say() primitive causes the robot to speak the message contained in the text argument.         */
      /* The tone of voice is adjusted to reflect one of four emotions passed in the tone argument: happy, */
      /* sad, fearful, angry.                                                                              */
      /*                                                                                                   */
      /* Input text and tone  from port /say:i                                                             */

      tempBottle = sayPortIn->read(false);                   // non-blocking version
      /* do {                              
            tempBottle = sayPortIn->read(false);             // blocking version 
      } while ((tempBottle == NULL) && (isStopping() != true)); */
      if (tempBottle != NULL) {
         
         /* for some strange reason, YARP puts double quotes around every individual string in a bottle so we strip them here */
      
         j=0;
         for (i=0; i<min(STRINGLENGTH-1,(int)strlen(tempBottle->toString().c_str()));i++) {
            if (tempBottle->get(0).asString().c_str()[i]!='\"') {
               modifiedString[j] = tempBottle->get(0).asString().c_str()[i];
               j++; 
            }
         }
         modifiedString[j]='\0';

         p = (int) tempBottle->get(1).asInt();

         if (*verbose) printf("cognitiveControl:             say                    < %s %d\n", modifiedString, p);
      }
 

      /* Primitive 7 */

      /* enableRobot(state)                                                                                */
      /* ------------------------------------------------------------------------------------------------- */
      /* The enableRobot() primitive activates the robot                                                   */
      /*                                                                                                   */
      /* Input flag state (1 => enable, 0 => disable) from port /enableRobot:i                             */

      tempIntVector = enableRobotPortIn->read(false);                   // non-blocking version
      /* do {                              
            tempIntVector = enableRobotPortIn->read(false);             // blocking version 
      } while ((tempdIntVector == NULL) && (isStopping() != true)); */
      if (tempIntVector != NULL) {
         p = (int) (*tempIntVector)[0];
         if (*verbose) printf("cognitiveControl:             enableRobot            < %d\n", p);
      }


      /* Primitive 8 */

      /* getInterventionState(interventionDescriptor, stateDescriptor, cognitiveModeDescriptor)                   */
      /* --------------------------------------------------------------------------------------------------------- */
      /* The getInterventionState() primitive identifies the intervention that is currently being enacted,         */
      /* the current phase within that intervention (as defined by an equivalent finite state automaton            */
      /* with uniquely-labelled states), and situations when the controller is (cognitively) handling an           */
      /* unexpected deviation from a given intervention state (typcially as a result of the child’s                */
      /* behaviour exhibiting diminished engagement and/or performance in the current intervention).               */
      /*                                                                                                           */
      /* Output interventionDescriptor, stateDescriptor, cognitiveModeDescriptor from port /getInterventionState:o */  

      lambda = event_rate[current_primitive++] * elapsed_time; 
      count = samplePoisson(lambda);  
      //if (*verbose) printf("cognitiveControl:             getInterventionState lambda %4.3f \n", lambda);

      for (i=0; i<count; i++) {

         VectorOf<int> &dataVector = getInterventionStatePortOut->prepare();
         dataVector.resize(3);

         p = (int) (rand() % NUMBER_OF_INTERVENTIONS);  // randomly select identification number
         q = (int) (rand() % NUMBER_OF_STATES);         // randomly select identification number
         r = (int) (rand() % NUMBER_OF_MODES);          // randomly select identification number

         dataVector(0) = p;
         dataVector(1) = q;
         dataVector(2) = r;

         getInterventionStatePortOut->write(); 
         if (*verbose) printf("cognitiveControl:             getInterventionState   > %d %d %d\n",p, q, r);
      }


      /* last perception primitive processed */

      current_primitive = 0; //   reset the primitive counter for the next iteration
   }
}


void CognitiveControlThread::threadRelease() 
{
   /* for example, delete dynamically created data-structures */
}


/* Method to sample a Poisson distribution */

int CognitiveControlThread::samplePoisson(double lambda) {

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

