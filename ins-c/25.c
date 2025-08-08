void F25(int val, int times, int factor) {
    int i = 0;
    int result = val;
    do {
        result = result * factor;
        i = i + 1;
    } while (i < times);
}
