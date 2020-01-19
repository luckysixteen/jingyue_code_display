__author__ = 'Jingyue Fu'
import re
import copy
import math
from random import choice
import itertools
import time


start = time.clock()


class Country(object):
    def __init__(self, name, id):
        self.name = name
        self.id = id
        self.continental = []
        self.available = []

class Group(object):
    def __init__(self, id):
        self.id = id
        self.list = []
        self.continental = []
        self.num = len(self.continental)


class pot_iter:
    def __init__(self, pools, lens):
        self.pools = pools
        self.lens = lens
        self.iters = []
        self.result = []

    def next(self):
        if len(self.result) == 0:
            start = 0
        else:
            start = len(self.pools) - 1

        i = start
        while i < len(self.pools):
            if len(self.iters) - 1 < i:
                occupied = set(reduce(lambda x, y: x + y, self.result, []))
                pool = list(set(self.pools[i]) - occupied)
                self.iters.append(itertools.combinations(pool, self.lens[i]))
            self.result = self.result[:i]
            try:
                self.result.append(list(self.iters[i].next()))
            except StopIteration:
                if i == 0:
                    raise StopIteration
                self.iters = self.iters[:i]
                i -= 1
                continue
            i += 1
        return self.result

# Read TXT
f = open('input.txt', 'r')
input = list()
for line in open('input.txt'):
    line = f.readline()
    line = line.strip('\n')
    input.append(line)
f.close()


# Initial
groupNum = int(input[0])
groupCount = []
for i in range(groupNum):
    g = Group(i + 1)
    groupCount.append(g)
potNum = int(input[1])
countryNum = 0
potCount = range(1,potNum+1)
country = dict()
pot = [None] * potNum
potLength = 0
for i in range(potNum):
    pot[i] = input[i+2].split(',')
    potLength = len(pot[i]) if len(pot[i]) > potLength else potLength
    for j in range(len(pot[i])):
        countryNum += 1
        c = Country(pot[i][j].replace('\r', ''), countryNum)
        c.available = range(1,groupNum+1)
        pot[i][j] = c
        country[pot[i][j].name] = countryNum
continental = dict()
continental_num = dict()
for i in range(6):
    temp = input[i+potNum+2].split(':')
    str = temp[1].split(',')
    if temp[0] == 'AFC':
        temp[0] = 'A'
        continental_num['A'] = len(str)
    elif temp[0] == 'CAF':
        temp[0] = 'C'
        continental_num['C'] = len(str)
    elif temp[0] == 'CONCACAF':
        temp[0] = 'N'
        continental_num['N'] = len(str)
    elif temp[0] == 'CONMEBOL':
        temp[0] = 'S'
        continental_num['S'] = len(str)
    elif temp[0] == 'OFC':
        temp[0] = 'O'
        continental_num['O'] = len(str)
    elif temp[0] == 'UEFA':
        temp[0] = ['E','F']
        continental_num['E'] = len(str)
    str[0] = str[0].replace('\r','')
    if str[0] != 'None':
        for j in range(len(str)):
            str[j] = str[j].replace('\r','')
            continental[country[str[j]]] = temp[0]
# initial countries
for i in range(potNum):
    for j in range(len(pot[i])):
        pot[i][j].continental = continental[pot[i][j].id]

# print groupNum, potNum, countryNum

def INIT_FIRST_GROUP(pot, potCount, groupCount):
    for i in range(len(pot[0])):
        g = groupCount[i]
        g.list.append(pot[0][i].id)
        if len(pot[0][i].continental) == 2:
            g.continental.append(pot[0][i].continental[0])
        else:
            g.continental.append(pot[0][i].continental)
        pot[0][i].available = []
    potCount.remove(1)
    return potCount, groupCount

# def GET_RANDOM_POT(potCount):
#     next_pot = choice(potCount)
#     potCount.remove(next_pot)
#     return next_pot

def CHOICES_ORDER(dict1, dict2):
    order = map(lambda x: x, dict1)
    order = sorted(order, key=lambda x: dict2[x], reverse=True)
    order = sorted(order, key=lambda x: dict1[x])
    return order

def SEARCH_PREP (dict_available, dict_count, order):
    pools = list()
    length = list()
    for i in order:
        pools.append(dict_available[i])
        length.append(dict_count[i])
    return pools, length

def POT_ORDER(pot, groupCount):
    constraint_count = []
    for p in range(1,len(pot)):
        record = 0
        domain_continental = dict()
        for i in range(len(pot[p])):
            if pot[p][i].continental[0] in domain_continental:
                pointer = domain_continental[pot[p][i].continental[0]]
                pot[p][i].available = pot[p][pointer].available
                record += len(pot[p][i].available)
                continue
            for j in range(len(groupCount)):
                if set(pot[p][i].continental) <= set(groupCount[j].continental):
                    pot[p][i].available.remove(j+1)
            domain_continental[pot[p][i].continental[0]] = i
            record += len(pot[p][i].available)
        constraint_count.append(record)
    pot_order = sorted(range(len(constraint_count)), key=lambda k: constraint_count[k])
    return pot_order

