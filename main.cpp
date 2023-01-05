#include <vector>
#include <iostream>
#include <string>
#include <limits>
#include <omp.h>
#include "treeNode.cpp"


using namespace std;

// vector<vector<int>> findPathsSerial(Node* node, int sum, vector<int> path, vector<vector<int> > paths);
// vector<vector<int>> pathSumSerial(Node* root, int sum);
void findPathsSerial(Node* node, int sum, vector<int>& path, vector<vector<int> >& paths);
vector<vector<int>> pathSumSerial(Node* root, int sum);
void findPathsParallel(Node* node, int sum, vector<int>& path, vector<vector<int> >& paths);
vector<vector<int>> pathSumParallel(Node* root, int sum);

int main() {
    vector<vector<double>> inputs = {{5,4,8,11,NAN,13,4,7,2,NAN,5,1,3,4,6,3,NAN,5,6,8,4,NAN,2,NAN,NAN,4,6,NAN,4,6,8,NAN,5,3,NAN,2,4,NAN,5,6,4,3,3
                                    ,6,4,4,6,3,6,7,3,3,5,7,4,1,8,4,3,7,1,7,4,2,6,7,3,3,2,2,6,3,6,7,3,3,5,7,4,1,4,3,7,1,7,4,2,6,7,5,6,8,4,NAN,2},
                                    {5,4,8,11,NAN,13,4,7,2,NAN,NAN,5,1}};
    // vector<vector<double>> inputs = {{ 10, 20, 30, 40, 50, 60 }};


    vector<int> targetSums = {31, 22};

    int targetSumIndex = 0;
    for (auto input: inputs) {
        Node* root = createTree(input, input.size());
        // levelOrder(root);
        // cout << root->right->val;
        printTree(root);

        double start = omp_get_wtime();
        auto resultPathsSerial = pathSumSerial(root, targetSums[targetSumIndex]);
        double end = omp_get_wtime();
        cout << "Serial work took " << end - start << "seconds\n";

        start = omp_get_wtime();
        auto resultPathsParallel = pathSumParallel(root, targetSums[targetSumIndex]);
        end = omp_get_wtime();
        cout << "Parallel work took " << end - start << "seconds\n\n";

        cout << "Serial results: \n";
        for (auto path: resultPathsSerial) {
            for (auto step: path) {
                cout << step << " ";
            }
            cout << "\n";
        }

        cout << "Parallel results: \n";
        for (auto path: resultPathsParallel) {
            for (auto step: path) {
                cout << step << " ";
            }
            cout << "\n";
        }
        
        targetSumIndex++;
    }


	return 0;
}

vector<vector<int>> pathSumSerial(Node* root, int sum) {
    vector<vector<int>> paths;
    vector<int> path;
    findPathsSerial(root, sum, path, paths);
    return paths;  
}

void findPathsSerial(Node* node, int sum, vector<int>& path, vector<vector<int> >& paths) {
    if (!node) return;
    path.push_back(node -> val);
    if (!(node -> left) && !(node -> right) && sum == node -> val)
        paths.push_back(path);
    findPathsSerial(node -> left, sum - node -> val, path, paths);
    findPathsSerial(node -> right, sum - node -> val, path, paths);
    path.pop_back();
}


// vector<vector<int>> pathSumSerial(Node* root, int sum) {
//     vector<vector<int>> paths;
//     vector<int> path;
//     auto res = findPathsSerial(root, sum, path, paths);
//     return res;  
// }

// vector<vector<int>> findPathsSerial(Node* node, int sum, vector<int> path, vector<vector<int>> paths) {

//     // cout << "Sta je ovo ? \n";
//     path.push_back(node -> val);
    
//     if (node->left && node->right) {
//         auto vec1 = findPathsSerial(node->left, sum - node->val, path, paths);
//         auto vec2 = findPathsSerial(node->right, sum - node->val, path, paths);
//         vec1.insert( vec1.end(), vec2.begin(), vec2.end() );
//         return vec1;
//     } else if (node->left && !(node->right)) {
//         return findPathsSerial(node->left, sum - node->val, path, paths);
//     } else if (!(node->left) && node->right) {
//         return findPathsSerial(node->right, sum - node->val, path, paths);
//     } else if (!(node->left) && !(node->right)) {
//         if (!(node -> left) && !(node -> right) && sum == node -> val){
//             paths.push_back(path);
//             return paths;
//         }
        
//     } else {
//         return paths;
//     }
// }


vector<vector<int>> pathSumParallel(Node* root, int sum) {
    vector<vector<int>> paths;
    vector<int> path;
    findPathsParallel(root, sum, path, paths);
    return paths;  
}

void findPathsParallel(Node* node, int sum, vector<int>& path, vector<vector<int> >& paths) {

    if (!node) return;
    // #pragma omp critical
    // {
        path.push_back(node -> val);
        if (!(node -> left) && !(node -> right) && sum == node -> val)
            paths.push_back(path);
    // }

    #pragma omp parallel
    {
        #pragma omp single
        {
            #pragma omp task firstprivate(path)
            findPathsParallel(node -> left, sum - node -> val, path, paths);
            #pragma omp task firstprivate(path)
            findPathsParallel(node -> right, sum - node -> val, path, paths);
        }
    }
    
    // #pragma omp critical
    path.pop_back();
}