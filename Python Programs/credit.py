from cs50 import get_int

cardNumber = get_int("Number: ")

# Find the individual card numbers.
numbers = []
x, y = 10, 1

for i in range(16):
    numbers.append(int((cardNumber % x) / y))
    x *= 10
    y *= 10
    
# Find the first total
firstTotal = 0

for j in range(1, 17, 2):
    if ((numbers[j] * 2) > 9):
        firstTotal += (int((numbers[j] * 2) % 10)) + (int(((numbers[j] * 2) % 100) / 10))
    else:
        firstTotal += (numbers[j] * 2)

# Find the second total
secondTotal = 0

for s in range(0, 16, 2):
    secondTotal += numbers[s]

# Add the first and second totals together.
total = (firstTotal + secondTotal)

# Check the last digit of the total is a 0.
lastTotal = total % 10

# Validate.
valid = 0

if lastTotal == 0:
    valid = 1

else:
    valid = 0

# Check AMEX.
if ((numbers[15] == 0) and (numbers[14] == 3)) and ((numbers[13] == 4) or (numbers[13] == 7)) and (valid == 1):
    print("AMEX")

# Check Mastercard.
elif (numbers[15] == 5) and ((numbers[14] == 1) or (numbers[14] == 2) or (numbers[14] == 3) or (numbers[14] == 4) or (numbers[14] == 5)) and (valid == 1):
    print("MASTERCARD")

# Check Visa - 16 Digits.
elif (numbers[15] == 4) and (valid == 1):
    print("VISA")

# Check Visa - 13 Digits.
elif ((numbers[15] == 0) and (numbers[14] == 0) and (numbers[13] == 0) and (numbers[12] == 4)) and (valid == 1):
    print("VISA")

# If no match, then the card is invalid.
else:
    print("INVALID")