def UPDATE_AVAILABLE(pot, next_pot):
    '''
    input   : pot[n]
    variable: domain_continental - dict{continental : available}
              continental_count - dict{continental : count number}
              continental_rd - dict{continental : number of available}
              position - dict{continental : position[]}
    output  : choice_order - list[]
              pools - list[]
              length - list[]
    '''
    global groupCount

    p = next_pot
    domain_continental = dict()
    continental_count = dict()
    continental_rd = dict()
    position = dict()
    for i in range(len(pot[p])):
        if pot[p][i].continental[0] in domain_continental:
            temp = pot[p][i].continental[0]
            pot[p][i].available = domain_continental[temp]
            continental_count[pot[p][i].continental[0]] += 1
            position[temp].append(i)
            continue
        pot[p][i].available = range(1, groupNum + 1)
        for j in range(len(groupCount)):
            if set(pot[p][i].continental) <= set(groupCount[j].continental):
                pot[p][i].available.remove(j+1)
        if len(pot[p][i].available) == 0:
            choices_order = []
            return choices_order, [], [], position
        domain_continental[pot[p][i].continental[0]] = pot[p][i].available
        continental_count[pot[p][i].continental[0]] = 1
        continental_rd[pot[p][i].continental[0]] = len(pot[p][i].available)
        position[pot[p][i].continental[0]] = [i]
    choices_order = CHOICES_ORDER(continental_rd, continental_count)
    pools, length = SEARCH_PREP(domain_continental, continental_count, choices_order)
    # print position, pools, length
    return choices_order, pools, length, position


def ADDIN_GROUP(assignment, position, order, next_pot, groupCount):
    for i in range(len(order)):
        continental = order[i]
        if continental == 'E':
            for j in range(len(assignment[i])):
                num = assignment[i][j]
                pointer = position[continental][j]
                groupCount[num - 1].list.append(pot[next_pot][pointer].id)
                if 'E' in groupCount[num - 1].continental:
                    groupCount[num - 1].continental.append('F')
                else:
                    groupCount[num - 1].continental.append('E')
        else:
            for j in range(len(assignment[i])):
                num = assignment[i][j]
                pointer = position[continental][j]
                groupCount[num - 1].list.append(pot[next_pot][pointer].id)
                groupCount[num - 1].continental.append(continental)
    return groupCount


def get_convert(dict):
    return {v: k for k, v in dict.items()}


def pre_CHECK(groupNum, potLength, continental_num):
    for k, v in continental_num.iteritems():
        if k == 'E':
            if continental_num[k] > groupNum * 2:
                return False
        elif continental_num[k] > groupNum:
            return False
    return potLength <= groupNum


def recover(assignment, order):
    for i in range(len(order)):
        for group in order[i]:
            assignment[group - 1].continental.pop()
            assignment[group - 1].list.pop()


def show_assignment(assignment):
    if assignment == False:
        output = ['No']
        return output
    output = ['Yes']
    country_name = get_convert(country)
    for group in assignment:
        content = ''
        if group.list == []:
            content = 'None'
            output.append('None')
            print content
            continue
        for num in group.list:
            name = ','+country_name[num]
            content+=name
        print content.lstrip(',')
        output.append(content.lstrip(','))
    return output


def backtrack(assignment, pointer, pot, max_pot):
    if pointer == max_pot:
        return assignment
    choices_order, pools, length, position = UPDATE_AVAILABLE(pot, pointer)
    if choices_order == []:
        # print 'empty available', pointer
        return False
    p = pot_iter(pools, length)
    while True:
        try:
            assignment1 = ADDIN_GROUP(p.next(), position, choices_order,
                                      pointer, assignment)
        except StopIteration:
            return False
        result = backtrack(assignment1, pointer + 1, pot, max_pot)
        if result is not False:
            return result
        recover_group = p.result
        recover(assignment, recover_group)

if pre_CHECK(groupNum, potLength, continental_num):
    potCount, groupCount = INIT_FIRST_GROUP(pot, potCount, groupCount)
    pot_order = range(1,potNum)
    assignment = backtrack(groupCount, 1, pot, potNum)
    output = show_assignment(assignment)
else:
    output = ['No']


elapsed = (time.clock() - start)
print "Time used:", elapsed

open('output.txt', 'w').write('%s' % '\n'.join(output))