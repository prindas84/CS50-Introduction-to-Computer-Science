#include <stdio.h>
#include <cs50.h>
#include <ctype.h>

int check_errors(int argc, string key);
string cipher_code(string plain_text, string key);

int main(int argc, string argv[])
{
    //Check errors and return code.
    int error_code = check_errors(argc, argv[1]);

    //Error code 1.
    if (error_code == 1)
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }

    //Error code 2.
    else if (error_code == 2)
    {
        printf("Key must contain 26 characters.\n");
        return 1;
    }

    //Error code 3.
    else if (error_code == 3)
    {
        printf("Key must only contain alphabetic characters.\n");
        return 1;
    }

    //Error code 4.
    else if (error_code == 4)
    {
        printf("Key must not contain repeated characters.\n");
        return 1;
    }

    //Proceed without errors.
    else
    {
        string input = get_string("plaintext: ");

        string output = cipher_code(input, argv[1]);

        printf("ciphertext: %s\n", output);
        return 0;
    }

}

//Check Errors Function.
int check_errors(int argc, string key)
{
    int code = 0;

    //Check the user has inputted a key.
    if (argc != 2)
    {
        code = 1;
    }

    //Check the key is 26 characters.
    if (code != 1)
    {
        int length = 0;
        for (int i = 0; key[i] != '\0'; i++)
        {
            length++;
        }

        if (length != 26)
        {
            code = 2;
        }
    }

    //Check that all characters of the key are letters.
    if (code != 1 && code != 2)
    {
        int count = 0;
        for (int i = 0; key[i] != '\0'; i++)
        {
            if (isalpha(key[i]))
            {
                count++;
            }
        }

        if (count != 26)
        {
            code = 3;
        }
    }

    //Check that all characters have only been used once.
    if (code != 1 && code != 2 && code != 3)
    {
        int a = 0, b = 0, c = 0, d = 0, e = 0, f = 0, g = 0, h = 0, i = 0, j = 0, k = 0, l = 0, m = 0, n = 0, o = 0, p = 0, q = 0,
            r = 0, s = 0, t = 0, u = 0, v = 0, w = 0, x = 0, y = 0, z = 0;
        for (int counter = 0; counter < 26; counter++)
        {
            if (key[counter] == 'A' || key[counter] == 'a')
            {
                a++;
            }
            else if (key[counter] == 'B' || key[counter] == 'b')
            {
                b++;
            }
            else if (key[counter] == 'C' || key[counter] == 'c')
            {
                c++;
            }
            else if (key[counter] == 'D' || key[counter] == 'd')
            {
                d++;
            }
            else if (key[counter] == 'E' || key[counter] == 'e')
            {
                e++;
            }
            else if (key[counter] == 'F' || key[counter] == 'f')
            {
                f++;
            }
            else if (key[counter] == 'G' || key[counter] == 'g')
            {
                g++;
            }
            else if (key[counter] == 'H' || key[counter] == 'h')
            {
                h++;
            }
            else if (key[counter] == 'I' || key[counter] == 'i')
            {
                i++;
            }
            else if (key[counter] == 'J' || key[counter] == 'j')
            {
                j++;
            }
            else if (key[counter] == 'K' || key[counter] == 'k')
            {
                k++;
            }
            else if (key[counter] == 'L' || key[counter] == 'l')
            {
                l++;
            }
            else if (key[counter] == 'M' || key[counter] == 'm')
            {
                m++;
            }
            else if (key[counter] == 'N' || key[counter] == 'n')
            {
                n++;
            }
            else if (key[counter] == 'O' || key[counter] == 'o')
            {
                o++;
            }
            else if (key[counter] == 'P' || key[counter] == 'p')
            {
                p++;
            }
            else if (key[counter] == 'Q' || key[counter] == 'q')
            {
                q++;
            }
            else if (key[counter] == 'R' || key[counter] == 'r')
            {
                r++;
            }
            else if (key[counter] == 'S' || key[counter] == 's')
            {
                s++;
            }
            else if (key[counter] == 'T' || key[counter] == 't')
            {
                t++;
            }
            else if (key[counter] == 'U' || key[counter] == 'u')
            {
                u++;
            }
            else if (key[counter] == 'V' || key[counter] == 'v')
            {
                v++;
            }
            else if (key[counter] == 'W' || key[counter] == 'w')
            {
                w++;
            }
            else if (key[counter] == 'X' || key[counter] == 'x')
            {
                x++;
            }
            else if (key[counter] == 'Y' || key[counter] == 'y')
            {
                y++;
            }
            else if (key[counter] == 'Z' || key[counter] == 'z')
            {
                z++;
            }
        }
        
        //If any character has been used more than once, return error code.
        if (a >= 2 || b >= 2 || c >= 2 || d >= 2 || e >= 2 || f >= 2 || g >= 2 || h >= 2 || i >= 2 || j >= 2 || k >= 2 || l >= 2 ||
            m >= 2 || n >= 2 || o >= 2 || p >= 2 || q >= 2 || r >= 2 || s >= 2 || t >= 2 || u >= 2 || v >= 2 || w >= 2 || x >= 2 || 
            y >= 2 || z >= 2)
        {
            code = 4;
        }

    }
    return code;
}

//Function to apply cipher to the plaintext.
string cipher_code(string plain_text, string key)
{
    int position;
    for (int i = 0; plain_text[i] != '\0'; i++)
    {
        //Apply the cipher to capital letters.
        if (isupper(plain_text[i]))
        {
            position = plain_text[i] - 'A';
            plain_text[i] = toupper(key[position]);
        }
        
        //Apply the code to lowercase letters.
        if (islower(plain_text[i]))
        {
            position = plain_text[i] - 'a';
            plain_text[i] = tolower(key[position]);
        }

    }
    string cipher_text = plain_text;
    return cipher_text;
}

