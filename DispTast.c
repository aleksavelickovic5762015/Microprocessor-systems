unsigned short kp, cnt, oldstate = 0;
char txt[6];

// povezivanje tastature
char  keypadPort at PORTC;

// vezivanje bitova lcd-a za pinove porta
sbit LCD_RS at LATB4_bit;
sbit LCD_EN at LATB5_bit;
sbit LCD_D4 at LATB0_bit;
sbit LCD_D5 at LATB1_bit;
sbit LCD_D6 at LATB2_bit;
sbit LCD_D7 at LATB3_bit;

sbit LCD_RS_Direction at TRISB4_bit;
sbit LCD_EN_Direction at TRISB5_bit;
sbit LCD_D4_Direction at TRISB0_bit;
sbit LCD_D5_Direction at TRISB1_bit;
sbit LCD_D6_Direction at TRISB2_bit;
sbit LCD_D7_Direction at TRISB3_bit;

void main() {
  TRISD=0;
  
  cnt = 0;                                 // Reset counter
  Keypad_Init();                           // Initialize Keypad                              
  
  ADCON1 = 0x0F;                               // Svi pinovi su digitalni
  ADON_bit = 0;                                // Iskljuci AD konvertor
  CMCON  = 0x07;                               // Iskljuci Komparatore
  
  Lcd_Init();                              // inicijalizacija LCD
  Lcd_Cmd(_LCD_CLEAR);                     // Ocisti diplej
  Lcd_Cmd(_LCD_CURSOR_OFF);                // Iskljuci kursor
  Lcd_Out(1, 1, "1");
  Lcd_Out(1, 1, "Vrednost  :");                 // Ispisivanje poruke na lcd
  Lcd_Out(2, 1, "Broj puta :");

  do {
    kp = 0;                                // Resetovanje promenljive za tastaturu

    do
      // kp = Keypad_Key_Press();          // Sacuvaj stisnutu vrednost u promenljivu
      kp = Keypad_Key_Click();             // Sacuvaj stisnutu vrednost u promenljivu
    while (!kp);
   // Pretvori u ASCII vrednost
    switch (kp) {

      case  1: kp = 49; break; // 1
      case  2: kp = 50; break; // 2
      case  3: kp = 51; break; // 3
      case  4: kp = 65; break; // A
      case  5: kp = 52; break; // 4
      case  6: kp = 53; break; // 5
      case  7: kp = 54; break; // 6
      case  8: kp = 66; break; // B        
      case  9: kp = 55; break; // 7
      case 10: kp = 56; break; // 8
      case 11: kp = 57; break; // 9
      case 12: kp = 67; break; // C
      case 13: kp = 42; break; // *
      case 14: kp = 48; break; // 0
      case 15: kp = 35; break; // #
      case 16: kp = 68; break; // D

    }

    if (kp != oldstate) {
      cnt = 1;
      oldstate = kp;
      }
    else {
      cnt++;
      }
        
    Lcd_Chr(1, 13, kp);

    if (cnt == 255) {
      cnt = 0;
      }

    ByteToStr(cnt, txt);
    Lcd_Out(2, 12, txt);
  } while (1);
} 