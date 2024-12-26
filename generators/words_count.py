import random
import string

def generate_random_word(length=5):
    return ''.join(random.choices(string.ascii_lowercase, k=length))

def generate_csv(n, m, filename='random_words.csv'):
    
    word_types = [generate_random_word() for _ in range(m)]
    
    words = [random.choice(word_types) for _ in range(n)]
    
    with open(filename, mode='w', newline='') as file:
        for word in words:
            file.write(word)
            file.write(',')

n = 1000
m = 52
generate_csv(n, m, "files/word_count.txt")
