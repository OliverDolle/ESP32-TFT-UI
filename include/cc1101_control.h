#ifndef CC1101_CONTROL_H
#define CC1101_CONTROL_H

void initCC1101();
void cc1101Menu();
void sendCC1101Packet(const char* data, int length);

#endif
