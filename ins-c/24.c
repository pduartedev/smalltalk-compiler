void F24(int from, int to, int step) {
    int i;
    int product = 1;
    for (i = from; i < to; i = i + step) {
        product = product * i;
    }
}
