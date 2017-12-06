#include <SPI.h>
#include <WiFi.h>


// Home Network 
// char ssid[] = "TALKTALKB80B47"; //enter you network SSID (name) here
char ssid[] = "WiFi-Repeater"; // Network SSID
char pass[] = "383MDWQJ";    // Network password
// YSJ Network 
// char ssid[] = "CS";
// YSJ Network Password 
// pass[] = "";

int AirQuality = A0;  // Pin connected to pin 0 is the MQ135 Gas Sensor
const int Microphone = A1; // Pin connected to pin 1 Module Sound Sensor   
double MicsensorValue = 0.; // Setting the value for the microphone.

int status = WL_IDLE_STATUS;

WiFiServer server(80); // Creating a local server with the port 80.

void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  
  // attempt to connect to Wifi network:
  while ( status != WL_CONNECTED) { 
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network.
    status = WiFi.begin(ssid, pass);

    // wait 5 seconds for connection:
    delay(5000);
  } 
  server.begin();
  WifiStatus();
}


void loop() {
  // listen for incoming clients
  WiFiClient client = server.available();
  if (client) {
    Serial.println("new client");
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        if (c == '\n' && currentLineIsBlank) {
          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connnection: close");
          client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");
          client.println("<meta http-equiv=\"refresh\" content=\"5\">");
          client.println("<h1>Air & Noise Pollution Monitor</h1>"); 
          client.println("<h2>Air Pollution Monitor</h2>");
          AirSensor();
          client.println("<h2>Noise Pollution Monitor</h2>");
          MicSensor();
          client.println("</html>");
          break;
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        } 
        else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(1);
      // close the connection:
      client.stop();
      Serial.println("client disonnected");
  }
}

void AirSensor()
{
  WiFiClient client = server.available();
  int AirsensorValue = analogRead(A0);

  client.print("Air Quality: ");

  client.print(AirsensorValue);

  client.println("*PPM");

  client.print("<br>");

if (AirsensorValue <= 50)
{
  client.print("Air is hardly polluted.");
  client.println();
}

if (AirsensorValue >= 51 && AirsensorValue <= 100)
{
  client.print("Air is slightly polluted.");
  client.println();
}

if (AirsensorValue >= 101 && AirsensorValue <= 150)
{
  client.print("Air is rather polluted and unhealthy for individuals with sensitive lungs.");
  client.println();
}

if (AirsensorValue >= 151 && AirsensorValue <= 200)
{
  client.print("Air is rather polluted and unhealthy for all individuals.");
  client.println();
}

if (AirsensorValue >= 201 && AirsensorValue <= 300)
{
  client.print("Air is very polluted.");
  client.println();
}

if (AirsensorValue > 300)
{
  client.print("Air is incredibly polluted.");
  client.println();
}
 delay (5000);
}

void MicSensor()
{
  WiFiClient client = server.available();
  pinMode (Microphone, INPUT); // Set the signal pin as input
  MicsensorValue = analogRead(A1);
  double db = 20.0 * log10(MicsensorValue / 5.0) ;
  client.print("Sound level: ");
  client.print(db);
  client.println("dB");
  client.print("<br>");

  if (db <= 15)
  {
    client.print("Sound level is quiet.");
    client.println();
  }
  
  if (db >= 16 && db <= 30)
  {
    client.print("Sound level is not too loud.");
    client.println();
  }
  
  if (db >= 31 && db <= 60)
  {
    client.print("Sound level is moderate.");
    client.println();
  }
  
  if (db >= 61 && db <= 100)
  {
    client.print("Sound level is high.");
    client.println();
  }
  if (db > 101)
  {
    client.print("Sound level is very high.");
    client.println();
  }
}

void WifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
}

