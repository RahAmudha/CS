"""
Generating a Floor Plan
Step 1:
    Create outside structure using basic walls
    Doesn't need any mutations or crossover yet
    Starts from empty grid of size * size rooms
    Randomly removes some rooms from the outside perimeter
Step 2:
    Modify rooms using mutation and crossover
    Place in Doors, locked Doors, empty spaces for corridors
    This step should take the most time
Step 3:
    Choose the best child using fitness function
    Designing the fitness function will be really hard
"""

from copy import deepcopy
from random import random, shuffle, randint, choice, sample

class Floorplan(object):
    __slots__ = ["genome", "_fitness", "start_location", "exit_location"]
    
    def __init__(self, genome):
        self.start_location = self.set_start(genome)
        self.exit_location = self.set_exit(genome)
        self.genome = deepcopy(genome)
        self._fitness = None
        
    def to_level(self):
        return self.genome
    
    def print_level(self):
        for row in self.genome:
            for col in row:
                print(col, end="")
            print()
                
    def calculate_fitness(self):
        #Items we want to keep track of (incomplete)
        room_count = 1
        locked_door_count = 0
        enemy_count = 0
        item_count = 0

        height = len(self.genome)
        width = len(self.genome[0])

        for y in range(height):
            for x in range(width):
                cell = self.genome[y][x]
                if cell in ['┋', '┉']: #Locked doors
                    locked_door_count +=1
                elif cell == 'E':
                    enemy_count += 1
                elif cell == 'T':
                    item_count += 1

        room_count = locked_door_count + 1
        
        ### Check for completability here ###
        '''
        Notes:
        When checking for completability we must also check for locked doors and whether or not we have access to keys. (use BFS)
        '''
        visited = set()
        self.dfs(self.start_location, visited) 
        #temp for now, as key functionality has not been implemented

        #calculating fitness score
        fitness_score = (room_count * 2 + 
                         locked_door_count * 1.5 + 
                         enemy_count * 2 + 
                         item_count * 2)
        #Values can and should be changed.

        ### Penalties ###
        if locked_door_count < 5:
            fitness_score *= .9 # too few doors
        if locked_door_count > 10:
            fitness_score *= .6 # too many doors    
        if item_count < 3:
            fitness_score *= .8 # too few items
        if item_count > 12:
            fitness_score *= .7 # too many items
        if enemy_count > 5:
            fitness_score *= .3 # too many enemies
        if enemy_count < 2:
            fitness_score *= .6 # too few enemies

        self._fitness = fitness_score
        return self

        #pass
    
    def set_start(self, genome):
        height = len(genome)
        width = len(genome[0])
        start = (int(height/2) + 1, int(width/2) + 2)
        
        new_start = set_random_point(genome, start)
        return new_start
        
    def set_exit(self, genome):
        
        potential_exit = set_random_point(genome, self.start_location)
        return potential_exit
    
    def dfs(self, node, visited):
        if node not in visited:
            visited.add(node)
            y, x = node
            for dy, dx in [(0, 1), (1, 0), (0, -1), (-1, 0)]:
                ny, nx = y + dy, x + dx
                if self.genome[ny][nx] in [' ', '┋', '┉', 'T', 'E']:
                    self.dfs((ny, nx), visited)

    def fitness(self):
        if self._fitness is None:
            self.calculate_fitness()
        return self._fitness  
        #pass
    
    def mutate(self):
        mutations = [self.add_door, 
                     self.remove_door, 
                     self.add_enemy, 
                     self.remove_enemy, 
                     self.add_item, 
                     self.remove_item]
        
        num_mutations = randint(1,3)
        for _ in range(num_mutations):
            mutation = choice(mutations)
            mutation()

        self._fitness = None # reseting fitness after mutation
        return self 
        #pass

    def add_door(self):
        vertical_door = {'top': ['═','╩','╝','╚','╞','╡','╥','╨'], 'bottom': ['═','╦','╗','╔','╞','╡','╥','╨']}
        horizontal_door = {'left': ['║', '╗', '╝', '╣','╥','╨','╞','╡'], 'right': ['║','╔','╚','╠','╥','╨','╞','╡']}
        
        height = len(self.genome)
        width = len(self.genome[0])
        
        possible_locations = []
        for row in range(len(self.genome)):
            for col in range(len(self.genome[0])):
                if self.genome[row][col] != ' ': # can't place doors in non-empty spaces
                    continue
                if not (row % 2 == 1 and col % 4 == 0 or row % 2 == 0 and col % 4 == 2): # condition to check certain tiles
                    continue    
                if row == 0 or row == len(self.genome) - 1 or col == 0 or col == len(self.genome[0]):
                    continue
                if self.genome[row][col:width] == [' ' for _ in range(width-col)]:
                    continue  
                if self.genome[row][0:col] == [' ' for _ in range(col)]:
                    continue
                if [self.genome[row-y][col] for y in range(row+1)] == [' ' for _ in range(row+1)]:
                    continue
                if [self.genome[row+y][col] for y in range(height-row)] == [' ' for _ in range(height-row)]:
                    continue
                possible_locations.append((row, col))
        
        row, col = choice(possible_locations)
                
        if self.genome[row-1][col] in vertical_door['top'] and self.genome[row+1][col] in vertical_door['bottom']:
            
            if self.genome[row-1][col] == '═':
                self.genome[row-1][col] = '╦'
            elif self.genome[row-1][col] == '╩':
                self.genome[row-1][col] = '╬'
            elif self.genome[row-1][col] == '╝':
                self.genome[row-1][col] = '╣'
            elif self.genome[row-1][col] == '╚':
                self.genome[row-1][col] = '╠'
            elif self.genome[row-1][col] == '╞':
                self.genome[row-1][col] = '╔'
            elif self.genome[row-1][col] == '╡':
                self.genome[row-1][col] = '╗'
            elif self.genome[row-1][col] == '╨':
                self.genome[row-1][col] = '║'
        
            if self.genome[row+1][col] == '═':
                self.genome[row+1][col] = '╩'
            elif self.genome[row+1][col] == '╦':
                self.genome[row+1][col] = '╬'
            elif self.genome[row+1][col] == '╗':
                self.genome[row+1][col] = '╣'
            elif self.genome[row+1][col] == '╔':
                self.genome[row+1][col] = '╠'
            elif self.genome[row+1][col] == '╞':
                self.genome[row+1][col] = '╚'
            elif self.genome[row+1][col] == '╡':
                self.genome[row+1][col] = '╝'
            elif self.genome[row+1][col] == '╥':
                self.genome[row+1][col] = '║'
                
            self.genome[row][col] = '┋'
            
            return
                
        if self.genome[row][col-2] in horizontal_door['left'] and self.genome[row][col+2] in horizontal_door['right']:
            
            if self.genome[row][col-2] == '║':
                self.genome[row][col-2] = '╠'
            elif self.genome[row][col-2] == '╗':
                self.genome[row][col-2] = '╦'
            elif self.genome[row][col-2] == '╝':
                self.genome[row][col-2] = '╩'
            elif self.genome[row][col-2] == '╣':
                self.genome[row][col-2] = '╬'
            elif self.genome[row][col-2] == '╥':
                self.genome[row][col-2] = '╔'
            elif self.genome[row][col-2] == '╨':
                self.genome[row][col-2] = '╚'
            elif self.genome[row][col-2] == '╡':
                self.genome[row][col-2] = '═'
        
            if self.genome[row][col+2] == '║':
                self.genome[row][col+2] = '╣'
            elif self.genome[row][col+2] == '╔':
                self.genome[row][col+2] = '╦'
            elif self.genome[row][col+2] == '╚':
                self.genome[row][col+2] = '╩'
            elif self.genome[row][col+2] == '╠':
                self.genome[row][col+2] = '╬'
            elif self.genome[row][col+2] == '╥':
                self.genome[row][col+2] = '╗'
            elif self.genome[row][col+2] == '╨':
                self.genome[row][col+2] = '╝'
            elif self.genome[row][col+2] == '╞':
                self.genome[row][col+2] = '═'
            
            self.genome[row][col] = '┉'
            self.genome[row][col-1] = '═'
            self.genome[row][col+1] = '═'
            
            return

    def remove_door(self):
        doors = []
        for y, row in enumerate(self.genome):
            for x, cell in enumerate(row):
                if cell in ['┋', '┉']:
                    doors.append((y, x))

        if doors:
            y, x = choice(doors)
            if self.genome[y][x] == '┋':
                self.genome[y][x] = ' '
                if self.genome[y+1][x] == "╠":
                    self.genome[y+1][x] = '╔'
                elif self.genome[y+1][x] == '╣':
                    self.genome[y+1][x] = '╗'
                elif self.genome[y+1][x] == '╬':
                    self.genome[y+1][x] = '╦'
                elif self.genome[y+1][x] == '╩':
                    self.genome[y+1][x] = '═'
                elif self.genome[y+1][x] == '╚':
                    self.genome[y+1][x] = '╞'
                elif self.genome[y+1][x] == '╝':
                    self.genome[y+1][x] = '╡'
                elif self.genome[y+1][x] == '║':
                    self.genome[y+1][x] = '╥'
                    
                if self.genome[y-1][x] == "╠":
                    self.genome[y-1][x] = '╚'
                elif self.genome[y-1][x] == '╣':
                    self.genome[y-1][x] = '╝'
                elif self.genome[y-1][x] == '╬':
                    self.genome[y-1][x] = '╩'
                elif self.genome[y-1][x] == '╦':
                    self.genome[y-1][x] = '═'
                elif self.genome[y-1][x] == '╔':
                    self.genome[y-1][x] = '╞'
                elif self.genome[y-1][x] == '╗':
                    self.genome[y-1][x] = '╡'
                elif self.genome[y-1][x] == '║':
                    self.genome[y-1][x] = '╨'    
                
            else:  # '-' or '┉'
                self.genome[y][x-1:x+2] = '   '
                if self.genome[y][x-2] == '╠':
                    self.genome[y][x-2] = '║'
                elif self.genome[y][x-2] == '╦':
                    self.genome[y][x-2] = '╗'
                elif self.genome[y][x-2] == '╩':
                    self.genome[y][x-2] = '╝'
                elif self.genome[y][x-2] == '╬':
                    self.genome[y][x-2] = '╣'
                elif self.genome[y][x-2] == '═':
                    self.genome[y][x-2] = '╡'
                elif self.genome[y][x-2] == '╔':
                    self.genome[y][x-2] = '╥'
                elif self.genome[y][x-2] == '╚':
                    self.genome[y][x-2] = '╨'
                    
                if self.genome[y][x+2] == '╣':
                    self.genome[y][x+2] = '║'
                elif self.genome[y][x+2] == '╦':
                    self.genome[y][x+2] = '╔'
                elif self.genome[y][x+2] == '╩':
                    self.genome[y][x+2] = '╚'
                elif self.genome[y][x+2] == '╬':
                    self.genome[y][x+2] = '╠'
                elif self.genome[y][x+2] == '═':
                    self.genome[y][x+2] = '╞'
                elif self.genome[y][x+2] == '╗':
                    self.genome[y][x+2] = '╥'
                elif self.genome[y][x+2] == '╝':
                    self.genome[y][x+2] = '╨'
                
    def postprocess(self):  # for now just places keys so that the level is possible
        visited = set()
        door_locations = []
        doors = []
        
        def find_path(node):
            if node not in visited:
                visited.add(node)
                adj = [(node[0]+1, node[1]),(node[0]-1, node[1]),(node[0], node[1]+1),(node[0], node[1]-1)]
                shuffle(adj)
                for y, x in adj:
                    if self.genome[y][x] in ['┋', '┉'] and (y,x) not in door_locations:
                        door_locations.append((y,x))
                        doors.append((y,x))
                        while True:
                            r_y, r_x = choice(list(visited))
                            if self.genome[r_y][r_x] == ' ':
                                break
                        self.genome[r_y][r_x] = 'K'
                    if self.genome[y][x] in [' ', 'T', 'E']:
                        find_path((y,x))
                        
        find_path(self.start_location)
        while door_locations:
            door = choice(door_locations)
            find_path(door)
            door_locations.remove(door)
            
        print("All doors:", doors)

    def place_entity(self, entity_char):
        height = len(self.genome)
        width = len(self.genome[0])

        valid_positions = []
        for row in range(2, height - 2):
            for col in range(4, width - 4):
                if self.genome[row][col] != ' ':
                    continue
                if col % 4 == 0:
                    continue

                if self.genome[row][col:width] == [' ' for _ in range(width-col)]:
                    continue
                if self.genome[row][0:col] == [' ' for _ in range(col)]:
                    continue
                if [self.genome[row-y][col] for y in range(row+1)] == [' ' for _ in range(row+1)]:
                    continue
                if [self.genome[row+y][col] for y in range(height-row)] == [' ' for _ in range(height-row)]:
                    continue

                #check if there is a wall or door nearby
                if (self.genome[row-1][col] in ['═', '╩', '╦', '╬'] or
                    self.genome[row+1][col] in ['═', '╩', '╦', '╬'] or
                    self.genome[row][col-2] in ['║', '╠', '╣', '╬'] or
                    self.genome[row][col+2] in ['║', '╠', '╣', '╬']):
                    valid_positions.append((row, col))

        if not valid_positions:
            return False
        
        y, x = choice(valid_positions)
        self.genome[y][x] = entity_char
        return True
                
    
    def add_enemy(self):
        self.place_entity('E')

    def remove_enemy(self):
        enemies = [(y, x) for y, row in enumerate(self.genome) for x, cell in enumerate(row) if cell == 'E']
        if enemies:
            y, x = choice(enemies)
            self.genome[y][x] = ' '

    def add_item(self):
        self.place_entity('T')

    def remove_item(self):
        items = [(y, x) for y, row in enumerate(self.genome) for x, cell in enumerate(row) if cell == 'I']
        if items:
            y, x = choice(items)
            self.genome[y][x] = ' '

    def move_locations(self):
        height = len(self.genome)
        width = len(self.genome[0])
        start = (int(height/2) + 1, int(width/2) + 2)
        
        self.genome[self.start_location[0]][self.start_location[1]] = ' '
        self.genome[self.exit_location[0]][self.exit_location[1]] = ' '
        
        self.start_location = None
        self.exit_location = None
        
        self.start_location = set_random_point(self.genome, start)
        self.genome[self.start_location[0]][self.start_location[1]] = 'S'
        
        self.exit_location = set_random_point(self.genome, self.start_location)
        self.genome[self.exit_location[0]][self.exit_location[1]] = 'X'
               
    def generate_children(self, other):
        child1 = deepcopy(self)
        child2 = deepcopy(other)

        #perform crossover
        crossover_point = randint(0, len(self.genome) - 1)
        #child1.genome[crossover_point:], child2.genome[crossover_point:] = child2.genome[crossover_point:], child1.genome[crossover_point:]
        #child2.genome[crossover_point:], child1.genome[crossover_point:] = safe_crossover(child1.genome[crossover_point:], child2.genome[crossover_point:])
        
        immutable_start = child1.start_location
        immutable_exit = child1.exit_location
        

        #mutate
        child1.mutate()
        child2.mutate()
        
        c_floorplan1 = Floorplan(child1.genome)
        c_floorplan2 = Floorplan(child2.genome)
        
        c_floorplan1.start_location, c_floorplan1.exit_location = immutable_start, immutable_exit
        c_floorplan2.start_location, c_floorplan2.exit_location = immutable_start, immutable_exit
        
        c_floorplan1.genome[immutable_start[0]][immutable_start[1]] = 'S'
        c_floorplan1.genome[immutable_exit[0]][immutable_exit[1]] = 'X'

        return c_floorplan1, c_floorplan2
        #pass
        
       
    @classmethod  
    def generate_blank(floor):
        randamnt = 0
        
        bitmap = create_bitmap(randamnt)
        f = bitmap_to_level(bitmap)
        
        delete_border_space(f)
        
        return floor(f), bitmap
    
    @classmethod
    def generate_random(floor):
        
        randamnt = 9 # amount that the blank should change
                        # 9 is the max
                        
        bitmap = create_bitmap(randamnt)
        
        f = bitmap_to_level(bitmap)
        f = maze_maker(f, len(bitmap))
        
        delete_border_space(f)

        return floor(f), bitmap  

