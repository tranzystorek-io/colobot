/*
 * This file is part of the Colobot: Gold Edition source code
 * Copyright (C) 2001-2021, Daniel Roux, EPSITEC SA & TerranovaTeam
 * http://epsitec.ch; http://colobot.info; http://github.com/colobot
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see http://gnu.org/licenses
 */

 /**
  * \file graphics/opengl/gl33renderers.h
  * \brief OpenGL 3.3 renderers
  */

#pragma once

#include "graphics/core/renderers.h"

#include <GL/glew.h>

#include <array>
#include <vector>

// Graphics module namespace
namespace Gfx
{

class CGL33Device;

class CGL33UIRenderer : public CUIRenderer
{
public:
    CGL33UIRenderer(CGL33Device* device);
    virtual ~CGL33UIRenderer();

    virtual void SetProjection(float left, float right, float bottom, float top) override;
    virtual void SetTexture(const Texture& texture) override;
    virtual void SetColor(const glm::vec4& color) override;
    virtual void SetTransparency(TransparencyMode mode) override;

    virtual void DrawPrimitive(PrimitiveType type, int count, const Vertex2D* vertices) override;

    virtual Vertex2D* BeginPrimitive(PrimitiveType type, int count) override;
    virtual Vertex2D* BeginPrimitives(PrimitiveType type, int drawCount, const int* counts) override;
    virtual bool EndPrimitive() override;

private:
    void UpdateUniforms();

    CGL33Device* const m_device;

    // Uniform data
    struct Uniforms
    {
        glm::mat4 projectionMatrix;
        glm::vec4 color;
    };
    Uniforms m_uniforms = {};

    // true means uniforms need to be updated
    bool m_uniformsDirty = false;

    // Uniform buffer object
    GLuint m_uniformBuffer = 0;

    // Vertex buffer object
    GLuint m_bufferVBO = 0;
    // Vertex array object
    GLuint m_bufferVAO = 0;
    // VBO capacity
    GLsizei m_bufferCapacity = 128 * 1024;
    // Buffer offset
    GLsizei m_bufferOffset = 0;
    
    // Buffer mapping state
    PrimitiveType m_type = {};
    // Number of drawn primitives
    GLuint m_drawCount = 0;
    // Total count of drawn vertices
    GLuint m_currentCount = 0;
    // Starting offset for each drawn primitive
    std::vector<GLint> m_first;
    // Numbers of vertices for each drawn primitive
    std::vector<GLsizei> m_count;
    // True means currently drawing
    bool m_mapped = false;
    // True means mapping failed, using auxiliary buffer
    bool m_backup = false;

    // Buffered vertex data
    std::vector<Vertex2D> m_buffer;

    // Shader program
    GLuint m_program = 0;

    // 1x1 white texture
    GLuint m_whiteTexture = 0;
    // Currently bound texture
    GLuint m_currentTexture = 0;
};

class CGL33TerrainRenderer : public CTerrainRenderer
{
public:
    CGL33TerrainRenderer(CGL33Device* device);
    virtual ~CGL33TerrainRenderer();

    virtual void Begin() override;

    virtual void End() override;

    //! Sets projection matrix
    virtual void SetProjectionMatrix(const glm::mat4& matrix) override;
    //! Sets view matrix
    virtual void SetViewMatrix(const glm::mat4& matrix) override;
    //! Sets model matrix
    virtual void SetModelMatrix(const glm::mat4& matrix) override;

    //! Sets albedo color
    virtual void SetAlbedoColor(const Color& color) override;
    //! Sets albedo texture
    virtual void SetAlbedoTexture(const Texture& texture) override;
    //! Sets emissive color
    virtual void SetEmissiveColor(const Color& color) override;
    //! Sets emissive texture
    virtual void SetEmissiveTexture(const Texture& texture) override;
    //! Sets material parameters
    virtual void SetMaterialParams(float roughness, float metalness, float aoStrength) override;
    //! Sets material texture
    virtual void SetMaterialTexture(const Texture& texture) override;

