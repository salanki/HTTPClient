# HTTPClient for Arduino WiFi Shield

## Overview

An Arduino HTTP Client that uses the Arduino WiFi Library to make HTTP requests
and handle responses. This is a fork of the HTTP Client library at https://github.com/interactive-matter/HTTPClient . Except for the inital setup all API functions are the same (for GET, POST and so on), please refer to their documentation.

## Usage
The only thing that differs from the original implementation is the initiation, you supply only the hostname of the server in the constructor.

```c++
HTTPClient client("www.google.com");
```

There is an HTTP Get example included that will show how to get started and also some tricks in error handling.  

The Arduino WiFi Library is currently very poor. The performance is lacking and it is not stable. There are some watchdog tricks included in the example but in some cases you might have to reset the entire arduino and shield externally (either via button press or using the RESET pin). 

The WiFi library currently does not support running a server at the same time you are running a client, if you are intending to build a sketch which both acts as a server and makes client connections it will lead to a lot of headache and misery.
