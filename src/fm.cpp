#include "fm.h"

void FM::Output(double cpu_time, string out_file) {
    ofstream OFS_out;
    OFS_out.open(out_file);
    OFS_out << "Cut_size: " << netDis->cut_size << endl;
    OFS_out << "G1_size: " << netDis->G1_size << endl;
    OFS_out << "G2_size: " << netDis->G2_size << endl;
    OFS_out << "Time: " << cpu_time << " s" << endl;
    OFS_out << "G1: " << endl;
    for (map<string, Cell *>::iterator cell_it = netDis->Group1.begin(); cell_it != netDis->Group1.end(); ++cell_it)
        OFS_out << (cell_it->second->node_name) << " ";
    OFS_out << ";" << endl;
    OFS_out << "G2: " << endl;
    for (map<string, Cell *>::iterator cell_it = netDis->Group2.begin(); cell_it != netDis->Group2.end(); ++cell_it)
        OFS_out << (cell_it->second->node_name) << " ";
    OFS_out << ";" << endl;
    OFS_out << "Cut_set: " << endl;
    for (vector<Net *>::iterator cut_it = netDis->CutNetList.begin(); cut_it != netDis->CutNetList.end(); ++cut_it)
        OFS_out << ((*cut_it)->net_name) << " ";
    OFS_out << ";" << endl;
    cout << "Finish." << endl;
    cout << endl;
    cout << endl;

    OFS_out.close();
}

bool cmp(BP *a, BP *b) {
    return a->max_cell_gain < b->max_cell_gain;
}
bool rsort(Position *a, Position *b) {
    return a->Gk > b->Gk;
}

void FM::MvCell(Cell *base_cell, list<BP *>::reverse_iterator it) {
    GkList.push_back(base_cell->gain);
    UpdateBaseCell(base_cell, it);
    UpdateGain(base_cell);
    tmp_CellList.push_back(base_cell);
    ++num_lock;
}

void FM::UpdateBaseCell(Cell *base_cell, list<BP *>::reverse_iterator it) {
    if (base_cell->group == "A") {
        BuckList1[(*it)->max_cell_gain].erase(
            base_cell->node_name);  // erase from BuckList1
        if (BuckList1[(*it)->max_cell_gain].empty()) {
            Bucket_Pointer.erase(--(it.base()));  // delete max pointer
        }
        G1_size -= base_cell->area;
        G2_size += base_cell->area;
    } else {
        BuckList2[(*it)->max_cell_gain].erase(
            base_cell->node_name);  // erase from BuckList2
        if (BuckList2[(*it)->max_cell_gain].empty()) {
            Bucket_Pointer.erase(--(it.base()));  // delete max pointer
        }
        G2_size -= base_cell->area;
        G1_size += base_cell->area;
    }
    base_cell->gain = 0;
    base_cell->locked = true;  // lock base_cell
}

void FM::UpdateGain(Cell *base_cell) {
    if (base_cell->group == "A") {
        for (vector<Net *>::iterator net_it = base_cell->NetList.begin(); net_it != base_cell->NetList.end(); ++net_it)
            UpdateNet(*net_it, "A", "B", (*net_it)->nG1, (*net_it)->nG2, BuckList1, BuckList2, Bucket_Pointer);
    } else {
        for (vector<Net *>::iterator net_it = base_cell->NetList.begin(); net_it != base_cell->NetList.end(); ++net_it)
            UpdateNet(*net_it, "B", "A", (*net_it)->nG2, (*net_it)->nG1, BuckList2, BuckList1, Bucket_Pointer);
    }
}

void FM::Compute_Init_Gains() {
	for (map<string, Cell* >::iterator cell_it = cells->CellArray.begin() ; cell_it != cells->CellArray.end() ; ++cell_it) {
		cell_it->second->gain = 0;
        cell_it->second->locked = false;
		if (cell_it->second->group == "A") {
			// iterate each net of this cell
			// From A To B
			for (vector<int>::size_type n_idx = 0 ; n_idx < cell_it->second->NetList.size() ; ++n_idx) {
				// this net has only one cell in Group A  (move from A to B will decrease cutsize)
				if (cell_it->second->NetList.at(n_idx)->nG1 == 1) cell_it->second->gain++;
				// this net has no cell in group B        (move from A to B will increase cutsize)
				if (cell_it->second->NetList.at(n_idx)->nG2 == 0) cell_it->second->gain--;
			}
            if (BuckList1[(cell_it->second->gain) + cells->Pmax].empty()) {
                BP *bp = new BP(cell_it->second->group, (cell_it->second->gain) + cells->Pmax);
                Bucket_Pointer.push_back(bp);                
            }
            BuckList1[(cell_it->second->gain) + cells->Pmax].insert(pair<string, Cell*>(cell_it->second->node_name, cell_it->second));
		} else {
			// iterate each net of this cell
			// From B To A
			for (vector<int>::size_type n_idx = 0 ; n_idx < cell_it->second->NetList.size() ; ++n_idx) {
				// this net has only one cell in Group A  (move from A to B will decrease cutsize)
				if (cell_it->second->NetList.at(n_idx)->nG2 == 1) cell_it->second->gain++;
				// this net has no cell in group B        (move from A to B will increase cutsize)
				if (cell_it->second->NetList.at(n_idx)->nG1 == 0) cell_it->second->gain--;
			}
            if (BuckList2[(cell_it->second->gain) + cells->Pmax].empty()) {
                BP *bp = new BP(cell_it->second->group, (cell_it->second->gain) + cells->Pmax);
                Bucket_Pointer.push_back(bp);                
            }
            BuckList2[(cell_it->second->gain) + cells->Pmax].insert(pair<string, Cell*>(cell_it->second->node_name, cell_it->second));
        }
	}	
    Bucket_Pointer.sort(cmp);
}

