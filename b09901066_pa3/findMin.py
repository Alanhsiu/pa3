data = {}

# Read data from file
with open('ibm01.txt', 'r') as file:
    for line in file:
        key, value = line.strip().split()
        data[int(key)] = float(value)

# Find key with minimum value
min_value = min(data.values())
min_key = min(data, key=data.get)

print("The key corresponding to the minimum value is:", min_key)
