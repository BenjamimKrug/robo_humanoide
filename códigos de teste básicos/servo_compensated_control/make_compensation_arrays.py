import csv
import sys 

file_outputs = []

results = []

with open(sys.argv[1]) as csvfile:
    reader = csv.reader(csvfile)
    for row in reader: # each row is a list
        results.append(row)

for i in range(1, len(results[0])):
    file_outputs.append("pwm_val_t " + results[0][i] + "_comp_array[] = {\n")

last_pos = len(results) - 1

for i in range(1, last_pos - 1):
    for j in range(1, len(results[i])):
        pos = j - 1
        file_outputs[pos] += "\t"
        distance = int(results[i + 1][0]) - int(results[i][0])
        start = int(results[i][j])
        end = int(results[i + 1][j])
        step = (end - start) / distance
        for p in range(0, distance):
            file_outputs[pos] += str(int(start + p * step)) + ", "
        file_outputs[pos] += "\n"

for j in range(1, len(results[last_pos])):
    pos = j - 1
    file_outputs[pos] += "\t" + results[last_pos][j] + ",\n"

for i in range(0, len(results[0]) - 1):
     file_outputs[i] += "};"

print(file_outputs)

output = "\n\n".join(file_outputs)
print(output)

