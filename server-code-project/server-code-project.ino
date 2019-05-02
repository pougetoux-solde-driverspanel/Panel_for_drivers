#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <PxMatrix.h>
#include "DefaultImages.h"
#include <Arduino_JSON.h>
#ifndef STASSID
#define STASSID "Private123"
#define STAPSK  "19911991"
#endif
#ifdef ESP8266

#include <Ticker.h>
Ticker display_ticker;
#define P_LAT 16
#define P_A 5
#define P_B 4
#define P_C 15
#define P_D 12
#define P_E 0
#define P_OE 2

#endif
uint8_t display_draw_time=0;

String colors[32];
PxMATRIX display(32,32,P_LAT, P_OE,P_A,P_B,P_C,P_D);
// Some standard colors
uint16_t myRED = display.color565(255, 0, 0);
uint16_t myGREEN = display.color565(0, 255, 0);
uint16_t myBLUE = display.color565(0, 0, 255);
uint16_t myWHITE = display.color565(255, 255, 255);
uint16_t myYELLOW = display.color565(255, 255, 0);
uint16_t myCYAN = display.color565(0, 255, 255);
uint16_t myMAGENTA = display.color565(255, 0, 255);
uint16_t myBLACK = display.color565(0, 0, 0);

uint16 myCOLORS[8] = {myRED, myGREEN, myBLUE, myWHITE, myYELLOW, myCYAN, myMAGENTA, myBLACK};
const char* ssid = STASSID;
const char* password = STAPSK;

//---------------------------------------------------------------
//Our HTML webpage contents in program memory
const char MAIN_page[] PROGMEM = R"=====(
  <!DOCTYPE html>
