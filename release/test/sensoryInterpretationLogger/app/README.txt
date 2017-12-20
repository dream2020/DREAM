
Sensory Interpretation Logger Test
==================================

Unit test for sensoryInterpretationLogger component. Instanciates the DREAM
architecture with placeholder components and loggs data from
sensoryInterpretation. The complete list of ports is available in
sensoryInterpretation.ini.

Edit the xml file to suit your own platform.  The only change that should be 
necessary is to replace the node name (dream1 in the template).

- modify the node name to whatever is specified in your yarp runserver name 
  (dream1 by default)
- modify the path to the /config directory in the context parameter,
  (Remember: the resource finder searches from $DREAM_ROOT/release/ so make 
  the path relative to this folder)
- modify the path to the log file to satisfy your needs. The default log file 
  is sensoryInterpretation.log and will be located in the context directory. 

By default, the component will perform asynchronous logging. Synchronous logging
can be enabled by specifying a period > 0 in the configuration file. All ports
will then be read in a non-blocking mode with the specified period. For details
on the logging formats, please refer to the component documentation.

The list of input ports to sensoryInterpretationLogger can be adjusted to
satisfy your needs, and can also be configured to log data from sources other
than sensoryInterpretation. This is done by adding or removing ports from the
[LOGPORTS] section in the configuration file. Please refer to the
documentation for details.

# Erik Billing
# 2016-02-18



