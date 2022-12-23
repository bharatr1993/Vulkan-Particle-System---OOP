#include "first_app.hpp"

// std
#include <array>
#include <stdexcept>

namespace lve {

    int moveLeft = GLFW_KEY_LEFT;
    int moveRight = GLFW_KEY_RIGHT;
    int moveForward = GLFW_KEY_UP;
    int moveBackward = GLFW_KEY_DOWN;

    playerControls currentMove = playerControls::None;

    void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {

        if (key == moveForward && action == GLFW_PRESS)
            currentMove =playerControls::moveUp;
        else if(key == moveForward && action == GLFW_RELEASE && currentMove==playerControls::moveUp)
            currentMove =playerControls::None;
        else if(key == moveBackward && action == GLFW_PRESS)
            currentMove = playerControls::moveDown;
        else if(key == moveBackward && action == GLFW_RELEASE && currentMove==playerControls::moveDown)
            currentMove = playerControls::None;

    }


FirstApp::FirstApp() {


    lveRenderer = std::make_unique<LveRenderer>(lveDevice, lveWindow, lveSwapChain, vertPath, fragPath, compPath,bmpPath, vertices,objPath);
    glfwSetKeyCallback(lveWindow.window,keyCallback);
    lveRenderer->transform.projectionMatrix = glm::perspective( glm::radians(60.),(double) lveWindow.getAspectRatio(), 0.1, 100. );
    lveRenderer->transform.projectionMatrix *= glm::lookAt(  glm::vec3(0.0f,0.0f,3.0f),  glm::vec3(0.0f,0.0f,0.0f),  glm::vec3(0.0f,1.0f,0.0f) );
    lveRenderer->lveComputePipeline->prepareMiscBuffer(100,fElapsedTime);

    //lveRenderer->createComputeCommandBuffer();//Just for Testing!!!Remove
}

FirstApp::~FirstApp() {}

void FirstApp::run() {
  while (!lveWindow.shouldClose()) {
       std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
       renderScene();
       lveRenderer->lveComputePipeline->updateComputeDescriptor();
       lveRenderer->createCommandBuffers(pipelineNames);
       lveRenderer->createComputeCommandBuffer();
       lveRenderer->drawFrame();
       glfwPollEvents();
       std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
       std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
       fElapsedTime=time_span.count();
  }

  vkDeviceWaitIdle(lveDevice.device());
}

void FirstApp::renderScene() {

        double xpos, ypos;
        glfwGetCursorPos(lveWindow.window, &xpos, &ypos);

        xpos-=400;
        ypos-=300;

        xpos=xpos/400.0f;
        ypos=ypos/300.0f;

        xpos = clamp(xpos,1,-1);
        ypos = clamp(ypos,1,-1);

        lveRenderer->lveComputePipeline->miscInfo.mousePos=glm::vec2(xpos,ypos);


        lveRenderer->lveComputePipeline->commitMiscBuffer(fElapsedTime);


        bMat.translation = {0.0f,0.0f,-10.75f};
        bMat.rotation = {0.0f,0.0f,glm::pi<float>()};
        bMat.scale={10.5f,10.5f,10.5f};

        tfMat.translation = {0.0f,-1.0f,0.0f};
        tfMat.rotation = {0.0f,0.0f,0.0f};
        tfMat.scale={0.0025,0.0025,0.0025};

        lveRenderer->transform.transform = tfMat.mat4();
        lveRenderer->transform.bckgTransform=bMat.mat4();
        lveRenderer->transform.scale = glm::vec2(tfMat.scale.x, tfMat.scale.y);
        lveRenderer->transform.normalMatrix = glm::mat4(glm::inverseTranspose(glm::mat3(lveRenderer->transform.transform)));

        for (auto& bufferPtr : lveRenderer->lveBufferPtrVec)
        {
            bufferPtr->writeToBuffer((void*)&lveRenderer->transform, sizeof(lveRenderer->transform));

        }

        lveRenderer->descriptorSet->updateTransformDescriptorSet();



}

}  // namespace lve