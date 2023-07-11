//12th July 2023 Okubo Heavy Industries
//QtPy Falcon targeting computer
//Based on 3D Cube by Colin Ord

#include <U8g2lib.h>

#define N_width  7
#define N_height 7
static unsigned char N_bits[] = {
 0x88,0x88,0x9c,0x9c,0xbe,0xb6,0xa2};

#define S_width  7
#define S_height 7
static unsigned char S_bits[] = {
 0xa2,0xb6,0xbe,0x9c,0x9c,0x88,0x88};

#define E_width  7
#define E_height 7
static unsigned char E_bits[] = {
 0x80,0x87,0x9e,0xfc,0x9e,0x87,0x80};

#define W_width  7
#define W_height 7
static unsigned char W_bits[] = {
 0x80,0xf0,0xbc,0x9f,0xbc,0xf0,0x80};

#define NE_width  7
#define NE_height 7
static unsigned char NE_bits[] = {
 0xc0,0xb0,0xbc,0x9f,0x9f,0x8c,0x8c};

#define NW_width  7
#define NW_height 7
static unsigned char NW_bits[] = {
 0x81,0x86,0x9e,0xfc,0xfc,0x98,0x98};

#define SE_width  7
#define SE_height 7
static unsigned char SE_bits[] = {
 0x8c,0x8c,0x9f,0x9f,0xbc,0xb0,0xc0};

#define SW_width  7
#define SW_height 7
static unsigned char SW_bits[] = {
 0x98,0x98,0xfc,0xfc,0x9e,0x86,0x81};


//The following line will need changing depending on your board type!
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

float tx, nx, p;
float ty, ny, py;
float rot, rotx, roty, rotz, rotxx, rotyy, rotzz, rotxxx, rotyyy, rotzzz;
int i; //0 to 360
int fl, scale; //focal length
int wireframe[49][3];
float scalefactor = 0.7;

int point[3][2];
int point2[3][2];
int j;
int k;
//float p;
float t;
int bezierx;
int bezierx2;
int x=0;
int y=0;

int xold = 0;
int yold = 0;

int x2=0;
int y2=0;

int xold2 = 0;
int yold2 = 0;


int originx = 64;
int originy = 32; //32

int front_depth = 12;
int back_depth = -12;

//Store grid vertices
int grid_vertex[49][3] = {
 { -25, -25, front_depth},
 {25, -25, front_depth},
 {25, 25, front_depth},
 { -25, 25, front_depth},
 { -25, -25, back_depth},
 {25, -25, back_depth},
 {25, 25, back_depth},
 { -25, 25, back_depth},//7

 { -15, -25, front_depth},//8
 {15, -25, front_depth},
 {25, -15, front_depth},
 { 25, 15, front_depth},
 { 15, 25, front_depth},
 {-15, 25, front_depth},//13
 {-25, 15, front_depth},
 { -25, -15, front_depth},
 {-25, 0, front_depth},//16
 { 25, 0, front_depth},//17
 {0, -25, front_depth},//18
 { 0, 25, front_depth},//19


 { -15, -25, back_depth},//15
 {15, -25, back_depth},
 {25, -15, back_depth},
 { 25, 15, back_depth},
 { 15, 25, back_depth},
 {-15, 25, back_depth},
 {-25, 15, back_depth},
 { -25, -15, back_depth},
 {-25, 0, back_depth},//28
 { 25, 0, back_depth},//29
 {0, -25, back_depth},//30
 { 0, 25, back_depth},//31

 { -5, -25, front_depth},//32
 {5, -25, front_depth},
 {25, -5, front_depth},
 { 25, 5, front_depth},
 { 5, 25, front_depth},
 {-5, 25, front_depth},//37
 {-25, 5, front_depth},
 { -25, -5, front_depth},

 { -5, -25, back_depth},//40
 {5, -25, back_depth},
 {25, -5, back_depth},
 { 25, 5, back_depth},
 { 5, 25, back_depth},
 {-5, 25, back_depth},
 {-25, 5, back_depth},
 { -25, -5, back_depth},

 
 {00, 00, 00}, //centre point

 

};

int fd = 0; //0=orthographic

void setup(void)
{
 pinMode(A1, INPUT);
 randomSeed(analogRead(A1));  
 u8g2.begin();

 //Clear Screen
 u8g2.firstPage();
 do {
 // empty screen
 } while ( u8g2.nextPage() );
}

