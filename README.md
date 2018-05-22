# IoT Internship Code Challenge - 22nd May 2018
## Due: 23rd 9:00am May or Earlier


##### Choose between the two tasks/challenges given below

###### Task 1
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
- Use of PlatformIO where applicable is a bonus.
- Integrating with  CI/CD tools such as TravisCI/CircleCI/Bitbucket-pipelines/Appveyor  will be an added bonus! Attach badge in the README if you choose to do so.
- Your repo name should be Thanos. 

##### How to submit
1. Head over to the code challenge [here](http://atdevoutreach.viewdocs.io/codeChallenge05018/CodeChallenge05018/) to get acquainted.

2.  Check out the format for submitting your code [here](http://atdevoutreach.viewdocs.io/codeChallenge05018/CodeChallengeSteps/)

3.  Make sure when creating a branch to use your correct phone Number, as this is what we will use to get back to you.
> NB: As a branch-name you can also use your email.
> See you on the other side, and best of luck!




###### Task 2

## Slack
In case you have any questions, join our Slack [here](https://slackin-africastalking.now.sh/)

#### About Africa's Talking Code Challenges
Please read the overview [here.](http://atdevoutreach.viewdocs.io/codeChallenge05018/)
