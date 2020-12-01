#include <iostream>
#include "Server.h"

int main(){
    Server s(13);
    // finished when 2 clients join
    s.listen();
    // main process
    s.run();
}
