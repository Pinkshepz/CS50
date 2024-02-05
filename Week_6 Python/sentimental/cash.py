from cs50 import get_float


def main():
    # Ask how many dollars the customer owed
    dollars = get_dollars()

    # Calculate the number of quarters to give the customer
    quarters = calculate_quarters(round(dollars, 2))
    dollars = dollars - quarters * 0.25

    # Calculate the number of dimes to give the customer
    dimes = calculate_dimes(round(dollars, 2))
    dollars = dollars - dimes * 0.10

    # Calculate the number of nickels to give the customer
    nickels = calculate_nickels(round(dollars, 2))
    dollars = dollars - nickels * 0.05

    # Calculate the number of pennies to give the customer
    pennies = calculate_pennies(round(dollars, 2))
    dollars = dollars - pennies * 0.01

    # Sum coins
    coins = quarters + dimes + nickels + pennies

    # Print total number of coins to give the customer
    print(coins)


def get_dollars():
    while True:
        dollars = get_float("Change owed: ")
        if dollars > 0:
            break
    return dollars


def calculate_quarters(dollars):
    count_qt = 0
    while dollars >= 0.25:
        dollars -= 0.25
        count_qt += 1
    return count_qt


def calculate_dimes(dollars):
    count_dm = 0
    while dollars >= 0.10:
        dollars -= 0.10
        count_dm += 1
    return count_dm


def calculate_nickels(dollars):
    count_ni = 0
    while dollars >= 0.05:
        dollars -= 0.05
        count_ni += 1
    return count_ni


def calculate_pennies(dollars):
    return int(dollars * 100)


# Call function
main()
