# SoulRock (first project)
A dungeon adventure game. Collect stones and solve riddles to escape!

## Game Overview

Soul Rock is a game of adventure and chance - the objective is to collect all 7
stones, each with a specific and unique property, and make it out of the
labyrinth in the fewest steps possible, by returning to the place you spawn. 
The stones will spawn in random positions based on a seed you give - no game is 
as fun without SOME sort of repeatability!

Along with each stone is an aura that can help you see where the stone is; a
riddle you must answer first before obtaining the stone; and if it's too hard to find,
there will be a corresponding finder scroll with its own aura in a different location
on the map with a hint to the location of the stone.

The game will consist of a main HUD area, which will tell you your inventory,
your steps taken, and the area around you that you can see. You can move around with
the W, A, S, and D keys, and you will also be able to look at your full inventory. When you return to your spawn position,
as long as you have all 7 stones, you will have the chance of putting the stones
on the altar, ending the game and displaying your leaderboard.

__Stone Properties__

* The blue stone has no special properties about it. This will be helpful!
* The white stone moves about as you approach it. Be on your feet!
* The green stone will disappear and reappear at will. You can only pick it up when it's visible!
* The black stone has an aura that grows and shrinks. Sometimes it'll be hard to see!
* The purple stone has a curse - there is a substantial chance it will kick you back when you try to pick it up!
* The red stone cannot be picked up until you have found its finder scroll. Pick up every item you see!
* The orange stone is the end goal - while its aura is much bigger, you must have all the other stones to obtain it!

__What next?__

* End of game - bring the stones to where you spawn and press 'x' to return them to the altar, letting you escape!
* Your leaderboard with high scores will be displayed and saved to a file.

|Group Members     |
|------------------|
|1. Jose Hernandez |
|2. Kaelan Willauer|
|3. Noah Griffith  |
|4. Trevor Natiuk  |

## Running It Again

This version now builds cleanly on macOS/Linux with a standard C++17 compiler.

```bash
make
./soulrock
```

Or in one step:

```bash
make run
```

## Browser Port

There is now a quick web port that keeps the text-terminal feel and is designed
to be hosted on GitHub Pages.

### Local web build

Install Emscripten (`em++`) first, then run:

```bash
make web
```

That creates a static site in `dist/`.

### GitHub Pages

The repository includes a GitHub Actions workflow that can build the WebAssembly
version and deploy it to GitHub Pages. In your repository settings, set Pages to
use GitHub Actions as the source, then push the changes.
