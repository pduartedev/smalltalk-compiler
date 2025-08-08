int F50(int n, int a, int b) {
    int sum = 0;                                /* 1. Variable declaration */
    int temp = (a * b) - (a + b) / 2;

    if ((n % 2 == 0) && (a + b > 0)) {          /* 2. If statement */
        sum = (n * n) + (a * 2) - (b / 2);
    }

    if ((a * a + b * b) % 3 == 0) {             /* 3. If-else statement */
        sum = sum + (a + b) * (a - b);
    } else {
        sum = sum - ((a + b) * (a + b)) / (n + 1);
    }

    while (n > 1) {                             /* 4. While loop */
        sum = sum + (n * temp) - (n % 3) + (a + b);
        n = n - ((n % 2) + 1);
    }

    for (n = 0; n < 4; n = n + 1) {             /* 5. For loop */
        sum = sum + (n * n) - (a - b) * (n + 1);
    }

    if (sum > 100) {
        /* 6. Function call (7. Recursion) embedded in expression */
        int r = ((sum / 3) - F50(n + 1, a - 1, b + 2)) * 2 + (a % 3);
        sum = sum + r;
    }

    return sum + ((a * b) % (n + 2)) - temp;
}
