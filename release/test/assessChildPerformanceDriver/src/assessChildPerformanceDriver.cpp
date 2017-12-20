#include <yarp/sig/all.h>
#include <yarp/os/all.h>
#include <yarp/os/RFModule.h>
#include <yarp/os/Network.h>
#include <yarp/os/Thread.h>
#include <yarp/os/Time.h>
#include <iostream>
#include <vector>
#include <string>
using namespace yarp::os;
using namespace yarp::sig;
using namespace std;

/*****************************************************************************/
/* Functions to generate fake data */
/*****************************************************************************/
VectorOf<int> voice_event(int good) {

  VectorOf<int> y(1);

  y[0] = good;
  return y;

}

/*****************************************************************************/
VectorOf<double> gaze_event(int event_type, int good) {

  VectorOf<double> y(3);

  for (int i = 0; i < 3; i++) y[i] = 0;

  switch(event_type) {
    case 5: // look left
      if (good) y[0] = -50;
      else y[0] = 50;

      break;

    case 6: // look right
      if (good) y[0] = 50;
      else y[0] = -50;

      break;

    default:
      cout << "Requested unrecognised gaze event; this is a bug in the test\n";

  }

  return y;

}

/*****************************************************************************/
VectorOf<double> body_event(int event_type, int good) {

  //the strategy here is to send appropriat edeltas for body pose that are then processed
  //in main because body movements are a function over time.
  VectorOf<double> y(30);

  for (int i = 0; i < 30; i++) y[i] = 0;

  switch (event_type) {
    case 7:  // point left
      y[14] = -0.1; //move the left wrist forward

      if (good) y[12] = -0.1; //move the left wrist to the left
      else y[12] = 0.1; //move it to the right instead

      break;

    case 8:  // point right
      y[14] = -0.1; //move the left wrist forward

      if (good) y[12] = 0.1; //move the left wrist to the right
      else y[12] = -0.1; //move it to the left instead

      break;

    case 9:  // no movement

      if (!good)  for (int i = 0; i < 30; i++) y[i] = -10; //move towards the sandtray if it is supposed to be bad movement
      break;

    default:
      cout << "Requested unrecognised body event; this is a bug in the test\n";    
  }

  return  y;

}

/*****************************************************************************/
VectorOf<double> sensible_body() {

  VectorOf<double> y(30);

  for (int i = 0; i < 30; i++) y[i] = 100;

  //position the wrist that is tested in this unit test in a reasonable position in space
  y[14] = 100; 
  y[12] = 0; 

  return  y;

}
/*****************************************************************************/
VectorOf<double> trajectory_event(int event_type, int good) {

  VectorOf<double> y(12);

  if (event_type == 9) event_type++; // ensure that indices work in the if/else below

  for (int i = 0; i < 10; i++) y[i] = 0;

  if (good) y[event_type - 10] = 1;
  else y[event_type - 10] = 0;

  return  y;

}

/*****************************************************************************/
VectorOf<int> sandtray_event(int event_type, int good) {

  VectorOf<int> y(2);

  /* Expected sandtray events from intervention scripts:
     1: child perform good sandtray move 
     2: child touch sandtray image 
     3: child does not touch sandtray 
     4: child touch robot-owned sandtray image

    Sandtray events from actual sandtray
    0   good sandtray move
    1   bad sandtray move
    2   touch on sandtray image
    3   touch off sandtray image
    4   child touch robot-owned sandtray image

  */

  for (int i = 0; i < 2; i++) y[i] = -10;

  switch (event_type) {
      case 1: //expect good sandtray move
        y[0] = 1 - good; //0 is good as far as the sandtray is concerned.
        break;

      case 2: // touch sandtray image
        y[0] = 3 - good; //2 is good as far as the sandtray is concerned.
        break;

      case 3: //does not touch sandtray
        if (good == 0) y[0] = 2; //just send a touch if that would have been bad
        break;

      case 4: //touch robot-owned sandtray image
        if (good) y[0] = 4;
        else y[0] = 1;
        break;

      default:
        cout << "Requested unrecognised sandtray event; this is a bug in the test\n";      
  }


  return y;


}

