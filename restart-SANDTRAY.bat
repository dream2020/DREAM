SET app="C:\DREAM\release\app\sandtrayModules.xml"
yarpmanager --application %APP% --stop --exit
timeout 2
yarpmanager --application %APP% --kill --exit
timeout 2
yarpmanager --application %APP% --run --exit
timeout 2
yarpmanager --application %APP% --connect --exit