void FM::UpdateNet(Net *net, string gF, string gT, unsigned int &From, unsigned int &To, vector< map<string, Cell *> > &GroupF, vector< map<string, Cell *> > &GroupT, list<BP *> &Ptr) {
    if (To == 0) {
        for (vector<Cell *>::iterator cell_it = net->CellList.begin(); cell_it != net->CellList.end(); ++cell_it) {
            if (!(*cell_it)->locked) {
                GroupF[(*cell_it)->gain + cells->Pmax].erase( (*cell_it)->node_name);
                if (GroupF[(*cell_it)->gain + cells->Pmax].empty()) {
                    for (list<BP *>::iterator b_it = Ptr.begin(); b_it != Ptr.end(); ++b_it) { 
                        if (((*b_it)->max_cell_gain == (*cell_it)->gain + int(cells->Pmax)) and ((*cell_it)->group == (*b_it)->group)) {
                            Ptr.erase(b_it);  
                            break;
                        }
                    }
                }
                ++(*cell_it)->gain;
                if (GroupF[(*cell_it)->gain + cells->Pmax].empty()) {
                    BP *bp = new BP((*cell_it)->group, (*cell_it)->gain + cells->Pmax);
                    Ptr.push_back(bp); 
                }
                GroupF[(*cell_it)->gain + cells->Pmax].insert( pair<string, Cell *>((*cell_it)->node_name, (*cell_it)));
            }
        }
    } else if (To == 1) {
        for (vector<Cell *>::iterator cell_it = net->CellList.begin(); cell_it != net->CellList.end(); ++cell_it) {
            if (!(*cell_it)->locked and (*cell_it)->group == gT) {
                GroupT[(*cell_it)->gain + cells->Pmax].erase (
                    (*cell_it)->node_name);
                if (GroupT[(*cell_it)->gain + cells->Pmax].empty()) {
                    for (list<BP *>::iterator b_it = Ptr.begin(); b_it != Ptr.end(); ++b_it) {
                        if (((*b_it)->max_cell_gain == (*cell_it)->gain + int(cells->Pmax)) and ((*cell_it)->group == (*b_it)->group)) {
                            Ptr.erase(b_it);  
                            break;
                        }
                    }
                }
                --(*cell_it)->gain;
                if (GroupT[(*cell_it)->gain + cells->Pmax].empty()) {
                    BP *bp = new BP((*cell_it)->group, (*cell_it)->gain + cells->Pmax);
                    Ptr.push_back(bp);  
                }
                GroupT[(*cell_it)->gain + cells->Pmax].insert(pair<string, Cell *>((*cell_it)->node_name,(*cell_it)));
            }
        }
    }
    --From;
    ++To;
    if (From == 0) {
        for (vector<Cell *>::iterator cell_it = net->CellList.begin(); cell_it != net->CellList.end(); ++cell_it) {
            if (!(*cell_it)->locked) {
                GroupT[(*cell_it)->gain + cells->Pmax].erase( (*cell_it)->node_name);
                if (GroupT[(*cell_it)->gain + cells->Pmax].empty()) {
                    for (list<BP *>::iterator b_it = Ptr.begin(); b_it != Ptr.end(); ++b_it) {
                        if (((*b_it)->max_cell_gain == (*cell_it)->gain + int(cells->Pmax)) and ((*cell_it)->group == (*b_it)->group)) {
                            Ptr.erase(b_it); 
                            break;
                        }
                    }
                }
                --(*cell_it)->gain;
                if (GroupT[(*cell_it)->gain + cells->Pmax].empty()) {
                    BP *bp = new BP((*cell_it)->group, (*cell_it)->gain + cells->Pmax);
                    Ptr.push_back(bp);  // delete max pointer
                }
                GroupT[(*cell_it)->gain + cells->Pmax].insert(pair<string, Cell *>((*cell_it)->node_name,(*cell_it)));
            }
        }
    } else if (From == 1) {
        for (vector<Cell *>::iterator cell_it = net->CellList.begin();
             cell_it != net->CellList.end(); ++cell_it) {
            if (!(*cell_it)->locked and (*cell_it)->group == gF) {
                GroupF[(*cell_it)->gain + cells->Pmax].erase((*cell_it)->node_name);
                if (GroupF[(*cell_it)->gain + cells->Pmax].empty()) {
                    for (list<BP *>::iterator b_it = Ptr.begin(); b_it != Ptr.end(); ++b_it) {
                        if (((*b_it)->max_cell_gain == (*cell_it)->gain + int(cells->Pmax)) and ((*cell_it)->group == (*b_it)->group)) {
                            Ptr.erase(b_it);  
                            break;
                        }
                    }
                }
                ++(*cell_it)->gain;
                if (GroupF[(*cell_it)->gain + cells->Pmax].empty()) {
                    BP *bp = new BP((*cell_it)->group, (*cell_it)->gain + cells->Pmax);
                    Ptr.push_back(bp);  
                }
                GroupF[(*cell_it)->gain + cells->Pmax].insert( pair<string, Cell *>((*cell_it)->node_name, (*cell_it)));
            }
        }
    }
}


