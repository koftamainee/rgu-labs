import random

with open(file="test", mode="w", encoding="UTF-8") as fptr:
    for i in range(10**6):
        fptr.write(chr(random.randint(33, 126)))