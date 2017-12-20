videoSourceTEST.xml

This application demonstates the use of the videoSource module which 
streams an image from a video file to a port. This allows the application to be run 
without having to be physically connected to the robot cameras. 
The videoSource module streams the images to /videoSource/image:o so that you can adapt the 
application to read from the cameras simply by removing the videoSource module.

An example video file is provided in the $DREAM_ROOT/components/videoSource/config directory
along with the videoSource.ini configuration file.

Edit the xml application file to suit your own platform. The only changes that should be necessary are 

- modify the node name to whatever is specified in your yarp run --server /nodename command (dream1 in the TEST)
- modify the path to the /config directory in the context parameter 
  (remember: the resource finder searches from $DREAM_ROOT/ so make the path relative to this.
- make sure that the port dependencies are satisfied

Erik Billing
2015-11-27