<html>
    <head>
        <link rel="stylesheet" href="https://maxcdn.bootstrapcdn.com/bootstrap/4.0.0/css/bootstrap.min.css" integrity="sha384-Gn5384xqQ1aoWXA+058RXPxPg6fy4IWvTNh0E263XmFcJlSAwiGgFAW/dAiS6JXm" crossorigin="anonymous">
    </head>
    <style>
        html, body {
            height: 100%;
            margin: 0px;
        }

        #buttons-column { 
            width: 50%;
            margin-left: 25%;
            padding-top: 10%;
            height: 40%;
            display: flex;
            flex-direction: column;
        }
        .buttons-row {
            display: flex;
            flex: 1;
        }
        .spebutton {
            margin: 10px;
            flex-basis: 30%;
            flex: 1;
        }
        #footer{
            background:#ccc;
            position:absolute;
            bottom:0;
            width:100%;
            height:10%;
            padding-top: 1%;
            padding-bottom: 1%;
            text-align: center;
        }
        .editbutton {
            width: 15%;
        }
        .grid { margin:1em auto; border-collapse:collapse }
        .grid td {
            cursor:pointer;
            width:15px; height:15px;
            border:1px solid #ccc;
            background-color: black;
        }
    </style>

    <body>
        <div class="modal fade" id="exampleModal" tabindex="-1" role="dialog" aria-labelledby="exampleModalLabel" aria-hidden="true">
            <div class="modal-dialog" role="document">
                <div class="modal-content">
                    <div class="modal-header">
                        <h5 class="modal-title" id="exampleModalLabel">Modification du motif du bouton</h5>
                        <button type="button" class="close" data-dismiss="modal" aria-label="Close">
                            <span aria-hidden="true">&times;</span>
                        </button>
                    </div>
                    <div class="modal-body">
                        <div>
                            <input type="color" id="colorpicker" name="colorpicker" value="#e66465">
                            <label for="head">Choix de la couleur</label>
                        </div>
                        <div id="griddiv"></div>
                    </div>
                    <div class="modal-footer">
                        <button type="button" class="btn btn-secondary" data-dismiss="modal">Close</button>
                        <input type="hidden" id="selectedbutton">
                        <button type="button" class="btn btn-primary" onclick="saveschema()">Save changes</button>
                    </div>
                </div>
            </div>
        </div>

        <div id="buttons-column">
            <div class="buttons-row">
                <button class="spebutton" id="button1" onclick="selectoption(1)">1</button>
                <button class="spebutton" id="button2" onclick="selectoption(2)">2</button>
                <button class="spebutton" id="button3" onclick="selectoption(3)">3</button>
            </div>
            <div class="buttons-row">
                <button class="spebutton" id="button4" onclick="selectoption(4)">4</button>
                <button class="spebutton" id="button5" onclick="selectoption(5)">5</button>
                <button class="spebutton" id="button6" onclick="selectoption(6)">6</button>
            </div>
        </div>

        <div id="footer">
            <button type="button" class="btn btn-primary editbutton" onclick="openmodal(1)">edit 1</button>
            <button type="button" class="btn btn-primary editbutton" onclick="openmodal(2)">edit 2</button>
            <button type="button" class="btn btn-primary editbutton" onclick="openmodal(3)">edit 3</button>
            <button type="button" class="btn btn-primary editbutton" onclick="openmodal(4)">edit 4</button>
            <button type="button" class="btn btn-primary editbutton" onclick="openmodal(5)">edit 5</button>
            <button type="button" class="btn btn-primary editbutton" onclick="openmodal(6)">edit 6</button>
        </div>

        <script src="https://ajax.googleapis.com/ajax/libs/jquery/3.4.0/jquery.min.js"></script>
        <script src="https://cdnjs.cloudflare.com/ajax/libs/popper.js/1.12.9/umd/popper.min.js" integrity="sha384-ApNbgh9B+Y1QKtv3Rn7W3mgPxhU9K/ScQsAP7hUibX39j7fakFPskvXusvfa0b4Q" crossorigin="anonymous"></script>
        <script src="https://maxcdn.bootstrapcdn.com/bootstrap/4.0.0/js/bootstrap.min.js" integrity="sha384-JZR6Spejh4U02d8jOt6vLEHfe/JQGiRRSQQxSfFWpi1MquVdAyjUar5+76PVCmYl" crossorigin="anonymous"></script>
        <script type="text/javascript" language="javascript">
            $(document).ready(function() {
                document.getElementById("colorpicker").onchange = function() {
                    backRGB = this.value;
                    console.log(backRGB);
                    
                    for(var i = 0; i < 32; i++) {
                        for(var j = 0; j < 32; j++) {
                            var color = $("#griddiv")[0].children[0].children[i].children[j].style.backgroundColor;
                            if(color == ""){
                                continue;
                            }
                            else {
                                $("#griddiv")[0].children[0].children[i].children[j].style.backgroundColor = backRGB;
                            }
                        }
                    }
                }
            });

            function clickableGrid( rows, cols, callback ){
                var grid = document.createElement('table');
                grid.className = 'grid';
                for (var r=0;r<rows;++r){
                    var tr = grid.appendChild(document.createElement('tr'));
                    for (var c=0;c<cols;++c){
                        var cell = tr.appendChild(document.createElement('td'));
                        cell.addEventListener('click',(function(el,r,c){
                            return function(){
                                callback(el,r,c);
                            }
                        })(cell,r,c),false);
                    }
                }
                return grid;
            }  

            function openmodal(button) {
                $("#selectedbutton").val(button);
                document.getElementById("griddiv").innerHTML = "";

                var grid = clickableGrid(32,32,function(el,row,col){
                    if(el.style.backgroundColor == "") {
                        el.style.backgroundColor = $("#colorpicker")[0].value;    
                    }
                    else {
                        el.style.backgroundColor = "";
                    }
                });

                $("#griddiv")[0].appendChild(grid);

                $('#exampleModal').modal('show')
            }

            function saveschema() {
                var buttonid = $("#selectedbutton").val();

                var colors = "";
                
                for(var i = 0; i < 32; i++) {
                    for(var j = 0; j < 32; j++) {
                        var color = $("#griddiv")[0].children[0].children[i].children[j].style.backgroundColor;
                        if(color == "") {
                            color = "0";
                        }
                        else {
                            color = "1";
                        }

                        colors += (color + ",");
                    }
                }
                
                console.log(colors);

                $.ajax({
                  type: "POST",
                  url: "http://192.168.43.38/modify",
                  data: {"selected": buttonid, "color": $("#colorpicker")[0].value, "colors": JSON.stringify(colors)}
                });
            }

            function selectoption(button) {
                $.ajax({
                      type: "POST",
                      url: "http://192.168.43.38/call",
                      data: {"selected": button}
                });
            }
        </script>
    </body>
