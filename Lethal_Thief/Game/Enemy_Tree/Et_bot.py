#!/usr/bin/env python
#
import logging, traceback, sys, os, inspect
logging.basicConfig(filename=__file__[:-3] +'.log', filemode='w', level=logging.DEBUG)
currentdir = os.path.dirname(os.path.abspath(inspect.getfile(inspect.currentframe())))
parentdir = os.path.dirname(currentdir)
sys.path.append(parentdir)

from Enemy_Tree.Enemy import *
from Enemy_Tree.Checks import *
from Enemy_Tree.Behaviors import *
from Room import Room
from Enemy_Tree.Et_nodes import Selector, Sequence, Action, Check

# from planet_wars import PlanetWars, finish_turn

# You have to improve this tree or create an entire new one that is capable
# of winning against all the 5 opponent bots
def setup_behavior_tree():

    # Top-down construction of behavior tree
    root = Selector(name='High Level Ordering of Strategies')
    
    # Move towards the player if near
    move_plan = Sequence(name="Move To Player If Near")
    check_distance = Check(distance_to_player)
    move_enemy = Action(move_to_player)
    move_plan.child_nodes = [check_distance, move_enemy]
    
    # # move around randomly if the player is not near 
    # move_randomly = Sequence(name="Move Around Randomly")
    # move_enemy_rnd = Action(move_enemy_randomly)
    # move_randomly.child_nodes = [move_enemy_rnd]
    
    #Construct tree
    root.child_nodes = [move_plan]

    logging.info('\n' + root.tree_to_string())
    return root
