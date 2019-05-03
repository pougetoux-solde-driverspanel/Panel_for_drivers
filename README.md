# Panel_for_drivers

## Concept

Our goal for this scholar project was to create a device that can allow the driver of a vehicle (car, motorcycle, bike, etc) to transmit some emotions to the driver(s) behind him.  
We choose a graphic representation of this emotions, so that they can be displayed easily with a led panel, with the utilisation of emoticons. For example, with our device, the user could print on the led panel behind the vehicle :  

* An angry emoticon (if some car behind him is too close, etc)
* A thumbs up (to thanks some other driver for a good behavior)
* Some text
* Anything he want to express to an other driver !    

It should be possible for the user to control the led panel with the help of a website, that can be accessed on the same network that the device is connected.  
With this website, the user will be able to call 6 patterns (with the help of buttons), that will be transmitted to the led panel.  
6 others buttons will be dedicated to the modification of this patterns individually. The user can modify a pattern by choosing a color, and selecting each pixel that should display this color. For a reason of simplicity, only one color will be displayed in the led panel. Then, he'll can save his modification, and on the next click on the button of this pattern, the new one will be displayed.

## Realisation

Here we can see how is it possible to call a pattern on the website and it is displayed on the led panel :  


And here we can see the adding of a new pattern and its association with a button. Then, clicking on the button display the new pattern on the led panel :  

![add](https://zupimages.net/up/19/18/5o1o.gif)

## Equipment

### Led Panel

![Led panel](https://zupimages.net/up/19/18/dzn4.png)

__Link__ : https://hackspark.fr/fr/affichage/986-32x32-rgb-led-matrix-panel-p4-high-resolutions.html  

We choose this led panel for our project because we were next to the store "HackSpark" that sell it, and it was pretty easy to use a all in one led matrix panel, rather than to make our own. Indeed, some soldering and electric cabling are very time consuming and our delay for this project was about 4 days, so it was the perfect option. 32x32 was enough for our utilisation, because the led are very brightful and it's not needed to have a big panel for the other drivers to see it.

### ESP8266

![Arduino](https://www.jaycar.com.au/medias/sys_master/images/9229382647838/XC3802-wifi-mini-esp8266-main-boardImageMain-515.jpg)

__Link__ : https://www.jaycar.com.au/wifi-mini-esp8266-main-board/p/XC3802  

For the arduino part, the use of an ESP8266 was logical : it is cheap, easy to configure, and have the two abilities that we wanted to use (can be connected to the net, and can host a website).  
Furthermore, our project teacher give us some equipment, and the ESP8266 was a part of it, so no need the search for an other chip more adapted.

### Power supply

![Power supply](https://zupimages.net/up/19/18/ey8e.png)  

The power supply is an alimentation that deliver 5v and can be plug into any electrical outlet.  
We plug into the port of this alimentation a converter that allow us to directly link the positive and negative pole to the led panel, as we can see with this picture :

![Converter](https://zupimages.net/up/19/18/opml.png)  

For the arduino part, it can be plugged with a micro-usb to some electrical outlet, to a computer, etc.

## Libraries

### PxMatrix  

We use this library to control the led matrix panel.  
It really suited our configuration because it's for a 32x32/64x32 led panel coupled with an ESP8266.  
Furthermore, it include a tutorial to manage the cabling of the project, wich really help us to make it work.

__Link__ : https://github.com/2dom/PxMatrix

### ESP8266

For the ESP8266 part, we use the tutorial of our project teacher, which explain what libraries to install and how to install it to connect the ESP8266 to the computer. The libraries installed during this tutorial contains all of the examples needed to host a website on the arduino.

__Link__ : https://github.com/honnet/itp  


## Cabling  

### Led panel  

![led panel](https://zupimages.net/up/19/18/lo90.png)  

As we can see, the cabling follow the tutorial of __PxMatrix__ : the RGB are plugged into the output (except for the R1 which is plugged into the arduino), and all the other cables are plugged into the ESP8266.  
Also, there is the two cables (positive/negative) of the alimentation screw into the panel.

### ESP8266  

![esp](https://zupimages.net/up/19/18/dfo2.png)  

This is the arduino part of the cabling. It is alimented with a micro-usb, and every cable (except for the black of the ground) are plugged into the led panel (following the cabling given in the library __PxMatrix__).

### Breadbox

![breadbox](https://zupimages.net/up/19/18/n924.png)  

Here is the part where ground cables are connected. One is pointing to the ground of the arduino, and the others are pointing to the ground of the led panel (3 cables).

## Code

The main file for the project is the server-code-project.ino (in the folder of the same name). It should then be televersed into the ESP8266.  
There is 2 modifications that are needed for the project to work, before the televersing :

### Changing network credentials  

```
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
```

At the top of the file there is theses includes. The network name is contained in the STASSID, and the password in the STAPSK. To reproduce this project and before televersing the code, you need to modify these two constants to match your own network.

### Changing IP in AJAX Request  

```
const ip = "http://192.168.43.38/";
``` 

There is three AJAX request that point to the same server. Instead of hard-coding the ip for each, we use a constant named ip at the start of the javascript part (line 170).
You need to change it to your ESP8266 IP if you want it to work for you.  
To get the IP :
* Televerse the code in the ESP
* Open the Serial port
* See the IP be printed in it
* Change it in the code at line 170
* Televerse again the code in the ESP  

__Care__ : each time you connect the arduino to a new network, the ip need to be changed.

## Website

Now that the code is televersed inside the ESP8266, the main page of the website is accessible at http://your_ip/mainpage.  
__Note__ : You should be on the same network as the arduino for you to access the website 

## Opening

To transfer data, we used a website hard-coded in html/css/jquery inside the .ino file. Everytime an user call for the mainpage, the server send a string that contains all this code. The jquery rendered in the browser make ajax request into the same server (so no problem of CORS requests), which are received by the server on the ESP8266.  
This technique is really easy to proceed and we had results very quick, but there is an important problem which forced us to reformat our code multiple times : we could'nt send very much data between jquery and the server. So it was not possible for us to transfer a design with multiple colors, and we were forced to make only monochrom pattern.    

One of the multiple solutions that we could use if we had more time is to find a server that allow the transfer of bigger data, or have a configuration file that contain the maximum length of sent data and can be modified. An other could have been to separate our requests to send only a line each time. We tried that last solution, it worked but it was very slow, so we make the choice to keep eficacity more than beauty. Finally, the solution could be to use a different arduino, much faster, with an external wifi module. Like that, there is no problem of rapidity and any architecture would have worked !

