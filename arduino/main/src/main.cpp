#include <Arduino.h>
#include <Servo.h>

float temp_zone_froide;                                         // création de la variable pour la température de la zone froide
Servo ServoTrappe;                                              // création d'un servo

void setup() {
  // put your setup code here, to run once:

  ServoTrappe.attach(3);                                        // initialisation de la broche 3 pour le servo moteur (pour la trappe)
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

void