def safe_crossover(g_sel1, g_sel2):
    for entry in range(len(g_sel1)):
        if g_sel1 == 'K' == g_sel2:
            continue
        temp = g_sel2[entry]
        g_sel2[entry] = g_sel1[entry]
        g_sel1[entry] = temp
    return g_sel1, g_sel2

def set_random_point(genome, start):
    visited = set()
    def dfs(node):
        if node not in visited:
            visited.add(node)
            dir = [(0, 1), (1, 0), (0, -1), (-1, 0)]
            shuffle(dir)
            for dy, dx in dir:
                new_pos = (node[0] + dy, node[1] + dx)
                if genome[new_pos[0]][new_pos[1]] in ['┋', '┉', ' ', 'S', 'X']:
                    dfs(new_pos)
    
    dfs(start)
    
    best_location = max(visited, key=lambda v: distance(start, v), default=None)
    return best_location

def create_bitmap(randamnt):
    size = 20
    bitmap = [[1 for _ in range(size)] for _ in range(size)]
    
    for _ in range(randamnt):
        for col in range(size):
            if random() < 0.1:
                x = 0
                while bitmap[x][col] != 1:
                    x += 1
                bitmap[x][col] = 0
            if random() < 0.1:
                x = 1
                while bitmap[size-x][col] != 1:
                    x += 1
                bitmap[size-x][col] = 0
        for row in range(size):
            if random() < 0.1:
                y = 0
                while bitmap[row][y] != 1:
                    y += 1
                bitmap[row][y] = 0
            if random() < 0.1:
                y = 1
                while bitmap[row][size-y] != 1:
                    y += 1
                bitmap[row][size-y] = 0
        delete_isolated(bitmap)
    
    for row in range(size):
        bitmap[row].insert(0, 0)
        bitmap[row].append(0)
    
    bitmap.insert(0, [0 for _ in range(size+2)])
    bitmap.append([0 for _ in range(size+2)])
    
    return bitmap

