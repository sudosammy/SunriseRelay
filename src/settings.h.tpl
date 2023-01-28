#define DEBUG true
#define SSID ""
#define PASSWD ""                           // Make sure this is strong since there's so many handshakes

#define RELAY 0                             // relay connected to  GPIO0
#define NTP_SERVER "oceania.pool.ntp.org"   // Network Time Server (probably no need to change this)
#define LAT ""                              // For use in the API call https://api.sunrise-sunset.org/json?lat=&lng=
#define LON ""

#define SLEEPTIME 268435                    // This cannot be bigger than 268435 milliseconds (4.47 minutes)