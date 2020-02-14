#include <bits/stdc++.h>
#include "read_file.h"
#include "ds.h"
using namespace std;

void ReadFile::read_file(string node_file, string net_file){
    ifstream IFS_node;
    ifstream IFS_net;
    IFS_node.open(node_file);
    IFS_net.open(net_file);
    string line = "";
    regex pat_net("NetDegree");
    regex pat_node("[a-z][0-9]+");
    regex pat_ter("terminal");
    regex pat_terni("terminal_NI");
    regex pat_nnodes("NumNodes");
    regex pat_nters("NumTerminals");
    regex pat_nnets("NumNets");
    regex pat_npins("NumPins");
    string dummy;
    string node_name;
    string net_name;
    int h,w;
    int num_pins;

    // read node
    while(getline(IFS_node,line)) {
        dummy.clear();
        istringstream token(line);
        if ((regex_search(line, pat_node)) and !(regex_search(line, pat_ter) or regex_search(line, pat_terni))) {
            Cell *cell = new Cell;//new cell
            token >> node_name >> h >> w;
            cell->node_name = node_name;
            cell->area = h*w;
            cells->Total_Size += cell->area;
            if (cell->area > cells->Max_Size) {
                cells->Max_Size = cell->area;
            }
            cells->CellArray.insert(pair<string, Cell* >(cell->node_name, cell ) ) ;
        } else if (regex_search(line, pat_nnodes)) {
            token >> dummy >> dummy >> cells->NumNodes;  
        } else if (regex_search(line, pat_nters)) {
            token >> dummy >> dummy >> cells->NumTerminals; 
        }
    }
    cells->bound = 0.4 * cells->Total_Size;

    // read net
    while(getline(IFS_net,line)) {
        istringstream token(line);
        if (regex_search(line, pat_net)) {
            token >> dummy >> dummy >> num_pins >> net_name;
            Net *net = new Net;
            net->net_degree = num_pins;
            this->net_name = net_name;
            net->net_name = this->net_name;
            nets->NetArray[net->net_name] = net;
        } else if (regex_search(line, pat_node)) {
            token >> node_name;
            auto find_cell = cells->CellArray.find(node_name);
            if (find_cell != cells->CellArray.end()) {
                Cell *cell = cells->CellArray[node_name];//cell name
                cell->NetList.push_back(nets->NetArray[this->net_name]);
                nets->NetArray[this->net_name]->CellList.push_back(cell); 
                /*
                vector<string> current_node;
                for (auto no : nets->NetArray[this->net_name]->CellList) {
                    current_node.push_back(no->node_name);
                }
                auto Nit = find(current_node.begin(), current_node.end(), node_name);
                if (Nit == current_node.end()) {
                    nets->NetArray[this->net_name]->CellList.push_back(cell); 
                }
                */
            } else { // this node is a terminal
                nets->NumPins--;
                nets->NetArray[this->net_name]->net_degree--;
                if (nets->NetArray[this->net_name]->net_degree == 0) {
                    nets->NetArray.erase(this->net_name);
                    nets->NumNets--;
                }
            }
        } else if (regex_search(line, pat_nnets)) {
            token >> dummy >> dummy >> (nets->NumNets);
        } else if (regex_search(line, pat_npins)) {
            token >> dummy >> dummy >> (nets->NumPins);
        }
    }
	for (map<string, Cell* >::iterator cell_it = this->cells->CellArray.begin() ; cell_it != this->cells->CellArray.end() ; ++cell_it) {
    	if(cell_it->second->NetList.size() > this->cells->Pmax) {
    		this->cells->Pmax = cell_it->second->NetList.size();
        }
	}

    IFS_node.close();
    IFS_net.close();
}
