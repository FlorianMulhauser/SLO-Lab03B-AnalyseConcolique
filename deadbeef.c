#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <klee/klee.h>

void test(char *buf) {
	if (buf[0]==´d´) {
		if (buf[1]==´e´) {
			if (buf[2]==´a´) {
				if (buf[3]==´d´) {
					if (buf[4]==´b´) {
						if (buf[5]==´e´ ){
							if (buf[6]==´e´) {
								if (buf[7]==´f´) {
									assert(0);
								} else printf("%c", buf[7]);
							} else printf("%c", buf[6]);
						}else printf("%c", buf[5]);
					}else printf("%c", buf[4]);
				}else printf("%c", buf[3]);
			}else printf("%c", buf[2]);
		}else printf("%c", buf[1]);
	} else printf("%c", buf[0]);
	return 0;
}
int main(int argc, char** argv){
	
	test(argv[1]);
	return 0;
}
