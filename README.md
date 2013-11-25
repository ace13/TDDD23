TDDD23
======

The game I'm developing for my TDDD23 - Design and Programming of Computer Games course.

It's written in C++11 using SFML, Box2D, and Angelscript.


Idea
----

The game is a spinoff from the old artillery type games where two players took turns aiming and shooting at eachother, only this game is set in space with planets and spaceships. Gravity will play a big role and weapons will be more powerful.

Dependencies
------------

Since I'm writing this using C++11 (or as much of C++11 as my compiler supports in the case of windows), you need a recent enough compiler that it supports the features I'm using. GCC 4.3 or newer is a good start on Linux. Visual Studio 2010 will work on Windows though 2013 should probably be recommended, not that I'm using it myself. Clang I have no idea about right now, but I'd say go with a recent version there too.

It uses the following libraries, with the version(s) I'm using personally inside of parentheses:

* Angelscript (2.25 and 2.28 SVN) - http://www.angelcode.com/angelscript/
* Box2D (Revision 255 SVN) - http://box2d.org/
* SFML (Git rev 86897a8 and 44b98b4) - http://sfml-dev.org/

Building
--------

You need to get CMake, at least version 2.5, and run it using this folder as the source directory.
CMake can generate makefiles for many operating systems and IDEs, anything from basic *nix makefiles to Visual Studio solutions for Windows.

When you've built the game you also need to copy over the content of the resources folder, so that they live next to the binary.

Not-so-frequently Asked Questions
---------------------------------

Q: But where are the nice graphics?
A: I'm not an artist.

Q: How about sounds then?
A: Still not an artist.

Q: ...music?
A: I'll let you figure this one out yourself.
