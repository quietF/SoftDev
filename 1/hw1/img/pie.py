import plotly.plotly as py
import sys

input_file = str(sys.argv[1])

print(input_file)

a = open(input_file, 'r').readlines()

labels = a[0].split(' ')
values = a[1].split(' ')

var = input_file.replace('cProfile','').replace('.dat','')
var = var[1:]
pos = var.index('_')

percentage = var[:pos]
var = var[pos:]
pos = var.index('_')

side = var[pos:]
side = side.replace('_','')

title = str('cProfile for {0}x{0} input landscape file and {1}% land-to-water.'.format(side, percentage))

fig = {'data': [{'labels': labels, 'values': values, 'type': 'pie'}], 'layout': {'title': title}}

output_file = input_file.replace('.dat', '')

py.plot(fig, filename = output_file, image = 'png')
