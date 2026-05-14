#include "Mesh.h"
#include "Scene.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <gtest/gtest.h>
#include <vector>

class SceneTest : public ::testing::Test {
protected:
  GLFWwindow *window;

  void SetUp() override {
    // Initialize GLFW for a headless OpenGL context
    ASSERT_TRUE(glfwInit()) << "Failed to initialize GLFW";

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE); // Hidden window

    window = glfwCreateWindow(800, 600, "Test Window", nullptr, nullptr);
    ASSERT_NE(window, nullptr) << "Failed to create GLFW window";

    glfwMakeContextCurrent(window);

    // Initialize GLAD
    ASSERT_TRUE(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        << "Failed to initialize GLAD";
  }

  void TearDown() override {
    if (window) {
      glfwDestroyWindow(window);
    }
    glfwTerminate();
  }
};

TEST_F(SceneTest, AddMesh) {
  Scene scene(glm::mat4(1.0f), glm::mat4(1.0f));

  std::vector<float> vertices = {0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
                                 0.0f, 0.0f, 1.0f, 0.0f};
  std::vector<int> faces = {0, 1, 2};

  auto mesh = std::make_unique<Mesh>(&vertices, &faces);

  // Add mesh to scene
  scene.addMesh(std::move(mesh), "TestTriangle", glm::mat4(1.0f));

  // Verify scene counts
  EXPECT_EQ(scene.getMeshes().size(), 1);
  EXPECT_EQ(scene.getMeshName(scene.getMeshes()[0].get()), "TestTriangle");
}
