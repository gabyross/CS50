# TODO
import sys
import csv
from cs50 import SQL

def main():
    #Check command-line arguments
    if len(sys.argv) != 2:
        print("Usage: python import.py characters.csv")
        return 1

    db = SQL("sqlite:///students.db")

    #Open the csv file and read its contents into memory
    with open(sys.argv[1], 'r') as csv_file:
        #convert into sorted dict
        students_file = csv.DictReader(csv_file, delimiter = ",")

        for row in students_file:

            #determine heads and its content
            name = row["name"]
            names_list = name.split()

            house = row["house"]
            birth = row["birth"]

            #insert none value if the person doesnt have middle name
            if len(names_list) == 2:
                first = names_list[0]
                last = names_list[1]
                db.execute("INSERT INTO students (first, middle, last, house, birth) VALUES(?, ?, ?, ?, ?)", first, None, last, house, birth)

            #insert corresponding values
            elif len(names_list) == 3:
                first = names_list[0]
                middle = names_list[1]
                last = names_list[2]
                db.execute("INSERT INTO students (first, middle, last, house, birth) VALUES(?, ?, ?, ?, ?)", first, middle, last, house, birth)


if __name__ == '__main__':
    main()
