#include "kDTree.hpp"
#define INT_MAX 2147483647
//============================== kDTree ==============================
kDTree::kDTree(int k){ 
    this->k = k; 
    root = nullptr;
}

void freeTree(kDTreeNode* node){
    if(!node) return;
    freeTree(node->left);
    freeTree(node->right);
    delete node;
    node = nullptr;
}
kDTree::~kDTree(){
    freeTree(this->root);
}

kDTreeNode* copy(kDTreeNode* node){
    if(!node) return node;
    kDTreeNode* ans = new kDTreeNode(node->data);
    ans->left = copy(node->left);
    ans->right = copy(node->right);
    return ans;
}
const kDTree& kDTree::operator=(const kDTree &other){
    freeTree(root);
    this->k = other.k;
    root = copy(other.root);
    return *this;
}
kDTree::kDTree(const kDTree &other): k(other.k), root(nullptr){
    root = copy(other.root);
}


string order(kDTreeNode* node){
    int n = node->data.size();
    if(!n) return "";
    string s = "";
    s += '(';
    s += to_string(node->data[0]);
    for(int i = 1; i < n; ++i) s += ", " + to_string(node->data[i]);
    s += ") ";
    return s;
}

string inorder(kDTreeNode* node){
    if(!node) return "";
    string s = "";
    s += inorder(node->left);
    s += order(node);
    s += inorder(node->right);
    return s;
}
void kDTree::inorderTraversal() const{
    string s = inorder(root);
    s.pop_back();   cout << s;
}

string preorder(kDTreeNode* node){
    if(!node) return "";
    string s = "";
    s += order(node);
    s += preorder(node->left);
    s += preorder(node->right);
    return s;
}
void kDTree::preorderTraversal() const{
    string s = preorder(root);
    s.pop_back();   cout << s;
}

string postorder(kDTreeNode* node){
    if(!node) return "";
    string s = "";
    s += postorder(node->left);
    s += postorder(node->right);
    s += order(node);
    return s;
}
void kDTree::postorderTraversal() const{
    string s = postorder(root);
    s.pop_back();   cout << s;
}


int h(kDTreeNode* node){
    if(!node) return 0;
    return 1 + max( h(node->left), h(node->right) );
}
int kDTree::height() const{
    return h(root);
}

void countNode(kDTreeNode* node, int& n){
    if(!node) return;   ++n;
    countNode(node->left, n);
    countNode(node->right, n);
}
int kDTree::nodeCount() const{
    int n = 0;
    countNode(root, n);
    return n;
}

void countLeaf(kDTreeNode* node, int& n){
    if(!node) return;
    if(!node->left && !node->right) ++n;
    countLeaf(node->left, n);
    countLeaf(node->right, n);
}
int kDTree::leafCount() const{
    int n = 0;
    countLeaf(root, n);
    return n;
}


void add(kDTreeNode*& node, const vector<int> &point, int& lvl, const int& k){
    if(!node) node = new kDTreeNode(point);
    else{
        lvl %= k;
        if(node->data[lvl] > point[lvl]) 
             add(node->left , point, ++lvl, k);
        else add(node->right, point, ++lvl, k);
    }
}
void kDTree::insert(const vector<int> &point){
    if(point.size() != k) return;
    int lvl = 0; //mat phang chia hien tai
    add(root, point, lvl, k);
}
vector<int> findmin(kDTreeNode* node, const int& lvl){
    if(!node) return {};
    vector<int> lh = findmin(node->left, lvl);
    vector<int> rh = findmin(node->right, lvl);
    vector<int> ans = node->data;
    if(!lh.empty() && lh[lvl] < ans[lvl]) ans = lh;
    if(!rh.empty() && rh[lvl] < ans[lvl]) ans = rh;
    return ans;
}

void erase(kDTreeNode*& node, const vector<int> &point, int& lvl, const int& k){
    if(!node) return;
    lvl %= k;

    if(node->data != point){
        if(node->data[lvl] > point[lvl]) erase(node->left, point, ++lvl, k);
        else erase(node->right, point, ++lvl, k);
    }
    else{
        if(!node->left && !node->right){ //leaf node
            delete node; 
            node = nullptr;
        } 

        else if(node->right){ //have right subtree 
            vector<int> tmp = findmin(node->right, lvl);
            node->data = tmp;
            erase(node->right, tmp, ++lvl, k);
        }

        else{ //have left subtree
            vector<int> tmp = findmin(node->left, lvl);
            node->data = tmp;
            node->right = node->left;
            node->left = nullptr;
            erase(node->right, tmp, ++lvl, k);
        }
    }
}


