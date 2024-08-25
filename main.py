import random
import math

class GraphGenerator:
    def __init__(self, target_node_count, min_x, max_x, min_y, max_y, min_distance_from_node, grid_interval=None, max_connection_length=None):
        self.target_node_count = target_node_count
        self.min_x = min_x
        self.max_x = max_x
        self.min_y = min_y
        self.max_y = max_y
        self.min_distance = min_distance_from_node
        self.grid_interval = grid_interval  # Grid interval for structured node placement
        self.max_connection_length = max_connection_length  # Maximum length for connections
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
        # Adjusted logic to ensure target node count is met
        if self.grid_interval is None:
            while len(self.nodes) < self.target_node_count:
                x = random.uniform(self.min_x, self.max_x)
                y = random.uniform(self.min_y, self.max_y)
                if self.is_valid_node((x, y)):
                    self.nodes.append((x, y))
        else:
            grid_points = [(x, y) for x in range(self.min_x, self.max_x+1, self.grid_interval) for y in range(self.min_y, self.max_y+1, self.grid_interval)]
            random.shuffle(grid_points)  # Shuffle grid points to maintain randomness
            for point in grid_points:
                if len(self.nodes) >= self.target_node_count:
                    break
                if self.is_valid_node(point):
                    self.nodes.append(point)

            # If target not met due to strict grid interval and min_distance, reduce constraints
            if len(self.nodes) < self.target_node_count:
                additional_attempts = 0
                while len(self.nodes) < self.target_node_count and additional_attempts < 10000:
                    x = random.uniform(self.min_x, self.max_x)
                    y = random.uniform(self.min_y, self.max_y)
                    if self.is_valid_node((x, y)):
                        self.nodes.append((x, y))
                    additional_attempts += 1

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

        def segments_intersect(p1, q1, p2, q2):
            """ Return True if line segments p1q1 and p2q2 intersect. """
# checks orientation and connectivity
            def orientation(p, q, r):
                val = (q[1] - p[1]) * (r[0] - q[0]) - (q[0] - p[0]) * (r[1] - q[1])
                if val == 0:
                    return 0
                elif val > 0:
                    return 1
                else:
                    return 2

            def on_segment(p, q, r):
                if min(p[0], q[0]) <= r[0] <= max(p[0], q[0]) and min(p[1], q[1]) <= r[1] <= max(p[1], q[1]):
                    return True
                return False

            o1 = orientation(p1, q1, p2)
            o2 = orientation(p1, q1, q2)
            o3 = orientation(p2, q2, p1)
            o4 = orientation(p2, q2, q1)

            if o1 != o2 and o3 != o4:
                return True

            if o1 == 0 and on_segment(p1, q1, p2):
                return True

            if o2 == 0 and on_segment(p1, q1, q2):
                return True

            if o3 == 0 and on_segment(p2, q2, p1):
                return True

            if o4 == 0 and on_segment(p2, q2, q1):
                return True

            return False

        """Check if a new connection between two nodes is valid."""
        # Ensuring not exceeding the connection limits per node
        if self.connections_count(node_index1) >= 5 or self.connections_count(node_index2) >= 5:
            return False

        if self.max_connection_length is not None and self.distance(self.nodes[node_index1], self.nodes[
            node_index2]) > self.max_connection_length:
            return False

        new_connection = (self.nodes[node_index1], self.nodes[node_index2])
        for i in range(len(self.connections)):
            index1, index2 = self.connections[i]
            existing_connection = (self.nodes[index1], self.nodes[index2])

            # Ignore connections that share a node with the new connection

            if node_index1 not in (index1, index2) and node_index2 not in (index1, index2):
                if segments_intersect(new_connection[0], new_connection[1], existing_connection[0],
                                      existing_connection[1]):
                    return False  # Intersection found, don't allow this connection

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
# enhanced_generator = GraphGeneratorEnhanced(target_node_count=60, min_x=0, max_x=35000, min_y=0, max_y=35000, min_distance_from_node=3000, max_connection_length=10000, grid_interval=4000)
# enhanced_generator = GraphGeneratorEnhanced(target_node_count=10, min_x=0, max_x=5000, min_y=0, max_y=5000, min_distance_from_node=2000, max_connection_length=10000, grid_interval=4000)
enhanced_generator = GraphGeneratorEnhanced(target_node_count=200, min_x=0, max_x=77000, min_y=0, max_y=77000, min_distance_from_node=3000, max_connection_length=10000, grid_interval=4000)
nodes, connections = enhanced_generator.generate_graph()

print("Generated number of nodes:", len(nodes))

# Write the graph to JSON file using the following format:
# {
#   "nodes": [{"x": 1.0, "y": 2.0}, {"x": 3.0, "y": 4.0}],
#   "connections": [{"nodeIndex1": 0, "nodeIndex2": 1}]
# }
import json
graph_data = {"nodes": [{"x": x, "y": y} for x, y in nodes], "connections": [{"nodeIndex1": i, "nodeIndex2": j} for i, j in connections]}
with open('map.json', 'w') as f:
    json.dump(graph_data, f,indent=4)

# Plot the graph
import matplotlib.pyplot as plt
import numpy as ny

for node in nodes:
    plt.plot(node[0], node[1], 'o')
for connection in connections:
    node1 = nodes[connection[0]]
    node2 = nodes[connection[1]]
    plt.plot([node1[0], node2[0]], [node1[1], node2[1]], 'k-')

plt.show()