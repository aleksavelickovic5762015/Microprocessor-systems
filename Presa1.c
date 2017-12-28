char txt[7];
#include <string.h>

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


Keypad_Init();                                 // Inicijalizacija tastature
char kp;                                       // Dugme tastature
int duzina=4;                                  // Cetri karaktera je duzina sifre
char unos[5];                                  // Sifra koja se sada unosi, trenutno..
int brPokusaja;
char korisnici[][10] = {"Petre","Milice", "Natasa"};
char sifre[][10] = {"1234\0","4321\0","ABCD\0"};
int k;
char keypadPort at PORTC;                      // Port C se koristi za tastaturu          

void displej(char ispis[4],int red,int kol) 
                                               // Resetovanje displeja
{
  Lcd_Init();                                  // Inicijalizuj displej

  Lcd_Cmd(_LCD_CLEAR);                         // Ocisti displej
  Lcd_Cmd(_LCD_CURSOR_OFF);                    // Iskljuci kursor
  Lcd_Out(red,kol,ispis);                      // Ispisi na displeju, na mestu (red,kol), string "ispis"
}


void tastatura(char unos[5],int duzina)        // Procedura za unos cetri karaktera sa tastature
{
 int brUnetih=0;
 displej("unesi sifru:",1,1);                  // Ispisuje na displej dati string
 while (brUnetih<duzina)                       // ...korisnik treba da unese sve karaktere, broj karaktera je "duzina"
 {
    kp = 0;                                    // Resetuj promenljivu kp
    do                                         // Cekaj da dugme tastature bude stisnuto i pusteno
      kp = Keypad_Key_Click();                 // Cuvaj poziciju stisnutog tastera tastature
    while (!kp);
    

    switch (kp)                                // Prevesti promenljivu kp u njenu ASCII vrednost
    {
      case 1: kp = 49; break; // 1
      case 2: kp = 50; break; // 2
      case 3: kp = 51; break; // 3
      case 4: kp = 65; break; // A
      case 5: kp = 52; break; // 4
      case 6: kp = 53; break; // 5
      case 7: kp = 54; break; // 6
      case 8: kp = 66; break; // B
      case 9: kp = 55; break; // 7
      case 10: kp = 56; break; // 8
      case 11: kp = 57; break; // 9
      case 12: kp = 67; break; // C
      case 13: kp = 42; break; // *
      case 14: kp = 48; break; // 0
      case 15: kp = 35; break; // #
      case 16: kp = 68; break; // D
      default:  kp = "V";
      break;
    }
     unos[brUnetih] = kp;                      // Cuva svaku od cetiri unete vrednosti
     Lcd_Out(1,13+brUnetih,"*");               // Na displeju se pojavljuje *, kada korisnik unese karakter
     //Lcd_Chr(1, 13+i, kp);                   // Na displej izlazi uneti karakter
     brUnetih++;
 }
 unos[4] = '\0';
}

sbit SMERI at LATD0_bit;
#define SMERU !PORTA.F0

#define TAST1 !PORTA.F1
#define TAST2 !PORTA.F2

#define gore !PORTA.F3
#define priVrhu !PORTA.F4
#define priDnu !PORTA.F5
#define dole !PORTE.F0

//int goreT, priVrhuT, priDnu, doleT;

sbit brzina1 at LATD1_bit;
sbit brzina2 at LATD2_bit;

int pb1, pb2;

int pustenTaster;

