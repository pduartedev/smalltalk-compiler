
int F50(int n, int a, int b) {
    int sum = 0;                               /* Level 0 */
    int temp = (a * b) - (a + b) / 2;         /* Level 0 */

    if ((n % 2 == 0) && (a + b > 0)) {        /* Level 1 */
        int x = (n * n) + (a * 2);             /* Level 2 */
        {
            int y = x - (b / 2);                /* Level 3 */
            sum = y + helper(a, b, n);          /* Function call embedded in expression */
        }
    }

    if ((a * a + b * b) % 3 == 0) {            /* Level 1 */
        int y = (a + b) * (a - b);             /* Level 2 */
        {
            sum = sum + y;                      /* Level 3 */
        }
    } else {
        int d = ((a + b) * (a + b)) / (n + 1); /* Level 2 */
        {
            sum = sum - d;                      /* Level 3 */
        }
    }

    while (n > 1) {                            /* Level 1 */
        int step = ((n % 3) + 1);              /* Level 2 */
        {
            sum = sum + (n * temp) - step + (a + b); /* Level 3 */
            n = n - ((n % 2) + 1);
        }
    }

    for (n = 0; n < 4; n = n + 1) {            /* Level 1 */
        int v = (n * n) - (a - b) * (n + 1);   /* Level 2 */
        {
            sum = sum + v;                      /* Level 3 */
        }
    }

    if (sum > 100) {                           /* Level 1 */
        {
            int r = ((sum / 3) - F50(n + 1, a - 1, b + 2)) * 2 + (a % 3); /* Recursive call *inside* expression */
            sum = sum + r;
        }
    }

    int tail = ((a * b) % (n + 2)) - temp;     /* Level 0 */
    sum = sum + tail;

    return sum;
}
