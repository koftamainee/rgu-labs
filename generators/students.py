import random
import string

def generate_student_id():
    return random.randint(1, 1000)

def generate_name():
    length = random.randint(3, 10)
    return ''.join(random.choices(string.ascii_letters, k=length))

def generate_group():
    return ''.join(random.choices(string.ascii_uppercase + string.digits, k=5))

def generate_grades():
    return [random.randint(0, 100) for _ in range(5)]

def generate_student():
    student_id = generate_student_id()
    first_name = generate_name()
    last_name = generate_name()
    group = generate_group()
    grades = generate_grades()
    return f"{student_id} {first_name} {last_name} {group} {' '.join(map(str, grades))}"

def generate_students_file(filename, count):
    with open(filename, 'w') as file:
        for _ in range(count):
            file.write(generate_student() + '\n')

if __name__ == "__main__":
    filename = "files/students.txt"
    count = 100
    generate_students_file(filename, count)
    print("File created")
