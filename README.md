# DCap

The Application is a IB TWS API based solution aimed at coding any number of strategies for stock trading.
The Main thread creates threads for child strategies and Market Data Producer thread.

Currrently, we have a test strategy which will be slowly extended to support a large number of straegies owing to the scalable, multi-threaded design.

The application uses the wrapper classes created by Jan Boonen. Here: 
https://github.com/JanBoonen/TwsApiCpp/tree/master/TwsApiC%2B%2B
