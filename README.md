
# AIRus 

AirRUS est un dispositif IoT compact et autonome capable de mesurer en temps réel la qualité de l’air ambiant dans des environnements urbains, scolaires ou professionnels. L’objectif est de fournir des données précises sur les polluants pour sensibiliser les utilisateurs et améliorer les conditions de travail des élèves.
Ce système permet aussi de détecter les pics de pollution intérieure ou extérieure et de générer des alertes via une interface web ou mobile et a un  bracelet connecté destiné aux enfants les plus vulnérables à savoir les asthmatique .le bracelet est composé d’un récepteur,un vibreure et d’un buzzer qui permet de prévenir l’enfant ou l’enseignant de prendre les précautions nécessaire en cas de pic de pollution.


## Plan

 - [Fonctionalites](#)
 - [Liste materiel](#)
 - [Architecture](#)
 - [Deploiement](#)
 - [Demo](#)

## Fonctionalitees

- Touch mode
- Affichage inteactif
- Mode economie d'energie
- Multiecran
- Alert



### Apercu

![App Screenshot](https://github.com/djirotechno/Airus_project/blob/main/deploiement/IMG_2448.png)


## Liste materiel 


| ID | Designation     | Quantitee                |
| :-------- | :------- | :------------------------- |
| 01 | XIAO esp32C6 | 1 |
| 02| Batterie Lithium - ion 3,7v | 1|
| 03     | Imprimante 3D Ender v3 | 1|
|04|Boutton Touch TT223 | 1|
|05| Moteur Vibrateur en Disque|1|
|06|Ecran OLED  ssd1306|1|

### Architecture

![architecture_airus](https://github.com/djirotechno/Airus_project/blob/main/architecture_airus.png)

### Cablage
| Composant | ESP PIN   |
| :-------- | :------- |
| Oled1:GND |  esp:GND|
| Oled1:VCC | esp:3V3|
|Oled1:SCL | esp:D5 |
|Oled1:SDA|esp:D4 |
|Vibrateur:+| esp:5v|
|Vibrateur:-|esp:GND|
|BTouch:DAT|esp:D1|
|BTouch:VCC | esp:3.3v|
|BTouch:GND"|esp:GND|


## Deploiement des capteurs

Nous avons deployer 6 capteur 4 dans des etablissements scolaire(Lycee djignabo,CEM malick FALL,CEM Boucotte sud,Lycee ahoune sane) a ziguinchor 
et 2 structures publics (Hopital regional de ziguinchor - Aeropor international de capskiring)

![Airgradient outdoor](httphttps://www.airgradient.com/outdoor/)

![Airgradient API](hhttps://api.airgradient.com/public/docs/api/v1/#/)

### En image
|| | |
| :---: | :---: | :---: |
| ![image](https://github.com/djirotechno/Airus_project/blob/main/deploiement/IMG_2837.JPG) | ![Alt Text 2](https://github.com/djirotechno/Airus_project/blob/main/deploiement/IMG_2879.JPG) | ![Alt Text 3](https://github.com/djirotechno/Airus_project/blob/main/deploiement/IMG_2973.JPG) |

##Demo
<p align="center">
  <a href="https://youtube.com/shorts/V1lu1ZZ0HvY">
    <img src="https://img.youtube.com/vi/V1lu1ZZ0HvY/0.jpg" alt="Demo Video">
  </a>
</p>
