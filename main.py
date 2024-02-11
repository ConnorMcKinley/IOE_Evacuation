import random
import math

class GraphGenerator:
    def __init__(self, target_node_count, min_x, max_x, min_y, max_y, min_distance_from_node):
        self.target_node_count = target_node_count
        self.min_x = min_x
        self.max_x = max_x
        self.min_y = min_y
        self.max_y = max_y
        self.min_distance = min_distance_from_node
        self.nodes = []  # Each node is a tuple (x, y)
        self.connections = []  # Each connection is a tuple (node_index1, node_index2)

    def distance(self, point1, point2):
        """Calculate the Euclidean distance between two points."""
        return math.sqrt((point1[0] - point2[0])**2 + (point1[1] - point2[1])**2)

    def is_valid_node(self, node):
        """Check if the node is valid considering the minimum distance from other nodes."""
        for existing_node in self.nodes:
            if self.distance(node, existing_node) < self.min_distance:
                return False
        return True

    def generate_nodes(self):
        """Generate nodes within the specified bounds ensuring they respect the minimum distance."""
        while len(self.nodes) < self.target_node_count:
            x = random.uniform(self.min_x, self.max_x)
            y = random.uniform(self.min_y, self.max_y)
            new_node = (x, y)
            print(len(self.nodes))
            if self.is_valid_node(new_node):
                self.nodes.append(new_node)

    def find_closest_node(self, node_index):
        """Find the closest node to the specified node that is not already directly connected."""
        min_distance = float('inf')
        closest_node = None
        for i, other_node in enumerate(self.nodes):
            if i != node_index and not self.is_directly_connected(node_index, i):
                dist = self.distance(self.nodes[node_index], other_node)
                if dist < min_distance:
                    min_distance = dist
                    closest_node = i
        return closest_node

    def is_directly_connected(self, node_index1, node_index2):
        """Check if two nodes are directly connected."""
        return any(c for c in self.connections if (node_index1 in c and node_index2 in c))

    def can_connect(self, node_index1, node_index2):
        """Check if a new connection between two nodes is valid."""
        # Ensuring not exceeding the connection limits per node
        if self.connections_count(node_index1) >= 5 or self.connections_count(node_index2) >= 5:
            return False
        # More checks can be added here, e.g., intersecting with other connections
        return True

    def connections_count(self, node_index):
        """Count how many connections a node has."""
        return sum(1 for c in self.connections if node_index in c)

    def generate_connections(self):
        """Attempt to generate connections respecting the constraints."""
        for node_index in range(len(self.nodes)):
            while self.connections_count(node_index) < 3:
                closest_node = self.find_closest_node(node_index)
                if closest_node is not None and self.can_connect(node_index, closest_node):
                    self.connections.append((node_index, closest_node))
                else:
                    break  # Avoid infinite loop in case no valid connection can be made

    def generate_graph(self):
        """Generate the nodes and connections for the graph."""
        self.generate_nodes()
        self.generate_connections()
        return self.nodes, self.connections
from collections import deque

class GraphGeneratorEnhanced(GraphGenerator):
    def is_fully_connected(self):
        """Check if the graph is fully connected using BFS."""
        if not self.nodes:
            return False
        visited = [False] * len(self.nodes)
        queue = deque([0])  # Start BFS from the first node
        visited[0] = True
        while queue:
            node_index = queue.popleft()
            for i, j in self.connections:
                if i == node_index and not visited[j]:
                    visited[j] = True
                    queue.append(j)
                elif j == node_index and not visited[i]:
                    visited[i] = True
                    queue.append(i)
        return all(visited)

    def connect_disjoint_sets(self):
        """Connect disjoint sets of nodes to ensure the graph is fully connected."""
        while not self.is_fully_connected():
            # Find two nodes from disjoint sets (naive approach for demonstration)
            for i in range(len(self.nodes)):
                for j in range(i + 1, len(self.nodes)):
                    if not self.is_directly_or_indirectly_connected(i, j):
                        self.connections.append((i, j))
                        break
                else:
                    continue
                break

    def is_directly_or_indirectly_connected(self, node_index1, node_index2):
        """Check if two nodes are connected directly or through a series of connections."""
        visited = [False] * len(self.nodes)
        return self.dfs_check_connected(node_index1, node_index2, visited)

    def dfs_check_connected(self, current, target, visited):
        """Depth-first search to check connectivity between two nodes."""
        if current == target:
            return True
        visited[current] = True
        for i, j in self.connections:
            if i == current and not visited[j]:
                if self.dfs_check_connected(j, target, visited):
                    return True
            elif j == current and not visited[i]:
                if self.dfs_check_connected(i, target, visited):
                    return True
        return False

    def generate_graph(self):
        """Override to ensure the graph is fully connected."""
        self.generate_nodes()
        self.generate_connections()
        self.connect_disjoint_sets()  # Ensure full connectivity
        return self.nodes, self.connections

# Enhanced example usage
enhanced_generator = GraphGeneratorEnhanced(target_node_count=90, min_x=0, max_x=50000, min_y=0, max_y=50000, min_distance_from_node=4000)
nodes, connections = enhanced_generator.generate_graph()

# Write the graph to JSON file using the following format:
# {
#   "nodes": [{"x": 1.0, "y": 2.0}, {"x": 3.0, "y": 4.0}],
#   "connections": [{"nodeIndex1": 0, "nodeIndex2": 1}]
# }
import json
graph_data = {"nodes": [{"x": x, "y": y} for x, y in nodes], "connections": [{"nodeIndex1": i, "nodeIndex2": j} for i, j in connections]}
with open('map.json', 'w') as f:
    json.dump(graph_data, f)

# Plot the graph
import matplotlib.pyplot as plt
for node in nodes:
    plt.plot(node[0], node[1], 'o')
for connection in connections:
    node1 = nodes[connection[0]]
    node2 = nodes[connection[1]]
    plt.plot([node1[0], node2[0]], [node1[1], node2[1]], 'k-')
plt.show()
