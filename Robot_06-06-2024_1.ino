//*************ENTETE***************
//Nom du fichier = Robot_06-06-2024_1
//Nom,Prenom = Pierrick DeCamp
//Hardware =
//Date = 06/06/2024

//************Librairie*************
#include <Adafruit_SH110X.h>
#include <Adafruit_NeoPixel.h>
#include "DabbleGamepad.h"
#include <Arduino.h>

DabbleGamepad gamepad;
Adafruit_SH1107 display = Adafruit_SH1107(64, 128, &Wire);

//************Constante**************
#define IN1 10
#define IN2 13
#define IN3 11
#define IN4 12

#define BP_A 9
#define BP_B 6
#define BP_C 5

#define Mode 0
#define ModeBT 1
#define ModeAUTO 2

#define TRIG_PIN_G A0
#define ECHO_PIN_G A1
#define TRIG_PIN_D A2
#define ECHO_PIN_D A3

//***********Variable*********

unsigned long Impulsion_Echo_G; //défintion de la variable des capteur US
unsigned long Impulsion_Echo_D;

unsigned int Distance_D; //définition de la variable de la distance
unsigned int Distance_G;

byte mode = Mode;
byte vitesse = 150;

//***********FONCTIONS******************

void Avance(void) {  //définition des fonction des mouvements
  analogWrite(IN1, vitesse);
  digitalWrite(IN2, 0);
  analogWrite(IN3, vitesse);
  digitalWrite(IN4, 0);
}

void Arriere(void) {
  analogWrite(IN1, 0);
  digitalWrite(IN2, 1);
  analogWrite(IN3, 0);
  digitalWrite(IN4, 1);
}

void Stop(void) {
  analogWrite(IN1, 255);
  digitalWrite(IN2, 1);
  analogWrite(IN3, 255);
  digitalWrite(IN4, 1);
}

void Gauche(void) {
  analogWrite(IN1, 255);
  digitalWrite(IN2, 0);
  analogWrite(IN3, 0);
  digitalWrite(IN4, 1);
}

void Droit(void) {
  analogWrite(IN1, 0);
  digitalWrite(IN2, 1);
  analogWrite(IN3, 255);
  digitalWrite(IN4, 0);
}


void setup() {

  Serial.begin(9600);  // démarage des ports série bt et de la délécomande
  Serial.begin(9600);
  gamepad.begin(9600);

  pinMode(IN1, OUTPUT); //définition des pins moteurs 
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  pinMode(TRIG_PIN_G, OUTPUT); //définition des pins US
  pinMode(ECHO_PIN_G, INPUT);
  pinMode(TRIG_PIN_D, OUTPUT);
  pinMode(ECHO_PIN_D, INPUT);

  pinMode(BP_A, INPUT_PULLUP); //définition des bouton 
  pinMode(BP_B, INPUT_PULLUP);
  pinMode(BP_C, INPUT_PULLUP);

  display.begin(0x3C, true); //affichage du buffers 
  display.display();
  delay(200);

  display.clearDisplay(); //Efface de l'écran 
  display.display();

  display.setTextColor(SH110X_WHITE);
  display.setRotation(1);
  display.setCursor(0, 0);
  display.println("Mode BT");
  display.println("");
  display.println("");
  display.println("");
  display.println("Mode US");
  display.display();

  while (Mode == 0) { //Choix du mode 
    if (!digitalRead(BP_A))
      mode = ModeBT;
    if (!digitalRead(BP_C))
      mode = ModeAUTO;
  }
}

void loop() {
  if (mode == ModeBT) { //Démarage du mode BT
    display.setCursor(50, 10);
    display.println("Mode BT");
    display.display();

    gamepad.update();  //fonction qui raffraichie la lecture du bluetooth
    if (gamepad.isButtonPressed(UP)) {
      Avance();
    }
    if (gamepad.isButtonPressed(DOWN)) {
      Stop();
    }
    if (gamepad.isButtonPressed(LEFT)) {
      Gauche();
    }
    if (gamepad.isButtonPressed(RIGHT)) {
      Droite();
    }
    if (gamepad.isButtonPressed(TRIANGLE)) {
      Arriere();
    }
    if (gamepad.isButtonPressed(CROSS)) {
      Stop();
    }
  }

  if (Mode == Mode_AUTO) { //mode auto 
    digitalWrite(TRIG_PIN_D, 1);  //mesure capteur Droit
    delayMicroseconds(100);
    digitalWrite(TRIG_PIN_D, 0);

    Impulsion_Echo_D = pulseIn(ECHO_PIN_D, HIGH);
    Serial.println(Impulsion_Echo_D);

    Distance_D = Impulsion_Echo_D * 0.034 / 2;
    Serial.println(Distance_D);
    delay(1);


    digitalWrite(TRIG_PIN_G, 1);  //mesure capteur gauche
    delayMicroseconds(100);
    digitalWrite(TRIG_PIN_G, 0);

    Impulsion_Echo_G = pulseIn(ECHO_PIN_G, HIGH);
    Serial.println(Impulsion_Echo_G);

    Distance_G = Impulsion_Echo_G * 0.034 / 2;
    Serial.println(Distance_G);
    delay(1);

    analogWrite(IN1, vitesse);
    digitalWrite(IN2, 0);
    analogWrite(IN3, vitesse);
    digitalWrite(IN4, 0);

    if (Distance_D < 3) {
      Rotation_G();
      delay(50);
    }
    if (Distance_G < 3) {
      Rotation_D();
      delay(50);
    }
  }
}
