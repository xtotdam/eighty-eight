from __future__ import print_function

__author__ = 'xtotdam'

import numpy as np
from pyColorprint.clrprint import color_print_num as cpn


def nextblock():
    """
Generates next block value, prefers open blocks

    :return: next block value
    :rtype: integer
    """
    return 20 * np.random.choice(2, p=[.75, .25]) + np.random.choice(9) + 1


def check_array(array):
    """
Checks line for blocks in it and generates its clone with number of blocks in line in the blocks places

    :param array: 1D array with blocks
    :return: checked line
    :rtype : 1D np.array
    """
    check_lines = []
    for c_ind, line in enumerate(array):
        c = np.zeros(len(line) + 1)
        value = 0
        for ind, item in enumerate([int(bool(i)) for i in np.append(line, [0])]):
            if item:
                value += 1
            else:
                c[ind - value:ind] = [value] * value
                value = 0
        check_lines.append(c[:-1])
    return np.array(check_lines, dtype=int)


def color_block(num):
    colors = {0: 242, 1: 21, 2: 68, 3: 51, 4: 46, 5: 226, 6: 214, 7: 196, 8: 129, 9: 201}
    blocks = {0: ' ', 1: '1', 2: '2', 3: '3', 4: '4', 5: '5', 6: '6', 7: '7', 8: '8', 9: '@'}
    shadow, white = 252, 15
    if not num / 10:
        return cpn('[' + blocks[num] + ']', colors[num], 0)
    elif num / 10 == 1:
        return cpn('[' + blocks[num / 10] + ']', shadow, 0)
    else:
        return cpn('[*]', white, 0)


