#pragma once

#include "content/loader.hpp"

namespace gorc {

    class colormap_loader : public loader {
    public:
        static fourcc const type;

        virtual std::unique_ptr<asset> deserialize(input_stream &is,
                                                   content_manager &,
                                                   asset_id,
                                                   service_registry const &,
                                                   std::string const &name) const override;

        std::vector<path> const &get_prefixes() const override;
        maybe<char const *> get_default() const override;
    };
}
