#include "mesh.h"

#include "core/logger.h"

namespace rain
{
    void Mesh::load(const std::string& _path)
    {
        Scene scene;
        read_scene_fbx(_path, &scene);
        vertices = scene.meshes[0].vertices;
        indices = scene.meshes[0].indices;
    }

    void DisplayTextureInfo(fbxsdk::FbxTexture* pTexture, int pBlendMode)
    {
        fbxsdk::FbxFileTexture *lFileTexture = fbxsdk::FbxCast<fbxsdk::FbxFileTexture>(pTexture);
        fbxsdk::FbxProceduralTexture *lProceduralTexture = fbxsdk::FbxCast<fbxsdk::FbxProceduralTexture>(pTexture);

        RAIN_LOG_RAW("            Name: '%s'\n", (char *)pTexture->GetName());
        if (lFileTexture)
        {
            RAIN_LOG_RAW("            Type: File Texture\n");
            RAIN_LOG_RAW("            File Name: '%s\n", (char *)lFileTexture->GetFileName());
        }
        else if (lProceduralTexture)
        {
            RAIN_LOG_RAW("            Type: Procedural Texture\n");
        }
        RAIN_LOG_RAW("            Scale U: %f\n", pTexture->GetScaleU());
        RAIN_LOG_RAW("            Scale V: %f\n", pTexture->GetScaleV());
        RAIN_LOG_RAW("            Translation U: %f\n", pTexture->GetTranslationU());
        RAIN_LOG_RAW("            Translation V: %f\n", pTexture->GetTranslationV());
        RAIN_LOG_RAW("            Swap UV: %d\n", pTexture->GetSwapUV());
        RAIN_LOG_RAW("            Rotation U: %f\n", pTexture->GetRotationU());
        RAIN_LOG_RAW("            Rotation V: %f\n", pTexture->GetRotationV());
        RAIN_LOG_RAW("            Rotation W: %f\n", pTexture->GetRotationW());

        const char* lAlphaSources[] = { "None", "RGB Intensity", "Black" };

        RAIN_LOG_RAW("            Alpha Source: %s\n", lAlphaSources[pTexture->GetAlphaSource()]);
        RAIN_LOG_RAW("            Cropping Left: %f\n", pTexture->GetCroppingLeft());
        RAIN_LOG_RAW("            Cropping Top: %f\n", pTexture->GetCroppingTop());
        RAIN_LOG_RAW("            Cropping Right: %f\n", pTexture->GetCroppingRight());
        RAIN_LOG_RAW("            Cropping Bottom: %f\n", pTexture->GetCroppingBottom());

        const char* lMappingTypes[] = { "Null", "Planar", "Spherical", "Cylindrical",
            "Box", "Face", "UV", "Environment" };

        RAIN_LOG_RAW("            Mapping Type: %s\n", lMappingTypes[pTexture->GetMappingType()]);

        if (pTexture->GetMappingType() == fbxsdk::FbxTexture::ePlanar)
        {
            const char* lPlanarMappingNormals[] = { "X", "Y", "Z" };

            RAIN_LOG_RAW("            Planar Mapping Normal: %s\n", lPlanarMappingNormals[pTexture->GetPlanarMappingNormal()]);
        }

        const char* lBlendModes[] = { "Translucent", "Additive", "Modulate", "Modulate2", "Over", "Normal", "Dissolve", "Darken", "ColorBurn", "LinearBurn",
                                        "DarkerColor", "Lighten", "Screen", "ColorDodge", "LinearDodge", "LighterColor", "SoftLight", "HardLight", "VividLight",
                                        "LinearLight", "PinLight", "HardMix", "Difference", "Exclusion", "Substract", "Divide", "Hue", "Saturation", "Color",
                                        "Luminosity", "Overlay" };

        if (pBlendMode >= 0)
            RAIN_LOG_RAW("            Blend Mode: %s\n", lBlendModes[pBlendMode]);
        RAIN_LOG_RAW("            Alpha: %f\n", pTexture->GetDefaultAlpha());

        if (lFileTexture)
        {
            const char* lMaterialUses[] = { "Model Material", "Default Material" };
            RAIN_LOG_RAW("            Material Use: %s\n", lMaterialUses[lFileTexture->GetMaterialUse()]);
        }

        const char* pTextureUses[] = { "Standard", "Shadow Map", "Light Map",
            "Spherical Reflexion Map", "Sphere Reflexion Map", "Bump Normal Map" };

        RAIN_LOG_RAW("            Texture Use: %s\n", pTextureUses[pTexture->GetTextureUse()]);
        RAIN_LOG_RAW("\n");

    }

