#include "mesh.h"

#include "core/logger.h"

namespace rain
{
    void Mesh::load(const std::string& _path)
    {
        fbxsdk::FbxManager* sdkManager = fbxsdk::FbxManager::Create();
        fbxsdk::FbxIOSettings *ios = fbxsdk::FbxIOSettings::Create(sdkManager, IOSROOT);
        sdkManager->SetIOSettings(ios);

        fbxsdk::FbxImporter* importer = fbxsdk::FbxImporter::Create(sdkManager, "");

        if (!importer->Initialize(_path.c_str(), -1, sdkManager->GetIOSettings()))
        {
            RAIN_LOG("Call to fbxsdk::FbxImporter::Initialize() failed.\n");
            RAIN_LOG("Error returned: %s\n\n", importer->GetStatus().GetErrorString());
            return;
        }

        fbxsdk::FbxScene* scene = fbxsdk::FbxScene::Create(sdkManager, "scene");

        importer->Import(scene);
        importer->Destroy();

        fbxsdk::FbxNode* rootNode = scene->GetRootNode();
        if (rootNode)
        {
            for (i32 i = 0; i < rootNode->GetChildCount(); i++)
            {
                fbxsdk::FbxNode* childNode = rootNode->GetChild(i);

                fbxsdk::FbxNodeAttribute::EType attributeType;

                if (childNode->GetNodeAttribute() == NULL)
                {
                    FBXSDK_printf("NULL Node Attribute\n\n");
                }
                else
                {
                    attributeType = (childNode->GetNodeAttribute()->GetAttributeType());

                    switch (attributeType)
                    {
                    //case fbxsdk::FbxNodeAttribute::eSkeleton:
                    //    display_skeleton(childNode, "");
                    //    break;
                    case fbxsdk::FbxNodeAttribute::eMesh:
                        fbxsdk::FbxMesh* fbxMesh = (fbxsdk::FbxMesh*)childNode->GetNodeAttribute();
                        read_mesh_fbx(fbxMesh, this);
                        //display_link(fbxMesh);
                        break;
                    }
                }
            }
        }

        sdkManager->Destroy();
    }

    void display_metadata_connection(FbxObject* pObject)
    {
        int nbMetaData = pObject->GetSrcObjectCount<FbxObjectMetaData>();
        if (nbMetaData > 0)
            RAIN_LOG_RAW("    MetaData connections\n");

        for (int i = 0; i < nbMetaData; i++)
        {
            FbxObjectMetaData* metaData = pObject->GetSrcObject<FbxObjectMetaData>(i);
            RAIN_LOG_RAW("        Name: %s\n", (char*)metaData->GetName());
        }
    }

    void display_skeleton(FbxNode* pNode, std::string tab)
    {
        FbxSkeleton* lSkeleton = (FbxSkeleton*)pNode->GetNodeAttribute();

        RAIN_LOG_RAW("%sSkeleton Name: %s\n", tab.c_str(), (char *)pNode->GetName());
        display_metadata_connection(lSkeleton);

        const char* lSkeletonTypes[] = { "Root", "Limb", "Limb Node", "Effector" };

        RAIN_LOG_RAW("%s    Type: %s\n", tab.c_str(), lSkeletonTypes[lSkeleton->GetSkeletonType()]);

        if (lSkeleton->GetSkeletonType() == FbxSkeleton::eLimb)
        {
            RAIN_LOG_RAW("%s    Limb Length: %f\n", tab.c_str(), lSkeleton->LimbLength.Get());
        }
        else if (lSkeleton->GetSkeletonType() == FbxSkeleton::eLimbNode)
        {
            RAIN_LOG_RAW("%s    Limb Node Size: %f\n", tab.c_str(), lSkeleton->Size.Get());
        }
        else if (lSkeleton->GetSkeletonType() == FbxSkeleton::eRoot)
        {
            RAIN_LOG_RAW("%s    Limb Root Size: %f\n", tab.c_str(), lSkeleton->Size.Get());
        }

        for (int i = 0; i < pNode->GetChildCount(); ++i)
        {
            fbxsdk::FbxNode* child = pNode->GetChild(i);
            if (child->GetNodeAttribute()->GetAttributeType() == fbxsdk::FbxNodeAttribute::eSkeleton)
            {
                display_skeleton(pNode->GetChild(i), tab + "\t");
            }
        }
    }