void draw_vertices(void)
{
 u8g2.drawPixel (rotxxx, rotyyy);
}

void draw_wireframe(void)
{

 u8g2.drawLine(wireframe[0][0], wireframe[0][1], wireframe[1][0], wireframe[1][1]);
 u8g2.drawLine(wireframe[1][0], wireframe[1][1], wireframe[2][0], wireframe[2][1]);
 u8g2.drawLine(wireframe[2][0], wireframe[2][1], wireframe[3][0], wireframe[3][1]);
 u8g2.drawLine(wireframe[3][0], wireframe[3][1], wireframe[0][0], wireframe[0][1]);

 u8g2.drawLine(wireframe[4][0], wireframe[4][1], wireframe[5][0], wireframe[5][1]);
 u8g2.drawLine(wireframe[5][0], wireframe[5][1], wireframe[6][0], wireframe[6][1]);
 u8g2.drawLine(wireframe[6][0], wireframe[6][1], wireframe[7][0], wireframe[7][1]);
 u8g2.drawLine(wireframe[7][0], wireframe[7][1], wireframe[4][0], wireframe[4][1]);

 u8g2.drawLine(wireframe[8][0], wireframe[8][1], wireframe[13][0], wireframe[13][1]);
 u8g2.drawLine(wireframe[9][0], wireframe[9][1], wireframe[12][0], wireframe[12][1]);
 u8g2.drawLine(wireframe[10][0], wireframe[10][1], wireframe[15][0], wireframe[15][1]);
 u8g2.drawLine(wireframe[11][0], wireframe[11][1], wireframe[14][0], wireframe[14][1]);

 u8g2.drawLine(wireframe[20][0], wireframe[20][1], wireframe[25][0], wireframe[25][1]);
 u8g2.drawLine(wireframe[21][0], wireframe[21][1], wireframe[24][0], wireframe[24][1]);
 u8g2.drawLine(wireframe[22][0], wireframe[22][1], wireframe[27][0], wireframe[27][1]);
 u8g2.drawLine(wireframe[23][0], wireframe[23][1], wireframe[26][0], wireframe[26][1]);

 u8g2.drawLine(wireframe[32][0], wireframe[32][1], wireframe[37][0], wireframe[37][1]);
 u8g2.drawLine(wireframe[33][0], wireframe[33][1], wireframe[36][0], wireframe[36][1]);
 u8g2.drawLine(wireframe[34][0], wireframe[34][1], wireframe[39][0], wireframe[39][1]);
 u8g2.drawLine(wireframe[35][0], wireframe[35][1], wireframe[38][0], wireframe[38][1]);

 u8g2.drawLine(wireframe[40][0], wireframe[40][1], wireframe[45][0], wireframe[45][1]);
 u8g2.drawLine(wireframe[41][0], wireframe[41][1], wireframe[44][0], wireframe[44][1]);
 u8g2.drawLine(wireframe[42][0], wireframe[42][1], wireframe[47][0], wireframe[47][1]);
 u8g2.drawLine(wireframe[43][0], wireframe[43][1], wireframe[46][0], wireframe[46][1]);

u8g2.drawEllipse(wireframe[48][0], wireframe[48][1],9,7);

}

