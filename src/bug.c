void*
memchr(const void*, int, unsigned long);

int
main() {
    const char *a = "", *b;
    unsigned long i = 1;

    do {
        //if (a != 0) {
        //    void *c = memchr(a, 0, i);
        //}

        void *c = memchr(a, 0, i);

        b = a;
        a = 0;
        i = 0;
    } while (b);

    return 0;
}