    void display_link(FbxGeometry* pGeometry)
    {
        //Display cluster now

        //int i, lLinkCount;
        //FbxCluster* lLink;

        int i, j;
        int lSkinCount = 0;
        int lClusterCount = 0;
        FbxCluster* lCluster;

        lSkinCount = pGeometry->GetDeformerCount(FbxDeformer::eSkin);

        //lLinkCount = pGeometry->GetLinkCount();
        for (i = 0; i != lSkinCount; ++i)
        {
            lClusterCount = ((FbxSkin *)pGeometry->GetDeformer(i, FbxDeformer::eSkin))->GetClusterCount();
            for (j = 0; j != lClusterCount; ++j)
            {
                RAIN_LOG_RAW("    Cluster %d\n", i);

                lCluster = ((FbxSkin *)pGeometry->GetDeformer(i, FbxDeformer::eSkin))->GetCluster(j);
                //lLink = pGeometry->GetLink(i);    

                const char* lClusterModes[] = { "Normalize", "Additive", "Total1" };

                RAIN_LOG_RAW("    Mode: %s\n", lClusterModes[lCluster->GetLinkMode()]);

                if (lCluster->GetLink() != NULL)
                {
                    RAIN_LOG_RAW("        Name: %s\n", (char *)lCluster->GetLink()->GetName());
                }

                FbxString lString1 = "        Link Indices: ";
                FbxString lString2 = "        Weight Values: ";

                int k, lIndexCount = lCluster->GetControlPointIndicesCount();
                int* lIndices = lCluster->GetControlPointIndices();
                double* lWeights = lCluster->GetControlPointWeights();

                for (k = 0; k < lIndexCount; k++)
                {
                    lString1 += lIndices[k];
                    lString2 += (float)lWeights[k];

                    if (k < lIndexCount - 1)
                    {
                        lString1 += ", ";
                        lString2 += ", ";
                    }
                }

                lString1 += "\n";
                lString2 += "\n";

                FBXSDK_printf(lString1);
                FBXSDK_printf(lString2);

                RAIN_LOG_RAW("\n");

                FbxAMatrix lMatrix;

                lMatrix = lCluster->GetTransformMatrix(lMatrix);
                RAIN_LOG_RAW("        Transform Translation: (%f, %f, %f)\n", lMatrix.GetT()[0], lMatrix.GetT()[1], lMatrix.GetT()[2]);
                RAIN_LOG_RAW("        Transform Rotation: (%f, %f, %f)\n", lMatrix.GetR()[0], lMatrix.GetR()[1], lMatrix.GetR()[2]);
                RAIN_LOG_RAW("        Transform Scaling: (%f, %f, %f)\n", lMatrix.GetS()[0], lMatrix.GetS()[1], lMatrix.GetS()[2]);

                lMatrix = lCluster->GetTransformLinkMatrix(lMatrix);
                RAIN_LOG_RAW("        Transform Link Translation: (%f, %f, %f)\n", lMatrix.GetT()[0], lMatrix.GetT()[1], lMatrix.GetT()[2]);
                RAIN_LOG_RAW("        Transform Link Rotation: (%f, %f, %f)\n", lMatrix.GetR()[0], lMatrix.GetR()[1], lMatrix.GetR()[2]);
                RAIN_LOG_RAW("        Transform Link Scaling: (%f, %f, %f)\n", lMatrix.GetS()[0], lMatrix.GetS()[1], lMatrix.GetS()[2]);

                if (lCluster->GetAssociateModel() != NULL)
                {
                    lMatrix = lCluster->GetTransformAssociateModelMatrix(lMatrix);
                    RAIN_LOG_RAW("        Associate Model: %s\n", (char *)lCluster->GetAssociateModel()->GetName());
                    RAIN_LOG_RAW("        Associate Model Translation: (%f, %f, %f)\n", lMatrix.GetT()[0], lMatrix.GetT()[1], lMatrix.GetT()[2]);
                    RAIN_LOG_RAW("        Associate Model Rotation: (%f, %f, %f)\n", lMatrix.GetR()[0], lMatrix.GetR()[1], lMatrix.GetR()[2]);
                    RAIN_LOG_RAW("        Associate Model Scaling: (%f, %f, %f)\n", lMatrix.GetS()[0], lMatrix.GetS()[1], lMatrix.GetS()[2]);
                }

                RAIN_LOG_RAW("\n");
            }
        }
    }



