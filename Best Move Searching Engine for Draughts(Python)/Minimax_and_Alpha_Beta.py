__author__ = 'Jingyue Fu'
import re
import copy
#import time

#start = time.clock()

# Read TXT
f = open('input.txt', 'r')
input = list()
for line in open('input.txt'):
    line = f.readline()
    line = line.strip('\n')
    input.append(line)
f.close()

# Initial
player = input[0]
alg = input[1]
if int(input[2]) > 10:
    depth = 10
else:
    depth = int(input[2])
board = [None] * 8
for i in range(0, 8):
    board[i] = input[i + 3].split(',')
award = input[11].split(',')
award = map(int, award)
benchmark = ['H', 'G', 'F', 'E', 'D', 'C', 'B', 'A']


class Player(object):
    def __init__(self, name, award):
        self.name = name
        self.role = ''
        self.award = award
        self.pieces = list()
        self.piecesFinish = list()
        self.piecesMove = list()
        self.piecesNum = 0
        self.nextmovepiece = [-1, -1]
        self.utility = 0
        self.nextmove = [-1, -1]

    def displayPlayer(self):
        print(self.name, self.role, self.award, self.pieces, self.piecesFinish,
              self.piecesNum)


class State(object):
    def __init__(self, board, depth=0):
        self.staterole = ""
        self.depth = depth
        self.stateboard = board
        self.statechoices = list()
        self.children = list()
        self.statevalue = 0
        self.alpha = float("-inf")
        self.beta = float("inf")

    def get_choices(self, player1, player2, board):
        if self.staterole == player1.role:
            self.statechoices = get_movepieces(player1, board)
        else:
            self.statechoices = get_movepieces(player2, board)

    def get_staterole(self):
        return self.staterole

    def get_statevalue(self):
        return self.statevalue

    def get_children(self):
        return self.children

    def add(self, state):
        self.children.append(state)

    def go(self, value):
        for child in self.children:
            if child.get_statevalue == value:
                return child
        return None

    def displayState(self):
        print(self.stateboard, self.statevalue, self.depth, self.staterole,
              self.statechoices)


def get_role(input, player1, player2):
    if re.search(input, player1.name, re.IGNORECASE):
        player1.role = "MAX"
        player2.role = "MIN"
    else:
        player2.role = "MAX"
        player1.role = "MIN"


def get_pieces(board, star, circle):
    star.pieces = list()
    star.piecesFinish = list()
    circle.pieces = list()
    circle.piecesFinish = list()
    star.piecesNum = 0
    circle.piecesNum = 0
    for i in range(0, 8):
        for j in range(0, 8):
            if re.search('S', board[i][j], re.IGNORECASE):
                s = board[i][j]
                s = s.strip('Ss')
                num = int(s)
                star.piecesNum += num
                if i == 0:
                    for k in range(num):
                        star.piecesFinish.append([i, j])
                else:
                    for k in range(num):
                        star.pieces.append([i, j])
            elif re.search('C', board[i][j], re.IGNORECASE):
                s = board[i][j]
                s = s.strip('Cc')
                num = int(s)
                circle.piecesNum += num
                if i == 7:
                    for k in range(num):
                        circle.piecesFinish.append([i, j])
                else:
                    for k in range(num):
                        circle.pieces.append([i, j])


def get_award(player):
    player.utility = 0
    for i in range(len(player.pieces)):
        pointer = player.pieces[i][0]
        player.utility += player.award[pointer]
    for i in range(len(player.piecesFinish)):
        pointer = player.piecesFinish[i][0]
        player.utility += player.award[pointer]
    return player.utility


def get_utility(player1, player2):
    if player1.role == "MAX":
        max_award = get_award(player1)
        min_award = get_award(player2)
    else:
        max_award = get_award(player2)
        min_award = get_award(player1)
    utility = max_award - min_award
    return utility


def get_movepieces_star(player, B):
    player.piecesMove = list()
    for i in range(len(player.pieces)):
        x = player.pieces[i][0]
        y = player.pieces[i][1]
        if x == 1:
            if y - 1 > -1:
                if re.search('S', B[x - 1][y - 1], re.IGNORECASE) or re.search(
                        '0', B[x - 1][y - 1], re.IGNORECASE):
                    player.piecesMove.append((x, y, x - 1, y - 1))
            if y + 1 < 8:
                if re.search('S', B[x - 1][y + 1], re.IGNORECASE) or re.search(
                        '0', B[x - 1][y + 1], re.IGNORECASE):
                    player.piecesMove.append((x, y, x - 1, y + 1))
        else:
            if y - 1 > -1 and re.search('0', B[x - 1][y - 1]):
                player.piecesMove.append((x, y, x - 1, y - 1))
            if y - 2 > -1 and re.search('C', B[x - 1][y - 1], re.IGNORECASE):
                if x - 2 == 0:
                    player.piecesMove.append((x, y, x - 2, y - 2))
                elif re.search('0', B[x - 2][y - 2]):
                    player.piecesMove.append((x, y, x - 2, y - 2))
            if y + 1 < 8 and re.search('0', B[x - 1][y + 1]):
                player.piecesMove.append((x, y, x - 1, y + 1))
            if y + 2 < 8 and re.search('C', B[x - 1][y + 1], re.IGNORECASE):
                if x - 2 == 0:
                    player.piecesMove.append((x, y, x - 2, y + 2))
                elif re.search('0', B[x - 2][y - 2]):
                    player.piecesMove.append((x, y, x - 2, y + 2))
    player.piecesMove.sort()
    return player.piecesMove


