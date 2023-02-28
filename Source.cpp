#include <iostream>
#include <queue>
#include <fstream>
#include <cmath>
#include <vector>
#include <unordered_map>
#include <climits>

using namespace std;

/*using vi = vector<int>;
using vvi = vector<vi>;*/
using ii = pair<int, int>;

struct hash_pair
{
	template <class T1, class T2>
	size_t operator()(const pair<T1, T2> &p) const
	{
		auto hash1 = hash<T1>{}(p.first);
		auto hash2 = hash<T2>{}(p.second);

		if (hash1 != hash2)
		{
			return hash1 ^ hash2;
		}

		// If hash1 == hash2, their XOR is zero.
		return hash1;
	}
};

struct Nodo
{
	ii pos;
	ii desde;
	int dist;
	double distEuch;

	Nodo()
	{
		this->pos = {0, 0};
		this->desde = {0, 0};
		this->dist = 0;
		this->distEuch = INT_MAX;
	}

	Nodo(ii pos)
	{
		this->pos = pos;
		this->desde = {-1, -1};
		this->dist = 0;
		this->distEuch = INT_MAX;
	}

	Nodo(ii pos, int dist, double distEuch)
	{
		this->pos = pos;
		this->desde = {-1, -1};
		this->dist = dist;
		this->distEuch = distEuch;
	}

	Nodo(ii pos, ii desde, int dist, double distEuch)
	{
		this->pos = pos;
		this->desde = desde;
		this->dist = dist;
		this->distEuch = distEuch;
	}

	bool operator>(const Nodo &otro) const
	{
		return ((this->distEuch > otro.distEuch) || (this->dist > otro.dist && this->distEuch > otro.distEuch));
	}
};

using vii = vector<ii>;

void AStar(ii s, ii dest, unordered_map<ii, Nodo, hash_pair> &dist, unordered_map<ii, vii, hash_pair> &adjList)
{
	priority_queue<Nodo, vector<Nodo>, greater<Nodo>> pq;
	double distEuch = hypot((s.first - dest.first), (s.second - dest.second));
	pq.push({s, 0, distEuch});
	while (!pq.empty())
	{
		Nodo front = pq.top();
		pq.pop();
		int d = front.dist;
		ii u = front.pos;
		if (u == dest)
			return;
		for (auto pos : adjList[u])
		{
			Nodo a = dist[pos];
			int resA = front.dist + a.dist;
			int resB = dist[a.pos].dist;
			if (front.dist + a.dist < dist[a.pos].dist)
			{
				dist[pos].distEuch -= dist[a.pos].dist;
				dist[pos].dist = front.dist + a.dist;
				dist[pos].distEuch += dist[a.pos].dist;
				dist[pos].desde = front.pos;
				pq.push(a);
			}
		}
	}
}

void findEuchlideanDistance(unordered_map<ii, Nodo, hash_pair> &distancias, const ii &dest)
{
	for (auto node : distancias)
	{
		node.second.distEuch = hypot(pow(node.first.first - dest.first, 2), pow(node.first.second - dest.second, 2));
	}
}

void showSolution(unordered_map<ii, Nodo, hash_pair> &distancias, const ii &dest)
{
	ii cur = dest;
	int counter = 1;
	while (cur.first != -1)
	{
		cout << "Nodo " << counter++ << " --> Fila: " << cur.first << ". Columna: " << cur.second << "\n";
		cur = distancias[cur].desde;
	}
}

int main()
{
	ifstream file("mapa.txt");
	int F, C;
	file >> F >> C;
	unordered_map<ii, Nodo, hash_pair> distancias;	  // K: coordenada. V: Nodo en dicha coordenada
	unordered_map<ii, vii, hash_pair> adjList(F * C); // K: coordenada. V: Vector de coordenadas contiguas disponibles
	string value;
	ii src, dst;
	for (int i = 0; i < F; i++)
	{
		for (int j = 0; j < C; j++)
		{
			file >> value;
			if (value != "X")
			{ // Todo lo que no sea una posicion que se puede recorrer se ignora
				Nodo aux = Nodo({i, j});
				distancias.insert({{i, j}, aux}); // Insertamos el nodo en la posici�n i, j con valores por defecto
				if (distancias.count({i - 1, j}))
				{ // Si se ha visitado el nodo que est� justo encima, se enlazan
					adjList[{i, j}].push_back({i - 1, j});
					adjList[{i - 1, j}].push_back({i, j});
				}
				if (distancias.count({i, j - 1}))
				{ // Si se ha visitado el nodo que est� justo a la izquierda, se enlazan
					adjList[{i, j}].push_back({i, j - 1});
					adjList[{i, j - 1}].push_back({i, j});
				}
			}
			if (value == "S")
				src = {i, j}; // Se guarda el origen
			else if (value == "D")
				dst = {i, j}; // Se guarda el destino
		}
	}
	findEuchlideanDistance(distancias, dst);
	AStar(src, dst, distancias, adjList);
	showSolution(distancias, dst);

	file.close();
	return 0;
}