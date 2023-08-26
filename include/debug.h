#ifndef DEBUG_H
#define DEBUG_H

// #define DEBUG

#ifdef DEBUG
#define DEBUG_INIT Serial.begin(9600); while (!Serial); Serial.println("Started");
#define DEBUG_PRINT(x) Serial.print(x)
#define DEBUG_PRINTLN(x) Serial.println(x)
#else
#define DEBUG_INIT do {} while (0)
#define DEBUG_PRINT(x) do {} while (0)
#define DEBUG_PRINTLN(x) do {} while (0)
#endif

#endif // DEBUG_H
