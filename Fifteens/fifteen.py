


import numpy as np
from graph import Graph, Vertex
from random import choice

class Fifteen:

    def __init__(self, size = 4):
        self.size = size
        self.tiles = np.array ([i for i in range (1, size ** 2)] + [0])
        self.adj = {t: [t + i for i in [1, -1, size, -size] if int ((t + i) / size ** 2 + size) == size] for t in range (self.size ** 2)}
            
    def draw(self):
        output = ''
        print ('+---' * self.size + '+')
        for i in range (len (self.tiles)):
            if int (self.tiles [i] / 10) == 0:
                output += f'|   ' if self.tiles [i] == 0 else f'| {str (self.tiles [i])} '
            else: output += f'|{str (self.tiles [i])} '
            if (i + 1) % self.size == 0:
                print (output + '|\n' + '+---' * self.size + '+')
                output = ''
                
    def __str__(self):
        output = ''
        for i in range (len (self.tiles)):
            if int (self.tiles [i] / 10) == 0: output += ' '
            output += '  ' if self.tiles [i] == 0 else str (self.tiles [i]) + ' '
            if (i + 1) % self.size == 0: output += '\n'
        return output
            
    def transpose (self, i, j = 0):
        idI, idJ = np.where (self.tiles == i), np.where (self.tiles == j)
        self.tiles [idI], self.tiles [idJ] = j, i
        
    def is_valid_move (self, move):
        return np.where (self.tiles == 0) [0] [0] in self.adj [np.where (self.tiles == move) [0] [0]]

    def update (self, move): 
        if self.is_valid_move (move): self.transpose (move)
    
    def shuffle (self, moves = 100):
        for i in range (moves): self.transpose (choice (self.adj [np.where (self.tiles == 0) [0] [0]]))
    
    def is_solved (self):
        return str (self) == ' 1  2  3  4 \n 5  6  7  8 \n 9 10 11 12 \n13 14 15    \n'

    # verify if the puzzle is solvable (optional)
    def is_solvable (self):
        pass

    # solve the puzzle (optional)
    def solve (self):
        pass

if __name__ == '__main__':
    game = Fifteen ()
    game.draw ()
    print (str (game))
    assert str (game) == ' 1  2  3  4 \n 5  6  7  8 \n 9 10 11 12 \n13 14 15    \n'
    assert game.is_valid_move (15) == True
    assert game.is_valid_move (12) == True
    assert game.is_valid_move (14) == False
    assert game.is_valid_move (1) == False
    game.update (15)
    assert str (game) == ' 1  2  3  4 \n 5  6  7  8 \n 9 10 11 12 \n13 14    15 \n'
    game.update (15)
    assert str (game) == ' 1  2  3  4 \n 5  6  7  8 \n 9 10 11 12 \n13 14 15    \n'
    
    assert game.is_solved () == True
    game.shuffle ()
    assert str (game) != ' 1  2  3  4 \n 5  6  7  8 \n 9 10 11 12 \n13 14 15    \n'
    assert game.is_solved () == False
    
    
    game = Fifteen()
    game.shuffle()
    game.draw()
    while True:
        move = input('Enter your move or q to quit: ')
        if move == 'q':
            break
        elif not move.isdigit():
            continue
        game.update(int(move))
        game.draw()
        if game.is_solved():
            break
    print('Game over!')
    
    
