#include <iostream>
#include <limits>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>
#include <fstream>

using namespace std;

/* data : 
 * 		N sequences
 *		distance matrix N x N
 *		
 * operational data structures:
 *		cluster vector
 *		cluster struct:
 *				vector of indexes
 */

using DM = vector<vector<float>>;

struct Tree {
	bool _leaf = false;
	int _label;
	float _rootLength;
	shared_ptr<Tree> _left;
	shared_ptr<Tree> _right;
	Tree(int label) { 
			_leaf = true;
			_label = label;
			_rootLength = 0.0f;
	}
	Tree(shared_ptr<Tree> a, shared_ptr<Tree> b, float score) {
		_left = a;
		_right = b;	
		_rootLength = 0.5f * score;
	}
	decltype(auto) isLeaf() { return _leaf; }
	decltype(auto) getRootLength() { return _rootLength; }
	decltype(auto) getLabel() { return _label; }
	decltype(auto) getLeftRootLen() { return _rootLength - _left->getRootLength(); }
	decltype(auto) getRightRootLen() { return _rootLength - _right->getRootLength(); }
};
	
struct ClusterPair {
	int _size;
	shared_ptr<Tree> _tree;
	shared_ptr<ClusterPair> _a;
	shared_ptr<ClusterPair> _b;
	decltype(auto) getTree() { return _tree; }
	decltype(auto) first() { return _a; }
	decltype(auto) second() { return _b; }
	decltype(auto) getSize() { return _size; }
	ClusterPair(int label) {
		_size = 1;
		_tree = make_shared<Tree>(label);	
	} 
	ClusterPair(shared_ptr<ClusterPair> a, shared_ptr<ClusterPair> b, float score) {
		_size = a->getSize() + b->getSize();
		this->_tree = make_shared<Tree>(a->getTree(), b->getTree(), score);
	}
};

inline decltype(auto) mergeClusters(shared_ptr<ClusterPair> a, shared_ptr<ClusterPair> b, float score) {
	return make_shared<ClusterPair>(a, b, score);	
}

inline decltype(auto) calcScore(DM& dm, const vector<shared_ptr<ClusterPair>>& clusters, int c1, int c2) {
	return dm[c1][c2];
}

decltype(auto) UPGMA(DM& dm) {
	float temp_score;
	int best_i, best_j;
	vector<shared_ptr<ClusterPair>> clusters;
	shared_ptr<ClusterPair> a, b;
	for(auto i = 0; i < dm.size(); ++i) {
		clusters.push_back(make_shared<ClusterPair>(i));
	}
	while(clusters.size() > 1) {
		float minScore = numeric_limits<float>::max();
		for(int i = clusters.size() - 1; i >= 0; --i) {
			for(int j = 0; j < i; ++j) {
				if(i == j) continue;
				if((temp_score = dm[i][j]) < minScore)	{
					a = clusters[i]; b = clusters[j]; minScore = temp_score;	
					best_i = i; best_j = j;
				}
			}
		}
		dm.push_back(vector<float>(dm[0].size(), 0.0));
		for(auto& vec : dm) {
			vec.resize(vec.size() + 1, 0);
		}
		for(auto i = 0; i < dm.size(); ++i) {
			if(i == best_i || i == best_j)
					continue;
			dm[dm.size() - 1][i] = ( a->getSize() * dm[best_i][i] + b->getSize() * dm[best_j][i] ) / (a->getSize() + b->getSize());
        	dm[i][dm.size() - 1] = dm[dm.size() - 1][i];
		}
        // we have found the best suiting pair
		// now we 
		// 1) delete them from the vector
		dm.erase(dm.begin() + best_i);
		dm.erase(dm.begin() + best_j + (best_i < best_j) * (-1));
		for(auto & elem : dm) {
			elem.erase(elem.begin() + best_i);
			elem.erase(elem.begin() + best_j + (best_i < best_j) * (-1));
		}
		clusters.erase(clusters.begin() + best_i);
		clusters.erase(clusters.begin() + best_j + (best_i < best_j) * (-1));
		// 2) merge them
		// 3) insert in the end
		clusters.push_back(mergeClusters(a, b, minScore));
		for(auto& d : dm) {
			for(auto& el : d) {
				cout << " " << el << flush;
			}
			cout << endl;
		}
	}
	return clusters[0]->getTree();
}

string saveHelper(shared_ptr<Tree> root, vector<string>& names) {
	cout << root->getRootLength() << endl;
	if(root->isLeaf()) return names[root->getLabel()];	
	return "("+ saveHelper(root->_left, names) + ":" + to_string(root->getRootLength() - root->getLeftRootLen()) + ","
						   + saveHelper(root->_right, names) + ":" + to_string(root->getRootLength() - root->getRightRootLen()) + ")";
}

void saveTree(shared_ptr<Tree> tree, string filename, vector<string>& names) {
	ofstream out;
	out.open(filename);
	out << saveHelper(tree, names) << flush;
	out.close();
}

int main(int argc, char** argv) {
	if(argc != 6) {
		cout << "please provide num sequences, \n"
				"delimiter symbol and \n "
				"filename for distance matrix \n"
			    "the output tree filename \n"
			    "and the filename with names of sequences \n"	<< endl;
		exit(-1);
	}
	string line;
	ifstream inp;
	ifstream seqinp;
	string distances = argv[3];
	size_t pos;
	auto size = stoi(argv[1]);
	string delimiter = argv[2];
	string _ofile = argv[4];
	string seq_names = argv[5];
	vector<string> names;
	seqinp.open(seq_names);	
	if(seqinp.is_open()) {
		string temp = "";	
		while(getline(seqinp, temp)) {
			if(temp.size() > 1) names.push_back(temp);
		}
	}
	int row = 0;
	int column = 0;
	DM dm(size);
	for(auto& elem : dm) {
		elem.resize(size);
	}
	inp.open(distances);
	if(inp.is_open()) {
		while( getline(inp, line)) {
				column = 0;
				while((pos = line.find(";")) != std::string::npos) {
					dm[row][column++] = stof(line.substr(0, pos));
					line.erase(0, pos + delimiter.length());
				}
				++row;
		}
	}
	auto res = UPGMA(dm);
	saveTree(res, _ofile, names);
}

