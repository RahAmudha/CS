import math
import logging, traceback, sys, os, inspect
logging.basicConfig(filename=__file__[:-3] +'.log', filemode='w', level=logging.DEBUG)
currentdir = os.path.dirname(os.path.abspath(inspect.getfile(inspect.currentframe())))
parentdir = os.path.dirname(currentdir)
sys.path.append(parentdir)

# Finds distance to player, return True if witihin x blocks 
def distance_to_player(state): 

    # Get current player and enemy cordinates
    Player_x = state.Player.x 
    Player_y = state.Player.y 
    Enemy_x = state.Enemy.x 
    Enemy_y = state.Enemy.y
    
    # Close to Enemy
    close_to_enemy = 5.00
    
    # Get distance between player and enemy 
    dist = math.sqrt((Player_x - Enemy_x) ** 2 + (Player_y - Enemy_y) ** 2)
    
    # returns True if the player is close to the enemy
    if dist < close_to_enemy: 
        return True

    else: 
        return False 
        
    
    
    

