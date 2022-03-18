# Wordle Aidle
Wordle has been the hype of the internet for January and February 2022, and I have jumped into the bandwagon of playing the game. However, sometimes guessing a word each day may take too much time (apparently 5 minutes each day is a lot for a game.. 😐) and I would like to have something that automatically helps me solve today's Wordle, so that I won't lose my streak.

Therefore like a lot of programmers who jumped into the bandwagon, I've made a tool to solve Wordle, hopefully better than I can 😅.

## The Game of Wordle
Chances are if you came here, you probably don't need any introduction to the game. However, here's a brief explaination of how the game works.

The game starts with a 6 by 5 grid, each would be filled with a letter. Your goal is to guess a mystery 5-letter-word which is determined daily. You are to make a guess without any hints from the start. For each entry you've entered, you will receive hints on whether a letter exists in the mystery word, as well as whether if it is in the right position.

The hints are colour coded as follows:
 - Green - Letter is in the word and in the correct position
 - Yellow - Letter is in the word but in the wrong position
 - Grey - Letter is not in the word

You are to guess the mystery word making use of the hints in 6 tries. You will pass if you've guessed the word in under 6 tries, or if failed the mystery word will be revealed.

You can play the [Wordle game here](https://www.nytimes.com/games/wordle/index.html).

## Application
The solver application is named Wordle Aidle — inspired from Wordle, which was inspired creatively by the creator's name, Josh Wardle. It is a Command Line Interface (CLI) application that involves the Terminal. However, unlike many CLI applications, this application does not take in any arguments (not yet), but it prints out menus with several options for user to input, and navigetes to the feature the user has chosen.

## New York Times crackdown on [Wordle Archive](https://metzger.media/games/wordle-archive/) (Disclaimer)
*[[Ars Technica Article](https://arstechnica.com/gaming/2022/03/new-york-times-takes-down-third-party-wordle-archive/)]*

Recently, The New York Times (NYT) has requested to shut down Wordle Archive, because (as quoted by a New York Times representative):
> "The usage was unauthorized, and we were in touch with them"

The representative also said, "We don't plan to comment beyond that," leaving us with no clues on what are the fates of Wordle imitations.
It's the first time that the NYT has actively taken down a Wordle-like game. This might indicate a threat for all existing Wordle clone games, especially those that use the official Wordle list like this solver.
The site now displays a page saying "Thanks for playing" and "Sadly, the New York Times has requested that the Wordle Archive be taken down."

![archive_closed](https://user-images.githubusercontent.com/43104884/158791196-bdf5dfea-2d29-40bf-be3e-698766edbeaf.png)

The NYT had also filed for a trademark for Wordle when they bought the game, but there's no evidence that the trademark had been enforced.

However, since this project does not have large attention, I will include a feature to allow you to play past Wordle words. This keeps the archives accessible for the time being. I will also be adding a warning for each of the games that are logged to make it clear that only official scores on the NYT are legitimate.

*Speculation: It is possible that the NYT is intending to put ads on Wordle to earn ad revenue, and any clone games are a threat to their business.*

In case of a threat by the NYT, the name of this tool is subject to change, and some of the solver's features, or the entire solver itself, might have to be removed.

## Binaries Download
The binaries are available to download in [releases](https://github.com/snqzspg/wordle-aidle/releases). Note that the application is not signed and therefore will be recognised as risky and being blocked from execution, as the owners of the operating systems did not check them. You will have to get around the block in order to use the binaries. 

Note that because there's no real way other than to get your trusted programming expert friends to decompile the binaries, trusting is the only thing you can do if you want to use the binaries directly. It is therefore suggested, should you feel uncomfortable with this, to build from source, and get your trusted programming expert friend to check that it is doing nothing suspecious. 😅

### Why are they not signed?
Simple answer: **I DO NOT want to PAY** for something trivial like getting this tool to be signed.
### Windows
Windows version is provided as an `.exe` file. Simply download it and run the file.

Since this tool is not commonly downloaded, the Windows Smartscreen will block the file from opening. You have to click on "More Info" to reveal the options to run the file.

The Windows executable is a 32-bit application.

### macOS
There are two versions, `intelx86_64` and `arm64`. They are for intel and M1 macs respectively. They are labelled as the file name.

Once downloaded, you can extract the application inside the zip files. Before you can run the file in any fashion, you have to right click on the application and click "Open". This will give you the option to tell the Gatekeeper to unblock and run the file.

![gatekeeper-guide](https://user-images.githubusercontent.com/43104884/158795352-c50697a7-ee68-48be-b9e5-63409f9f631d.png)

Once successful though, it is advised to close the application and re-open it from a terminal instead of double-clicking. Open Terminal and navigate to the file's location by typing `cd <path>`, then type `./waidle` to run.

Intel version of this app can currently run on M1 macs, but the arm version is not compatible on Intel macs.

### Linux
The binaries is compiled using Ubuntu Windows Subsystem for Linux (WSL), and is only tested on Ubuntu WSL and Arch Linux. It is recommended for you to build from source with the guide below.

But if you want to use the binary instead you can download the respective versions for your system. Then open Terminal/Console and navigate to the file's location by typing `cd <path>`, then `chmod +x waidle` to unblock the file. Then type `./waidle` to run.

## Building the Solver Application
As of now the solver does not include any binary files (thou for Windows there will be soon.. but not sure how soon), so if you want to use it you will have to download the source code and build it yourself.

It is not as hard as it sounds though..

In order to build this application you will need to download the GNU Compiler Collection (GCC) and make, then install it into your system. The installation of GCC and make differs depending on the Operating System (OS) you are running.

### Windows
For Windows it is recommended to use [Minimalist GNU for Windows (MinGW)](https://www.mingw-w64.org/). You can download the command line tools required [here](https://www.mingw-w64.org/downloads/#mingw-builds) (Click on the Sourceforge link). Once installed you should be able to use commands like gcc and make (sometimes it could be 'mingw32-make' or 'mingw64-make'). Open Command Prompt (or PowerShell) and test the two commands out.
#### GCC expected output
```
gcc: fatal error: no input files
compilation terminated.
```
#### Make expected output
```
mingw32-make: *** No targets specified and no makefile found.  Stop.
```
To build, you will need to navigate to the root directory of the clone by typing `cd ` and dragging the folder into the prompt window. Then type `make for_windows` (or `mingw32-make for_windows` or `mingw64-make for_windows`) and it will automatically build the application. After it's done, the executable `waidle.exe` will be generated.
### macOS
To install GCC and make for macOS you will need to install Xcode Command Line Tools. You can install it using the terminal command `xcode-select --install` and follow the prompt instructions. Alternatively you can also use homebrew to install Xcode Command Line Tools. You can also do [a quick google search](https://www.google.com/search?q=xcode+command+line+tools) on how to install.

After installation you should see output similar to the ones shown in the Windows section above when you type `gcc` and `make`.

To build, navigate to the base directory using Terminal and simply type `make for_mac`. After building, you can type ./waidle to run.
### Linux
For linux users, use your distro's package manager to install GCC and make. Example `sudo apt install gcc` for debian/ubuntu-based systems, or `sudo pacman -S gcc` for arch-based systems.

To build, navigate to the base directory using Terminal/Console and simply type `make for_linux`. After building, you can type ./waidle to run.

### Cleanup
The folder `__build_tmp__` will be created during the building process. You can manually delete the folder after building or run `make clean_windows` or `make clean_linux` to remove the folder.

**NOTE:** Running the above command will delete **everything** inside the `__build_tmp__` folder, including any new stuff you've put inside or stuff you happen to have in the folder before building.

## Features
This app is hardcoded with all 12972 valid words that Wordle accepts as answers, including the 2309 Wordle answers. 

*Note that there are other programmers stating that there are 2315 words. That is because they were fetching from an old word list from Wordle before NYT deleted words that are considered sensitive or offensive. The 25 deleted words are in a comment in the hardcoded dictionary source file.*

In addition, there are 5812 additional hardcoded words that are not considered valid for Wordle, collated from different sources. The sources are in the comments of the hardcoded dictionary source file.

*This project is having its first days on GitHub. It need time to adopt to it's new envrionment.
Stay tuned as this page is in the progress of making. The rest is to be written...*
