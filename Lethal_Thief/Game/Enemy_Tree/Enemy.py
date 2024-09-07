#Code for enemy 

import heapq
import random

# Main Enemy Class
class Enemy:
    
    def __init__(self, i, j):
        self.x = j
        self.y = i
        self.size_x = 84
        self.size_y = 40

    #sets x and y to be random values within the room
    def set_rnd_cord(self): 
        self.x = random.randint(0, self.room.size - 1)
        self.y = random.randint(0, self.room.size - 1)
        
    # moves player based on Bot inputd

    def move_enemy(self, player_input, size): 
        
        # player movement 
        if player_input == 'w' or player_input == 'W':
            self.y -= 1 
            
        elif player_input == 's' or player_input == 'S': 
            self.y += 1
        
        elif player_input == 'd' or player_input == 'D': 
            self.x += 1
            
        elif player_input == 'a' or player_input == 'A': 
            self.x -= 1
            
        else: 
            print("Please enter a valid character")
            
        # if player choose value outside of board, reset back to original position
        if self.y < 0: 
            print("Cannot leave the room!")
            self.y  = 0
        
        elif self.y > size - 1: 
            print("Cannot leave the room!")
            self.y  = size - 1
            
        elif self.x < 0: 
            print("Cannot leave the room!")
            self.x  = 0
            
        elif self.x > size - 1 : 
            print("Cannot leave the room!")
            self.x  = size - 1
        
        return
    
    #helper function for A*
    def heuristic(self, x1, y1, x2, y2):
        
        # Manhattan distance
        return abs(x1 - x2) + abs(y1 - y2)
    
    #helper function for A*
    def get_neighbors(self, x, y, state):
        neighbors = []
        for dx, dy in [(-1, 0), (1, 0), (0, -1), (0, 1)]:
            nx, ny = x + dx, y + dy
            if 0 <= nx < self.size_x and 0 <= ny < self.size_y and (state.Board.board[ny][nx]!="║" and state.Board.board[ny][nx]!="X" and state.Board.board[ny][nx]!="═" and state.Board.board[ny][nx]!="┋" and state.Board.board[ny][nx]!="┉"):
                neighbors.append((nx, ny))
                
        return neighbors
    
    #implementation of A* to make a path from enemy to player
    def a_star(self, start_x, start_y, goal_x, goal_y, state):
        open_list = []
        heapq.heappush(open_list, (0, start_x, start_y, 0, None))  # (f, x, y, g, parent)
        closed_set = set()
        came_from = {}
        
        while open_list:
            f, x, y, g, parent = heapq.heappop(open_list)
            if (x, y) in closed_set:
                continue
            
            came_from[(x, y)] = parent
            closed_set.add((x, y))
            
            if (x, y) == (goal_x, goal_y):
                # Reconstruct path
                path = []
                while came_from[(x, y)] is not None:
                    path.append((x, y))
                    (x, y) = came_from[(x, y)]
                return path[::-1]  # Return reversed path
            
            for nx, ny in self.get_neighbors(x, y, state):
                if (nx, ny) in closed_set:
                    continue
                
                g_new = g + 1
                h_new = self.heuristic(nx, ny, goal_x, goal_y)
                f_new = g_new + h_new
                
                heapq.heappush(open_list, (f_new, nx, ny, g_new, (x, y)))
        
        return []  # No path found

    #function to call to make the enemy travel directly towards the player
    def chase_player(self, player_x, player_y, state):   
        path = self.a_star(self.x, self.y, player_x, player_y, state)
        #return false if there isnt a path from enemy to player
        if not path:
            return False
        if path and len(path) > 1:
            state.Board.board[self.y][self.x]=" "
            self.x, self.y = path[0]  # Move to the next step in the path
            state.Board.board[self.y][self.x]="E"
            return True