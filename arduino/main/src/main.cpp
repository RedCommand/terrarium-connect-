#include <Arduino.h>
#include <Servo.h>

float temp_zone_froide;                                                         // création de la variable pour la température de la zone froide
Servo ServoTrappe;                                                              // création d'un servo

void setup() {
  // put your setup code here, to run once:

  ServoTrappe.attach(3);                                                        // initialisation de la broche 3 pour le servo moteur (pour la trappe)
}

void loop() {
  // put your main code here, to run repeatedly:

}


void trappe(int angle = 90) {
  // création d'une fonction permetant de gérer la trappe (possibilitée de définir l'angle d'ouverture, sinon l'angle par défault sera 90°)
  // exemple d'utilisation de la fonction : "trappe()" pour ouvrir la trappe à l'angle par default. Ou bien : "trappe(35)" pour ouvrir la trappe à 35°.
  // ATTENTION : la variable "angle" dois etre comprise entre 0 et 180°

  ServoTrappe.write(angle);
}

void get_temp_zone_froide() {
  // création d'une fonction permetant de lire la température de la zone froide
  // exemple d'utilisation de la fonction : "float maTemperature = get_temp_zone_froide()"
  int temp = analogRead(A1);                                                    // lis la valeur analogique sur la broche A1 (le capteur de température de la zone froide)
  temp = map(temp, 0, 1023, 0, 5000);                                           // convertie la valeur analogique en mV
  float pas = 0.01;                                                             // exemple pour un pas de 0.01°C/mV
  float temperature = temp * pas;                                               // convertie la variable "temp" en °C (grâce au pas)
  return (temperature);                                                         // renvoie la température
}
