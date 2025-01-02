import random
import string

def generate_random_word():
    return ''.join(random.choices(string.ascii_lowercase, k=random.randint(1,10)))

def generate_csv(n, m, filename='random_words.csv'):
    
    word_types = [generate_random_word() for _ in range(m)]
    
    words = [random.choice(word_types) for _ in range(n)]
    
    with open(filename, mode='w', newline='') as file:
        for word in words:
            file.write(word)
            file.write(',')

n = 10000
m = 100
generate_csv(n, m, "files/word_count.txt")
