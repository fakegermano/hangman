# Hangman

Client-Server Hangman Game final project for the MC833 course at @Unicamp

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
You can open as many clients as you want! Each of them you play a different game with different answers!

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
If you press `1` a new game will start.

At present time the `2` and `3` options are not fully implemented yet.

## Authors

* **Giovani Garuffi Rondel** - *RA - 155559* - (https://github.com/giogaruffi)
* **Daniel Germano Travieso** - *RA - 145767* - (https://github.com/fakegermano)
