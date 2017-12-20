heartbeatTEST.xml

This application demonstates the use of the heartbeat component in interaction
with videoSource. The heartbeat component sends a constant heartbeat on the
default output port. videoSource is configured to listen to the heartbeat and
sends one frame for each received beat.

The source video file is specified in videoSource/config/videoSource.ini.
Frame rate is specified in heartbeat/config/heartbeat.ini.

Edit the xml application file to suit your own platform. The only changes that
should be necessary are

- modify the node name to whatever is specified in your yarp run --server
  /nodename command (dream1 in the TEST)
- modify the path to the /config directory in the context parameter (remember:
  the resource finder searches from $DREAM_ROOT/ so make the path relative to
  this.
- make sure that the port dependencies are satisfied

Erik Billing 2016-01-22


