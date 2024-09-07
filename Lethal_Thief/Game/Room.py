# Code for Room Generation
from Enemy_Tree.Enemy import Enemy

# Room Class
class Room:
    # initialize useful variables
    def __init__(self):
        # Room size
        self.size = 45
        self.board = [[' ' for _ in range(84)] for _ in range(self.size)]
        
    # Get the Board from the txt file 
    def read_board(self, state): 
        
        with open('Final_Room.txt', 'r', encoding='utf-8') as file:
            
            # index for line in file
            i = 0
            for line in file:
                for j in range(len(line) - 1):
                    
                    # set the board
                    self.board[i][j] = line[j]
                    #print("setting character", line[j], "at cord", i, j)
                        
                    # if 'P' is found mark player location 
                    if line[j] == 'P':
                        state.Player.y = i 
                        state.Player.x = j
                        
                    # elif 'E' is found create a new class and add to enemy list 
                    elif line[j] == 'E':
                        new_enemy = Enemy(i, j)
                        state.Enemies.append(new_enemy)
                        
                # increment index
                i += 1
                   
        print('\n') 
    
    def display(self):
                    
        for i in range(len(self.board) - 1): 
            line = ''.join(self.board[i])
            print(line)
        
        # print the board
        print("Game Board:")
    
        return 
    
    
    