void kDTree::remove(const vector<int> &point){
    if(point.size() != k) return;
    int lvl = 0; //mat phang chia hien tai
    erase(root, point, lvl, k);
}


bool look(kDTreeNode* node, const vector<int> &point, int& lvl, const int& k){
    if(!node) return 0;
    if(node->data == point) return 1;
    lvl %= k;
    if(node->data[lvl] > point[lvl])
         return look(node->left , point, ++lvl, k);
    else return look(node->right, point, ++lvl, k); 
}

bool kDTree::search(const vector<int> &point){
    if(point.size() != k) return 0;
    int lvl = 0; //mat phang chia hien tai
    return look(root, point, lvl, k);
}


void merge(vector<vector<int>>& arr, int left, int mid, int right, const int& lvl, const int& n) {
    int n1 = mid - left + 1;
    int n2 = right - mid;
    vector<vector<int>> L(n1, vector<int>(n)), R(n2, vector<int>(n));

    for (int i = 0; i < n1; i++) L[i] = arr[left + i];
    for (int j = 0; j < n2; j++) R[j] = arr[mid + 1 + j];
    int i = 0, j = 0, k = left; 

    while (i < n1 && j < n2) {
        if (L[i][lvl] <= R[j][lvl]) {
            arr[k] = L[i];
            ++i;
        } else {
            arr[k] = R[j];
            ++j;
        }
        ++k;
    }

    while (i < n1) {
        arr[k] = L[i];
        ++i;
        ++k;
    }
    while (j < n2) {
        arr[k] = R[j];
        ++j;
        ++k;
    }
}

void mergeSort(vector<vector<int>>& arr, int left, int right, const int& lvl){
    int n = arr[0].size();
    if (left >= right) return;
    int mid = left + (right - left) / 2;
    mergeSort(arr, left, mid, lvl);
    mergeSort(arr, mid + 1, right, lvl);
    merge(arr, left, mid, right, lvl, n);
}

kDTreeNode* build(kDTreeNode* node, vector<vector<int>> &pointList, int lvl, const int& k){
    int nums = pointList.size();
    if(!nums) return nullptr; //empty vector
    lvl %= k;
    mergeSort(pointList, 0, nums-1, lvl);
    int mid = (nums-1)/2;
    while(mid && pointList[mid][lvl] == pointList[mid-1][lvl]) --mid;
    node = new kDTreeNode(pointList[mid]);
    vector<vector<int>> lh = vector<vector<int>>(pointList.begin(), pointList.begin() + mid);
    vector<vector<int>> rh = vector<vector<int>>(pointList.begin() + mid + 1, pointList.end());
    node->left  = build(node->left , lh, lvl + 1, k);
    node->right = build(node->right, rh, lvl + 1, k);
    return node;
}

void kDTree::buildTree(const vector<vector<int>> &pointList){
    freeTree(root);
    vector<vector<int>> tmp = pointList;
    root = build(root, tmp, 0, k);
}


int R(vector<int> a, vector<int> b){
    if(a.size() != b.size()) throw runtime_error("error calculating distance");
    int n = a.size(), ans = 0;
    for(int i = 0; i < n; ++i)
        ans += (a[i] - b[i]) * (a[i] - b[i]);
    return ans;
}
kDTreeNode* near(kDTreeNode* node, const vector<int> &target, int lvl, const int& k){
    if(!node) throw std::runtime_error("from near, nullptr node");
    kDTreeNode* ans = nullptr;      lvl %= k;

    if(node->data[lvl] > target[lvl]){ //left case
        if(!node->left) return node; //step 3
        ans = near(node->left, target, lvl + 1, k); //step 2
        int dis = R(ans->data, target);
        int tmp = R(node->data, target);
        if(tmp < dis) ans = node, dis = tmp;    //step 4.1
        int r = (node->data[lvl] - target[lvl]) * (node->data[lvl] - target[lvl]);
        if(r <= dis && node->right){    //step 4.2
            kDTreeNode* check = near(node->right, target, lvl + 1, k);
            tmp = R(check->data, target);
            if(tmp < dis) ans = check, dis = tmp;
        }
    }
    else{                               //right case
        if(!node->right) return node; //step 3
        ans = near(node->right, target, lvl + 1, k);//step 2
        int dis = R(ans->data, target);
        int tmp = R(node->data, target);
        if(tmp < dis) ans = node, dis = tmp;    //step 4.1
        int r = (node->data[lvl] - target[lvl]) * (node->data[lvl] - target[lvl]);
        if(r <= dis && node->left){    //step 4.2
            kDTreeNode* check = near(node->left, target, lvl + 1, k);
            tmp = R(check->data, target);
            if(tmp < dis) ans = check, dis = tmp;
        }
    }
    return ans;  
}

