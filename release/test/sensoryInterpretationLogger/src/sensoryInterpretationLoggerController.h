 /** @file sensoryInterpretationLoggerController.h 
  *
  * Version 1.0
  * 
  * Generated by YarpGenerator, Emmanuel Senft, PLYM
  *
  *
  * \defgroup DREAM_cognitiveComponent sensoryInterpretationLogger
  * 
  * Main class with code, use the yarpInterface class to communicate with the other components
  * 
  * \section lib_sec Libraries
  *
  * None
  *
  * \section in_files_sec Input Data Files
  *
  * None
  *
  * \section out_data_sec Output Data Files
  *
  * None
  *
  * \author 
  * 
  * <Erik Billing>, <University of Skovde>  
  * 
  * Copyright (C) 2014 DREAM Consortium
  * 
  */

/* 
 * Copyright (C) 2014 DREAM Consortium
 * FP7 Project 611391 co-funded by the European Commission
 *
 * Author:  Erik Billing, University of Skovde 
 * Email:   erik.billing@his.se 
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
  * xx/xx/xx Version 1.0: ... (Erik Billing)
  */ 
 
#ifndef SENSORYINTERPRETATIONLOGGERCONTROLLER
#define SENSORYINTERPRETATIONLOGGERCONTROLLER

#include <iostream>
#include <string>
#include <vector>

class SensoryInterpretationLogHandler;

class SensoryInterpretationLoggerController{
private:
	SensoryInterpretationLogHandler* _yarpInterface;
public:
	/* class methods */
	SensoryInterpretationLoggerController(SensoryInterpretationLogHandler *yarpInterface);
   //Need to add the methods calling/being called by the YarpInterface
};

#endif