    //! Sets detail texture
    virtual void SetDetailTexture(const Texture& texture) override;
    //! Sets shadow map
    virtual void SetShadowMap(const Texture& texture) override;

    //! Sets light parameters
    virtual void SetLight(const glm::vec4& position, const float& intensity, const glm::vec3& color) override;
    //! Sets sky parameters
    virtual void SetSky(const Color& color, float intensity) override;
    //! Sets shadow parameters
    virtual void SetShadowParams(int count, const ShadowParam* params) override;

    //! Sets fog parameters
    virtual void SetFog(float min, float max, const glm::vec3& color) override;

    //! Draws terrain object
    virtual void DrawObject(const glm::mat4& matrix, const CVertexBuffer* buffer) override;

private:
    CGL33Device* const m_device;

    // Uniform data
    GLint m_projectionMatrix = -1;
    GLint m_viewMatrix = -1;
    GLint m_shadowMatrix = -1;
    GLint m_modelMatrix = -1;
    GLint m_normalMatrix = -1;

    GLint m_cameraPosition = -1;
    GLint m_lightPosition = -1;
    GLint m_lightIntensity = -1;
    GLint m_lightColor = -1;

    GLint m_skyColor = -1;
    GLint m_skyIntensity = -1;

    GLint m_fogRange = -1;
    GLint m_fogColor = -1;

    GLint m_albedoColor = -1;
    GLint m_emissiveColor = -1;
    GLint m_roughness = -1;
    GLint m_metalness = -1;
    GLint m_aoStrength = -1;

    struct ShadowUniforms
    {
        GLint transform;
        GLint offset;
        GLint scale;
    };

    GLint m_shadowRegions = 0;
    ShadowUniforms m_shadows[4] = {};

    // Shader program
    GLuint m_program = 0;

    // Texture unit bindings
    const int m_albedoIndex = 4;
    const int m_detailIndex = 5;
    const int m_emissiveIndex = 6;
    const int m_materialIndex = 7;
    const int m_shadowIndex = 8;

    // 1x1 white texture
    GLuint m_whiteTexture = 0;
    // Currently bound albedo texture
    GLuint m_albedoTexture = 0;
    // Currently bound detail texture
    GLuint m_detailTexture = 0;
    // Currently bound emissive texture
    GLuint m_emissiveTexture = 0;
    // Currently bound material texture
    GLuint m_materialTexture = 0;
    // Currently bound shadow map
    GLuint m_shadowMap = 0;
};

class CGL33ShadowRenderer : public CShadowRenderer
{
public:
    CGL33ShadowRenderer(CGL33Device* device);
    virtual ~CGL33ShadowRenderer();

    virtual void Begin() override;

    virtual void End() override;

    //! Sets projection matrix
    virtual void SetProjectionMatrix(const glm::mat4& matrix) override;
    //! Sets view matrix
    virtual void SetViewMatrix(const glm::mat4& matrix) override;
    //! Sets model matrix
    virtual void SetModelMatrix(const glm::mat4& matrix) override;

    //! Sets texture
    virtual void SetTexture(const Texture& texture) override;

    //! Sets shadow map
    virtual void SetShadowMap(const Texture& texture) override;
    //! Sets shadow region
    virtual void SetShadowRegion(const glm::vec2& offset, const glm::vec2& scale) override;

    //! Draws terrain object
    virtual void DrawObject(const CVertexBuffer* buffer, bool transparent) override;

private:
    CGL33Device* const m_device;

    // Uniform data
    GLint m_projectionMatrix = -1;
    GLint m_viewMatrix = -1;
    GLint m_modelMatrix = -1;
    GLint m_alphaScissor = -1;

    // Shader program
    GLuint m_program = 0;

    // Framebuffer 
    GLuint m_framebuffer = 0;
    int m_width = 0;
    int m_height = 0;
};

} // namespace Gfx
