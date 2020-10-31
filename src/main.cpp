/*
 * Zenith Supersport 286 Keyboard interpreter 
 */

#include <Keypad.h>
#include "usb_desc.h"
#include "usb_seremu.h"

const byte ROWS = 16;
const byte COLS = 6;
#ifdef M
#undef M
#endif
#define M(n) ((n)&KEYCODE_MASK)

/*{'H', M(KEY_TILDE), M(KEY_TAB), M(KEY_CAPS_LOCK), M(MODIFIERKEY_LEFT_SHIFT), M(MODIFIERKEY_LEFT_CTRL)},
    {M(KEY_F1), M(KEY_1), M(KEY_Q), 0, 0, M(MODIFIERKEY_GUI)},
    {M(KEY_F2), M(KEY_2), M(KEY_W), M(KEY_A), 0, 0},
    {M(KEY_F3), M(KEY_3), M(KEY_E), M(KEY_S), M(KEY_Z), M(KEY_LEFT_ALT)},
    {M(KEY_F4), M(KEY_4), M(KEY_R), M(KEY_D), M(KEY_X), 0},
    {M(KEY_F5), M(KEY_5), M(KEY_R), M(KEY_F), M(KEY_C), 0}
    ,*/
char keys[ROWS][COLS] = {
    {M(KEY_1), M(KEY_2), M(KEY_3), M(KEY_4), M(KEY_5), M(KEY_6)},
    {M(KEY_PAUSE), M(KEY_BACKSPACE), 0, M(KEY_ENTER), M(KEY_UP), M(KEY_DOWN)},
    {M(KEY_DELETE), 0, M(KEY_RIGHT_BRACE), M(KEY_BACKSLASH), M(MODIFIERKEY_RIGHT_SHIFT), M(KEY_LEFT)},
    {M(KEY_F12), M(KEY_EQUAL), M(KEY_LEFT_BRACE), M(KEY_QUOTE), M(KEY_SLASH), M(MODIFIERKEY_CTRL)},
    {M(KEY_F11), M(KEY_MINUS), M(KEY_P), M(KEY_SEMICOLON), M(KEY_PERIOD), 0},
    {M(KEY_F10), M(KEY_0), M(KEY_O), M(KEY_L), M(KEY_COMMA), M(MODIFIERKEY_ALT)},
    {M(KEY_F9), M(KEY_9), M(KEY_I), M(KEY_K), M(KEY_M), 0},
    {M(KEY_F8), M(KEY_8), M(KEY_U), M(KEY_J), M(KEY_N), 0},
    {M(KEY_F7), M(KEY_7), M(KEY_Y), M(KEY_H), M(KEY_B), M(KEY_SPACE)},
    {M(KEY_F6), M(KEY_6), M(KEY_T), M(KEY_G), M(KEY_V), 0},
    {M(KEY_F5), M(KEY_5), M(KEY_R), M(KEY_F), M(KEY_C), 0},
    {M(KEY_F4), M(KEY_4), M(KEY_E), M(KEY_D), M(KEY_X), 0},
    {M(KEY_F3), M(KEY_3), M(KEY_W), M(KEY_S), M(KEY_Z), M(MODIFIERKEY_ALT)},
    {M(KEY_F2), M(KEY_2), M(KEY_Q), M(KEY_A), 0, 0},
    {M(KEY_F1), M(KEY_1), 0, 0, 0, M(MODIFIERKEY_GUI)},
    {M(KEY_ESC), M(KEY_TILDE), M(KEY_TAB), M(KEY_CAPS_LOCK), M(MODIFIERKEY_LEFT_SHIFT), M(MODIFIERKEY_CTRL)}

};

