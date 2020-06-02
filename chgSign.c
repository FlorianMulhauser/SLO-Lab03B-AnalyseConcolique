#include <assert.h>
#include <stdlib.h>
#include <stdio.h>


int chg_sign(int x, int y) {
        if (x > y){
                x = x + y;
                y = x - y;
                x = x -y; 
                if (x - y > 0)
                        assert(0);
                return x-y;
        }
        return x-y;
} 