def get_movepieces_circle(player, B):
    player.piecesMove = list()
    for i in range(len(player.pieces)):
        x = player.pieces[i][0]
        y = player.pieces[i][1]
        if x == 6:
            if y - 1 > -1:
                if re.search('C', B[x + 1][y - 1], re.IGNORECASE) or re.search(
                        '0', B[x + 1][y - 1], re.IGNORECASE):
                    player.piecesMove.append((x, y, x + 1, y - 1))
            if y + 1 < 8:
                if re.search('C', B[x + 1][y + 1], re.IGNORECASE) or re.search(
                        '0', B[x + 1][y + 1], re.IGNORECASE):
                    player.piecesMove.append((x, y, x + 1, y + 1))
        else:
            if y - 1 > -1 and re.search('0', B[x + 1][y - 1]):
                player.piecesMove.append((x, y, x + 1, y - 1))
            if y - 2 > -1 and re.search('S', B[x + 1][y - 1], re.IGNORECASE):
                if x + 2 == 7:
                    player.piecesMove.append((x, y, x + 2, y - 2))
                elif re.search('0', B[x + 2][y - 2]):
                    player.piecesMove.append((x, y, x + 2, y - 2))
            if y + 1 < 8 and re.search('0', B[x + 1][y + 1]):
                player.piecesMove.append((x, y, x + 1, y + 1))
            if y + 2 < 8 and re.search('S', B[x + 1][y + 1], re.IGNORECASE):
                if x + 2 == 7:
                    player.piecesMove.append((x, y, x + 2, y + 2))
                elif re.search('0', B[x + 2][y + 2]):
                    player.piecesMove.append((x, y, x + 2, y + 2))
    player.piecesMove.sort()
    return player.piecesMove


def get_movepieces(player, B):
    if player.name == 'star':
        return get_movepieces_star(player, B)
    else:
        return get_movepieces_circle(player, B)


def update_board(child, state, player1, player2):
    x1, y1, x2, y2 = child
    newBoard = copy.deepcopy(state.stateboard)
    newBoard[x1][y1] = '0'
    if state.staterole == player1.role:
        if player1.name == 'star':
            if x1 - x2 == 2:
                if y2 > y1:
                    newBoard[x2 + 1][y2 - 1] = '0'
                else:
                    newBoard[x2 + 1][y2 + 1] = '0'
            if x2 == 0:
                if re.search('S', newBoard[x2][y2], re.IGNORECASE):
                    s = newBoard[x2][y2]
                    s = s.strip('Ss')
                    num = int(s) + 1
                    newBoard[x2][y2] = 'S' + str(num)
                else:
                    newBoard[x2][y2] = 'S1'
            else:
                newBoard[x2][y2] = 'S1'
        else:
            if x2 - x1 == 2:
                if y2 > y1:
                    newBoard[x2 - 1][y2 - 1] = '0'
                else:
                    newBoard[x2 - 1][y2 + 1] = '0'
            if x2 == 7:
                if re.search('C', newBoard[x2][y2], re.IGNORECASE):
                    s = newBoard[x2][y2]
                    s = s.strip('Cc')
                    num = int(s) + 1
                    newBoard[x2][y2] = 'C' + str(num)
                else:
                    newBoard[x2][y2] = 'C1'
            else:
                newBoard[x2][y2] = 'C1'
    else:
        if player2.name == 'star':
            if x1 - x2 == 2:
                if y2 > y1:
                    newBoard[x2 + 1][y2 - 1] = '0'
                else:
                    newBoard[x2 + 1][y2 + 1] = '0'
            if x2 == 0:
                if re.search('S', newBoard[x2][y2], re.IGNORECASE):
                    s = newBoard[x2][y2]
                    s = s.strip('Ss')
                    num = int(s) + 1
                    newBoard[x2][y2] = 'S' + str(num)
                else:
                    newBoard[x2][y2] = 'S1'
            else:
                newBoard[x2][y2] = 'S1'
        else:
            if x2 - x1 == 2:
                if y2 > y1:
                    newBoard[x2 - 1][y2 - 1] = '0'
                else:
                    newBoard[x2 - 1][y2 + 1] = '0'
            if x2 == 7:
                if re.search('C', newBoard[x2][y2], re.IGNORECASE):
                    s = newBoard[x2][y2]
                    s = s.strip('Cc')
                    num = int(s) + 1
                    newBoard[x2][y2] = 'C' + str(num)
                else:
                    newBoard[x2][y2] = 'C1'
            else:
                newBoard[x2][y2] = 'C1'
    return newBoard