def delete_isolated(bitmap):
    size = len(bitmap)
    for row in range(size):
        for col in range(size):
            if bitmap[row][col] == 0:
                continue
            if row < size - 1 and bitmap[row+1][col] == 1:
                continue
            if row > 0 and bitmap[row-1][col] == 1:
                continue
            if col < size - 1 and bitmap[row][col+1] == 1:
                continue
            if col > 0 and bitmap[row][col-1] == 1:
                continue
            bitmap[row][col] = 0
            
def bitmap_to_level(bitmap):
    size = len(bitmap)
    height = size * 2 + 1
    width = 2 * height - 1
    f = [[' ' for _ in range(width)] for _ in range(height)]
    for row in range(size):
        for col in range(size): 
            if bitmap[row][col] == 0:
                if row < size - 1 and bitmap[row+1][col] == 1:
                    f[(row+1)*2][4*col+1: 4*col+4] = '═══'
                if row > 0 and bitmap[row-1][col] == 1:
                    f[row*2][4*col+1: 4*col+4] = '═══'
                if col < size - 1 and bitmap[row][col+1] == 1:
                    f[row*2+1][4*(col+1)] = '║'
                if col > 0 and bitmap[row][col-1] == 1:
                    f[row*2+1][4*(col)] = '║'
     
    return f     

