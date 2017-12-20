/* 
 * Copyright (C) 2014 DREAM Consortium
 * FP7 Project 611391 co-funded by the European Commission
 *
 * Author:  David Vernon, University of Sk�vde 
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
 * 20/08/14  First version validated (David Vernon)
 */ 

#include "protoComponentGUI.h" 

int main(int argc, char * argv[])
{
   /* initialize yarp network */ 
 
   Network yarp;

   /* create your module */

   ProtoComponentGUI protoComponentGUI; 

   /* prepare and configure the resource finder */

   ResourceFinder rf;
   rf.setVerbose(true);
   rf.setDefaultConfigFile("protoComponentGUI.ini");          // can be overridden by --from parameter
   rf.setDefaultContext("components/protoComponent/config");   // can be overridden by --context parameter
   rf.configure("DREAM_ROOT", argc, argv);                    // environment variable with root of configuration path
 
   /* run the module: runModule() calls configure first and, if successful, it then runs */

   protoComponentGUI.runModule(rf);

   return 0;
}