def terminal_test(state):
    if state.staterole == star.role:
        if star.piecesNum == 0:
            return True
        else:
            return False
    if state.staterole == circle.role:
        if circle.piecesNum == 0:
            return True
        else:
            return False


def switch_output(bestRoad):
    if len(bestRoad) < 2:
        NEXT_MOVE = "pass"
    else:
        NEXT_MOVE = benchmark[bestRoad[0]] + str(
            bestRoad[1] + 1) + "-" + benchmark[bestRoad[2]] + str(
                bestRoad[3] + 1)
    return NEXT_MOVE


NUMBER_OF_NODES = 1
bestRoad = list()
MYOPIC_UTILITY = 0
passFlag = 0
floor = depth + 1


def MINIMAX(state, depth):
    global NUMBER_OF_NODES
    global bestRoad
    global MYOPIC_UTILITY
    global passFlag
    if terminal_test(state) or depth == 0:
        return state.statevalue
    if state.staterole == "MIN":
        util = float("inf")
        if len(state.statechoices) == 0:
            if passFlag == 0:
                passFlag += 1
                state.children.append("pass")
                temp = floor - depth
                newBoard = state.stateboard
                node = State(newBoard, temp)
                if state.staterole == "MAX":
                    node.staterole = "MIN"
                else:
                    node.staterole = "MAX"
                get_pieces(newBoard, star, circle)
                node.get_choices(star, circle, newBoard)
                node.statevalue = get_utility(circle, star)
                NUMBER_OF_NODES += 1
                childnode = MINIMAX(node, depth - 1)
                if util > childnode:
                    util = childnode
                    if node.depth == 1:
                        bestRoad = child
            else:
                NUMBER_OF_NODES += 1
                return state.statevalue
        for child in state.statechoices:
            if passFlag == 1:
                passFlag = 0
            temp = floor - depth
            newBoard = update_board(child, state, star, circle)
            node = State(newBoard, temp)
            if state.staterole == "MAX":
                node.staterole = "MIN"
            else:
                node.staterole = "MAX"
            get_pieces(newBoard, star, circle)
            node.get_choices(star, circle, newBoard)
            node.statevalue = get_utility(circle, star)
            state.add(node)
            NUMBER_OF_NODES += 1
            childnode = MINIMAX(node, depth - 1)
            if util > childnode:
                util = childnode
                if node.depth == 1:
                    bestRoad = child
    else:
        util = float("-inf")
        if len(state.statechoices) == 0:
            if passFlag == 0:
                passFlag += 1
                state.children.append("pass")
                temp = floor - depth
                newBoard = state.stateboard
                node = State(newBoard, temp)
                if state.staterole == "MAX":
                    node.staterole = "MIN"
                else:
                    node.staterole = "MAX"
                get_pieces(newBoard, star, circle)
                node.get_choices(star, circle, newBoard)
                node.statevalue = get_utility(circle, star)
                NUMBER_OF_NODES += 1
                childnode = MINIMAX(node, depth - 1)
                if util < childnode:
                    util = childnode
                    if node.depth == 1:
                        bestRoad.append("pass")
                        MYOPIC_UTILITY = node.statevalue
            else:
                NUMBER_OF_NODES += 1
                return state.statevalue
        for child in state.statechoices:
            if passFlag == 1:
                passFlag = 0
            temp = floor - depth
            newBoard = update_board(child, state, star, circle)
            node = State(newBoard, temp)
            if state.staterole == "MAX":
                node.staterole = "MIN"
            else:
                node.staterole = "MAX"
            get_pieces(newBoard, star, circle)
            node.get_choices(star, circle, newBoard)
            node.statevalue = get_utility(circle, star)
            state.add(node)
            NUMBER_OF_NODES += 1
            childnode = MINIMAX(node, depth - 1)
            if util < childnode:
                util = childnode
                if node.depth == 1:
                    bestRoad = child
                    MYOPIC_UTILITY = node.statevalue
    return util


