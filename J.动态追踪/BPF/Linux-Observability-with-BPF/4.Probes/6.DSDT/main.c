#include <sys/sdt.h>
#include <sys/time.h>

int main() {
  DTRACE_PROBE(hello-usdt, probe-main);
 }