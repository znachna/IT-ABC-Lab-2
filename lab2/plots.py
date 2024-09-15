import os
import json
import matplotlib.pyplot as plt

current_dir = os.path.dirname(os.path.abspath(__file__))
json_file_path = os.path.join(current_dir, 'points.json')

with open(json_file_path, 'r') as f:
    data = json.load(f)

x_initial = []
y_initial = []

for point in data['initial function']:
    x_value = point['x']
    y_value = point['y']
    x_initial.append(x_value)
    y_initial.append(y_value)

x_polynomial = []
y_polynomial = []

for point in data['polynomial function']:
    x_value = point['x']
    y_value = point['y']
    x_polynomial.append(x_value)
    y_polynomial.append(y_value)

where_below = []
where_above = []

for i in range(len(y_initial)):
    y_i = y_initial[i]
    y_p = y_polynomial[i]

    if y_i <= y_p:
        where_below.append(True)
        where_above.append(False)
    else:
        where_below.append(False)
        where_above.append(True)

plt.figure(figsize=(10, 10))

plt.subplot(2, 2, 1)
plt.plot(x_initial, y_initial, marker='o', label='Initial Function', color='blue')
plt.title('Initial Function')
plt.xlabel('X')
plt.ylabel('Y')
plt.grid(True)
plt.legend()

plt.subplot(2, 2, 2)
plt.plot(x_polynomial, y_polynomial, marker='o', label='Polynomial Function', color='green')
plt.title('Polynomial Function')
plt.xlabel('X')
plt.ylabel('Y')
plt.grid(True)
plt.legend()

plt.subplot(2, 1, 2)
plt.plot(x_initial, y_initial, label='Initial Function', color='blue', marker='o')
plt.plot(x_polynomial, y_polynomial, label='Polynomial Function', color='green', marker='o')

plt.fill_between(x_initial, y_initial, y_polynomial, where=where_below, 
                 interpolate=True, color='lightgreen', alpha=0.5)

plt.fill_between(x_initial, y_initial, y_polynomial, where=where_above, 
                 interpolate=True, color='lightblue', alpha=0.5)

plt.title('Overlay and Area Between Functions')
plt.xlabel('X')
plt.ylabel('Y')
plt.grid(True)

plt.tight_layout()
plt.show()
