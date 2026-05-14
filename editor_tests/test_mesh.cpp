#include <gtest/gtest.h>
#include "Mesh.h"
#include <vector>
#include <glm/glm.hpp>

TEST(MeshTest, SingleTriangleInitialization) {
    std::vector<float> vertices = {
        0.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f
    };
    std::vector<int> faces = {
        0, 1, 2
    };

    Mesh mesh(&vertices, &faces);

    // Assert basic counts
    EXPECT_EQ(mesh.getVertices().size(), 3);
    EXPECT_EQ(mesh.getFaces().size(), 1);
    EXPECT_EQ(mesh.getHalfEdges().size(), 3);
    EXPECT_EQ(mesh.getEdges().size(), 3);

    // Verify face normals (should be roughly +Z if counter-clockwise)
    // 1-0 = (1,0,0)
    // 2-0 = (0,1,0)
    // Cross = (0,0,1)
    glm::vec3 normal = mesh.getFaces()[0].normal;
    EXPECT_FLOAT_EQ(normal.x, 0.0f);
    EXPECT_FLOAT_EQ(normal.y, 0.0f);
    EXPECT_FLOAT_EQ(normal.z, 1.0f);

    // Verify vertex normals
    for (const auto& v : mesh.getVertices()) {
        EXPECT_FLOAT_EQ(v.normal.x, 0.0f);
        EXPECT_FLOAT_EQ(v.normal.y, 0.0f);
        EXPECT_FLOAT_EQ(v.normal.z, 1.0f);
    }
}

TEST(MeshTest, SharedEdgeTwins) {
    // Two triangles sharing an edge (0-1)
    // Tri 1: 0, 1, 2
    // Tri 2: 1, 0, 3
    std::vector<float> vertices = {
        0.0f, 0.0f, 0.0f, // 0
        1.0f, 0.0f, 0.0f, // 1
        0.0f, 1.0f, 0.0f, // 2
        1.0f, -1.0f, 0.0f // 3
    };
    std::vector<int> faces = {
        0, 1, 2,
        1, 0, 3
    };

    Mesh mesh(&vertices, &faces);

    EXPECT_EQ(mesh.getVertices().size(), 4);
    EXPECT_EQ(mesh.getFaces().size(), 2);
    
    // Total half-edges should be 6
    EXPECT_EQ(mesh.getHalfEdges().size(), 6);
    
    // Shared edge 0-1 and 1-0 should merge, so 5 edges total
    EXPECT_EQ(mesh.getEdges().size(), 5);

    // Find the twin for halfedges
    int twin_found_count = 0;
    for (const auto& he : mesh.getHalfEdges()) {
        if (he.twin != -1) {
            twin_found_count++;
        }
    }
    // Only the shared edge should have twins (2 half-edges have a twin)
    EXPECT_EQ(twin_found_count, 2);
}
