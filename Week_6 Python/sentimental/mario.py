# ---------- Mario-less ----------

# Get height of pyramid
while True:
    try:
        height = int(input("Height: "))
        if height > 0 and height < 9:
            break
    except:
        pass

# Print pyramid
print(*((" " * (height - i - 1)) + ("#" * (i + 1)) for i in range(height)), sep = "\n")

# ---------- Mario-more ----------

# Print pyramid
print(*((" " * (height - i - 1)) + ("#" * (i + 1)) + "  " + ("#" * (i + 1))
        for i in range(height)), sep="\n")
