# Surveillance Hybride
Système de surveillance hybride avec Microcontrôleur STM32 + Capteurs

## Introduction
L’objectif de ce projet est de concevoir un système de surveillance hybride d’une maison (secondaire), combinant une Fonction Principale de mesure de température et de luminosité et un dispositif secondaire de surveillance d’intrusion (nocturne). 
Le système sera donc capable de faire à la fois des relevés périodiques de métriques issues de l’environnement (Température, Luminosité) et d’avertir en cas de détection d’une intrusion en se basant sur un autre capteur (Infrarouge).

Pour implémenter ce système, nous nous baserons sur le microcontrôleur STM32. Nous interfacerons la carte STM32 avec plusieurs capteurs ainsi qu’un PC pour logger les données issues des relevés périodiques de la fonction principale.

Le schéma ci-dessous illustre de manière simplifiée l’ensemble du système qui sera mis en œuvre :
![image](https://github.com/user-attachments/assets/11d02a35-489b-4699-8f5a-da279219a8c2)

## Périphériques utilisés
Les périphériques utilisés au cours de ce Projet sont :
-	UART pour logger les données
-	Capteurs « environnementaux »
	- Capteur de Température – I2C – MCP9808
	- Capteur de Luminosité – KY-018
-	Capteurs « environnementaux »
	- Capteur Infrarouge – KY-032

Note : Le Capteur de Luminosité envoi des « signaux » Analogique ; il faudra donc mettre en place un CAN (Convertisseur Analogique Numérique), pour que le processeur puisse traiter le signal qu’il reçoit du capteur. 
Les autres capteurs utilisés envoient des signaux Numériques.

### UART
L’UART est utilisé pour transmettre les données reçues par le PC via une liaison série (en l’occurrence « Serial Debug Assistant »).

### Capteur de Température – I2C – MCP9808
![image](https://github.com/user-attachments/assets/675f05b4-3343-496d-870a-99314d8b4690)

#### Liaison Physique
Pour utiliser le boitier à l’adresse I2C 1010000, on ne s’occupe pas des broches A0, A1 et A2.
Pour les branchements entre les ports du STM32 et le capteur :
- On relie le port PB8 à SCL.
-	On relie le port PB9 à SDA.
-	Pour l’alimentation, on relie le port correspondant (3V3) à la plaque (au niveau du ‘+’) et on relie la plaque au capteur sur VDD.
-	Pour le port GND, on relie ce port à la plaque (au niveau du ‘-‘) et on relie la plaque au capteur sur GND.

![image](https://github.com/user-attachments/assets/9702a0a2-4e84-4f56-b2e5-e98a2f6ef9f7)

### Capteur de Luminosité – KY-018
![image](https://github.com/user-attachments/assets/ddcd7009-cadf-4767-80fc-6e46df133380)

#### Liaison Physique
https://sensorkit.joy-it.net/fr/sensors/ky-018

![image](https://github.com/user-attachments/assets/b17b8883-e2a2-499d-983b-65ef2576e984)

Pour relier le capteur au STM32, on affecte les broches comme indiqué ci-dessous:
-	Le port « GND » du capteur est relié au GND du STM32
-	L’alimentation du capteur est reliée au port « 5V » du STM32
-	Le port qui envoie le signal du capteur est relié au PA0 du STM32

![image](https://github.com/user-attachments/assets/7de7da34-9352-45b2-9248-6e9b490facef)

### Capteur Infrarouge – KY-032
Note : Le Capteur Infrarouge (Capteur d'Obstacles) dispose d’une sortie numérique permettant simplement de générer un événement lorsque l’obstacle est détecté.

#### Liaison Physique
https://sensorkit.joy-it.net/fr/sensors/ky-032

![image](https://github.com/user-attachments/assets/316cdf5c-5bbc-4288-960a-0c81056db392)

-	On connecte le GND du capteur au GND du STM32
-	On connecte l’alimentation du capteur au port « 5V » du STM32
-	On connecte la sortie du signal du capteur au port PA10

![image](https://github.com/user-attachments/assets/6bcd005e-5310-466e-81aa-9b3862df327d)

## Gestion du Temps
Les relevés périodiques de la Fonction Principale et de la Fonction Secondaire doivent suivre le schéma ci-dessous :
![image](https://github.com/user-attachments/assets/723180f5-40e3-4bbf-9121-1b7dfedeb6be)

https://youtu.be/VfbW6nfG4kw

### Configuration de Base
Pour la Fonction Principale et la Fonction Secondaire, dans le but de réaliser un relevé périodique, il faut réaliser certaines étapes « de base » (voir ANNEXE – Interfaces Graphiques – Gestion du Temps) ...

... Après avoir modifié l’Interface Graphique, il faut modifier le code source afin de pouvoir lancer le TIMER 2 et le TIMER 5 en mode Interruption.

### Fonction Principale
![image](https://github.com/user-attachments/assets/6b95e9b1-b23a-4e02-904e-a56fc46eb4a6)

Utilisation d’un TIMER (TIMER 2) ; toutes les 5 minutes, il y aura une nouvelle mesure par les Capteurs « environnementaux » (Température + Luminosité) et un envoi des données au PC.

300*84000000 / 8400 = 3000000

Donc, pour que L’UEV (Update Event) ait lieu toutes 5 minutes (toutes les 300 secondes), si on met le Prescaler du TIMER 2 à 8400-1, on met donc la « Counter Period » à 3000000-1.
![image](https://github.com/user-attachments/assets/6904a6f7-6335-468b-ad83-09a027f33823)

### Fonction Secondaire
![image](https://github.com/user-attachments/assets/b1bf3ab4-d3fc-4c7b-a9f9-2c1c77a6492c)

Utilisation d’un TIMER (TIMER 5) ; toutes les minutes, à moins d’une « Désactivation des Alertes », il y aura un nouveau « test » par le Capteur d’Intrusion Infrarouge et un envoi des données au PC.

Donc, pour que L’UEV ait lieu toutes 1 minutes (toutes les 60 secondes), si on met le Prescaler du TIMER 5 à 8400, on met donc la « Counter Period » à 60*84000000 / 8400 = 600000.
![image](https://github.com/user-attachments/assets/a7ae82b0-88b8-423d-842c-1cf025881998)

#### Désactivation des Alertes
On applique une « Scrutation ».

Si on appuie sur le bouton power (PC13) et qu’une alerte a été déclenchée (si un obstacle a été détecté par le Capteur Infrarouge et que la LED LD2 reliée à PA5 est allumée), les alertes sont donc désactivées (LD2 s’éteint) ; il faut donc mettre le PC13 en « GPIO_Input » et PA5 en « GPIO_Output ».

![image](https://github.com/user-attachments/assets/421dd9a8-4677-4c2d-93a4-c1474c1afb9d)

## Conclusion
Ce projet m’aura permis d’apprendre à mettre en œuvre des Capteurs de Température, de Luminosité et aussi Infrarouge (d’Obstacle) en interface avec un microcontrôleur STM32.

L’UART m’aura permis d’apprendre à utiliser une liaison série (Serial Debug Assistant) pour transmettre les données reçues par le PC et les afficher sur un « terminal ».

La mise en place des Relais Périodiques m’aura permis de mieux comprendre la « Périodicité » avec les TIMER et aussi les Interruptions (récupération à intervalle de temps régulier des (nouvelles) données envoyées par les capteurs).

La « Désactivation des Alertes » m’aura permis de mettre en œuvre une « Scrutation » (surveiller le système, à l'affût d’une « action particulière », pour « déclencher un événement »).

## ANNEXE
### Interfaces Graphiques
#### UART
La communication UART (Universal Asynchronous Receiver Transmitter) est asynchrone.

Pour transmettre les données reçues par le PC via Serial Debug Assistant avec l’UART, sur l’interface graphique de STM32, on met le USART2 en mode « Asynchronous » ;

![image](https://github.com/user-attachments/assets/3718ab0f-d50f-4473-ae35-c0b3fe93ff13)

Note : Pour que la communication se passe bien, il faut veiller à ce que l’émetteur et le récepteur partagent les mêmes paramètres de transmission.

#### Gestionnaire des Périphériques
![image](https://github.com/user-attachments/assets/c0c27f69-a6b8-4d95-8e48-d41bae58987c)

Le ST-Link Debug est le device utilisé pour programmer et débugger le microcontrôleur STM32 via l’USB. Le port COM virtuel (COM3) utilisé pour échanger les données utilisateur entre le PC et le STM32.

#### Fonction printf()
Pour que cette fonction puisse afficher de nombres flottants, il faut modifier les paramètres de compilation, dans les paramètres du projet, en ajoutant le flag « -u_printf_float » ;

![image](https://github.com/user-attachments/assets/655e4f9a-0fca-445c-8246-8a6c7bc40f21)

On peut aussi, dans « MCU Setting », cocher une case pour utiliser les « float » avec printf, afin d’être sûr que cela fonctionne ;

![image](https://github.com/user-attachments/assets/6b3b93a3-3da3-4b96-9c87-7fdf1be17d29)

#### Capteur de Température – I2C – MCP9808
L’I2C1 a les broches SCL et SDA accessibles via PB8 et PB9 respectivement ; on met donc PB8 en « I2C1_SCL » et PB9 en « I2C_SDA ».
![image](https://github.com/user-attachments/assets/a5b6aa0c-ec24-4e3e-b17c-cf89ef9b621f) ![image](https://github.com/user-attachments/assets/c80a76e7-4bdb-420d-8989-15860f94f0bc)

Après avoir configuré PB8 et PB9, on active la Connexion I2C ;

![image](https://github.com/user-attachments/assets/40a6a6b1-4876-4168-8c84-18d5ae5d1c9f)

#### Capteur de Luminosité – KY-018
- On reçoit le signal analogique envoyé par le capteur via le PA0 ; il faut donc configurer le PA0 en « ADC1_IN0 » ; ![image](https://github.com/user-attachments/assets/aba208fa-b99a-41e3-b295-7bf7d084840f)
- Lorsque de la lumière est détectée (lorsqu’elle est « allumée »), la LED LD2 qui est reliée à PA5 s’allume. Sinon, lorsque la lumière est « éteinte », LD2 s’éteint ; il faut donc mettre PA5 en « GPIO_Output » ;

  ![image](https://github.com/user-attachments/assets/adfda73e-96e4-41d1-929d-567c8f6a5fee)
- Activer le IN0 de l’ADC1 pour gérer les Signaux Analogiques ;

  ![image](https://github.com/user-attachments/assets/cdd2dba6-02c1-4b1a-b295-122f168191b9)
- Activer le NVIC, pour autoriser les Interruptions (pour la Gestion de Temps) ;

  ![image](https://github.com/user-attachments/assets/24048f62-4340-4321-93e9-467f43d21b00)

#### Capteur Infrarouge
Le signal numérique du Capteur Infrarouge est reçu via le port PA10 ; on met donc PA10 en GPIO_Input ;
![image](https://github.com/user-attachments/assets/2764e283-d866-452f-976e-358a735dfdea)

La LED LD2 qui s’allume en cas d’obstacle détecté est reliée au PA5 ; on met donc PA5 en GPIO_Output ;
![image](https://github.com/user-attachments/assets/5aa135b5-64ac-421d-abaf-32c5638d9c6d)

#### Gestion du Temps
##### Configuration de Base
Note : L’horloge d’entrée du TIMER 2 et du TIMER 5 est à h=84 MHz (84 000 000 Hz)
![image](https://github.com/user-attachments/assets/5c0c9bee-1f6a-4897-90b8-bbcd5a1cbc10)

Dans le but de réaliser un Relevé Périodique :
- Définir une source d’horloge interne ; Par exemple, pour le TIMER 2 :

  ![image](https://github.com/user-attachments/assets/55ec184d-bbb1-4ebd-911a-8a3e464e8c22)
- Régler la valeur « p » du Prescaler, la valeur « pc » de la Période de Comptage ainsi que le Trigger Event Mode, afin de pouvoir générer un UEV (Update Event) toutes n secondes.

  La formule à utiliser est la suivante : n*(h/p) = pc. 

  Par exemple : pour que l’UEV ait lieu toutes les 1 secondes, si on met le Prescaler à 10000, 1*84000000/10000=8400. On met donc la « Counter Period » à 8400.
  
  Avant les modifications :

  ![image](https://github.com/user-attachments/assets/17b1ce97-5cfe-4b4c-95f5-ec6897c61f80)

- Mettre le Trigger Event Mode en « *Update Event* »

  ![image](https://github.com/user-attachments/assets/8c6cbd62-67e8-4623-8c3d-92984cbde136)

- Autoriser l’interruption global du TIMER ;

  Par exemple, pour le TIMER 2 :

  ![image](https://github.com/user-attachments/assets/f53641c2-2d13-4b91-b5cb-886fa98f2400)

-> Pareil pour le TIMER 5

- Activer le NVIC de l’ADC1 (vérifier qu’il est activé), pour autoriser les Interruptions ;

  ![image](https://github.com/user-attachments/assets/d0484def-5bb5-46f8-aaa7-0493e21c9461)

#### Résultat final
À la fin des modifications précédentes, les ports sont configurés comme illustré ci-dessous ;
![image](https://github.com/user-attachments/assets/d52204cb-8814-41dc-bcf7-fbd3edd97dab)

### Codes Sources
#### Périphériques
##### UART
###### UART avancé : printf
Re-définir l’implémentation d’une fonction printf redirigeant les caractères vers l’UART et non la sortie standard console en C.

Dans le fichier *main.c* :

![image](https://github.com/user-attachments/assets/a40af489-9b97-41a8-81e9-427c02bed527)

###### Fonction printf()
Dans le fichier *main.c* :

![image](https://github.com/user-attachments/assets/e285a788-365b-4632-9b46-0159912ff90c)

Preuve du fonctionnement, via Serial Debug Assistant :

![image](https://github.com/user-attachments/assets/1641620d-4e43-43ba-bc34-29d6215564b2)

##### Capteur de Température – I2C – MCP9808
Dans le fichier *main.c*

![image](https://github.com/user-attachments/assets/7e681e98-240a-455b-8b2a-46c6ec1d46c1)

![image](https://github.com/user-attachments/assets/f4d0da31-fed7-4187-8771-d8f849c94c53)

![image](https://github.com/user-attachments/assets/b92b9f5f-2eb0-4f51-82db-63d289dfd2a7)

##### Capteur de Luminosité – KY-018
Dans le fichier *main.c*

![image](https://github.com/user-attachments/assets/a81797b4-bfa1-4c07-a0b0-3220a8f314ca)

![image](https://github.com/user-attachments/assets/bb6f2210-95f0-4e67-b083-5fbecdc07905)

Note : Au début de chaque itération de la boucle « while », on lance l’ADC (on s’assure qu’il est bien lancé), afin que les données récoltées par le capteur se mettent à jour en fonction de l’environnement (pour si la luminosité devait varier au cours du temps).

Résultat :

![image](https://github.com/user-attachments/assets/6b70dc48-3b20-48bb-bd46-b58b9c3df990) ![image](https://github.com/user-attachments/assets/6eda8435-f2bc-4c6d-845c-90422246df86)

##### Capteur Infrarouge
Dans le fichier *main.c* :

![image](https://github.com/user-attachments/assets/bbcea53e-8d12-4af6-a335-e108fdf7b029)

-> Quand un obstacle est détecté par le capteur, la LED LD2 s’allume.

![image](https://github.com/user-attachments/assets/3b473e03-876e-4b05-944e-cb09692f2fe6)

-> Si l’obstacle devant le capteur est retiré (disparait), LD2 s’éteint.

![image](https://github.com/user-attachments/assets/cac30932-e539-49e5-b113-28785d96c72a)

#### Gestion du temps
Après avoir implémenté les différentes « fonctions » pour l’usage des capteurs (Température, Luminosité, Infrarouge), on met en place un Relevé Périodique.

##### Fonction Principale
Pour l’usage des Capteurs de Température et de Luminosité :

Dans le fichier *main.c*, on considère la procédure ci-dessous, pour l’usage du Capteur de Luminosité ;

![image](https://github.com/user-attachments/assets/e9dd372c-7fea-4950-bfca-46bc76172951)

![image](https://github.com/user-attachments/assets/9e0cb6b0-0580-4d78-84dd-0374ad262a21)

On considère aussi la procédure « *getTempI2C()* » qui contient le code pour le Capteur de Température – I2C – MCP9808.

![image](https://github.com/user-attachments/assets/ca40c6eb-719d-4aa2-83c9-4c0e0e9b5f91)

-> Cette procédure permet de gérer le Relais Périodique (toutes les 5 minutes) de la Fonction Principale, en utilisant le TIMER 2 « htim2 ».

##### Fonction Secondaire
Dans le fichier *main.c*, on considère la procédure pour l’usage du Capteur Infrarouge ;

![image](https://github.com/user-attachments/assets/b7d0b71b-f4e0-4ce9-9460-b5382a468033)

![image](https://github.com/user-attachments/assets/96206731-bbff-48c3-9d1f-578e8c042083)

![image](https://github.com/user-attachments/assets/93eb8eb3-b24c-4303-8e48-02be6d85f1a3)

-> On ajoute la gestion du Relais Périodique (toutes les minutes) de la Fonction Secondaire, en utilisant le TIMER 5 « htim5 ».

#### Désactivation des Alertes
![image](https://github.com/user-attachments/assets/fb3246df-eb70-47dd-946c-93b2b32589b6)

-> Si on appuie sur le bouton poussoir alors qu’un obstacle a été détecté par le Capteur Infrarouge et qu’une alerte a été émise (que la LED LD2 est allumée), l’alerte est désactivée (LD2 s’éteint).
-> Sinon, en cas d’obstacle détecté, l’alerte reste active (LD2 continue de s’allumer (au moins) jusqu’à la fin du TIMER associé (TIMER 5) ou jusqu’à une pression sur le bouton poussoir).
