#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <climits>
#include <sstream>
#include <algorithm>
using namespace std;

class City {
public:
  int id;
  string code;
  string name;
  int population;
  int elevation;

  City() : id(0), population(0), elevation(0) {}  // Default constructor                                                                                                                                    

  City(int _id, const string& _code, const string& _name, int _population, int _elevation)
    : id(_id), code(_code), name(_name), population(_population), elevation(_elevation) {}
};

class Graph {
private:
  vector<vector<pair<int, int>>> adjList;  // pair<destination, weight>                                                                                                                      
  unordered_map<string, City> cities;  // City code to City map                                                                                                                                   

public:
  Graph(int size) {
    adjList.resize(size);
  }

  void addEdge(int src, int dest, int weight) {
    adjList[src].push_back({dest, weight});
  }

  void addCity(const City& city) {
    cities[city.code] = city;
  }

  City getCityByCode(const string &code) const {
    return cities.at(code);
  }

  City getCityById(int id) const {
    for (const auto &entry : cities) {
      if (entry.second.id == id) {
        return entry.second;
      }
    }
    throw runtime_error("City ID not found");
  }
  pair<int, vector<int>> shortestPath(int src, int dest) {
    vector<int> distance(adjList.size(), INT_MAX);
    vector<int> previous(adjList.size(), -1);
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    distance[src] = 0;
    pq.push({0, src});


    while (!pq.empty()) {
      int curr = pq.top().second;
      pq.pop();

      for (auto &edge : adjList[curr]) {
        int next = edge.first;
        int weight = edge.second;

        if (distance[curr] + weight < distance[next]) {
          distance[next] = distance[curr] + weight;
          pq.push({distance[next], next});
          previous[next] = curr;
        }
      }
    }

    vector<int> path;
    for (int v = dest; v != -1; v = previous[v])
      path.push_back(v);

    reverse(path.begin(), path.end());

    return {distance[dest], path};
  }
};



int main(int argc, char *argv[]) {
  if(argc != 3) {
    cerr << "Usage: ./prog8 <from_city_code> <to_city_code>" << endl;
    return 1;
  }

  string fromCityCode = argv[1];
  string toCityCode = argv[2];

  Graph graph(20);

  ifstream cityFile("city.txt");
  string line;
  while(getline(cityFile, line)) {
    istringstream ss(line);
    int id, population, elevation;
    string code, name;
    ss >> id >> code >> name >> population >> elevation;
    graph.addCity(City(id, code, name, population, elevation));
  }

  ifstream roadFile("road.txt");
  while(getline(roadFile, line)) {
    istringstream ss(line);
    int src, dest, weight;
    ss >> src >> dest >> weight;
    graph.addEdge(src, dest, weight);
  }

  City fromCity = graph.getCityByCode(fromCityCode);
  City toCity = graph.getCityByCode(toCityCode);

  cout << "From City: " << fromCity.name << ", population " << fromCity.population << ", elevation " << fromCity.elevation << endl;
  cout << "To City: " << toCity.name << ", population " << toCity.population << ", elevation " << toCity.elevation << endl;

  auto result = graph.shortestPath(fromCity.id, toCity.id);
  if(result.second.size() == 1 && result.second[0] == toCity.id) {
    cout << "No route from " << fromCity.name << " to " << toCity.name << endl;
  } else {
    cout << "The shortest distance from " << fromCity.name << " to " << toCity.name << " is " << result.first << endl;
    cout << "through the route: ";
    for(size_t i = 0; i < result.second.size(); ++i) {
      // Using getCityById here instead of getCityByCode                                                                                                                                                    
      cout << graph.getCityById(result.second[i]).name;
      if(i != result.second.size() - 1) {
        cout << "->";
      }
    }
    cout << endl;

    return 0;
  }
}
