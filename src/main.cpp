/*
 * Zenith Supersport 286 Keyboard interpreter 
 */

#include <Keypad.h>

const byte ROWS = 6;
const byte COLS = 16;
#ifdef M
#undef M
#endif
#define M(n) ((n)&KEYCODE_MASK)

char keys[COLS][ROWS] = {

    {'H', M(KEY_TILDE), M(KEY_TAB), M(KEY_CAPS_LOCK), M(MODIFIERKEY_LEFT_SHIFT), M(MODIFIERKEY_LEFT_CTRL)},
    {M(KEY_F1), M(KEY_1), M(KEY_Q), 0, 0, M(MODIFIERKEY_GUI)},
    {M(KEY_F2), M(KEY_2), M(KEY_W), M(KEY_A), 0, 0},
    {M(KEY_F3), M(KEY_3), M(KEY_E), M(KEY_S), M(KEY_Z), M(KEY_LEFT_ALT)},
    {M(KEY_F4), M(KEY_4), M(KEY_R), M(KEY_D), M(KEY_X), 0},
    {M(KEY_F5), M(KEY_5), M(KEY_T), M(KEY_F), M(KEY_C), 0},
    {M(KEY_F6), M(KEY_6), M(KEY_Y), M(KEY_G), M(KEY_V), 0},
    {M(KEY_F7), M(KEY_7), M(KEY_U), M(KEY_H), M(KEY_B), M(KEY_SPACE)},
    {M(KEY_F8), M(KEY_8), M(KEY_I), M(KEY_J), M(KEY_N), 0},
    {M(KEY_F9), M(KEY_9), M(KEY_O), M(KEY_K), M(KEY_M), 0},
    {M(KEY_F10), M(KEY_0), M(KEY_L), M(KEY_COMMA), M(MODIFIERKEY_RIGHT_ALT)},
    {M(KEY_F11), M(KEY_MINUS), M(KEY_P), M(KEY_SEMICOLON), M(KEY_PERIOD), 0},
    {M(KEY_F12), M(KEY_EQUAL), M(KEY_LEFT_BRACE), M(KEY_QUOTE), M(KEY_SLASH), M(MODIFIERKEY_RIGHT_CTRL)},
    {M(KEY_DELETE), 0, M(KEY_RIGHT_BRACE), M(KEY_BACKSLASH), M(MODIFIERKEY_RIGHT_SHIFT), M(KEY_LEFT)},
    {M(KEY_PAUSE), M(KEY_BACKSPACE), 0, M(KEY_ENTER), M(KEY_UP), M(KEY_DOWN)},
    {M(KEY_PRINTSCREEN), M(KEY_HOME), M(KEY_PAGE_UP), M(KEY_PAGE_DOWN), M(KEY_END), M(KEY_RIGHT)}

};

byte colPins[COLS] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 14, 15, 16}; //connect to the row pinouts of the keypad
byte rowPins[ROWS] = {22, 20, 21, 19, 18, 17};                               //connect to the column pinouts of the keypad

Keypad kpd = Keypad(makeKeymap(keys), colPins, rowPins, COLS, ROWS);

unsigned long loopCount;
unsigned long startTime;
String msg;
int x = 0;

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

  if (kpd.getKeys())
  {
    Serial.println("key: --" + kpd.getKey());
  }

  // Fills kpd.key[ ] array with up-to 10 active keys.
  // Returns true if there are ANY active keys.

} //