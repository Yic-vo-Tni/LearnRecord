//
// Created by lenovo on 10/8/2023.
//
#include "iostream"
#include "queue"
#include "stack"

struct Graph{
    char* vertex;
    int** edge;
    int vertexNum, edgeNum;
};

Graph* initGraph(int vertexNum){
    auto* graph = new Graph;
    graph->vertex = new char[vertexNum + 1];
    graph->edge = new int* [vertexNum];
    for(size_t i = 0; i < vertexNum; i++){
        graph->edge[i] = new int[vertexNum];
    }
    graph->vertexNum = vertexNum;
    graph->edgeNum = 0;
    return graph;
}

void createGraph(Graph* graph, const char* vertex, const int* edge){
    for(size_t i = 0; i < graph->vertexNum; i++){
        graph->vertex[i] = vertex[i];
        for(size_t j = 0; j < graph->vertexNum; j++){
            graph->edge[i][j] = edge[i * graph->vertexNum + j];
            if(graph->edge[i][j] != 0){
                graph->edgeNum++;
            }
        }
    }
    graph->edgeNum /= 2;
}

void BFS(const Graph* graph, int startVertex){
    bool* visited = new bool[graph->vertexNum];
    for(size_t i = 0; i < graph->vertexNum; i++){
        visited[i] = false;
    }

    std::queue<size_t> queue;
    visited[startVertex] = true;
    queue.push(startVertex);

    while (!queue.empty()){
        size_t currentVertex = queue.front();
        queue.pop();
        std::cout << graph->vertex[currentVertex] << " ";

        for(size_t i = 0; i < graph->vertexNum; i++){
            if(graph->edge[currentVertex][i] != 0 && !visited[i]){
                visited[i] = true;
                queue.push(i);
            }
        }
    }

    delete[] visited;
}

void DFS(const Graph* graph, int startVertex) {
    bool* visited = new bool[graph->vertexNum];
    for (size_t i = 0; i < graph->vertexNum; i++) {
        visited[i] = false;
    }

    std::stack<int> stack;
    visited[startVertex] = true;
    stack.push(startVertex);

    while (!stack.empty()) {
        int currentVertex = stack.top();
        stack.pop();
        std::cout << graph->vertex[currentVertex] << " ";

        for (size_t i = 0; i < graph->vertexNum; i++) {
            if (graph->edge[currentVertex][i] != 0 && !visited[i]) {
                visited[i] = true;
                stack.push(i);
            }
        }
    }

    delete[] visited;
}

void printGraph(const Graph* graph){
    std::cout << "Vertices: ";
    for(size_t i = 0; i < graph->vertexNum; i++){
        std::cout << graph->vertex[i] << " ";
    }
    std::cout << std::endl;

    std::cout << "Edges: " << std::endl;
    for(size_t i = 0; i < graph->vertexNum; i++){
        for(size_t j = 0; j < graph->vertexNum; j++){
            if(graph->edge[i][j] != 0){
                std::cout << graph->vertex[i] << " -- " << graph->vertex[j] << "  ";
            }
        }
        std::cout << std::endl;
    }
}


void releaseGraph(Graph* graph){
    for(size_t i = 0; i < graph->vertexNum; i++){
        delete[] graph->edge[i];
    }
    delete[] graph->edge;
    delete[] graph->vertex;
    delete graph;
}


int main(){
    Graph* graph = initGraph(5);
    int edge[5][5] = {
            0, 1, 1, 1, 0,
            1, 0, 1, 1, 1,
            1, 1, 0, 0, 0,
            1, 1, 0, 0, 1,
            0, 1, 0, 1, 0
    };
    createGraph(graph, "ABCDE", *edge);
    printGraph(graph);

    std::cout << "BFS Traversal: ";
    BFS(graph, 0);
    std::cout << std::endl;
    std::cout << "DFS Traversal: ";
    DFS(graph, 0);
    std::cout << std::endl;



    releaseGraph(graph);

    return 0;
}