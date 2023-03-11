from cs50 import get_int

# Prompt the user for the height of the pyramid. Repeat until the user enters a number between 1 and 8.
while True:
    n = get_int("Height: ")
    if n >= 1 and n <= 8:
        break

# Create a variable for the number of spaces and the number of hashes.
space = (n - 1)
hashSymbol = (n - space)

# For each row in the height of the pyramid.
for i in range(n):
    
    # Print the spaces.
    for s in range(space):
        print(" ", end="")
        
    # Print the hashes.
    for h in range(hashSymbol):
        print("#", end="")
        
    # Print the middle gap.
    print("  ", end="")
    
    # Print the second pyramid
    for j in range(hashSymbol):
        print("#", end="")
    
    # Create a new row.
    print()
    
    # Adjust the new space and hash count for the next row.
    space -= 1
    hashSymbol += 1

