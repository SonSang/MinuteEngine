//
// *******************************************************************************************
// Author	: Sang Hyun Son 
// Email	: shh1295@gmail.com
// Github	: github.com/SonSang
// *******************************************************************************************
//

#version 330 core

// Final output
out vec4 FragColor;

// Material structure
struct Material {
    vec3 emission;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

uniform vec3 color;
uniform vec3 edgeColor;     // Only needed for geometric entities with face.
uniform bool polygonMode;   // If true, render face (GL_FILL). Else, render line (GL_LINE).
uniform bool textureMode;   // If true, use texture to render instead of material.
uniform bool phongMode;     // If true, use Phong shading. Else, use simple shading. 
uniform Material    material;   // Used for Phong shading.

// ==================================================== Lights =============================================== //
// Light structure
struct Light {
    int type;   // 0 : Directional Light
                // 1 : Point Light
    vec3 position;
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    bool valid;
};
uniform int         lightNum;
uniform Light       light[16];      // Used for Phong shading
// ==================================================== Shadows ============================================== //
// Shadow structure
struct Shadow {
    sampler2D data;                 // Shadow map
    mat4 shadowMat;                 // Transform that takes world coords to depth map's local coords
    bool valid;
};
uniform int         shadowNum;
uniform Shadow      shadow[16];     // Used for shadow mapping ( for each light )
// ==================================================== Textures ============================================= //
in vec3 oTexCoord;

struct Texture2D {
    sampler2D data;
    bool valid;
};

uniform Texture2D diffuseMap;
uniform Texture2D specularMap;
uniform Texture2D normalMap;
uniform Texture2D parallaxMap;
// =========================================================================================================== //

// Matrix
in mat4 oModelMat;
in mat4 oViewMat;
in mat4 oModelViewMat;

// Position - Normal
in vec3 oPosition;
in vec3 oNormal;

// Tangent space
in vec3 oTangent;
in vec3 oBitangent;

// Global variables : These variables are assumed to be ready before any subroutine is called
vec3 eyePosition;
vec3 eyeNormal;

mat3 TBN;       // Tangent - Bitangent - Normal matrix that takes a vector in normal map local space to world space
mat3 eyeTBN;    // TBN matrix that takes a vector in normal map local space to view space

/* ================================== Phong shading lighting =================================== */
vec3 phongLightDiffuse(vec3 diffuse, vec3 normal, Light pl);
vec3 phongLightSpecular(vec3 specular, float shininess, vec3 normal, Light pl);
/* ============================================================================================= */

/* ================================== Shadow =================================================== */
float inShadowFactor(Shadow shadow);    // 1.0 (Fully in shadow) - 0.0 (Fully out shadow)
/* ============================================================================================= */

/* ================================== Parallax mapping ========================================= */
struct PMOption {
    int mode;   // 0 : Single
                // 1 : Multiple
                // 2 : Occlusion
    float depthScale;
    int minLayers;
    int maxLayers;
};
uniform PMOption pmOption;
vec3 pmSingle();        // Use single layer sampling to determine new texture coordinates
vec3 pmMultiple();      // Use multi layer sampling to determine new texture coordinates
vec3 pmOcclusion();     // Use multi layer sampling and linear interpolation to determine new texture coordinates
/* ============================================================================================= */

/* ================================== Environment mapping ========================================= */
uniform samplerCube envMap;
uniform vec3 cameraPosition;
uniform bool emMode;
uniform float emFactor;
/* ============================================================================================= */

vec3 renderByTexture() {
    vec3 ret = vec3(0.0, 0.0, 0.0);
    
    if (phongMode) {
        vec3 normal;
        vec3 texCoord;
        if (parallaxMap.valid) {
            if (pmOption.mode == 0)
                texCoord = pmSingle();
            else if (pmOption.mode == 1)
                texCoord = pmMultiple();
            else
                texCoord = pmOcclusion();
            while (texCoord.x > 1.0)
                texCoord.x -= 1.0;
            while (texCoord.x < 0.0)
                texCoord.x += 1.0;
            while (texCoord.y > 1.0)
                texCoord.y -= 1.0;
            while (texCoord.y < 0.0)
                texCoord.y += 1.0;
            /*
            if (texCoord.x > 1.0 || texCoord.y > 1.0 || texCoord.x < 0.0 || texCoord.y < 0.0)
                discard;
            */
        }
        else 
            texCoord = oTexCoord;
        if (normalMap.valid) {
            // (Normal texture's) Local space normal
            normal = texture2D(normalMap.data, texCoord.st).rgb;
            normal = normalize(normal * 2.0 - 1.0);     // Domain change from [0, 1] to [-1, 1]

            // View space normal
            normal = eyeTBN * normal;
        }
        else
            normal = eyeNormal;

        float shadowFactor = 0.0;
        for(int i = 0; i < shadowNum; i++) {
            if(shadow[i].valid) {
                float tmpShadowFactor = inShadowFactor(shadow[i]);
                if(tmpShadowFactor > shadowFactor)
                    shadowFactor = tmpShadowFactor;
            }
        }   

        if (diffuseMap.valid) {
            vec3 diffuse = vec3(0.0, 0.0, 0.0);
            vec3 diffusePixel = texture2D(diffuseMap.data, texCoord.st).rgb;
            for (int i = 0; i < lightNum; i++)
                diffuse += phongLightDiffuse(diffusePixel, normal, light[i]);
            ret = diffuse * (1.0 - shadowFactor);
        }
        if (specularMap.valid) {
            vec3 specular = vec3(0.0, 0.0, 0.0);
            vec3 specularPixel = texture2D(specularMap.data, texCoord.st).rgb;
            for (int i = 0; i < lightNum; i++)
                specular += phongLightSpecular(specularPixel, 1000.0, normal, light[i]);
            ret += specular * (1.0 - shadowFactor);
        }

        if(emMode) {
            vec3 wPosition = (oModelMat * vec4(oPosition, 1.0)).xyz;
            vec3 wNormal = (oModelMat * vec4(oNormal, 0.0)).xyz;
            vec3 I = normalize(wPosition - cameraPosition);
            vec3 R = reflect(I, normalize(wNormal));
            vec3 emColor = texture(envMap, R).rgb;
            ret += emColor * emFactor;
        }
    }
    else {
        if (diffuseMap.valid) 
            ret = texture2D(diffuseMap.data, oTexCoord.st).rgb;
    }
    return ret;
}

vec3 renderByMaterial() {
    // Emission
    vec3 emission = material.emission;

    // Ambient
    vec3 ambient = vec3(0.0, 0.0, 0.0);
    for (int i = 0; i < lightNum; i++)
        ambient += material.ambient.rgb * light[i].ambient.rgb;

    float shadowFactor = 0.0;
    for(int i = 0; i < shadowNum; i++) {
        if(shadow[i].valid) {
            float tmpShadowFactor = inShadowFactor(shadow[i]);
            if(tmpShadowFactor > shadowFactor)
                shadowFactor = tmpShadowFactor;
        }
    }   

    // Environment
    vec3 environment;
    if(emMode) {
        vec3 wPosition = (oModelMat * vec4(oPosition, 1.0)).xyz;
        vec3 wNormal = (oModelMat * vec4(oNormal, 0.0)).xyz;
        vec3 I = normalize(wPosition - cameraPosition);
        vec3 R = reflect(I, normalize(wNormal));
        vec3 emColor = texture(envMap, R).rgb;
        environment = emColor * emFactor;
    }

    // Diffuse.
    vec3 normal = eyeNormal;
    vec3 diffuse = vec3(0.0, 0.0, 0.0);
    for (int i = 0; i < lightNum; i++)
        diffuse += phongLightDiffuse(material.diffuse + environment, normal, light[i]);
    diffuse = diffuse * (1.0 - shadowFactor);

    // Specular.
    vec3 specular = vec3(0.0, 0.0, 0.0);
    for (int i = 0; i < lightNum; i++)
        specular += phongLightSpecular(material.specular, material.shininess, normal, light[i]);
    specular = specular * (1.0 - shadowFactor);

    return emission + ambient + diffuse + specular;
}

// Compute TBN matrix ( with Gram - Schmidt diagonalization )
void computeTBN() {
    // World TBN
    vec3 T = normalize(vec3(oModelMat * vec4(oTangent.xyz, 0.0)));
    vec3 N = normalize(vec3(oModelMat * vec4(oNormal.xyz, 0.0)));
    T = normalize(T - dot(T, N) * N);
    vec3 B = cross(N, T);

    TBN = mat3(T, B, N);

    // Eye TBN
    T = normalize((oViewMat * vec4(T, 0.0)).xyz);
    B = normalize((oViewMat * vec4(B, 0.0)).xyz);
    N = normalize((oViewMat * vec4(N, 0.0)).xyz);

    eyeTBN = mat3(T, B, N);
}

void main(void) {
    eyePosition = (oModelViewMat * vec4(oPosition, 1.0)).xyz;
    eyeNormal = normalize((oModelViewMat * vec4(oNormal.xyz, 0.0)).xyz);
    computeTBN();

    if(polygonMode) {
        if (textureMode)
            FragColor = vec4(renderByTexture(), 1.0);
        else {
            if (phongMode)
                FragColor = vec4(renderByMaterial(), 1.0);
            else
                FragColor = vec4(color, 1.0);
        }        
    }
    else
        FragColor = vec4(edgeColor, 1.0);
}

/* ================================== Phong shading by light =================================== */
vec3 phongLightDiffuse(vec3 diffuse, vec3 normal, Light light) {
    vec3 lightDir;
    if (light.type == 0) {
        // Directional Light
        lightDir = -normalize(oViewMat * vec4(light.direction, 0.0)).xyz;
    }
    else {
        // Point Light
        vec3 lightPos = (oViewMat * vec4(light.position, 1.0)).xyz;
        lightDir = normalize(lightPos - eyePosition);
    }

    float diffuseFactor = dot(normal, lightDir);
//    if (diffuseFactor < 0.0)
//        diffuseFactor = -diffuseFactor;         // Back face rendering
    return diffuseFactor * (light.diffuse.rgb * diffuse.rgb);
}
vec3 phongLightSpecular(vec3 specular, float shininess, vec3 normal, Light light) {
    vec3 lightDir;
    if (light.type == 0) {
        // Directional Light
        lightDir = -normalize(oViewMat * vec4(light.direction, 0.0)).xyz;
    }
    else {
        // Point Light
        vec3 lightPos = (oViewMat * vec4(light.position, 1.0)).xyz;
        lightDir = normalize(lightPos - eyePosition);
    }

    vec3 viewDir = normalize(-eyePosition);    // In eye space, camera is at (0, 0, 0);
    vec3 reflectDir = reflect(-lightDir, normal);
    float specularFactor = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    return specularFactor * (light.specular.rgb * specular);
}
/* ============================================================================================= */

/* ================================== Shadow =================================================== */
float inShadowFactor(Shadow shadow) {
    if(!shadow.valid)
        return 0.0;

    vec4 shadowMapCoords4 = shadow.shadowMat * oModelMat * vec4(oPosition, 1.0);
    vec3 shadowMapCoords = shadowMapCoords4.xyz / shadowMapCoords4.w;
    shadowMapCoords = shadowMapCoords * 0.5 + 0.5;

    //float shadowMapDepth = texture2D(shadow.data, shadowMapCoords.xy).r;
    float currentDepth = shadowMapCoords.z;

    float factor = 0.0;
    float bias = 0.005;

    vec2 texelSize = 1.0 / vec2(1000.0, 1000.0);

    int sampleRes = 1;
    float sampleNum = 0.0;
    for(int x = -sampleRes; x <= sampleRes; x++) {
        for(int y = -sampleRes; y <= sampleRes; y++) {
            vec2 tmpShadowMapCoords = shadowMapCoords.xy + vec2(x, y) * texelSize;
            if(tmpShadowMapCoords.x < 0.0 || 
                tmpShadowMapCoords.x > 1.0 ||
                tmpShadowMapCoords.y < 0.0 || 
                tmpShadowMapCoords.y > 1.0)
                continue;
            float shadowMapDepth = texture2D(shadow.data, shadowMapCoords.xy + vec2(x, y) * texelSize).r;
            if(currentDepth - bias > shadowMapDepth)
                factor += 1.0;
            sampleNum += 1.0;
        }
    }
    factor /= (sampleNum * 1.5);
    return factor;
}
/* ============================================================================================= */

/* ================================== Parallax mapping ========================================= */
vec3 pmSingle() {
    vec3 tangentSpaceViewDir;
    mat3 invEyeTBN;
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            invEyeTBN[i][j] = eyeTBN[j][i];

    vec3 eyeSpaceViewDir = -eyePosition;
    tangentSpaceViewDir = normalize(invEyeTBN * eyeSpaceViewDir);

    float depth = 1.0 - texture2D(parallaxMap.data, oTexCoord.st).r;
    vec2 p = tangentSpaceViewDir.xy * (depth * pmOption.depthScale) / tangentSpaceViewDir.z;
    return oTexCoord - vec3(p.xy, 0.0);
}
vec3 pmMultiple() {
    vec3 tangentSpaceViewDir;
    mat3 invEyeTBN;
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            invEyeTBN[i][j] = eyeTBN[j][i];

    vec3 eyeSpaceViewDir = -eyePosition;
    tangentSpaceViewDir = normalize(invEyeTBN * eyeSpaceViewDir);

    float minLayers = float(pmOption.minLayers);
    float maxLayers = float(pmOption.maxLayers);
    float numLayers = mix(maxLayers, minLayers, max(tangentSpaceViewDir[2], 0.0));
    float layerDepth = 1.0 / numLayers;
    float currentLayerDepth = 0.0;
    vec2 P = tangentSpaceViewDir.xy * pmOption.depthScale;
    vec2 dTexCoord = P / numLayers;

    vec2 cTexCoord = oTexCoord.st;
    float currentDepthMapValue = 1.0 - texture2D(parallaxMap.data, cTexCoord.st).r;
    while (currentDepthMapValue > currentLayerDepth) {
        cTexCoord -= dTexCoord;
        currentDepthMapValue = 1.0 - texture2D(parallaxMap.data, cTexCoord.st).r;
        currentLayerDepth += layerDepth;
    }
    return vec3(cTexCoord.xy, 0.0);
}
vec3 pmOcclusion() {
    vec3 tangentSpaceViewDir;
    mat3 invEyeTBN;
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            invEyeTBN[i][j] = eyeTBN[j][i];

    vec3 eyeSpaceViewDir = -eyePosition;
    tangentSpaceViewDir = normalize(invEyeTBN * eyeSpaceViewDir);

    float minLayers = float(pmOption.minLayers);
    float maxLayers = float(pmOption.maxLayers);
    float numLayers = mix(maxLayers, minLayers, max(tangentSpaceViewDir[2], 0.0));
    float layerDepth = 1.0 / numLayers;
    float currentLayerDepth = 0.0;
    vec2 P = tangentSpaceViewDir.xy * pmOption.depthScale;
    vec2 dTexCoord = P / numLayers;

    vec2 cTexCoord = oTexCoord.st;
    float currentDepthMapValue = 1.0 - texture2D(parallaxMap.data, cTexCoord.st).r;
    while (currentDepthMapValue > currentLayerDepth) {
        cTexCoord -= dTexCoord;
        currentDepthMapValue = 1.0 - texture2D(parallaxMap.data, cTexCoord.st).r;
        currentLayerDepth += layerDepth;
    }
    vec2 prevTexCoord = cTexCoord + dTexCoord;
    float afterDepth = currentDepthMapValue - currentLayerDepth;
    float beforeDepth = texture2D(parallaxMap.data, prevTexCoord).r - currentLayerDepth + layerDepth;

    float weight = afterDepth / (afterDepth - beforeDepth);
    vec2 fTexCoord = prevTexCoord * weight + cTexCoord * (1.0 - weight);

    return vec3(fTexCoord.xy, 0.0);
}
/* ============================================================================================= */