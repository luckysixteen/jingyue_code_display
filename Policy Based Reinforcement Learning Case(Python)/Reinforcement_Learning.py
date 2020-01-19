__author__ = 'Jingyue Fu'
import numpy as np
import time
import copy


start = time.clock()

# load txt
f = open('input.txt', 'r')
input = list()
for line in open('input.txt'):
    line = f.readline()
    line = line.strip('\n')
    input.append(line)
f.close()

row  = int(input[0].split(',')[0])
column = int(input[0].split(',')[1])
grid = np.zeros((row, column))

w_num = int(input[1])
wall = list()
for i in range(w_num):
    x = int(input[i + 2].split(',')[0])
    y = int(input[i + 2].split(',')[1])
    grid[x-1,y-1] = None
    wall.append((-x+row,y-1))

t_num = int(input[w_num + 2])
terminal = list()
r_t = float ('-inf')
center = (-1,-1)
for i in range(t_num):
    x = int(input[i + w_num + 3].split(',')[0])
    y = int(input[i + w_num + 3].split(',')[1])
    r = float(input[i + w_num + 3].split(',')[2])
    if r > r_t:
        r_t = r
        center = (-x+row,y-1)
    grid[x-1,y-1] = r
    terminal.append((-x+row,y-1))

grid = np.flip(grid, 0)

pw = float(input[t_num + w_num + 3].split(',')[0])
pr = float(input[t_num + w_num + 3].split(',')[1])
rw = float(input[t_num + w_num + 4].split(',')[0])
rr = float(input[t_num + w_num + 4].split(',')[1])
df = float(input[t_num + w_num + 5])

orientations = [1,2,3,4,5,6,7,8]
orien_dict = {0: 'Exit', 1:'Walk Up', 2: 'Walk Down', 3: 'Walk Left', 4: 'Walk Right', 5: 'Run Up', 6: 'Run Down', 7: 'Run Left', 8: 'Run Right'}

def turn_right(orientation):
    if orientation == 3 or orientation == 4 or orientation == 7 or orientation == 8:
        return orientation - 2
    elif orientation == 1 or orientation == 5:
        return orientation + 3
    else:
        return orientation + 1


def turn_left(orientation):
    if orientation == 1 or orientation == 2 or orientation == 5 or orientation == 6:
        return orientation + 2
    elif orientation == 3 or orientation == 7:
        return orientation - 1
    else:
        return orientation - 3

def argmin(seq, fn):
    best = seq[0]
    best_score = fn(best)
    for x in seq:
        x_score = fn(x)
        if x_score < best_score:
            best, best_score = x, x_score
    return best


def argmax(seq, fn):
    return argmin(seq, lambda x: -fn(x))


class MDP(object):

    def __init__(self, grid, terminals, gamma):
        self.init = (0,0)
        self.terminals = terminals

        self.actlist = [1,2,3,4,5,6,7,8]
        self.states=list()
        self.statereverse = list()
        self.stateset=set()
        self.reward=grid

        self.gamma = gamma

        for x in range(len(self.reward)):
            for y in range(len(self.reward[0])):
                if np.isnan(grid[x][y]):
                    continue
                self.states.append((x, y))
                self.stateset.add((x,y))

        for t in terminals:
            self.states.remove(t)


    def R(self, state, action):
        if action in (1,2,3,4):
            return rw
        elif action in (5,6,7,8):
            return rr
        else:
            return self.reward[state]

    def T(self, state, action):
        if action in (1,2,3,4):
            return [(pw, self.go(state, action)),
                    (round(0.5*(1-pw),5), self.go(state, turn_right(action))),
                    (round(0.5*(1-pw),5), self.go(state, turn_left(action)))]
        elif action in (5,6,7,8):
            return [(pr, self.go(state, action)),
                    (round(0.5*(1-pr),5), self.go(state, turn_right(action))),
                    (round(0.5*(1-pr),5), self.go(state, turn_left(action)))]
        else:
            return[(1.0, state)]


    def go(self, state, action):
        mid = (state[0], state[1])

        if action < 5:
            if action <3:
                if action == 1:
                    new_state = (state[0] - 1, state[1])
                else:
                    new_state = (state[0] + 1, state[1])
            else:
                if action == 3:
                    new_state = (state[0], state[1] - 1)
                else:
                    new_state = (state[0] , state[1] + 1)
        else:
            if action <7:
                if action ==5:
                    new_state = (state[0] - 2, state[1])
                    mid = (state[0] - 1, state[1])
                else:
                    new_state = (state[0] + 2, state[1])
                    mid = (state[0] + 1, state[1])
            else:
                if action == 7:
                    new_state = (state[0], state[1]-2)
                    mid = (state[0], state[1]-1)
                else:
                    new_state = (state[0], state[1] + 2)
                    mid = (state[0], state[1] + 1)

        if new_state in self.stateset and mid in self.stateset:
            return new_state
        else:
            return state

    def actions(self, state):
        if state in self.terminals:
            return [0]
            # return [None]
        else:
            return self.actlist

    def P(self):
        transition = [[[None] * 8 for i in range(column)] for j in range(row)]
        for s in self.stateset:
            for a in orientations:
                transition[s[0]][s[1]][a - 1] = self.T(s, a)
        return transition



