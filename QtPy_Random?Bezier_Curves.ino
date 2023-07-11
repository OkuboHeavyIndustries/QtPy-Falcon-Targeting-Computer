#include <U8g2lib.h>

//The following line will need changing depending on your board type!
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

int point[3][2];

int x;
int y;

void setup() {
  // put your setup code here, to run once:
u8g2.begin();

//Clear Screen
 u8g2.firstPage();
 do {
// empty screen
 } while ( u8g2.nextPage() );
 u8g2.setBitmapMode(1);
}

void loop(void)
{
  u8g2.firstPage();
  for (int j=0; j<3; j++){
  point[j][0]= random(0,129);
  point[j][1] = random(0,65);
 
  u8g2.drawDisc(point[j][0], point[j][1], 3);
  }

  for (float t=0; t<1; t=t+0.02){
  
  float p = (1 - t);

  x = ((sq(p)*point[0][0]) + (2*(p)*t*point[1][0]) + (sq(t)*point[2][0]));
  y = ((sq(p)*point[0][1])+ (2*(p)*t*point[1][1]) + (sq(t)*point[2][1]));

  u8g2.drawPixel(x,y);

  }
  u8g2.nextPage();
  delay(1000);
}
