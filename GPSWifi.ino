#include <TinyGPS++.h> // library for GPS module
#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
TinyGPSPlus gps;  // The TinyGPS++ object
SoftwareSerial ss(4, 5); // The serial connection to the GPS device
const char* ssid = ""; //ssid of your wifi
const char* password = ""; //password of your wifi
float latitude , longitude;
int year , month , date, hour , minute , second;
String date_str , time_str , lat_str , lng_str;
int pm;
WiFiServer server(80);


void setup()
{
  Serial.begin(115200);
  ss.begin(9600);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password); //connecting to wifi
  while (WiFi.status() != WL_CONNECTED)// while wifi not connected
  {
    delay(500);
    Serial.print("."); //print "...."
  }
  Serial.println("");
  Serial.println("WiFi connected");
  server.begin();
  Serial.println("Server started");
  Serial.println(WiFi.localIP());  // Print the IP address
}

void loop()
{
  while (ss.available() > 0) //while data is available
  {
    if (gps.encode(ss.read())) //read gps data
    {
      if (gps.location.isValid()) //check whether gps location is valid
      {
        latitude = gps.location.lat();
        lat_str = String(latitude , 6); // latitude location is stored in a string
        longitude = gps.location.lng();
        lng_str = String(longitude , 6); //longitude location is stored in a string
      }
      if (gps.date.isValid()) //check whether gps date is valid
      {
        date_str = "";
        date = gps.date.day();
        month = gps.date.month();
        year = gps.date.year();
        if (date < 10)
         date_str = '0';
        date_str += String(date);// values of date,month and year are stored in a string
        date_str += " / ";


        WiFiClient client = server.available(); // Check if a client has connected
        if (!client)
        {
          return;
        }
        // Prepare the response
        String s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n <!DOCTYPE html> <html> <head> <title>GPS DATA</title> <style>";
        s += "a:link {background-color: YELLOW;text-decoration: none;}";
        s += "table, th, td </style> </head> <body> <h1  style=";
        s += "font-size:300%;";
        s += " ALIGN=CENTER> GPS DATA</h1>";
        s += "<p ALIGN=CENTER style=""font-size:150%;""";
        s += "> <b>Location Details</b></p> <table ALIGN=CENTER style=";
        s += "width:50%";
        s += "> <tr> <th>Latitude</th>";
        s += "<td ALIGN=CENTER >";
        s += lat_str;
        s += "</td> </tr> <tr> <th>Longitude</th> <td ALIGN=CENTER >";
        s += lng_str;
        s += "</td> </tr> <tr>  <th>Date</th> <td ALIGN=CENTER >";
        s += date_str;
        s += "</td></tr> <tr> <th>Time</th> <td ALIGN=CENTER >";
        s += time_str;
        s += "</td>  </tr> </table> ";
        s += "</body> </html> \n";
        client.print(s); // all the values are send to the webpage
        delay(100);
      }

    }


  }
}
    