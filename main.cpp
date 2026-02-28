#include <gtest/gtest.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <unordered_set>
#include <cassert>
#include "graph.hpp"

TEST(IsSubgraph, sameGraph) {
  Graph<int> G {3};
  G.addEdge(0, 1, 2);
  G.addEdge(1, 0, -1);
  G.addEdge(1, 2, 10);
  G.addEdge(2, 1, -5);
  EXPECT_TRUE(isSubgraph(G, G));
}

TEST(IsSubgraph, edgeWeightsDiffer) {
  Graph<double> G {3};
  G.addEdge(0, 1, 2.1);
  G.addEdge(1, 0, -1.4);
  G.addEdge(1, 2, 10.0);
  G.addEdge(2, 1, -5.89);
  Graph<double> H {G};
  H.removeEdge(2, 1);
  H.addEdge(2, 1, 1.7);
  EXPECT_FALSE(isSubgraph(H, G));
}

TEST(IsSubgraph, subgraphCanBeSmaller) {
  Graph<double> G {3};
  G.addEdge(0, 1, 2.1);
  G.addEdge(1, 0, -1.4);
  G.addEdge(1, 2, 10.0);
  G.addEdge(2, 1, -5.89);
  Graph<double> H {2};
  G.addEdge(0, 1, 2.1);
  G.addEdge(1, 0, -1.4);
  EXPECT_TRUE(isSubgraph(H, G));
}

TEST(IsSubgraph, subgraphCannotBeLarger) {
  Graph<double> G {3};
  G.addEdge(0, 1, 2.1);
  G.addEdge(1, 0, -1.4);
  G.addEdge(1, 2, 10.0);
  G.addEdge(2, 1, -5.89);
  Graph<double> H {4};
  H.addEdge(0, 1, 2.1);
  H.addEdge(1, 0, -1.4);
  H.addEdge(1, 2, 10.0);
  H.addEdge(2, 1, -5.89);
  EXPECT_FALSE(isSubgraph(H, G));
}

TEST(IsSubgraph, superGraph) {
  Graph<double> G {4};
  G.addEdge(0, 1, 2.1);
  G.addEdge(1, 0, -1.4);
  G.addEdge(1, 2, 10.0);
  G.addEdge(2, 1, -5.89);
  G.addEdge(3, 1, 1.27);
  Graph<double> H {G};
  H.addEdge(0, 3, -1.0);
  EXPECT_FALSE(isSubgraph(H, G));
  EXPECT_TRUE(isSubgraph(G, H));
}

TEST(IsTree, smallTree) {
  Graph<double> G {5};
  G.addEdge(0, 2, 2.1);
  G.addEdge(0, 4, -1.4);
  G.addEdge(4, 1, 10.5);
  G.addEdge(1, 3, -2.7);
  EXPECT_TRUE(isTreePlusIsolated(G, 0));
}

TEST(IsTree, smallTreePlusIsolated) {
  Graph<double> G {6};
  G.addEdge(0, 2, 2.1);
  G.addEdge(0, 5, -1.4);
  G.addEdge(5, 1, 10.5);
  G.addEdge(1, 3, -2.7);
  EXPECT_TRUE(isTreePlusIsolated(G, 0));
}

TEST(IsTree, unreachableVertexNotIsolated) {
  Graph<double> G {10};
  G.addEdge(0, 2, 2.1);
  G.addEdge(0, 4, -1.4);
  G.addEdge(4, 1, 10.5);
  G.addEdge(1, 3, -2.7);
  G.addEdge(0, 9, 2.4);
  G.addEdge(2, 7, 8.8);
  G.addEdge(5, 6, 9.3);
  EXPECT_FALSE(isTreePlusIsolated(G, 0));
}

TEST(IsTree, unreachableEdgeDirectionNotIsolated) {
  Graph<double> G {7};
  G.addEdge(0, 2, 2.1);
  G.addEdge(0, 4, -1.4);
  G.addEdge(1, 4, 10.5);
  G.addEdge(1, 3, -2.7);
  G.addEdge(0, 5, 2.4);
  G.addEdge(2, 6, 8.8);
  EXPECT_FALSE(isTreePlusIsolated(G, 0));
}

TEST(IsTree, undirectedCycle) {
  Graph<double> G {7};
  G.addEdge(0, 2, 2.1);
  G.addEdge(0, 4, -1.4);
  G.addEdge(4, 1, 10.5);
  G.addEdge(1, 3, -2.7);
  G.addEdge(0, 5, 2.4);
  G.addEdge(2, 6, 8.8);
  G.addEdge(6, 5, 6.2);
  EXPECT_FALSE(isTreePlusIsolated(G, 0));
}

TEST(IsTree, undirectedCycleDifferentRoot) {
  Graph<int> G {4};
  G.addEdge(3, 1, 4);
  G.addEdge(3, 0, -1);
  G.addEdge(3, 2, 2);
  G.addEdge(1, 0, 9);
  EXPECT_FALSE(isTreePlusIsolated(G, 3));
}

TEST(pathLengths, binaryTree) {
  Graph<int> G {7};
  G.addEdge(0, 1, 2);
  G.addEdge(0, 2, -1);
  G.addEdge(1, 3, 1);
  G.addEdge(1, 4, 5);
  G.addEdge(2, 5, 3);
  G.addEdge(2, 6, 2);
  std::vector<int> distances {0, 2, -1, 3, 7, 2, 1};
  EXPECT_EQ(pathLengthsFromRoot(G, 0), distances);
}

