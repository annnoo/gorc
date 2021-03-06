#pragma once

#include "libold/base/application.hpp"
#include "view_layer.hpp"
#include "presenter.hpp"
#include "presenter_mapper.hpp"
#include "clear_screen_view.hpp"
#include "places/action/action_view.hpp"
#include "jk/vfs/jk_virtual_file_system.hpp"
#include "game/level_state.hpp"
#include "utility/service_registry.hpp"
#include "client_renderer_object_factory.hpp"
#include "libold/base/utility/randomizer.hpp"

namespace gorc {
namespace client {

namespace world {
class level_view;
class level_presenter;
}

class application : public gorc::application<view_layer, presenter, presenter_mapper> {
private:
    service_registry const &services;
    presenter_mapper mapper;
    clear_screen_view clear_view;
    std::unique_ptr<virtual_container> episode_ctr;

    maybe<scoped_delegate> print_delegate;

    void register_verbs();

public:
    std::string input_episodename;
    std::string input_levelname;

    jk_virtual_file_system& virtual_filesystem;

    client_renderer_object_factory renderer_object_factory;
    utility::randomizer randomizer;

    std::unique_ptr<world::level_view> level_view;
    std::unique_ptr<action::action_view> action_view;

    game::level_state components;

    application(service_registry const &services);
    ~application();

    virtual void startup(event_bus& eventbus, content_manager& content) override;
    virtual void shutdown() override;

    virtual void update(const gorc::time& time, const box<2, int>& view_size) override;

    virtual void create_options(options&) override;
};

}
}
