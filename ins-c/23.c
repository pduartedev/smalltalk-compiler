void F23(int start, int limit, int step) {
    int i = start;
    int total = 0;
    while (i < limit) {
        total = total + i;
        i = i + step;
    }
}
