#include<iostream>
#include<stdio.h>
#include<conio.h>
#include<string>
#include<functional>
#include<cmath>
#include<cstdlib>
#include<cctype>
#include<stdexcept>
using namespace std;

char G[50][50];
string equation;

//calculates the user-entered equation
double calculate(double x)
{
    size_t pos = 0;

    function<double()> expression, term, unary, power, value;

    auto skip_spaces = [&]()
        {
            while (pos < equation.size() &&
                isspace(static_cast<unsigned char>(equation[pos])))
            {
                pos++;
            }
        };

    auto take = [&](char symbol)
        {
            skip_spaces();

            if (pos < equation.size() && equation[pos] == symbol)
            {
                pos++;
                return true;
            }

            return false;
        };

    value = [&]() -> double
        {
            if (take('('))
            {
                double result = expression();

                if (!take(')'))
                    throw runtime_error("Missing closing bracket");

                return result;
            }

            if (take('x'))
                return x;

            skip_spaces();

            const char* start = equation.c_str() + pos;
            char* end;
            double number = strtod(start, &end);

            if (start == end)
                throw runtime_error("Invalid equation");

            pos += end - start;
            return number;
        };

    power = [&]() -> double
        {
            double result = value();

            if (take('^'))
                result = pow(result, unary());

            return result;
        };

    unary = [&]() -> double
        {
            if (take('+'))
                return unary();

            if (take('-'))
                return -unary();

            return power();
        };

    term = [&]() -> double
        {
            double result = unary();

            while (true)
            {
                if (take('*'))
                    result *= unary();
                else if (take('/'))
                {
                    double divisor = unary();

                    if (divisor == 0)
                        throw runtime_error("Division by zero");

                    result /= divisor;
                }
                else
                    return result;
            }
        };

    expression = [&]() -> double
        {
            double result = term();

            while (true)
            {
                if (take('+'))
                    result += term();
                else if (take('-'))
                    result -= term();
                else
                    return result;
            }
        };

    double result = expression();
    skip_spaces();

    if (pos != equation.size())
        throw runtime_error("Unexpected character in equation");

    if (!isfinite(result))
        throw runtime_error("Equation has no finite result");

    return result;
}

//base graph initiator functions
void initiate()
{
    int i, j;
    for (i = 0; i < 50; i++)
    {
        for (j = 0; j < 50; j++)
        {
            G[j][i] = ' ';
        }
    }
}

//assigns x-axis
void assign_Xaxis()
{
    int i;
    for (i = 0; i < 50; i++)
    {
        G[i][24] = '-';
    }
}

//assigns y-axis
void assign_Yaxis()
{
    int i;
    for (i = 0; i < 50; i++)
    {
        G[24][i] = '|';
    }
}

//assigns both axes to the base 2D arrays
void assign_axes()
{
    assign_Xaxis();
    assign_Yaxis();
}
//end of base graph initator functions

//plotting functions:
//finds the max y-value
int max(int x_max, int x_mag)
{
    int y;
    int y1 = 0;
    int i;
    for (i = 0; i <= x_max; i = i + x_mag)
    {
        y = calculate(i);//user input function
        if (y > y1)
        {
            y1 = y;
        }
    }
    return y1;
}

//plots the graph in the 2d array
void plot()
{
    cout << "Enter equation in x (example: x^2 + 2*x + 1): ";
    getline(cin >> ws, equation);

    int x, x1, y, y1;
    int x_mag, y_mag;
    int x_max, x_min;
    int y_max;

    cout << "enter x_max: ";
    cin >> x_max;
    cout << "enter x_min: ";
    cin >> x_min;

    x_mag = x_max / 25;
    y_max = max(x_max, x_mag);
    cout << y_max;
    y_mag = y_max / 25;

    //positive x_axis
    for (x1 = 0; x1 <= x_max; x1 = x1 + x_mag)
    {
        y1 = calculate(x1);//user input function
        y = y1 / y_mag;
        x = x1 / x_mag;
        G[x + 24][24 - y] = '*';
    }

    for (x1 = 0; x1 > x_min; x1 = x1 - x_mag)
    {
        y1 = calculate(x1);//user input function
        y = -(y1 / y_mag);
        x = x1 / x_mag;
        G[x + 24][24 + y] = '*';
    }
}
//end of plotting functions

//displays the base graph
void display()
{
    int x;
    int y;
    for (y = 0; y < 50; y++)
    {
        for (x = 0; x < 50; x++)
        {
            cout << G[x][y];
        }
        cout << endl;
    }
}

void main()
{
    initiate();
    assign_axes();
    plot();
    display();
}