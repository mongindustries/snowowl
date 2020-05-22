
struct game_app: app {

std::optional<std::reference_wrapper<window>> main_window;
std::optional<sample_loop> loop;

void on_create();
void on_destroy();
}

void game_app::on_create() {

}

void game_app::on_destroy() {

}
