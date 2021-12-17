#include <Servo.h>
Servo servo_12;
int  Niveau;
int  max2;
int  min2;
int digitalPin = 7; // pour le relais
void ouvrir_la_porte() {
         servo_12.write(10);
          delay (1000); 
                  }

void fermee() {
             servo_12.write(90);
               delay (1000); 
               }
void min1() {
         digitalWrite(2,HIGH);
          digitalWrite(3,LOW);
          Serial.println("  , niveau inf a 50%  R");

              }

void max1() {
       digitalWrite(2,LOW);
        digitalWrite(3,HIGH);
         Serial.println(" ,niveau   sup a 50% V");

}

void setup()
{
  Niveau = 0;

  max2 = 427;

  min2 = 350;

  Serial.begin(9600);

   servo_12.attach(12);

  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(digitalPin, OUTPUT);
}


void loop(){  

       Niveau = analogRead(A0);
       Serial.print(Niveau);
       if (Niveau < min2) {
          min1();
          digitalWrite(digitalPin, HIGH);
          delay (500);

                       }

    
       if (Niveau > max2) {
           max1();
          digitalWrite(digitalPin, LOW);//La pompe est  arrête 

                        }
 delay(2000);
 }
