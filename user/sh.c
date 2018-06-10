int print_str(const char *str)
{
    int a;
    asm volatile("int $0x80" : "=a" (a) : "0" (0), "b"(str));
    return a;
}

int main(void)
{
    print_str(">");
    return 0;
}
