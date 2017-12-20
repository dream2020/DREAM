#include <yarp/sig/all.h>
#include <yarp/os/all.h>
#include <yarp/os/RFModule.h>
#include <yarp/os/Network.h>
#include <yarp/os/Thread.h>
#include <iostream>
#include <string>

using namespace yarp::os;
using namespace yarp::sig;
using namespace std;

//this constructs an appropriate good or bad body-center vector
double Bodydata(int good) {

  double y = 0;

  if (!good) y = 10; //simulate body too far off-center
  return y;

}

/*****************************************************************************/

//this constructs an appropriate good or bad Facial expression vector
int Smiledata(int good) {

  int y = 0;

  if (!good) y = 3; //something other than 1 is currently taken to be bad
  return y;

}

/*****************************************************************************/

//this constructs an appropriate good or bad mutual gaze vector
int Gazedata(int good) {

  int y = 1;
  if (!good) y = 0; 
  return y;

}

/*****************************************************************************/

int main(int argc, char *argv[]) {
  
  //VectorOf<double> flag;
  //flag.resize(1);

  Network yarp;


  BufferedPort<VectorOf<int> > faceOut;
  BufferedPort<VectorOf<int> > gazeOut; 
  BufferedPort<VectorOf<double> > bodyOut; 
  BufferedPort<VectorOf<double> > flagIn; 
  
  double answer;
  bool ok;
  bool first_reply = 0;

  VectorOf<double> sendbody;
  VectorOf<int> sendface;
  VectorOf<int> sendgaze;

  sendbody.resize(3);
  sendface.resize(2);
  sendgaze.resize(1);

  for (int i = 0; i < 3; i++) sendbody[i] = -1;
  for (int i = 0; i < 2; i++) sendface[i] = -1;
  sendgaze[0] = -100;
  sendface[1] = 1;


  VectorOf<double> *flag = NULL;

  ok =       faceOut.open("/assessChildEngagementDriver/identifyFaceExpression:o");
  ok = ok && gazeOut.open("/assessChildEngagementDriver/checkMutualGaze:o");
  ok = ok && bodyOut.open("/assessChildEngagementDriver/getBody:o");
 
  ok = ok && flagIn.open("/assessChildEngagementDriver/engagementFlag:i");

  if (!ok) {
    cout << "Something went wrong";
    return 1;
  }

  RpcServer ctrl_port;
  ctrl_port.open("/assessChildEngagementDriver");

  Bottle test_in;
  ctrl_port.read(test_in,true);



  //cycle through all possible states relevant for child engagement, and send these
  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 2; j++) {
      for (int k = 0; k < 2; k++) {

        Bottle test_out;

        test_out.addString(string("Simulating events: smile ") + to_string(static_cast<long long> (i)) + ", body " + to_string(static_cast<long long> (j)) + ", gaze " + to_string(static_cast<long long> (k)));


        sendface[0] = Smiledata(i);
        sendbody[0] = Bodydata(j);
        sendgaze[0] = Gazedata(k);


        VectorOf<int>&face_o = faceOut.prepare();
        face_o = sendface;
        faceOut.write();
   
        VectorOf<double>&body_o = bodyOut.prepare();
        body_o = sendbody;
        bodyOut.write();
   
        VectorOf<int>&gaze_o = gazeOut.prepare();
        gaze_o = sendgaze;
        gazeOut.write();

        flag = flagIn.read();


        test_out.addString(string ("Response obtained: ") + to_string(static_cast<long long>( (double) (*flag)[0])));

        ctrl_port.reply(test_out);


        Bottle test_in;
        ctrl_port.read(test_in,true);



      }
    }
  }

  Bottle test_out;
  test_out.addString("Test done; the driver will now exit and this port cease to exist. This won't look pretty...");
  ctrl_port.reply(test_out);

  return 0;
}

