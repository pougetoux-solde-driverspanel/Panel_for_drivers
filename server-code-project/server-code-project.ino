#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <PxMatrix.h>
//#include <FS.h>
#include "DefaultImages.h"
#ifndef STASSID
//#define STASSID "Private123"
//#define STAPSK  "19911991"
#define STASSID "Robin"
#define STAPSK  "0123456789"
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
uint16_t custom1[1024];
uint16_t custom2[1024];
uint16_t custom3[1024];
uint16_t custom4[1024];
uint16_t custom5[1024];
uint16_t custom6[1024];
bool custom1Bool = false;
bool custom2Bool = false;
bool custom3Bool = false;
bool custom4Bool = false;
bool custom5Bool = false;
bool custom6Bool = false;
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
                        <select id="pattern-select">
                            <option value="">--Sélectionner une option--</option>
                            <option value="fuck">Fuck</option>
                            <option value="neutral">Neutral smile</option>
                            <option value="smile">Smile</option>
                            <option value="thumbsu">Thumbs up</option>
                            <option value="thumbsd">Thumbs down</option>
                            <option value="unhappy">Unhappy smile</option>
                        </select>
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
                $('select').on('change', function() {
                    for(var i = 0; i < 32; i++) {
                        for(var j = 0; j < 32; j++) {
                            $("#griddiv")[0].children[0].children[i].children[j].style.backgroundColor = "";
                        }
                    }
                    switch(this.value) {
                        case "fuck":
                            var fuck = "4,15;4,16;4,17;4,18;4,19;5,15;5,18;5,19;5,20;6,15;6,20;6,21;7,15;7,21;7,22;7,23;7,24;8,16;8,24;8,25;8,26;8,27;8,28;9,12;9,13;9,14;9,15;9,16;9,17;9,18;9,19;9,28;9,29;10,11;10,29;11,11;11,29;12,11;12,29;13,3;13,4;13,5;13,6;13,7;13,8;13,9;13,10;13,11;13,12;13,13;13,14;13,15;13,16;13,29;14,2;14,29;15,2;15,29;16,3;16,4;16,5;16,6;16,7;16,8;16,9;16,10;16,11;16,12;16,13;16,14;16,29;17,10;17,29;18,10;18,29;19,10;19,29;20,11;20,12;20,13;20,14;20,15;20,16;20,17;20,29;21,11;21,29;22,11;22,29;23,12;23,29;24,13;24,14;24,15;24,16;24,17;24,18;24,19;24,20;24,21;24,22;24,23;24,24;24,25;24,26;24,27;24,28;24,29";
                            fuck.split(';').forEach(j => {
                                var splitted = j.split(",");
                                $("#griddiv")[0].children[0].children[splitted[1]].children[splitted[0]].style.backgroundColor = "#FF0000";
                                $("#colorpicker")[0].value = "#FF0000";
                            });
                            break;
                        case "neutral":
                             var neutral = "0,12;0,13;0,14;0,15;0,16;0,17;0,18;0,19;1,9;1,10;1,11;1,12;1,13;1,14;1,15;1,16;1,17;1,18;1,19;1,20;1,21;1,22;2,8;2,9;2,10;2,11;2,12;2,13;2,14;2,15;2,16;2,17;2,18;2,19;2,20;2,21;2,22;2,23;3,6;3,7;3,8;3,9;3,10;3,11;3,12;3,19;3,20;3,21;3,22;3,23;3,24;3,25;4,5;4,6;4,7;4,8;4,9;4,22;4,23;4,24;4,25;4,26;5,4;5,5;5,6;5,7;5,24;5,25;5,26;5,27;6,3;6,4;6,5;6,6;6,21;6,22;6,23;6,25;6,26;6,27;6,28;7,3;7,4;7,5;7,21;7,22;7,23;7,26;7,27;7,28;7,29;8,2;8,3;8,4;8,21;8,22;8,23;8,27;8,28;8,29;9,1;9,2;9,3;9,4;9,21;9,22;9,23;9,27;9,28;9,29;9,30;10,1;10,2;10,3;10,21;10,22;10,23;10,28;10,29;10,30;11,1;11,2;11,3;11,7;11,8;11,9;11,10;11,11;11,12;11,13;11,14;11,15;11,16;11,17;11,21;11,22;11,23;11,28;11,29;11,30;12,0;12,1;12,2;12,3;12,7;12,8;12,9;12,10;12,11;12,12;12,13;12,14;12,15;12,16;12,17;12,21;12,22;12,23;12,28;12,29;12,30;12,31;13,0;13,1;13,2;13,7;13,8;13,9;13,10;13,11;13,12;13,13;13,14;13,15;13,16;13,17;13,21;13,22;13,23;13,29;13,30;13,31;14,0;14,1;14,2;14,21;14,22;14,23;14,29;14,30;14,31;15,0;15,1;15,2;15,21;15,22;15,23;15,29;15,30;15,31;16,0;16,1;16,2;16,21;16,22;16,23;16,29;16,30;16,31;17,0;17,1;17,2;17,21;17,22;17,23;17,29;17,30;17,31;18,0;18,1;18,2;18,7;18,8;18,9;18,10;18,11;18,12;18,13;18,14;18,15;18,16;18,17;18,21;18,22;18,23;18,29;18,30;18,31;19,0;19,1;19,2;19,3;19,7;19,8;19,9;19,10;19,11;19,12;19,13;19,14;19,15;19,16;19,17;19,21;19,22;19,23;19,28;19,29;19,30;19,31;20,1;20,2;20,3;20,7;20,8;20,9;20,10;20,11;20,12;20,13;20,14;20,15;20,16;20,17;20,21;20,22;20,23;20,28;20,29;20,30;21,1;21,2;21,3;21,21;21,22;21,23;21,28;21,29;21,30;22,1;22,2;22,3;22,4;22,21;22,22;22,23;22,27;22,28;22,29;22,30;23,2;23,3;23,4;23,21;23,22;23,23;23,27;23,28;23,29;24,3;24,4;24,5;24,21;24,22;24,23;24,26;24,27;24,28;24,29;25,3;25,4;25,5;25,6;25,21;25,22;25,23;25,25;25,26;25,27;25,28;26,4;26,5;26,6;26,7;26,24;26,25;26,26;26,27;27,5;27,6;27,7;27,8;27,9;27,22;27,23;27,24;27,25;27,26;28,6;28,7;28,8;28,9;28,10;28,11;28,12;28,19;28,20;28,21;28,22;28,23;28,24;28,25;29,7;29,8;29,9;29,10;29,11;29,12;29,13;29,14;29,15;29,16;29,17;29,18;29,19;29,20;29,21;29,22;29,23;29,24;30,9;30,10;30,11;30,12;30,13;30,14;30,15;30,16;30,17;30,18;30,19;30,20;30,21;30,22;31,12;31,13;31,14;31,15;31,16;31,17;31,18;31,19";
                             neutral.split(';').forEach(j => {
                                var splitted = j.split(",");
                                $("#griddiv")[0].children[0].children[splitted[1]].children[splitted[0]].style.backgroundColor = "#FFD000";
                                 $("#colorpicker")[0].value = "#FFD000";
                            });
                            break;
                        case "smile":
                             var smile = "0,10;0,11;0,12;0,13;0,14;0,15;0,16;0,17;0,18;0,19;0,20;0,21;0,22;1,8;1,9;1,10;1,11;1,12;1,13;1,14;1,15;1,16;1,17;1,18;1,19;1,20;1,21;1,22;1,23;1,24;2,7;2,8;2,9;2,10;2,11;2,12;2,20;2,21;2,22;2,23;2,24;2,25;3,6;3,7;3,8;3,9;3,10;3,22;3,23;3,24;3,25;3,26;3,27;4,5;4,6;4,7;4,8;4,24;4,25;4,26;4,27;4,28;5,4;5,5;5,6;5,7;5,21;5,25;5,26;5,27;5,28;6,3;6,4;6,5;6,6;6,20;6,21;6,26;6,27;6,28;6,29;7,2;7,3;7,4;7,5;7,20;7,21;7,22;7,27;7,28;7,29;7,30;8,2;8,3;8,4;8,21;8,22;8,23;8,28;8,29;8,30;9,1;9,2;9,3;9,4;9,22;9,23;9,28;9,29;9,30;9,31;10,1;10,2;10,3;10,7;10,8;10,9;10,10;10,11;10,12;10,13;10,14;10,15;10,22;10,23;10,24;10,29;10,30;10,31;11,1;11,2;11,3;11,7;11,8;11,9;11,10;11,11;11,12;11,13;11,14;11,15;11,22;11,23;11,24;11,25;11,29;11,30;11,31;12,0;12,1;12,2;12,7;12,8;12,9;12,10;12,11;12,12;12,13;12,14;12,15;12,23;12,24;12,25;12,30;12,31;13,0;13,1;13,2;13,23;13,24;13,25;13,26;13,30;13,31;14,0;14,1;14,2;14,24;14,25;14,26;14,30;14,31;15,0;15,1;15,2;15,24;15,25;15,26;15,30;15,31;16,0;16,1;16,2;16,24;16,25;16,26;16,30;16,31;17,0;17,1;17,2;17,24;17,25;17,26;17,30;17,31;18,0;18,1;18,2;18,24;18,25;18,26;18,30;18,31;19,0;19,1;19,2;19,23;19,24;19,25;19,26;19,30;19,31;20,1;20,2;20,3;20,7;20,8;20,9;20,10;20,11;20,12;20,13;20,14;20,15;20,23;20,24;20,25;20,29;20,30;20,31;21,1;21,2;21,3;21,7;21,8;21,9;21,10;21,11;21,12;21,13;21,14;21,15;21,22;21,23;21,24;21,25;21,29;21,30;21,31;22,1;22,2;22,3;22,4;22,7;22,8;22,9;22,10;22,11;22,12;22,13;22,14;22,15;22,22;22,23;22,24;22,28;22,29;22,30;22,31;23,2;23,3;23,4;23,22;23,23;23,28;23,29;23,30;24,2;24,3;24,4;24,5;24,21;24,22;24,23;24,27;24,28;24,29;24,30;25,3;25,4;25,5;25,6;25,20;25,21;25,22;25,26;25,27;25,28;25,29;26,4;26,5;26,6;26,7;26,20;26,21;26,25;26,26;26,27;26,28;27,4;27,5;27,6;27,7;27,8;27,21;27,24;27,25;27,26;27,27;27,28;28,5;28,6;28,7;28,8;28,9;28,23;28,24;28,25;28,26;28,27;29,7;29,8;29,9;29,10;29,11;29,12;29,20;29,21;29,22;29,23;29,24;29,25;29,26;30,8;30,9;30,10;30,11;30,12;30,13;30,14;30,15;30,16;30,17;30,18;30,19;30,20;30,21;30,22;30,23;30,24;31,10;31,11;31,12;31,13;31,14;31,15;31,16;31,17;31,18;31,19;31,20;31,21;31,22";
                             smile.split(';').forEach(j => {
                                var splitted = j.split(",");
                                $("#griddiv")[0].children[0].children[splitted[1]].children[splitted[0]].style.backgroundColor = "#11FF00";
                                 $("#colorpicker")[0].value = "#11FF00";
                            });
                            break;
                        case "thumbsu":
                             var thumbsu = "3,14;3,15;3,16;3,18;3,19;3,20;4,13;4,16;4,17;4,18;4,20;4,21;4,23;4,24;5,13;5,17;5,21;5,22;5,25;5,27;5,28;6,13;6,17;6,21;6,25;6,26;6,29;7,13;7,17;7,21;7,25;7,29;8,13;8,17;8,21;8,25;8,29;9,13;9,17;9,21;9,25;9,29;10,13;10,17;10,21;10,25;10,29;11,2;11,3;11,4;11,5;11,6;11,7;11,13;11,17;11,21;11,25;11,29;12,1;12,7;12,8;12,9;12,10;12,11;12,12;12,13;12,17;12,21;12,25;12,29;13,1;13,13;13,17;13,21;13,25;13,29;14,1;14,2;14,13;14,14;14,15;14,16;14,18;14,19;14,20;14,21;14,22;14,25;14,29;15,3;15,4;15,5;15,23;15,24;15,25;15,26;15,29;16,6;16,7;16,27;16,28;16,29;17,8;17,9;17,29;18,10;18,29;19,11;19,12;19,29;20,13;20,14;20,28;20,29;21,15;21,16;21,28;22,16;22,28;23,16;23,28;24,16;24,28;25,16;25,28;26,16;26,28;27,16;27,28;28,17;28,18;28,19;28,20;28,21;28,22;28,23;28,24;28,25;28,26;28,27";
                             thumbsu.split(';').forEach(j => {
                                var splitted = j.split(",");
                                $("#griddiv")[0].children[0].children[splitted[1]].children[splitted[0]].style.backgroundColor = "#11FF00";
                                 $("#colorpicker")[0].value = "#11FF00";
                            });
                            break;
                        case "thumbsd":
                             var thumbsd = "3,11;3,12;3,13;3,15;3,16;3,17;4,7;4,8;4,10;4,11;4,13;4,14;4,15;4,18;5,3;5,4;5,6;5,9;5,10;5,14;5,18;6,2;6,5;6,6;6,10;6,14;6,18;7,2;7,6;7,10;7,14;7,18;8,2;8,6;8,10;8,14;8,18;9,2;9,6;9,10;9,14;9,18;10,2;10,6;10,10;10,14;10,18;11,2;11,6;11,10;11,14;11,18;11,24;11,25;11,26;11,27;11,28;11,29;12,2;12,6;12,10;12,14;12,18;12,19;12,20;12,21;12,22;12,23;12,24;12,30;13,2;13,6;13,10;13,14;13,18;13,30;14,2;14,6;14,9;14,10;14,11;14,12;14,13;14,15;14,16;14,17;14,18;14,29;14,30;15,2;15,5;15,6;15,7;15,8;15,26;15,27;15,28;16,2;16,3;16,4;16,24;16,25;17,2;17,22;17,23;18,2;18,21;19,2;19,19;19,20;20,2;20,3;20,17;20,18;21,3;21,15;21,16;22,3;22,15;23,3;23,15;24,3;24,15;25,3;25,15;26,3;26,15;27,3;27,15;28,4;28,5;28,6;28,7;28,8;28,9;28,10;28,11;28,12;28,13;28,14";
                             thumbsd.split(';').forEach(j => {
                                var splitted = j.split(",");
                                $("#griddiv")[0].children[0].children[splitted[1]].children[splitted[0]].style.backgroundColor = "#FF0000";
                                 $("#colorpicker")[0].value = "#FF0000";
                            });
                            break;
                        case "unhappy":
                             var unhappy = "0,12;0,13;0,14;0,15;0,16;0,17;0,18;0,19;1,9;1,10;1,11;1,12;1,13;1,14;1,15;1,16;1,17;1,18;1,19;1,20;1,21;1,22;2,8;2,9;2,10;2,11;2,12;2,13;2,14;2,15;2,16;2,17;2,18;2,19;2,20;2,21;2,22;2,23;3,6;3,7;3,8;3,9;3,10;3,11;3,12;3,19;3,20;3,21;3,22;3,23;3,24;3,25;4,5;4,6;4,7;4,8;4,9;4,22;4,23;4,24;4,25;4,26;5,4;5,5;5,6;5,7;5,24;5,25;5,26;5,27;6,3;6,4;6,5;6,6;6,23;6,25;6,26;6,27;6,28;7,3;7,4;7,5;7,22;7,23;7,24;7,26;7,27;7,28;7,29;8,2;8,3;8,4;8,21;8,22;8,23;8,24;8,27;8,28;8,29;9,1;9,2;9,3;9,4;9,21;9,22;9,23;9,27;9,28;9,29;9,30;10,1;10,2;10,3;10,21;10,22;10,23;10,28;10,29;10,30;11,1;11,2;11,3;11,7;11,8;11,9;11,10;11,11;11,12;11,13;11,14;11,15;11,16;11,17;11,20;11,21;11,22;11,28;11,29;11,30;12,0;12,1;12,2;12,3;12,7;12,8;12,9;12,10;12,11;12,12;12,13;12,14;12,15;12,16;12,17;12,20;12,21;12,22;12,28;12,29;12,30;12,31;13,0;13,1;13,2;13,7;13,8;13,9;13,10;13,11;13,12;13,13;13,14;13,15;13,16;13,17;13,19;13,20;13,21;13,22;13,29;13,30;13,31;14,0;14,1;14,2;14,19;14,20;14,21;14,29;14,30;14,31;15,0;15,1;15,2;15,19;15,20;15,21;15,29;15,30;15,31;16,0;16,1;16,2;16,19;16,20;16,21;16,29;16,30;16,31;17,0;17,1;17,2;17,19;17,20;17,21;17,29;17,30;17,31;18,0;18,1;18,2;18,7;18,8;18,9;18,10;18,11;18,12;18,13;18,14;18,15;18,16;18,17;18,20;18,21;18,22;18,29;18,30;18,31;19,0;19,1;19,2;19,3;19,7;19,8;19,9;19,10;19,11;19,12;19,13;19,14;19,15;19,16;19,17;19,20;19,21;19,22;19,28;19,29;19,30;19,31;20,1;20,2;20,3;20,7;20,8;20,9;20,10;20,11;20,12;20,13;20,14;20,15;20,16;20,17;20,20;20,21;20,22;20,23;20,28;20,29;20,30;21,1;21,2;21,3;21,21;21,22;21,23;21,28;21,29;21,30;22,1;22,2;22,3;22,4;22,21;22,22;22,23;22,24;22,27;22,28;22,29;22,30;23,2;23,3;23,4;23,22;23,23;23,24;23,27;23,28;23,29;24,3;24,4;24,5;24,23;24,24;24,26;24,27;24,28;24,29;25,3;25,4;25,5;25,6;25,25;25,26;25,27;25,28;26,4;26,5;26,6;26,7;26,24;26,25;26,26;26,27;27,5;27,6;27,7;27,8;27,9;27,22;27,23;27,24;27,25;27,26;28,6;28,7;28,8;28,9;28,10;28,11;28,12;28,19;28,20;28,21;28,22;28,23;28,24;28,25;29,7;29,8;29,9;29,10;29,11;29,12;29,13;29,14;29,15;29,16;29,17;29,18;29,19;29,20;29,21;29,22;29,23;29,24;30,9;30,10;30,11;30,12;30,13;30,14;30,15;30,16;30,17;30,18;30,19;30,20;30,21;30,22;31,12;31,13;31,14;31,15;31,16;31,17;31,18;31,19";
                             unhappy.split(';').forEach(j => {
                                var splitted = j.split(",");
                                $("#griddiv")[0].children[0].children[splitted[1]].children[splitted[0]].style.backgroundColor = "#FF0000";
                                 $("#colorpicker")[0].value = "#FF0000";
                            });
                            break;
                    }
                });
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
                    url: "http://192.168.43.46/modify",
                    data: {"selected": buttonid, "color": $("#colorpicker")[0].value, "colors": JSON.stringify(colors)}
                });
            }

            function selectoption(button) {
                $.ajax({
                    type: "POST",
                    url: "http://192.168.43.46/call",
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
void copyImage(uint16_t *dest, uint16_t *source)
{
  for (int i = 0; i < 1024; ++i)
  {
    dest[i] = source[i]; 
  }
}
void saveImage(String buttonId, uint16_t *image)
{
 
}
void setup(void) {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(led, OUTPUT);
  digitalWrite(led, 0);
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  //SPIFFS.begin();
 
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
    
    server.send_P(200, "text/html", MAIN_page, 20598);
    
  });
  

  server.on("/call", HTTP_POST, []() {
    if(server.arg(0) == "1") {
      if (custom1Bool) 
        drawImage(0, 0, custom1);
      else
        drawImage(0, 0, SMILE);
      Serial.println("ici 1");
    }
    else if(server.arg(0) == "2") {
      if (custom2Bool)
        drawImage(0, 0, custom2);
      else
        drawImage(0, 0, UNHAPPYSMILE);
      Serial.println("ici 2");
    }
    else if(server.arg(0) == "3") {
      if (custom3Bool)
        drawImage(0, 0, custom3);
      else
        drawImage(0, 0, NEUTRALSMILE);
      Serial.println("ici 3");
    }
    else if(server.arg(0) == "4") {
      if (custom4Bool)
        drawImage(0, 0, custom4);
      else
        drawImage(0, 0, THUMBSUP);
      Serial.println("ici 4");
    }
    else if(server.arg(0) == "5") {
      if (custom5Bool)
        drawImage(0, 0, custom5);
      else
        drawImage(0, 0, THUMBSDOWN);
      Serial.println("ici 5");
    }
    else if(server.arg(0) == "6") {
      if (custom6Bool)
        drawImage(0, 0, custom6);
      else
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
    //drawImage(0, 0, image);
    saveImage(server.arg(0), image);
  });

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void loop(void) {
  server.handleClient();
  MDNS.update();
}
