#include <SPI.h>
#include <WiFi.h>


// Home Network 
// char ssid[] = "TALKTALKB80B47"; //enter you network SSID (name) here
// char ssid[] = "WiFi-Repeater"; // Network SSID
// char pass[] = "383MDWQJ";    // Network password
// YSJ Network 
  char ssid[] = "CS";
// YSJ Network Password 
  char pass[] = "oPh2Vu2u";

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
    status = WiFi.begin(ssid, pass);

    // wait 5 seconds for connection:
    delay(5000);
  } 
  server.begin(); // Creates a local server.
  WifiStatus(); // Prints status of the Wifi.
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
          client.println("<h1>Air & Noise Pollution Monitor</h1>"); // Main header.
          client.println("<h2>Air Pollution Monitor</h2>"); // Sub-heading.
          AirSensor(); // Calling and using the AirSensor class.
          client.println("<h2>Noise Pollution Monitor</h2>");
          MicSensor(); // Calling and using the MicSensor class.
          client.println("</html>");
          break;
        }
        if (c == '\n') {
          // Starting a new line
          currentLineIsBlank = true;
        } 
        else if (c != '\r') {
          // Have a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // Give the browser time to receive data
    delay(1);
      // Close connection:
      client.stop();
      Serial.println("client disonnected");
  }
}

void AirSensor()
{
  WiFiClient client = server.available(); // Creating a WifiClient to allow for posting to a web page.
  int AirsensorValue = analogRead(A0); // Read from the specified pin.

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
  pinMode (Microphone, INPUT); // Set the pin as input
  MicsensorValue = analogRead(A1); // Reads the specified pin.
  double db = 20.0 * log10(MicsensorValue / 5.0) ; // This is what converts the microphone output voltage to Decibels. 
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

