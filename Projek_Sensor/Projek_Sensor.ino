#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>

SoftwareSerial ArduinoUno (7, 6);
int i, j, k;

LiquidCrystal_I2C lcd(0x27,16,2);//adrressing lcd 0x27,16X2 lcd

//library disertakan di deskripsi
#define MQ2_sensor A0 //definisi variabel
#define MQ2RL 5000 //nilai RL =10 pada sensor
#define MQ2m -0.385 //hasil perhitungan gradien
#define MQ2b 1.396 //hasil perhitungan perpotongan
#define MQ2Ro 220 //hasil pengukuran RO

#define MQ7_sensor A1 //definisi variabel
#define MQ7RL 10000 //nilai RL =10 pada sensor
#define MQ7m -0.653 //hasil perhitungan gradien
#define MQ7b 1.275 //hasil perhitungan perpotongan
#define MQ7Ro 1760 //hasil pengukuran RO

#define MQ8_sensor A2 //definisi variabel
#define MQ8RL 10000 //nilai RL =10 pada sensor
#define MQ8m -1.329 //hasil perhitungan gradien
#define MQ8b 5.338 //hasil perhitungan perpotongan
#define MQ8Ro 460 //hasil pengukuran RO

const int numReadings0 = 5;//nilai penambilan sample pembacaan sebesar 5 kali
const int numReadings1 = 5;
const int numReadings2 = 5;
float readings0[numReadings0];
float readings1[numReadings1];  
float readings2[numReadings2];    
int readIndex0 = 0;  
int readIndex1 = 0;
int readIndex2 = 0;           
float total0 = 0;
float total1 = 0; 
float total2 = 0;               
float average0 = 0;      
float average1 = 0;
float average2 = 0;          

void setup() {
  Serial.begin(115200);
  ArduinoUno.begin(9600);
  lcd.init();
  lcd.backlight();
   for (int thisReading0 = 0; thisReading0 < numReadings0; thisReading0++) {
    readings0[thisReading0] = 0; }
    for (int thisReading1 = 0; thisReading1< numReadings1; thisReading1++) {
    readings1[thisReading1] = 0; }
    for (int thisReading2 = 0; thisReading2 < numReadings2; thisReading2++) {
    readings2[thisReading2] = 0;
  }
for (int times = 1; times <=10; times++) {
    lcd.setCursor(0,0);
    lcd.print("  PRE HEATING");
    lcd.setCursor(3,1);
    lcd.print(times);
    lcd.print(" Seconds");
    delay(1000);
  }
  lcd.clear();

}

void loop() {

  float VRL0; 
  float VRL1;
  float VRL2;
  float RS0;  
  float RS1;
  float RS2;
  float ratio0; 
  float ratio1;
  float ratio2;

  // MQ 2 -----------------------------------
   
  VRL0 = analogRead(MQ2_sensor)*(5/1023.0); //konversi analog ke tegangan
  RS0 =(5.0 /VRL0-1)* 5000 ;//rumus untuk RS
  ratio0 = RS0/MQ2Ro;  // rumus mencari ratio
  float ppm0 = pow(10, ((log10(ratio0)-MQ2b)/MQ2m));//rumus mencari ppm
  
  total0 = total0 - readings0[readIndex0];
  readings0[readIndex0] = ppm0;
  total0 = total0 + readings0[readIndex0];
  readIndex0 = readIndex0 + 1;
  if (readIndex0 >= numReadings0) {
    readIndex0 = 0;
  }
  average0 = total0 / numReadings0;

  // MQ 7 ----------------------------------

  VRL1 = analogRead(MQ7_sensor)*(5/1023.0); //konversi analog ke tegangan
  RS1 =(5.0 /VRL1-1)* 10000 ;//rumus untuk RS
  ratio1 = RS1/MQ7Ro;  // rumus mencari ratio
  float ppm1 = pow(10, ((log10(ratio1)-MQ7b)/MQ7m));//rumus mencari ppm
  
  total1 = total1 - readings1[readIndex1];
  readings1[readIndex1] = ppm1;
  total1 = total1 + readings1[readIndex1];
  readIndex1 = readIndex1 + 1;
  if (readIndex1 >= numReadings1) {
    readIndex1 = 0;
  }
  average1 = total1 / numReadings1;

  // MQ 8 -------------------------------------

    VRL2 = analogRead(MQ8_sensor)*(5/1023.0);         //konversi analog ke tegangan
    RS2 =(5.0 /VRL2-1)* 10000 ;                       //rumus untuk RS
    ratio2 = RS2/MQ8Ro;                               // rumus mencari ratio
    float ppm2 = pow(10, ((log10(ratio2)-MQ8b)/MQ8m));//rumus mencari ppm
  
    total2 = total2 - readings2[readIndex2];
    readings2[readIndex2] = ppm2;
    total2 = total2 + readings2[readIndex2];
    readIndex2 = readIndex2 + 1;
    if (readIndex2 >= numReadings2) {
      readIndex2 = 0;
      }
    average2 = total2 / numReadings2;
    
  lcd.setCursor(0,0);
  lcd.print("CH4:");
  //menampilkan nilai rata-rata ppm setelah 5 kali pembacaan
  lcd.setCursor(6,0);
  lcd.print(average0);
  lcd.print(" PPM");
  lcd.print("  ");
  delay(1000);
  
  lcd.setCursor(0,1);
  lcd.print("CO :");
  //menampilkan nilai rata-rata ppm setelah 5 kali pembacaan
  lcd.setCursor(6,1);
  lcd.print(average1);
  lcd.print(" PPM");
  lcd.print("  ");
  delay(500);

  lcd.setCursor(0,0);
  lcd.print("H2 :");
  //menampilkan nilai rata-rata ppm setelah 5 kali pembacaan
  lcd.setCursor(6,0);
  lcd.print(average2);
  lcd.print(" PPM");
  lcd.print("  ");
  delay(1000);
  
// Komunikasi Serial ke NodeMCU

  i = average0 + 2000 ;
  ArduinoUno.print(i);
  ArduinoUno.println("\n");
  Serial.println(i);

  j = average1 + 4000 ;
  ArduinoUno.print(j);
  ArduinoUno.println("\n");
  Serial.println(j);

  k = average2 + 6000 ;
  ArduinoUno.print(k);
  ArduinoUno.println("\n");
  Serial.println(k);
  delay(1000);

  }
