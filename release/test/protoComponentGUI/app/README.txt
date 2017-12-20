protoComponentGUI.xml

This application demonstates the use of the protoComponentGUI module. 
This is a simple example to illustrate how to control the protoComponent module interactively
with a GUI that is compliant with mandatory DREAM software standards for

1. File organization
2. Internal source code documentation
3. Component functionality

as well as recommended DREAM software standards for 

4. Programming style
5. Programming practice 

These standards are documented in Appendices A-E of Deliverable 3.2 

Overall, the standards address the 4 Cs of component-based software engineering:

- configuration
- coordination
- computation
- computation
 
Functionally, the component provides a graphic user interface to another component,
protoComponent that, in turn, 

- reads an input image from an input port, 
- converts it to a binary image based on the supplied threshold,
- and writes it to an output port.

The GUI itself displays the raw colour input image and the processed binary output image, 
writes image statistics in an output box, and allows the theshold to be altered 
using a slider widget. 

A complete tutorial for this example is available on the DREAM wiki 

Edit the xml file to suit your own platform.  The only change that should be 
necessary is to replace the node name (dream1 in the template).

- modify the node name to whatever is specified in your yarp run --server /nodename command (its dream1 in the TEST)
- modify the path to the /config directory in the context parameter 
  (remember: the resource finder searches from $DREAM_ROOT/release/ so make the path relative to this.
- make sure that the port dependencies are satisfied

Note that the application has one port dependency:

/robot/cam/left 

which streams images from the left camera.

If this port is not available from the robot (and it is unlikely to be if you are
running this example to get familiar with how components work) you can run another
component - imageSource - to emulate this functionality and make this port available
with valid data from an image file being streamed on it.  To do this, just run
imageSource.xml in the imageSource application directory.

David Vernon
27/10/2014
