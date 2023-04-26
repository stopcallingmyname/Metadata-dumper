# Metadata-dumper
This repository contains a DLL library that can be used to dump the encrypted "global-metadata.dat" file of a game created using the Unity engine. The code provided in the repository is written in C++ and is used to extract the metadata file from the game's assembly DLL.

The main code extracts the metadata from the game's assembly DLL using a specific pattern and saves it as a binary file named "global-metadata.dump.dat". The metadata file is typically used by Unity games to store information about classes, methods, and other game-related data. This library can be useful for developers or researchers who want to analyze the internal structure of Unity games.

# How-To-Use
To use this library to dump the encrypted metadata file of a Unity game, you need to follow the steps below:

- Download or clone the repository from GitHub.
- Build the DLL library using Visual Studio or any other compatible compiler. 
- Make sure to select the correct configuration (x86 or x64) based on your system and game.
- Open the Unity game's assembly DLL in a disassembler such as dnSpy.
- Identify the function that loads the metadata file and note its signature or pattern.
- Inject the built DLL library into the game's process using a tool such as Extreme Injector or Cheat Engine.

Note that this process requires some knowledge of reverse engineering and DLL injection. 
It should only be done for research or educational purposes and not for any illegal or malicious activities. 
Also, keep in mind that not all Unity games may use the same metadata file encryption scheme or file path, so some modifications may be required based on the game being analyzed
