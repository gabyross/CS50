from cs50 import get_int

# function that counts digits
# numbers will be numbers from input
def count_digits(numbers):
    # starting number of digits
    digits_amount = 0
    while (numbers != 0):
        numbers = numbers // 10
        digits_amount += 1
    return digits_amount


def add_digits(num):
    sum = 0
    while (num != 0):
        last_digit = num % 10
        num = (num // 10)
        sum += last_digit
    return sum


def odd_digits(num):
    odds_sum = 0
    # elimino el ultimo digito para empezar con el penultimo
    num = num // 10
    while (num != 0):
        # obtengo el penultimo digito
        odd_num = num % 10
        # asumo que es impar y lo multiplico por dos
        mul = odd_num * 2
        # sumo los digitos impares
        odds_sum += add_digits(mul)
        # divido entrre 100 para actualizar el numero
        num = num // 100
    return odds_sum


def even_digits(num):
    even_sum = 0
    while (num != 0):
        # obtengo el ultimo digito una vez dividido
        even_num = num % 10
        # lo sumo
        even_sum += even_num
        # elimino dos digitos para sumar los no mltiplicados
        new_num = num // 100
        # actualizo el numero
        num = new_num
    return even_sum


def main():
    #ask user for credit number
    credit = get_int("Credit number: ")

    #calculate number of digits
    number_of_digits = count_digits(credit)

    # verifying odd and even digits to sum
    odd_sum = odd_digits(credit)
    even_sum = even_digits(credit)

    total_sum = odd_sum + even_sum

    # checksum
    is_valid = False  #(total_sum % 10 != 0)

    if (total_sum % 10 == 0):
        is_valid = True

    # chech what kind of credit card is given
    first_2_numbers = credit // 10**(number_of_digits - 2)
    first_number = credit // 10**(number_of_digits - 1)
    type_c = "INVALID"

    if ((is_valid) and (number_of_digits == 13 or number_of_digits == 16) and first_number == 4):
        type_c = "VISA"

    elif ((is_valid) and number_of_digits == 15 and (first_2_numbers == 34 or first_2_numbers == 37)):
        type_c = "AMEX"

    elif ((is_valid) and number_of_digits == 16 and (first_2_numbers == 51 or first_2_numbers == 52
             or first_2_numbers == 53 or first_2_numbers == 54 or first_2_numbers == 55)):
        type_c = "MASTERCARD"

    print(type_c)
    return 0

if __name__ == '__main__':
    main()