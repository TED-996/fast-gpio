#include <module.h>

// Register access
int Module::_SetupAddress(unsigned long int blockBaseAddr,
                          unsigned long int blockSize) {
  int m_mfd;
  int page_size;
  if ((m_mfd = open("/dev/mem", O_RDWR)) < 0) {
    return EXIT_FAILURE; // maybe return -1
  }
  regAddress =
      (unsigned long int *)mmap(NULL, 1024, PROT_READ | PROT_WRITE,
                                MAP_FILE | MAP_SHARED, m_mfd, blockBaseAddr);
  close(m_mfd);
  if (regAddress == MAP_FAILED) {
    return EXIT_FAILURE; // maybe return -2
  }

  return EXIT_SUCCESS; // regAddress is now populated
}

void Module::_WriteReg(unsigned long int registerOffset,
                       unsigned long int value) {
    *(regAddress + registerOffset) = value;
}

unsigned long int Module::_ReadReg(unsigned long int registerOffset) {
  unsigned long int value = 0x0;
  // read the value
  value = *(regAddress + registerOffset);

  return (value);
}

// change the value of a single bit
void Module::_SetBit(unsigned long int &regVal, int bitNum, int value) {
  if (value == 1) {
    regVal |= (1 << bitNum);
  } else {
    regVal &= ~(1 << bitNum);
  }

  // try this out
  // regVal ^= (-value ^ regVal) & (1 << bitNum);
}

// find the value of a single bit
int Module::_GetBit(unsigned long int regVal, int bitNum) {
  int value;

  // isolate the specific bit
  value = ((regVal >> bitNum) & 0x1);

  return (value);
}
