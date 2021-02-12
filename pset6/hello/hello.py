from cs50 import get_string
def main():
    #ask user for name
    name = get_string("What is your name?: ")
    print("hello, " + name)

if __name__ == '__main__':
    main()