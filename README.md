# Hangman

Client-Server Hangman Game final project for the MC833 course @Unicamp

## Getting Started

Download or clone the repository at https://github.com/fakegermano/hangman

### Prerequisites

The program runs entirely on C, so that's all you need for it to function properly!

### Preparing to play

After you clone the repository, on the main folder run the following command:
```
make
```
And a folder called `build` should be created.
Then, on the build folder, first run the server with any valid value for `PORT`

```
./server {PORT}
```
Open another terminal tab and in the same folder run the following command:
```
./client 127.0.0.1 {PORT}
```
With the same `PORT` value as used in the server.
You can open as many clients as you want! Each of them will play a different game with different answers!

## Running the game

As soon as you run the game, you are given the Main Menu screen:
```
============================
Welcome to the Hangman Game!
Please choose an option
============================

1) Play Single-player mode
2) Be hangman on next match
3) Play Multi-player Mode
'-h' for our help menu"
```

If you press the `-h` command you will be shown a summary of the rules of the game and the name of the developers.
If you press `1` a new game will start. As soon as the game starts, the server app will print on the terminal the answer (but you should try to see if you can guess right without cheating!)

At present time the `2` and `3` options are not fully implemented yet.

## Example screens
```
===========================
Welcome to the Hangman Game
===========================
The hangman has a word for you: _._._._
You have 6 lifes remaining
Guess a letter ([a-z|A-Z]):
Press '0' to exit to Main Menu
```
```
===============================================
THE HANGMAN SAYS:
Letter not present in _.a._.a._._
===============================================
=============================================
+-----------------------------------+
|(A)|(B)| C |(D)| E | F | G | H | I |
+-----------------------------------+
| J | K | L | M | N | O |(P)| Q | R |
+-----------------------------------+
| S | T | U |(V)| W |(X)| Y | Z |   |
+-----------------------------------+
===========================
Welcome to the Hangman Game
===========================
The hangman has a word for you: _.a._.a._._
BE CAREFUL! You only have 1 life remaining
Guess a letter ([a-z|A-Z]):
Press '0' to exit to Main Menu
```
```
===============================================
THE HANGMAN SAYS:
YOU WON! THE WORD WAS: m.a.n.a.g.e
===============================================
Press '0' to exit to Main Menu
```
## Authors

* **Giovani Garuffi Rondel** - *RA - 155559* - (https://github.com/giogaruffi)
* **Daniel Germano Travieso** - *RA - 145767* - (https://github.com/fakegermano)
