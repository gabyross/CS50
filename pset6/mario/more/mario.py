from cs50 import get_int
def main():
    #ask user for height
    height = get_int("Height: ")

    hash = "#"
    dot = " "

    while (height < 1 or height >= 9):
        height = get_int("Height: ")


    for i in range(1, height + 1):

        #print dots before hashes
        for d in range(height - i):
            print(dot, end="")


        #print every repetition of hashes in a new line
        for r in range(i):
            print(hash, end="")
            r += 1

        #print dots before hashes
        for d in range(2):
            print(dot, end="")
            d += 1

        #print every repetition of hashes in a new line
        for r in range(i):
            print(hash, end="")
            r += 1

        print("\n", end="")
        i += 1


if __name__ == '__main__':
    main()