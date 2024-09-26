fptr1 = open(file="12_test_file", mode="r", encoding="UTF-8")
fptr2 = open(file="12_test_file_decrypted", mode="r", encoding="UTF-8")

data1 = fptr1.read()
data2 = fptr2.read()
if data1 == data2:
    print("Correct!")

fptr1.close()
fptr2.close()