def maze_maker(f, size):
    height = size * 2 + 1
    width = 2 * height - 1
    
    start = (int(height/2) + 1, int(width/2) + 2)
    
    visited = set()
    maze_dfs(f, visited, start, None, None)
    for row in f:
        for col in row:
            print(col, end='')
        print()
    
    add_corners(f)
    return f

def possible_moves(f, pos, parent):
    adj = []
    if f[pos[0]][pos[1]+2] == " ":
        adj.append((pos[0], pos[1]+4))
    if f[pos[0]+1][pos[1]] == " ":
        adj.append((pos[0]+2, pos[1]))
    if f[pos[0]][pos[1]-2] == " ":
        adj.append((pos[0], pos[1]-4))
    if f[pos[0]-1][pos[1]] == " ":
        adj.append((pos[0]-2, pos[1]))
    if parent in adj:
        adj.remove(parent)
    shuffle(adj)
    return adj

def maze_dfs(f, visited, node, parent, dir):    
    if node not in visited:
        visited.add(node)
        adj = possible_moves(f, node, parent)
        for a in adj:
            dir = find_direction(node, a)
            maze_dfs(f, visited, a, node, dir)
    else:      
        place_walls(f, parent, dir)
    
def find_direction(pos, new_pos):
    if pos[0] == new_pos[0] and pos[1] > new_pos[1]:
        return '<'
    if pos[0] == new_pos[0] and pos[1] < new_pos[1]:
        return '>'
    if pos[1] == new_pos[1] and pos[0] > new_pos[0]:
        return '^'
    if pos[1] == new_pos[1] and pos[0] < new_pos[0]:
        return 'v'
    
