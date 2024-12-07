import random
import string

def generate_name():
    length = random.randint(3, 10)
    return ''.join(random.choices(string.ascii_letters, k=length)).capitalize()

def generate_salary():
    return round(random.uniform(30000, 120000), 2)

filename = "files/employees.txt"
count = 100000
with open(filename, "w") as file:
    for i in range(count):
        id = i
        name = generate_name()
        surname = generate_name()
        salary = generate_salary()
        file.write(f"{id} {name} {surname} {salary}\n")

print(f"File {filename} has been created with {count} employees.")
