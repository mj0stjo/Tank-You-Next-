# Tank You, Next!
Tank You, Next! is a 3D tank game written in C++ and utilizing the OpenGL graphics library. This readme provides an overview of the game, its features, the codebase and instructions on how to play.

## 1. Game Description
In Tank You, Next!, up to three players control their own tanks in a battle. The tanks can be maneuvered using the "W," "A," "S," and "D" keys for movement, while the "I", "J", "K", "L" and SPACE is used to aim and fire. The game takes place on a three-dimensional platform that includes various obstacles strategically placed throughout. Player movements are synchronized using a WebSocket, ensuring real-time gameplay between the players.

The objective of the game is to score hits on the opponent's tank. After a tank is hit, players have the option to restart and engage in the battle again.

## 2. Features
- Synchronizing players over local network
- Settings via config file:
    - Tank color
    - Server / IP
- Tank movement
- Shooting
- Damage system
- Sounds

## 3. Build and Play
You can download the binaries of the latest release [here](https://github.com/mj0stjo/Tank-You-Next-/releases/download/v0.9.0-alpha/tank-you-next-alpha-0.9.zip).
- Settings (`Tank-You-Next-/config.txt`):
    - RGB value for tank
    - IP
        - `<IP to connect to>` (Client-Mode)
        - `server` (Server-Mode)
- Starting:
    - Start the `launch` script.

## 5. Licence
- MIT - Licence
- For Contact:
  - [Daniel Stempfle](https://github.com/kastanileel)
  - [Johannes Möst](https://github.com/mj0stjo)