def place_walls(f, pos, dir):
    if dir == '>':
        f[pos[0]][pos[1]+2] = '║'
    if dir == '<':
        f[pos[0]][pos[1]-2] = '║'
    if dir == 'v':
        f[pos[0]+1][pos[1]-1:pos[1]+2] = '═══'
    if dir == '^':
        f[pos[0]-1][pos[1]-1:pos[1]+2] = '═══'
        
def add_corners(f):
    height = len(f)
    width = len(f[0])
    for row in range(2, height-2, 2):
        for col in range(4, width-4, 4):
            if f[row-1][col] == '║':
                f[row][col] = '╨'
            if f[row+1][col] == '║':
                f[row][col] = '╥'
            if f[row][col-1] == '═':
                f[row][col] = '╡'
            if f[row][col+1] == '═':
                f[row][col] = '╞'
            
            if f[row-1][col] == '║' and f[row][col-1] == '═':
                f[row][col] = '╝'
            if f[row+1][col] == '║' and f[row][col-1] == '═':
                f[row][col] = '╗'
            if f[row-1][col] == '║' and f[row][col+1] == '═':
                f[row][col] = '╚'
            if f[row+1][col] == '║' and f[row][col+1] == '═':
                f[row][col] = '╔'
            if f[row][col-1] == f[row][col+1] == '═':
                f[row][col] = '═'
                if f[row-1][col] == '║':
                    f[row][col] = '╩'
                if f[row+1][col] == '║':
                    f[row][col] = '╦'
            if f[row-1][col] == f[row+1][col] == '║':
                f[row][col] = '║'
                if f[row][col-1] == '═':
                    f[row][col] = '╣'
                if f[row][col+1] == '═':
                    f[row][col] = '╠' 
                    
            if f[row][col-1] == f[row][col+1] == '═' and f[row-1][col] == f[row+1][col] == '║':
                f[row][col] = '╬'
                
