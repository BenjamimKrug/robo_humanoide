import csv
import sys 

file_outputs = []

results = []
try:
    var_type = sys.argv[3]
except:
    var_type = "pwm_val_t"


with open(sys.argv[1]) as csvfile:
    reader = csv.reader(csvfile)
    for row in reader: # each row is a list
        results.append(row)

for i in range(1, len(results[0])):
    file_outputs.append(var_type + " " + results[0][i] + "_comp_array[] = {\n")

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

header_guard = sys.argv[2].upper().replace(".", "_")
last_index = header_guard.rfind("/")
if last_index != -1:
    header_guard = header_guard[last_index + 1:]

output = "#ifndef " + header_guard + "\n"
output += "#define " + header_guard + "\n"
output += "#include <stdint.h>\n"
output += "#include <stdio.h>\n\n"
output += "\n\n".join(file_outputs)
output += "\n\n#endif"

with open(sys.argv[2], "w") as file:
    file.write(output)
