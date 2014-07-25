__author__ = 'xtotdam'

from GameField import GameField

# todo: scores
# todo: highest chain
# todo: add clrprint as submodule

a = []
for i in xrange(1, 9):
    a.append([i]*8)
del a

gf = GameField(debugging=True)
gf.init(num=30)
gf.fall()
gf.output()

while not gf.gameover:
    while not gf.ready:
        if gf.chains > 1:
            gf.output()
        gf.check()
        gf.destroy()
        gf.explode()
        gf.check_for_ready()
        gf.debug_print()
        gf.remove_and_reveal()
        gf.fall()
        gf.newlevel()
        if gf.highestchain < gf.chains:
            gf.highestchain = gf.chains
        gf.chains = 1
    gf.output()
    try:
        gf.placenew()
    except (EOFError, KeyboardInterrupt):
        gf.gameover = True
    gf.fall()
gf.gameoverscreen()