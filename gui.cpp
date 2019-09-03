#include <nanogui/screen.h>
#include <nanogui/window.h>
#include <nanogui/layout.h>
#include <nanogui/label.h>
#include <nanogui/button.h>
#include <nanogui/colorpicker.h>
#include <iostream>
#include <type_traits>
#include <string>
#include <cstdlib>
#include <cstdint>



class ExampleApplication : public nanogui::Screen{
    public:
        ExampleApplication() : nanogui::Screen(Eigen::Vector2i(500, 500), "Color Button"){
            using namespace nanogui;

            Screen* win = this;
            int rgb[3];

            //create window
            Window *window = new Window(this, "Windwow Color");
            window->setPosition(Vector2i(0,0));
            GridLayout *layout = new GridLayout(Orientation::Horizontal, 2, Alignment::Middle, 15, 5);
            layout->setColAlignment({Alignment::Maximum, Alignment::Fill});
            layout->setSpacing(0, 10);
            window->setLayout(layout);

            //create color picker widget
            new Label(window, "Background Color:", "sans-bold");
            srand(time(NULL));
            rgb[0] = rand() % 256;
            rgb[1] = rand() % 256;
            rgb[2] = rand() % 256;
            auto cp = new ColorPicker(window, {rgb[0], rgb[1], rgb[2], 255});
            cp->setFixedSize({100, 20});
            cp->setFinalCallback([](const Color &c){
                std::cout <<  "ColorPicker Final Callback: ["
                          << c.r() << ", "
                          << c.g() << ", "
                          << c.b() << "]" << std::endl;
            });

            //create style button widget
            new Label(window, "Random Color:", "sans-bold");
            Button *b = window->add<Button>("Random Color");

            //set colors
            win->setBackground(nanogui::Color(rgb[0], rgb[1], rgb[2], 255));
            b->setBackgroundColor(nanogui::Color(rgb[0], rgb[1], rgb[2], 255));
            
            //create callbacks for both
            b->setCallback(
                [win, b, cp]{
                    const nanogui::Color col(rand()%256, rand()%256, rand()%256, 255);
                    win->setBackground(col);
                    b->setBackgroundColor(col);
                    cp->setColor(col);
                    std::cout <<  "New Screen Color: ["
                              << col.r() << ", "
                              << col.g() << ", "
                              << col.b() << "]" << std::endl;

                }
            );
            cp->setFinalCallback(
                [win, b, cp](const Color &c){
                    win->setBackground(c);
                    b->setBackgroundColor(c);
                    cp->setColor(c);
                    std::cout <<  "New Screen Color: ["
                              << c.r() << ", "
                              << c.g() << ", "
                              << c.b() << "]" << std::endl;
                }
            );

            performLayout();
            
        }
};

int main(){
    try{
        nanogui::init();
        {
            nanogui::ref<ExampleApplication> app = new ExampleApplication();
            app->drawAll();
            app->setVisible(true);
            nanogui::mainloop();
        }
        nanogui::shutdown();
    } catch (const std::runtime_error &e){
        std::string error_msg = std::string("Caught a fatal error: ") + std::string(e.what());
        return -1;
    }
    return 0;
}