byte rowPins[ROWS] = {16, 15, 14, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {22, 20, 21, 19, 18, 17};                               //connect to the column pinouts of the keypad

Keypad kpd = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

unsigned long loopCount;
unsigned long startTime;
String msg;
int x = 0;

boolean modifierShiftPressed = false;
boolean modifierShiftLeftPressed = false;
boolean modifierShiftRightPressed = false;
boolean modifierAltPressed = false;
boolean modifierGuiPressed = false;
boolean modifierCtrlPressed = false;

void setup()
{
  Serial.begin(9600);
  loopCount = 0;
  startTime = millis();
  msg = "";
}
void loop()
{
  loopCount++;
  //char key = kpd.getKey();
  if ((millis() - startTime) > 5000)
  {
    Serial.print("Average loops per second = ");
    Serial.println(loopCount / 5);
    startTime = millis();
    loopCount = 0;
  }

  // Fills kpd.key[ ] array with up-to 10 active keys.
  // Returns true if there are ANY active keys.
  if (kpd.getKeys())
  {
    for (int i = 0; i < LIST_MAX; i++) // Scan the whole key list.
    {
      if (kpd.key[i].stateChanged) // Only find keys that have changed state.
      {
        switch (kpd.key[i].kstate)
        { // Report active key state : IDLE, PRESSED, HOLD, or RELEASED
        case PRESSED:
          msg = " PRESSED.";

          Serial.print("Key ");
          Serial.print(kpd.key[i].kchar);
          Serial.println(msg);
          // release all the keys at the same instant
          Keyboard.set_key1(kpd.key[i].kchar);
          Keyboard.send_now();

          //Keyboard.set_key1(0);
          //Keyboard.send_now();
          if (kpd.key[i].kchar == char(M(MODIFIERKEY_LEFT_SHIFT)))
          {
            Serial.print("Shift ");

            Keyboard.set_modifier(M(MODIFIERKEY_LEFT_SHIFT));
            Keyboard.send_now();
            modifierShiftLeftPressed = true;
          }
          if (kpd.key[i].kchar == char(M(MODIFIERKEY_RIGHT_SHIFT)))
          {
            Serial.print("Shift ");

            Keyboard.set_modifier(M(MODIFIERKEY_RIGHT_SHIFT));
            Keyboard.send_now();
            modifierShiftLeftPressed = true;
          }
          if (kpd.key[i].kchar == char(M(MODIFIERKEY_CTRL)))
          {
            Keyboard.set_modifier(M(MODIFIERKEY_CTRL));
            Keyboard.send_now();
            modifierCtrlPressed = true;
          }
          if (kpd.key[i].kchar == char(M(MODIFIERKEY_GUI)))
          {
            Keyboard.set_modifier(M(MODIFIERKEY_GUI));
            Keyboard.send_now();
            modifierGuiPressed = true;
          }
          if (kpd.key[i].kchar == char(M(MODIFIERKEY_ALT)))
          {
            if (modifierCtrlPressed)
            {
              Keyboard.set_modifier(MODIFIERKEY_CTRL | MODIFIERKEY_ALT);
              Keyboard.send_now();
            }
            else
            {
              Keyboard.set_modifier(M(MODIFIERKEY_ALT));
              Keyboard.send_now();
            }
            modifierAltPressed = true;
          }

          break;

        case HOLD:
          msg = " HOLD.";
          //Keyboard.set_modifier(M(MODIFIERKEY_LEFT_SHIFT));
          //Keyboard.send_now();

          Serial.print(msg);
          Keyboard.set_key1(kpd.key[i].kchar);
          Keyboard.send_now();

          break;
        case RELEASED:
          msg = " RELEASED.";
          Serial.println(msg);
          Keyboard.set_key1(0);
          Keyboard.send_now();
          if (kpd.key[i].kchar == char(M(MODIFIERKEY_LEFT_SHIFT)))
          {
            Serial.print("Shift Released ");
            Keyboard.set_modifier(0);
            Keyboard.send_now();
            modifierShiftLeftPressed = false;
          }
          if (kpd.key[i].kchar == char(M(MODIFIERKEY_RIGHT_SHIFT)))
          {
            Serial.print("Shift Released ");
            Keyboard.set_modifier(0);
            Keyboard.send_now();
            modifierShiftRightPressed = false;
          }
          if (kpd.key[i].kchar == char(M(MODIFIERKEY_CTRL)))
          {
            Keyboard.set_modifier(0);
            Keyboard.send_now();
            modifierCtrlPressed = false;
          }
          if (kpd.key[i].kchar == char(M(MODIFIERKEY_GUI)))
          {
            Keyboard.set_modifier(0);
            Keyboard.send_now();
            modifierGuiPressed = false;
          }
          if (kpd.key[i].kchar == char(M(MODIFIERKEY_ALT)))
          {
            Keyboard.set_modifier(0);
            Keyboard.send_now();
            modifierAltPressed = false;
          }

          break;
        case IDLE:
          msg = " IDLE.";
        }
      }
    }
  }
} // End9