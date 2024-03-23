# EASILY CONFIGURE YOUR ARDUINO'S WIFI
**!!!WORK IN PROGRESS!!!**

# UI
Repository: https://github.com/smartfastlabs/arduino-wifi-config
<center>



</center>

To use the UI:

**NOTE:** You must load the UI over http not https.

# Open http://connect.arduino-wifi.com
<img width="699" style="margin-x: auto" alt="image" src="https://github.com/smartfastlabs/arduino-wifi-config/assets/598661/54c399a7-6129-437f-841b-4b18a7a93746">

# Switch your WiFi to the Access Point
<img width="302" alt="image" src="https://github.com/smartfastlabs/arduino-wifi-config/assets/598661/6a168ece-643e-48f9-b8c8-1c53aa482eef">

# Configure your WiFi credentials and IP
<img width="679" alt="image" src="https://github.com/smartfastlabs/arduino-wifi-config/assets/598661/4845e870-a48e-4b67-aacd-f7a3ae4e8df6">

# Click "Save Changes and Connect"
<img width="661" alt="image" src="https://github.com/smartfastlabs/arduino-wifi-config/assets/598661/3455ab6c-b246-4977-905f-10027efa73c3">


## Actual Documentation
**Coming Soon**


# Example CURLS
    curl -v 'http://192.48.56.2/health' 
    curl -v -X POST 'http://192.48.56.2/set'  -H 'password: <PASSWORD>' -H 'ssid: <SSID>'
    curl -v 'http://192.48.56.2/config' 
    curl -v 'http://192.48.56.2/run' 
