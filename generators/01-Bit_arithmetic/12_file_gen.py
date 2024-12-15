import random

with open(file="12_test_file", mode="w", encoding="UTF-8") as fptr:
    for i in range(10**6):
        fptr.write(chr(random.randint(33, 126)))

