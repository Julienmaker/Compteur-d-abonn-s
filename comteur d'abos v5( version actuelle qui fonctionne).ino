//Julien Fillaut Compteur d'abonnés

// Pour pouvoir téléverser le code sur un ESP-32 téléchargez le package "ESP-32" dans le gestionnaire de cartes et collez cet URL "https://dl.espressif.com/dl/package_esp32_index.json" dans l'URL dans de cartes supplémentaires




#include <SPI.h>                                                                                   // Déclaration des librairies nécessaires à l'écran Oled et du nombre de pixels ect...
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#include <WiFi.h>                                                                                // Déclaration des librairies nécessaires à la requête API YouTube
#include <WiFiClientSecure.h>
#include <YoutubeApi.h>
#include <ArduinoJson.h>

char ssid[] = "";       // Nom du réseau Wifi
char password[] = "";  // Mot de passe du réseau Wifi
#define API_KEY ""  // Clé Api YouTube
#define CHANNEL_ID "" // ID de la chaîne YouTube
//------- ---------------------- ------

WiFiClientSecure client;
YoutubeApi api(API_KEY, client);
unsigned long api_mtbs = 10000; // Déclaration du délai de la requête (ici toutes les 10 secondes)
unsigned long api_lasttime;   

long subs = 0;

void setup() {
Serial.begin(9600);
// SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
Serial.println(F("SSD1306 allocation failed"));
for(;;); // Don't proceed, loop forever
}
  Serial.print("Connecting Wifi: ");
  Serial.println(ssid);

     client.setInsecure();
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  IPAddress ip = WiFi.localIP();
  Serial.println(ip);

display.display();
delay(2000); // Délai de 2 secondes

}
void loop() {
  
  if (millis() > api_lasttime + api_mtbs)  {
    if(api.getChannelStatistics(CHANNEL_ID))
  
    {
      Serial.println("---------Stats---------");          // Affichage des données analytiques de la chaîne dans la Moniteur Série
      Serial.print("Subscriber Count: ");
      Serial.println(api.channelStats.subscriberCount);
      Serial.print("View Count: ");
      Serial.println(api.channelStats.viewCount);
      Serial.print("Comment Count: ");
      Serial.println(api.channelStats.commentCount);
      Serial.print("Video Count: ");
      Serial.println(api.channelStats.videoCount);
      Serial.println("------------------------");
    }
    api_lasttime = millis(); 

 }

long abo_nb = api.channelStats.subscriberCount;   // Définir le stockage du nombre d'abonnés (c'est à dire "api.channelStats.subscriberCount") dans une variable du nom de "abos_nb"

display.clearDisplay(); // Initialisation de l'écran Oled
display.display();
delay(100);
display.setTextSize(3);  // Définir la taille du texte qui s'affichera sur l'écran
display.setTextColor(WHITE); // Définir la couleur du texte qui s'affichera
display.setCursor(40,22); // Définir le placement du texte
display.print(abo_nb); //  Ecrire la valeur contenue dans la variable "abo_nb" sur l'écran Oled, c'est à dire la nombre d'abonnés de la chaîne YouTube
display.display();     // Réinitialisation de l'écran Oled
delay(200);            // Délai de 200 millisecondes
}