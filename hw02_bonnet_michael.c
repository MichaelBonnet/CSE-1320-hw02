#include <stdio.h>
#include <assert.h>
#define PI (3.14159265358979323846)
#include <math.h>

// I'm quite proud of the fact that like ~30% of this file is comments.
// I still want to find a sexier/more concise way to implement init().

// The purpose of the following variable is to act as a single dimensional array storing the sin of each index as that index's value.
float lookup_table[360];


// The purpose of the following function is to return the factorial of the passed argument.
// The following function achieves this by incrementing i from 1 up to and including the int value
// of the passed argument, and then multiplying each of the incremented values of i together
// to get the factorial.
// This is done in the background and requires no user activity.
int fact(int factorial_quantity)
{
    int fact = 1;

    for (int i = 1; i <= factorial_quantity; i++)
    {
        fact *= i;
    }

    assert(fact != 0); // this assert tests if somehow the factorial has come to be equal to zero,
                       // which would cause a division by zero error in the taylorseries_ function.

    assert(fact > 0);  // this assert tests if somehow the factorial has come to be less than zero,
                       // which would severely effect the results of the taylorseries_ function.

    return fact;
}

// The purpose of the following function is to convert the passed argument, 
// a quantity of degrees, to radians.
// The following function achieves this by multiplying the degree quantity
// by the quantity (PI/180), as PI radians is equal to 180 degrees.
// This is done in the background to convert user input to a usable unit
// and requires no user activity.
float deg_to_rad(float input_angle)
{
    assert(PI == 3.14159265358979323846); // This assert checks to make certain I have the proper value of pi.
    float deg_to_rad = input_angle * (PI/180);

    return deg_to_rad;
}

// The purpose of the following function is to determine the sum of 
// a Taylor Series of the sine of the quantity of radians derived from
// user input in units of degrees to the 4th term (not including the first term x).
// The following function achieves this through summing the first 4 terms of the taylor series sequentially.
// This function happens in the background and requires no user interaction.
float taylorseries_(float input_angle)
{
    float  sine;

    sine = input_angle - pow(input_angle,3)/fact(3) + pow(input_angle,5)/fact(5)
             - pow(input_angle,7)/fact(7);

    return sine;
}


// The purpose of the following function is to populate the global variable
// lookup table with roughly accurate results of sine(x) for x >= 0 and x < 360.
// The following function achieves this through:
// (1) iterating through i for range 91 > i >= 0,
//     assigning the results to the corresponding indexes.
// (2) mirroring the results of (1) to the indexes 180 > i >= 91
//     assigning the results to the corresponding indexes.
// (3) iterating through the lookup table range 360 > i >= 181,
//     assigning each index with the value of the mirror index * (-1), as the bottom half of the unit circle is negative in sin.
// This function happens in the background and requires no user interaction.
void init(void)
{
    int mirror_value;

    for(size_t i = 0; i < 91; i++)
    {
        lookup_table[i] = taylorseries_(deg_to_rad(i));
    }

    for(size_t i = 91; i < 181; i++)
    {
        mirror_value = 180 - i;
        lookup_table[i] = lookup_table[mirror_value];
    }

    for(size_t i = 181; i < 360; i++)
    {
        mirror_value = i - 180;
        lookup_table[i] = (-1) * lookup_table[mirror_value];
    }

    // The following four asserts simply check that halfway through each quadrant,
    // that the values in that quadrant have the correct sign.
    // This makes sure that the lookup table is at least somewhat usable,
    // and won't church out wholly unrecognizable results.
    assert(lookup_table[45]  > 0);
    assert(lookup_table[135] > 0);
    assert(lookup_table[225] < 0);
    assert(lookup_table[315] < 0);
}

// The purpose of the following function is to linearly interpolate between two (x, y) ordered pairs.
// The following function achieves this through implementing the basic linear interpoplation equation.
// What is essentially happening here is taking the two ordered pairs as the index and index value
// whose indexes are the upper and lower bounds of the range in which the true x value could be.
// The function then executes a slightly moved around version of the common linear interpolation equation,
// solving for y, which should be roughly in the neighborhood of the correct sin of the true x value.
// This function happens in the background and requires no user interaction.
float interpolation(float x0, float y0, float x1, float y1, float x)
{
    float interpolation = y0 + (x - x0) * ( (y1 - y0)/(x1 - x0) );

    return interpolation;
}

// The purpose of the following function is to determine the proper x-values, and therefore y-values,
// of the two ordered pairs (x0, y0) and (x1, y1) to be used in linear interpolation in order to find
// a more exact value for the sine of a noninteger degree quantity between 0 and 359 degrees.
// The following function achieves this through assigning a float (the quantity) to an int, truncating it
// and therefore rounding it to the proper x0 value. The proper x(1) value is determined by adding 1 to x0;
// the other necessary terms are derived from these two quantities and the input angle.
// This function also performs input validation by automaticaly returning a 0.0 for inputs outside the intended domain.
// This function happens in the background and requires no user interaction.
float sin_(float input_angle)
{
    float result;
    int lowest_x_value = input_angle;
    int highest_x_value = lowest_x_value + 1;

    if (input_angle < 0.0)
    {
        return 0.0;
    }
        else if (input_angle >= 360.0)
        {
            return 0.0;
        }
            else if (lowest_x_value == input_angle)
            {
                return lookup_table[lowest_x_value];
            }
                else
                {
                    result = interpolation(lowest_x_value, lookup_table[lowest_x_value], highest_x_value, lookup_table[highest_x_value], input_angle);

                    return result;
                }
}

// The purpose of the following function is to find the error, or difference,
// between the "true" sine value and the sine value calculated through interpolation and Taylor Series.
// The following function achieves this by subtracting the result of sin_, the interpolation value,
// from the "true" sine value.
// This function happens in the background and requires no user interaction.
float error_(float input_angle)
{   
    float error_ = sin(deg_to_rad(input_angle)) - sin_(input_angle);

    return error_;
}


// The purpose of the following function is to execute all functions and logic to produce the desired outcome.
// This is generally the purpose of functions called 'main'.
// This main function works by calling init() to populate an array of sine values,
// then prompting the user for a degree quantity to return values for.
// by doing the prompting within a do-while loop, it is always executed at least once, and keeps going
// until the user exits the program via typing -1 when prompted for input.
int main (void)
{
    init();

    float input_angle;

    do
    {
        scanf("%f", &input_angle);

        if (input_angle == -1)
        {
            return 0;
        }
            else
            {
                printf("%f %f %f %f\n", input_angle, sin_(input_angle), sin(deg_to_rad(input_angle)), error_(input_angle));
            }

    } while (input_angle != -1);

    return 0;
}
