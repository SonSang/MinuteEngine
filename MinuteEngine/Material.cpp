/*
 *******************************************************************************************
 * Author	: Sang Hyun Son
 * Email	: shh1295@gmail.com
 * Github	: github.com/SonSang
 *******************************************************************************************
 */

#include "Material.h"
#include "glm/gtc/random.hpp"

namespace ME {
    Material Material::create(const Color& e, const Color& a, const Color& d, const Color& s, float sh) {
        Material mat;
        mat.emission = e;
        mat.ambient = a;
        mat.diffuse = d;
        mat.specular = s;
        mat.shininess = sh;
        return mat;
    }

    void Material::setEmission(const Color& c) {
        this->emission = c;
    }
    void Material::setAmbient(const Color& c) {
        this->ambient = c;
    }
    void Material::setDiffuse(const Color& c) {
        this->diffuse = c;
    }
    void Material::setSpecular(const Color& c) {
        this->specular = c;
    }
    void Material::setShininess(float s) {
        this->shininess = s;
    }

    Color Material::getEmission() const noexcept {
        return emission;
    }
    Color Material::getAmbient() const noexcept {
        return ambient;
    }
    Color Material::getDiffuse() const noexcept {
        return diffuse;
    }
    Color Material::getSpecular() const noexcept {
        return specular;
    }
    float Material::getShininess() const noexcept {
        return shininess;
    }

    const static Color defaultEmission = Color::create(0.0f, 0.0f, 0.0f);
    const static Color defaultAmbient = Color::create(0.2f, 0.2f, 0.2f);
    const static Color defaultSpecular = Color::create(1.0f, 1.0f, 1.0f);
    const static float defaultShininess = 10.0f;

    Material Material::white() noexcept {
        return create(defaultEmission, defaultAmbient, Color::create(1.0f, 1.0f, 1.0f), defaultSpecular, defaultShininess);
    }
    Material Material::black() noexcept {
        return create(defaultEmission, defaultAmbient, Color::create(0.0f, 0.0f, 0.0f), defaultSpecular, defaultShininess);
    }
    Material Material::red() noexcept {
        return create(defaultEmission, defaultAmbient, Color::create(1.0f, 0.0f, 0.0f), defaultSpecular, defaultShininess);
    }
    Material Material::green() noexcept {
        return create(defaultEmission, defaultAmbient, Color::create(0.0f, 1.0f, 0.0f), defaultSpecular, defaultShininess);
    }
    Material Material::blue() noexcept {
        return create(defaultEmission, defaultAmbient, Color::create(0.0f, 0.0f, 1.0f), defaultSpecular, defaultShininess);
    }
    Material Material::random() noexcept {
        Color diffuse = Color::create(glm::linearRand(0.0f, 1.0f), glm::linearRand(0.0f, 1.0f), glm::linearRand(0.0f, 1.0f));
        return create(defaultEmission, defaultAmbient, diffuse, defaultSpecular, defaultShininess);
    }
}