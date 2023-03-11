from cs50 import get_string


def main():

    # Get the paragraph from the user.
    paragraph = get_string("Text: ")

    # Get the number of characters in the paragraph.
    total_chars = find_chars(paragraph)

    # Get the number of words in a paragraph.
    total_words = find_words(paragraph)

    # Get the number of sentances in a paragraph.
    total_sentances = find_sentances(paragraph)

    # Calculate L and S.
    l = (total_chars / (total_words / 100.00))
    s = (total_sentances / (total_words / 100.00))

    # Calculate the index.
    index = round(0.0588 * l - 0.296 * s - 15.8)

    # Print the readability level.
    if index >= 16:
        print("Grade 16+")
    elif index < 1:
        print("Before Grade 1")
    else:
        print(f"Grade {index}")


def find_chars(text):

    total = 0

    # For every character we should add 1 to the total count.
    for i in range(len(text)):
        if (text[i].upper() in ["A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z"]):
            total += 1

    # Return the total number of characters to the program.
    return total


def find_words(text):

    # The count should start from 1 because the last word won't otherwise be counted.
    total = 1

    # For every space we should add 1 to the total count.
    for i in range(len(text)):
        if (text[i] == " "):
            total += 1

    # Return the total number of words to the program.
    return total


def find_sentances(text):

    total = 0

    # For every . ! ? we should add 1 to the total count.
    for i in range(len(text)):
        if (text[i] == '.' or text[i] == '!' or text[i] == '?'):
            total += 1

    # Return the total number of sentances to the program.
    return total


if __name__ == "__main__":
    main()