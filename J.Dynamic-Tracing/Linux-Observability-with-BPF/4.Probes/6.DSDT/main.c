#include <sys/sdt.h>

int main() {

   DTRACE_PROBE("hello-usdt", "probe-main");

 }