def value_iteration(mdp, epsilon=0.0):
    U1 = np.zeros((row, column))
    R, T, gamma = mdp.R, mdp.T, mdp.gamma
    count = 0
    for t in terminal:
        U1[t] = R(t,0)
    for s in mdp.states:
        U1[s] = max(R(s,1), R(s,5))

    transition = mdp.P()
    print "0"
    print U1

    def neighbour(x, y):
        n = []
        up = (x - 1, y)
        down = (x + 1, y)
        left = (x, y - 1)
        right = (x, y + 1)
        for n_ in [left, right, up, down]:
            if n_[0] in range(row) and n_[1] in range(column) and grid[n_[0]][n_[1]] is not None and \
                    (n_[0], n_[1]) not in mdp.terminals:
                n.append(n_)
        return n

    update = []
    for t in mdp.terminals:
        update += neighbour(t[0], t[1])

    while True:
        count += 1
        delta = 0
        visited = {}
        while update:
            update_ = []
            for s in update:
                visited[s] = True
            for s in update:
                if s not in mdp.stateset:
                    continue
                tran_s = transition[s[0]][s[1]]
                prev = U1[s]
                U1[s] = max([gamma * sum([tran_s[a-1][i][0] * U1[tran_s[a-1][i][1]] for i in range(3)]) + R(s, a) for a in orientations])
                loss = abs(U1[s] - prev)
                # if loss > epsilon:
                update_ += filter(lambda st: (st not in visited) and (st not in update_), neighbour(s[0], s[1]))
                delta = max(delta, loss)
            update = update_
        print count
        print U1
        print "delta: ", delta
        if delta <= epsilon:
            return U1
        for t in mdp.terminals:
            update += neighbour(t[0], t[1])

def expected_utility(a, s, U, mdp):
    sumu = sum([ p * U[s1] for (p, s1) in mdp.T(s, a)])
    u = mdp.R(s,a)+mdp.gamma*sumu
    return u


def best_policy(mdp, U):
    pi = [ [None] * column for i in range(row) ]
    for s in mdp.stateset:
        temp = argmax(mdp.actions(s), lambda a: expected_utility(a, s, U, mdp))
        pi[s[0]][s[1]] = orien_dict[temp]
    return pi

def show_pi(pi):
    output = []
    for row in pi:
        content = ''
        for i in range(len(row)):
            name = ','+str(row[i])
            content += name
        # print content.lstrip(',')
        output.append(content.lstrip(','))
    return output

a = MDP(grid, terminal, df)
U = value_iteration(a)
pi = best_policy(a,U)
output = show_pi(pi)

elapsed = (time.clock() - start)
print "Time used:", elapsed


open('output.txt', 'w').write('%s' % '\n'.join(output))