</html>
)=====";
//---------------------------------------------------------------

ESP8266WebServer server(80);

const int led = 13;
void display_updater()
{

  display.display(70);

}
void handleRoot() {
  digitalWrite(LED_BUILTIN, LOW);   // Turn the LED on (Note that LOW is the voltage level
  delay(1000);
  server.send(200, "text/plain", "hello from esp8266!");
  digitalWrite(LED_BUILTIN, HIGH);
}
void drawImage(int x, int y, uint16_t *image)
{
  
  int width = 32;
  int height = 32;
  
  for (int xx = 0; xx < height * width; xx++)
  {
    display.drawPixel(xx % width + x , xx / width + y, image[xx]);
  }
  delay(5000);
  display.clearDisplay();
  
  
}
void drawImageMonochrome(int x, int y, int *image, uint16_t color)
{
  
  int width = 32;
  int height = 32;
  
  for (int xx = 0; xx < height * width; xx++)
  {
    Serial.println(image[xx]);
    if (image[xx] == 1) {
      display.drawPixel(xx % width + x , xx / width + y, color);
       //Serial.println("CA DOIT AFFICHER UN TRUC");
    }
    
  }
  delay(5000);
  display.clearDisplay();
  
  
}
void handleNotFound() {
  digitalWrite(led, 1);
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
  digitalWrite(led, 0);
}

void setup(void) {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(led, OUTPUT);
  digitalWrite(led, 0);
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  display.begin(16);
  display.clearDisplay();
  display.display(0);
  display_ticker.attach(0.002, display_updater);
  display.clearDisplay();
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);

  server.on("/inline", []() {
    server.send(200, "text/plain", "this works as well");
  });

  server.on("/mainpage", []() {
    String s = MAIN_page;
    server.send(200, "text/html", s);
  });

  server.on("/call", HTTP_POST, []() {
    if(server.arg(0) == "1") {
      drawImage(0, 0, SMILE);
      Serial.println("ici 1");
    }
    else if(server.arg(0) == "2") {
      drawImage(0, 0, UNHAPPYSMILE);
      Serial.println("ici 2");
    }
    else if(server.arg(0) == "3") {
      drawImage(0, 0, NEUTRALSMILE);
      Serial.println("ici 3");
    }
    else if(server.arg(0) == "4") {
      drawImage(0, 0, THUMBSUP);
      Serial.println("ici 4");
    }
    else if(server.arg(0) == "5") {
      drawImage(0, 0, THUMBSDOWN);
      Serial.println("ici 5");
    }
    else if(server.arg(0) == "6") {
      drawImage(0, 0, FUCKYOU);
      Serial.println("ici 6");
    }
  });

  server.on("/modify", HTTP_POST, []() {
    Serial.println(server.arg(1));
    Serial.println(server.arg(2));
    uint16_t image[1024];
    for (int i = 0; i < 1024; ++i)
    {
      if ((int)(server.arg(2)[2*i+1]) - 48 == 0)
        image[i]=0;
      else
        image[i]=0x07E0;
      
    }
    drawImage(0, 0, image);
  });

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void loop(void) {
  server.handleClient();
  MDNS.update();
}
