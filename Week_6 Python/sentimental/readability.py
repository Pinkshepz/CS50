from cs50 import get_string


# get text
text = get_string("Text: ")

# create variable to store text parameters
letters = 0
words = 1.0
sentences = 0

for i in range(len(text)):
    if ((text[i].lower() >= "a" and text[i].lower() <= "z")):
        letters += 1

    if (text[i] == " "):
        words += 1

    if (text[i] == '.' or text[i] == '!' or text[i] == '?'):
        sentences += 1


# calculate Coleman-Liau index
L = 100 * (letters / words)
S = 100 * (sentences / words)

index = round(0.0588 * L - 0.296 * S - 15.8)

# print out grade
if (index < 1):
    print("Before Grade 1")

if (index > 16):
    print("Grade 16+")

print(f"Grade {index}")
