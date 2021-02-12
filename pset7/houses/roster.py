# TODO
import sys
import csv
from cs50 import SQL

def main():
    #Check command-line arguments
    if len(sys.argv) != 2:
        print("Usage: python roster.py name_of_the_house")
        return 1

    db = SQL("sqlite:///students.db")

    students_table = db.execute("SELECT * FROM students WHERE house = ? ORDER BY last, first;", sys.argv[1])

    for elem in students_table:

        if elem["middle"] == None:
            print(elem["first"] + " " + elem["last"] + ", born " + str(elem["birth"]))

        else:
            print(elem["first"] + " " + elem["middle"] + " " + elem["last"] + ", born " + str(elem["birth"]))


if __name__ == '__main__':
    main()