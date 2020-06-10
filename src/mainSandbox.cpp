#include "Sandbox.h"

int main() {

  Sandbox sandbox;
  if (sandbox.getStatus()) {
    sandbox.run();
  }

  return 0;
}