void main() 
{ 
  ADCON1 = 0x0F;                               // Svi pinovi su digitalni
  ADON_bit = 0;                                // Iskljuci AD konvertor
  CMCON  = 0x07;                               // Iskljuci Komparatore
  
  
  TRISD=0;
  PORTD=0;
  TRISA = 0xFF;                                // porta A su ulazna
  TRISE0_bit = 1;                              // postavi RE0 kao ulazni

  Delay_ms(200);                               // Cekaj da se stabilizuju promene..
  while(1)
  {
    brPokusaja=0;
    while(brPokusaja<3)                          // Tri pokusaja
    {
      tastatura(unos,duzina);                    // Procedura za unos cetri karaktera sa tastature
      k=0;
      while(k<3)                                 // Tri korisnika su registrovana
      {
        if(!strcmp(unos,sifre[k]))               // Da li neko od korisnika ima sifru "unos"?
        {
          displej("Dobrodosli",1,1);             // Sifra je tacna..
          Lcd_Out(1,11,korisnici[k]);            // ..Ispisuje ime korisnika sa datom sifrom
          Delay_ms(2000);
          break;                                 // ..Nema potrebe da prolazi kroz ostale sifre korisnika
        }
        k++;                                     // ..nije taj korisnik, mozda je drugi?
      }
      if (k<3) break;                            // korisnik se ispravno registrovao...
      else                                       // ..ako ne, jos pokusaja..
      {
        displej("netacan unos",1,1);
        Delay_ms(2000);
        brPokusaja++;
      }
    }
      if(k<3)                                    // Ako se korisnik registrovao..
      {
        pb1 = 2;
        pb2 = 2;
        while(1)
        {  

          SMERI = SMERU;
              
          if(TAST1 && TAST2)
          {
              if (pb1 != 2)
              {
                 brzina1 = pb1;
                 pb1 = 2;
                 if (brzina1 == 1)
                    displej("brzina 1",1,1);                 
              }
              if (pb2 != 2)
              {
                 brzina2 = pb2;
                 pb2 = 2;
                 if (brzina2 == 1)
                    displej("brzina 2",1,1);
              }
              if(gore)
              {
                  displej("Gore",1,1);
                  if(SMERI == 1)
                  {
                      brzina1 = 1;
                      Lcd_Out(2,1,"brzina 1      ");
                  }
                  else
                  {
                      brzina1 = 0;
                      Lcd_Out(2,1,"nema kretanja");
                  }
                  Delay_ms(10);
              }
              if(priVrhu)
              {
                  displej("Pri vrhu",1,1);
                  if(SMERI == 1)
                  {
                      brzina2 = 1;
                      brzina1 = 0;
                      Lcd_Out(2,1,"brzina 2      ");
                  }
                  else
                  {
                      brzina2 = 0;
                      brzina1 = 1;
                      Lcd_Out(2,1,"brzina 1      ");
                  }
                  Delay_ms(10);
              }
              if(priDnu)
              {
                  displej("Pri dnu",1,1);
                  if(SMERI == 1)
                  {
                      brzina2 = 0;
                      brzina1 = 1;
                      Lcd_Out(2,1,"brzina 1      ");
                  }
                  else   
                  {
                      brzina2 = 1;
                      brzina1 = 0;
                      Lcd_Out(2,1,"brzina 2      ");
                  }
                  Delay_ms(10);
              }
              if(dole)
              {
                  displej("Dole",1,1);
                  if(SMERI == 1)
                  {
                      brzina1 = 0;
                      Lcd_Out(2,1,"nema kretanja");
                  }
                  else
                  {
                      brzina1 = 1;
                      Lcd_Out(2,1,"brzina 1      ");
                  }
                  Delay_ms(10);               
              }
          }
          else
          {
              if(brzina1==1 || brzina2==1)
              {
                pb1 = brzina1;
                pb2 = brzina2;
                
                brzina1 = 0;
                brzina2 = 0;
                displej("pusteniTasteri",1,1);
                Lcd_Out(2,1,"nema kretanja");
              }
          }
        }
        //displej("izasao sam iz petlje",1,1); 
      }
      else
      {
       displej("SistemZakljucan",1,1);           // Ispunjena su tri pokusaja..
           
       Lcd_Out(2,14,"5");
       Delay_ms(1000);
       Lcd_Out(2,14,"4");
       Delay_ms(1000);
       Lcd_Out(2,14,"3");
       Delay_ms(1000);
       Lcd_Out(2,14,"2");
       Delay_ms(1000);
       Lcd_Out(2,14,"1");
       Delay_ms(1000);
      }
  }
  //kraj
}