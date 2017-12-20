/** @file commonFonctionalities.h
*
* Version 1.0
*
* \defgroup DREAM_cognitiveComponent sharedResources
*
* Header class to share commonFonctionalities class based on boost asio
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
* <Emmanuel Senft>, <Plymouth University>
*
* Copyright (C) 2014 DREAM Consortium
*
*/

/*
* Copyright (C) 2014 DREAM Consortium
* FP7 Project 611391 co-funded by the European Commission
*
* Author:  Emmanuel Senft, Plymouth University
* Email:   emmanuel.senft@plymouth.ac.uk
* Website: www.dream2020.eu
*
* This program comes with ABSOLUTELY NO WARRANTY
*/

/*
* Audit Trail
* -----------
* 07/11/2016 Version 1.0: ... (Emmanuel Senft)
*/
#ifndef COMMONFUNCTIONALITIES
#define COMMONFUNCTIONALITIES

#include <boost/asio.hpp>
#include <string>
#include <vector>
#include <sstream>

class CommonFunctionalities {
private:
	boost::asio::io_service *_io;
	
public:
	CommonFunctionalities(){
		_io = new boost::asio::io_service();
	}
	
	void waitms(int millisec){
		boost::asio::deadline_timer t(*_io);
		t.expires_from_now(boost::posix_time::milliseconds(millisec));
		t.wait();
	}
	std::vector<std::string> split(const std::string &s, char delim){
		std::vector<std::string> elems;
		std::stringstream ss;
		ss.str(s);
		std::string item;
		while (std::getline(ss, item, delim)) {
			elems.push_back(item);
		}
		return elems;
	}
	std::string removeString(std::string base, std::string toRemove) {
		std::string::size_type index = base.find(toRemove);
		if (index != std::string::npos)
			base.erase(index, toRemove.length());
		return base;
	}


};

#endif