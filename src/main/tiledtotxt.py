import json
import sys

def process(l):
    return ['P' if i == 54 else 'F' if i == 45 else 'M' for i in l]

with open(sys.argv[1]) as data:
    grid = json.load(data)

with open('grid.txt', 'w') as g:
    lol = process(grid['layers'][0]['data'])
    g.write(''.join(''.join(map(str, lol))) + '\n')