    void FindAndDisplayTextureInfoByProperty(FbxProperty pProperty, bool& pDisplayHeader, int pMaterialIndex)
    {

        if (pProperty.IsValid())
        {
            int lTextureCount = pProperty.GetSrcObjectCount<FbxTexture>();

            for (int j = 0; j < lTextureCount; ++j)
            {
                //Here we have to check if it's layeredtextures, or just textures:
                FbxLayeredTexture *lLayeredTexture = pProperty.GetSrcObject<FbxLayeredTexture>(j);
                if (lLayeredTexture)
                {
                    RAIN_LOG_RAW("    Layered Texture: %d\n", j);
                    int lNbTextures = lLayeredTexture->GetSrcObjectCount<FbxTexture>();
                    for (int k = 0; k < lNbTextures; ++k)
                    {
                        FbxTexture* lTexture = lLayeredTexture->GetSrcObject<FbxTexture>(k);
                        if (lTexture)
                        {

                            if (pDisplayHeader) {
                                RAIN_LOG_RAW("    Textures connected to Material ", pMaterialIndex);
                                pDisplayHeader = false;
                            }

                            //NOTE the blend mode is ALWAYS on the LayeredTexture and NOT the one on the texture.
                            //Why is that?  because one texture can be shared on different layered textures and might
                            //have different blend modes.

                            FbxLayeredTexture::EBlendMode lBlendMode;
                            lLayeredTexture->GetTextureBlendMode(k, lBlendMode);
                            RAIN_LOG_RAW("    Textures for %s\n", pProperty.GetName());
                            RAIN_LOG_RAW("        Texture %d\n", k);
                            DisplayTextureInfo(lTexture, (int)lBlendMode);
                        }

                    }
                }
                else
                {
                    //no layered texture simply get on the property
                    FbxTexture* lTexture = pProperty.GetSrcObject<FbxTexture>(j);
                    if (lTexture)
                    {
                        //display connected Material header only at the first time
                        if (pDisplayHeader) {
                            RAIN_LOG_RAW("    Textures connected to Material %d\n", pMaterialIndex);
                            pDisplayHeader = false;
                        }

                        RAIN_LOG_RAW("    Textures for %s\n", pProperty.GetName());
                        RAIN_LOG_RAW("        Texture %d\n", j);
                        DisplayTextureInfo(lTexture, -1);
                    }
                }
            }
        }//end if pProperty

    }

    void display_textures_info(fbxsdk::FbxGeometry* pGeometry)
    {
        int lMaterialIndex;
        FbxProperty lProperty;
        if (pGeometry->GetNode() == NULL)
            return;
        int lNbMat = pGeometry->GetNode()->GetSrcObjectCount<FbxSurfaceMaterial>();
        for (lMaterialIndex = 0; lMaterialIndex < lNbMat; lMaterialIndex++) {
            FbxSurfaceMaterial *lMaterial = pGeometry->GetNode()->GetSrcObject<FbxSurfaceMaterial>(lMaterialIndex);
            bool lDisplayHeader = true;

            //go through all the possible textures
            if (lMaterial) {

                int lTextureIndex;
                FBXSDK_FOR_EACH_TEXTURE(lTextureIndex)
                {
                    lProperty = lMaterial->FindProperty(FbxLayerElement::sTextureChannelNames[lTextureIndex]);
                    FindAndDisplayTextureInfoByProperty(lProperty, lDisplayHeader, lMaterialIndex);
                }

            }//end if(lMaterial)

        }// end for lMaterialIndex     
    }

