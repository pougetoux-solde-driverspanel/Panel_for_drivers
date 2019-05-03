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

## Equipment

### Led Panel

![Led panel](https://zupimages.net/up/19/18/dzn4.png)

__Link__ : https://hackspark.fr/fr/affichage/986-32x32-rgb-led-matrix-panel-p4-high-resolutions.html  

We choose this led panel for our project because we were next to the store "HackSpark" that sell it, and it was pretty easy to use a all in one led matrix panel, rather than to make our own. Indeed, some soldering and electric cabling are very time consuming and our delay for this project was about 4 days, so it was the perfect option.

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

## Library

## WebSite
