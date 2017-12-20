imageSourceTEST.xml

This application demonstates the use of the imageSource module which 
streams an image from a file to a port. This allows the application to be run 
without having to be physically connected to the iCub cameras.  Random noise is 
added to each image to differentiate between frames. In this application, 
the imageSource module streams the images to /robot/cam/left so that you can adapt the 
application to read from the cameras simply by removing the imageSource module.

The image file is provided in the $DREAM_ROOT/release/components/imageSource/config directory
along with the imageSource.ini configuration file.

Edit the xml.template file to suit your own platform.  
The only changes that should be necessary are 

- modify the node name to whatever is specified in your yarp run --server /nodename command (dream1 in the TEST)
- modify the path to the /config directory in the context parameter 
  (remember: the resource finder searches from $DREAM_ROOT/release/ so make the path relative to this.
- make sure that the port dependencies are satisfied

David Vernon
25/8/2014