    void read_scene_fbx(const std::string& _path, Scene* _scene)
    {
        const char* lFilename = _path.c_str();
        
        fbxsdk::FbxManager* lSdkManager = fbxsdk::FbxManager::Create();
        fbxsdk::FbxIOSettings *ios = fbxsdk::FbxIOSettings::Create(lSdkManager, IOSROOT);
        lSdkManager->SetIOSettings(ios);

        fbxsdk::FbxImporter* lImporter = fbxsdk::FbxImporter::Create(lSdkManager, "");

        if (!lImporter->Initialize(lFilename, -1, lSdkManager->GetIOSettings()))
        {
            RAIN_LOG("Call to fbxsdk::FbxImporter::Initialize() failed.\n");
            RAIN_LOG("Error returned: %s\n\n", lImporter->GetStatus().GetErrorString());
            return;
        }

        fbxsdk::FbxScene* lScene = fbxsdk::FbxScene::Create(lSdkManager, "scene");

        lImporter->Import(lScene);
        lImporter->Destroy();

        fbxsdk::FbxNode* FbxRootNode = lScene->GetRootNode();
        if (FbxRootNode)
        {
            for (i32 i = 0; i < FbxRootNode->GetChildCount(); i++)
            {
                fbxsdk::FbxNode* childNode =FbxRootNode->GetChild(i);

                fbxsdk::FbxNodeAttribute::EType lAttributeType;

                if (childNode->GetNodeAttribute() == NULL)
                {
                    FBXSDK_printf("NULL Node Attribute\n\n");
                }
                else
                {
                    lAttributeType = (childNode->GetNodeAttribute()->GetAttributeType());

                    switch (lAttributeType)
                    {
                    default:
                        break;
                    case fbxsdk::FbxNodeAttribute::eMarker:
                        break;

                    case fbxsdk::FbxNodeAttribute::eSkeleton:
                        break;

                    case fbxsdk::FbxNodeAttribute::eMesh:
                    {
                        fbxsdk::FbxMesh* fbxMesh = (fbxsdk::FbxMesh*)childNode->GetNodeAttribute();
                        _scene->meshes.push_back(Mesh());
                        read_mesh_fbx(fbxMesh, &(_scene->meshes.back()));
                        display_textures_info(fbxMesh);
                        break;
                    }
                    case fbxsdk::FbxNodeAttribute::eNurbs:
                        break;

                    case fbxsdk::FbxNodeAttribute::ePatch:
                        break;

                    case fbxsdk::FbxNodeAttribute::eCamera:
                        break;

                    case fbxsdk::FbxNodeAttribute::eLight:
                        break;

                    case fbxsdk::FbxNodeAttribute::eLODGroup:
                        break;
                    }
                }
            }
        }

        lSdkManager->Destroy();
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


    //void Mesh::read(FbxMesh* pMesh)
    //{
    //    int i, lControlPointsCount = pMesh->GetControlPointsCount();
    //    FbxVector4* lControlPoints = pMesh->GetControlPoints();
    //    vertices.reserve(lControlPointsCount);

    //    for (i = 0; i < lControlPointsCount; i++)
    //    {
    //        Vertex v {};
    //        v.position = glm::vec3(lControlPoints[i][0], lControlPoints[i][1], lControlPoints[i][2]);
    //        vertices.push_back(v);

    //        //for (int j = 0; j < pMesh->GetElementNormalCount(); j++)
    //        //{
    //        //    FbxGeometryElementNormal* leNormals = pMesh->GetElementNormal(j);
    //        //    if (leNormals->GetMappingMode() == FbxGeometryElement::eByControlPoint)
    //        //    {
    //        //        if (leNormals->GetReferenceMode() == FbxGeometryElement::eDirect)
    //        //            RAIN_LOG_RAW("            Normal Vector: (%f, %f, %f)\n",
    //        //                leNormals->GetDirectArray().GetAt(i)[0],
    //        //                leNormals->GetDirectArray().GetAt(i)[1],
    //        //                leNormals->GetDirectArray().GetAt(i)[2]);
    //        //    }
    //        //}
    //    }


    //    int j, lPolygonCount = pMesh->GetPolygonCount();

    //    //RAIN_LOG_RAW("    Polygons\n");

    //    int vertexId = 0;
    //    for (i = 0; i < lPolygonCount; i++)
    //    {
    //        //RAIN_LOG_RAW("        Polygon %d\n", i);
    //        int l;

    //        for (l = 0; l < pMesh->GetElementPolygonGroupCount(); l++)
    //        {
    //            FbxGeometryElementPolygonGroup* lePolgrp = pMesh->GetElementPolygonGroup(l);
    //            switch (lePolgrp->GetMappingMode())
    //            {
    //            case FbxGeometryElement::eByPolygon:
    //                if (lePolgrp->GetReferenceMode() == FbxGeometryElement::eIndex)
    //                {
    //                    int polyGroupId = lePolgrp->GetIndexArray().GetAt(i);
    //                    break;
    //                }
    //            default:
    //                // any other mapping modes don't make sense
    //                break;
    //            }
    //        }

    //        int lPolygonSize = pMesh->GetPolygonSize(i);

    //        for (j = 0; j < lPolygonSize; j++)
    //        {
    //            int lControlPointIndex = pMesh->GetPolygonVertex(i, j);



    //            indices.push_back(lControlPointIndex);

    //            for (l = 0; l < pMesh->GetElementVertexColorCount(); l++)
    //            {
    //                FbxGeometryElementVertexColor* leVtxc = pMesh->GetElementVertexColor(l);

    //                switch (leVtxc->GetMappingMode())
    //                {
    //                default:
    //                    break;
    //                case FbxGeometryElement::eByControlPoint:
    //                    switch (leVtxc->GetReferenceMode())
    //                    {
    //                    case FbxGeometryElement::eDirect:

    //                        break;
    //                    case FbxGeometryElement::eIndexToDirect:
    //                    {
    //                        int id = leVtxc->GetIndexArray().GetAt(lControlPointIndex);

    //                    }
    //                    break;
    //                    default:
    //                        break; // other reference modes not shown here!
    //                    }
    //                    break;

    //                    //case FbxGeometryElement::eByPolygonVertex:
    //                    //{
    //                    //    switch (leVtxc->GetReferenceMode())
    //                    //    {
    //                    //    case FbxGeometryElement::eDirect:
    //                    //        RAIN_LOG_RAW("            Color vertex: (%f, %f, %f, %f)\n",
    //                    //            leVtxc->GetDirectArray().GetAt(vertexId)[0],
    //                    //            leVtxc->GetDirectArray().GetAt(vertexId)[1],
    //                    //            leVtxc->GetDirectArray().GetAt(vertexId)[2],
    //                    //            leVtxc->GetDirectArray().GetAt(vertexId)[3]);
    //                    //        break;
    //                    //    case FbxGeometryElement::eIndexToDirect:
    //                    //    {
    //                    //        int id = leVtxc->GetIndexArray().GetAt(vertexId);
    //                    //        RAIN_LOG_RAW("            Color vertex: (%f, %f, %f, %f)\n",
    //                    //            leVtxc->GetDirectArray().GetAt(id)[0],
    //                    //            leVtxc->GetDirectArray().GetAt(id)[1],
    //                    //            leVtxc->GetDirectArray().GetAt(id)[2],
    //                    //            leVtxc->GetDirectArray().GetAt(id)[3]);
    //                    //    }
    //                    //    break;
    //                    //    default:
    //                    //        break; // other reference modes not shown here!
    //                    //    }
    //                    //}
    //                    //break;

    //                case FbxGeometryElement::eByPolygon: // doesn't make much sense for UVs
    //                case FbxGeometryElement::eAllSame:   // doesn't make much sense for UVs
    //                case FbxGeometryElement::eNone:       // doesn't make much sense for UVs
    //                    break;
    //                }
    //            }
    //            for (l = 0; l < pMesh->GetElementUVCount(); ++l)
    //            {
    //                FbxGeometryElementUV* leUV = pMesh->GetElementUV(l);

    //                switch (leUV->GetMappingMode())
    //                {
    //                default:
    //                    break;
    //                case FbxGeometryElement::eByControlPoint:
    //                    switch (leUV->GetReferenceMode())
    //                    {
    //                    case FbxGeometryElement::eDirect:

    //                        break;
    //                    case FbxGeometryElement::eIndexToDirect:
    //                    {
    //                        int id = leUV->GetIndexArray().GetAt(lControlPointIndex);

    //                    }
    //                    break;
    //                    default:
    //                        break; // other reference modes not shown here!
    //                    }
    //                    break;

    //                case FbxGeometryElement::eByPolygonVertex:
    //                {
    //                    int lTextureUVIndex = pMesh->GetTextureUVIndex(i, j);
    //                    switch (leUV->GetReferenceMode())
    //                    {
    //                    case FbxGeometryElement::eDirect:
    //                    case FbxGeometryElement::eIndexToDirect:
    //                    {
    //                        vertices[lControlPointIndex].texCoords[l].x = leUV->GetDirectArray().GetAt(lTextureUVIndex)[0];
    //                        vertices[lControlPointIndex].texCoords[l].y = leUV->GetDirectArray().GetAt(lTextureUVIndex)[1];

    //                    }
    //                    break;
    //                    default:
    //                        break; // other reference modes not shown here!
    //                    }
    //                }
    //                break;

    //                case FbxGeometryElement::eByPolygon: // doesn't make much sense for UVs
    //                case FbxGeometryElement::eAllSame:   // doesn't make much sense for UVs
    //                case FbxGeometryElement::eNone:       // doesn't make much sense for UVs
    //                    break;
    //                }
    //            }
    //            for (l = 0; l < pMesh->GetElementNormalCount(); ++l)
    //            {
    //                FbxGeometryElementNormal* leNormal = pMesh->GetElementNormal(l);

    //                if (leNormal->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
    //                {
    //                    switch (leNormal->GetReferenceMode())
    //                    {
    //                    case FbxGeometryElement::eDirect:

    //                        break;
    //                    case FbxGeometryElement::eIndexToDirect:
    //                    {
    //                        int id = leNormal->GetIndexArray().GetAt(vertexId);

    //                    }
    //                    break;
    //                    default:
    //                        break; // other reference modes not shown here!
    //                    }
    //                }

    //            }
    //            for (l = 0; l < pMesh->GetElementTangentCount(); ++l)
    //            {
    //                FbxGeometryElementTangent* leTangent = pMesh->GetElementTangent(l);

    //                if (leTangent->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
    //                {
    //                    switch (leTangent->GetReferenceMode())
    //                    {
    //                    case FbxGeometryElement::eDirect:

    //                        break;
    //                    case FbxGeometryElement::eIndexToDirect:
    //                    {
    //                        int id = leTangent->GetIndexArray().GetAt(vertexId);

    //                    }
    //                    break;
    //                    default:
    //                        break; // other reference modes not shown here!
    //                    }
    //                }

    //            }
    //            for (l = 0; l < pMesh->GetElementBinormalCount(); ++l)
    //            {

    //                FbxGeometryElementBinormal* leBinormal = pMesh->GetElementBinormal(l);

    //                if (leBinormal->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
    //                {
    //                    switch (leBinormal->GetReferenceMode())
    //                    {
    //                    case FbxGeometryElement::eDirect:

    //                        break;
    //                    case FbxGeometryElement::eIndexToDirect:
    //                    {
    //                        int id = leBinormal->GetIndexArray().GetAt(vertexId);

    //                    }
    //                    break;
    //                    default:
    //                        break; // other reference modes not shown here!
    //                    }
    //                }
    //            }
    //            vertexId++;
    //        } // for polygonSize
    //    } // for polygonCount


    //}



    //void read_mesh(fbxsdk::FbxNode* _node)
    //{
    //    FbxMesh* mesh = (FbxMesh*)_node->GetNodeAttribute();

    //    int i, lControlPointsCount = mesh->GetControlPointsCount();
    //    FbxVector4* lControlPoints = mesh->GetControlPoints();

    //    for (i = 0; i < lControlPointsCount; i++)
    //    {
    //        RAIN_LOG_RAW("        Control Point %d\n", i);
    //        RAIN_LOG_RAW("            Coordinates: (%f, %f, %f)\n", lControlPoints[i][0], lControlPoints[i][1], lControlPoints[i][2]);

    //        for (int j = 0; j < mesh->GetElementNormalCount(); j++)
    //        {
    //            FbxGeometryElementNormal* leNormals = mesh->GetElementNormal(j);
    //            if (leNormals->GetMappingMode() == FbxGeometryElement::eByControlPoint)
    //            {
    //                if (leNormals->GetReferenceMode() == FbxGeometryElement::eDirect)
    //                    RAIN_LOG_RAW("            Normal Vector: (%f, %f, %f)\n",
    //                        leNormals->GetDirectArray().GetAt(i)[0],
    //                        leNormals->GetDirectArray().GetAt(i)[1],
    //                        leNormals->GetDirectArray().GetAt(i)[2]);
    //            }
    //        }
    //    }
    //}



    //void Mesh::read_node(fbxsdk::FbxNode* _node)
    //{
    //    FbxNodeAttribute::EType lAttributeType;
    //    int i;

    //    if (_node->GetNodeAttribute() == NULL)
    //    {
    //        FBXSDK_printf("NULL Node Attribute\n\n");
    //    }
    //    else
    //    {
    //        lAttributeType = (_node->GetNodeAttribute()->GetAttributeType());

    //        switch (lAttributeType)
    //        {
    //        default:
    //            break;
    //        case FbxNodeAttribute::eMarker:
    //            break;

    //        case FbxNodeAttribute::eSkeleton:
    //            break;

    //        case FbxNodeAttribute::eMesh:
    //        {
    //            //read_mesh(_node);
    //            FbxMesh* mesh = (FbxMesh*)_node->GetNodeAttribute();
    //            read(mesh);
    //            break;
    //        }
    //        case FbxNodeAttribute::eNurbs:
    //            break;

    //        case FbxNodeAttribute::ePatch:
    //            break;

    //        case FbxNodeAttribute::eCamera:
    //            break;

    //        case FbxNodeAttribute::eLight:
    //            break;

    //        case FbxNodeAttribute::eLODGroup:
    //            break;
    //        }
    //    }
    //}


    //void Mesh::read_polygons(FbxMesh* pMesh)
    //{
    //    int i, j, lPolygonCount = pMesh->GetPolygonCount();
    //    FbxVector4* lControlPoints = pMesh->GetControlPoints();

    //    RAIN_LOG_RAW("    Polygons\n");

    //    int vertexId = 0;
    //    for (i = 0; i < lPolygonCount; i++)
    //    {
    //        RAIN_LOG_RAW("        Polygon %d\n", i);
    //        int l;

    //        for (l = 0; l < pMesh->GetElementPolygonGroupCount(); l++)
    //        {
    //            FbxGeometryElementPolygonGroup* lePolgrp = pMesh->GetElementPolygonGroup(l);
    //            switch (lePolgrp->GetMappingMode())
    //            {
    //            case FbxGeometryElement::eByPolygon:
    //                if (lePolgrp->GetReferenceMode() == FbxGeometryElement::eIndex)
    //                {
    //                    int polyGroupId = lePolgrp->GetIndexArray().GetAt(i);
    //                    RAIN_LOG_RAW("        Assigned to group: %d\n", polyGroupId);
    //                    break;
    //                }
    //            default:
    //                // any other mapping modes don't make sense
    //                RAIN_LOG_RAW("        \"unsupported group assignment\"\n");
    //                break;
    //            }
    //        }

    //        int lPolygonSize = pMesh->GetPolygonSize(i);

    //        for (j = 0; j < lPolygonSize; j++)
    //        {
    //            int lControlPointIndex = pMesh->GetPolygonVertex(i, j);

    //            RAIN_LOG_RAW("            Coordinates %d: (%f, %f, %f)\n",
    //                lControlPointIndex,
    //                lControlPoints[lControlPointIndex][0],
    //                lControlPoints[lControlPointIndex][1],
    //                lControlPoints[lControlPointIndex][2]);

    //            for (l = 0; l < pMesh->GetElementVertexColorCount(); l++)
    //            {
    //                FbxGeometryElementVertexColor* leVtxc = pMesh->GetElementVertexColor(l);

    //                switch (leVtxc->GetMappingMode())
    //                {
    //                default:
    //                    break;
    //                case FbxGeometryElement::eByControlPoint:
    //                    switch (leVtxc->GetReferenceMode())
    //                    {
    //                    case FbxGeometryElement::eDirect:
    //                        RAIN_LOG_RAW("            Color vertex: (%f, %f, %f, %f)\n",
    //                            leVtxc->GetDirectArray().GetAt(lControlPointIndex)[0],
    //                            leVtxc->GetDirectArray().GetAt(lControlPointIndex)[1], 
    //                            leVtxc->GetDirectArray().GetAt(lControlPointIndex)[2], 
    //                            leVtxc->GetDirectArray().GetAt(lControlPointIndex)[3]);
    //                        break;
    //                    case FbxGeometryElement::eIndexToDirect:
    //                    {
    //                        int id = leVtxc->GetIndexArray().GetAt(lControlPointIndex);
    //                        RAIN_LOG_RAW("            Color vertex: (%f, %f, %f, %f)\n",
    //                            leVtxc->GetDirectArray().GetAt(id)[0],
    //                            leVtxc->GetDirectArray().GetAt(id)[1],
    //                            leVtxc->GetDirectArray().GetAt(id)[2],
    //                            leVtxc->GetDirectArray().GetAt(id)[3]);
    //                    }
    //                    break;
    //                    default:
    //                        break; // other reference modes not shown here!
    //                    }
    //                    break;

    //                //case FbxGeometryElement::eByPolygonVertex:
    //                //{
    //                //    switch (leVtxc->GetReferenceMode())
    //                //    {
    //                //    case FbxGeometryElement::eDirect:
    //                //        RAIN_LOG_RAW("            Color vertex: (%f, %f, %f, %f)\n",
    //                //            leVtxc->GetDirectArray().GetAt(vertexId)[0],
    //                //            leVtxc->GetDirectArray().GetAt(vertexId)[1],
    //                //            leVtxc->GetDirectArray().GetAt(vertexId)[2],
    //                //            leVtxc->GetDirectArray().GetAt(vertexId)[3]);
    //                //        break;
    //                //    case FbxGeometryElement::eIndexToDirect:
    //                //    {
    //                //        int id = leVtxc->GetIndexArray().GetAt(vertexId);
    //                //        RAIN_LOG_RAW("            Color vertex: (%f, %f, %f, %f)\n",
    //                //            leVtxc->GetDirectArray().GetAt(id)[0],
    //                //            leVtxc->GetDirectArray().GetAt(id)[1],
    //                //            leVtxc->GetDirectArray().GetAt(id)[2],
    //                //            leVtxc->GetDirectArray().GetAt(id)[3]);
    //                //    }
    //                //    break;
    //                //    default:
    //                //        break; // other reference modes not shown here!
    //                //    }
    //                //}
    //                //break;

    //                case FbxGeometryElement::eByPolygon: // doesn't make much sense for UVs
    //                case FbxGeometryElement::eAllSame:   // doesn't make much sense for UVs
    //                case FbxGeometryElement::eNone:       // doesn't make much sense for UVs
    //                    break;
    //                }
    //            }
    //            for (l = 0; l < pMesh->GetElementUVCount(); ++l)
    //            {
    //                FbxGeometryElementUV* leUV = pMesh->GetElementUV(l);

    //                switch (leUV->GetMappingMode())
    //                {
    //                default:
    //                    break;
    //                case FbxGeometryElement::eByControlPoint:
    //                    switch (leUV->GetReferenceMode())
    //                    {
    //                    case FbxGeometryElement::eDirect:
    //                        RAIN_LOG_RAW("            Texture UV %d: (%f, %f)\n",
    //                            l,
    //                            leUV->GetDirectArray().GetAt(lControlPointIndex)[0],
    //                            leUV->GetDirectArray().GetAt(lControlPointIndex)[1]);
    //                        break;
    //                    case FbxGeometryElement::eIndexToDirect:
    //                    {
    //                        int id = leUV->GetIndexArray().GetAt(lControlPointIndex);
    //                        RAIN_LOG_RAW("            Texture UV %d: (%f, %f)\n",
    //                            l,
    //                            leUV->GetDirectArray().GetAt(id)[0],
    //                            leUV->GetDirectArray().GetAt(id)[1]);
    //                    }
    //                    break;
    //                    default:
    //                        break; // other reference modes not shown here!
    //                    }
    //                    break;

    //                case FbxGeometryElement::eByPolygonVertex:
    //                {
    //                    int lTextureUVIndex = pMesh->GetTextureUVIndex(i, j);
    //                    switch (leUV->GetReferenceMode())
    //                    {
    //                    case FbxGeometryElement::eDirect:
    //                    case FbxGeometryElement::eIndexToDirect:
    //                    {
    //                        RAIN_LOG_RAW("            Texture UV %d polygon %d : (%f, %f)\n",
    //                            l,
    //                            lTextureUVIndex,
    //                            leUV->GetDirectArray().GetAt(lTextureUVIndex)[0],
    //                            leUV->GetDirectArray().GetAt(lTextureUVIndex)[1]);
    //                    }
    //                    break;
    //                    default:
    //                        break; // other reference modes not shown here!
    //                    }
    //                }
    //                break;

    //                case FbxGeometryElement::eByPolygon: // doesn't make much sense for UVs
    //                case FbxGeometryElement::eAllSame:   // doesn't make much sense for UVs
    //                case FbxGeometryElement::eNone:       // doesn't make much sense for UVs
    //                    break;
    //                }
    //            }
    //            for (l = 0; l < pMesh->GetElementNormalCount(); ++l)
    //            {
    //                FbxGeometryElementNormal* leNormal = pMesh->GetElementNormal(l);

    //                if (leNormal->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
    //                {
    //                    switch (leNormal->GetReferenceMode())
    //                    {
    //                    case FbxGeometryElement::eDirect:
    //                        RAIN_LOG_RAW("            Normal: (%f, %f, %f)\n",
    //                            leNormal->GetDirectArray().GetAt(vertexId)[0],
    //                            leNormal->GetDirectArray().GetAt(vertexId)[1],
    //                            leNormal->GetDirectArray().GetAt(vertexId)[2]);
    //                        break;
    //                    case FbxGeometryElement::eIndexToDirect:
    //                    {
    //                        int id = leNormal->GetIndexArray().GetAt(vertexId);
    //                        RAIN_LOG_RAW("            Normal: (%f, %f, %f)\n",
    //                            leNormal->GetDirectArray().GetAt(id)[0],
    //                            leNormal->GetDirectArray().GetAt(id)[1],
    //                            leNormal->GetDirectArray().GetAt(id)[2]);
    //                    }
    //                    break;
    //                    default:
    //                        break; // other reference modes not shown here!
    //                    }
    //                }

    //            }
    //            for (l = 0; l < pMesh->GetElementTangentCount(); ++l)
    //            {
    //                FbxGeometryElementTangent* leTangent = pMesh->GetElementTangent(l);

    //                if (leTangent->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
    //                {
    //                    switch (leTangent->GetReferenceMode())
    //                    {
    //                    case FbxGeometryElement::eDirect:
    //                        RAIN_LOG_RAW("            Tangent: (%f, %f, %f)\n",
    //                            leTangent->GetDirectArray().GetAt(vertexId)[0],
    //                            leTangent->GetDirectArray().GetAt(vertexId)[1],
    //                            leTangent->GetDirectArray().GetAt(vertexId)[2]);
    //                        break;
    //                    case FbxGeometryElement::eIndexToDirect:
    //                    {
    //                        int id = leTangent->GetIndexArray().GetAt(vertexId);
    //                        RAIN_LOG_RAW("            Tangent: (%f, %f, %f)\n",
    //                            leTangent->GetDirectArray().GetAt(id)[0],
    //                            leTangent->GetDirectArray().GetAt(id)[1],
    //                            leTangent->GetDirectArray().GetAt(id)[2]);
    //                    }
    //                    break;
    //                    default:
    //                        break; // other reference modes not shown here!
    //                    }
    //                }

    //            }
    //            for (l = 0; l < pMesh->GetElementBinormalCount(); ++l)
    //            {

    //                FbxGeometryElementBinormal* leBinormal = pMesh->GetElementBinormal(l);

    //                if (leBinormal->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
    //                {
    //                    switch (leBinormal->GetReferenceMode())
    //                    {
    //                    case FbxGeometryElement::eDirect:
    //                        RAIN_LOG_RAW("            Binormal: (%f, %f, %f)\n",
    //                            leBinormal->GetDirectArray().GetAt(vertexId)[0],
    //                            leBinormal->GetDirectArray().GetAt(vertexId)[1],
    //                            leBinormal->GetDirectArray().GetAt(vertexId)[2]);
    //                        break;
    //                    case FbxGeometryElement::eIndexToDirect:
    //                    {
    //                        int id = leBinormal->GetIndexArray().GetAt(vertexId);
    //                        RAIN_LOG_RAW("            Binormal: (%f, %f, %f)\n",
    //                            leBinormal->GetDirectArray().GetAt(id)[0],
    //                            leBinormal->GetDirectArray().GetAt(id)[1],
    //                            leBinormal->GetDirectArray().GetAt(id)[2]);
    //                    }
    //                    break;
    //                    default:
    //                        break; // other reference modes not shown here!
    //                    }
    //                }
    //            }
    //            vertexId++;
    //        } // for polygonSize
    //    } // for polygonCount


    //    //check visibility for the edges of the mesh
    //    for (int l = 0; l < pMesh->GetElementVisibilityCount(); ++l)
    //    {
    //        FbxGeometryElementVisibility* leVisibility = pMesh->GetElementVisibility(l);
    //        RAIN_LOG_RAW("    Edge Visibility : \n");
    //        switch (leVisibility->GetMappingMode())
    //        {
    //        default:
    //            break;
    //            //should be eByEdge
    //        case FbxGeometryElement::eByEdge:
    //            //should be eDirect
    //            for (j = 0; j != pMesh->GetMeshEdgeCount(); ++j)
    //            {
    //                RAIN_LOG_RAW("        Edge %d\n", j);
    //                RAIN_LOG_RAW("              Edge visibility: %d\n", leVisibility->GetDirectArray().GetAt(j));
    //            }

    //            break;
    //        }
    //    }
    //    RAIN_LOG_RAW("\n");
    //}




    //void Mesh::read_scene(const std::string& _path)
    //{
    //    const char* lFilename = _path.c_str();

    //    fbxsdk::FbxManager* lSdkManager = fbxsdk::FbxManager::Create();
    //    fbxsdk::FbxIOSettings *ios = fbxsdk::FbxIOSettings::Create(lSdkManager, IOSROOT);
    //    lSdkManager->SetIOSettings(ios);

    //    fbxsdk::FbxImporter* lImporter = fbxsdk::FbxImporter::Create(lSdkManager, "");

    //    if (!lImporter->Initialize(lFilename, -1, lSdkManager->GetIOSettings()))
    //    {
    //        RAIN_LOG("Call to FbxImporter::Initialize() failed.\n");
    //        RAIN_LOG("Error returned: %s\n\n", lImporter->GetStatus().GetErrorString());
    //        return;
    //    }

    //    fbxsdk::FbxScene* lScene = fbxsdk::FbxScene::Create(lSdkManager, "scene");

    //    lImporter->Import(lScene);
    //    lImporter->Destroy();

    //    fbxsdk::FbxNode* pFbxRootNode = lScene->GetRootNode();
    //    if (pFbxRootNode)
    //    {
    //        for (int i = 0; i < pFbxRootNode->GetChildCount(); i++)
    //        {
    //            fbxsdk::FbxNode* pFbxChildNode = pFbxRootNode->GetChild(i);
    //            read_node(pFbxChildNode);
    //        }
    //    }

    //    lSdkManager->Destroy();

    //    RAIN_LOG("vertices : %d", vertices.size());
    //    RAIN_LOG("indices : %d", indices.size());
    //}


 

}



