from cs50 import get_int


# Get input for credit card number
while True:
    credit = get_int("Number: ")
    if credit > 0:
        break

# Create generator for credit digits
creditGenerator = reversed(list(int(digit) for digit in str(credit)))

# Create variable for cumulative sum
sum = 0

# Iterate over digits
while True:
    try:
        # Collect odd position value
        sum += next(creditGenerator)
    except:
        # If there is no remaining digits
        break

    try:
        # Collect even position value
        product = next(creditGenerator) * 2
        sum += (product % 10) + int(product / 10)
    except:
        # If there is no remaining digits
        break


# Return whether credit number is vaild or not
if sum % 10 == 0:
    if ((str(credit)[0:2] in ["34", "37"]) & (len(str(credit)) == 15)):
        print("AMEX")

    elif ((str(credit)[0] == "4") & (len(str(credit)) in [13, 16])):
        print("VISA")

    elif (((int(str(credit)[0:2]) > 50) & (int(str(credit)[0:2]) < 56)) & (len(str(credit)) == 16)):
        print("MASTERCARD")

    else:
        print("INVALID")

else:
    print("INVALID")
