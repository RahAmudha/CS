# make a generator
def all_perms(elements):
    if len(elements) <= 1:
        yield elements
    else:
        for perm in all_perms(elements[1:]):
            for i in range(len(elements)):
                yield perm[:i] + elements[0:1] + perm[i:]

def check_queen (data):
    possible_solutions = []
    for perm in data:
        is_queen = True
        for i in range (len (perm)):
            for j in range (len (perm)):
                if abs (i - j) == abs (perm [i] - perm [j]) and i != j:
                    is_queen = False
        if is_queen:
            possible_solutions.append (perm)
    return possible_solutions
                    
if __name__ == '__main__':
    num_of_q = input ('Enter a number of queens: ')
    data = [i + 1 for i in range (int (num_of_q))]
    solution = check_queen (all_perms (data))
    print (f'The {len (data)}-queens puzzle has {len (solution)} solutions:')
    for i in solution:
        print (i)
