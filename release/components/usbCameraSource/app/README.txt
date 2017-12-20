USB Camera Source Test
=======================


This application reads images from three cameras and display image streams (videos) in real0time through three Yarpviews, and saving video to files.

System architecture ports being used:
  

--None



Resources required to run the test:
 

--Three cameras being connected to the PC/WorkStation



Parameter setting in the configuration file (usbCameraSource.ini):

--outputPort          /usbCameraSource/camLeft:o      
 

--outputPort1         /usbCameraSource/camMid:o
 
--outputPort2         /usbCameraSource/camRight:o
 

Above parameters define the output port in the component, which is consistent with the standard output port defined in Wiki. Generally, the parameters are not allowed to be changed by users. The change of them may cause that other components cannot find the corresponding video stream from usbCameraSource.
 

--cameraDeviceID      2
--cameraDeviceID1     0	
--cameraDeviceID2     1
 

These parameters assign camera device IDs to usbCameraSource component. Device ID is an identification that is automatically assigned when a camera connects to a PC/Workstation. cameraDeviceID, cameraDeviceID1 and cameraDeviceID2 should be uniquely set, referring to the left, middle and right camera respectively. If they are changed to the same number or nubmers that are not related to any camera device, the compoent will not able to read images properly.

--frequency     25
 

It determines the maximum image acquisition speed 25 fps. A lower frequency will reduce the speed accordingly, while a higher frequency may increase the maximum speed but it is very likely to be limited by the performance of the camera. 


--width  1280
--height 960
 

--savingPath                            c:/videoSave/

The path for video data saving . Please create the directory first in your system.

--savingWithRatio			0.5
--savingHeightRatio			0.5

The compression ratio of the original video resolution for the with and height. Change them accordingly to adjust the recording resolution.


--width               			1280
--height              			960

The resolution of images are defined in these two parameters. The values can be changed to the resolution according to the applications. 
 
Ports being used in the APP
:
 
/usbCameraSource/camLeft:o
 
/usbCameraSource/camMid:o
 
/usbCameraSource/camRight:o

Yinfeng Fang
26/06/2016

