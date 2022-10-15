#include <Arduino.h>
#include <Servo.h>
#include <TimerOne.h>

Servo servoX;
Servo servoY;
Servo servoZ;

String data;
String get_data[3];
byte a;
int stringData;
String per_data;
int index = 0;

// Tentukan pin servo
int X = 9;
int Y = 10;

int x = 0;
int y = 0;
int z = 0;

// Arah servo
int up = 70;  // Kanan
int dw = 110; // Kiri
int st = 90;  // Stop

// Pertama ambil nilaiAll [] = {x, y ,z}
// Cek Apakah ada minus?
// Jika ada, x misalnya, buat x jadi 0, nilaiPlus [] = {0, y, z}
// Isi x ke nilaiMinus [] = {x, 0, 0}

// pada nilaiPlus, cek nilai terbesarDariPlus, terbesarDariPlusPlus = ?
// pada nilaiMinus, cek nilai terkecilDariMinus, terkecilDariMinusMinus = ?

// masuk ke program utama
// Jika nilaiplus, kurangi berapa langkah didalam for
//

void runServo(int valX, int valY, int valZ)
{
  // Ambil nilai
  int nilaiAll[] = {valX, valY, valZ};

  int nilaiPlus[] = {0, 0, 0};
  int nilaiMinus[] = {0, 0, 0};

  // Tampilkan data dari serial
  Serial.println("Nilai : [" + String(nilaiAll[0]) + "," + String(nilaiAll[1]) + "," + String(nilaiAll[2]) + "]");

  // Cek apakah ada nilai minus
  for (index = 0; index < 3; index++)
  {
    if (nilaiAll[index] > 0)
    {
      nilaiPlus[index] = nilaiAll[index];
      nilaiMinus[index] = 0;
    }

    else if (nilaiAll[index] < 0)
    {
      nilaiMinus[index] = nilaiAll[index];
      nilaiPlus[index] = 0;
    }
  }

  // Lihat nilai dalam array di serial monitor
  Serial.println("|--> Plus : [" + String(nilaiPlus[0]) + "," + String(nilaiPlus[1]) + "," + String(nilaiPlus[2]) + "]");
  Serial.println("|--> Minus: [" + String(nilaiMinus[0]) + "," + String(nilaiMinus[1]) + "," + String(nilaiMinus[2]) + "]");

  // Cek nilai terbesarDariPlus
  int terbesarDariPlus = 0;

  for (index = 0; index < 3; index++)
  {
    if (nilaiPlus[index] > terbesarDariPlus)
    {
      terbesarDariPlus = nilaiPlus[index];
    }
  }

  Serial.println("     |--> TerbesarDariPlus : " + String(terbesarDariPlus));

  // Cek nilai terkecilDariMinus
  int terkecilDariMinus = 0;

  for (index = 0; index < 3; index++)
  {
    if (nilaiMinus[index] < terkecilDariMinus)
    {
      terkecilDariMinus = nilaiMinus[index];
    }
  }

  Serial.println("     |--> TerkecilDariMinus: " + String(terkecilDariMinus));

  // Lihat berapa tinggi nilai
  terkecilDariMinus = terkecilDariMinus * -1;

  int langkah = 0;

  if (terbesarDariPlus >= terkecilDariMinus)
  {
    langkah = terbesarDariPlus;
  }
  else
    langkah = terkecilDariMinus;

  Serial.println("          |--> " + String(langkah) + " Langkah");

  int hitValX = valX;
  int hitValY = valY;
  int hitValZ = valZ;

  for (int y = 1; y <= langkah; y++)
  {
    for (int x = 0; x < 1; x++)
    {

      // Cek apakah harus putar kekanan?
      if (hitValX >= 1)
      {
        Serial.println("X Kanan");
        hitValX--;
        servoX.write(up);
      }

      if (hitValY >= 1)
      {
        Serial.println("Y Kanan");
        hitValY--;
        servoY.write(up);
      }

      if (hitValZ >= 1)
      {
        Serial.println("Z Kanan");
        hitValZ--;
        servoZ.write(up);
      }

      // Cek apakah harus putar ke kiri?
      if (hitValX <= -1)
      {
        Serial.println("X Kiri");
        hitValX++;
        servoX.write(dw);
      }

      if (hitValY <= -1)
      {
        Serial.println("Y Kiri");
        hitValY++;
        servoY.write(dw);
      }

      if (hitValZ <= -1)
      {
        Serial.println("Z Kiri");
        hitValZ++;
        servoZ.write(dw);
      }

      // Jangan ganti nilai dari 8500, ini untuk delay
      delayMicroseconds(8500);

      servoX.write(st);
      servoY.write(st);
      servoZ.write(st);
    }
  }
  // // If the val is minus, turn to left
  // if (valX < 0)
  // {
  //   Serial.print("X: " + String(valX));
  //   for (int y = valX; y <= 0; y++)
  //   {
  //     for (int x = 0; x <= 1; x++)
  //     {
  //       servoX.write(dw);
  //       delayMicroseconds(8500);
  //       servoX.write(st);
  //     }
  //   }
  // }
  Serial.println("");
}

String split(String data, char character, int pos)
{
  stringData = 0;
  per_data = "";

  for (int i = 0; i < data.length() - 1; i++)
  {

    if (data[i] == character)
    {
      stringData++;
    }

    else if (stringData == pos)
    {
      per_data.concat(data[i]);
    }

    else if (stringData > pos)
    {
      return per_data;
      break;
    }
  }

  return per_data;
}

void getSerial()
{
  while (Serial.available() != 0)
  {
    data = Serial.readString();

    for (a = 0; a < 3; a++)
    {

      get_data[a] = split(data, ' ', a);

      if (get_data[a] != NULL)
      {

        if (a == 0)
        {
          x = get_data[0].toInt();
        }

        if (a == 1)
        {
          y = get_data[1].toInt();
        }

        if (a == 2)
        {
          z = get_data[2].toInt();
        }
      }
    }
    runServo(x, y, z);
  }
}

void setup()
{
  Serial.begin(9600);

  servoX.attach(X);
  servoY.attach(Y);
}

void loop()
{

  getSerial();
}