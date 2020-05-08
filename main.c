#include <stdio.h>
#include "dataio.h"
#include "dataio.c"
int main() {
    MData* mdata = readData("/home/stach/CLionProjects/Louvain/data/mycielskian4/mycielskian4.mtx");
    printData(mdata);
    destroyMData(mdata);
    return 0;
}
