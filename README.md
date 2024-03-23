# EASILY CONFIGURE YOUR ARDUINO'S WIFI
**!!!WORK IN PROGRESS!!!**

# UI
Repository: https://github.com/smartfastlabs/arduino-wifi-config

To use the UI:
**NOTE:** You must load the UI over http not https.

- Open http://connect.arduino-wifi.com
- Switch your WiFi to the Access Point
- Configure WiFi credentials.
- Click Run

## Actual Documentation
**Coming Soon**


# Example CURLS
    curl -v 'http://192.48.56.2/health' 
    curl -v -X POST 'http://192.48.56.2/set'  -H 'password: <PASSWORD>' -H 'ssid: <SSID>'
    curl -v 'http://192.48.56.2/config' 
    curl -v 'http://192.48.56.2/run' 
