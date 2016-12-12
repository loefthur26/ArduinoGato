#include <SPFD5408_Adafruit_GFX.h>    // Libreria para pintar sobre la pantalla
#include <SPFD5408_Adafruit_TFTLCD.h> // LIbraria donde se definen caracteristicas de las pantallas
#include <SPFD5408_TouchScreen.h>     // Libreria para poder reconocer el sensor Touch de las pantallas LCD

#define YP A3 
#define XM A2
#define YM 9
#define XP 8

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 320);

#define LCD_CS A3
#define LCD_CD A2
#define LCD_WR A1
#define LCD_RD A0
#define LCD_RESET A4  // Boton de reset que está definido al pin analógico 4 de la placa

// Asignacion de nombres de los colores de 16-bits
#define NEGRO     0x0000
#define AZUL      0x002F
#define ROJO      0xF800
#define VERDE     0x07E0
#define CYAN      0x07FF
#define MAGENTA   0xF81F
#define AMARILLO  0xFFE0
#define BLANCO    0xFFFF

Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);

#define MARCAX 1
#define MARCAO 0
#define VACIO -1
#define EMPATE -2

#define TS_MINX 125
#define TS_MAXX 965

#define TS_MINY 85
#define TS_MAXY 905

#define MINPRESSURE 10
#define MAXPRESSURE 1000

int Tablero[10];
int tocaTurno = MARCAO;

void setup(void) {
  tft.reset();
  tft.begin(0x9341);    // Codigo de identificacion, este depende del modelo de la pantalla.
  tft.setRotation(0);   // se determina la orientación de la pantalla
  TSPoint p = ts.getPoint();
  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);
  ReiniciarJuego();
  }

void loop() {

  TSPoint p = ts.getPoint();
  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);
  // a point object holds x y and z coordinates 
  p.x = map(p.x, TS_MINX, TS_MAXX, 0, tft.width());
  p.y = map(p.y, TS_MINY, TS_MAXY, 0, tft.height());
 

  if (p.z > MINPRESSURE && p.z < MAXPRESSURE) {
    //p.x;
    //p.y;
    //p.z;
     
    int pos = getPosicion(p.x,p.y);
    if (Tablero[pos] == -1) {
      if (tocaTurno == MARCAO ) {
        pintarO(pos);
        Tablero[pos] = tocaTurno;
        tocaTurno = MARCAX;
      } else {
        pintarX(pos);
        Tablero[pos] = tocaTurno;
        tocaTurno = MARCAO;
      }
      
      int ganador = revGanador();
      if (ganador != VACIO) {
        if (ganador == MARCAX) {
          tft.setTextColor(BLANCO);
          tft.setTextSize(6);
          tft.setCursor(15 , 140);
          tft.print("GANA X");
          } 
        if (ganador == MARCAO) { 
          tft.setTextColor(BLANCO);
          tft.setTextSize(6);
          tft.setCursor(15 , 140);
          tft.print("GANA O");
          } 
        if (ganador == EMPATE) {
          tft.setTextColor(BLANCO);
          tft.setTextSize(6);
          tft.setCursor(15 , 140);
          tft.print("EMPATE");
          }
        delay(2000);
        ReiniciarJuego();
      }
    }   
   //  Tft.fillCircle(p.x,p.y,2,VERDE);
   }
  }

int revGanador() {

  int ganador = VACIO;
  //Surely there is a better way?
  
  //Horizontal
  if (Tablero[1] == Tablero[2] && Tablero[1] == Tablero[3] && ganador == VACIO) {ganador=Tablero[1]; }
  if (Tablero[4] == Tablero[5] && Tablero[4] == Tablero[6] && ganador == VACIO) {ganador=Tablero[4]; }
  if (Tablero[7] == Tablero[8] && Tablero[7] == Tablero[9] && ganador == VACIO) {ganador=Tablero[7]; }  

  //Vertical
  if (Tablero[1] == Tablero[4] && Tablero[1] == Tablero[7] && ganador == VACIO) {ganador=Tablero[1]; }
  if (Tablero[2] == Tablero[5] && Tablero[2] == Tablero[8] && ganador == VACIO) {ganador=Tablero[2]; }
  if (Tablero[3] == Tablero[6] && Tablero[3] == Tablero[9] && ganador == VACIO) {ganador=Tablero[3]; }

  //Diagonal
  if (Tablero[1] == Tablero[5] && Tablero[1] == Tablero[9] && ganador == VACIO) {ganador=Tablero[1]; }
  if (Tablero[3] == Tablero[5] && Tablero[3] == Tablero[7] && ganador == VACIO) {ganador=Tablero[3]; }
  
  // Al final se verifica que todos los espacios están ocupados. Si lo están, es un empate 
  if (ganador == VACIO && Tablero[1] > -1 && Tablero[2] > -1 && Tablero[3] > -1 && Tablero[4] > -1 && Tablero[5] > -1 && Tablero[6] > -1 && Tablero[7] > -1 && Tablero[8] > -1 && Tablero[9] > -1 ) { ganador = EMPATE; }
  
  return ganador;
  }

void ReiniciarJuego() {
  pintarTablero(); // Pinta el tablero de nuevo
  for (int i=0;i<10;i++) { Tablero[i] = VACIO; }
  }

void pintarTablero() {
  // Primero se dibuja el tablero de juego sobre la pantalla en blanco
  // Por lo que se pinta el fondo de color negro
  tft.fillRect(0,0,239,319,NEGRO);
  // Los bloques tienen 80px por eje 'x'
  // Los bloques tienen 106px por eje 'y'
  // Las 'x' y 'y' son 75% del ancho del bloque.  
  tft.drawLine(0,106,239,106,VERDE);
  tft.drawLine(0,212,239,212,VERDE);
  tft.drawLine(80,0,80,319,VERDE);
  tft.drawLine(160,0,160,319,VERDE);
  }

int getPosicion(int x, int y) {
  int row =0;
  int col =0;

  col = x/80;
  row = y/106;

  int pos = row * 3 + col +1 ;
  return pos;
  }

void pintarX(int pos) {
  // Dibujo una X en la pantalla
  tft.drawLine(getCentrorX(pos)-30,getCentrorY(pos)-30, getCentrorX(pos)+30, getCentrorY(pos)+30,ROJO);
  tft.drawLine(getCentrorX(pos)+30,getCentrorY(pos)-30, getCentrorX(pos)-30, getCentrorY(pos)+30,ROJO);
  }

void pintarO(int pos) { 
  // Dibuja una O en la pantalla
  tft.drawCircle(getCentrorX(pos),getCentrorY(pos), 30, ROJO);
  }


int getCentrorX( int pos ) {
  // Devuelve la posición central rn el eje 'x' para un punto determinado en el tablero.
  if (pos == 1) return 40;
  if (pos == 4) return 40;
  if (pos == 7) return 40;
  if (pos == 2) return 120;
  if (pos == 5) return 120;
  if (pos == 8) return 120;
  if (pos == 3) return 200;
  if (pos == 6) return 200;
  if (pos == 9) return 200;
  }

int getCentrorY ( int pos ) {
  // Devuelve la posición central rn el eje 'y' para un punto determinado en el tablero.
  if (pos == 1) return 53;
  if (pos == 2) return 53;
  if (pos == 3) return 53;
  if (pos == 4) return 159;
  if (pos == 5) return 159;
  if (pos == 6) return 159;
  if (pos == 7) return 265;
  if (pos == 8) return 265;
  if (pos == 9) return 265;
  }

