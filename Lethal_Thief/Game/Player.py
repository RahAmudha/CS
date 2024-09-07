#Code for player 
import random

class Player:
    def __init__(self):
        #create the player in a random location
        self.x = 0
        self.y = 0
        self.prev = " "
        
    def set_rnd_cord(self, board_size): 
        self.x = random.randint(0, board_size)
        self.y = random.randint(0, board_size)
         
    # moves player based on player input
    def move_player(self, state, player_input, size): 
        Player_x = self.x 
        Player_y = self.y 
        Board = state.Board.board
        
        # player movement 
        if player_input == 'w' or player_input == 'W':
            if Board[Player_y - 1][Player_x] != ' ' and Board[Player_y-1][Player_x] != '┉':
                print("Cannot Move Up")
            
            else: 
                # Replace current player position with a space
                Board[Player_y][Player_x] = self.prev
                
                # Place player at new position
                self.prev =  Board[Player_y - 1][Player_x]
                Board[Player_y - 1][Player_x] = 'P'
                
                # Update player cordinates 
                self.y -= 1
            
        # moves player down
        elif player_input == 's' or player_input == 'S': 
            if Board[Player_y + 1][Player_x] != ' ' and Board[Player_y+1][Player_x] != '┉':
                print("Cannot Move Down")
            
            else: 
                # Replace current player position with a space
                Board[Player_y][Player_x] = self.prev
                
                # Place player at new position 
                self.prev =  Board[Player_y + 1][Player_x]
                Board[Player_y + 1][Player_x] = 'P'
                
                # Update player cordinates 
                self.y += 1
        
        # move player to the right
        elif player_input == 'd' or player_input == 'D':
            if Board[Player_y][Player_x + 1] != ' ' and Board[Player_y][Player_x + 1] != '┋':
                print("Cannot Move Right")
            
            else: 
                # Replace current player position with a space
                Board[Player_y][Player_x] = self.prev
                
                # Place player at new position 
                self.prev =  Board[Player_y][Player_x+1]
                Board[Player_y][Player_x + 1] = 'P'
                
                # Update player cordinates 
                self.x += 1
                
        elif player_input == 'a' or player_input == 'A': 
            if Board[Player_y][Player_x - 1] != ' ' and Board[Player_y][Player_x - 1] != '┋':
                print("Cannot Move Left")
            
            else: 
                # Replace current player position with a space
                Board[Player_y][Player_x] = self.prev
                
                # Place player at new position 
                self.prev =  Board[Player_y][Player_x-1]
                Board[Player_y][Player_x - 1] = 'P'
                
                # Update player cordinates 
                self.x -= 1
            
        else: 
            print("Please enter a valid character")
        
        return 
    
