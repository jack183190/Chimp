# How to implement game!!
TODO: Collisions
TODO: Different maps

Two simulations, one is player, one is opponent.
If player simulation runs out of lives, it loses and tells the other client that they won.

BloonManager class (one per simulation) handles moving bloons, lives, winning, losing.

WaveManager class to spawn waves (one per simulation)
When receiving start wave packet from server, start a wave on both simulations.
Once wave finished on player simulation, send a wave finished packet to the server.
Once both clients have sent a wave finished packet (or 60 seconds has passed?) server sends another wave start packet to both clients.

TowerManager class (one per simulation) handles rendering towers, their projectiles, and making towers attack nearby bloons. Also handles damaging bloons.
Has functions for place tower, remove tower(?), upgrade tower(?)

TowerListener class (opponent simulation) listens for tower place, remove, upgrade packets and calls the tower manager functions

TowerEditor class (player simulation) handles ui for a player placing, removing, upgrading towers
TowerEditor is responsible for sending packets so that the other client TowerListener can receive them