#include <cs50.h>
#include <stdio.h>

int main(void)
{

    //Get the credit card number.
    long number = get_long("Number: ");

    //Find the individual card numbers.
    int one, two, three, four, five, six, seven, eight, nine, ten, eleven, twelve, thirteen, fourteen, fifteen, sixteen;

    one = number % 10;
    two = ((number % 100) / 10);
    three = ((number % 1000) / 100);
    four = ((number % 10000) / 1000);
    five = ((number % 100000) / 10000);
    six = ((number % 1000000) / 100000);
    seven = ((number % 10000000) / 1000000);
    eight = ((number % 100000000) / 10000000);
    nine = ((number % 1000000000) / 100000000);
    ten = ((number % 10000000000) / 1000000000);
    eleven = ((number % 100000000000) / 10000000000);
    twelve = ((number % 1000000000000) / 100000000000);
    thirteen = ((number % 10000000000000) / 1000000000000);
    fourteen = ((number % 100000000000000) / 10000000000000);
    fifteen = ((number % 1000000000000000) / 100000000000000);
    sixteen = ((number % 10000000000000000) / 1000000000000000);

    //Step One - Multiply every other digit by two.
    int sumTwo, sumFour, sumSix, sumEight, sumTen, sumTwelve, sumFourteen, sumSixteen;

    sumTwo = (two * 2);
    sumFour = (four * 2);
    sumSix = (six * 2);
    sumEight = (eight * 2);
    sumTen = (ten * 2);
    sumTwelve = (twelve * 2);
    sumFourteen = (fourteen * 2);
    sumSixteen = (sixteen * 2);

    //Find the sum of the individual product digits
    int firstTwo, firstFour, firstSix, firstEight, firstTen, firstTwelve, firstFourteen, firstSixteen,
        secondTwo, secondFour, secondSix, secondEight, secondTen, secondTwelve, secondFourteen, secondSixteen;

    if (sumTwo > 9)
    {
        firstTwo = sumTwo % 10;
        secondTwo = ((sumTwo % 100) / 10);
        sumTwo = (firstTwo + secondTwo);
    }

    if (sumFour > 9)
    {
        firstFour = sumFour % 10;
        secondFour = ((sumFour % 100) / 10);
        sumFour = (firstFour + secondFour);
    }

    if (sumSix > 9)
    {
        firstSix = sumSix % 10;
        secondSix = ((sumSix % 100) / 10);
        sumSix = (firstSix + secondSix);
    }

    if (sumEight > 9)
    {
        firstEight = sumEight % 10;
        secondEight = ((sumEight % 100) / 10);
        sumEight = (firstEight + secondEight);
    }

    if (sumTen > 9)
    {
        firstTen = sumTen % 10;
        secondTen = ((sumTen % 100) / 10);
        sumTen = (firstTen + secondTen);
    }

    if (sumTwelve > 9)
    {
        firstTwelve = sumTwelve % 10;
        secondTwelve = ((sumTwelve % 100) / 10);
        sumTwelve = (firstTwelve + secondTwelve);
    }

    if (sumFourteen > 9)
    {
        firstFourteen = sumFourteen % 10;
        secondFourteen = ((sumFourteen % 100) / 10);
        sumFourteen = (firstFourteen + secondFourteen);
    }

    if (sumSixteen > 9)
    {
        firstSixteen = sumSixteen % 10;
        secondSixteen = ((sumSixteen % 100) / 10);
        sumSixteen = (firstSixteen + secondSixteen);
    }

    //Add the first set of digits together.

    int firstTotal = (sumTwo + sumFour + sumSix + sumEight + sumTen + sumTwelve + sumFourteen + sumSixteen);

    //Add the second set of digits together.

    int secondTotal = (one + three + five + seven + nine + eleven + thirteen + fifteen);

    //Add the first and second totals together.

    int total = (firstTotal + secondTotal);

    //Check the last digit of the total is a 0.

    int lastTotal = total % 10;

    //Validate.

    int valid;

    if (lastTotal == 0)
    {
        valid = 1;
    }
    else
    {
        valid = 0;
    }

    //Check AMEX.
    if (((sixteen == 0) && (fifteen == 3)) && ((fourteen == 4) || (fourteen == 7)) && (valid == 1))

    {
        printf("AMEX\n");
    }

    //Check Mastercard.
    else if (((sixteen == 5) && ((fifteen == 1) || (fifteen == 2) || (fifteen == 3) || (fifteen == 4) || (fifteen == 5)))
             && (valid == 1))
    {
        printf("MASTERCARD\n");
    }

    //Check Visa - 16 Digits.
    else if ((sixteen == 4) && (valid == 1))
    {
        printf("VISA\n");
    }

    //Check Visa - 13 Digits.
    else if (((sixteen == 0) && (fifteen == 0) && (fourteen == 0) && (thirteen == 4)) && (valid == 1))
    {
        printf("VISA\n");
    }

    //If no match - Invalid Card.
    else
    {
        printf("INVALID\n");
    }
}

