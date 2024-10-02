#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

double PI = M_PI;

// Function prototypes
double calculate(const char *expression, int *error);
double evaluate_expression(const char **expression_ptr, int *error);
double parse_term(const char **expression_ptr, int *error);
double parse_factor(const char **expression_ptr, int *error);
double parse_number(const char **expression_ptr, int *error);
void skip_whitespace(const char **expression_ptr);
double evaluate_operation(double a, double b, char operator, int *error);

// Main calculator function
double calculate(const char *expression, int *error) {
    *error = 0;
    const char *expression_ptr = expression;
    return evaluate_expression(&expression_ptr, error);
}

// Function to evaluate the expression based on BODMAS (Addition, Subtraction handled here)
double evaluate_expression(const char **expression_ptr, int *error) {
    double result = parse_term(expression_ptr, error);
    skip_whitespace(expression_ptr);
    while (**expression_ptr == '+' || **expression_ptr == '-') {
        char operator = **expression_ptr;
        (*expression_ptr)++;
        double next_term = parse_term(expression_ptr, error);
        result = evaluate_operation(result, next_term, operator, error);
    }

    return result;
}

// Function to parse terms (Multiplication, Division handled here)
double parse_term(const char **expression_ptr, int *error) {
    double result = parse_factor(expression_ptr, error);
    skip_whitespace(expression_ptr);
    while (**expression_ptr == '*' || **expression_ptr == '/') {
        char operator = **expression_ptr;
        (*expression_ptr)++;
        double next_factor = parse_factor(expression_ptr, error);
        result = evaluate_operation(result, next_factor, operator, error);
    }

    return result;
}

// Function to parse factors (Handles parentheses, sin, cos, and numbers)
double parse_factor(const char **expression_ptr, int *error) {
    skip_whitespace(expression_ptr);

    if (**expression_ptr == '(') {
        (*expression_ptr)++; // Skip '('
        double result = evaluate_expression(expression_ptr, error);
        if (**expression_ptr == ')') {
            (*expression_ptr)++; // Skip ')'
        } else {
            *error = 1;  // Mismatched parentheses
        }
        return result;
    }

    if (strncmp(*expression_ptr, "sin(", 4) == 0) {
        (*expression_ptr) += 4; // Skip "sin("
        double value = evaluate_expression(expression_ptr, error);
        if (**expression_ptr == ')') {
            (*expression_ptr)++;
            return sin(value * PI / 180.0); // Convert degrees to radians
        }
    } else if (strncmp(*expression_ptr, "cos(", 4) == 0) {
        (*expression_ptr) += 4; // Skip "cos("
        double value = evaluate_expression(expression_ptr, error);
        if (**expression_ptr == ')') {
            (*expression_ptr)++;
            return cos(value * PI / 180.0); // Convert degrees to radians
        }
    } else if (strncmp(*expression_ptr, "tan(", 4) == 0) {
        (*expression_ptr) += 4; // Skip "tan("
        double value = evaluate_expression(expression_ptr, error);
        if (**expression_ptr == ')') {
            (*expression_ptr)++;
            return tan(value * PI / 180.0); // Convert degrees to radians
        }
    }

    return parse_number(expression_ptr, error);
}

// Function to parse numbers
double parse_number(const char **expression_ptr, int *error) {
    skip_whitespace(expression_ptr);

    char *end_ptr;
    double value = strtod(*expression_ptr, &end_ptr);
    
    if (*expression_ptr == end_ptr) { // Invalid number
        *error = 1;
        return 0;
    }

    *expression_ptr = end_ptr;
    return value;
}

// Function to skip whitespace characters
void skip_whitespace(const char **expression_ptr) {
    while (isspace(**expression_ptr)) {
        (*expression_ptr)++;
    }
}

// Function to evaluate basic arithmetic operations
double evaluate_operation(double a, double b, char operator, int *error) {
    switch (operator) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/': 
            if (b == 0) {
                *error = 1;  // Division by zero
                return 0;
            }
            return a / b;
        default:
            *error = 1;  // Invalid operator
            return 0;
    }
}



int main() {
    char input[100];
    int error = 0;

    printf("Welcome to the enhanced miniLab. Enter your math problem: \n");

    while (1) {
        printf("> ");
        fgets(input, 100, stdin);

        // Exit on "exit" command
        if (strncmp(input, "exit", 4) == 0) {
            break;
        }

        double result = calculate(input, &error);
        

        if (error) {
            printf("Error: Invalid input or operation.\n");
        } else {
            printf("ans:\n");
            printf("%10g\n", result);
        }
    }

    printf("Goodbye!\n");
    return 0;
}
