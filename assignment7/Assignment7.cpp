#include "assignment7/Assignment7.h"
#include "common/core.h"

std::shared_ptr<Camera> Assignment7::CreateCamera() const
{
    const glm::vec2 resolution = GetImageOutputResolution();
    std::shared_ptr<Camera> camera = std::make_shared<PerspectiveCamera>(resolution.x / resolution.y, 26.6f);
    camera->SetPosition(glm::vec3(0.f, -4.1469f, 0.73693f));
    camera->Rotate(glm::vec3(1.f, 0.f, 0.f), PI / 2.f);
    
    
//    //camera->Translate(glm::vec3(0.f, -90.f, 0.f)); //far
//    camera->Translate(glm::vec3(0.f, -2.f, 3.f)); //near
//    camera->Rotate(glm::vec3(1.f, 0.f, 0.f), -PI / 8.f);
//    //camera->Rotate(glm::vec3(0.f, 0.f, 1.f), PI / 10.f);
 
    
    //from right
    camera->Translate(glm::vec3(13.f, 8.f, 4.f)); //near
    camera->Rotate(glm::vec3(1.f, 0.f, 0.f), - PI / 10.f);
    camera->Rotate(glm::vec3(0.f, 0.f, 1.f), PI / 4.f);


    return camera;
}


