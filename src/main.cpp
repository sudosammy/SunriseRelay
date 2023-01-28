#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiUdp.h>
#include <NTPClient.h>
#include <ArduinoJson.h>
#include <settings.h>

WiFiUDP ntpUDP; // Define NTP Client
NTPClient timeClient(ntpUDP, NTP_SERVER, 0, 10); // we set _updateInterval to ensure NTP time is updated

uint8_t sunsetSaveDay = 10; // set this to a number that cannot be possible
tm sunset;
tm sunrise;

void saveSunset() {
  String host = (String)"https://api.sunrise-sunset.org/json?lat=" + LAT + "&lng=" + LON + "&formatted=0";
  WiFiClientSecure client;
  client.setInsecure(); //the magic line, use with caution
  client.connect(host, 443);

  HTTPClient http;
  http.begin(client, host);
  int httpCode = http.GET();

  if (httpCode == HTTP_CODE_OK) { // Check for successful response
    String payload = http.getString();
    
    DynamicJsonDocument json(512);
    deserializeJson(json, payload);

    if (DEBUG) {
      Serial.println("API Sunrise: " + (String)json["results"]["sunrise"]);
      Serial.println("API Sunset: " + (String)json["results"]["sunset"]);
    }

    // save sunrise/sunset times
    strptime(json["results"]["sunrise"], "%Y-%m-%dT%H:%M:%S", &sunrise);
    strptime(json["results"]["sunset"], "%Y-%m-%dT%H:%M:%S", &sunset);

  } else {
    Serial.println("Error checking status with server");
    Serial.println(http.getString());
    // notify me?
  }
}

void updSunset() {
  timeClient.update(); // update NTP time
  if(timeClient.getDay() != sunsetSaveDay) { // a day has passed
    if (DEBUG) { Serial.println("A day has passed, update the sunrise/sunset times"); }
    saveSunset();
    sunsetSaveDay = timeClient.getDay();
  }
}

void toggleRelay() {
  char sunriseString[9];
  strftime(sunriseString, sizeof(sunriseString), "%H:%M:%S", &sunrise);

  char sunsetString[9];
  strftime(sunsetString, sizeof(sunsetString), "%H:%M:%S", &sunset);

  if (DEBUG) {
    Serial.println("Saved Sunrise: " + String(sunriseString));
    Serial.println("Saved Sunset: " + String(sunsetString));
    Serial.println("Time Now: " + String(timeClient.getFormattedTime()));
  }

  if (timeClient.getFormattedTime() > sunriseString || timeClient.getFormattedTime() < sunsetString) {
    digitalWrite(RELAY, HIGH);
    Serial.println("Daytime. Relay toggled off");
  } else {
    digitalWrite(RELAY, LOW);
    Serial.println("Nighttime. Relay toggled on");
  }
}

void connectWifi() {
  if (WiFi.status() == WL_CONNECTED) {
    return; // already connected to WiFi
  }

  WiFi.mode(WIFI_STA);
  if (DEBUG) { Serial.println((String)"Connecting to " + SSID); }
  WiFi.begin(SSID, PASSWD);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    if (DEBUG) { Serial.print("."); }
  }
  WiFi.setAutoReconnect(true);
  Serial.println("");
  if (DEBUG) { Serial.println("WiFi connected"); }
}

void lightsleepCallback() {
  // ok to use blocking functions in the callback, but not
  // delay(), which appears to cause a reset
  Serial.println("Woke up!");
  Serial.flush();
}

void enterSleep(long sleepTimeMilliSeconds) {
  if (sleepTimeMilliSeconds > 268435) {
    Serial.println("WARNING: Risk of integer overflow. Not sleeping");
    return;
  }
  Serial.println("Sleeping...");

  Serial.flush();
  WiFi.mode(WIFI_OFF);

  // for timer-based light sleep to work, the os timers need to be disconnected
  extern os_timer_t *timer_list;
  timer_list = nullptr;

  // enable light sleep
  wifi_fpm_set_sleep_type(LIGHT_SLEEP_T);
  wifi_fpm_open();
  // specify callback function name
  wifi_fpm_set_wakeup_cb(lightsleepCallback);
  // light sleep function requires microseconds
  wifi_fpm_do_sleep(sleepTimeMilliSeconds * 1000);
  // timed light sleep is only entered when the sleep command is
  // followed by a delay() that is at least 1ms longer than the sleep
  delay(sleepTimeMilliSeconds + 1);
}

void setup() {
  Serial.begin(115200);
  pinMode(RELAY, OUTPUT);
  digitalWrite(RELAY, LOW);
}
 
void loop() {
  connectWifi();

  if (DEBUG) {
    Serial.print("Relay IP address: ");
    Serial.println(WiFi.localIP());
  }

  updSunset();

  toggleRelay();

  enterSleep(SLEEPTIME); // 10e3 = 10,000 milliseconds = 10 seconds
}
