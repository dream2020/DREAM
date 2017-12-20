SET app="release\app\DREAM.xml"
yarpmanager --application %APP% --run --exit
timeout 10
yarpmanager --application %APP% --connect --exit
