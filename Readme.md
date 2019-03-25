# Assignment 2 - Simple UDP Client Server Application

  ![logo_c ]
  # Features :
  - The **client** program reads input from user and performs the following actions repeatedly until directed to exit:
  - [X] If the user enters 's', it will ask user to enter a string. It then sends "xxx\n"to the server(where "xxx" is the string entered by the user, which could includeany character except"\n"), and receives a message from the server and displaythe message 
  on the screen.
  - [X] If the user enters 'q', it will exit
  - [X] If the user enters anything else, it will ignore
  - [X] If it receives a message from the server at any time, it will print the received message in a new line.
  - [X] It will automatically send"\n" to the server every 5 seconds.S

  - The **server** program performs following operations:
    -[X] Whenever a message that is not "\n"is received from a client, the server sends the same message back to all known active clients(i.e. the clients that the server has received messages fromwithin the past 15 seconds), including the one the message is received from.

  # To Build client and server program:
  ``` sh
  $ cd nwp-project2-udp-sockets/client
  $ make
  $ cd nwp-project2-udp-sockets/server
  $ make
  ```
  Run  ```make clean``` in ```client``` and ```server``` directories to clear the compiled binaries.
  # To Run:
  > Run client as:
  >>``` sh
  >>$ cd nwp-project1
  >>$ ./client.out xxx.xxx.xxx yy
  >>```
  > Run server as:
  >>``` sh
  >>$ cd nwp-project1
  >>$ ./server yy
  >>```

  Where ```xxx.xxx.xxx``` is the server IP address and ```yy``` is the port number. 
To use the client software, just follow the prompts on screen :)

  # Assumptions:
  * Maximum number of client connections allowed is defined 
  * Maximum length of message to be exchanged is defined
  * These macros are defined in respective *helper.h* files

 [logo_c]: <https://seeklogo.com/images/C/c-programming-language-logo-9B32D017B1-seeklogo.com.png>