void kDTree::nearestNeighbour(const vector<int> &target, kDTreeNode *&best){ //*best -> *&best
    best = near(root, target, 0, k);
    vector<int> tmp = best->data;
    best = new kDTreeNode(tmp);
}
void kDTree::kNearestNeighbour(const vector<int> &target, int k, vector<kDTreeNode*> &bestList){
    kDTree tree = *this;
    for(int i = 0; i < k; ++i){
        kDTreeNode* tmp = nullptr;
        tree.nearestNeighbour(target, tmp);
        if(!tmp) throw runtime_error("from kNearestNeighbour, tmp is nullptr");
        bestList.push_back(tmp);
        tree.remove(tmp->data);
    }
}

//============================== kNN ==============================
kNN::kNN(int k): k(k){}
void kNN::fit(Dataset &X_train, Dataset &y_train){
    for(auto i : X_train.data){
        vector<int> tmp = vector<int>(i.begin(), i.end());
        x.push_back(tmp);
    }
    for(auto i : y_train.data) y.push_back(i.front());
    int row = 0, col = 0;
    X_train.getShape(row, col);
    //row->số node trong cây, col->số chiều trong 1 node
    tree = kDTree(col);
    tree.buildTree(x);
}

template <typename T>
int find(const vector<T>& x, const T& data){
    int n = x.size();
    for(int i = 0; i < n; ++i)
        if(x[i] == data) return i;
    return -1;
}

Dataset kNN::predict(Dataset &X_test){
    Dataset y_test; y_test.columnName.push_back("label");
    list<list<int>> x = X_test.data;
    for(auto i : x){
        vector<kDTreeNode *> bestList;
        vector<int> target = vector<int>(i.begin(), i.end());
        tree.kNearestNeighbour(target, this->k, bestList);
        map search;
        for(auto tmp : bestList){
            int idx = find(this->x, tmp->data);
            if(idx == -1 || idx >= y.size()) throw runtime_error("out of range");
            search.add(y[idx]);
        }
        y_test.data.push_back({search.maxkey()});
    }
    return y_test;
}

double kNN::score(const Dataset &y_test, const Dataset &y_pred){
    int correct = 0, total = 0; //total is row
    y_test.getShape(total, correct); correct = 0;

    for(auto it_test = y_test.data.begin(), it_pred = y_pred.data.begin(); 
             it_test != y_test.data.end(); ++it_test, ++it_pred){
        if( (*it_test).front() == (*it_pred).front())
            ++correct;
    }
    return (double)correct/total;
}

map::map() : head(nullptr){}
map::~map(){ clear(); }
void map::add(int key){
    if(head){
        pair* tmp = head;
        pair* tail = nullptr;
        while(tmp){
            tail = tmp;
            if(tmp->key == key){ tmp->val++; return; }
            tmp = tmp->next;
        }
        tail->next = new pair(key);
    }
    else head = new pair(key);
}
int map::maxkey(){
    if(!head) return -1; //error
    int key = -1, val = 0;
    pair* tmp = head;
    while(tmp){
        if(tmp->val > val){
            key = tmp->key;
            val = tmp->val;
        }
        else if(tmp->val == val && tmp->key < key) key = tmp->key;
        tmp = tmp->next;
    }
    if(key == -1) throw runtime_error("from maxkey(), no return value");
    return key;
}
void map::clear(){
    while(head){
        pair* del = head;
        head = head->next;
        delete del;
    }
}


