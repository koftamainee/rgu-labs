import random

with open(file="14.txt", mode="w", encoding="UTF-8") as fptr:
        for i in range(10000):
            fptr.write(str(random.randint(-10000, 10000)) + " ")