void FM::Partition() {
    for (map<string, Cell* >::iterator cell_it = cells->CellArray.begin() ; cell_it != cells->CellArray.end() ; ++cell_it) {
        if (cells->bound > netDis->G1_size) {
            netDis->G1_size += cell_it->second->area;
            cell_it->second->group = "A";
            //netDis->Group1[cell_it->second->node_name] = cell_it->second; 
            netDis->Group1.insert(pair<string, Cell* >(cell_it->second->node_name, cell_it->second)); 
        } else {
            netDis->G2_size += cell_it->second->area;
            cell_it->second->group = "B";
            //netDis->Group2[cell_it->second->node_name] = cell_it->second; 
            netDis->Group2.insert(pair<string, Cell* >(cell_it->second->node_name, cell_it->second));
        }
    }
    cout << "Initial Status" << endl;
    cout << "Number of pins: " << nets->NumPins << endl;
    cout << "Group1: " << netDis->G1_size << endl;
    cout << "Group2: " << netDis->G2_size << endl;

	// iterate all nets
    for (map<string, Net* >::iterator net_it = nets->NetArray.begin() ; net_it != nets->NetArray.end() ; ++net_it) {
		// number of cells in current net
		for (vector<int>::size_type c_idx = 0; c_idx < net_it->second->CellList.size(); ++c_idx){
			// check each cell in this net is in group1 or group2
			map<string, Cell* >::iterator find_cell = netDis->Group1.find(net_it->second->CellList.at(c_idx)->node_name);
			// find it in group1 => value A of this net ++
			if (find_cell != netDis->Group1.end()) {
        		net_it->second->nG1++;
			// find it in group2 => value B of this net ++
      		} else if(find_cell == netDis->Group1.end()) {
        		net_it->second->nG2++;
      		}
		}
		// Both value A and B > 0 => critical net
		if ((net_it->second->nG1) > 0 and (net_it->second->nG2) > 0) {
			netDis->cut_size++;
		}
	}
	cout << "Cut size: " << netDis->cut_size << endl;

    int Gk;
    lower_bound = cells->bound - cells->Max_Size;
    upper_bound = cells->bound + cells->Max_Size;
    bool Gk_flag = true;
    unsigned int iters = 0;
    //do {
    while (Gk_flag) {
        ++iters;
        Gk = 0;
        this->G1_size = netDis->G1_size;
        this->G2_size = netDis->G2_size;
        num_lock = 0;
        tmp_CellList.clear();
        GkList.clear();
        Gk_List.clear();
        pos_List.clear();
        Compute_Init_Gains();
        netDis->cut_size = 0;
        bool mode_b = false;
        bool flag = false;
        Gk_flag = false;
        while (num_lock < cells->CellArray.size()) {
            flag = false;
            netDis->CutNetList.clear();
            list<BP *>::reverse_iterator it;

            for (it = Bucket_Pointer.rbegin(); it != Bucket_Pointer.rend(); ++it) {
                map<string, Cell *>::iterator cell_it;
                if ((*it)->group == "A") {
                    for (cell_it = BuckList1[(*it)->max_cell_gain].begin(); cell_it != BuckList1[(*it)->max_cell_gain].end(); ++cell_it) {
                        if ((this->G1_size - cell_it->second->area) >= lower_bound) {
                            MvCell(cell_it->second, it);
                            flag = true;
                            break;
                        }
                    }
                } else {  // B
                    for (cell_it = BuckList2[(*it)->max_cell_gain].begin(); cell_it != BuckList2[(*it)->max_cell_gain].end(); ++cell_it) {
                        if ((this->G1_size + cell_it->second->area) <= upper_bound) {
                            MvCell(cell_it->second, it);
                            flag = true;
                            break;
                        }
                        if (mode_b) {
                            MvCell(cell_it->second, it);
                            flag = true;
                            break;
                        }
                    }
                }
                if (flag)
                    break;
            }
            if (it == Bucket_Pointer.rend() and !Bucket_Pointer.empty()) {  // check B size mode
                mode_b = true;
            }
            Bucket_Pointer.sort(cmp);
        }  

        for (vector<int>::size_type gk_idx = 0; gk_idx < GkList.size(); ++gk_idx) {
            Gk += GkList[gk_idx];
            Position *pos = new Position(gk_idx, Gk);
            Gk_List.push_back(pos);
        }
        Gk_List.sort(rsort);
        list<Position *>::iterator it0;
        list<Position *>::iterator it1;
        for (it0 = Gk_List.begin(); it0 != Gk_List.end(); ++it0) { 
            it1 = ++it0; 
            --it0;
            if ((*it0)->Gk <= 0) {
                break;
            }
            if (((*it1)->Gk >= (*it0)->Gk) and it1 != Gk_List.end()) {
                pos_List.push_back((*it0)->position);
            } else {
                pos_List.push_back((*it0)->position);
                break;
            }
        }
        for (vector<int>::size_type p_idx = 0; p_idx < pos_List.size(); ++p_idx) {
            NetDis tmp_nd = *(netDis);
            for (vector<int>::size_type cut_idx = 0; cut_idx <= pos_List[p_idx]; ++cut_idx) {  // UpdatingCutSet
                if (tmp_CellList[cut_idx]->group == "A") {
                    tmp_nd.Group1.erase( tmp_CellList[cut_idx] ->node_name);  // delete base cell in Group1
                    tmp_nd.Group2.insert( pair<string, Cell *>( tmp_CellList[cut_idx]->node_name, tmp_CellList[cut_idx]));  // add base cell in Group2
                    tmp_nd.G1_size -= tmp_CellList[cut_idx]->area;
                    tmp_nd.G2_size += tmp_CellList[cut_idx]->area;
                } else {  // B
                    tmp_nd.Group2.erase( tmp_CellList[cut_idx] ->node_name);  // delete base cell in Group2
                    tmp_nd.Group1.insert( pair<string, Cell *>( tmp_CellList[cut_idx]->node_name, tmp_CellList[cut_idx]));  // add base cell in Group1
                    tmp_nd.G2_size -= tmp_CellList[cut_idx]->area;
                    tmp_nd.G1_size += tmp_CellList[cut_idx]->area;
                }
            }
            if (lower_bound <= tmp_nd.G1_size and tmp_nd.G1_size <= upper_bound) {
                for (vector<int>::size_type cut_idx = 0; cut_idx <= pos_List[p_idx]; ++cut_idx)
                    if (tmp_CellList[cut_idx]->group == "A") tmp_CellList[cut_idx]->group = "B";
                    else tmp_CellList[cut_idx]->group = "A";
                *netDis = tmp_nd;
                Gk_flag = true;
                break;
            }
        }
        // iterate critical nets to compute cut size
        for (map<string, Net*>::iterator net_it = nets->NetArray.begin(); net_it != nets->NetArray.end(); ++net_it) {
            net_it->second->nG1 = 0;
            net_it->second->nG2 = 0;

            for (vector<int>::size_type c_idx = 0; c_idx < net_it->second->CellList.size(); ++c_idx) {
                map<string, Cell*>::iterator find_cell = netDis->Group1.find(net_it->second->CellList.at(c_idx)->node_name);
                if (find_cell != netDis->Group1.end()) {
                    net_it->second->nG1++;
                } else if (find_cell == netDis->Group1.end()) {
                    net_it->second->nG2++;
                }
            }
            if ((net_it->second->nG1) > 0 and (net_it->second->nG2) > 0) {
                netDis->CutNetList.push_back(net_it->second);
                netDis->cut_size++;
            }
        }

        cout << "------------------------------------" << endl;
        cout << "Iteration " << iters << endl;
        cout << "Size of Group1: " << netDis->G1_size << endl;
        cout << "Size of Group2: " << netDis->G2_size << endl;
        cout << "Cut Size: " << netDis->cut_size << endl;
        cout << endl;
    }
}
