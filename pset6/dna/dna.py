import sys
import csv
import re

def cargar_csv():
    #Open the csv file and read its contents into memory
    with open(sys.argv[1], 'r') as csv_file:
        #convert into sorted dict
        reader = list(csv.DictReader(csv_file))
        return reader


def cargar_adn():
    #Open the DNA sequence and read its contents into memory
    with open(sys.argv[2], 'r') as dna_txt_file:
        return dna_txt_file.readline()


def str_seqs(dna_txt_file, reader):
    #Compute the individual STR counts

    #Agrego las llaves a una lista para luego compararlas... AGATC, etc
    list_of_keys = list(reader[0].keys())[1:]

    #compute how many times do they repeat consecutively in a person dna
    list_of_values = []
    cadena = dna_txt_file

    for elem in list_of_keys:
        key = elem
        #compute how many times STR repeats starting at that position
        count = count_keys(cadena, key)

        list_of_values.append(count)

    #volverlo un dicitonario para comparar al final y obtener el match
    result = dict(zip(list_of_keys, list_of_values))
    return result


def count_keys(cadena, key):

    #get all the groups of key
    #example.. key = 'aa'   groups = aa, aaaa, aaaaaa, aa
    groups = re.findall('((?:' + re.escape(key) + ')*)', cadena)

    #NOTA PARA GABY DEL FUTURO
    #re = regular expresions
    #findall() devuelve todas las subcadenas de la entrada que coinciden con el patrón sin superposición
    #* se repite cero o más veces (Permitiendo que un patrón se repita cero veces significa que no necesita aparecer en absoluto para que coincida)
    #?:patron  Para crear un grupo sin captura, usa la sintaxis...
    '''Definir un grupo que contenga un subpatrón también es útil en casos
    donde la cadena que coincide con el subpatrón no es parte de lo que
    debería ser extraído del texto completo. Este tipo de grupos se llama
    sin captura. Los grupos sin captura se pueden usar para describir patrones
    de repetición o alternativas, sin aislar la porción de emparejamiento de la
    cadena en el valor devuelto'''

    #calculate the largest group of all
    largest = max(groups, key=len)

    #obtain the specific number of key maximum repetitons
    counter = (len(largest) // len(key))

    return counter


def compare_dna(sqc_dict, ppl_dna):
    #Compare the STR counts against each row in the csv file
    #sqc_dict es un dict con la cantidad de veces que se repite cada secuencia en el archivo de adn completo
    #ppl_dna es una lista donde cada elemento es un dict por persona con la secuencia de adn individual de las personas
    name = 'No match'

    for person in ppl_dna:
        count = 0
        for pair in sqc_dict.items():
            #print(pair)
            key = pair[0]
            value = pair[1]
            person_value = int(person[key])

            if person_value == value:
                count+=1
                if len(sqc_dict) == count:
                    name = person['name']
                    break

    print(name)


def main():
    #Check command-line arguments
    if len(sys.argv) != 3:
        print("Usage: python dna.py data.csv sequence.txt")
        return 1

    #Open the csv file and read its contents into memory
    reader = cargar_csv()

    #Open the DNA sequence and read its contents into memory
    dna_txt_file = cargar_adn()

    #compute the individual STR counts (how many times do they repeat consecutively in a person dna)
    secuencia_de_adn = str_seqs(dna_txt_file, reader)

    #Obtain possible match
    compare_dna(secuencia_de_adn, reader)


if __name__ == '__main__':
    main()