TEST(pathLengths, lengthsDecrease) {
  Graph<int> G {7};
  G.addEdge(0, 1, -1);
  G.addEdge(0, 2, -1);
  G.addEdge(1, 3, -1);
  G.addEdge(1, 4, -1);
  G.addEdge(2, 5, -1);
  G.addEdge(2, 6, -1);
  std::vector<int> distances {0, -1, -1, -2, -2, -2, -2};
  EXPECT_EQ(pathLengthsFromRoot(G, 0), distances);
}

TEST(pathLengths, rootNotZero) {
  Graph<int> G {8};
  G.addEdge(3, 2, 1);
  G.addEdge(3, 4, 4);
  G.addEdge(7, 5, -10);
  G.addEdge(7, 6, 2);
  G.addEdge(3, 7, 3);
  G.addEdge(4, 1, -5);
  G.addEdge(2, 0, 11);
  std::vector<int> distances {12, -1, 1, 0, 4, -7, 5, 3};
  EXPECT_EQ(pathLengthsFromRoot(G, 3), distances);
}

TEST(pathLengths, sameTreeDifferentWeights) {
  Graph<int> G {8};
  G.addEdge(3, 2, 4);
  G.addEdge(3, 4, 2);
  G.addEdge(7, 5, -10);
  G.addEdge(7, 6, 1);
  G.addEdge(3, 7, 9);
  G.addEdge(4, 1, -5);
  G.addEdge(2, 0, -2);
  std::vector<int> distances {2, -3, 4, 0, 2, -1, 10, 9};
  EXPECT_EQ(pathLengthsFromRoot(G, 3), distances);
}

TEST(EdgesRelaxedTest, sizeOne) {
  Graph<int> G(1);
  // bestDistanceTo.at(source) must be 0
  std::vector<int> bestDistanceTo {0};
  EXPECT_TRUE(allEdgesRelaxed(bestDistanceTo, G, 0));
  bestDistanceTo = {1};
  EXPECT_FALSE(allEdgesRelaxed(bestDistanceTo, G, 0));
}

TEST(EdgesRelaxedTest, smallLine) {
  Graph<int> G(6);
  G.addEdge(0, 1, 1);
  G.addEdge(1, 2, 1);
  G.addEdge(2, 3, 1);
  G.addEdge(3, 4, 1);
  G.addEdge(4, 5, 1);
  std::vector<int> bestDistanceTo {0, 1, 2, 3, 4, 5};
  EXPECT_TRUE(allEdgesRelaxed(bestDistanceTo, G, 0));
  ++bestDistanceTo.back();
  EXPECT_FALSE(allEdgesRelaxed(bestDistanceTo, G, 0));
}

TEST(EdgesRelaxedTest, smallCyclePlusEdge) {
  Graph<int> G(6);
  G.addEdge(0, 1, 1);
  G.addEdge(1, 2, 1);
  G.addEdge(2, 3, 1);
  G.addEdge(3, 4, 1);
  G.addEdge(4, 5, 1);
  G.addEdge(5, 0, 1);
  G.addEdge(0, 3, 1);
  std::vector<int> bestDistanceTo {0, 1, 2, 1, 2, 3};
  EXPECT_TRUE(allEdgesRelaxed(bestDistanceTo, G, 0));
  bestDistanceTo.at(3) += 1;
  EXPECT_FALSE(allEdgesRelaxed(bestDistanceTo, G, 0));
}

TEST(EdgesRelaxedTest, smallNegativeWeight) {
  Graph<int> G(4);
  G.addEdge(0, 1, 4); 
  G.addEdge(0, 2, 5); 
  G.addEdge(2, 1, -3); 
  G.addEdge(1, 3, 1); 
  std::vector<int> bestDistanceTo {0, 2, 5, 3};
  EXPECT_TRUE(allEdgesRelaxed(bestDistanceTo, G, 0));
  bestDistanceTo = {0, 2, 5, 5};
  EXPECT_FALSE(allEdgesRelaxed(bestDistanceTo, G, 0));
}

TEST(EdgesRelaxedTest, negativeStar) {
  Graph<int> G(6);
  G.addEdge(0, 1, 1); 
  G.addEdge(0, 2, 1); 
  G.addEdge(0, 3, 1); 
  G.addEdge(0, 4, 1); 
  G.addEdge(0, 5, 1); 
  G.addEdge(2, 4, -1); 
  G.addEdge(4, 5, -1); 
  std::vector<int> bestDistanceTo {0, 1, 1, 1, 1, 1};
  EXPECT_FALSE(allEdgesRelaxed(bestDistanceTo, G, 0));
  bestDistanceTo = {0, 1, 1, 1, 0, -1};
  EXPECT_TRUE(allEdgesRelaxed(bestDistanceTo, G, 0));
}

TEST(EdgesRelaxedTest, smallNegativeCycle) {
  Graph<int> G(4);
  G.addEdge(0, 1, 1); 
  G.addEdge(1, 2, 1); 
  G.addEdge(2, 3, 1); 
  G.addEdge(3, 1, -3); 
  std::vector<int> bestDistanceTo {0, -100, -100, -100};
  EXPECT_FALSE(allEdgesRelaxed(bestDistanceTo, G, 0));
}

TEST(EdgesRelaxedTest, dijkstraFail) {
  Graph<int> G(5);
  G.addEdge(0, 1, 4);
  G.addEdge(1, 2, -2);
  G.addEdge(2, 3, -2);
  G.addEdge(3, 4, 1);
  G.addEdge(0, 3, 2);
  std::vector<int> bestDistanceTo {0, 4, 2, 0, 3};
  EXPECT_FALSE(allEdgesRelaxed(bestDistanceTo, G, 0));
}

int main(int argc, char* argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
