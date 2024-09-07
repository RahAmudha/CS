import random

def move_enemy_randomly(state):
    bot = state.Enemies
    Board_size = state.Board.size 
    
    move = random.choice(['W', 'A', 'S', 'D'])
    if move == 'W' and bot.y > 0:
        bot.y -= 1
    elif move == 'S' and bot.y < Board_size- 1: # have to give it board state later
        bot.y += 1
    elif move == 'A' and bot.x > 0:
        bot.x -= 1
    elif move == 'D' and bot.x < Board_size - 1: # have to give it board state later
        bot.x += 1
    return True 

def move_to_player(state):
    Bot = state.Enemies
    User = state.Player
    
    return Bot.chase_player(User.x, User.y, state)