    //fbxsdk::FbxVector4 getNormal(fbxsdk::FbxGeometryElementNormal* normalElement, int polyIndex, int posIndex)
    //{
    //    if (normalElement->GetMappingMode() == fbxsdk::FbxGeometryElement::eByControlPoint)
    //    {
    //        if (normalElement->GetReferenceMode() == fbxsdk::FbxGeometryElement::eDirect)
    //            return normalElement->GetDirectArray().GetAt(posIndex);
    //        int i = normalElement->GetIndexArray().GetAt(posIndex);
    //        return normalElement->GetDirectArray().GetAt(i);
    //    }
    //    else if (normalElement->GetMappingMode() == fbxsdk::FbxGeometryElement::eByPolygonVertex)
    //    {
    //        if (normalElement->GetReferenceMode() == fbxsdk::FbxGeometryElement::eDirect)
    //            return normalElement->GetDirectArray().GetAt(polyIndex);
    //        int i = normalElement->GetIndexArray().GetAt(polyIndex);
    //        return normalElement->GetDirectArray().GetAt(i);
    //    }
    //    return fbxsdk::FbxVector4();
    //}

    //fbxsdk::FbxVector2 getUV(fbxsdk::FbxGeometryElementUV* uvElement, int polyIndex, int posIndex)
    //{
    //    if (uvElement->GetMappingMode() == fbxsdk::FbxGeometryElement::eByControlPoint)
    //    {
    //        if (uvElement->GetReferenceMode() == fbxsdk::FbxGeometryElement::eDirect)
    //            return uvElement->GetDirectArray().GetAt(posIndex);
    //        int i = uvElement->GetIndexArray().GetAt(posIndex);
    //        return uvElement->GetDirectArray().GetAt(i);
    //    }
    //    else if (uvElement->GetMappingMode() == fbxsdk::FbxGeometryElement::eByPolygonVertex)
    //    {
    //        if (uvElement->GetReferenceMode() == fbxsdk::FbxGeometryElement::eDirect)
    //            return uvElement->GetDirectArray().GetAt(polyIndex);
    //        int i = uvElement->GetIndexArray().GetAt(polyIndex);
    //        return uvElement->GetDirectArray().GetAt(i);
    //    }
    //    return fbxsdk::FbxVector2();
    //}

   








