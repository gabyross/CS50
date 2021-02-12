from cs50 import get_string

def count_letters(text):

    letters = 0
    n = len(text)

    for i in range(n):
        # check if is an alphabetic character
        if (text[i].isalpha()):
            # increment letters wether is opper or lowercase
            letters += 1

    return letters


def count_words(text):

    words = 0
    n = len(text)

    for i in range(n):
        # check if is an alphabetic character with an space
        if (i != n - 1):
            if ((text[i].isspace() == False) and text[i + 1].isspace()):
                # increment words if is a letter followed by a space
                words += 1

    # si la posicion del largo del string menos uno es un caracter, incrementar la variable words
    if (text[n - 1].isspace() == False):
        words += 1

    return words


def count_sentences(text):

    sentences = 0
    n = len(text)

    for i in range(n):
        # check if is a character used to end sentences
        if ((text[i] == '!') or (text[i] == '.') or (text[i] == '?')):
            # increment sentences if is a !, . or a ?
            sentences += 1

    return sentences


def main():
    # ask user for text
    text = get_string("Text: ")

    # count number of letters, words, and sentences in the text, and print it
    letters = count_letters(text)
    words = count_words(text)
    sentences = count_sentences(text)

    # they must be floats
    L = (100 * letters) / words
    S = (100 * sentences) / words

    # round grade to the closest int number
    grade = round(0.0588 * L - 0.296 * S - 15.8)

    if (grade > 16):
        print("Grade 16+")

    elif (grade < 1):
        print("Before Grade 1")

    else:
        print("Grade ", grade)


if __name__ == '__main__':
    main()