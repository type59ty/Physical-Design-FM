#include <bits/stdc++.h>
#include "fm.h"
#include "read_file.h"
#include "time.h"
using namespace std;

int main(int argc, char **argv) {
    clock_t start, end;
    start = clock();

    Cells *cells = new Cells();
    Nets *nets = new Nets;
    NetDis *netDis = new NetDis;

    string node_file  = argv[1];
    string net_file   = argv[2];
    string out_file   = argv[3];

    cout << "Reading files...\n";
    ReadFile *readFile = new ReadFile(cells, nets);
    readFile->read_file(node_file, net_file);

    FM *fm = new FM(netDis, cells, nets);
    fm->Partition();

    end = clock();
    double cpu_time = ((double) end - (double) start) / CLOCKS_PER_SEC;

    fm->Output(cpu_time, out_file);

    return 0;
}
