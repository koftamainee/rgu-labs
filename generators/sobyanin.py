import random
import string


def random_name(length=10):
    return "".join(random.choices(string.ascii_letters, k=length))


def random_gender():
    return random.choice(["M", "W"])


def random_income():
    return round(random.uniform(1000, 5000), 2)


def random_birth_date():
    day = random.randint(1, 28)
    month = random.randint(1, 12)

    day = str(day).zfill(2)
    month = str(month).zfill(2)

    year = random.randint(1950, 2000)
    return f"{day}.{month}.{year}"


def generate_test_file(file_path, num_records=50):
    with open(file_path, "w") as file:
        for _ in range(num_records):
            last_name = random_name()
            first_name = random_name()
            patronymic = random_name()
            birth_date = random_birth_date()
            gender = random_gender()
            income = random_income()
            file.write(
                f"{last_name} {first_name} {patronymic} {birth_date} {gender} {income}\n"
            )


test_file_path = "files/city.txt"
generate_test_file(test_file_path, 50)
print(f"File created: {test_file_path}")
