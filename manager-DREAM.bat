SET app="release\app\DREAM.xml"
rem start "YARP SERVER" cmd /K yarp server --write
rem start "DREAM1" cmd /K yarp run --server /dream1
gyarpmanager --application %APP%
