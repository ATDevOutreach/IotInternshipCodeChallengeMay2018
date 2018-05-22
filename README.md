# IoT Internship Code Challenge - 22nd May 2018
## Due: 23rd 9:00am May or Earlier


##### There are two tasks in this challenge, choose between the two. The submission instructions are specified at the bottom of the challenge page.

## Task 1
Write an IoT application that generates climatic data(wind-speed, humidity and pressure).

### Steps:
- Write the climatic data generation library in any preferred language.
- Write extensive unit tests for the climatic data generation  library.

###### Hints:
- Humidity is given as : val hum = 100.00 * ((611 * exp(5423 * ((1 / 273)) - (1 / (dp + 273)))) / (611 * exp(5423 * ((1 / 273)-(1 / temp))))) : where temp is any randomly generated floating point number (temperature) between 15 and 42. dp   is a value - dew point given by  val dp = (100.00 - relhum) / 5 ,where relhum is a random number (relative humidity) between 55.00 and 75.00

- Wind- speed is given as : val ws = ((0.0001) * long : where long is any longitude value randomly generated between 98.00 and 102.05 

- Pressure is given as : val press = 101325 * exp(((0.00 - 9.81) * 0.0289644 * (200))/(8.31444598 * (temp + 273))) : see the temperature range above.

- Make sure that your app/routine uses this library.

- Using CMake as C/C++ build tool is highly encouraged.You’re free to use SCons or Gradle or other only where it applies.

- Using GoogleTest(C/C++) or Unity Test Suite(C) for testing is encouraged only where it applies.You’re free to use CPPUnit, BDD, Pytest,TestCafe, Mocha ,EUnit etc where applicable.

- Compile it on any platform e.g Raspberry Pi for Python, Arduino or Intel  Edison for C or C++ or Python, or ESP-X in C/C++ or Lua.
- Use of [PlatformIO](https://platformio.org/) where applicable is a bonus.
- Integrating with  CI/CD tools such as [TravisCI](https://travis-ci.org/)/[CircleCI](https://circleci.com/)/[Bitbucket-pipelines](https://bitbucket.org/product/features/pipelines)/[Appveyor](https://www.appveyor.com/)  will be an added bonus! Attach [badge in the README](http://shields.io/) if you choose to do so.


## Task 2

Write a simple application/routine that can read data from a sensor, send it to an MQTT server(attach a screenshot of data sent as seen by the receiving client).
On the same application add a functionality that allows MQTTBox/ MQTTSpy  or any other MQTT Client application to send commands through an MQTT server to your device and perform an action such as blink LED, spin a motor etc. 
A proper understanding of [MQTT](http://docs.oasis-open.org/mqtt/mqtt/v3.1.1/os/mqtt-v3.1.1-os.html) is required for this challenge.Please read the given requirements below before proceeding.CI/CD integration is a huge bonus and so are unit  tests!

###### Requirements for this task include:
- You can setup MQTTBox through these guides([1](http://workswithweb.com/html/mqttbox/installing_apps.html) , [2](http://workswithweb.com/html/mqttbox/mqtt_client_settings.html)). You also have an option to setup and configure [MQTT-Spy](https://www.eclipse.org/paho/components/mqtt-spy/) as detailed [here](https://github.com/eclipse/paho.mqtt-spy/wiki/GettingStarted) and [here](https://github.com/eclipse/paho.mqtt-spy/wiki) . Pick either or any known MQTT client you’re comfortable with, such the [Mosquitto CLI](https://mosquitto.org/download/) .

- Your username is thor , password is avengers , the hostname is [sungura1-angani-ke-host.africastalking.com](http://sungura1-angani-ke-host.africastalking.com/)  port is 10883 . All your topics should start with thor/data/<your-first-name>/<something-else> . Be sure to replace <your-first-name> and <something-else> with actual values reflecting your actual name and <something-else> should be the topic to publish or subscribe data to or from.For example, if my first name is loki and I’d like my devices to send data to or read data from a topic called ragnarok, my topic is therefore thor/data/loki/ragnarok . There are no SSL certificates exchanged between your client/device and the server so ensure that the section on SSL/Certs is left unchecked or ignored. Note that you’re free to create any topic namespace after your first-name  e.g. thor/data/loki/temp or thor/data/jane/humidity/room1 etc. No special characters allowed. Ensure to pass a client-id(any string of text representing your device name) when making any connection to the server.

- For Arduino-like environments including ESP-X or Edison(Arduino) [use our library](https://github.com/TheBeachMaster/AfricasTalkingIOT-dist) . For C++ projects (Pi, Edison,BBX etc) use [our other library](https://github.com/TheBeachMaster/AfricasTalkingIoT-cpp) . See provided links for other languages (list not complete feel free to look for libraries for other languages):
- Python [link](https://github.com/eclipse/paho.mqtt.python)
- JS [link](https://www.eclipse.org/paho/clients/js/)
- Rust [link](https://github.com/eclipse/paho.mqtt.rust)
- Java [link](https://www.eclipse.org/paho/clients/java/)
- C [link](https://github.com/eclipse/paho.mqtt.embedded-c)
- C# [link](https://github.com/eclipse/paho.mqtt.m2mqtt)

- Attach [Fritzing](http://fritzing.org/) schematics and relevant wiring diagrams in a folder named wiring  in your project root path for your hardware setup.


##### How to submit
1. Check out the format for submitting your code [here](http://atdevoutreach.viewdocs.io/IotInternshipCodeChallengeMay2018/CodeChallengeSteps/)

2.  Make sure when creating a branch to use your correct phone Number, as this is what we will use to get back to you.

> NB: As a branch-name you can also use your email.
> See you on the other side, and best of luck!


## Slack
In case you have any questions, join our Slack [here](https://slackin-africastalking.now.sh/) and join the #internship-challenge channel.

#### About Africa's Talking Code Challenges
Please read the overview [here.](http://atdevoutreach.viewdocs.io/IotInternshipCodeChallengeMay2018/)