class GameField():
    """
Gamefield class, describes gamefield, block falling, disappear checking, disappearing, disclosing etc mechanisms
    """
    level_edge = 20
    debug = False
    gameover = False
    ready = False
    nextb = nextblock()
    inited = False
    highestchain = 0

    def __init__(self, edge=level_edge, debugging=debug, initfield=None):
        if not initfield:
            self.field = np.zeros((8, 8), dtype=int)
        else:
            self.field = np.array(initfield, dtype=int)
            self.inited = True
        self.score = 0
        self.following = 0
        self.level = 1
        self.chains = 1
        self.level_edge = edge
        self.indicator = 0
        self.debug = debugging

    def init(self, num=20):
        """
Initializes game field with 'num' random blocks, chosen by .nextblock().

        :param num: blocks number, but their real number is equal or less
        :return: nothing
        """
        if not self.inited:
            for i in xrange(num):
                self.field[np.random.randint(0, 8)][np.random.randint(0, 8)] = nextblock()
            if self.debug:
                print('Initialized successful...')

    def output(self):
        """
Game field output function

        :return: nothing
        """
        print('\nNext: {}  Score: {}   Current chain: {}'.format(color_block(self.nextb), self.score, self.chains))
        for num, line in enumerate(self.field):
            print('{}|'.format(8 - num), end='')
            for block in line:
                print(color_block(block), end='')
            print()
        print('   1  2  3  4  5  6  7  8')
        print('New level progress: {}/{}'.format(self.indicator, self.level_edge))

    def debug_print(self):
        """
Print debug information about gamefield

        :return: nothing
        """
        if self.debug:
            print('\n'
                  'Field and check arrays: lines, rows, checks...\n'
                  'Ready now? {}; Next: {} ({}); Chain: {}'.format(self.ready, color_block(self.nextb),
                                                                   self.nextb, self.chains))
            for i in xrange(8):
                print('{}|'.format(8 - i), end='')

                for block in self.field[i]:
                    print(color_block(block), end='')
                print(' ', end='')

                for block in self.field[i]:
                    print('[{}]'.format(str(block).rjust(2)), end='')
                print(' ', end='')

                for value in self.checks[0][i]:
                    print(color_block(value), end='')
                print('#', end='')

                for value in self.checks[1][i]:
                    print(color_block(value), end='')
                print('#', end='')

                for value in self.checks[2][i]:
                    print(color_block(value), end='')
                print()

    def placenew(self):
        """
Places new block into game field

        :return: nothing
        """
        right = False
        self.indicator += 1
        place = 0
        while not right:
            place = raw_input('>>>  ')
            try:
                place = int(place)
                right = True
            except ValueError:
                try:
                    place = ord(place[0])
                    right = True
                except IndexError:
                    pass

        place += (place / 8 + 1) * 8
        place = (place - 1) % 8 + 1

        if self.debug:
            print('Placing new [{}] to <{}>...'.format(self.nextb, place))

        if self.field[0][place - 1]:
            self.gameover = True
        else:
            self.field[0][place - 1] = self.nextb
            self.nextb = nextblock()
        self.ready = False

    def fall(self):
        """
Makes flying blocks fall

        :return: nothing
        """
        fallen = 8
        if self.debug:
            print('Falling...')
        while fallen:
            fall = np.zeros(8, dtype=int)
            for j in xrange(8):
                for i in xrange(7):
                    if not self.field[i + 1][j] % 10 and self.field[i][j] % 10:
                        # checks for blank space and block above
                        (self.field[i + 1][j], self.field[i][j]) = (self.field[i][j], self.field[i + 1][j])
                        fall[j] = 1
            fallen = fall.sum()
            if self.debug:
                print(fall, fallen)
        if self.debug:
            print('Falling successful')

    def check(self):
        """
Checks for blocks, that are to be destroyed. Writes checked array to self.checks

        :return: nothing
        """
        check_lines = check_array(self.field)
        check_rows = check_array(self.field.T).T
        self.checks = np.array([check_lines, check_rows, np.zeros((8, 8))], dtype=int)
        if self.debug:
            print('Checked successfully...')

    def destroy(self):
        """
Destroys blocks. Sets flag for .reveal(), therefore for .check()

        :return: nothing
        """
        for i in xrange(8):
            for j in xrange(8):
                if self.field[i][j] and self.field[i][j] in (self.checks[0][i][j], self.checks[1][i][j]):
                    self.checks[2][i][j] = 1
        self.chains += 1
        if self.debug:
            print('Destroy matrix created successfully...')

    def explode(self):
        """
Explodes bombs, if can

        :return: nothing
        """
        for i in xrange(8):
            for j in xrange(8):
                if self.field[i][j] == 9 and (self.field.any(axis=0)[i] or self.field.any(axis=1)[j]):
                    self.checks[2][i][j] = 2
        if self.debug:
            print('Bombs added to destroy matrix successfully...')

    def check_for_ready(self):
        """
Checks for gamefield to be ready for new block placement

        :return: nothing
        """
        if not self.checks[2].sum():
            self.ready = True
        else:
            self.ready = False
        if self.debug:
            print('Ready? ', self.ready)

    def remove_and_reveal(self):
        """
Reveals blocks after destroys nearby. Uses magic arrays got from np.indices((3,3)), in tuples (x,y)

        :return: nothing
        """
        one = ((-1, -1), (-1, 1), (1, -1), (1, 1))
        two = ((-1, 0), (0, -1), (0, 1), (1, 0))
        indices = []
        for index, value in np.ndenumerate(self.checks[2]):
            if value:
                i, j = index[0], index[1]
                if value == 1:
                    scoreadd = 10 * self.field[i][j]
                    self.score += scoreadd
                    print('{} points for block destroying'.format(scoreadd))
                self.field[i][j] = 0
                for x, y in one:
                    indices.append((i + x, j + y))
                if value == 2:
                    self.score += 200
                    print('200 points for bomb exploding')
                    for x, y in two:
                        indices.append((i + x, j + y))
        indices = set(indices)
        for i, j in indices:
            try:
                prev = self.field[i][j]
                if self.field[i][j] / 10:
                    self.field[i][j] -= 10
                if self.field[i][j] == 19:
                    self.field[i][j] = 9  # because there are no shadowed bombs
                if prev != self.field[i][j]:
                    self.score += 100
                    print('100 points for block revealing')
            except IndexError:
                pass
        if self.debug:
            print('Blocks removed, neighbors revealed successfully...')

    def newlevel(self):
        """
Starts new level.

        :return: nothing
        """
        if self.indicator == self.level_edge:
            self.indicator = 0
            if self.debug:
                print('New level...')
            if self.field[0].sum():
                self.gameover = True
            np.roll(self.field, 1, axis=0)
            self.field[7] = np.array([nextblock() for i in xrange(8)])

    def gameoverscreen(self):
        """
Draws nice gameover screen

        :return: nothing
        """
        print('Game over')
        print('Your score : {}'.format(self.score))
        print('Highest chain : {}'.format(self.highestchain))