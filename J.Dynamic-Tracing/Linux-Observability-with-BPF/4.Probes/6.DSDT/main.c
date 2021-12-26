#include <sys/sdt.h>
#include <sys/time.h>

int main() {
  struct timeval tv;
  DTRACE_PROBE1(hello-usdt, probe-main, tv.tv_sec);

 }