/*****************************************************************************/
/* The actual test */
/*****************************************************************************/

int main(int argc, char *argv[]) {
  
  //VectorOf<double> flag;
  //flag.resize(1);

  cout << "Start" << endl;

  Network yarp;

  BufferedPort<VectorOf<int> > voiceOut;
  BufferedPort<VectorOf<double> > gazeOut; 
  BufferedPort<VectorOf<double> > bodyOut; 
  BufferedPort<VectorOf<double> > trajectoryOut; 
  BufferedPort<VectorOf<int> > eventOut; 

  BufferedPort<VectorOf<int> > statusOut; 
  

  BufferedPort<VectorOf<int> > trajectoryIn; 
  BufferedPort<VectorOf<double> > performanceIn; 
  

  bool ok;
  double sent_time;
  int read_perf;

  VectorOf<int> sendvoice;
  sendvoice.resize(1);
  sendvoice[0] = -1;

  VectorOf<double> sendgaze;
  sendgaze.resize(3);
  for (int i = 0; i < 3; i++) sendgaze[i] = -1;

  VectorOf<double> sendbody;
  sendbody.resize(30);
  for (int i = 0; i < 30; i++) sendbody[i] = -1;

  VectorOf<double> bodydelta;
  bodydelta.resize(30);
  for (int i = 0; i < 30; i++) bodydelta[i] = 0;

  VectorOf<double> sendtraj;
  sendtraj.resize(12);
  for (int i = 0; i < 12; i++) sendtraj[i] = -1;

  VectorOf<int> sendevent;
  sendevent.resize(2);
  for (int i = 0; i < 2; i++) sendevent[i] = -1;


  VectorOf<int> sendstatus;
  sendstatus.resize(7);
  for (int i = 0; i < 7; i++) sendstatus[i] = -1;


  VectorOf<int> *traj = NULL;
  VectorOf<double> *perf = NULL;

  ok = voiceOut.open("/assessChildPerformanceDriver/identifyVoice:o");
  ok = ok && gazeOut.open("/assessChildPerformanceDriver/getEyeGaze:o"); 
  ok = ok && bodyOut.open("/assessChildPerformanceDriver/getBodyPose:o"); 
  ok = ok && trajectoryOut.open("/assessChildPerformanceDriver/identifyTrajectory:o"); 
  ok = ok && statusOut.open("/assessChildPerformanceDriver/getInterventionStatus:o"); 
  ok = ok && eventOut.open("/assessChildPerformanceDriver/interactionEvent:o"); 
  ok = ok && trajectoryIn.open("/assessChildPerformanceDriver/identifyTrajectory:i"); 
  ok = ok && performanceIn.open("/assessChildPerformanceDriver/getChildPerformance:i"); 

  if (!ok) {
    cout << "Something went wrong";
    return 1;
  }

  RpcServer ctrl_port;
  ctrl_port.open("/assessChildPerformanceDriver");

  Bottle test_in;
  ctrl_port.read(test_in,true);

  //be nice to the person running this - be explicit about the behaviours tested
  vector<string> behaviour_list;
  behaviour_list.push_back("child perform good sandtray move");
  behaviour_list.push_back("child touch sandtray image");
  behaviour_list.push_back("child does not touch sandtray");
  behaviour_list.push_back("child touch robot-owned sandtray image");
  behaviour_list.push_back("look left");
  behaviour_list.push_back("look right");
  behaviour_list.push_back("point left");
  behaviour_list.push_back("point right");
  behaviour_list.push_back("no movement");
  behaviour_list.push_back("child speaks");
  behaviour_list.push_back("hand wave");
  behaviour_list.push_back("hands covering eyes");
  behaviour_list.push_back("hands on head");
  behaviour_list.push_back("fly");
  behaviour_list.push_back("drive car");
  behaviour_list.push_back("drink/smell");
  behaviour_list.push_back("new complex traj 1");
  behaviour_list.push_back("new complex traj 2");
  behaviour_list.push_back("new complex traj 3");
  behaviour_list.push_back("new complex traj 4");
  behaviour_list.push_back("new complex traj 5");


  int expected_rt = 1500; //for the present purposes, expect the behaviours to happen within 1.5s

  sendstatus[4] = expected_rt;

  //cycle through the 20 possible behaviours that can be sent to assessChildPerformance
  for (int behaviour_type = 1; behaviour_type <= 21; behaviour_type++) {

    sendstatus[2] = behaviour_type;

    //for each behaviour, pretend both a bad and a good performance reported by sensory analysis
    for (int good = 0; good < 2; good++) {

      Bottle test_out;

      if (good) test_out.addString(string("Simulating a good ") + behaviour_list.at(behaviour_type-1));
      else      test_out.addString(string("Simulating a bad ") + behaviour_list.at(behaviour_type-1));

       VectorOf<int>&status_o = statusOut.prepare();
       status_o = sendstatus;
       statusOut.write();

       read_perf = 0;

       switch (behaviour_type) {

        //Sandtray related behaviour expected
        case 1: // child perform good sandtray move 
        case 2: // child touch sandtray image 
        case 3: // child does not touch sandtray 
        case 4: // child touch robot-owned sandtray image
          sendevent = sandtray_event(behaviour_type, good);
          break;

        //gaze
        case 5:  // look left
        case 6:  // look right
          sendgaze = gaze_event(behaviour_type, good);
          break;

        //body pose
        case 7:  // point left
        case 8:  // point right
        case 9:  // no movement
          sendbody = sensible_body();
          bodydelta = body_event(behaviour_type, good);
          break;

        //voice
        case 10: // child speaks
          sendvoice = voice_event(good);
          break;

        //trajectory
        case 11: // hand wave (1)
        case 12: // hands covering eyes (2) 
        case 13: // hands on head (3)
        case 14: // fly (4)
        case 15: // drive car (5)
        case 16: // drink/smell (6)
        case 17: // new complex traj 1 (7)
        case 18: // new complex traj 2 (8)
        case 19: // new complex traj 3 (9)
        case 20: // new complex traj 4 (10)
        case 21: // new complex traj 5 (11)

          sendtraj = trajectory_event(behaviour_type, good);
          break;

        default:
          cout << "Trying a behaviour that does not exist. This is a bug in tesst\n";
      }
        

      sent_time = Time::now();
      while(Time::now() <= sent_time + 5) {

        //don't expect the real sandtray to alyway send events  
        if (behaviour_type < 5 &&  sendevent[0] >= 0) {  
          VectorOf<int>&event_o = eventOut.prepare();
          event_o = sendevent;
          eventOut.write();
        }

        VectorOf<double>&gaze_o = gazeOut.prepare();
        gaze_o = sendgaze;
         gazeOut.write();

        VectorOf<int>&voice_o = voiceOut.prepare();
        voice_o = sendvoice;
        voiceOut.write();

        VectorOf<double>&traj_o = trajectoryOut.prepare();
        traj_o = sendtraj;
        trajectoryOut.write();

        VectorOf<double>&body_o = bodyOut.prepare();
        for (int i = 0; i < 30; i++) sendbody[i] += bodydelta[i]; // keep moving the body
        body_o = sendbody;
        bodyOut.write();
    
        perf = performanceIn.read(false);

        if (perf != NULL && (*perf)[0] != -1 ) {
          test_out.addString("Response: ");
          test_out.addDouble((double) (*perf)[0]);
          read_perf = 0;
          break;
        } else if (perf != NULL && (*perf)[0] == -1 ) { 
            // make sure we catch -1 outputs if they happen
            read_perf = -1;
        }
      }

      if (read_perf == -1) test_out.addString("Response: -1");
      else if (perf == NULL) test_out.addString("Response: NULL");

      ctrl_port.reply(test_out);


      Bottle test_in;
      ctrl_port.read(test_in,true);

    }
  }


  Bottle test_out;
  test_out.addString("Test done; the driver will now exit and this port cease to exist. This won't look pretty...");
  ctrl_port.reply(test_out);


  return 0;
}

