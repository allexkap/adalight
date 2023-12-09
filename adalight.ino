#define NUM_LEDS 110
#define DI_PIN 2

#define CURRENT_LIMIT 0


#include <FastLED.h>

const char prefix[] = "Ada\n";
CRGB leds[NUM_LEDS];


int blocking_serial_read(){
  while (!Serial.available());
  return Serial.read();
}

void wait_ada() {
  while (true) {
    for (int i = 0; i < sizeof prefix - 2; ) {
      if (prefix[i++] != blocking_serial_read()) i = 0;
    }

    uint8_t hi, lo, chk;
    hi = blocking_serial_read();
    lo = blocking_serial_read();
    chk = blocking_serial_read();
    if (chk == hi ^ lo ^ 0x55) break;
  }
}

void recv_ada() {
  memset(leds, 0, NUM_LEDS * sizeof(struct CRGB));
  for (int i = 0; i < NUM_LEDS; i++) {
    byte r, g, b;
    leds[i].r = blocking_serial_read();
    leds[i].g = blocking_serial_read();
    leds[i].b = blocking_serial_read();
  }
  FastLED.show();
}


void setup() {
  FastLED.addLeds<WS2812, DI_PIN, GRB>(leds, NUM_LEDS);
  if (CURRENT_LIMIT > 0) FastLED.setMaxPowerInVoltsAndMilliamps(5, CURRENT_LIMIT);
  Serial.begin(115200);
  Serial.print(prefix);
}

void loop() {
  wait_ada();
  recv_ada();
}
