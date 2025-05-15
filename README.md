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
