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

using DM = vector<vector<int>>;

struct Tree {
	bool _leaf;
	int _label;
	float _rootLength;
	shared_ptr<Tree> _left;
	shared_ptr<Tree> _right;
	Tree(int label) { 
			_leaf = true;
			_label = label;
	}
	Tree(shared_ptr<Tree> a, shared_ptr<Tree> b, float score) {
		_left = a;
		_right = b;	
		_rootLength = 0.5 * score;
	}
	decltype(auto) isLeaf() { return _leaf; }
	decltype(auto) getRootLength() { return _rootLength; }
	decltype(auto) getLabel() { return _label; }
};
	
struct ClusterPair {
	bool _single;
	vector<int> _label;
	int _size;
	shared_ptr<Tree> _tree;
	shared_ptr<ClusterPair> _a;
	shared_ptr<ClusterPair> _b;
	decltype(auto) isSingle() { return _single; }
	decltype(auto) getTree() { return _tree; }
	decltype(auto) first() { return _a; }
	decltype(auto) second() { return _b; }
	decltype(auto) getLabel() { return _label[0]; }
	decltype(auto) getSize() { return _size; }
	ClusterPair(int label) : _single(true) {
		_label.push_back(label);
		_size = 1;
		_tree = make_shared<Tree>(label);	
	} 
	ClusterPair(shared_ptr<ClusterPair> a, shared_ptr<ClusterPair> b, float score) : _a(a), _b(b) {
		auto av = a->getAllSequences();
		auto bv = b->getAllSequences();
		_label.insert(_label.end(), av.begin(), av.end());
		_label.insert(_label.end(), bv.begin(), bv.end());
		_single = false;
		_size = a->getSize() + b->getSize();
		this->_tree = make_shared<Tree>(a->getTree(), b->getTree(), score);
	}
	const vector<int>& getAllSequences() {
			return _label;
	}
};

decltype(auto) mergeClusters(shared_ptr<ClusterPair> a, shared_ptr<ClusterPair> b, float score) {
	return make_shared<ClusterPair>(a, b, score);	
}

decltype(auto) calcScore(const DM& dm, shared_ptr<ClusterPair> c1, shared_ptr<ClusterPair> c2) {
	float score = 0.0f;
	auto av = c1->getAllSequences();	
	auto bv = c2->getAllSequences();	
	for(auto& elem_a : av) 
			for(auto& elem_b : bv) {
				score += dm[elem_a][elem_b];
			}
	return score / (c1->getSize() + c2->getSize());
}

decltype(auto) UPGMA(const DM& dm) {
	const int s = dm.size();
	float temp_score;
	int best_i, best_j;
	vector<shared_ptr<ClusterPair>> clusters;
	shared_ptr<ClusterPair> a, b;
	for(auto i = 0; i < s; ++i) {
		clusters.push_back(make_shared<ClusterPair>(i));
	}
	while(clusters.size() > 1) {
		float minScore = numeric_limits<float>::max();
		for(int i = clusters.size() - 1; i >= 0; --i) {
			for(int j = 0; j < i; ++j) {
				if((temp_score = calcScore(dm, clusters[i], clusters[j])) < minScore)	{
					a = clusters[i]; b = clusters[j]; minScore = temp_score;	
					best_i = i; best_j = j;
				}
			}
		}
		// we have found the best suiting pair
		// now we 
		// 1) delete them from the vector
		clusters.erase(clusters.begin() + best_i);
		clusters.erase(clusters.begin() + best_j);
		// 2) merge them
		// 3) insert in the end
		clusters.push_back(mergeClusters(a, b, minScore));
	}
	return clusters[0]->getTree();
}

string saveHelper(shared_ptr<Tree> root) {
	if(root->isLeaf()) return to_string(root->getLabel());	
	return "("+ saveHelper(root->_left) + ":" + to_string(root->getRootLength()) + ","
						   + saveHelper(root->_right) + ":" + to_string(root->getRootLength()) + ")";
}

void saveTree(shared_ptr<Tree> tree, string filename) {
	ofstream out;
	out.open(filename);
	out << saveHelper(tree) << flush;
	out.close();
}

int main(int argc, char** argv) {
	if(argc != 4) {
		cout << "please provide  num sequences and \n"
				"delimiter symbol and \n "
				"filename for distance matrix" << endl;
		exit(-1);
	}
	string line;
	ifstream inp;
	string distances = argv[3];
	size_t pos;
	auto size = stoi(argv[1]);
	string delimiter = argv[2];
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
					dm[row][column++] = stoi(line.substr(0, pos));
					line.erase(0, pos + delimiter.length());
				}
				++row;
		}
	}
	auto res = UPGMA(dm);
	saveTree(res, "tree.newick");
}

