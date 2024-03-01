# "Hey, Thats My Fish!" Game
A simple version of the game "Hey That's My Fish" coded with C.

The C code is written in Dev-C++ using Allegro graphics library.

The user chooses between 2, 3 and 4 player modes and based on the choice, 4, 3 or 2 penguins are assigned to each user.

After that an empty game board appears in which each player has to choose where they to put their penguins and it permits users from putting penguin in a spot where another player has already taken.

## Rules
The penguins can move on the board from square to square and collect fish from the squares they step on.

When a penguin steps on a square, it would take the square for itself and after the penguins moves to another one, it will turn to ice so no other player can use it again.

Penguins can move to 4 sides of the square and can not jump across a taken square or another penguin on their way. The game permits them from doing so and asks them to choose another path.

When there is not an accessible square for the penguin to step on, it would be out. And a player is out when all of their penguins are.

10 squares have three fish, 20 squares have two and 34 have one.

The winner is the person whose penguins collected the most fish. If the number of fish for more than one player was the same, the person with the most collected squares would win. And If the number of squares was equal, the players with the most fish and squares would win.
