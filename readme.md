# saucebot

saucebot is an nhentai client app with a GUI controlled (partly) by a command line.  
Currently, it has reached version 0.3 "エッチ" (ecchi).  

## Current Features

saucebot, primarily, includes a number viewer and downloader. Searches will be coming soon.

## Building saucebot

saucebot is a windows app, and thus you will need to be on windows to build and use it.  
You will need `gcc`, `windres` (windows resource compiler), and GNU Make.  
I suggest getting MinGW to get `gcc`, `windres`, and GNU Make.
To build, go to the main folder and type `make` into a command prompt. The executable will be found at `.\bin\saucebot.exe`.
As of right now, saucebot is not being built for release. Change the define in src/main/saucebot.c to define release. This will prevent the console window from opening.

## License

saucebot is licensed under the [MIT license](https://choosealicense.com/licenses/mit/).