    void read_mesh_fbx(fbxsdk::FbxMesh* _fbxMesh, Mesh* _mesh)
    {
        i32 polygonCount = _fbxMesh->GetPolygonCount();
        fbxsdk::FbxVector4* bfxVertices = _fbxMesh->GetControlPoints();
        _mesh->vertices.resize(_fbxMesh->GetControlPointsCount());

        u32 vertexId = 0;
        for (i32 i = 0; i < polygonCount; i++)
        {
            int lPolygonSize = _fbxMesh->GetPolygonSize(i);

            for (i32 j = 0; j < lPolygonSize; j++)
            {
                int vertexIndex = _fbxMesh->GetPolygonVertex(i, j);
                _mesh->indices.push_back(vertexIndex);
                _mesh->vertices[vertexIndex].position = glm::vec3(bfxVertices[vertexIndex][0], bfxVertices[vertexIndex][2], -bfxVertices[vertexIndex][1]);
                
                for (i32 binormalIndex = 0; binormalIndex < _fbxMesh->GetElementUVCount(); ++binormalIndex)
                {
                    fbxsdk::FbxGeometryElementUV* currentUV = _fbxMesh->GetElementUV(binormalIndex);

                    switch (currentUV->GetMappingMode())
                    {
                    default:
                        break;
                    case fbxsdk::FbxGeometryElement::eByPolygonVertex:
                    {
                        int textureUVIndex = _fbxMesh->GetTextureUVIndex(i, j);
                        switch (currentUV->GetReferenceMode())
                        {
                        case fbxsdk::FbxGeometryElement::eDirect:
                        case fbxsdk::FbxGeometryElement::eIndexToDirect:
                        {
                            _mesh->vertices[vertexIndex].texCoords[binormalIndex].x = f32(currentUV->GetDirectArray().GetAt(textureUVIndex)[0]);
                            _mesh->vertices[vertexIndex].texCoords[binormalIndex].y = f32(currentUV->GetDirectArray().GetAt(textureUVIndex)[1]);
                        }
                        break;
                        default:
                            break;
                        }
                    }
                    break;

                    case fbxsdk::FbxGeometryElement::eByPolygon: // doesn't make much sense for UVs
                    case fbxsdk::FbxGeometryElement::eAllSame:   // doesn't make much sense for UVs
                    case fbxsdk::FbxGeometryElement::eNone:       // doesn't make much sense for UVs
                        break;
                    }
                }
                for (i32 binormalIndex = 0; binormalIndex < _fbxMesh->GetElementNormalCount(); ++binormalIndex)
                {
                    fbxsdk::FbxGeometryElementNormal* currentNormal = _fbxMesh->GetElementNormal(binormalIndex);

                    if (currentNormal->GetMappingMode() == fbxsdk::FbxGeometryElement::eByPolygonVertex)
                    {
                        switch (currentNormal->GetReferenceMode())
                        {
                        case fbxsdk::FbxGeometryElement::eDirect:
                            _mesh->vertices[vertexIndex].normal = glm::vec3
                            (
                                currentNormal->GetDirectArray().GetAt(vertexId)[0],
                                currentNormal->GetDirectArray().GetAt(vertexId)[1],
                                currentNormal->GetDirectArray().GetAt(vertexId)[2]
                            );
                            break;
                        case fbxsdk::FbxGeometryElement::eIndexToDirect:
                        {
                            int id = currentNormal->GetIndexArray().GetAt(vertexId);
                            _mesh->vertices[vertexIndex].normal = glm::vec3
                            (
                                currentNormal->GetDirectArray().GetAt(id)[0],
                                currentNormal->GetDirectArray().GetAt(id)[1],
                                currentNormal->GetDirectArray().GetAt(id)[2]
                            );
                        }
                        break;
                        default:
                            break; // other reference modes not shown here!
                        }
                    }
                }
                for (i32 binormalIndex = 0; binormalIndex < _fbxMesh->GetElementTangentCount(); ++binormalIndex)
                {
                    fbxsdk::FbxGeometryElementTangent* currentTangent = _fbxMesh->GetElementTangent(binormalIndex);

                    if (currentTangent->GetMappingMode() == fbxsdk::FbxGeometryElement::eByPolygonVertex)
                    {
                        switch (currentTangent->GetReferenceMode())
                        {
                        case fbxsdk::FbxGeometryElement::eDirect:
                            _mesh->vertices[vertexIndex].normal = glm::vec3
                            (
                                currentTangent->GetDirectArray().GetAt(vertexId)[0],
                                currentTangent->GetDirectArray().GetAt(vertexId)[1],
                                currentTangent->GetDirectArray().GetAt(vertexId)[2]
                            );
                            break;
                        case fbxsdk::FbxGeometryElement::eIndexToDirect:
                        {
                            int id = currentTangent->GetIndexArray().GetAt(vertexId);
                            _mesh->vertices[vertexIndex].normal = glm::vec3
                            (
                                currentTangent->GetDirectArray().GetAt(id)[0],
                                currentTangent->GetDirectArray().GetAt(id)[1],
                                currentTangent->GetDirectArray().GetAt(id)[2]
                            );
                        }
                        break;
                        default:
                            break; // other reference modes not shown here!
                        }
                    }

                }
                for (i32 binormalIndex = 0; binormalIndex < _fbxMesh->GetElementBinormalCount(); ++binormalIndex)
                {

                    fbxsdk::FbxGeometryElementBinormal* currentBinormal = _fbxMesh->GetElementBinormal(binormalIndex);

                    if (currentBinormal->GetMappingMode() == fbxsdk::FbxGeometryElement::eByPolygonVertex)
                    {
                        switch (currentBinormal->GetReferenceMode())
                        {
                        case fbxsdk::FbxGeometryElement::eDirect:
                            _mesh->vertices[vertexIndex].normal = glm::vec3
                            (
                                currentBinormal->GetDirectArray().GetAt(vertexId)[0],
                                currentBinormal->GetDirectArray().GetAt(vertexId)[1],
                                currentBinormal->GetDirectArray().GetAt(vertexId)[2]
                            );
                            break;
                        case fbxsdk::FbxGeometryElement::eIndexToDirect:
                        {
                            int id = currentBinormal->GetIndexArray().GetAt(vertexId);
                            _mesh->vertices[vertexIndex].normal = glm::vec3
                            (
                                currentBinormal->GetDirectArray().GetAt(id)[0],
                                currentBinormal->GetDirectArray().GetAt(id)[1],
                                currentBinormal->GetDirectArray().GetAt(id)[2]
                            );
                        }
                        break;
                        default:
                            break; // other reference modes not shown here!
                        }
                    }
                }
                vertexId++;
            } // for polygonSize
        } // for polygonCount
    }
}