def ALPHABETA(state, depth, a, b):
    global NUMBER_OF_NODES
    global bestRoad
    global MYOPIC_UTILITY
    global passFlag
    if terminal_test(state) or depth == 0:
        return state.statevalue
    if state.staterole == "MIN":
        util = float("inf")
        if len(state.statechoices) == 0:
            if passFlag == 0:
                passFlag += 1
                state.children.append("pass")
                temp = floor - depth
                newBoard = state.stateboard
                node = State(newBoard, temp)
                if state.staterole == "MAX":
                    node.staterole = "MIN"
                else:
                    node.staterole = "MAX"
                get_pieces(newBoard, star, circle)
                node.get_choices(star, circle, newBoard)
                node.statevalue = get_utility(circle, star)
                NUMBER_OF_NODES += 1
                if state.beta != float("inf"):
                    node.beta = state.beta
                childnode = ALPHABETA(node, depth - 1, node.alpha, node.beta)
                if util > childnode:
                    util = childnode
                    if node.depth == 1:
                        bestRoad = child
                state.beta = min(state.beta, util)
                if util <= state.alpha:
                    return util
            else:
                NUMBER_OF_NODES += 1
                return state.statevalue
        for child in state.statechoices:
            if passFlag == 1:
                passFlag = 0
            temp = floor - depth
            newBoard = update_board(child, state, star, circle)
            node = State(newBoard, temp)
            if state.staterole == "MAX":
                node.staterole = "MIN"
            else:
                node.staterole = "MAX"
            get_pieces(newBoard, star, circle)
            node.get_choices(star, circle, newBoard)
            node.statevalue = get_utility(circle, star)
            state.add(node)
            NUMBER_OF_NODES += 1
            if state.beta != float("inf"):
                node.beta = state.beta
            childnode = ALPHABETA(node, depth - 1, node.alpha, node.beta)
            if util > childnode:
                util = childnode
                if node.depth == 1:
                    bestRoad = child
            state.beta = min(state.beta, util)
            if util <= state.alpha:
                return util
        return util
    else:
        util = float("-inf")
        if len(state.statechoices) == 0:
            if passFlag == 0:
                passFlag += 1
                state.children.append("pass")
                temp = floor - depth
                newBoard = state.stateboard
                node = State(newBoard, temp)
                if state.staterole == "MAX":
                    node.staterole = "MIN"
                else:
                    node.staterole = "MAX"
                get_pieces(newBoard, star, circle)
                node.get_choices(star, circle, newBoard)
                node.statevalue = get_utility(circle, star)
                NUMBER_OF_NODES += 1
                if state.alpha != float("-inf"):
                    node.alpha = state.alpha
                childnode = ALPHABETA(node, depth - 1, node.alpha, node.beta)
                if util < childnode:
                    util = childnode
                    if node.depth == 1:
                        bestRoad.append("pass")
                        MYOPIC_UTILITY = node.statevalue
                state.alpha = max(state.alpha, util)
                if util >= state.beta:
                    return util
            else:
                NUMBER_OF_NODES += 1
                return state.statevalue
        for child in state.statechoices:
            if passFlag == 1:
                passFlag = 0
            temp = floor - depth
            newBoard = update_board(child, state, star, circle)
            node = State(newBoard, temp)
            if state.staterole == "MAX":
                node.staterole = "MIN"
            else:
                node.staterole = "MAX"
            get_pieces(newBoard, star, circle)
            node.get_choices(star, circle, newBoard)
            node.statevalue = get_utility(circle, star)
            state.add(node)
            NUMBER_OF_NODES += 1
            if state.alpha != float("-inf"):
                node.alpha = state.alpha
            childnode = ALPHABETA(node, depth - 1, node.alpha, node.beta)
            if util < childnode:
                util = childnode
                if node.depth == 1:
                    bestRoad = child
                    MYOPIC_UTILITY = node.statevalue
            state.alpha = max(state.alpha, util)
            if util >= state.beta:
                return util
        return util


star = Player("star", award[::-1])
circle = Player("circle", award)
root = State(board, 0)
get_role(player, star, circle)

root.staterole = "MAX"
get_pieces(board, star, circle)
root.get_choices(star, circle, board)
root.statevalue = get_utility(circle, star)

if re.search('minimax', alg, re.IGNORECASE):
    FARSIGHTED_UTILITY = MINIMAX(root, depth)
else:
    FARSIGHTED_UTILITY = ALPHABETA(root, depth, root.alpha, root.beta)
NEXT_MOVE = switch_output(bestRoad)

print NEXT_MOVE
print MYOPIC_UTILITY
print FARSIGHTED_UTILITY
print NUMBER_OF_NODES
# elapsed = (time.clock() - start)
# print("Time used:",elapsed)

# output = list()
# output.append(NEXT_MOVE)
# output.append(str(MYOPIC_UTILITY))
# output.append(str(FARSIGHTED_UTILITY))
# output.append(str(NUMBER_OF_NODES))
# open('output.txt', 'w').write('%s' % '\n'.join(output))