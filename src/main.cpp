#include <Arduino.h>
#include <DHT.h>
#include <esp_sleep.h>

const int DHT_PIN = 4;
const int DHT_TYPE = DHT22;
const uint64_t SLEEP_US = 60ULL * 1000000ULL;

DHT dht(DHT_PIN, DHT_TYPE);

RTC_DATA_ATTR uint32_t bootCount = 0;
RTC_DATA_ATTR float lastTemp = NAN;
RTC_DATA_ATTR float lastHum  = NAN;

void printWakeReason() {
  esp_sleep_wakeup_cause_t reason = esp_sleep_get_wakeup_cause();
  switch (reason) {
    case ESP_SLEEP_WAKEUP_TIMER: Serial.println(F("Wake: timer")); break;
    case ESP_SLEEP_WAKEUP_EXT0:  Serial.println(F("Wake: ext0")); break;
    default: Serial.println(F("Wake: power-on or reset")); break;
  }
}

void setup() {
  Serial.begin(115200);
  delay(300);
  bootCount++;

  Serial.print(F("Boot #"));
  Serial.println(bootCount);
  printWakeReason();

  dht.begin();
  delay(500); // DHT needs settle time after power-up

  float t = dht.readTemperature();
  float h = dht.readHumidity();

  if (!isnan(t) && !isnan(h)) {
    lastTemp = t;
    lastHum  = h;
    Serial.print(F("T=")); Serial.print(t, 1);
    Serial.print(F(" H=")); Serial.println(h, 1);
  } else {
    Serial.println(F("DHT read failed - keeping previous RTC values"));
    Serial.print(F("Last T=")); Serial.print(lastTemp);
    Serial.print(F(" H=")); Serial.println(lastHum);
  }

  Serial.print(F("Sleeping for "));
  Serial.print((uint32_t)(SLEEP_US / 1000000ULL));
  Serial.println(F(" s"));
  Serial.flush();

  esp_sleep_enable_timer_wakeup(SLEEP_US);
  esp_deep_sleep_start();
}

void loop() {
  // not reached
}