    //void LoadMesh(Mesh* _mesh, const std::string& _path)
    //{
    //    const char* lFilename = _path.c_str();

    //    fbxsdk::FbxManager* lSdkManager = fbxsdk::FbxManager::Create();
    //    fbxsdk::FbxIOSettings *ios = fbxsdk::FbxIOSettings::Create(lSdkManager, IOSROOT);
    //    lSdkManager->SetIOSettings(ios);

    //    fbxsdk::FbxImporter* lImporter = fbxsdk::FbxImporter::Create(lSdkManager, "");

    //    if (!lImporter->Initialize(lFilename, -1, lSdkManager->GetIOSettings()))
    //    {
    //        RAIN_LOG("Call to FbxImporter::Initialize() failed.\n");
    //        RAIN_LOG("Error returned: %s\n\n", lImporter->GetStatus().GetErrorString());
    //        exit(-1);
    //    }

    //    fbxsdk::FbxScene* lScene = fbxsdk::FbxScene::Create(lSdkManager, "scene");

    //    lImporter->Import(lScene);
    //    lImporter->Destroy();

    //    fbxsdk::FbxNode* pFbxRootNode = lScene->GetRootNode();
    //    if (pFbxRootNode)
    //    {
    //        for (int i = 0; i < pFbxRootNode->GetChildCount(); i++)
    //        {
    //            fbxsdk::FbxNode* pFbxChildNode = pFbxRootNode->GetChild(i);

