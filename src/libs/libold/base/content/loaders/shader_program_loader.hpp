#pragma once

#include "libold/base/content/loader.hpp"
#include <GL/gl.h>

namespace gorc {
namespace content {
namespace loaders {

class shader_program_loader : public loader {
protected:
    GLuint program_type;

    shader_program_loader(GLuint program_type);

public:
    virtual std::unique_ptr<asset> deserialize(input_stream &file,
                                               content_manager &,
                                               service_registry const &) const override;

    virtual std::vector<path> const& get_prefixes() const override;
};

class vertex_program_loader : public shader_program_loader {
public:
    static fourcc const type;

    vertex_program_loader();
};

class fragment_program_loader : public shader_program_loader {
public:
    static fourcc const type;

    fragment_program_loader();
};

}
}
}