#ifndef COLOR_H
#define COLOR_H

#include <nlohmann/json.hpp>



struct color
{
    float r, g, b, a;
    
    color(const float r, const float g, const float b, const float a) : r(r), g(g), b(b), a(a) {}

    explicit color(ImVec4 c) : r(c.x), g(c.y), b(c.z), a(c.w) {}

    static color red()
    {
        return {1.0f, 0.0f, 0.0f, 1.0f};
    }

    static color red(float a)
    {
        return {1.0f, 0.0f, 0.0f, a};
    }

    static color black()
    {
        return color(0.0f, 0.0f, 0.0f, 1.0f);
    }

    static color black(float a)
    {
        return color(0.0f, 0.0f, 0.0f, a);
    }

    static color white()
    {
        return color(1.0f, 1.0f, 1.0f, 1.0f);
    }

    static color white(float a)
    {
        return {1.0f, 1.0f, 1.0f, a};
    }



    // primary colors
    static color moonstone(float a = 1.0f)
    {
        return {0.223f, 0.635f, 0.682f, a};
    }

    static color dark_cyan(float a = 1.0f)
    {
        return {0.196f, 0.564f, 0.603f, a};
    }

    static color jet(float a = 1.0f)
    {
        return {0.1764706, 0.1764706, 0.16470589, a};
    }

    static color ebony(float a = 1.0f)
    {
        return {0.3254902, 0.34117648, 0.29803923, a};
    }

    static color kelp(float a = 1.0f)
    {
        return{0.24313726, 0.24705882, 0.22352941, a};
    }

    static color black_olive(float a = 1.0f)
    {
        return{0.21960784, 0.25882354, 0.23137255, a};
    }

    static color feldgrau(float a = 1.0f)
    {
        return {0.24705882, 0.36862746, 0.3529412, a};
    }



    
    /**
     * 
     * @return r: 0.0f, g: 0.5f, b: 0.0f, a: 1.0f
     */
    static color green()
    {
        return {0.0f, 0.5f, 0.0f, 1.0f};
    }

    /**
     * @return r: 0.0f, g: 0.5f, b: 0.0f, a: <b>a</b>
     */
    static color green(float a)
    {
        return {0.0f, 0.5f, 0.0f, a};
    }

    static color persian_red()
    {
        return {0.8f, 0.2f, 0.2f, 1.0f};
    }

    static color persian_red(float a)
    {
        return {0.8f, 0.2f, 0.2f, a};
    }

    static color transparent()
    {
        return {0.0f, 0.0f, 0.0f, 0.0f};
    }
    
    
    operator ImVec4() const
    {
        return {r, g, b, a};
    }

    nlohmann::json serialize() const
    {
        nlohmann::json json_data;
        json_data["r"] = r;
        json_data["g"] = g;
        json_data["b"] = b;
        json_data["a"] = a;
        return json_data;
    }

    static color deserialize(const nlohmann::json &json_data)
    {
        return {json_data["r"], json_data["g"], json_data["b"], json_data["a"]};
    }
    
};



#endif //COLOR_H