// Assignment 7 Part 1 TODO: Change the '1' here.
// 0 -- Naive.
// 1 -- BVH.
// 2 -- Grid.
#define ACCELERATION_TYPE 2

                             
std::shared_ptr<Scene> Assignment7::CreateScene() const
{
    std::shared_ptr<Scene> newScene = std::make_shared<Scene>();
    
    // Objects
    std::vector<std::shared_ptr<aiMaterial>> loadedMaterials;
    std::vector<std::shared_ptr<MeshObject>> cubeObjects = MeshLoader::LoadMesh("metal.obj", &loadedMaterials);
    std::vector<std::shared_ptr<aiMaterial>> loadedMaterials2;
    std::vector<std::shared_ptr<MeshObject>> cubeObjects2 = MeshLoader::LoadMesh("woods.obj", &loadedMaterials2);
    std::vector<std::shared_ptr<aiMaterial>> loadedMaterials3;
    std::vector<std::shared_ptr<MeshObject>> cubeObjects3 = MeshLoader::LoadMesh("box.obj", &loadedMaterials3);
    std::vector<std::shared_ptr<aiMaterial>> waterDrop1Material;
    //std::vector<std::shared_ptr<MeshObject>> waterDrop1Obj = MeshLoader::LoadMesh("waterDrops.obj", &waterDrop1Material);
    std::vector<std::shared_ptr<MeshObject>> waterDrop1Obj = MeshLoader::LoadMesh("water3.obj", &waterDrop1Material);
    std::vector<std::shared_ptr<aiMaterial>> loadedDuckMaterial;
    std::vector<std::shared_ptr<MeshObject>> duckObj = MeshLoader::LoadMesh("duck.obj", &loadedDuckMaterial);
    
    //metal:
    for (size_t i = 0; i < cubeObjects.size(); ++i) {
   
        std::shared_ptr<BlinnPhongMaterial> waterTapMaterial = std::make_shared<BlinnPhongMaterial>();
        
        waterTapMaterial->LoadMaterialFromAssimp(loadedMaterials[i]);
        cubeObjects[i]->SetMaterial(waterTapMaterial);
        
        waterTapMaterial->SetDiffuse(glm::vec3(.2f, 0.2f, 0.2f));
        //waterTapMaterial->SetAmbient(glm::vec3(1.f, 1.f, 1.f));
        waterTapMaterial->SetSpecular(glm::vec3(0.8f, 0.8f, 0.8f), 40.f);
        waterTapMaterial->SetReflectivity(0.5f);
        
        
        std::shared_ptr<SceneObject> cubeSceneObject = std::make_shared<SceneObject>();
        cubeSceneObject->AddMeshObject(cubeObjects[i]);
        cubeSceneObject->MultScale(0.01);
        cubeSceneObject->Rotate(glm::vec3(1.f, 0.f, 0.f), PI / 2.f);
        cubeSceneObject->Translate(glm::vec3(0.f, 10.0f, -13.f)); //
        //cubeSceneObject->Rotate(glm::vec3(1.f, 0.f, 0.f), PI / 15.f);

        cubeSceneObject->CreateAccelerationData(AccelerationTypes::BVH);
        cubeSceneObject->ConfigureAccelerationStructure([](AccelerationStructure* genericAccelerator) {
            BVHAcceleration* accelerator = dynamic_cast<BVHAcceleration*>(genericAccelerator);
            accelerator->SetMaximumChildren(2);
            accelerator->SetNodesOnLeaves(2);
        });

        cubeSceneObject->ConfigureChildMeshAccelerationStructure([](AccelerationStructure* genericAccelerator) {
            BVHAcceleration* accelerator = dynamic_cast<BVHAcceleration*>(genericAccelerator);
            accelerator->SetMaximumChildren(2);
            accelerator->SetNodesOnLeaves(2);
        });
        newScene->AddSceneObject(cubeSceneObject);
    }
    
    //woods
    for (size_t i = 0; i < cubeObjects2.size(); ++i) {
        std::shared_ptr<BlinnPhongMaterial> basinMaterial = std::make_shared<BlinnPhongMaterial>();
        //std::shared_ptr<Material> materialCopy2 = basinMaterial->Clone();
        basinMaterial->LoadMaterialFromAssimp(loadedMaterials2[i]);
        basinMaterial->SetDiffuse(glm::vec3(.4f, .4f, .4f));
        //basinMaterial->SetAmbient(glm::vec3(.3f, .3f, .3f));
        basinMaterial->SetSpecular(glm::vec3(0.3f, 0.3f, 0.3f), 40.f);
        basinMaterial->SetReflectivity(0.f);
         cubeObjects2[i]->SetMaterial(basinMaterial);
        
        std::shared_ptr<SceneObject> cubeSceneObject = std::make_shared<SceneObject>();
        cubeSceneObject->AddMeshObject(cubeObjects2[i]);
        cubeSceneObject->MultScale(0.01);
        cubeSceneObject->Rotate(glm::vec3(1.f, 0.f, 0.f), PI / 2.f);
        cubeSceneObject->Translate(glm::vec3(0.f, 10.0f, -13.f)); //z:front - back +
        //cubeSceneObject->Rotate(glm::vec3(1.f, 0.f, 0.f), PI / 15.f);
        
        cubeSceneObject->CreateAccelerationData(AccelerationTypes::BVH);
        cubeSceneObject->ConfigureAccelerationStructure([](AccelerationStructure* genericAccelerator) {
            BVHAcceleration* accelerator = dynamic_cast<BVHAcceleration*>(genericAccelerator);
            accelerator->SetMaximumChildren(2);
            accelerator->SetNodesOnLeaves(2);
        });
        
        cubeSceneObject->ConfigureChildMeshAccelerationStructure([](AccelerationStructure* genericAccelerator) {
            BVHAcceleration* accelerator = dynamic_cast<BVHAcceleration*>(genericAccelerator);
            accelerator->SetMaximumChildren(2);
            accelerator->SetNodesOnLeaves(2);
        });
        newScene->AddSceneObject(cubeSceneObject);
    }
    
    //plane
    for (size_t i = 0; i < cubeObjects3.size(); ++i) {
        std::shared_ptr<BlinnPhongMaterial> boxMaterial = std::make_shared<BlinnPhongMaterial>();
        boxMaterial->LoadMaterialFromAssimp(loadedMaterials3[i]);
        
        boxMaterial->SetDiffuse(glm::vec3(0.4f, 0.4f, 0.4f));
        //boxMaterial->SetAmbient(glm::vec3(0.4, 0.4, 0.4));
        boxMaterial->SetSpecular(glm::vec3(0.4f, 0.4f, 0.4f), 40.f);
        boxMaterial->SetReflectivity(0.3f);
        
        cubeObjects3[i]->SetMaterial(boxMaterial);
        
        std::shared_ptr<SceneObject> cubeSceneObject = std::make_shared<SceneObject>();
        cubeSceneObject->AddMeshObject(cubeObjects3[i]);
        cubeSceneObject->MultScale(0.01);
        cubeSceneObject->Rotate(glm::vec3(1.f, 0.f, 0.f), PI / 2.f);
        cubeSceneObject->Translate(glm::vec3(0.f, 10.0f, -13.f)); //z:front - back +


        cubeSceneObject->CreateAccelerationData(AccelerationTypes::BVH);
        cubeSceneObject->ConfigureAccelerationStructure([](AccelerationStructure* genericAccelerator) {
            BVHAcceleration* accelerator = dynamic_cast<BVHAcceleration*>(genericAccelerator);
            accelerator->SetMaximumChildren(2);
            accelerator->SetNodesOnLeaves(2);
        });

        cubeSceneObject->ConfigureChildMeshAccelerationStructure([](AccelerationStructure* genericAccelerator) {
            BVHAcceleration* accelerator = dynamic_cast<BVHAcceleration*>(genericAccelerator);
            accelerator->SetMaximumChildren(2);
            accelerator->SetNodesOnLeaves(2);
        });
        newScene->AddSceneObject(cubeSceneObject);
    }
    
    //waterDrop1
    for (size_t i = 0; i < waterDrop1Obj.size(); ++i) {
        std::shared_ptr<BlinnPhongMaterial> waterDrop1Mt = std::make_shared<BlinnPhongMaterial>();
        waterDrop1Mt->LoadMaterialFromAssimp(waterDrop1Material[i]);
        waterDrop1Mt->SetDiffuse(glm::vec3(0.f, 0.5f, 0.5f));
        waterDrop1Mt->SetAmbient(glm::vec3(0.0, 0.0f, 0.0f));
        waterDrop1Mt->SetSpecular(glm::vec3(0.8f, 0.8f, 0.8f), 40.f);
        waterDrop1Mt->SetReflectivity(0.8f);
        waterDrop1Mt->SetTransmittance(0.8f);
        waterDrop1Obj[i]->SetMaterial(waterDrop1Mt);

        std::shared_ptr<SceneObject> cubeSceneObject = std::make_shared<SceneObject>();
        cubeSceneObject->AddMeshObject(waterDrop1Obj[i]);
        cubeSceneObject->MultScale(0.01);
        cubeSceneObject->Rotate(glm::vec3(1.f, 0.f, 0.f), PI / 2.f);
        cubeSceneObject->Translate(glm::vec3(0.f, 10.0f, -13.f)); //z:front - back +
        //cubeSceneObject->Rotate(glm::vec3(1.f, 0.f, 0.f), PI / 15.f);

        cubeSceneObject->CreateAccelerationData(AccelerationTypes::BVH);
        cubeSceneObject->ConfigureAccelerationStructure([](AccelerationStructure* genericAccelerator) {
            BVHAcceleration* accelerator = dynamic_cast<BVHAcceleration*>(genericAccelerator);
            accelerator->SetMaximumChildren(2);
            accelerator->SetNodesOnLeaves(2);
        });

        cubeSceneObject->ConfigureChildMeshAccelerationStructure([](AccelerationStructure* genericAccelerator) {
            BVHAcceleration* accelerator = dynamic_cast<BVHAcceleration*>(genericAccelerator);
            accelerator->SetMaximumChildren(2);
            accelerator->SetNodesOnLeaves(2);
        });
        newScene->AddSceneObject(cubeSceneObject);
    }
    
    for (size_t i = 0; i < duckObj.size(); ++i) {
        std::shared_ptr<BlinnPhongMaterial> duckMt = std::make_shared<BlinnPhongMaterial>();
        duckMt->LoadMaterialFromAssimp(loadedDuckMaterial[i]);
        duckMt->SetDiffuse(glm::vec3(0.f, 0.0f, 0.0f));
        duckMt->SetAmbient(glm::vec3(0.0, 0.0f, 0.0f));
        duckMt->SetSpecular(glm::vec3(0.0f, 0.0f, 0.0f), 40.f);
        duckMt->SetReflectivity(0.f);
        duckMt->SetTransmittance(0.f);
                duckObj[i]->SetMaterial(duckMt);
        
        std::shared_ptr<SceneObject> cubeSceneObject = std::make_shared<SceneObject>();
        cubeSceneObject->AddMeshObject(duckObj[i]);
        //cubeSceneObject->MultScale(0.01);
        cubeSceneObject->Rotate(glm::vec3(1.f, 0.f, 0.f), PI / 2.f);
        cubeSceneObject->Translate(glm::vec3(0.4f, 16.0f, -4.3f)); //z:front - back +
        cubeSceneObject->Rotate(glm::vec3(0.f, 0.f, 1.f), PI);
        
        cubeSceneObject->CreateAccelerationData(AccelerationTypes::BVH);
        cubeSceneObject->ConfigureAccelerationStructure([](AccelerationStructure* genericAccelerator) {
            BVHAcceleration* accelerator = dynamic_cast<BVHAcceleration*>(genericAccelerator);
            accelerator->SetMaximumChildren(2);
            accelerator->SetNodesOnLeaves(2);
        });
        
        cubeSceneObject->ConfigureChildMeshAccelerationStructure([](AccelerationStructure* genericAccelerator) {
            BVHAcceleration* accelerator = dynamic_cast<BVHAcceleration*>(genericAccelerator);
            accelerator->SetMaximumChildren(2);
            accelerator->SetNodesOnLeaves(2);
        });
        newScene->AddSceneObject(cubeSceneObject);
    }
    
    // Lights
    
    std::shared_ptr<Light> pointLight1 = std::make_shared<PointLight>();
    pointLight1->SetPosition(glm::vec3(50.f, -100.0101f, 10.f));
    pointLight1->SetLightColor(glm::vec3(.5f, .5f, .5f));
    
    std::shared_ptr<Light> pointLight2 = std::make_shared<PointLight>();
    pointLight2->SetPosition(glm::vec3(5.,-30., 10));
    pointLight2->SetLightColor(glm::vec3(0.5f, 0.5f, 0.5f));
    
    std::shared_ptr<Light> pointLight3 = std::make_shared<PointLight>();
    pointLight3->SetPosition(glm::vec3(10, 0, 10.f));
    pointLight3->SetLightColor(glm::vec3(0.5f, 0.5f, 0.5f));
    
    //inside the basin
    std::shared_ptr<Light> pointLight4 = std::make_shared<PointLight>();
    pointLight4->SetPosition(glm::vec3(-50.f,22.0101f, 10.f));
    pointLight4->SetLightColor(glm::vec3(.5f, .5f, .5f));
    
    std::shared_ptr<Light> pointLight5 = std::make_shared<PointLight>();
    pointLight5->SetPosition(glm::vec3(50.f,20.0101f, 10.f));
    pointLight5->SetLightColor(glm::vec3(.5f, .5f, .5f));
    
    
#if ACCELERATION_TYPE == 0
    newScene->GenerateAccelerationData(AccelerationTypes::NONE);
#elif ACCELERATION_TYPE == 1
    newScene->GenerateAccelerationData(AccelerationTypes::BVH);
#else
    UniformGridAcceleration* accelerator = dynamic_cast<UniformGridAcceleration*>(newScene->GenerateAccelerationData(AccelerationTypes::UNIFORM_GRID));
    assert(accelerator);
    // Assignment 7 Part 2 TODO: Change the glm::ivec3(10, 10, 10) here.
    accelerator->SetSuggestedGridSize(glm::ivec3(3, 3, 3));
#endif
    //newScene->AddLight(pointLight1);
    newScene->AddLight(pointLight2);
    newScene->AddLight(pointLight3);
    newScene->AddLight(pointLight4);
    newScene->AddLight(pointLight5);
    return newScene;
    
}
std::shared_ptr<ColorSampler> Assignment7::CreateSampler() const
{
    std::shared_ptr<JitterColorSampler> jitter = std::make_shared<JitterColorSampler>();
    jitter->SetGridSize(glm::ivec3(1, 1, 1));
    return jitter;
}

std::shared_ptr<class Renderer> Assignment7::CreateRenderer(std::shared_ptr<Scene> scene, std::shared_ptr<ColorSampler> sampler) const
{
    return std::make_shared<BackwardRenderer>(scene, sampler);
}

int Assignment7::GetSamplesPerPixel() const
{
    return 1;
}

bool Assignment7::NotifyNewPixelSample(glm::vec3 inputSampleColor, int sampleIndex)
{
    return true;
}

int Assignment7::GetMaxReflectionBounces() const
{
    return 2;
}

int Assignment7::GetMaxRefractionBounces() const
{
    return 4;
}

glm::vec2 Assignment7::GetImageOutputResolution() const
{
    //return glm::vec2(1920.f, 1080.f);
    return glm::vec2(640.f, 480.f);
}

