import sys
import csv


def main():

    # Check to ensure the user has used the correct command-line arguments.
    if len(sys.argv) != 3:
        print("Usage: python dna.py data.csv sequence.txt")
        sys.exit(1)

    # Create a list of the nucleotides being used in the uploaded database.
    columns = []
    with open(sys.argv[1], "r") as csvfile:
        reader = csv.reader(csvfile)
        for i in reader:
            for column in i[1:]:
                columns.append(column)
            break

    # Create a table of everyone listed in the database. Append their names and counts to the table.
    database = []
    with open(sys.argv[1]) as csvfile:
        readerTwo = csv.DictReader(csvfile)
        for x in readerTwo:
            database.append(x)

    # Read the DNA sequence into memory on file.
    sequence = []
    dna = ""
    with open(sys.argv[2], "r") as file:
        reader = csv.reader(file)
        for row in reader:
            dna = row[0]
        for char in dna:
            sequence.append(char)
            
    # Build a list of the total number of times each nucleotide appears in a DNA sequence.  
    countList = stepOne(sequence, columns)
    
    # Check the database for a DNA match.
    # For every person in the database.
    match = 0
    for person in database:
        matchCount = 0 
        counter = 0
        
        # Check every nucleotide count in the countList and compare it to the count for each nucleotide for that person.
        for z in columns:
            if int(person[z]) == int(countList[counter]):
                matchCount += 1
                counter += 1
            else:
                break
            
        # If the counts for the person match the counts in the countList, then print the persons name.
        if matchCount == len(countList):
            print(person["name"])
            match += 1
            
    # If there are no matches, print No Match.
    if match == 0:
        print("No Match")

            
def stepOne(sequence, columns):
    
    # For each nucleotide, run the second stepo function which will calculate the highest sequencial count in the DNA sequence´.
    countList = []
    for j in columns:
        nucleotide = []
        for char in j:
            nucleotide.append(char)
        count = stepTwo(sequence, nucleotide)
        countList.append(count)
    
    # Return the count for each nucleotide to the main function as a list.
    return countList


def stepTwo(sequence, nucleotide):
    
    # Set the tracking variables to 0.
    count, position, total = 0, 0, 0
    length = (len(nucleotide))
    
    # While you have not reached the end of the list, check to see if you have found the DNA combination.
    while position < (len(sequence) - (length - 1)):
        true = 0
        for j in range(length):
            if sequence[position + j] == nucleotide[j]:
                true += 1
            else:
                position += 1
                count = 0
                break
        
        # If a combination has been found, increment the count and reposition the position variable to check the next location.
        if true == length:
            count += 1
            position += length
            
        # If the last sequential count is the highest recorded so far, then set the total as that number.
        if count > total:
            total = count

    # Return the highest count total to the previous function, to add it to the list.
    return total


if __name__ == "__main__":
    main()
