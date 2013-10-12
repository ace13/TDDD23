TDDD23
======

The game I'm developing for my TDDD23 - Design and Programming of Computer Games course.


Idea
----

The game is a spinoff from the old artillery type games where two players took turns aiming and shooting at eachother, only this game is set in space with planets and spaceships. Gravity will play a big role and weapons will be more powerful.

Dependencies
------------

Since I'm writing this using C++11 (or as much of C++11 as my compiler supports in the case of windows), you need a recent enough compiler that it supports the features I'm using. GCC 4.3 or newer is a good start on Linux. Visual Studio 2010 will work on Windows though 2013 should probably be recommended, not that I'm using it myself. Clang I have no idea about right now, but I'd say go with a recent version there too.

It uses the following libraries, with the version(s) I'm using inside of parentheses:

Angelscript (2.25 and 2.28 SVN) - http://www.angelcode.com/angelscript/
Box2D (Revision 255 SVN) - http://box2d.org/
SFML (Git rev 86897a8 and 44b98b4) - http://sfml-dev.org/
