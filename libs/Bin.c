#include <stdio.h>
#include <stdlib.h>

unsigned long binsize(unsigned long nb)
{
    /* return the size in bits of a binary number */

    if (nb)
        return 1 + binsize(nb >> 1);
    else
        return 0;
}

unsigned long powtwo(unsigned long nb)
{
    /* remake of power of two */

    if (nb == 0)
        return 1;
    return powtwo(nb - 1) << 1;
}

void tobin(unsigned long nb)
{
    /* print the binarie representation of a number (mostly for debuging) */

    const unsigned long nbsize = binsize(nb) ? binsize(nb) - 1 : 0;

    unsigned long cur = powtwo(nbsize);

    while (cur > 0)
    {
        if (nb / cur > 0)
        {
            putchar('1');
            nb -= cur;
        }
        else
        {
            putchar('0');
        }
        cur /= 2;
    }
    printf("\n");
}

void bintostr(unsigned long nb, unsigned char size, char* str)
{
    /* put in a string nb's bin representation */

    unsigned long cur = powtwo(size);
    int index = 0;
    
    if(str){
        while (cur > 0)
        {
            if (nb / cur > 0)
            {
                str[index] = '1';
                nb -= cur;
            }
            else
            {
                str[index] = '0';
            }
            cur /= 2;
            index++;
        }
        str[index] = '\0';
    }
}
