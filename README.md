# LSMS - Library-Simulation-Management-System by Simon
LSMS is a fast, lightweight Library-Simulation-Management-System for educational purposes

## About
This C++ Project's purpose is to get back into the language by creating a fairly complex Library System, that stores user information across different files. 
The focus should lie on the code's architecture and efficiency. Therefore the beauty of the end product might be lagging ;) 

## What's been implemented so far
* A CSV-Parser - featuring a Row, Header, CSVParser, and Error class contained in namespace CSV. The CSV-Parser can be called with a Header Element containing the files structure and a filename to open. If the file does not exist it is created. In case the Header does not match the given header the CSVParser corrects it. Elements inside the CSV file can than be accessed via the .getRow function and the index operators (e.g. [0] or ["SOMEHEADER"])
* A User Class that takes care of registration, login, and instantiating the Userinfo - it also provides access to all the user related data
* A Library Class that holds the main loop and provides the basic functions listed below

## Features
* Rent a book -> The function looks up the availability and updates both the userinfo and the inventory
* Read a book -> still in progress
* Show my books -> function mainly provided by the userclass which returns a string with all the public infos
* List books 
* Buy a book -> same as rent a book - this function only writes the book to the `owned` category in userinfo
* return a book (unavailable)
* Different User types -> User is fully supported while Publisher is only implemented on a surface level

## File structure
Using SQL for that project would have probably been the smarter way, but that never was the purpose, right?!
I wanted to have to come up with my own way of organizing data and handling it in my own C++ functions in order to refresh my knowledge regarding the reading and writing to a file. So here's how I organized the Userinfo:
* The ```User``` Class reads and writes users in data/Userfile.csv in the following order ```USERNAME,PASSWORD,UID``` whereas ```UID``` is a unique 7 letter string with either a 'U', 'A' or 'P' at the beginning (for User, Admin, Publisher) followed by a unique six-digit number
* The ```UID``` can be found in the data/Userinfo.txt file also handled by the ```User``` class. The information here is presented like this
```
NextPosition~===========UID======
Name:Heinrich,Musterman
Books:
BID,DATE,POSITION
BID,DATE,POSITION
-----------------------------------------------------------------------------
Owned:
BID,DATE,POSITION
---------------------------------------------------------------------------------------------------
```
```------``` These serve as space holders making it easier to write to the file since I can simply overwrite them instead of having to create a .tmp copy of the entire file
* Finally the ```Library``` Class has access to data/Inventory.csv with this file structure ```BID,NAME,AUTHOR,COPIES,RENTED,LINK```. The ```BID```s mentioned in Usersinfo.txt match one of the books in the inventory which in return holds the path (```LINK```) to the books file in data/books

## Installation and Usage
* Download the Code from Github - see Releases for latest Version (I advise you let the data/ folder where it is - no handling included for that yet)
* Run the .exe or compile the code yourself using VSCode and g++ or MVSC (other compilers and IDEs have not been tested yet)
* Follow the program's instructions
This should work out of the box. If not please don't hesitate to hit me up. ! Note that this program makes use of C++17 features so if your machine doesn't support it you might run into problems on compilation!

## Issues and bugs
If you find any type of bug please raise an issue here with all the necessary details you can provide so that I can replicate and fix it. Thanks in advance, that really helps ;)
Also, don't hesitate to raise new Issues with feature requests or ideas!

### What to do if nothing works anymore
Press CTRL+C go into data/ and delete the Userinfo.txt and Userfile.csv. On the next startup, the program should simply create new files and it should run again. 
But don't forget to share the issue you ran into ;)

## Development 
Development happens in feature branches like ```feat69``` so that the main branch always is funtional. Before the feature branch is ready to merge bugs must be fix in specific bug branches marked with ```iss``` or ```bug```.

## Contribution
...always welcome. Please feel free to fork this repo and train your own C++ skills with it! Or if you wish to share your ideas here :) !Note that I will upload Contribution guidelines in the future. So if you want to contribute now please keep in mind the overall structure of the code. Still I'm always up for critic!