def delete_border_space(f):
    height = len(f)
    width = len(f[0])
    for row in range(len(f)):
        del f[row][width-4:width]
        del f[row][0:4]
    del f[height-2:height]
    del f[0:2]
    
def distance(pos1, pos2):
    return abs(pos1[0]-pos2[0]) + abs(pos1[1]-pos2[1])

def save_level(floor:Floorplan, filename):
    f = open(f'Levels/{filename}', 'w')
    for row in floor.genome:
        for col in row:
            f.write(col)
        if row != floor.genome[-1]:
            f.write('\n')

def evolve_floorplan(initial_floorplan, num_generations = 5, population_size = 4): #chance num_generations to the amount of times you wish for mutate to occur
    current_population = [initial_floorplan]
    
    save_level(initial_floorplan, "Initial.txt")

    for _ in range(population_size - 1):
        new_floorplan = Floorplan(initial_floorplan.genome)
        new_floorplan.mutate()
        current_population.append(new_floorplan)
    
    for generation in range(num_generations):
        print(f"Generation {generation + 1}")
    
        children = []
        for _ in range(population_size):
            parent1, parent2 = sample(current_population, 2)
            child1, child2 = parent1.generate_children(parent2)
            children.extend([child1, child2])

        #combined population
        combined_population = current_population + children

        #best selection for next gen
        combined_population.sort(key=lambda x: x.fitness(), reverse=True)
        current_population = combined_population[:population_size]

        #print best fitness
        best_fitness = current_population[0].fitness()
        print(f"Best fitness: {best_fitness}")

    current_population[0].postprocess()
    
    #return best floorplan
    return current_population[0]


if __name__ == "__main__":
    #generating initial floor plan
    initial_floorplan, bitmap = Floorplan.generate_random()
    for row in bitmap:
        for col in row:
            print(col,end=" ")
        print()
    print("Initial floorplan:")
    initial_floorplan.print_level()
    print(f"Initial fitness: {initial_floorplan.fitness()}")

    #evolve time
    best_floorplan = evolve_floorplan(initial_floorplan, num_generations=30, population_size=10)

    print("\nBest floorplan after evolution:")
    best_floorplan.print_level()
    print(f"Final fitness: {best_floorplan.fitness()}")

    #saving best floorplan
    save_level(best_floorplan, "Final.txt")
    print("Best floorplan saved to 'Levels/test.txt'")
