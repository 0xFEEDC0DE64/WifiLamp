# WifiLamp
I have a lamp in my room where I intercepted the powerline using a relais and ESP2866 to control it from my phone/amazon dash button.

The micro controller connects to my home WiFi and starts a webserver to control it. APIs exist to simplify control from other software components like [android app](https://github.com/Waboodoo/HTTP-Shortcuts).

![Screenshot](/screenshot.png)

## Parts
1. [Wemos D1 Mini](https://wiki.wemos.cc/products:d1:d1_mini)
2. [Wemos D1 Mini Relais Board](https://wiki.wemos.cc/products:d1_mini_shields:relay_shield)
3. [IKEA Aläng Lamp](https://www.ikea.com/at/de/catalog/products/60176039/)
4. Random 5V 1A Phone charger with USB plug unsoldered (see photo)a
5. (optional) [Amazon Dash Button](https://www.amazon.com/gp/help/customer/display.html/ref=hp_left_v4_sib?ie=UTF8&nodeId=201746300)

![Installation inside lamp](/photo.jpg)

## Software components
1. There is the code running on the micro controller. The code is [here](/WifiLamp.ino). I compiled it using the Arduino IDE. There is some setup needed to get it running on ESP2866. I don't know how to set it up anymore. I also implemented some remote update functionality to apply updates via WiFi.
2. I could not fit the switch on/off images into the firmware, so I moved them to a CDN apache webserver in my LAN. The contents of the cdn are [here](/cdn/).
3. To control my light using a (free) amazon dash button, i have a small nodejs script running inside a 256MB Ram ubuntu vm on my home server. The code is [here](/amzn-dash-lamp.js). I hate scripting languages like JavaScript and I have no clue how to get it running on your machine. There is a dependency to the amazon dash button library. I don't know how to install it. That's why I have a VM that I can backup!

## Other implementations
You can use the same circuit / software in other projects too. I made a WiFi-controlled power plug too!

![](/photos/IMG_20180407_141424_817.jpg)
![](/photos/IMG_20180407_162012_269.jpg)
![](/photos/IMG_20180407_141145_516.jpg)