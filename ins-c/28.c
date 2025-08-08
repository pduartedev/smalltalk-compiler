int F28(int n, int a, int b) {
    int sum = 0;                    /* 1. Variable declaration */

    if (n <= 0) {                   /* 2. If statement */
        return 0;
    }

    if (a > b) {                    /* 3. If-else statement */
        sum = a - b;
    } else {
        sum = b - a;
    }

    while (n > 0) {                 /* 4. While loop */
        sum = sum + n;
        n = n - 1;
    }

    for (n = 0; n < 3; n = n + 1) { /* 5. For loop */
        sum = sum + n;
    }

    if (sum > 10) {                 /* 6. Function call + 7. Recursion */
        return F50(sum / 3, a, b);  /* recursive call */
    }

    return sum;
}