    //            if (pFbxChildNode->GetNodeAttribute() == NULL)
    //                continue;

    //            fbxsdk::FbxNodeAttribute::EType AttributeType = pFbxChildNode->GetNodeAttribute()->GetAttributeType();

    //            if (AttributeType != fbxsdk::FbxNodeAttribute::eMesh)
    //                continue;

    //            fbxsdk::FbxMesh* pMesh = (fbxsdk::FbxMesh*)pFbxChildNode->GetNodeAttribute();

    //            fbxsdk::FbxVector4* pVertices = pMesh->GetControlPoints();
    //            
    //            for (int j = 0; j < pMesh->GetPolygonCount(); j++)
    //            {
    //                int iNumVertices = pMesh->GetPolygonSize(j);
    //                assert(iNumVertices == 3);

    //                for (int k = 0; k < iNumVertices; k++) {
    //                    int iControlPointIndex = pMesh->GetPolygonVertex(j, k);
    //                    Vertex vertex;
    //                    vertex.position[0] = (float)pVertices[iControlPointIndex].mData[0];
    //                    vertex.position[1] = (float)pVertices[iControlPointIndex].mData[1];
    //                    vertex.position[2] = (float)pVertices[iControlPointIndex].mData[2];
    //                    _mesh->vertices.push_back(vertex);
    //                }
    //            }

    //            int vertNum = 0;

    //            for (int polyIndex = 0; polyIndex < pMesh->GetPolygonCount(); ++polyIndex)
    //            {
    //                const auto polySize = pMesh->GetPolygonSize(polyIndex);
    //                //face_indices[currentPosPolyIndex].resize(polySize);

    //                for (int polyVertIndex = 0; polyVertIndex < polySize; ++polyVertIndex)
    //                {
    //                    int vertIndex = pMesh->GetPolygonVertex(polyIndex, polyVertIndex);

    //                    _mesh->indices.push_back(vertIndex);
    //                    fbxsdk::FbxVector4 normal = getNormal(pMesh->GetElementNormal(), vertNum, vertIndex);
    //                    fbxsdk::FbxVector2 uv = getUV(pMesh->GetElementUV(), vertNum, vertIndex);
    //                    vertNum++;
    //                }
    //            }
    //        }
    //    }

    //    lSdkManager->Destroy();
    //}