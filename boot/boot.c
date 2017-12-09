__asm__(".code16gcc\n");
__asm__("jmpl $0x0000, $main\n");

void printStr(const char* pStr) {
  while(*pStr){
    __asm__ __volatile__ (
          "int $0x10" : : "a"(0x0e00 | *pStr), "b"(0x0007)
      );
    ++pStr;
  }
}

void main(){
  printStr("Hello, World");
}
