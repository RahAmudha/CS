from Room import Room
from Player import Player 
from Enemy_Tree.Enemy import Enemy
from Enemy_Tree.Et_bot import *
import logging
import os

# Stores the overall Game State 
class Game: 
    def __init__(self):
        self.Enemies = []
        self.Player = None 
        self.Board = None
        self.Alive = True

#  main function (run to play)
if __name__ == "__main__":
    
    # Create class for game board 
    Game_State = Game()
    
    # Create
    Game_State.Board = Room()
    Game_State.Player = Player()

    
    Board = Game_State.Board
    User = Game_State.Player
    # Bot = Game_State.Enemies
    
    # Print start message 
    print("Starting the Game")
    
    # Get Board Contents from txt file 
    Board.read_board(Game_State)

    # Display the board 
    Board.display()
    
    # Play Game
    try:
        # Debugging information for behavior tree
        logging.basicConfig(filename=__file__[:-3] + '.log', filemode='w', level=logging.DEBUG)
        #behavior_tree = setup_behavior_tree()
            
        #continue while player is alive
        while Game_State.Alive: 
           
            # Get player input
            player_input = input("Enter either w (UP), a (Left), s (Down), or d (Right) and push enter when finished : \n")
            
            # clearing the terminal
            os.system('cls') 
            
            

            # update player position
            User.move_player(Game_State, player_input, Board.size)

            for enemy in Game_State.Enemies:
                enemy.chase_player(User.x, User.y, Game_State)
        
            # Display the board 
            Board.display()

            for enemy in Game_State.Enemies:
                #check if the enemy can kill you
                for dx, dy in [(-1, 0), (1, 0), (0, -1), (0, 1)]:
                    nx, ny = User.x + dx, User.y + dy
                    if nx == enemy.x and ny == enemy.y or User.x==enemy.x and User.y==enemy.y:
                        #if the enemy is within range then the player dies
                        print("You were caught!!")
                        Game_State.Alive=False
                        break
    
    except KeyboardInterrupt: 
        print("Game Ended")
        
    