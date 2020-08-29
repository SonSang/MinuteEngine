/*
 *******************************************************************************************
 * Author	: Sang Hyun Son
 * Email	: shh1295@gmail.com
 * Github	: github.com/SonSang
 *******************************************************************************************
 */

#ifndef __ME_MATERIAL_H__
#define __ME_MATERIAL_H__

#ifdef _MSC_VER
#pragma once
#endif

#include "Color.h"

namespace ME {
    class Material {
    private:
        Color emission;
        Color ambient;
        Color diffuse;
        Color specular;
        float shininess;
    public:
        static Material create(const Color& e, const Color& a, const Color& d, const Color& s, float sh);
        void setEmission(const Color& c);
        void setAmbient(const Color& c);
        void setDiffuse(const Color& c);
        void setSpecular(const Color& c);
        void setShininess(float s);

        Color getEmission() const noexcept;
        Color getAmbient() const noexcept;
        Color getDiffuse() const noexcept;
        Color getSpecular() const noexcept;
        float getShininess() const noexcept;

        static Material white() noexcept;
        static Material black() noexcept;
        static Material red() noexcept;
        static Material green() noexcept;
        static Material blue() noexcept;
        static Material random() noexcept;

    };
}

#endif