void loop(void)
{
  


 
  populatebeziers();  
  t=0;

  //picture loop
 
 for (int angle = 0; angle <= 360; angle = angle + 3) {
  t=t+0.02;
  u8g2.firstPage();
  
  do {
    
   for (int i = 0; i < 49; i++) {

     rot = angle * 0.0174532; //0.0174532 = one degree
    //rotateY
    rotz = grid_vertex[i][2] * cos(rot) - grid_vertex[i][0] * sin(rot);
    rotx = grid_vertex[i][2] * sin(rot) + grid_vertex[i][0] * cos(rot);
    roty = grid_vertex[i][1];
    //rotateX
    rotyy = roty * cos(rot) - rotz * sin(rot);
    rotzz = roty * sin(rot) + rotz * cos(rot);
    rotxx = rotx;
    //rotateZ
    rotxxx = rotxx * cos(rot) - rotyy * sin(rot);
    rotyyy = rotxx * sin(rot) + rotyy * cos(rot);
    rotzzz = rotzz;

    //orthographic projection
    rotxxx = (rotxxx) * scalefactor + originx;
    rotyyy = (rotyyy) * scalefactor + originy;

    //store new vertices values for wireframe drawing
    wireframe[i][0] = rotxxx;
    wireframe[i][1] = rotyyy;
    wireframe[i][2] = rotzzz;

     //draw_vertices();
   

   }
    //calculate bezier curves
    p = (1 - t);
    x = ((sq(p)*point[0][0]) + (2*(p)*t*point[1][0]) + (sq(t)*point[2][0]));
    y = ((sq(p)*point[0][1]) + (2*(p)*t*point[1][1]) + (sq(t)*point[2][1]));
    x2 = ((sq(p)*point2[0][0]) + (2*(p)*t*point2[1][0]) + (sq(t)*point2[2][0]));
    y2 = ((sq(p)*point2[0][1]) + (2*(p)*t*point2[1][1]) + (sq(t)*point2[2][1]));
    
    if (x > xold && y > yold){
     u8g2.drawXBM( x, y, 7, 7, SE_bits);
    }
    if (x < xold && y < yold){
     u8g2.drawXBM( x, y, 7, 7, NW_bits);
    }
    if (x < xold && y > yold){
     u8g2.drawXBM( x, y, 7, 7, SW_bits);
    }
    if (x > xold && y < yold){
     u8g2.drawXBM( x, y, 7, 7, NE_bits);
    }
    if (x < xold && y == yold){
      u8g2.drawXBM( x, y, 7, 7, W_bits);
    }
    if (x > xold && y == yold){
     u8g2.drawXBM( x, y, 7, 7, E_bits);
    }
    if (x == xold && y < yold){
     u8g2.drawXBM( x, y, 7, 7, N_bits);
    }
    if (x== xold && y > yold){
     u8g2.drawXBM( x, y, 7, 7, S_bits);
    }

    if (x2 > xold2 && y2 > yold2){
     u8g2.drawXBM( x2, y2, 7, 7, SE_bits);
    }
    if (x2 < xold2 && y2 < yold2){
     u8g2.drawXBM( x2, y2, 7, 7, NW_bits);
    }
    if (x2 < xold2 && y2 > yold2){
     u8g2.drawXBM( x2, y2, 7, 7, SW_bits);
    }
    if (x2 > xold2 && y2 < yold2){
     u8g2.drawXBM( x2, y2, 7, 7, NE_bits);
    }
    if (x2 < xold2 && y2 == yold2){
     u8g2.drawXBM( x2, y2, 7, 7, W_bits);
    }
    if (x2 > xold2 && y2 == yold2){
     u8g2.drawXBM( x2, y2, 7, 7, E_bits);
    }
    if (x2 == xold2 && y2 < yold2){
     u8g2.drawXBM( x2, y2, 7, 7, N_bits);
    }
    if (x2== xold2 && y2 > yold2){
     u8g2.drawXBM( x2, y2, 7, 7, S_bits);
    }

    xold = x;
    yold = y;

    xold2 = x2;
    yold2 = y2;

    

    draw_wireframe();


   } while (u8g2.nextPage());
 }
}

void populatebeziers(){

for (j=0; j<3; j++){
  bezierx = random(0,100);
  if (bezierx < 25){
    point[j][0]= -10;
    point[j][1] = random(-50,95);
  }
  else if (bezierx < 50){
    point[j][0]=  138;
    point[j][1] = random(-50,95);
  }
  else if (bezierx < 75){
    point[j][0]=  random(-50,189);;
    point[j][1] = -10;
  }
  else {
    point[j][0]=  random(-50,189);;
    point[j][1] = 74;
  }

}

for (k=0; k<3; k++){
  bezierx2 = random(0,100);
  if (bezierx2 < 25){
    point2[k][0]= -10;
    point2[k][1] = random(-50,105);
  }
  else if (bezierx2 < 50){
    point2[k][0]=  138;
    point2[k][1] = random(-50,105);
  }
  else if (bezierx2 < 75){
    point2[k][0]=  random(-50,189);;
    point2[k][1] = -10;
  }
  else {
    point2[k][0]=  random(-50,189);;
    point2[k][1] = 74;
  }
}
}
